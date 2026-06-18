/*****************************************************************************
 *   wdttest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Prelimnary version, first Release
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 * The LEDs share pins with Trace port. If ETM is enabled the LEDs can not be used. 
 *
 ******************************************************************************/
#include <nxp/iolpc2468.h>
#include "type.h"
#include "target.h"
#include "irq.h"
#include "timer.h"
#include "wdt.h"

extern volatile DWORD timer_counter;
volatile DWORD feed_counter = 0;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  TargetResetInit();

  /*** The main Function is an endless loop ****/
  init_timer( TIME_INTERVAL );

  WDTInit();
  enable_timer( 0 );

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
