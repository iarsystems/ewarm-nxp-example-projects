/****************************************************************************
 *   $Id: blinky.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC13Axx Blinky example
 *
 *   Description:
 *     This file contains LED blink code example which include timer,
 *     GPIO initialization, and clock monitoring.
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
****************************************************************************/
#include "LPC13Axx.h"                        /* LPC1xxx definitions */
#include "lpc13Axx_clkconfig.h"
#include "lpc13Axx_gpio.h"
#include "lpc13Axx_mrt.h"

#define TIME 1500
extern uint32_t mrt_counter[NUM_MRT_CHANNELS];
uint32_t freqMeasure = 0;

/* Main Program */

int main (void) {
  SystemCoreClockUpdate();
	GPIOInit();
	LPC_SWM->PINENABLE0 |= 0x08000000;
    /* Set port p0.16 to output */
  GPIOSetDir( 0, 16, 1 );
  GPIOSetDir( 0, 27, 1 );
	GPIOSetDir( 0, 30, 1 );
	/* Example use MRT channel 0 only */
	init_mrt(0, MRT_REPEATED_MODE, MRT_INTERVAL);
  enable_mrt_timer( 0 );

  while (1)                                /* Loop forever */
  {
		/* I/O configuration and LED setting pending. */
		if ( (mrt_counter[0] > 0) && (mrt_counter[0] <= 500) )
		{
			GPIOSetBitValue( 0, 16, 0 );
		}
		if ( (mrt_counter[0] > TIME) && (mrt_counter[0] <= 2*TIME) )
		{
			GPIOSetBitValue( 0, 16, 1 );
		}
		if ( (mrt_counter[0] > 2*TIME) && (mrt_counter[0] <= 3*TIME) )
		{
			 GPIOSetBitValue( 0, 27, 0 );
		}
		if ( (mrt_counter[0] > 3*TIME) && (mrt_counter[0] <= 4*TIME) )
		{
			 GPIOSetBitValue( 0, 27, 1 );
		}
		if ( (mrt_counter[0] > 4*TIME) && (mrt_counter[0] <= 5*TIME) )
		{
			 GPIOSetBitValue( 0, 30, 0 );
		}
		if ( (mrt_counter[0] > 5*TIME) && (mrt_counter[0] <= 6*TIME) )
		{
			 GPIOSetBitValue( 0, 30, 1 );
		}
		else if ( mrt_counter[0] > 6*TIME )
		{
			mrt_counter[0] = 0;
		}
  }
}
