/*****************************************************************************
 *   tmr32test.c:  32-bit timer C file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "timer32.h"
#include "gpio.h"

#define TEST_TIMER_NUM		0		/* 0 or 1 for 32-bit timers only */

extern volatile uint32_t timer32_0_counter;
extern volatile uint32_t timer32_1_counter;

/* Main Program */

int main (void) {
  SystemInit();

  /* TEST_TIMER_NUM is either 0 or 1 for 32-bit timer 0 or 1. */
  init_timer32(TEST_TIMER_NUM, TIME_INTERVAL);
  enable_timer32(TEST_TIMER_NUM);

  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
  
  /* Set port 3_0 to output */
  GPIOSetDir( 3, 0, 1 );

  while (1)                                /* Loop forever */
  {
#if TEST_TIMER_NUM
	/* I/O configuration and LED setting pending. */
	if ( (timer32_1_counter > 0) && (timer32_1_counter <= 50) )
	{
	  GPIOSetValue( 3, 0, 0 );
	}
	if ( (timer32_1_counter > 50) && (timer32_1_counter <= 100) )
	{
	  GPIOSetValue( 3, 0, 1 );
	}
	else if ( timer32_1_counter > 100 )
	{
	  timer32_1_counter = 0;
	}
#else
	/* I/O configuration and LED setting pending. */
	if ( (timer32_0_counter > 0) && (timer32_0_counter <= 50) )
	{
	  GPIOSetValue( 3, 0, 0 );
	}
	if ( (timer32_0_counter > 50) && (timer32_0_counter <= 100) )
	{
	  GPIOSetValue( 3, 0, 1 );
	}
	else if ( timer32_0_counter > 100 )
	{
	  timer32_0_counter = 0;
	}
#endif
  }
}

