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
#include <nxp/iolpc2378.h>
#include "type.h"
#include "irq.h"
#include "target.h"
#include "timer.h"
#include "fio.h"
#include "board.h"

extern volatile DWORD timer_counter;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  TargetResetInit();

  /********* The main Function is an endless loop ***********/
  init_timer( TIME_INTERVAL );

  /* GPIOInit() need to be carefully called if you want to run some
  other peripherals, it will set the PINSEL to default value and change
  the direction of IOs. */

  /* Initialize port for LCD Backlight LED P1.13 */
  GPIOInit( 0, FAST_PORT, DIR_OUT, USB_LINK_LED_MASK);

  /* Initialize timer for GPIO toggling timing */
  enable_timer( 0 );

  while ( 1 )
  {
    if ( timer_counter >= 0x20 )
    {

      USB_LINK_LED_FIO ^= USB_LINK_LED_MASK;

      timer_counter = 0;
    }
  }

  return 0;
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/
