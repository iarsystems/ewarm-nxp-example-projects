/***********************************************************************
 * $Id: lpc23xx_pwm_driver.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx PWM driver
 *
 * Description:
 *     This file contains driver support for the LPC2xx PWM.
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
 *********************************************************************/

#ifndef LPC23XX_PWM_DRIVER_H
#define LPC23XX_PWM_DRIVER_H

#include "lpc23xx_pwm.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * PWM device configuration commands
 **********************************************************************/

/* PWM waveform duty-cycle control options */
typedef enum {
    PWM_WAV_DUTY_ABS,   /* absolute register value */
    PWM_WAV_DUTY_PER    /* duty-cycle in % of the PWM period */
} PWM_IOCTL_DUTY_OPT;

/* PWM waveform offset control options */
typedef enum {
    PWM_WAV_OFFSET_ABS, /* absolute register value */
    PWM_WAV_OFFSET_PER  /* offset % of the PWM period */
} PWM_IOCTL_OFFSET_OPT;

/* PWM operating mode options */
typedef enum {
    PWM_SINGLE_EDGE,	/* Single edge */
    PWM_DUAL_EDGE		/* Dual edge */
} PWM_MODE_OPT;

/* PWM operating mode options */
typedef enum {
    PWM_CAPTURE_RISING = 0, /* Rising edge capture mode */
    PWM_CAPTURE_FALLING,	/* Falling edge capture mode */
    PWM_CAPTURE_ANY			/* Both rising and falling mode */
} PWM_ICAP_MODE_OPT;

/* PMW TC mode select option */
typedef enum {
	PWM_TC_MODE_TIMER = 0,		/* Timer mode  for TC of PWM */	
	PWM_TC_MODE_COUNTER,		/* Counter mode  for TC of PWM */
} PWM_TC_MODE_OPT;

/* PWM Input capture mode option */
typedef enum {
	PWM_ICAP_PCAP10 = 0,		/*  Input capture selected from PCAP1.0 pin */
	PWM_ICAP_CAP11				/*  Input capture selected from CAP1.1 pin */
} PWM_ICAP_OPT;

/***********************************************************************
 * PWM driver API functions
 **********************************************************************/

/* Set channel with duty, offset and mode
 * The duty/offset calculated in percentage
 * example: 
 * pwm_set_channel(1, PWM_SINGLE_EDGE, 30, 30);
 * It mean, set channel 1 for single mode, and duty is 30%. not care a bout offset.
 * pwm_set_channel(2, PWM_DUAL_EDGE, 30, 30);
 * It mean, set channel 1/2 for dual edge mode and the duty is 30%, with rising 
 * start at 30% 
 */
/* Init PWM controller */
S32 PWM_Init (S32 PWMNum);

/* Close PWM device */
S32 PWM_Close (S32 PWMdev);

/* Enable PWM activity */
S32 PWM_Enable (S32 PWMdev);

/* Disable PWM module */
S32 PWM_Disable (S32 PWMdev);

/* Start timer/counter in PWM module */
S32 PWM_StartTC (S32 PWMdev);

/* Stop timer/counter in PWM module */
S32 PWM_StopTC (S32 PWMdev);

/* Reset counter/timer in PWM module */
S32 PWM_ResetTC (S32 PWMdev);

/* Setup PWM module system */
S32 PWM_Setup (S32 PWMdev, U32 prescale, U32 period);

/* Set TC mode for PWM */
S32 PWM_SetTCMode (S32 PWMdev, S32 TCMode, S32 ICAPsource, S32 ICAPMode);

/* Update PWM system after re-configure */
S32 PWM_Update (S32 PWMdev);

/* Update PWM system immediately after re-configure */
S32 PWM_UpdateNow (S32 PWMdev);

/* Configure PWM channel */
S32 PWM_ConfigChannel (S32 PWMdev, U32 channel, S32 mode, 
						S32 duty_option, U32 duty, S32 offset_option, U32 offset);

/* Enable PWM channel */
S32 PWM_EnableChannel (S32 PWMdev, U32 channel);

/* Disable PWM channel */
S32 PWM_DisableChannel (S32 PWMdev, U32 channel);

/* Update PWM channel after re-configure */
S32 PWM_UpdateChannel (S32 PWMdev, U32 channel);

/* Enable PWM match interrupt */
S32 PWM_EnableMatchInt (S32 PWMdev, U32 matchsource);

/* Disable PWM match interrupt */
S32 PWM_DisableMatchInt (S32 PWMdev, U32 matchsource);

/* Enable Reset TC when matching */
S32 PWM_EnableMatchResetTC (S32 PWMdev, U32 matchsource);

/* Disable Reset TC when matching */
S32 PWM_DisableMatchResetTC (S32 PWMdev, U32 matchsource);

/* Enable Stop TC when matching */
S32 PWM_EnableMatchStopTC (S32 PWMdev, U32 matchsource);

/* Disable Stop TC when matching */
S32 PWM_DisableMatchStopTC (S32 PWMdev, U32 matchsource);

/* Read Pending Match Interrupt */
U32 PWM_ReadMatchIntPending (S32 PWMdev);

/* Clear pending PWM match interrupt flag */
S32 PWM_ClrMatchIntPending (S32 PWMdev, U32 matchsource);

/* Enable PWM capture interrupt */
S32 PWM_EnableCapInt (S32 PWMdev, U32 capsource);

/* Disable PWM capture interrupt */
S32 PWM_DisableCapInt (S32 PWMdev, U32 capsource);

/* Clear pending PWM capture interrupt flag */
S32 PWM_ClrCapIntPending (S32 PWMdev, U32 capsource);


#ifdef __cplusplus
}
#endif

#endif /* LPC23XX_PWM_DRIVER_H */
