/***********************************************************************
 * $Id: lpc23xx_rtc_driver.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx RTC driver
 *
 * Description:
 *     This file contains driver support for the LPC23xx RTC.
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
#ifndef LPC23XX_RTC_DRIVER_H_
#define LPC23XX_RTC_DRIVER_H_

#include "lpc23xx_rtc.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Public RTC enumeration
 **********************************************************************/

/* RTC clock source option */
typedef enum
{
	RTC_CLKSRC_EXT32K = 0,		/* Clock source from external 32 kHz oscillator */
	RTC_CLKSRC_SYSCLK			/* Clock source from system clock through prescale */
} RTC_CLKSRC_OPT;

/* RTC time type option */
typedef enum
{
	RTC_TIMETYPE_SECOND = 0,	/* Second */
	RTC_TIMETYPE_MINUTE,		/* Month */
	RTC_TIMETYPE_HOUR,			/* Hour */
	RTC_TIMETYPE_DAYOFWEEK,		/* Day of week */
	RTC_TIMETYPE_DAYOFMONTH,	/* Day of month */
	RTC_TIMETYPE_DAYOFYEAR,		/* Day of year */
	RTC_TIMETYPE_MONTH,			/* Month */
	RTC_TIMETYPE_YEAR			/* Year */
} RTC_TIMETYPE_OPT;


/***********************************************************************
 * Public RTC API functions
 **********************************************************************/

/* Initilize RTC device */
S32 RTC_Init (void);

/* Close RTC device */
S32 RTC_Close (S32 RTCdev);

/* Set Clock source for RTC device */
S32 RTC_SetClockSource (S32 RTCdev, S32 clksrc);

/* Reset clock tick counter in RTC device */
S32 RTC_Reset (S32 RTCdev);

/* Start RTC device */
S32 RTC_Start (S32 RTCdev);

/* Stop RTC device */
S32 RTC_Stop (S32 RTCdev);

/* Get tick counter in RTC */
U32 RTC_GetTickCnt (S32 RTCdev);

/* Enable Counter increment interrupt flags bit */
S32 RTC_EnableCII (S32 RTCdev, S32 Timetype);

/* Disable Counter increment interrupt flags bit */
S32 RTC_DisableCII (S32 RTCdev, S32 Timetype);

/* Enable Alarm interrupt flag */
S32 RTC_EnableALI (S32 RTCdev, S32 Timetype);

/* Disable Alarm interrupt flag */
S32 RTC_DisableALI (S32 RTCdev, S32 Timetype);

/* Enable SubSecSelSub-Second Select for interrupt */
S32 RTC_EnableSubSecSel (S32 RTCdev, U32 SSStype);

/* Disable SubSecSelSub-Second Select for interrupt */
S32 RTC_DisableSubSecSel (S32 RTCdev);

/* Set current time value for each type register */
S32 RTC_SetTime (S32 RTCdev, S32 Timetype, U32 TimeValue);

/* Get current time value for each type register */
U32 RTC_GetTime (S32 RTCdev, S32 Timetype);

/* Get full of time */
S32 RTC_GetFullTime (S32 RTCdev, RTC_TIME_Type *pFullTime);

/* Set alarm time value for each type register */
S32 RTC_SetALTime (S32 RTCdev, S32 Timetype, U32 ALValue);

/* Set alarm time value for each type register */
U32 RTC_GetALTime (S32 RTCdev, S32 Timetype);

/* Read the IRL register */
U32 RTC_ReadIntPending (S32 RTCdev);

/* Clear the IRL register */
S32 RTC_ClrIntPending (S32 RTCdev, U32 IntSrc);


#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_RTC_DRIVER_H_*/
