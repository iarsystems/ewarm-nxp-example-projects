/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_touch_scr.c
 *    Description : Touch screen driver module
 *
 *    History :
 *    1. Date        : May, 3 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "drv_touch_scr.h"

volatile Boolean Touch;
volatile Int16U  X,Y,Z;

/*************************************************************************
 * Function Name: Gpt2_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT2 interrupt handler
 *
 *************************************************************************/
void GPT2_Handler (void)
{
Int32U Data,Buffer[8];

  // Clear interrupt flag
  TSTAT2_bit.COMP = 1;
  Data = Data_Transfer(0, ATLAS_INTR_STAT0, 0);

  if(Data & 0x04)
  {
    // init measurement
    // ADREFEN, ADREFMODE, TSMOD = 2, ADINC1, ADINC2
    Data_Transfer(0x033400, ATLAS_ADC0, 1);
    // clear interrupt
    Data_Transfer(0x000005, ATLAS_INTR_STAT0, 1);
    // start conversion
    Data_Transfer(0x310409, ATLAS_ADC1, 1);
  }
  else if (Data & 0x01)
  {
    Data = Data_Transfer(0, ATLAS_ADC1, 0);
    Data &= 0x0007e0;
    Data |= 4UL << 8;
    Data_Transfer(Data, ATLAS_ADC1, 1);
    for(Int32U i = 0; i < 4; ++i)
    {
      Data = Data_Transfer(0, ATLAS_ADC2, 0);
      Buffer[i]   = (Data>> 2) & 0x3FF;
      Buffer[i+4] = (Data>>14) & 0x3FF;
    }
    // get pressure
    Z = Buffer[6] + Buffer[7];
    if(Z > TS_MIN_PRESSURE)
    {
      LED1_ON();
      // clear interrupt
      Data_Transfer(0x000001, ATLAS_INTR_STAT0, 1);
      // start conversion
      Data_Transfer(0x310409, ATLAS_ADC1, 1);
      // collect data
      X = Buffer[0] + Buffer[1] + Buffer[2];
      Y = Buffer[3] + Buffer[4] + Buffer[5];
      Touch = TRUE;
    }
    else
    {
      LED1_OFF();
      // init touch detection
      // ADREFEN, ADREFMODE, TSMOD = 1
      Data_Transfer(0x001C00, ATLAS_ADC0, 1);
      // clear interrupt
      Data_Transfer(0x00005, ATLAS_INTR_STAT0, 1);
      // touch interrupt mode
      Data_Transfer(0x210001, ATLAS_ADC1, 1);
      Touch = FALSE;
    }
  }
}


/*************************************************************************
 * Function Name: Data_Transfer
 * Parameters: Int32U Data, Int8U Addr, Boolean Write
 *
 * Return: Int32U
 *
 * Description: Transfer data by CSPI1
 *
 *************************************************************************/
static
Int32U Data_Transfer(Int32U Data, Int8U Addr, Boolean Write)
{
  Data = ((Write)?1UL<<31:0) |
         ((Int32U)(Addr & 0x3F)<<25) |
         ((Write)?(Data & 0x01FFFFFF):0);
  CSPI1_TXDATA = Data;
  // data ready to send
  CSPI1_CONREG_bit.XCH = 1;
  while(!CSPI1_INTREG_bit.RR || CSPI1_CONREG_bit.XCH);
  Data = CSPI1_RXDATA;
  return(Data & 0x00FFFFFF);
}

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Touch screen
 *
 *************************************************************************/
void TouchScrInit (void)
{
  // Init variable
  Touch = FALSE;
  X = Y = 0;

  // Init ATLAS
  // Enable ATLAS
  ATLAS_PWR_ON();

  // Init SPI1
  // PD31 assign to CSPI1 MOSI
  PTD_GIUS_bit.PIN31 = 0;PTD_GPR_bit.PIN31 = 0;
  // PD30 assign to CSPI1 MISO
  PTD_GIUS_bit.PIN30 = 0;PTD_GPR_bit.PIN30 = 0;
  // PD29 assign to CSPI1 CLK
  PTD_GIUS_bit.PIN29 = 0;PTD_GPR_bit.PIN29 = 0;
  // PD28 assign to CSPI1 SS0
  PTD_GIUS_bit.PIN28 = 0;PTD_GPR_bit.PIN28 = 0;
  // Enable CSPI1 clock
  PCCR0_bit.CSPI1_EN = 1;
  CSPI1_CONREG_bit.MODE = 1;          // Master mode
  CSPI1_CONREG_bit.POL = 0;           // SPI mode 0
  CSPI1_CONREG_bit.PHA = 0;
  CSPI1_CONREG_bit.SSPOL = 1;         // SS active high
  CSPI1_CONREG_bit.DR_CTL = 0;        // Ignore RDY signal
  CSPI1_CONREG_bit.BIT_COUNT = 32-1;  // 32 bit word
  // SCLK < 20MHz (PER_CLK2/16)
  CSPI1_CONREG_bit.DATARATE = 6;
  CSPI1_CONREG_bit.CS = 0;   // select CS0
  // Enable CSPI1
  CSPI1_CONREG_bit.SPIEN = 1;

  // Init touch detection
  // ADC assignment to Prim SPI
  Data_Transfer(1UL << 12, ATLAS_ARBT_PER_AUDIO, 1);

  // ADREFEN, ADREFMODE, TSMOD = 1 - Touch screen interrupt mode
  Data_Transfer(0x001c00, ATLAS_ADC0, 1);
  // clear interrupt
  Data_Transfer(0xFFFFFF, ATLAS_INTR_STAT0, 1);
  // touch interrupt mode
  Data_Transfer(0x210001, ATLAS_ADC1, 1);

  // Init GPT2 - define acquire time ~100us
  PCCR0_bit.GPT2_EN = 1;
  // Timer disable
  TCTL2_bit.TEN = 0;
  // clear tmr counter when time is disable
  TCTL2_bit.CC = 1;
  // Timer clock source PERCLK1
  TCTL2_bit.CLKSOURCE = 1;
  // Set prescaler
  TPRER2_bit.PRESCALER = 0;
  // Set polling period 20us
  TCMP2 = MPLL_CLK / ((PCDR1_bit.PERDIV1+1) * (1000000 / 20000));
  // Clear interrupt flag
  TSTAT2_bit.COMP = 1;
  // Comparator interrupt enable
  TCTL2_bit.COMP_EN = 1;
  // Init GPT2 interrupt
  AITC_SetupIntr(GPT2_Handler,FALSE,INT_GPT2,0);
  AITC_EnableIntSource(INT_GPT2);
  // Enable timer compare interrupt
  TCTL2_bit.COMP_EN = 1;
  // Timer enable
  TCTL2_bit.TEN = 1;
}

/*************************************************************************
 * Function Name: TouchScrGetStatus
 * Parameters: ToushRes_t * pData X,Y data
 *
 * Return: Int32U 0 - untouched
 *                1 - touched
 *
 * Description: Return current state of the touchscreen
 *
 *************************************************************************/
Boolean TouchGet (ToushRes_t * pData)
{
Boolean  TouchResReadyHold = Touch;
Int32U X_coordinate, Y_coordinate;

  if(TouchResReadyHold)
  {
    X_coordinate = X;
    Y_coordinate = Y;
    // Calculate X coordinate in pixels
    if (X_coordinate <= LEFT_UP_X)
    {
      X_coordinate = 0;
    }
    else if (X_coordinate >= RIGHT_DOWN_X)
    {
      X_coordinate = IMAGE_WIDTH;
    }
    else
    {
      X_coordinate = ((X_coordinate - LEFT_UP_X) * IMAGE_WIDTH)/(RIGHT_DOWN_X-LEFT_UP_X);
    }
    // Calculate Y coordinate in pixels
    if (Y_coordinate <= LEFT_UP_Y)
    {
      Y_coordinate = 0;
    }
    else if (Y_coordinate >= RIGHT_DOWN_Y)
    {
      Y_coordinate = IMAGE_HEIGHT;
    }
    else
    {
      Y_coordinate = ((Y_coordinate - LEFT_UP_Y) * IMAGE_HEIGHT)/(RIGHT_DOWN_Y-LEFT_UP_Y);
    }
    pData->X = X_coordinate;
    pData->Y = Y_coordinate;
  }
  return(TouchResReadyHold);
}
