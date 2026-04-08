/*****************************************************************************
 *   wdttest.c:  main C entry file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.07.19  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "target.h"
#include "irq.h"
#include "timer.h"
#include "wdt.h"
#include <intrinsics.h> /* for interrupts iar */

extern volatile DWORD timer_counter;
volatile DWORD feed_counter = 0;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{ 	    		
  /*** The main Function is an endless loop ****/
  init_timer( TIMER_TICKS );
  WDTInit();
  enable_timer();

  __enable_interrupt(); // iar

while( 1 ) 
  {
	if ( timer_counter > 0 )
	{
	  /* Feed the watchdog timer */
	  feed_counter++;
	  WDTFeed();
	  timer_counter = 0;
	}
  }
  return 0;
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
