/*****************************************************************************
 *   pmtest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
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
#include "irq.h"
#include "target.h"
#include "timer.h"
#include "pwrman.h"
#include "board.h"

extern volatile DWORD timer_counter;

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{

  TargetResetInit();

  SCS_bit.GPIOM = 1;                      /* enable fast io for GPIO0,1 */
  USB_LINK_LED_FDIR = USB_LINK_LED_MASK;  /* USB Link LED port output */
  USB_LINK_LED_FSET = USB_LINK_LED_MASK;  /* turn off USB Link LED */

  init_timer( TIME_INTERVAL );
  enable_timer( 0 );

  if ( PM_Init( ) != TRUE )
  {
    while( 1 );     /* fatal error */
  }

  /* After setting up the wake up source, go to power down mode,
  when the push B1 on GPIOWAKE is pressed, the system will wake
  up, Reset_MainClock() will be the first routine to be executed,
  the PLL will be reconfigured, finally, USB Link LED is blinking
  happily. */
  PCON = 0x2;

  ConfigurePLL();

  timer_counter = 0;
  while ( 1 )
  {
    if ( timer_counter >= 0x20)
    {
      USB_LINK_LED_FIO ^= USB_LINK_LED_MASK;
      timer_counter = 0;
    }
  }
  return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
