/*****************************************************************************
 *   pwmtest.c:  main C entry file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "irq.h"
#include "pwm.h"
#include "mtimer.h"
#include <intrinsics.h> /* for interrupts iar */

extern volatile DWORD PWMFlag;

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{	    
  DWORD cycle = PWM_CYCLE, offset = 0;

  /* Enable MSCSS timer0(m3) for PWM0 shadow register update */
  init_mtimer0(TIMER_MS_TICKS);
  PWM0_Init( cycle );
  PWM0_Start();
  
  /* Enable PWM interrupt */
  PWM0_INT_SET_ENABLE = UD_INT | TD_INT;

  __enable_interrupt(); // iar
  
  enable_mtimer0();

  
  while ( 1 )
  {
	if ( (PWMFlag & UD_INT) && (PWMFlag & TD_INT) )
	{
	  PWMFlag = 0;		/* Reset interrupt flag */
	  if( offset <= PWM_CYCLE )
	  {
		offset += PWM_OFFSET;
	  }
	  else
	  {
		offset = 0;
	  }
	  PWM0_Set( cycle, offset );
	}
  }
  PWM0_Stop();
  return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
