/***********************************************************************
 * $Id: lpc23xx_wdt_driver.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx WDT driver
 *
 * Description:
 *     This file contains driver support for the LPC23xx WDT.
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
#ifndef LPC23XX_WDT_DRIVER_H_
#define LPC23XX_WDT_DRIVER_H_


#include "lpc23xx_wdt.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Public WDT enumeration
 **********************************************************************/

/* Clock source option for WDT */
typedef enum
{
	WDT_CLKSRC_IRC = 0,		/* Clock source from Internal RC oscillator */
	WDT_CLKSRC_PCLK, 		/* Selects the APB peripheral clock (PCLK) */
	WDT_CLKSRC_RTC			/* Selects the RTC oscillator */
} WDT_CLK_OPT;

/* WDT operation mode */
typedef enum 
{
	WDT_MODE_INT_ONLY = 0,	/* Use WDT to generate interrupt only */
	WDT_MODE_RESET			/* Use WDT to generate interrupt and reset MCU */
} WDT_MODE_OPT;

/***********************************************************************
 * WDT driver API functions
 **********************************************************************/
/* Init the WDT */
S32 WDT_Init (S32 ClkSrc, S32 WDTMode, U32 TimeOut);

/* Start WDT by the first feeding */
S32 WDT_Start (void);

/* Reload WDT with current timeout value by feeding */ 
S32 WDT_ReloadTimeOut (void);

/* Update timeout value for WDT */
S32 WDT_UpdateTimeOut (U32 TimeOut);

/* Read Current Counter in WDT */
U32 WDT_ReadCnt (void);

/* Read pending flag */
U32 WDT_ReadFlagPending (void);

/* Clear TimeOut Flag */
void WDT_ClrTimeOutFlag (void);

#ifdef __cplusplus
}
#endif


#endif /*LPC23XX_WDT_DRIVER_H_*/
