/*****************************************************************************
 *   wdt.c:  Watchdog C file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.07.20  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "irq.h"
#include "target.h"
#include "timer.h"
#include "wdt.h"

volatile DWORD wdt_counter;

/*****************************************************************************
** Function name:		WDTHandler
**
** Descriptions:		Watchdog timer interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void WDTHandler(void)
{
  DWORD regVal;

  wdt_counter++;
  regVal = WDT_INT_STATUS;
  WDT_INT_CLR_STATUS = regVal;
  return;
}

/*****************************************************************************
** Function name:		WDTInit
**
** Descriptions:		Initialize watchdog timer, install the
**				watchdog timer interrupt handler
**
** parameters:			None
** Returned value:		true or false, return false if the VIC table
**				is full and WDT interrupt handler can be
**				installed.
** 
*****************************************************************************/
void WDTInit( void )
{
  DWORD regVal;

  wdt_counter = 0;

#if WDT_DEBUG_ENABLED
  regVal = WDT_WD_KEY;
  if ( regVal == WDT_KEY )
  {
	WDT_DEBUG = regVal | 0x01;	/* disable Reset on WDT timeout  */
	WDT_TIMEOUT = regVal;		/* Now, it's unlocked. */
  }  
#else  
  regVal = WDT_WD_KEY;
  if ( regVal == WDT_KEY )
  {
	WDT_TIMEOUT = regVal;		/* Now, it's unlocked. */
  }  
#endif
  WDT_TIMEOUT = WDT_TO_VALUE;	/* Now, it's locked again. */
  WDT_WTCR = WDT_KEY | WDEN;	/* Now, WDT timer starts */

  install_irq( WDT_INT, (void *)WDTHandler, HIGHEST_PRIORITY );
  EnableIntReq_IRQ( WDT_INT, ACTIVE_HIGH, HIGHEST_PRIORITY );
  WDT_INT_SET_ENABLE = 1<<8;	/* Enable interrupt on WDT */    
  return;
}

/*****************************************************************************
** Function name:		WDTFeed
**
** Descriptions:		Feed watchdog timer to prevent it from timeout
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void WDTFeed( void )
{
  WDT_WD_KEY = WDT_KEY;
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
