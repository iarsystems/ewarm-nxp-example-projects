/*****************************************************************************
 *   tmrtest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *  If Port 2 bits 0 - 9 are used by application PINSEL10 must be cleared and
 * this will disable trace port too.
 *
 ******************************************************************************/
#include "LPC230x.H"                        /* LPC23xx/24xx definitions */
#include "type.h"
#include "target.h"
#include "irq.h"
#include "timer.h"

extern volatile DWORD timer_counter;

/*****************************************************************************
**   Main Function  main()
*****************************************************************************/
int main (void)
{	    		
DWORD counter = 0;

  TargetResetInit();

  FIO2DIR = 0x000000FF;		/* P1.16..23 defined as Outputs */
  FIO2CLR = 0x000000FF;		/* turn off all the LEDs */

  init_timer( TIME_INTERVAL );
  enable_timer( 0 );

  while (1)
  {					/* Loop forever */
    if ( timer_counter >= (0x20 * counter) )
    {
      FIO2SET = 1 << counter;
      counter++;
      if ( counter > 8 )
      {
        counter = 0;	
        timer_counter = 0;
        FIO2CLR = 0x000000FF;
      }
    }
  }

  return 0;
}

/*****************************************************************************
**                            End Of File
******************************************************************************/
