/*****************************************************************************
 *   adctest.c:  main C entry file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.08.16  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "target.h"
#include "irq.h"
#include "adc.h"
#include "mtimer.h"
#include <intrinsics.h> /* for interrupts iar */


extern volatile DWORD ADC1Value[ITERATION][ADC_NUM];
extern volatile DWORD ADC1ValueAverage[ADC_NUM];
extern volatile DWORD ADC1IntDone;
extern volatile DWORD ADC1Iteration;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  DWORD i;

  /* Initialize ADC  */
  ADC1_Init();
  ADC1_Start();

  /* Enable MSCSS timer for ADC event trigger, match0 for ADC0 */
  init_mtimer0(TIMER_TICKS);
  enable_mtimer0();

 __enable_interrupt(); // iar
 
  ADC1Iteration = 0;
  while ( 1 )
  {
	if ( ADC1IntDone )
	{
	  for ( i = 0; i < ADC_NUM; i++ )
	  {
		ADC1ValueAverage[i] += ADC1Value[ADC1Iteration-1][i];
	  }
	  if ( ADC1Iteration == ITERATION )
	  {
		ADC1IntDone = 0;	/* complete ITERATION, calculate average */
		break;
	  }
	  ADC1_Start();
	  ADC1IntDone = 0;
	}
  }

  for ( i = 0; i < ADC_NUM; i++ )
  {
	ADC1ValueAverage[i] /= ITERATION;
  }
  return 0;
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/

