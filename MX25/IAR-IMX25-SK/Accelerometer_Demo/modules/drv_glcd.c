/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_glcd.c
 *    Description : Graphical LCD driver
 *
 *    History :
 *    1. Date        : 6, March 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 16 $
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include "board.h"
#include "drv_glcd.h"

#define C_HGLCD_V_SIZE          320
#define C_HGLCD_H_SIZE          240
#define C_GLCD_H_PULSE          2
#define C_GLCD_H_FRONT_PORCH    3
#define C_GLCD_H_BACK_PORCH     4
#define C_GLCD_V_PULSE          3
#define C_GLCD_V_FRONT_PORCH    2
#define C_GLCD_V_BACK_PORCH     15

#define _1MS_LOOP_DLY           24200UL
extern Int32U SDRAM_BASE_ADDR;

//#define LCD_VRAM_BASE_ADDR ((Int32U)&SDRAM_BASE_ADDR)
//#define LCD_CURSOR_BASE_ADDR ((Int32U)0xFFE10800)

static pFontType_t pCurrFont = NULL;
static LdcPixel_t TextColour;
static LdcPixel_t TextBackgndColour;

static Int32U TextX_Pos = 0;
static Int32U TextY_Pos = 0;

static Int32U XL_Win = 0;
static Int32U YU_Win = 0;
static Int32U XR_Win = C_GLCD_H_SIZE-1;
static Int32U YD_Win = C_GLCD_V_SIZE-1;

static Int32U TabSize = TEXT_DEF_TAB_SIZE;

static Int32U WindY_Size, WindX_Size;
static Int32U CurrY_Size, CurrX_Size;
static pInt32U pWind;
static pInt32U pPix;

static Int32U VBuffer[C_GLCD_H_SIZE * C_GLCD_V_SIZE];
static Int32U GRVBuffer[C_GLCD_H_SIZE * C_GLCD_V_SIZE];

#define LCD_VRAM_BASE_ADDR    ((Int32U)VBuffer)
#define LCD_GRVRAM_BASE_ADDR  ((Int32U)GRVBuffer)

static GLCD_Layer_t _Layer;

/*************************************************************************
 * Function Name: GLCD_Cursor_Cfg
 * Parameters: Int32U BlinkSpeed, Int32U H_Size, Int32U V_Size,
 *             Int32U ColMap
 *
 * Return: none
 *
 * Description: Configure the cursor
 *
 *************************************************************************/
void GLCD_Cursor_Cfg(Int32U BlinkSpeed, Int32U H_Size, Int32U V_Size, Int32U ColMap)
{
  LCWHBR_bit.CW = H_Size;
  LCWHBR_bit.CH = V_Size;
  LCWHBR_bit.BD = BlinkSpeed;
  LCWHBR_bit.BK_EN = 0 != BlinkSpeed;
  LCCMR = ColMap;
}

/*************************************************************************
 * Function Name: GLCD_Cursor_Ctlr
 * Parameters: CursorContrl_t CC, Boolean bOP
 *
 * Return: none
 *
 * Description: Cursor control
 *
 *************************************************************************/
void GLCD_Cursor_Ctlr(CursorContrl_t CC, Boolean bOP)
{
  LCPR_bit.CC = CC;
  LCPR_bit.OP = bOP;
}

/*************************************************************************
 * Function Name: GLCD_Move_Cursor
 * Parameters: x - cursor x position
 *             y - cursor y position
 *
 * Return: none
 *
 * Description: Moves cursor on position (x,y). Negativ values are posible.
 *
 *************************************************************************/
void GLCD_Move_Cursor(int x, int y)
{
  LCPR_bit.CXP = y;
  LCPR_bit.CYP = x;
}

/*************************************************************************
 * Function Name: GLCD_Init
 * Parameters: const Int32U *pPain, const Int32U * pPallete
 *
 * Return: none
 *
 * Description: GLCD controller init
 *
 *************************************************************************/
void GLCD_Init (const Int32U *pPain, const Int32U * pPallete)
{
pInt32U pDst = (pInt32U) LCD_VRAM_BASE_ADDR;

  _Layer = GLCD_BACKGROUND_LAYER;

  // Init System (SPI) Interface
  // LCD RST
  IOMUXC_SW_MUX_CTL_PAD_UART2_CTS_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_UART2_CTS_bit.SION = 1;
  GPIO4_DR_bit.DR29 = 1;
  GPIO4_GDIR_bit.GDIR29 = 1;

  // LCD CS
  IOMUXC_SW_MUX_CTL_PAD_CSPI1_SS1_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_CSPI1_SS1_bit.SION = 1;
  GPIO1_DR_bit.DR17 = 1;
  GPIO1_GDIR_bit.GDIR17 = 1;

  // LCD SCL
  IOMUXC_SW_MUX_CTL_PAD_CSPI1_SCLK_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_CSPI1_SCLK_bit.SION = 1;

  // LCD D0
  IOMUXC_SW_MUX_CTL_PAD_CSPI1_MISO_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_CSPI1_MISO_bit.SION = 1;

  // LCD DI
  IOMUXC_SW_MUX_CTL_PAD_CSPI1_MOSI_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_CSPI1_MOSI_bit.SION = 1;

  // Enable a clock
  CGCR1_bit.ipg_clk_cspi1 = 1;
  // Disable SPI module (reset module)
  CSPI1_CONREG_bit.EN   = 0;
  CSPI1_CONREG_bit.MODE = 1;
  CSPI1_CONREG_bit.XCH  = 0;
  CSPI1_CONREG_bit.SMC  = 1;
  CSPI1_CONREG_bit.POL  = 1;
  CSPI1_CONREG_bit.PHA  = 1;

  // CSPI1_CONREG_bit.SSCTL = 0;
  CSPI1_CONREG_bit.SSPOL = 0;
  CSPI1_CONREG_bit.DRCTL = 0;
  CSPI1_CONREG_bit.CS    = 1;
  CSPI1_CONREG_bit.DATA_RATE    = 1; // /4
  CSPI1_CONREG_bit.BURST_LENGTH = 8-1;

  // Enable SPI module
  CSPI1_CONREG_bit.EN = 1;
  CSPI1_STATREG_bit.TC = 1;

  // Power-up LCD
	GPIO4_DR_bit.DR29=0;
	_GLCD_Delay_ms(15);
	GPIO4_DR_bit.DR29=1;
	_GLCD_Delay_ms(20);

	//************* Start Initial Sequence **********//
  _GLCD_WriteCom(0x00E3);_GLCD_WriteData(0x3008);	// Set internal timing
  _GLCD_WriteCom(0x00E7);_GLCD_WriteData(0x0012);	// Set internal timing
	_GLCD_WriteCom(0x00EF);_GLCD_WriteData(0x1231);	// Set internal timing
	_GLCD_WriteCom(0x0001);_GLCD_WriteData(0x0100); // set SS and SM bit
	_GLCD_WriteCom(0x0002);_GLCD_WriteData(0x0700); // set 1 line inversion
	_GLCD_WriteCom(0x0003);_GLCD_WriteData(0x1020);	// set GRAM write direction and BGR=1.
	_GLCD_WriteCom(0x0004);_GLCD_WriteData(0x0000);	// Resize register
	_GLCD_WriteCom(0x0008);_GLCD_WriteData((C_GLCD_V_FRONT_PORCH << 8) || C_GLCD_V_BACK_PORCH);	// set the back porch and front porch
	_GLCD_WriteCom(0x0009);_GLCD_WriteData(0x0000);	// set non-display area refresh cycle ISC[3:0]
	_GLCD_WriteCom(0x000A);_GLCD_WriteData(0x0000);	// FMARK function
	_GLCD_WriteCom(0x000C);_GLCD_WriteData(0x0110); // RGB interface setting
  _GLCD_WriteCom(0x000D);_GLCD_WriteData(0x0000);	// Frame marker Position
	_GLCD_WriteCom(0x000f);_GLCD_WriteData(0x0000);	// RGB interface polarity
//*************Power On sequence ****************//
	_GLCD_WriteCom(0x0010);_GLCD_WriteData(0x0000);
  _GLCD_WriteCom(0x0011);_GLCD_WriteData(0x0007);
	_GLCD_WriteCom(0x0012);_GLCD_WriteData(0x0000);
	_GLCD_WriteCom(0x0013);_GLCD_WriteData(0x0000);
	_GLCD_Delay_ms(200);
	_GLCD_WriteCom(0x0010);_GLCD_WriteData(0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
  _GLCD_WriteCom(0x0011);_GLCD_WriteData(0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
  _GLCD_Delay_ms(50);
	_GLCD_WriteCom(0x0012);_GLCD_WriteData(0x001B); // Internal reference voltage= Vci;
  _GLCD_Delay_ms(50);
	_GLCD_WriteCom(0x0013);_GLCD_WriteData(0x1600);	// Set VDV[4:0] for VCOM amplitude
	_GLCD_WriteCom(0x0029);_GLCD_WriteData(0x0018);	// Set VCM[5:0] for VCOMH
  _GLCD_WriteCom(0x002B);_GLCD_WriteData(0x000C);	// Set Frame Rate
  _GLCD_Delay_ms(50);
	_GLCD_WriteCom(0x0020);_GLCD_WriteData(0x0000);
	_GLCD_WriteCom(0x0021);_GLCD_WriteData(0x0000);
// ----------- Adjust the Gamma Curve ----------//
	_GLCD_WriteCom(0x0030);_GLCD_WriteData(0x0000);
	_GLCD_WriteCom(0x0031);_GLCD_WriteData(0x0404);
	_GLCD_WriteCom(0x0032);_GLCD_WriteData(0x0304);
	_GLCD_WriteCom(0x0035);_GLCD_WriteData(0x0005);
	_GLCD_WriteCom(0x0036);_GLCD_WriteData(0x1604);
	_GLCD_WriteCom(0x0037);_GLCD_WriteData(0x0304);
	_GLCD_WriteCom(0x0038);_GLCD_WriteData(0x0303);
	_GLCD_WriteCom(0x0039);_GLCD_WriteData(0x0707);
	_GLCD_WriteCom(0x003C);_GLCD_WriteData(0x0500);
	_GLCD_WriteCom(0x003D);_GLCD_WriteData(0x000F);
//------------------ Set GRAM area ---------------//
	_GLCD_WriteCom(0x0050);_GLCD_WriteData(0x0000);	// Horizontal GRAM Start Address
	_GLCD_WriteCom(0x0051);_GLCD_WriteData(0x00EF);	// Horizontal GRAM End Address
	_GLCD_WriteCom(0x0052);_GLCD_WriteData(0x0000);	// Vertical GRAM Start Address
	_GLCD_WriteCom(0x0053);_GLCD_WriteData(0x013F); // Vertical GRAM End Address
	_GLCD_WriteCom(0x0060);_GLCD_WriteData(0xA700);	// Gate Scan Line
	_GLCD_WriteCom(0x0061);_GLCD_WriteData(0x0001);	// NDL,VLE, REV
	_GLCD_WriteCom(0x006A);_GLCD_WriteData(0x0000);	// set scrolling line
//-------------- Partial Display Control ---------//
  _GLCD_WriteCom(0x0080);_GLCD_WriteData(0x0000);
  _GLCD_WriteCom(0x0081);_GLCD_WriteData(0x0000);
  _GLCD_WriteCom(0x0082);_GLCD_WriteData(0x0000);
  _GLCD_WriteCom(0x0083);_GLCD_WriteData(0x0000);
  _GLCD_WriteCom(0x0084);_GLCD_WriteData(0x0000);
  _GLCD_WriteCom(0x0085);_GLCD_WriteData(0x0000);
//-------------- Panel Control -------------------//

	_GLCD_WriteCom(0x0090);_GLCD_WriteData(0x0010);
	_GLCD_WriteCom(0x0092);_GLCD_WriteData(0x6000);
  _GLCD_WriteCom(0x0095);_GLCD_WriteData(0x6000); // DIVE (division ratio) x RTNE (DOTCLKs) . DOTCLKs in 1H period.

  // Init RGB Interface
  // Enable a clock
  CGCR0_bit.hclk_lcdc = 1;
  CGCR0_bit.ipg_per_lcdc = 1;
  CGCR1_bit.ipg_clk_lcdc = 0;

  LSSAR = (Int32U)VBuffer;
  _LSR_bit.XMAX = (C_HGLCD_H_SIZE / 16);   // Holds screen X Axis
  _LSR_bit.YMAX = C_HGLCD_V_SIZE;          // Specifies the Height of
                                          // the LCD panel
  LVPWR = C_HGLCD_H_SIZE;

  // Cursors Controls
  LCPR_bit.CC = 0;    // Cursor control
  LCPR_bit.OP = 0;    // Arithmetic operator Control
  LCPR_bit.CXP = 0;   // Cursor X position
  LCPR_bit.CYP = 0;   // Cursor Y Position

  LCCMR = 0;

  // Panel Control register
  LPCR_bit.TFT      = 1;  // TFT enable
  LPCR_bit.COLOR    = 1;  // Color enable (set for TFT)
  LPCR_bit.PBSIZ    = 0;  // Panel Bus Width, 0=1 bit,1=2,2=4,3=8 (does not matter for TFT)
  LPCR_bit.BPIX     = 6;  // BPP, 0=1bpp,1=2,2=4,3=8,4=12,5=16,6=18
  LPCR_bit.PIXPOL   = 0;  // Pixel Polarity, 0 = Active High, 1 = Active Low
  LPCR_bit.FLMPOL   = 1;  // First Line Marker Polarity, "
  LPCR_bit.LPPOL    = 1;  // Line Pulse Polarity, "
  LPCR_bit.CLKPOL   = 0;  // LCD Shift Clock, 0 = Active Falling, 1 = Active Rising (reverse for TFT)
  LPCR_bit.OEPOL    = 1;  // Output Enable Polarity, 0= Active High, 1 = Active Low
  LPCR_bit.SCLKIDLE = 1;  // LSCLK Idle Enable (when VSYNC is idle in TFT mode)
  LPCR_bit.END_SEL  = 0;  // Endian Select, 0 = Little, 1 = Big
  LPCR_bit.SWAP_SEL = 1;  // Swap Select, 0 = 16bpp, 1 = else
  LPCR_bit.REV_VS   = 0;  // Reverse Vertical Scan
  LPCR_bit.ACD_SEL  = 0;  // ACD Clock Source select
  LPCR_bit.ACD      = 0;  // ACD Stuff
  LPCR_bit.SCLKSEL  = 1;  // LSCLK Select, 0 = Disable OE & LSCLK when no data output in TFT mode for power savings, 1 = Always on
  LPCR_bit.SHARP    = 0;  // Sharp Panel Signals Enable
  LPCR_bit.PCD      = 7;  // Pixel CLK Divider, actual divider is PCD value + 1

  LGWSAR            = LCD_GRVRAM_BASE_ADDR;
  LGWSR_bit.GWW     = (C_HGLCD_H_SIZE / 16);   // Holds screen X Axis
  LGWSR_bit.GWH     = C_HGLCD_V_SIZE;          // Specifies the Height of graphic window
  LGWVPWR_bit.GWVPW = C_HGLCD_H_SIZE;
  LGWPOR            = 0;
  LGWPR_bit.GWXP    = 0;
  LGWPR_bit.GWYP    = 0;
  LGWCR_bit.GWCKB   = 0x3F;     // Graphic window color keying blue component
  LGWCR_bit.GWCKG   = 0x3F;     // Graphic window color keying green component
  LGWCR_bit.GWCKR   = 0x3F;     // Graphic window color keying red component
  LGWCR_bit.GW_RVS  = 0;        // Vertical scan in normal direction
  LGWCR_bit.GWE     = 0;        // Disable graphic window on screen
  LGWCR_bit.GWCKE   = 0;        // Disable color keying of graphic window
  LGWCR_bit.GWAV    = 0;        // Graphic window totally transparent
  LGWDCR_bit.GWBT   = 0;        // Burst length is dynamic

  // Horizontal configuration register
  LHCR_bit.H_WAIT_2 = C_GLCD_H_BACK_PORCH-3;  // H_WAIT2 + 3 = delay between HSYNC & first data of next line
  LHCR_bit.H_WAIT_1 = C_GLCD_H_FRONT_PORCH-1; // H_WAIT1 + 1 = delay in Pixel CLK periods between OE & HSYNC
  LHCR_bit.H_WIDTH  = C_GLCD_H_PULSE;         // H_WIDTH + 1 = width of Horizontal Sync Pulse in Pixel CLK periods

  // Vertical configuration register
  LVCR_bit.V_WIDTH  = C_GLCD_V_PULSE;         // V_WIDTH = Vertical Sync pulse width in HSYNC periods
  LVCR_bit.V_WAIT_1 = C_GLCD_V_BACK_PORCH;    // V_WAIT1 = delay between OE and VSYNC (in TFT)
  LVCR_bit.V_WAIT_2 = C_GLCD_V_FRONT_PORCH;   // V_WAIT2 = delay between VSYNC and OE of the first line of next frame

  // Panning offset register
  LPOR = 0;

  // Sharp configuration register
  LSCR = 0;

  // PWM contrast control register
  LPCCR = 0;

  // Refresh Mode Control Register disable self refresh
  LRMCR_bit.SELF_REF = 0;

  // DMA control register
  // Burst Length, 0 = Dynamic, 1 = Fixed
  LDCR_bit.BURST = 0;
  // DMA High Mark, for fixed this is
  // the length of the burst,
  LDCR_bit.HM = 4;

  // for dynamic the buffer is filled until the
  // number of empty words is equal to the High Mark
  // minus two DMA Trigger Mark, when the number of
  // words of data left in the buffer reach is this,
  // new data is burst to fill it
  LDCR_bit.TM = 8;

  // Enable
  IOMUXC_SW_MUX_CTL_PAD_OE_ACD_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_OE_ACD_bit.SION = 1;
  // PCLK
  IOMUXC_SW_MUX_CTL_PAD_LSCLK_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LSCLK_bit.SION = 1;
  // HSYNH
  IOMUXC_SW_MUX_CTL_PAD_HSYNC_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_HSYNC_bit.SION = 1;
  // VSYNH
  IOMUXC_SW_MUX_CTL_PAD_VSYNC_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_VSYNC_bit.SION = 1;
  // LD
  IOMUXC_SW_MUX_CTL_PAD_LD0_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD1_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD2_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD3_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD4_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD5_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD6_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD7_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD8_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD9_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD10_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD11_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD12_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD13_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD14_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_LD15_bit.MUX_MODE = ALT0_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_D15_bit.MUX_MODE = ALT1_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_D14_bit.MUX_MODE = ALT1_MUX_MODE;

  CGCR1_bit.ipg_clk_lcdc = 1;

  if(NULL != pPallete)
  {
    GLCD_SetPallet(pPallete);
  }

  if (NULL == pPain)
  {
    // clear display memory
    for(Int32U i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE) > i; i++)
    {
      *pDst++ = 0;
    }
  }
  else
  {
    // set display memory
    for(Int32U i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE) > i; i++)
    {
      *pDst++ = *pPain++;
    }
  }
}

/*************************************************************************
 * Function Name: GLCD_SetPallet
 * Parameters: const Int32U * pPallete
 *
 * Return: none
 *
 * Description: GLCD init colour pallete
 *
 *************************************************************************/
void GLCD_SetPallet (const Int32U * pPallete)
{
pInt32U pDst = (pInt32U)&LBGLUT_BASE;
  assert(pPallete);
  for (Int32U i = 0; i < 256; i++)
  {
    *pDst++ = *pPallete++;
  }
}

/*************************************************************************
 * Function Name: GLCD_SetPalletGraphic
 * Parameters: const Int32U * pPallete
 *
 * Return: none
 *
 * Description: GLCD init graphic colour pallete
 *
 *************************************************************************/
void GLCD_SetPalletGraphic (const Int32U * pPallete)
{
pInt32U pDst = (pInt32U)&LGWLUT_BASE;
  assert(pPallete);
  for (Int32U i = 0; i < 256; i++)
  {
    *pDst++ = *pPallete++;
  }
}

/*************************************************************************
 * Function Name: GLCD_Ctrl
 * Parameters: Boolean bEna
 *
 * Return: none
 *
 * Description: GLCD enable disabe sequence
 *
 *************************************************************************/
void GLCD_Ctrl (Boolean bEna)
{
  if (bEna)
  {
    _GLCD_WriteCom(0x0007);_GLCD_WriteData(0x0133);	// 262K color and display ON
    _GLCD_WriteCom(0x0022);
    _GLCD_Delay_ms(50);
  }
  else
  {
    _GLCD_WriteCom(0x0007);_GLCD_WriteData(0x0000);	// 262K color and display ON
    _GLCD_Delay_ms(50);
  }
}

/*************************************************************************
 * Function Name: GLCD_SetFont
 * Parameters: pFontType_t pFont, LdcPixel_t Color
 *              LdcPixel_t BackgndColor
 *
 * Return: none
 *
 * Description: Set current font, font color and background color
 *
 *************************************************************************/
void GLCD_SetFont(pFontType_t pFont, LdcPixel_t Color, LdcPixel_t BackgndColor)
{
  pCurrFont = pFont;
  TextColour = Color;
  TextBackgndColour = BackgndColor;
}

/*************************************************************************
 * Function Name: GLCD_SetWindow
 * Parameters: Int32U X_Left, Int32U Y_Up,
 *             Int32U X_Right, Int32U Y_Down
 *
 * Return: none
 *
 * Description: Set draw window XY coordinate in pixels
 *
 *************************************************************************/
void GLCD_SetWindow(Int32U X_Left, Int32U Y_Up,
                    Int32U X_Right, Int32U Y_Down)
{
  assert(X_Right < C_GLCD_H_SIZE);
  assert(Y_Down < C_GLCD_V_SIZE);
  assert(X_Left < X_Right);
  assert(Y_Up < Y_Down);
  XL_Win = X_Left;
  YU_Win = Y_Up;
  XR_Win = X_Right;
  YD_Win = Y_Down;
}

/*************************************************************************
 * Function Name: GLCD_TextSetPos
 * Parameters: Int32U X_UpLeft, Int32U Y_UpLeft,
 *             Int32U X_DownLeft, Int32U Y_DownLeft
 *
 * Return: none
 *
 * Description: Set text X,Y coordinate in characters
 *
 *************************************************************************/
void GLCD_TextSetPos(Int32U X, Int32U Y)
{
  TextX_Pos = X;
  TextY_Pos = Y;
}

/*************************************************************************
 * Function Name: GLCD_TextSetTabSize
 * Parameters: Int32U Size
 *
 * Return: none
 *
 * Description: Set text tab size in characters
 *
 *************************************************************************/
void GLCD_TextSetTabSize(Int32U Size)
{
  TabSize = Size;
}

/*************************************************************************
 * Function Name: LCD_SET_WINDOW
 * Parameters: int c
 *
 * Return: none
 *
 * Description: Put char function
 *
 *************************************************************************/
static
void LCD_SET_WINDOW (Int32U X_Left, Int32U X_Right,
                     Int32U Y_Up, Int32U Y_Down)
{
  switch(_Layer)
  {
  case GLCD_BACKGROUND_LAYER:
    pPix = pWind = (pInt32U) LCD_VRAM_BASE_ADDR;
    break;
  case GLCD_GRAPHIC_WINDOW_LAYER:
    pPix = pWind = (pInt32U) LCD_GRVRAM_BASE_ADDR;
    break;
  default:
    assert(0);
  }

  pPix  += Y_Up + (X_Left*C_GLCD_V_SIZE);
  pWind += Y_Up + (X_Left*C_GLCD_V_SIZE);
  WindX_Size = X_Right - X_Left;
  WindY_Size = Y_Down - Y_Up;
  CurrX_Size = CurrY_Size = 0;
}

/*************************************************************************
 * Function Name: LCD_SET_WINDOW
 * Parameters: int c
 *
 * Return: none
 *
 * Description: Put char function
 *
 *************************************************************************/
static
void LCD_WRITE_PIXEL (Int32U Pixel)
{
  *pPix++ = Pixel;
  if (++CurrY_Size > WindY_Size)
  {
    CurrY_Size = 0;
    if(++CurrX_Size > WindX_Size)
    {
      CurrX_Size = 0;
    }
    pPix = pWind + CurrX_Size * C_GLCD_V_SIZE;
  }
}

/*************************************************************************
 * Function Name: GLCD_TextCalcWindow
 * Parameters: pInt32U pXL, pInt32U pXR,
 *             pInt32U pYU, pInt32U pYD,
 *             pInt32U pH_Size, pInt32U pV_Size
 *
 * Return: Boolean
 *          FALSE - out of window coordinate aren't valid
 *          TRUE  - the returned coordinate are valid
 *
 * Description: Calculate character window
 *
 *************************************************************************/
static
Boolean GLCD_TextCalcWindow (pInt32U pXL, pInt32U pXR,
                             pInt32U pYU, pInt32U pYD,
                             pInt32U pH_Size, pInt32U pV_Size)
{
  *pH_Size = pCurrFont->H_Size;
  *pV_Size = pCurrFont->V_Size;
  *pXL = XL_Win + (TextX_Pos*pCurrFont->H_Size);
  if(*pXL > XR_Win)
  {
    return(FALSE);
  }
  *pYU = YU_Win + (TextY_Pos*pCurrFont->V_Size);
  if(*pYU > YD_Win)
  {
    return(FALSE);
  }

  *pXR   = XL_Win + ((TextX_Pos+1)*pCurrFont->H_Size) - 1;
  if(*pXR > XR_Win)
  {
  	*pH_Size -= *pXR - XR_Win;
    *pXR = XR_Win;
  }

  *pYD = YU_Win + ((TextY_Pos+1)*pCurrFont->V_Size) - 1;
  if(*pYD > YD_Win)
  {
    *pV_Size -= *pYD - YD_Win;
    *pYD = YD_Win;
  }

  return(TRUE);
}

/*************************************************************************
 * Function Name: putchar
 * Parameters: int c
 *
 * Return: none
 *
 * Description: Put char function
 *
 *************************************************************************/
int GLCD_putchar (int c)
{
pInt8U pSrc;
Int32U V_Line;
Int32U xl,xr,yu,yd,Temp,V_Size, H_Size, SrcInc = 1;
Int32U WhiteSpaceNumb;
  if(pCurrFont == NULL)
  {
    return(EOF);
  }
  V_Line = (pCurrFont->V_Size / 8) + ((pCurrFont->V_Size % 8)?1:0);
  switch(c)
  {
  case '\n':  // go to begin of next line (NewLine)
    ++TextY_Pos;
    break;
  case '\r':  // go to begin of this line (Carriage Return)
  	// clear from current position to end of line
  	while(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size))
  	{
      LCD_SET_WINDOW(xl,xr,yu,yd);
	    for(Int32U i = 0; i < V_Size; ++i)
	    {
	      for(Int32U j = 0; j < H_Size; ++j)
	      {
	        LCD_WRITE_PIXEL(TextBackgndColour);
	      }
	    }
  		++TextX_Pos;
  	}
    TextX_Pos = 0;
    break;
  case '\b': // go back one position (BackSpace)
    if(TextX_Pos)
    {
      --TextX_Pos;
      // del current position
	  	if(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size))
	  	{
        LCD_SET_WINDOW(xl,xr,yu,yd);
		    for(Int32U i = 0; i < V_Size; ++i)
		    {
		      for(Int32U j = 0; j < H_Size; ++j)
		      {
		        LCD_WRITE_PIXEL(TextBackgndColour);
		      }
		    }
	  	}
    }
    break;
  case '\t':  // go to next Horizontal Tab stop
  	WhiteSpaceNumb = TabSize - (TextX_Pos%TabSize);
  	for(Int32U k = 0; k < WhiteSpaceNumb; ++k)
  	{
      LCD_SET_WINDOW(xl,xr,yu,yd);
	  	if(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size))
	  	{
		    for(Int32U i = 0; i < V_Size; ++i)
		    {
		      for(Int32U j = 0; j < H_Size; ++j)
		      {
		        LCD_WRITE_PIXEL(TextBackgndColour);
		      }
		    }
		    ++TextX_Pos;
	  	}
	  	else
	  	{
	  		break;
	  	}
  	}
    break;
  case '\f':  // go to top of page (Form Feed)
  	// clear entire window
  	H_Size = XR_Win - XL_Win;
  	V_Size = YD_Win - YU_Win;
    // set character window X left, Y right
    LCD_SET_WINDOW(XL_Win,XR_Win,YU_Win,YD_Win);
    // Fill window with background font color
    for(Int32U i = 0; i <= V_Size; ++i)
    {
      for(Int32U j = 0; j <= H_Size; ++j)
      {
        LCD_WRITE_PIXEL(TextBackgndColour);
      }
    }
  	
  	TextX_Pos = TextY_Pos = 0;
    break;
  case '\a':  // signal an alert (BELl)
    TEXT_BEL1_FUNC();
    break;
  default:
    // Calculate the current character base address from stream
    // and the character position
    if((c <  pCurrFont->CharacterOffset) &&
    	 (c >= pCurrFont->CharactersNuber))
   	{
   		c = 0;
    }
    else
    {
    	c -= pCurrFont->CharacterOffset;
    }
    pSrc = pCurrFont->pFontStream + (V_Line * pCurrFont->H_Size * c);
    // Calculate character window and fit it in the text window
    if(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size))
    {
	    // set character window X left, Y right
	    LCD_SET_WINDOW(xl,xr,yu,yd);
	    // Send char data
	    for(Int32U i = 0; i < H_Size; ++i)
	    {
        SrcInc = V_Line;
        for(Int32U j = 0; j < V_Size; ++j)
	      {
	        Temp = (*pSrc & (1UL << (j&0x7)))?TextColour:TextBackgndColour;
	        LCD_WRITE_PIXEL(Temp);
	        if((j&0x7) == 7)
	        {
	          ++pSrc;
            --SrcInc;
	        }
	      }
        // next line of character
	      pSrc += SrcInc;
	    }
    }
    ++TextX_Pos;
  }
  return(c);
}

/*************************************************************************
 * Function Name: GLCD_print
 * Parameters: char *s, const char *fmt, ...
 *
 * Return: none
 *
 * Description: Print formated string on the LCD
 *
 *************************************************************************/
void GLCD_print (const char *fmt, ...)
{
va_list ap;
char s[MAX_GLCD_STR_SIZE];
char *p_char = s;
  va_start(ap, fmt);
  vsprintf(s, fmt,ap);
  va_end(ap);
  while(0 != *p_char)
  {
    if(-1 == GLCD_putchar(*p_char++))
    {
      break;
    }
  }
}

/*************************************************************************
 * Function Name: GLCD_LoadPic
 * Parameters: Int32U X_Left, Int32U Y_Up, Bmp_t * pBmp
 *
 * Return: none
 *
 * Description: Load picture in VRAM memory area
 *
 *************************************************************************/
void GLCD_LoadPic (Int32U X_Left, Int32U Y_Up, Bmp_t * pBmp, Int32U Mask)
{
pInt32U pSrc = pBmp->pPicStream;
Int32U X_LeftHold;
pInt32U pData;

  switch(_Layer)
  {
  case GLCD_BACKGROUND_LAYER:
    pData = (pInt32U) LCD_VRAM_BASE_ADDR;
    break;
  case GLCD_GRAPHIC_WINDOW_LAYER:
    pData = (pInt32U) LCD_GRVRAM_BASE_ADDR;
    break;
  default:
    assert(0);
  }
  pData += X_Left + (Y_Up * C_GLCD_H_SIZE);
  for(Int32U i = 0; i < pBmp->V_Size; i++)
  {
    if(Y_Up++ >= C_GLCD_V_SIZE)
    {
      break;
    }
    for(Int32U j = 0; j < pBmp->H_Size; j++)
    {
      if(X_LeftHold++ >= C_GLCD_H_SIZE)
      {
        pSrc += pBmp->H_Size - j;
        break;
      }
      *(pData+j) = *pSrc++ ^ Mask;
    }
    X_LeftHold = X_Left;
    pData += C_GLCD_H_SIZE;
  }
}

/*************************************************************************
 * Function Name: GLCD_LoadPicTransparent
 * Parameters: Int32U X_Left, Int32U Y_Up, Bmp_t * pBmp
 *
 * Return: none
 *
 * Description: Load picture in VRAM memory area
 *
 *************************************************************************/
void GLCD_LoadPicTransparent (Int32U X_Left, Int32U Y_Up, Bmp_t * pBmp, Int32U Transp)
{
pInt32U pSrc = pBmp->pPicStream;
Int32U X_LeftHold;
pInt32U pData;

  switch(_Layer)
  {
  case GLCD_BACKGROUND_LAYER:
    pData = (pInt32U) LCD_VRAM_BASE_ADDR;
    break;
  case GLCD_GRAPHIC_WINDOW_LAYER:
    pData = (pInt32U) LCD_GRVRAM_BASE_ADDR;
    break;
  default:
    assert(0);
  }

  for(Int32U i = 0; i < pBmp->V_Size; i++)
  {
    if(Y_Up++ >= C_GLCD_V_SIZE)
    {
      break;
    }
    for(Int32U j = 0; j < pBmp->H_Size; j++)
    {
      if(X_LeftHold++ >= C_GLCD_H_SIZE)
      {
        pSrc += pBmp->H_Size - j;
        break;
      }
      if ( Transp == *pSrc)
        *pSrc++;
      else
        *(pData+j) = *pSrc++;
    }
    X_LeftHold = X_Left;
    pData += C_GLCD_H_SIZE;
  }
}

/*************************************************************************
 * Function Name: _GLCD_SPI_Transfer
 * Parameters: Int8U Data
 *
 * Return: Int8U
 *
 * Description: Transfer byte over SPI1
 *
 *************************************************************************/
static
Int8U _GLCD_SPI_Transfer (Int8U Data)
{
  CSPI1_STATREG_bit.TC = 1;
  CSPI1_TXDATA = Data;
  while(0 == CSPI1_STATREG_bit.TC);
  return(CSPI1_RXDATA);
}

/*************************************************************************
 * Function Name: _GLCD__GLCD_Delay_ms
 * Parameters: Int32U Dly
 *
 * Return: Int8U
 *
 * Description: Software delay 1ms resolution
 *
 *************************************************************************/
static
void _GLCD_Delay_ms(Int32U Dly)
{
volatile Int32U i;
  while(Dly--)
  {
    for(i = _1MS_LOOP_DLY; i; i--);
  }
}

/*************************************************************************
 * Function Name: _GLCD_WriteCom
 * Parameters: Int16U Addr
 *
 * Return: none
 *
 * Description: Set address of register into INDEX
 *
 *************************************************************************/
static
void _GLCD_WriteCom(Int16U Addr)
{
	GPIO1_DR_bit.DR17 = 0;
  _GLCD_SPI_Transfer(0x70);
  _GLCD_SPI_Transfer((Int8U)(Addr>>8));
  _GLCD_SPI_Transfer((Int8U)Addr);
	GPIO1_DR_bit.DR17 = 1;
}

/*************************************************************************
 * Function Name: _GLCD_WriteData
 * Parameters: Int16U Data
 *
 * Return: none
 *
 * Description: Write data
 *
 *************************************************************************/
static
void _GLCD_WriteData(Int16U Data)
{
	GPIO1_DR_bit.DR17 = 0;
  _GLCD_SPI_Transfer(0x72);
  _GLCD_SPI_Transfer((Int8U)(Data>>8));
  _GLCD_SPI_Transfer((Int8U)Data);
	GPIO1_DR_bit.DR17 = 1;
}

/*************************************************************************
 * Function Name: GLCD_InitGraphic
 * Parameters: const Int32U *pPain, const Int32U * pPallete
 *
 * Return: none
 *
 * Description: GLCD Init Graphic buffer
 *
 *************************************************************************/
void GLCD_InitGraphic (const Int32U *pPain, const Int32U * pPallete)
{
pInt32U pDst = (pInt32U) LCD_GRVRAM_BASE_ADDR;

  if(NULL != pPallete)
  {
    GLCD_SetPalletGraphic(pPallete);
  }

  if (NULL == pPain)
  {
    // clear display memory
    for(Int32U i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE) > i; i++)
    {
      *pDst++ = 0;
    }
  }
  else
  {
    // set display memory
    for(Int32U i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE) > i; i++)
    {
      *pDst++ = *pPain++;
    }
  }
}

/*************************************************************************
 * Function Name: GLCD_InitGraphic
 * Parameters: Boolean Enable, Int8U Blue, Int8U Green, Int8U Red
 *
 * Return: none
 *
 * Description: GLCD Init Graphic color keying
 *
 *************************************************************************/
void GLCD_GraphicColorKeying (Boolean Enable, Int8U Blue, Int8U Green, Int8U Red)
{
  LGWCR_bit.GWCKB   = Blue;     // Graphic window color keying blue component
  LGWCR_bit.GWCKG   = Green;    // Graphic window color keying green component
  LGWCR_bit.GWCKR   = Red;      // Graphic window color keying red component
  LGWCR_bit.GWCKE   = Enable;   // Disable color keying of graphic window
}

/*************************************************************************
 * Function Name: GLCD_GraphicTransparentCtrl
 * Parameters: Boolean Enable, Int8U
 *
 * Return: none
 *
 * Description: GLCD Init Graphic transparent control
 *
 *************************************************************************/
void GLCD_GraphicTransparentCtrl (Boolean Enable, Int8U Transparent)
{
  LGWCR_bit.GWAV = Transparent;
  LGWCR_bit.GWE  = Enable;
}

/*************************************************************************
 * Function Name: GLCD_PrintfLayerSelect
 * Parameters: GLCD_Layer_t Layer
 *
 * Return: none
 *
 * Description: Set print output layer
 *
 *************************************************************************/
void GLCD_PrintfLayerSelect (GLCD_Layer_t Layer)
{
  _Layer = Layer;
}


/*************************************************************************
 * Function Name: GLCD_Point
 * Parameters: Int32U X, Int32U Y, Int32U Color
 *
 * Return: none
 *
 * Description: Print Point
 *
 *************************************************************************/
void GLCD_Point (Int32U X, Int32U Y, Int32U Color)
{
pInt32U pPix;
  switch(_Layer)
  {
  case GLCD_BACKGROUND_LAYER:
    pPix = (pInt32U) LCD_VRAM_BASE_ADDR;
    break;
  case GLCD_GRAPHIC_WINDOW_LAYER:
    pPix = (pInt32U) LCD_GRVRAM_BASE_ADDR;
    break;
  default:
    assert(0);
  }
  pPix += Y + (X*C_GLCD_V_SIZE);
  *pPix = Color;
}

/*************************************************************************
 * Function Name: GLCD_Line
 * Parameters: Int32U X1,Int32U Y1, Int32U X2, Int32U Y2,
 *             Int32U Color, Int32U Thickness
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
void GLCD_Line(Int32U X1,Int32U Y1, Int32U X2, Int32U Y2,
               Int32U Color, Int32U Thickness)
{
Int32U a,j,step_err_l,step_err_h;
Int32S dx,dy,stepx,stepy,error;

  assert(C_GLCD_H_SIZE > X1);
  assert(C_GLCD_H_SIZE > X2);
  assert(C_GLCD_V_SIZE > Y1);
  assert(C_GLCD_V_SIZE > Y2);

  a = Thickness/2;
  if(X1 == X2)
  {
    // draw vertical line fast
    if(Y1 > Y2)
    {
      Y2 ^= Y1;
      Y1 ^= Y2;
      Y2 ^= Y1;
    }
    X1 -= a;
    for(; Y1 <= Y2; Y1++)
    {
      j = 0;
      do
      {
        assert(C_GLCD_H_SIZE > X1+j);
        GLCD_Point(X1+j,Y1,Color);
      }
      while(++j < Thickness);
    }
    return;
  }
  else if (Y1 == Y2)
  {
    // draw horizontal line fast
    if(X1 > X2)
    {
      X2 ^= X1;
      X1 ^= X2;
      X2 ^= X1;
    }
    Y1 -= a;

    for(; X1 <= X2; X1++)
    {
      j = 0;
      do
      {
        assert(C_GLCD_V_SIZE > Y1+j);
        GLCD_Point(X1,Y1+j,Color);
      }
      while(++j < Thickness);
    }
    return;
  }
  dx = X2-X1;
  dy = Y2-Y1;
  stepy = 1;
  stepx = 1;

  if(0 > dx)
  {
    dx = -dx;
    stepx = -1;
  }

  if(0 > dy)
  {
    dy = -dy;
    stepy = -1;
  }

  if(dx < dy)
  {
    step_err_l = dx << 1;
    step_err_h = dy << 1;
    error = step_err_l - dy;
    do
    {
      j = 0;
      X1-= a;
      do
      {
        assert(C_GLCD_H_SIZE > X1+j);
        GLCD_Point(X1+j,Y1,Color);
      }
      while(++j < Thickness);
      error += step_err_l;
      if(error >= 0)
      {
        error -= step_err_h;
        X1 += stepx;
      }
      Y1 += stepy;
    }
    while(dy--);
  }
  else
  {
    step_err_l = dy << 1;
    step_err_h = dx << 1;
    error = step_err_l - dx;
    do
    {
      j = 0;
      Y1-= a;
      do
      {
        assert(C_GLCD_V_SIZE > Y1+j);
        GLCD_Point(X1,Y1+j,Color);
      }
      while(++j < Thickness);
      error += step_err_l;
      if(error >= 0)
      {
        error -= step_err_h;
        Y1 += stepy;
      }
      X1 += stepx;
    }
    while(dx--);
  }
}

