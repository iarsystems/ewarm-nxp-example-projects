/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : touch_scr.c
 *    Description : Driver for the ADS7843 touch screen
 *
 *    History :
 *    1. Date        : 19 May 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 34 $
 **************************************************************************/
#include "touch_scr.h"

volatile Int32U  X,Y;
volatile TouchScrState_t TouchScrState;
volatile Boolean TouchMeasureEnable;

/*************************************************************************
 * Function Name: Touch_PenHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Touchscreen Pen interrupt handler
 *
 *************************************************************************/
static void Touch_PenHandler (void)
{
  // interrupt from the touchscreen controller (pen touch)
  TouchScrState = TS_MesureX1;
  // Disable interrupt
  IC_DisableInt(1<<IC_GPIO3INTR);
}

/*************************************************************************
 * Function Name: Dly_us
 * Parameters: unsigned long Dly
 *
 * Return: none
 *
 * Description: Delay for usec microSeconds base on Timer 3
 *
 * Notes: Max delay 17777 [us]
 *
 *************************************************************************/
#pragma location="ICODE"
static void Dly_us(unsigned long Dly)
{
register unsigned long long Count = (unsigned long long)TIMR3_INTR_CLK*Dly/1000000;

  if(Count > MAX_DELAY)
  {
    return;
  }
  TIMER_CONTROL3_bit.ENABLE = 0;  // Stop Timer 3 Count
  TIMER_CONTROL3_bit.MODE   = 1;  // Periodic
  // Set period
  TIMER_LOAD3_bit.LOAD      = Count;
  TIMER_TCEOI3 = 0;               // Clear interrupt flag
  TIMER_CONTROL3_bit.ENABLE = 1;  // Start Timer 3 Count
  while(!IC_INTRSR_bit.TC3OI);
  TIMER_CONTROL3_bit.ENABLE = 0;  // Stop Timer 3 Count
}

/*************************************************************************
 * Function Name: AD7843_Transfer
 * Parameters: Int32U Data
 *
 * Return: Int32U
 *
 * Description: Send a data by SPI1 and return the received data from SPI1
 *
 *************************************************************************/
Int32U ADS7843_Transfer(Int32U Data)
{
  CPLD_SPI_CTRL = 0;                    // Deassert all nCS
  // Send Data (fist 5 bits)
  CPLD_SPI_DATA = Data >> 3;            // Load data
  CPLD_SPI_CTRL = SPI_STCS | SPI_SPST;  // Start transition
  while(!(CPLD_SPI_CTRL & SPI_SPLD));   // Wait until data is loaded
  CPLD_SPI_CTRL &= ~SPI_SPST;
  while(!(CPLD_SPI_CTRL & SPI_SPDN));   // Wait until data is sanded
  // Wait Acquire time
  Dly_us(TC_ACQUIRE_TIME);
  // Send Data (last 3 bits)
  CPLD_SPI_DATA = Data << 5;            // Load data
  // Start transition
  CPLD_SPI_CTRL = SPI_STCS | SPI_SPST | SPI_SPRW;
  while(!(CPLD_SPI_CTRL & SPI_SPLD));   // Wait until data is loaded
  CPLD_SPI_CTRL &= ~SPI_SPST;
  while(!(CPLD_SPI_CTRL & SPI_SPDN));   // Wait until data is sanded

  // Receiving First 5 bit
  Data = CPLD_SPI_DATA << 8;
  // Receiving 5 -  11 bits
  CPLD_SPI_DATA = 0;
  CPLD_SPI_CTRL = SPI_STCS | SPI_SPST | SPI_SPRW;
  while(!(CPLD_SPI_CTRL & SPI_SPLD));
  CPLD_SPI_CTRL &= ~SPI_SPST;
  while(!(CPLD_SPI_CTRL & SPI_SPDN));
  Data |= CPLD_SPI_DATA;
  CPLD_SPI_CTRL = 0;                    // Deassert all nCS
  return(Data & 0xFFF);
}

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init SPI and External interrupt - LOW-level triggered
 *
 *************************************************************************/
void TouchScrInit (void)
{
  // Init variable
  TouchScrState = TS_NotTouch;
  TouchMeasureEnable = FALSE;
  X = (Int32U)-1; // Invalid data
  // Acquire timer interrupt disable
  IC_DisableInt(1<<IC_TC3OI);
  // Init CPLD SPI
  ADS7843_Transfer(AD7843_READ_Y);
  // Pen Interrupt Init
  GPIO_PFDD_bit.Py3DIR = 0; // PF3 input
  GPIO_GPIODB_bit.F3   = 0; // Disable pin debouncing
  GPIO_INTTYPE1_bit.F3 = 0; // level sensitive
  GPIO_INTTYPE2_bit.F3 = 0; // LOW-level triggered
  GPIO_GPIOINTEN_bit.F3= 1; // Configures F3 as an External Interrupt
  // registered the interrupt handler of Touchscreen Pen
  // Set address of the handler
  IC_SetVectoredIRQ(Touch_PenHandler,IC_GPIO3INTR);
  // Enable Ext. interrupt
  CPLD_INTR_MASK = 0x00;  // Touch interrupt enable
  IC_EnableInt(1<<IC_GPIO3INTR);
}


/*************************************************************************
 * Function Name: TouchScrStartMesure
 * Parameters: none
 *
 * Return: none
 *
 * Description: Starting the measurement
 *
 *************************************************************************/
void TouchScrStartMesure (void)
{
  if(TouchScrState != TS_NotTouch)
  {
    TouchMeasureEnable = TRUE; // Enable measure
  }
}

/*************************************************************************
 * Function Name: TouchImp
 * Parameters: none
 *
 * Return: none
 *
 * Description: Implement touch - adc result filtering and set touch flag
 *
 *************************************************************************/
void TouchImp (void)
{
static Int32U X_coordinate, Y_coordinate;
  if(TouchMeasureEnable)
  {
    switch(TouchScrState)
    {
    case TS_MesureX1:
      X_coordinate = Y_coordinate = 0;
    case TS_MesureX2:
    case TS_MesureX3:
    case TS_MesureX4:
      // Get X data
      X_coordinate += ADS7843_Transfer(AD7843_READ_X);
      ++TouchScrState;  // Next state
      break;
    case TS_MesureY1:
    case TS_MesureY2:
    case TS_MesureY3:
    case TS_MesureY4:
      // Get Y data
      Y_coordinate += ADS7843_Transfer(AD7843_READ_Y);
      ++TouchScrState;  // Next state
      break;
    case TS_PenCheckLast:
      TouchMeasureEnable = FALSE;
      // Pan interrupt line setup time
      Dly_us(TC_PEN_INTR_ACQUIRE_TIME);
      if(!(CPLD_INTR_MASK & 2))
      {
        X = (X_coordinate >> 2) & 0xFFF;
        Y = (Y_coordinate >> 2) & 0xFFF;
        TouchScrState = TS_MesureX1;
        break;
      }
    default:
      X = (Int32U)-1; // Invalid data
      // return to Touch detection state
      TouchScrState = TS_NotTouch;
      IC_EnableInt(1<<IC_GPIO3INTR);
    }
  }
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
Boolean TouchScrGetStatus (ToushRes_t * pData)
{
Int32U X_coordinate, Y_coordinate;

  if(X == (Int32U)-1) // Discard invalid data
  {
    return(FALSE);
  }
  X_coordinate = X;
  Y_coordinate = Y;

  // Calculate X coordinate in pixels
  if (X_coordinate <= LEFT_UP_X)
  {
    X_coordinate = 0;
  }
  else if (X_coordinate >= RIGHT_UP_X)
  {
    X_coordinate = LCD_WIDTH;
  }
  else
  {
    X_coordinate = ((X_coordinate - LEFT_UP_X) * LCD_WIDTH)/(RIGHT_UP_X-LEFT_UP_X);
  }
  // Calculate Y coordinate in pixels
  if (Y_coordinate <= LEFT_UP_Y)
  {
    Y_coordinate = 0;
  }
  else if (X_coordinate >= RIGHT_UP_Y)
  {
    Y_coordinate = LCD_HEIGHT;
  }
  else
  {
    Y_coordinate = ((Y_coordinate - LEFT_UP_Y) * LCD_HEIGHT)/(RIGHT_UP_Y-LEFT_UP_Y);
  }
  pData->X = LCD_WIDTH - X_coordinate;
  pData->Y = Y_coordinate;
  return(TRUE);
}
