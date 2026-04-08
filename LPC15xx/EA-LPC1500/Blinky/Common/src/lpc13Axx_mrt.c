/****************************************************************************
 *   $Id: lpc13Axx_mrt.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP lpc13Axx multi-rate timer(MRT) example
 *
 *   Description:
 *     This file contains MRT timer code example which include timer 
 *     initialization, timer interrupt handler, and related APIs for 
 *     timer setup.
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
#include "LPC13Axx.h"
#include "lpc13Axx_nmi.h"
#include "lpc13Axx_mrt.h"

volatile uint32_t mrt_counter[NUM_MRT_CHANNELS] = {0,0,0,0};
volatile uint32_t isrFlag = 0, irqNum;

/*****************************************************************************
** Function name:		delayMs
**
** Descriptions:		Start the timer delay in milo seconds until elapsed
**
** parameters:			timer number, Delay value in milo second			 
** 						
** Returned value:	None
** 
*****************************************************************************/
void delayMs(uint32_t delayInMs)
{
  /* wait until delay time has elapsed */
	LPC_MRT->Channel[0].INTVAL = delayInMs;
  LPC_MRT->Channel[0].INTVAL |= MRT_INTERVAL_LOAD;
  while (LPC_MRT->Channel[0].TIMER);
  return;
}

/******************************************************************************
** Function name:		MRT_IRQHandler
**
** Descriptions:		MRT interrupt handler
**
** parameters:			None
** Returned value:	None
** 
******************************************************************************/
void MRT_IRQHandler(void)
{  
	int i;
	isrFlag = 1;

	irqNum = LPC_MRT->IRQ_FLAG;
	
	for (i = 0; i < NUM_MRT_CHANNELS; i++)
	{
		if ( LPC_MRT->Channel[i].STAT & MRT_STAT_IRQ_FLAG )
		{
			LPC_MRT->Channel[i].STAT = MRT_STAT_IRQ_FLAG;			/* clear interrupt flag */
			mrt_counter[i]++;
		}
	}
  return;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**									install timer interrupt handler
**
** parameters:			channel num, mode, timer interval
** Returned value:	None
** 
******************************************************************************/
void init_mrt(uint32_t ch_num, uint32_t mode, uint32_t TimerInterval) 
{
  /* Enable clock to MRT and reset the MRT peripheral */
  LPC_SYSCON->SYSAHBCLKCTRL1 |= EN1_MRT;
	LPC_SYSCON->PRESETCTRL1 |= EN1_MRT;
	LPC_SYSCON->PRESETCTRL1 &= ~EN1_MRT;
	
	NVIC_DisableIRQ(MRT_IRQn);
	mrt_counter[ch_num] = 0;
	irqNum = 0;
	
  LPC_MRT->Channel[ch_num].INTVAL = TimerInterval;
  LPC_MRT->Channel[ch_num].INTVAL |= MRT_INTERVAL_LOAD;
	
	LPC_MRT->Channel[ch_num].CTRL = mode;
  return;
}

/******************************************************************************
** Function name:		enable_mrt_timer
**
** Descriptions:		Initialize timer interrupt
**
** parameters:			channel num
** Returned value:	None
** 
******************************************************************************/
void enable_mrt_timer( uint32_t ch_num )
{
	LPC_MRT->Channel[ch_num].CTRL |= MRT_INT_ENA;
	/* Enable the MRT Interrupt */
  NVIC_EnableIRQ(MRT_IRQn);
}

/******************************************************************************
** Function name:		enable_mrt_timer
**
** Descriptions:		Initialize timer interrupt
**
** parameters:			channel num
** Returned value:	None
** 
******************************************************************************/
void disable_mrt_timer( uint32_t ch_num )
{
	LPC_MRT->Channel[ch_num].CTRL &= ~MRT_INT_ENA;
	NVIC_DisableIRQ(MRT_IRQn);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
