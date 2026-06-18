/****************************************************************************
 *   $Id: lpc13Axx_rtc.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP lpc13Axx alarm/wake timer example
 *
 *   Description:
 *     This file contains alarm/wake timer code example which include timer 
 *     initialization, timer interrupt handler, and related APIs for 
 *     timer setup.
 *
 ****************************************************************************
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
****************************************************************************/
#include "lpc13Axx.h"
#include "lpc13Axx_rtc.h"

uint32_t alarm_flag_count = 0;
uint32_t wakeup_flag_count = 0;

/******************************************************************************
** Function name:		RTC_IRQHandler
**
** Descriptions:		alarm/wake timer interrupt handler
**
** parameters:			None
** Returned value:	None
** 
******************************************************************************/
void RTC_IRQHandler(void)
{
  if ( LPC_RTC->CTRL & RTC_ALARMFLAG )
  {
		alarm_flag_count++;
		LPC_RTC->CTRL |= RTC_ALARMFLAG;			/* clear interrupt flag */
  }
	 if ( LPC_RTC->CTRL & RTC_WAKEUPFLAG )
  {
		wakeup_flag_count++;
		LPC_RTC->CTRL |= RTC_WAKEUPFLAG;		/* clear interrupt flag */
  }
  return;
}

/******************************************************************************
** Function name:		init_rtc
**
** Descriptions:		Initialize RTC timer, select clock source, set timer interval,
**									install timer interrupt handler
**
** parameters:			clock source(0 is 1Hz Alarm, 1 is 1KHz Wakeup) and timer interval
** Returned value:	None
** 
******************************************************************************/
void init_rtc(uint32_t wakeup_ena, uint32_t timerInterval) 
{
  LPC_SYSCON->SYSAHBCLKCTRL0 |= EN0_RTC;
	LPC_SYSCON->PRESETCTRL0 |= EN0_RTC;

	if ( LPC_RTC->CTRL & RTC_SW_RESET )
	{
    LPC_RTC->CTRL &= ~RTC_SW_RESET;
	}
	/* If RTC OSC is not detected, it'll spin here forever. */
	while ( LPC_RTC->CTRL & RTC_OSC_FAIL );
	
	if ( wakeup_ena == 0 )
	{
    LPC_RTC->CTRL &= ~RTC_1KHZ_ENA;
    LPC_RTC->MATCH = timerInterval;
	}
	else
	{
    LPC_RTC->CTRL |= RTC_1KHZ_ENA;
    LPC_RTC->WAKE = timerInterval;
	}
	
  /* Enable the RTC Interrupt */
  NVIC_EnableIRQ(RTCALARM_IRQn);
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
