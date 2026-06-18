/*****************************************************************************
 *   einttest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
 * Jumpers:
 *  EINT0 - filled
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
#include "irq.h"
#include "target.h"
#include "extint.h"
#include "board.h"

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
    TargetResetInit();

    SCS_bit.GPIOM = 1;                        /* enable fast io for GPIO0,1 */
    USB_LINK_LED1_FDIR = USB_LINK_LED1_MASK;  /* USB Link LED port output */
    USB_LINK_LED1_FSET = USB_LINK_LED1_MASK;  /* turn off USB Link LED */

    /* initialize GPIO pins as external interrupts */
    EINTInit();

    /****** It's an endless loop waiting for external interrupt ******/
    /* EINT3 can be used to test the external interrupt
      It's shared with GPIO0,2 interrupts */
    while( 1 );

    return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
