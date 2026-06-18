/****************************************************************************
 *   $Id: wdttest.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC11Axx WDT example
 *
 *   Description:
 *     This file contains WDT test modules, main entry, to test WDT APIs.
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

#include "LPC11Axx.h"
#include "timer32.h"
#include "clkconfig.h"
#include "wdt.h"
#include "gpio.h"

extern volatile uint32_t timer32_0_counter;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  SystemInit();

  /* Config WDT clock */
#if 1
  WDT_CLK_Setup(WDTCLK_SRC_WDT_OSC);
#else
  WDT_CLK_Setup(WDTCLK_SRC_IRC_OSC);
#endif

  GPIOInit();

  /*** The main Function is an endless loop ****/
  init_timer32( 0, TIME_INTERVAL );

  WDTInit();
  enable_timer32( 0 );

  /* Set PIO0_16 to output */
  GPIOSetDir( 0, 16, 1 );

  /* Set PIO0_17 to output */
  GPIOSetDir( 0, 17, 1 );

  GPIOSetBitValue(0,16,1); /*turn off LEDs*/
  GPIOSetBitValue(0,17,1);

  if ((LPC_SYSCON->SYSRSTSTAT & 0x4) == 0x4)
  {
    LPC_SYSCON->SYSRSTSTAT |= 0x4;
    while( 1 )
    {
      /* I/O configuration and LED setting pending. */
      if ( (timer32_0_counter > 0) && (timer32_0_counter <= 200) )
      {
        GPIOSetBitValue( 0, 16, 0 );
      }
      if ( (timer32_0_counter > 200) && (timer32_0_counter <= 400) )
      {
        GPIOSetBitValue( 0, 16, 1 );
      }
      else if ( timer32_0_counter > 400 )
      {
        timer32_0_counter = 0;
      }
    }
  }
  else
  {
    while( 1 )
    {
    /* I/O configuration and LED setting pending. */
      if ( (timer32_0_counter > 0) && (timer32_0_counter <= 200) )
      {
        GPIOSetBitValue( 0, 17, 0 );
      }
      if ( (timer32_0_counter > 200) && (timer32_0_counter <= 400) )
      {
        GPIOSetBitValue( 0, 17, 1 );
      }
      else if ( timer32_0_counter > 400 )
      {
        timer32_0_counter = 0;
      }
    }
  }
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
