/*****************************************************************************
 *   einttest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
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
#include "LPC230x.h"				/* LPC23xx/24xx definitions */
#include "type.h"
#include "irq.h"
#include "target.h"
#include "extint.h"

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{	    		
    TargetResetInit();

    /* initialize GPIO pins as external interrupts */
    EINTInit();

    /****** It's an endless loop waiting for external interrupt ******/
    /* EINT0 can be used to test the external interrupt */
    while( 1 );

    return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
