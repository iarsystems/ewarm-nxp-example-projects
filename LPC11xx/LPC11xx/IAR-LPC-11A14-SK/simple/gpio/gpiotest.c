/****************************************************************************
 *   $Id: gpiotest.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC11Axx GPIO example
 *
 *   Description:
 *     This file contains GPIO test modules, main entry, to test GPIO APIs.
 *
****************************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.

* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors’
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.

****************************************************************************/

#include "LPC11Axx.h"     /* LPC11Axx Peripheral Registers */
#include "gpio.h"

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  SystemInit();

  GPIOInit();

  /* Route p0.4 to FLEX0, p1.0 to FLEX1, p0.16 to FLEX2,
  and P1.1 to FLEX3. */

  /* use port0_4 as input event, interrupt test. */
  GPIOSetDir( PORT0, 4, 0 );
  /* channel 0, port0_4, edge trigger, active high. */
  GPIOSetPinInterrupt( 0, 0, 4, 0, 1 );
  GPIOPinIntEnable( 0, 1 );

  /* use port1_0 as input event, interrupt test. */
  GPIOSetDir( PORT1, 0, 0 );
  /* channel 1, port1_0, edge trigger, active high. */
  GPIOSetPinInterrupt( 1, 1, 0, 0, 1 );
  GPIOPinIntEnable( 1, 1 );

  /* use port0_11 as input event, interrupt test. */
  GPIOSetDir( PORT0, 11, 0 );
  /* channel 2, port0_11, edge trigger, active high. */
  GPIOSetPinInterrupt( 2, 0, 11, 0, 1 );
  GPIOPinIntEnable( 2, 1 );

  /* use port1_1 as input event, interrupt test. */
  GPIOSetDir( PORT1, 1, 0 );
  /* channel 3, port1_1, edge trigger, active high. */
  GPIOSetPinInterrupt( 3, 1, 1, 0, 1 );
  GPIOPinIntEnable( 3, 1 );

  /* Route p0.17 to FLEX4, p1.2 to FLEX5, p0.23 to FLEX6,
  and P1.3 to FLEX7. */

  /* use port0_17 as input event, interrupt test. */
  GPIOSetDir( PORT0, 17, 0 );
  /* channel 4, port0_17, level trigger, active low. */
  GPIOSetPinInterrupt( 4, 0, 17, 1, 0 );
  GPIOPinIntEnable( 4, 0 );

  /* use port1_2 as input event, interrupt test. */
  GPIOSetDir( PORT1, 2, 0 );
  /* channel 5, port1_2, level trigger, active low. */
  GPIOSetPinInterrupt( 5, 1, 2, 1, 0 );
  GPIOPinIntEnable( 5, 0 );

  /* use port0_31 as input event, interrupt test. */
  GPIOSetDir( PORT0, 31, 0 );
  /* channel 6, port0_31, level trigger, active low. */
  GPIOSetPinInterrupt( 6, 0, 31, 1, 0 );
  GPIOPinIntEnable( 6, 0 );

  /* use port1_3 as input event, interrupt test. */
  GPIOSetDir( PORT1, 3, 0 );
  /* channel 7, port1_3, level trigger, active low. */
  GPIOSetPinInterrupt( 7, 1, 3, 1, 0 );
  GPIOPinIntEnable( 7, 1 );
  while( 1 );
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
