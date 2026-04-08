/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : touch_scr.c
 *    Description : Driver for the touch screen
 *
 *    History :
 *    1. Date        : 5 May 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 34 $
 **************************************************************************/
#include "touch_scr.h"

volatile TouchScrState_t TouchScrState;
volatile Int32U  X,Y;

// Pen detecting
const AdcCtrlWords_t AdcIdleTouchDetecting =
{
  .HighWord = ADC_SetTime(IDLY_DLY) | PMN_AN0 | IMN_REF_N | RMP_REF_P,
  .LowWord  = AN0_VDDA_WEAK | AN3_GND_STRONG | RMN_VREF_N,
};

// Pen was detected
const AdcCtrlWords_t AdcIdleTouchDetected =
{
  .HighWord = ADC_SetTime(IDLY_DLY) | PMN_AN0 | IMN_REF_N | RMP_REF_P,
  .LowWord  = RMN_VREF_N,
};

const AdcCtrlWords_t TouchScrSeq [TOUCH_MEASURE_STEPS] =
{
  // Precharge state - 1
  {
    .HighWord = ADC_SetTime(PRE_CHRAGE_DLY) | PMN_AN0 | IMN_REF_N | RMP_REF_P,
    .LowWord  = AN0_VDDA_STRONG | RMN_VREF_N,
  },
  // Touch detect - 2
  {
    .HighWord = ADC_SetTime(TOUCH_DET_DLY) | PMN_AN0 | IMN_REF_N | RMP_REF_P,
    .LowWord  = AN0_VDDA_WEAK | AN3_GND_STRONG | RMN_VREF_N,
  },
  // Measure 1.1 - 3
  {
    .HighWord = ADC_SetTime(X_MEAS_DLY) | PMN_AN2 | IMN_REF_N | RMP_AN0,
    .LowWord  = AN1_GND_STRONG | AN0_VDDA_STRONG | RMN_AN1,
  },
  // Measure 1.2 - 4
  {
    .HighWord = ADC_SetTime(X_MEAS_DLY) | PMN_AN2 | IMN_REF_N | RMP_AN0,
    .LowWord  = AN1_GND_STRONG | AN0_VDDA_STRONG | RMN_AN1,
  },
  // Measure 1.3 - 5
  {
    .HighWord = ADC_SetTime(X_MEAS_DLY) | PMN_AN2 | IMN_REF_N | RMP_AN0,
    .LowWord  = AN1_GND_STRONG | AN0_VDDA_STRONG | RMN_AN1,
  },
  // Measure 1.4 - 6
  {
    .HighWord = ADC_SetTime(X_MEAS_DLY) | PMN_AN2 | IMN_REF_N | RMP_AN0,
    .LowWord  = AN1_GND_STRONG | AN0_VDDA_STRONG | RMN_AN1,
  },
  // Measure 2.1 - 7
  {
    .HighWord = ADC_SetTime(Y_MEAS_DLY) | PMN_AN0 | IMN_REF_N | RMP_AN2,
    .LowWord  = AN3_GND_STRONG | AN2_VDDA_STRONG | RMN_AN3,
  },
  // Measure 2.2 - 8
  {
    .HighWord = ADC_SetTime(Y_MEAS_DLY) | PMN_AN0 | IMN_REF_N | RMP_AN2,
    .LowWord  = AN3_GND_STRONG | AN2_VDDA_STRONG | RMN_AN3,
  },
  // Measure 2.3 - 9
  {
    .HighWord = ADC_SetTime(Y_MEAS_DLY) | PMN_AN0 | IMN_REF_N | RMP_AN2,
    .LowWord  = AN3_GND_STRONG | AN2_VDDA_STRONG | RMN_AN3,
  },
  // Measure 2.4 - 10
  {
    .HighWord = ADC_SetTime(Y_MEAS_DLY) | PMN_AN0 | IMN_REF_N | RMP_AN2,
    .LowWord  = AN3_GND_STRONG | AN2_VDDA_STRONG | RMN_AN3,
  },
  // Precharge state - 11
  {
    .HighWord = ADC_SetTime(PRE_CHRAGE_DLY) | PMN_AN0 | IMN_REF_N | RMP_REF_P,
    .LowWord  = AN0_VDDA_STRONG | RMN_VREF_N,
  },
  // Touch detect - 12
  {
    .HighWord = ADC_SetTime(TOUCH_DET_DLY) | PMN_AN0 | IMN_REF_N | RMP_REF_P,
    .LowWord  = AN0_VDDA_WEAK | AN3_GND_STRONG | RMN_VREF_N,
  },
};

/*************************************************************************
 * Function Name: TouchScr_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Touchscreen common interrupt handler
 *
 *************************************************************************/
void  TouchScr_Handler (void)
{
  if(ADC_MIS_bit.PENSYNC)
  {
    TouchScrPen_Handler();
  }
  else
  {
    TouchScrEOC_Handler();
  }
}

/*************************************************************************
 * Function Name: TouchScrEOC_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Touchscreen end of conversion interrupt handler
 *
 *************************************************************************/
void TouchScrEOC_Handler (void)
{
Int32U Temp1, Temp2, Detect;
  Detect = ADC_RR;                // discard precharge value
  Detect = (ADC_RR >> 6) & 0x3FF; // touch detect value
  if(Detect < TOUCH_THRESHOLD)
  {
    Temp1  = (ADC_RR >> 6) & 0x3FF; // X - 1
    Temp1 += (ADC_RR >> 6) & 0x3FF; // X - 2
    Temp1 += (ADC_RR >> 6) & 0x3FF; // X - 3
    Temp1 += (ADC_RR >> 6) & 0x3FF; // X - 4

    Temp2  = (ADC_RR >> 6) & 0x3FF; // Y - 1
    Temp2 += (ADC_RR >> 6) & 0x3FF; // Y - 2
    Temp2 += (ADC_RR >> 6) & 0x3FF; // Y - 3
    Temp2 += (ADC_RR >> 6) & 0x3FF; // Y - 4

    Detect = ADC_RR;                // discard precharge value
    Detect = (ADC_RR >> 6) & 0x3FF; // touch detect value
    if(Detect < TOUCH_THRESHOLD)
    {
      TouchScrState = TOUCH_SCR_IDLE;
      Y = Temp1/4;
      X = Temp2/4;
    }
    else
    {
      TouchScrState = TOUCH_SCR_PEN_DETECTION;
    }
  }
  else
  {
    // Discard measure
    while(!ADC_FS_bit.FEMPTY)
    {
      Detect = ADC_RR;
    }
    TouchScrState = TOUCH_SCR_PEN_DETECTION;
  }
  // Pen Interrupt clear, End of Sequence Interrupt clear
  ADC_IC = 3;
  if(TouchScrState == TOUCH_SCR_PEN_DETECTION)
  {
    // Not a touch detected
    ADC_IHWCTRL = AdcIdleTouchDetecting.HighWord;
    ADC_ILWCTRL = AdcIdleTouchDetecting.LowWord;
    // Disable periodic update of position of the pen
    TouchScrState = TOUCH_SCR_PEN_DETECTION;
    ADC_GC_bit.SSM = 1;
    // Pen Interrupt Enable
    ADC_IE_bit.PMSK = 1;
  }
}

/*************************************************************************
 * Function Name: TouchScrPT_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Touchscreen Pen interrupt handler
 *
 *************************************************************************/
void TouchScrPen_Handler (void)
{
  // Pen Interrupt Disable
  ADC_IE_bit.PMSK = 0;
  ADC_GC_bit.SSM = 2;
  // Idle state is all switches off
  ADC_IHWCTRL = AdcIdleTouchDetected.HighWord;
  ADC_ILWCTRL = AdcIdleTouchDetected.LowWord;
}

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: VicIrqSlots_t IntrSlot
 *
 * Return: none
 *
 * Description: Init Touch screen sequence
 *
 *************************************************************************/
void TouchScrInit (VicIrqSlots_t IntrSlot)
{
volatile Int32U Dummy;
  // Init variable
  TouchScrState = TOUCH_SCR_PEN_DETECTION;
  // Global interrupts disable
  ADC_IE_bit.INTEN = 0;
  // Clear pending interrupts
  ADC_IC = 0x3; // Pen Interrupt clear, End of Sequence Interrupt clear
  // Set Bias
  ADC_IHWCTRL = AdcIdleTouchDetecting.HighWord;
  ADC_ILWCTRL = AdcIdleTouchDetecting.LowWord;
  // Drain ADC FIFO
  while(!ADC_FS_bit.FEMPTY)
  {
    Dummy = ADC_RR;
  }
  // Init measure sequence
  for(Int32U i = 0, * pHighDest = (Int32U *)&ADC_HWCB0, * pLowDest  = (Int32U *)&ADC_LWCB0;
      i < TOUCH_MEASURE_STEPS; ++i, ++pLowDest, ++pHighDest)
  {
    *pLowDest = TouchScrSeq[i].LowWord;
    *pHighDest = TouchScrSeq[i].HighWord;
  }
  // Enable interrupts
  ADC_IE_bit.PMSK   = 1;      // Pen Interrupt enable
  ADC_IE_bit.EOSMSK = 1;      // End-of-Sequence Interrupt enable
  // Set Power ctrl register
  // Set ADC clk frequency It must be between 200kHz and 2MHz
  ADC_PC_bit.CLKSEL = ADC_CLK_DIV;
  // Number of Conversions in Sequence
  ADC_PC_bit.NOC    = TOUCH_MEASURE_STEPS-1;
  ADC_PC_bit.REFEN  = 1;      // Reference Enable
  ADC_PC_bit.PWM    = 1;      // Standby (wake on SSB or Pen Interrupt, convert, return)
  // Set General cnfg register
  ADC_GC_bit.SSM    = 1;      // SSB or Pen Interrupt starts new conversions
  // Registering interrupt handlers
  VIC_SetVectoredIRQ(Vic2,TouchScr_Handler,IntrSlot,VIC2_TSCIN);
  VIC_EnableInt(Vic2,1UL << VIC2_TSCIN);
  // Global interrupts enable
  ADC_IE_bit.INTEN = 1;
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
  if(TouchScrState == TOUCH_SCR_IDLE)
  {
    ADC_GC_bit.SSB = 1; // Start conversion
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
  if(TouchScrState != TOUCH_SCR_IDLE)
  {
    return(FALSE);
  }
  __disable_interrupt();
  X_coordinate = X;
  Y_coordinate = Y;
  __enable_interrupt();
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
