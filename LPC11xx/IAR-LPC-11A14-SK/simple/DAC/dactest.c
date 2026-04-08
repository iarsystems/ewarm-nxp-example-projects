/****************************************************************************
 *   $Id: dactest.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC11Axx DAC example
 *
 *   Description:
 *     This file contains DAC test modules, main entry, to test DAC APIs.
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
 * documentation is hereby granted, under NXP Semiconductors?
 * relevant copyright in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.

 ****************************************************************************/

#include "LPC11Axx.h"                        /* LPC11Axx definitions */
#include "type.h"
#include "dac.h"

/*****************************************************************************
 **   Main Function  main()
 ******************************************************************************/
int main (void)
{
#if !ATRG_ENABLED
  uint32_t i = 0, m;
  #if sine_wave
     uint32_t sinewave[72]= /* Sine wave co-effs. Generates a Sine wave of max 69Hz */
     {30000,
      32179,
      34341,
      36470,
      38551,
      40565,
      42500,
      44339,
      46070,
      47678,
      49151,
      50479,
      51651,
      52658,
      53492,
      54148,
      54620,
      54905,
      55000,
      54905,
      54620,
      54148,
      53492,
      52658,
      51651,
      50479,
      49151,
      47678,
      46070,
      44339,
      42500,
      40565,
      38551,
      36470,
      34341,
      32179,
      30000,
      27821,
      25659,
      23530,
      21449,
      19435,
      17500,
      15661,
      13930,
      12322,
      10849,
      9521,
      8349,
      7342,
      6508,
      5852,
      5380,
      5095,
      5000,
      5095,
      5380,
      5852,
      6508,
      7342,
      8349,
      9521,
      10849,
      12322,
      13930,
      15661,
      17500,
      19435,
      21449,
      23530,
      25659,
      27821};
  #endif
#endif

  SystemInit();

  /* Initialize DAC  */
  DACInit();

  while ( 1 )
  {

#if !ATRG_ENABLED
#if sine_wave /* generates a sine wave */
    LPC_DAC->CR = ((sinewave[i]/80) << 6 )| DAC_BIAS;
    i++;
    for(m = 1000; m > 1; m--);
    if(i > 71)
    {
      i = 0;
    }
#else /* else generate a saw tooth */
    LPC_DAC->CR = (i << 6) | DAC_BIAS;
    i++;
    for(m = 1000; m > 1; m--);
    if ( i > MAX_DAC_VALUE )
    {
      i = 0;
    }
#endif

#else
    /* If it's triggered on ATRGx or timers, DAC output occurs inside DAC
  interrupt handler */
#endif
  }
}

/*****************************************************************************
 **                            End Of File
 *****************************************************************************/

