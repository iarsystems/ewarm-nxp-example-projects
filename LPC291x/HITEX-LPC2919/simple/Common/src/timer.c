/*****************************************************************************
 *   timer.c: Timer C file for Philips LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"			/* LPC29xx Peripheral Registers	*/
#include "type.h"
#include "target.h"
#include "irq.h"
#include "timer.h"

volatile DWORD timer_counter = 0;

/******************************************************************************
** Function name:		delayMs
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						Delay X milo second based on the parameter
**
** parameters:			Delay interval
** Returned value:		None
** 
******************************************************************************/
void delayMs( DWORD Interval ) 
{
 /* Reset counter */
  TIM0_TCR = 1<<1;
  TIM0_TCR &= ~(1<<1);
  
  /* divided by 80 to make it 1 second interval */
  /* please note, the PERIPH_CLK could be different for different
  peripherals, the UART clock is the same as SYS_CLK 80Mhz. */
  TIM0_PR = (PERIPH_CLK / Interval) - 1;
  TIM0_TCR |= (1<<0);		/* Start timer */
  while ( TIM0_TC >= TIM0_PR );
  TIM0_TCR &= ~(1<<0);		/* Stop timer */
  return;
}

/******************************************************************************
** Function name:		Timer0Handler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER_ISR( void )
{
  DWORD regVal;
   
  timer_counter++;		/* global timer counter */

  regVal = TIM0_INT_STATUS;
  TIM0_INT_CLR_STATUS = regVal;
  return;
}

/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void enable_timer( void )
{
  TIM0_TCR = (1<<1);
  TIM0_TCR &= ~(1<<1);
  TIM0_TCR |= (1<<0);
  TIM0_INT_SET_ENABLE = 0x01;
  return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void disable_timer( void )
{
  TIM0_TCR = 1<<1;
  TIM0_TCR &= ~(1<<1);
  TIM0_TCR &= ~(1<<0);
  TIM0_INT_CLR_ENABLE = 0x01;
  return;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void init_timer( DWORD timer_interval ) 
{
  timer_counter = 0;

  /* Reset counter */
  TIM0_TCR = 1<<1;
  TIM0_TCR &= ~(1<<1);
  
  install_irq( 2, (void *)TIMER_ISR, 0x0F );
  EnableIntReq_IRQ( 2, ACTIVE_HIGH, 0x0F );
  /* divided by 80 to make it 1 second interval */
  /* please note, the PERIPH_CLK could be different for different
  peripherals, the UART clock is the same as SYS_CLK 80Mhz. */
  TIM0_PR = (PERIPH_CLK / timer_interval) - 1;

  /* Reset counter if Match 0 counter equals to timer counter. */
  TIM0_MCR = 0x01;

  /* Set MATCH0 register */
  TIM0_MR0 = timer_interval/4 - 1;
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
