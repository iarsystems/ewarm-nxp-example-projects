/*****************************************************************************
 *   gpiotest.c:  main C entry file for NXP LPC11uxx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.19  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC11uxx.h"			/* LPC11uxx Peripheral Registers */
#include "gpio.h"

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  SystemInit();

  GPIOInit();

  /* use port0_7 as input event, interrupt test. */
  GPIOSetDir( PORT0, 7, 0 );
  /* port0_7 (USER Button), single trigger, active low. */
  GPIOSetFlexInterrupt(CHANNEL0, PORT0, 7, 0, 0 );

  /* use port0_16 as input event, interrupt test. */
  GPIOSetDir( PORT0, 16, 0 );
  /* port0_16 (WAKE-UP button), single edge trigger, active low. */
  GPIOSetFlexInterrupt(CHANNEL1, PORT0, 16, 0, 0 );

  while( 1 );
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
