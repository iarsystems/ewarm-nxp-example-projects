/***********************************************************************
* ID: lpc23xx_tc_driver.h 2008-10-31 HieuNguyen
*
* Project: LPC23xx timer driver
*
* Description:
*     This file contains driver support for the LPC23xx timer.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only  
* which provides customers with programming information regarding the  
* products. This software is supplied "AS IS" without any warranties.  
* NXP Semiconductors assumes no responsibility or liability for the 
* use of the software, conveys no license or title under any patent, 
* copyright, or mask work right to the product. NXP Semiconductors 
* reserves the right to make changes in the software without 
* notification. NXP Semiconductors also make no representation or 
* warranty that such application will be suitable for the specified 
* use without further testing or modification. 
**********************************************************************/
#ifndef LPC23XX_TC_DRIVER_H_
#define LPC23XX_TC_DRIVER_H_

#include "lpc23xx_tc.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Timer device enumeration
 **********************************************************************/
/* Timer/counter operating mode */
typedef enum
{
	TC_TIMER_MODE = 0,				/* Timer mode */
	TC_COUNTER_RISING_MODE,			/* Counter rising mode */
	TC_COUNTER_FALLING_MODE,		/* Counter falling mode */
	TC_COUNTER_ANY_MODE				/* Counter on both edges */
} TC_MODE_OPT;

/* Timer/Counter prescale option */
typedef enum
{
	TC_PRESCALE_TICKVAL = 0,		/* Prescale in absolute value */
	TC_PRESCALE_USVAL				/* Prescale in microsecond value */
} TC_PRESCALE_OPT;

/* Counter input option */
typedef enum
{
	TC_COUNTER_INCAP0 = 0,			/* CAPn.0 input pin for TIMERn */
	TC_COUNTER_INCAP1,				/* CAPn.1 input pin for TIMERn */
} TC_COUNTER_INPUT_OPT;

/* Timer/Counter external match option */
typedef enum
{
	TC_EXTMATCH_NOTHING = 0,		/* Do nothing for external output pin if match */
	TC_EXTMATCH_LOW,				/* Force external output pin to low if match */
	TC_EXTMATCH_HIGH,				/* Force external output pin to high if match */
	TC_EXTMATCH_TOGGLE				/* Toggle external output pin if match */
}TC_EXTMATCH_OPT;

/* Timer/counter capture mode options */
typedef enum {
	TC_CAPTURE_NONE = 0,	/* No Capture */
    TC_CAPTURE_RISING,		/* Rising catpture mode */
    TC_CAPTURE_FALLING,		/* Falling capture mode */
    TC_CAPTURE_ANY			/* On both edges */
} TC_CAP_MODE_OPT;

/***********************************************************************
 * Timer/counter driver functions
 **********************************************************************/
/* Init Timer/Counter device */
S32 TC_Init (S32 TCNum);

/* Close Timer/Counter device */
S32 TC_Close (S32 TCdev);

/* Start Timer/Counter device */
S32 TC_Start (S32 TCdev);

/* Stop Timer/Counter device */
S32 TC_Stop (S32 TCdev);

/* Reset count value in Timer/Counter device */
S32 TC_Reset (S32 TCdev);

/* Set operation mode for Timer/Counter device */
S32 TC_SetMode (S32 TCdev, S32 TCMode);

/* Setup Timer for Timer/Counter device */
S32 TC_SetupTimer (S32 TCdev, S32 prescale_option, U32 prescale);

/* Set Counter input source for Timer/Counter device */
S32 TC_SetCounterInput (S32 TCdev, S32 ICAPsource);

/* Configure External Match Output */
S32 TC_ConfigExtMatch (S32 TCdev, U32 matchsource, S32 ExtMatchtype);

/* Setup match value for each match channel */
S32 TC_SetMatch (S32 TCdev, U32 matchsource, U32 matchvalue);

/* Enable Timer/Counter match interrupt */
S32 TC_EnableMatchInt (S32 TCdev, U32 matchsource);

/* Disable Timer/Counter match interrupt */
S32 TC_DisableMatchInt (S32 TCdev, U32 matchsource);

/* Enable Reset Timer/Counter when matching */
S32 TC_EnableMatchReset (S32 TCdev, U32 matchsource);

/* Disable Reset Timer/Counter when matching */
S32 TC_DisableMatchReset (S32 TCdev, U32 matchsource);

/* Enable Stop Timer/Counter when matching */
S32 TC_EnableMatchStop (S32 TCdev, U32 matchsource);

/* Disable Stop Timer/Counter when matching */
S32 TC_DisableMatchStop (S32 TCdev, U32 matchsource);

/* Read Pending Match Interrupt in Timer/Counter device */
U32 TC_ReadIntPending (S32 TCdev);

/* Clear pending Timer/Counter match interrupt flag */
S32 TC_ClrMatchIntPending (S32 TCdev, U32 matchsource);

/* Enable Timer/Counter capture interrupt */
S32 TC_EnableCapInt (S32 TCdev, U32 capsource);

/* Disable Timer/Counter capture interrupt */
S32 TC_DisableCapInt (S32 TCdev, U32 capsource);

/* Configure capture mode for each capture source */
S32 TC_ConfigCapture (S32 TCdev, U32 capsource, S32 capmode);

/* Clear pending Timer/Counter capture interrupt flag */
S32 TC_ClrCapIntPending (S32 TCdev, U32 capsource);

/* Read value of capture register in timer/counter device */
U32 TC_ReadCapture (S32 TCdev, U32 capsource);

/***********************************************************************
 * Other MSTIMER driver functions
 **********************************************************************/

/* Delay for msec milliseconds (minimum) */
void TC_Waitms (S32 TCdev, U32 msec);

/* Delay for usec microseconds (minimum) */
void TC_Waitus (S32 TCdev, U32 usec);

#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_TC_DRIVER_H_*/
