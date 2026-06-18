/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_lcd.c
 *    Description : LCD driver module
 *
 *    History :
 *    1. Date        : May, 3 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/

#include "drv_lcd.h"
#pragma segment="DATA_NO_CAHCE"
#pragma location="DATA_NO_CAHCE"
#pragma data_alignment=4
__no_init Int8U LcdBuff[LCD_WIDTH*LCD_HEIGHT*(LCD_BPP/8)];

/*************************************************************************
 * Function Name: LcdSetup
 * Parameters: none
 *
 * Return: none
 *
 * Description: Set up LCD panel to display a video RAM data
 *
 *************************************************************************/
void LcdSetup(void)
{
Int32U * pData;

  // LCD OFF by bit 9 Memory Map IO Register
  LCD_OFF();

  // Enable LCD Clocks and LCD panel
  PCDR1_bit.PCLK_DIV3    = 7; // PERDIV3 is the divider for LCD
  PCCR0_bit.PERCLK3_EN   = 1;

  // Clock and GPIO setup
  PTA_GIUS = 0;   // Use Multiplex function
  PTA_GPR  = 0;   // Select primary function

  // LCD Panel
  LSSAR = (Int32U)LcdBuff;  // Screen Start Address of LCD Panel
  // holds pixel data for a new frame
  // from the SSA address.

  _LSR_bit.XMAX = (LCD_WIDTH / 16);  // Holds screen X Axis
  _LSR_bit.YMAX = LCD_HEIGHT;        // Specifies the Height of
                                    // the LCD panel
  LVPWR = IMAGE_WIDTH * IMAGE_BPP / 32;

  // Cursors Controls
  LCPR_bit.CC = 0;    // Cursor control
  LCPR_bit.OP = 0;    // Arithmetic operator Control
  LCPR_bit.CXP = 0;   // Cursor X position
  LCPR_bit.CYP = 0;   // Cursor Y Position

  LCCMR = 0;

  // Panel Control register
  LPCR_bit.TFT      = 1;  // TFT enable
  LPCR_bit.COLOR    = 1;  // Color enable (set for TFT)
  LPCR_bit.PBSIZ    = 3;  // Panel Bus Width, 0=1 bit,1=2,2=4,3=8 (does not matter for TFT)
  LPCR_bit.BPIX     = 5;  // BPP, 0=1bpp,1=2,2=4,3=8,4=12,5=16
  LPCR_bit.PIX_POL  = 1;  // Pixel Polarity, 0 = Active High, 1 = Active Low
  LPCR_bit.FLM_POL  = 0;  // First Line Marker Polarity, "
  LPCR_bit.LP_POL   = 0;  // Line Pulse Polarity, "
  LPCR_bit.CLK_POL  = 0;  // LCD Shift Clock, 0 = Active Falling, 1 = Active Rising (reverse for TFT)
  LPCR_bit.OE_POL   = 1;  // Output Enable Polarity, 0= Active High, 1 = Active Low
  LPCR_bit.SCLK_IDLE= 0;  // LSCLK Idle Enable (when VSYNC is idle in TFT mode)
  LPCR_bit.END_SEL  = 0;  // Endian Select, 0 = Little, 1 = Big
  LPCR_bit.SWAP_SEL = 0;  // Swap Select, 0 = 16bpp, 1 = else
  LPCR_bit.REV_VS   = 0;  // Reverse Vertical Scan
  LPCR_bit.ACD_SEL  = 1;  // ACD Clock Source select
  LPCR_bit.ACD      = 11; // ACD Stuff
  LPCR_bit.SCLK_SEL = 1;  // LSCLK Select, 0 = Disable OE & LSCLK when no data output in TFT mode for power savings, 1 = Always on
  LPCR_bit.SHARP    = 1;  // Sharp Panel Signals Enable
  LPCR_bit.PCD      = 7;  // Pixel CLK Divider, actual divider is PCD value + 1

  // Horizontal configuration register
  LHCR_bit.H_WAIT_2 = 6;  // H_WAIT2 + 3 = delay between HSYNC & first data of next line
  LHCR_bit.H_WAIT_1 = 15; // H_WAIT1 + 1 = delay in Pixel CLK periods between OE & HSYNC
  LHCR_bit.H_WIDTH  = 1;  // H_WIDTH + 1 = width of Horizontal Sync Pulse in Pixel CLK periods

  // Vertical configuration register
  LVCR_bit.V_WIDTH  = 1;  // V_WIDTH = Vertical Sync pulse width in HSYNC periods
  LVCR_bit.V_WAIT_1 = 9;  // V_WAIT1 = delay between OE and VSYNC (in TFT)
  LVCR_bit.V_WAIT_2 = 7;  // V_WAIT2 = delay between VSYNC and OE of the first line of next frame


  // Panning offset register
  LPOR = 0;

  // Sharp configuration register
  // PS_RISE_DELAY = delay between falling edge of CLS
  // and rising edge of PS in PERCLK2 periods
  LSCR_bit.PS_RISE_DELAY = 0;

  // CLS_RISE_DELAY + 1 = delay of rising edge of CLS
  // to last data out from previous line in PERCLK2
  LSCR_bit.CLS_RISE_DELAY = 18;

  // REV_TOGGLE_DELAY + 1 = delay between last data out of
  // previous line and transition of REV in PERCLK2
  LSCR_bit.REV_TOGGLE_DELAY = 3;

  // Default Grey Scale 2 settings, not relevant to TFT
  LSCR_bit.GRAY2 = 0;

  // Default Grey Scale 1 settings, not relevant to TFT
  LSCR_bit.GRAY1 = 0;

  // PWM contrast control register
  // CLS_HI_WIDTH + 1 = width of high CLS
  // pulse in Pixel Clock periods
  LPCCR_bit.CLS_HI_WIDTH = 169;

  // LD Mask - Enables/Disables LD[15:0] output
  // to zero for Sharp TFT power off sequence
  LPCCR_bit.LDMSK = 0;

  // Source Select for PWM counter, 0 = Line pulse,
  // 1 = Pixel Clock, 2 = LCD Clock (perclk2)
  LPCCR_bit.SCR = 1;

  // Contrast Control Enable
  LPCCR_bit.CC_EN = 1;

  // Pulse Width of PWM controlling contrast
  LPCCR_bit.PW = 255;

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

  PCCR0_bit.HCLK_LCDC_EN  = 1;
  LCD_ON();

  // Clear display
  for (pData = (Int32U *)LcdBuff;
       pData < (Int32U *)&LcdBuff[LCD_WIDTH*LCD_HEIGHT*(IMAGE_BPP/8)];
       ++pData)
  {
    *pData = 0;
  }
}



