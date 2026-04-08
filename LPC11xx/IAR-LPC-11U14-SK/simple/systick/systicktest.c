/*****************************************************************************
 *   blinky.c:  SYSTICK C file for NXP LPC11uxx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC11uxx.h"                        /* LPC11uxx definitions */
#include "gpio.h"

#define SYSTICK_DELAY		5000

volatile uint32_t TimeTick = 0;

/* SysTick interrupt happens every 10 ms */
void SysTick_Handler(void)
{
  TimeTick++;
}

void delaySysTick(uint32_t tick)
{
  uint32_t timetick;

  /* Clear SysTick Counter */
  SysTick->VAL = 0;
  /* Enable the SysTick Counter */
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

  timetick = TimeTick;
  while ((TimeTick - timetick) < tick);

  /* Disable SysTick Counter */
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  /* Clear SysTick Counter */
  SysTick->VAL = 0;
  return;
}

/* Main Program */

int main (void) {
  SystemInit();

  /* Called for system library in core_cmx.h(x=0 or 3). */
  SysTick_Config( SYSTICK_DELAY );

  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

  /* Set port 1_15 to output */
  GPIOSetDir( 1, 15, 1 );

  while (1)                                /* Loop forever */
  {
	delaySysTick(SYSTICK_DELAY);
	GPIOSetBitValue( 1, 15, 0 );
	delaySysTick(SYSTICK_DELAY);
	GPIOSetBitValue( 1, 15, 1 );
  }
}

