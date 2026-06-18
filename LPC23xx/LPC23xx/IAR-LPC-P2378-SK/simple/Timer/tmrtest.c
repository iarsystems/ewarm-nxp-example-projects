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
 *
 ******************************************************************************/
#include <nxp/iolpc2378.h>
#include "type.h"
#include "target.h"
#include "irq.h"
#include "timer.h"
#include "board.h"

extern volatile DWORD timer_counter;

/*****************************************************************************
**   Main Function  main()
*****************************************************************************/
int main (void)
{

  TargetResetInit();

  SCS_bit.GPIOM = 1;                      /* enable fast io for GPIO0,1 */
  USB_LINK_LED_FDIR = USB_LINK_LED_MASK;  /* USB Link LED port output */
  USB_LINK_LED_FSET = USB_LINK_LED_MASK;  /* turn off USB Link LED */

  init_timer( TIME_INTERVAL );
  enable_timer( 0 );

  while (1)
  {         /* Loop forever */
    if ( timer_counter >= 0x20)
    {
      USB_LINK_LED_FIO ^= USB_LINK_LED_MASK;
      timer_counter = 0;
    }
  }

  return 0;
}

/*****************************************************************************
**                            End Of File
******************************************************************************/
