/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_lcd.c
 *    Description : iMX31 PDK LCD driver module
 *
 *    History :
 *    1. Date        : 04.11.2008 
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *    $Revision: 5880 $
 **************************************************************************/

/** include files **/
#include <NXP/iomcimx31.h>
#include <string.h>
#include "arm_comm.h"
#include "drv_lcd.h"
#include "board.h"

/** local definitions **/
/*LCD SPI*/
#define LCD_SPI_PRM     (0x1<<8)
#define LCD_SPI_DISON   0x29
#define LCD_SPI_DISOFF  0x28
#define LCD_SPI_GAMSET  0x26
#define LCD_SPI_SLPIN   0x10
#define LCD_SPI_SLPOUT  0x11
#define LCD_SPI_PASET   0x2B
#define LCD_SPI_CASET   0x2A
#define LCD_SPI_MADCTL  0x36
#define LCD_SPI_COLMOD  0x3A
#define LCD_SPI_RAMWR   0x2C
#define LCD_SPI_PTLON   0x12
#define LCD_SPI_PTLAR   0x30
#define LCD_SPI_NORON   0x13
/** default settings **/

/** external functions **/
extern void Delay(Int32U time);
/** external data **/

/** internal functions **/
void Init_LCD_SPI(void);
void Send_LCD_SPI(Int32U Data);
/** public data **/
#pragma segment="DDRRAM_DATA_NO_CACHE"
#pragma location="DDRRAM_DATA_NO_CACHE"
#pragma data_alignment=4
__no_init Int16U LcdBgBuff[LCD_WIDTH*LCD_HEIGHT];
#pragma location="DDRRAM_DATA_NO_CACHE"
#pragma data_alignment=4
__no_init Int16U LcdFgBuff[PICTURE_WIDTH*PICTURE_HEIGHT];
/** private data **/


/** public functions **/
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

  /*LCD Pin config*/
  IOMUX_SW_MUX_CTL5_bit.SW_MUX_OUT_EN3 = 1;  /*VSYNC pin Func Enable*/
  IOMUX_SW_MUX_CTL5_bit.SW_MUX_OUT_EN2 = 1;  /*CONTRAST pin Func Enable*/
  IOMUX_SW_MUX_CTL6_bit.SW_MUX_OUT_EN3 = 0;  /*GPIO3_25 pin Func Enable*/
  IOMUX_SW_PAD_CTL9_bit.IPP_ODE2 = 0;       /*Standard CMOS Push Pull*/ 
  IOMUX_SW_PAD_CTL9_bit.IPP_DSE2 = 0;       /*Standard Drive Strength*/ 
  IOMUX_SW_PAD_CTL9_bit.IPP_PUE2 = 0;       /*Disable Pull Up and Keepre*/ 
  IOMUX_SW_PAD_CTL9_bit.IPP_PKE2 = 0;          
  IOMUX_SW_MUX_CTL7_bit.SW_MUX_OUT_EN0 = 0;  /*GPIO3_24 pin Enable*/
  IOMUX_SW_PAD_CTL10_bit.IPP_ODE0 = 0;       /*Standard CMOS Push Pull*/ 
  IOMUX_SW_PAD_CTL10_bit.IPP_DSE0 = 0;       /*Standard Drive Strength*/ 
  IOMUX_SW_PAD_CTL10_bit.IPP_PUE0 = 0;       /*Disable Pull Up and Keepre*/ 
  IOMUX_SW_PAD_CTL10_bit.IPP_PKE0 = 0;        
  IOMUX_SW_MUX_CTL8_bit.SW_MUX_OUT_EN3 = 1;  /*HSYNC pin Func Enable*/
  IOMUX_SW_MUX_CTL8_bit.SW_MUX_OUT_EN2 = 1;  /*FPSHIFT pin Func Enable*/
  IOMUX_SW_MUX_CTL9_bit.SW_MUX_OUT_EN3 = 1;  /*LCD_D15 pin Func Enable*/
  IOMUX_SW_MUX_CTL9_bit.SW_MUX_OUT_EN2 = 1;  /*LCD_D16 pin Func Enable*/
  IOMUX_SW_MUX_CTL9_bit.SW_MUX_OUT_EN1 = 1;  /*LCD_D17 pin Func Enable*/
  IOMUX_SW_MUX_CTL10_bit.SW_MUX_OUT_EN3 = 1; /*LCD_D11 pin Func Enable*/
  IOMUX_SW_MUX_CTL10_bit.SW_MUX_OUT_EN2 = 1; /*LCD_D12 pin Func Enable*/
  IOMUX_SW_MUX_CTL10_bit.SW_MUX_OUT_EN1 = 1; /*LCD_D13 pin Func Enable*/
  IOMUX_SW_MUX_CTL10_bit.SW_MUX_OUT_EN0 = 1; /*LCD_D14 pin Func Enable*/
  IOMUX_SW_MUX_CTL11_bit.SW_MUX_OUT_EN3 = 1; /*LCD_D7 pin Func Enable*/
  IOMUX_SW_MUX_CTL11_bit.SW_MUX_OUT_EN2 = 1; /*LCD_D8 pin Func Enable*/
  IOMUX_SW_MUX_CTL11_bit.SW_MUX_OUT_EN1 = 1; /*LCD_D9 pin Func Enable*/
  IOMUX_SW_MUX_CTL11_bit.SW_MUX_OUT_EN0 = 1; /*LCD_D10 pin Func Enable*/
  IOMUX_SW_MUX_CTL12_bit.SW_MUX_OUT_EN3 = 1; /*LCD_D3 pin Func Enable*/
  IOMUX_SW_MUX_CTL12_bit.SW_MUX_OUT_EN2 = 1; /*LCD_D4 pin Func Enable*/
  IOMUX_SW_MUX_CTL12_bit.SW_MUX_OUT_EN1 = 1; /*LCD_D5 pin Func Enable*/
  IOMUX_SW_MUX_CTL12_bit.SW_MUX_OUT_EN0 = 1; /*LCD_D6 pin Func Enable*/
  IOMUX_SW_MUX_CTL13_bit.SW_MUX_OUT_EN2 = 1; /*LCD_D0 pin Func Enable*/
  IOMUX_SW_MUX_CTL13_bit.SW_MUX_OUT_EN1 = 1; /*LCD_D1 pin Func Enable*/
  IOMUX_SW_MUX_CTL13_bit.SW_MUX_OUT_EN0 = 1; /*LCD_D2 pin Func Enable*/
  
  GPIO3_GDIR_bit.no24 = 1;   /*LCD Reset pin as output*/
  GPIO3_GDIR_bit.no25 = 1;   /*Data Enable pin as output*/
  GPIO3_DR_bit.no24 = 0;     /*Drive LCD Reset Low*/
  GPIO3_DR_bit.no24 = 0;     /*Drive Data Enable Low*/

  /*Power Up display*/
  Init_LCD_SPI();
  //
  Delay(100);           /*Delay 100us*/

  GPIO3_DR_bit.no24 = 1;      /*Drive LCD Reset High*/
  /**/
  Delay(15000);           /*Delay 15ms*/
  Send_LCD_SPI(LCD_SPI_MADCTL);
  Delay(10);              /*Delay 10us*/
  Send_LCD_SPI(0x100);
  Delay(15000);           /*Delay 15ms*/
  Send_LCD_SPI(LCD_SPI_SLPOUT);

  Delay(60000);           /*Delay 60ms*/

  Send_LCD_SPI(LCD_SPI_DISON);

  GPIO3_DR_bit.no25 = 1;      /*Drive Data Enable High*/
  
}
/** private functions **/
/*************************************************************************
 * Function Name: Init_LCD_SPI
 * Parameters: None
 *
 * Return: None
 *
 * Description: Init CSPI1 Module. Mastere mode SPI slock is 8MHz
 *
 *************************************************************************/
void Init_LCD_SPI(void)
{
  IOMUX_GPR |= 1<<2;                          /*Replaces Full UART Group with CSPI1 signals.*/
  IOMUX_SW_MUX_CTL28_bit.SW_MUX_OUT_EN0 = 2;  /*SCLK pin Alternate mode 1 Enable*/
  
  CCM_CGR2 |= 3<<2;                       //Enalbe CSPI1 Clock   
  CSPI1_CONREG_bit.MODE = 1;          // Master mode
  CSPI1_CONREG_bit.POL = 0;           // SPI mode 0
  CSPI1_CONREG_bit.PHA = 0; 
  CSPI1_CONREG_bit.SSPOL = 0;         // SS active Low
  CSPI1_CONREG_bit.DRCTL = 0;         // Ignore RDY signal
  CSPI1_CONREG_bit.BIT_COUNT = 9-1;  // 9 bit word
  // SCLK < 10MHz (IPG_CLK/8)
  CSPI1_CONREG_bit.DATA_RATE = 1;      
  CSPI1_CONREG_bit.CS = 2;            // select SS2
  // Enable CSPI1
  CSPI1_CONREG_bit.EN = 1;
}

/*************************************************************************
 * Function Name: Send_LCD_SPI
 *
 * Parameters: Int32U Data - 9 bit Data
 *              
 * Return: None 
 *
 * Description: Send Command or Parameter to LCD
 *
 *************************************************************************/
void Send_LCD_SPI(Int32U Data)
{
volatile Int32U Dummy;         
  CSPI1_TXDATA = Data;
  // data ready to send
  CSPI1_CONREG_bit.XCH = 1;
  while(!CSPI1_STATREG_bit.RR || CSPI1_CONREG_bit.XCH);
  Dummy = CSPI1_RXDATA;
}






