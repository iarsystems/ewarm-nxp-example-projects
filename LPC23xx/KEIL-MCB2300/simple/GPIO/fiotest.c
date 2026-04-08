/*****************************************************************************
 *   fiotest.c:  GPIO test main C entry file for NXP LPC23xx/24xx Family
 *    Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.14  ver 1.00    Prelimnary version, first Release
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
#include "LPC230x.h"                        /* LPC2xxx definitions */
#include "type.h"
#include "irq.h"
#include "target.h"
#include "timer.h"
#include "fio.h"

extern volatile DWORD timer_counter;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  DWORD counter;

  TargetResetInit();

  /********* The main Function is an endless loop ***********/
  init_timer( TIME_INTERVAL );

  /* GPIOInit() need to be carefully called if you want to run some
  other peripherals, it will set the PINSEL to default value and change
  the direction of IOs. */

  /* Initialize port for LED display, the LEDs are on Port 2.0~2.7 */
  GPIOInit( 2, FAST_PORT, DIR_OUT );

  /* Initialize timer for GPIO toggling timing */
  enable_timer( 0 );

  counter = 0;
  FIO2SET = 1 << counter;
  while ( 1 )
  {
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
*****************************************************************************/
