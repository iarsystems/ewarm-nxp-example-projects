/*****************************************************************************
 *   blinky.c:  LED blinky C file for NXP LPC11Uxx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC11uxx.h"         /* LPC11Uxx definitions */
#include "timer16.h"
#include "clkconfig.h"
#include "gpio.h"

#define TEST_TIMER_NUM		0		/* 0 or 1 for 16-bit timers only */

extern volatile uint32_t timer16_0_counter;
extern volatile uint32_t timer16_1_counter;

/* Main Program */

int main (void) {
  uint32_t interval;

  SystemInit();

  /* Config CLKOUT, mostly used for debugging. */
  CLKOUT_Setup( CLKOUTCLK_SRC_MAIN_CLK );
  LPC_IOCON->PIO0_1 &= ~0x07;	
  LPC_IOCON->PIO0_1 |=  0x01;		/* CLK OUT */

  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

  /* TEST_TIMER_NUM is either 0 or 1 for 16-bit timer 0 or 1. */
  interval = SystemCoreClock/1000 - 1;
  if ( interval > 0xFFFF )
  {
	interval = 0xFFFF;
  }
  init_timer16(TEST_TIMER_NUM, interval);
  enable_timer16(TEST_TIMER_NUM);

  /* Set port 1_15 to output */
  GPIOSetDir( 1, 15, 1 );

  while (1)                                /* Loop forever */
  {
#if TEST_TIMER_NUM
	/* I/O configuration and LED setting pending. */
	if ( (timer16_1_counter > 0) && (timer16_1_counter <= 200) )
	{
	  GPIOSetBitValue( 1, 15, 0 );
	}
	if ( (timer16_1_counter > 200) && (timer16_1_counter <= 400) )
	{
	  GPIOSetBitValue( 1, 15, 1 );
	}
	else if ( timer16_1_counter > 400 )
	{
	  timer16_1_counter = 0;
	}
#else
	/* I/O configuration and LED setting pending. */
	if ( (timer16_0_counter > 0) && (timer16_0_counter <= 200) )
	{
	  GPIOSetBitValue( 1, 15, 0 );
	}
	if ( (timer16_0_counter > 200) && (timer16_0_counter <= 400) )
	{
	  GPIOSetBitValue( 1, 15, 1 );
	}
	else if ( timer16_0_counter > 400 )
	{
	  timer16_0_counter = 0;
	}
#endif
  }
}
