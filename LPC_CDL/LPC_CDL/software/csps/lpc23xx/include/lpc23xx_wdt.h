/***********************************************************************
* $Id: lpc23xx_wdt.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx Watchdog timer definitions
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: Watchdog Timer
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
#ifndef LPC23XX_WDT_H_
#define LPC23XX_WDT_H_


#include "arm7tdmis_types.h"
#include "lpc23xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
* WDT register structures
**********************************************************************/
typedef struct 
{
	__IO U32 MOD;			/* Watchdog mode register */
	__IO U32 TC;			/* Watchdog timer constant register */
	__IO U32 FEED;			/* Watchdog feed sequence register */
	__IO U32 TV;			/* Watchdog timer value register */
	__IO U32 CLKSEL;		/* Watchdog clock source selection register */
} WDT_Type;

/**********************************************************************
* WDMOD register definitions
**********************************************************************/
/* WDT interrupt enable bit */
#define WDT_ENABLE			_BIT(0)
/* WDT interrupt enable bit */
#define WDT_RESET			_BIT(1)
/* WDT time out flag bit */
#define WDT_TOF_FLAG		_BIT(2)
/* WDT Time Out flag bit */
#define WDT_INT_FLAG		_BIT(3)

/**********************************************************************
* WDCLKSEL register definitions
**********************************************************************/
/* Clock selected from internal RC */
#define WDT_CLKSEL_IRC		0x00
/* Clock selected from PCLK */
#define WDT_CLKSEL_PCLK		0x01
/* Clock selected from external RTC */
#define WDT_CLKSEL_RTC		0x02


/* Macro pointing to WDT registers */
#define WDT ((WDT_Type *)(WDT_BASE))

#ifdef __cplusplus
}
#endif


#endif /*LPC23XX_WDT_H_*/
