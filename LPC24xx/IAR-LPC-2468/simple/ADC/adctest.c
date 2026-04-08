/*****************************************************************************
 *   adctest.c:  main C entry file for NXP LPC23xx/LPC24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.08.16  ver 1.00    Prelimnary version, first Release
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
#include "adc.h"
#include "board.h"

extern volatile DWORD ADC0Value[];
extern volatile DWORD ADC0IntDone;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
    TargetResetInit();

    /* Initialize ADC  */
    ADCInit( ADC_CLK );

#if ADC_INTERRUPT_FLAG      /* Interrupt driven */
    ADC0Read( ANALOG_TRIM_CHANNEL );
    while ( !ADC0IntDone );
#else           /* Polling */
    ADC0Value[ANALOG_TRIM_CHANNEL] = ADC0Read( ANALOG_TRIM_CHANNEL );
#endif
    return 0;
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/
