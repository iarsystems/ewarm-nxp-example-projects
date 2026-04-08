/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_touch_scr.c
 *    Description : Touch screen driver module
 *
 *    History :
 *    1. Date        : August, 8 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  Touch screen sampling algorithm
 *   1. Set Y1, Y2 to output 1, Set X1 - ADC input (ch 1), X2 input with pull-down
 *   2. Wait setup delay
 *   3. Check X2 state - if high the screen is touched set Y0 to low and wait
 *                        setup delay
 *                       else wait until interrupt by rising edge from X2
 *      When interrupt arise Y0 to low and wait setup delay
 *   4. Star conversion
 *   5. After TS_SAMPLES change Y1 high Y2 low and result calculate  1023 - ADC
 *   6. After TS_SAMPLES change Y1 to ADC  input Y2 input without pulls X1 and X2
 *      output in low and high level
 *   7. Same like X
 *   8. Apply sample delay and return back to poin 1
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "drv_touch_scr.h"
typedef enum _TouchScrState_t
{
  TS_INTR_SETUP_DLY = 0, TS_WAIT_FOR_TOUCH,
  TS_X1_SETUP_DLY, TS_X1_MEASURE,
  TS_X2_SETUP_DLY, TS_X2_MEASURE,
  TS_Y1_SETUP_DLY, TS_Y1_MEASURE,
  TS_Y2_SETUP_DLY, TS_Y2_MEASURE,
} TouchScrState_t;

#define LEFT_UP_X     100 * TS_SAMPLES * 2
#define RIGHT_UP_X    915 * TS_SAMPLES * 2

#define LEFT_UP_Y     155 * TS_SAMPLES * 2
#define RIGHT_UP_Y    815 * TS_SAMPLES * 2


static volatile Boolean Touch;
static volatile Int16U  X,Y;
static volatile Int16U  X_temp,Y_temp;
static volatile Boolean Touch_temp;

static volatile TouchScrState_t State;
static volatile Int32U Samples;

/*************************************************************************
 * Function Name: TimerIntr_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Sample timer interrupt handler
 *
 *************************************************************************/
void TMR1_IRQHandler (void)
{
  T1IR_bit.MR0INT = 1;  // clear pending interrupt
  T1TCR_bit.CR = 1;
  switch(State)
  {
  case TS_X1_SETUP_DLY:
  case TS_X2_SETUP_DLY:
  case TS_Y1_SETUP_DLY:
  case TS_Y2_SETUP_DLY:
    ++State;
    AD0CR_bit.START = 1;
    break;
  case TS_INTR_SETUP_DLY:
    ++State;
    TS_X2_INTR_CLR = TS_X2_MASK;
    if(0 == (TS_X2_FIO & TS_X2_MASK))
    {
      Touch_temp = Touch = FALSE;
      TS_X2_INTR_R |= TS_X2_MASK;
    }
    else
    {
      // Update X and Y
      if(Touch_temp)
      {
        X = X_temp;
        Y = Y_temp;
      }

      Touch = Touch_temp;

      // Y1 = 0, Y2 = 1;
      TS_Y1_FCLR = TS_Y1_MASK;
      // Disable X2 pull down
      TS_X2_IOCON &= ~(3<<3);
      // Reset sample counter
      Samples = 0;
      // Clear accumulators
      X_temp = Y_temp = 0;
      // Init setup delay
      if(Touch)
      {
        T1MR0 = TS_SETUP_DLY;
      }
      else
      {
        T1MR0 = TS_INIT_DLY;
      }
      State = TS_X1_SETUP_DLY;
      T1TCR = 1;
    }
    break;
  default:
    assert(0);
    break;
  }
  /**/
  NVIC_ClrPend(NVIC_TIMER1);
}

/*************************************************************************
 * Function Name: OnTouchIntr_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: On touch interrupt handler
 *
 *************************************************************************/
void GPIO_IRQHandler (void)
{
  // Disable and clear interrupt
  TS_X2_INTR_R  &= ~TS_X2_MASK;
  TS_X2_INTR_CLR =  TS_X2_MASK;
  // Init ACD measure setup delay
  if(TS_WAIT_FOR_TOUCH == State)
  {
    // Y1 = 0, Y2 = 1;
    TS_Y1_FCLR = TS_Y1_MASK;
    // Disable X2 pull down
    TS_X2_IOCON &= ~(3<<3);
    // Reset sample counter
    Samples = 0;
    // Clear accumulators
    X_temp = Y_temp = 0;
    // Init setup delay
    if(Touch)
    {
      T1MR0 = TS_SETUP_DLY;
    }
    else
    {
      T1MR0 = TS_INIT_DLY;
    }
    State = TS_X1_SETUP_DLY;
    T1TCR = 1;
  }
  else
  {
    assert(0);
  }
  NVIC_ClrPend(NVIC_GPIO);
}


/*************************************************************************
 * Function Name: ADC_Intr_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: End conversion interrupt handler
 *
 *************************************************************************/
void ADC_IRQHandler (void)
{
Int32U Data;
  AD0CR_bit.START = 0;
  Data = AD0GDR_bit.RESULT>>2;  //10 - bit result
  switch(State)
  {
  case TS_X1_MEASURE:
    Y_temp += Data;
    if(++Samples >= TS_SAMPLES)
    {
      Samples = 0;
      State = TS_X2_SETUP_DLY;
      // Y2 = 0, Y1 = 1;
      TS_Y2_FCLR = TS_Y2_MASK;
      TS_Y1_FSET = TS_Y1_MASK;
      // Init setup delay
      T1MR0 = TS_SETUP_DLY;
      T1TCR = 1;
    }
    else
    {
      AD0CR_bit.START = 1;
    }
    break;
  case TS_X2_MEASURE:
    Y_temp += 1023UL - Data;
    if(++Samples >= TS_SAMPLES)
    {
      Samples = 0;
      State = TS_Y1_SETUP_DLY;

      // X1 = 0, X2 = 1;
      TS_X1_FCLR  = TS_X1_MASK;
      TS_X2_FSET  = TS_X2_MASK;
      TS_X1_FDIR |= TS_X1_MASK;
      TS_X2_FDIR |= TS_X2_MASK;
      TS_X1_IOCON   = 0x1A0;

      // Y1 - ADC Ch0, Y2 input
      TS_Y1_FDIR &= ~TS_Y1_MASK;
      TS_Y2_FDIR &= ~TS_Y2_MASK;
      TS_Y1_IOCON   = 0x101;            // assign to ADC0 Ch0
      AD0CR_bit.SEL  = 1UL<<0;    // select Ch0

      // Init setup delay
      T1MR0 = TS_SETUP_DLY;
      T1TCR = 1;
    }
    else
    {
      AD0CR_bit.START = 1;
    }
    break;
  case TS_Y1_MEASURE:
    X_temp += 1023UL - Data;
    if(++Samples >= TS_SAMPLES)
    {
      Samples = 0;
      State = TS_Y2_SETUP_DLY;
      // X2 = 0, X1 = 1;
      TS_X2_FCLR = TS_X2_MASK;
      TS_X1_FSET = TS_X1_MASK;
      // Init setup delay
      T1MR0 = TS_SETUP_DLY;
      T1TCR = 1;
    }
    else
    {
      AD0CR_bit.START = 1;
    }
    break;
  case TS_Y2_MEASURE:
    X_temp += Data;
    if(++Samples >= TS_SAMPLES)
    {
      State = TS_INTR_SETUP_DLY;

      // Y1 = 1, Y2 = 1;
      TS_Y1_FSET  = TS_Y1_MASK;
      TS_Y2_FSET  = TS_Y2_MASK;
      TS_Y1_FDIR |= TS_Y1_MASK;
      TS_Y2_FDIR |= TS_Y2_MASK;
      TS_Y1_IOCON   = 0x1A0;

      // X1 - ADC Ch1, X2 input with pull down
      TS_X1_FDIR &= ~TS_X1_MASK;
      TS_X2_FDIR &= ~TS_X2_MASK;
      TS_X1_IOCON   = 0x101;          // assign to ADC0 Ch1
      TS_X2_IOCON  = 0x28;            // enable pull-down
      AD0CR_bit.SEL  = 1UL<<1;    // select Ch1

      // Init setup delay
      T1MR0 = TS_SAMPLE_DLY;
      T1TCR = 1;
      Touch_temp = TRUE;
    }
    else
    {
      AD0CR_bit.START = 1;
    }
    break;
  default:
    assert(0);
  }
  NVIC_ClrPend(NVIC_ADC);
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
  Touch_temp = Touch = FALSE;
  X = Y = 0;
  State = TS_INTR_SETUP_DLY;

  // Init GPIOs
  
  TS_X1_IOCON   = 0x101;   // ADC0 Ch1 disable pulls
  TS_X2_IOCON   = 0x28;   // general IO enable pull-down

  TS_Y1_IOCON   = 0x1A0;   // general IO disable pulls
  TS_Y2_IOCON   = 0x20;    // general IO disable pulls

  TS_X1_FDIR &= ~TS_X1_MASK;
  TS_X2_FDIR &= ~TS_X2_MASK;
  TS_Y1_FDIR |=  TS_Y1_MASK;
  TS_Y2_FDIR |=  TS_Y2_MASK;

  TS_Y1_FSET  =  TS_Y1_MASK;
  TS_Y2_FSET  =  TS_Y2_MASK;

  // Init Port interrupt
  TS_X2_INTR_R  &= ~TS_X2_MASK; // disable X2 rising edge interrupt
  TS_X2_INTR_CLR =  TS_X2_MASK;
  NVIC_IntEnable(NVIC_GPIO);

  // Init ADC
  PCONP_bit.PCAD = 1;         // Enable ADC clocks
  AD0CR_bit.PDN  = 1;         // converter is operational
  AD0CR_bit.START = 0;
  AD0CR_bit.SEL  = 1UL<<1;    // select Ch1
  AD0CR_bit.CLKDIV = CLK_GetClock(CLK_PERIPH)/ 500000;
  AD0CR_bit.BURST  = 0;       // disable burst

  // clear all pending interrupts
  while(ADSTAT_bit.ADINT)
  {
    volatile Int32U Dummy = AD0GDR;
  }

  ADINTEN_bit.ADGINTEN = 1;   // Enable global interrupt
  NVIC_IntEnable(NVIC_ADC);

  // Init delay timer
  PCONP_bit.PCTIM1 = 1; // Enable TIM0 clocks
  T1TCR = 2;            // stop and reset timer 0
  T1CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T1MCR_bit.MR0S = 1;   // stop timer if MR0 matches the TC
  T1MCR_bit.MR0R = 1;   // enable timer reset if MR0 matches the TC
  T1MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T1PR = (CLK_GetClock(CLK_PERIPH)/ 1000000) - 1; // 1us resolution
  T1MR0 = TS_SETUP_DLY;
  T1IR_bit.MR0INT = 1;  // clear pending interrupt
  /*Enable NVIC TMR1 Interrupt*/
  NVIC_IntEnable(NVIC_TIMER1);
  T1TCR = 1;            // start timer 0
}

/*************************************************************************
 * Function Name: TouchScrGetStatus
 * Parameters: ToushRes_t * pData X,Y data
 *
 * Return: Int32U 0 - untouched
 *                1 - touched
 *
 * Description: Return current state of the touch screen
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
    else if (X_coordinate >= RIGHT_UP_X)
    {
      X_coordinate = IMAGE_WIDTH;
    }
    else
    {
      X_coordinate = ((X_coordinate - LEFT_UP_X) * IMAGE_WIDTH)/(RIGHT_UP_X-LEFT_UP_X);
    }
    // Calculate Y coordinate in pixels
    if (Y_coordinate <= LEFT_UP_Y)
    {
      Y_coordinate = 0;
    }
    else if (Y_coordinate >= RIGHT_UP_Y)
    {
      Y_coordinate = IMAGE_HEIGHT;
    }
    else
    {
      Y_coordinate = ((Y_coordinate - LEFT_UP_Y) * IMAGE_HEIGHT)/(RIGHT_UP_Y-LEFT_UP_Y);
    }
    pData->X = X_coordinate;
    pData->Y = Y_coordinate;
  }
  return(TouchResReadyHold);
}
