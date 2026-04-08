/*****************************************************************************
 *   wdttest.c:  main C entry file for NXP LPC11xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.19  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC11xx.h"			/* LPC11xx Peripheral Registers */
#include "timer16.h"
#include "clkconfig.h"
#include "wdt.h"

extern volatile uint32_t timer16_0_counter;
volatile uint32_t feed_counter = 0;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  SystemInit();
  
  /* Config WDT clock */
  WDT_CLK_Setup(WDTCLK_SRC_MAIN_CLK);
   
  /*** The main Function is an endless loop ****/
  init_timer16( 0, TIME_INTERVAL );
    
  WDTInit();
  enable_timer16( 0 );

  while( 1 ) 
  {
	if ( timer16_0_counter > 0 )
	{
	  /* Feed the watchdog timer */
	  feed_counter++;
	  WDTFeed();
	  timer16_0_counter = 0;
	}
  }
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
