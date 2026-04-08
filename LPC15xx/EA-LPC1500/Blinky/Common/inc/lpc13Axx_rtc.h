/****************************************************************************
 *   $Id: lpc13Axx_rtc.h 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC13Axx software example
 *
 *   Description:
 *     This file contains definition and prototype for alarm/wake timer 
 *     configuration.
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
#ifndef __RTC_H 
#define __RTC_H

#define WAKEUP_ENA					0

/* Control register bit definition. */
#define RTC_SW_RESET				(0x1<<0)
#define RTC_OSC_FAIL				(0x1<<1)
#define RTC_ALARMFLAG				(0x1<<2)
#define RTC_WAKEUPFLAG			(0x1<<3)
#define RTC_ALARM_DPD				(0x1<<4)
#define RTC_WAKEUP_DPD			(0x1<<5)
#define RTC_1KHZ_ENA				(0x1<<6)

void RTC_IRQHandler(void);
void init_rtc(uint32_t clkSrc, uint32_t timerInterval);

#endif /* end __RTC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
