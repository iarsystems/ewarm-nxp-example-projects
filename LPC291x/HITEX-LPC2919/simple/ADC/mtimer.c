/*****************************************************************************
 *   mtimer.c: MSCSS Timer C file for Philips LPC29xx Family Microprocessors
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
#include "mtimer.h"


/* MSCSS timer and regular timer have different base address, they have 
identical registers setting. Each timer also has its own VIC entry.
The MSCSS timer interval is controlled by MSCSS_CLK_CONF while
the regular timer interval is controlled by TMR_CLK_CONF. Other than
that, the configuration of these timers are the same. */ 
  
volatile DWORD mtimer_counter = 0;

/******************************************************************************
** Function name:		MTimer0_Handler
**
** Descriptions:		MSCSS Timer/Counter0 interrupt handler
**
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void MTimer0_Handler( void )
{
  DWORD regVal;
   
  mtimer_counter++;		/* global timer counter */

  regVal = MTIM0_INT_STATUS;
  MTIM0_INT_CLR_STATUS = regVal;
  return;
}

/******************************************************************************
** Function name:		enable_mtimer
**
** Descriptions:		Enable MSCSS timer
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void enable_mtimer0( void )
{
  MTIM0_TCR = (1<<1);		/* Counter reset */
  MTIM0_TCR &= ~(1<<1);
  MTIM0_TCR |= (1<<0);		/* Enable counter */
  MTIM0_INT_SET_ENABLE = 0x01;
  return;
}

/******************************************************************************
** Function name:		disable_mtimer
**
** Descriptions:		Disable MSCSS timer
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void disable_mtimer0( void )
{
  MTIM0_TCR = 1<<1;			/* Counter reset */
  MTIM0_TCR &= ~(1<<1);
  MTIM0_TCR &= ~(1<<0);		/* Disable counter */
  MTIM0_INT_CLR_ENABLE = 0x01;
  return;
}

/******************************************************************************
** Function name:		init_mtimer
**
** Descriptions:		Initialize MSCSS timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void init_mtimer0( DWORD timer_interval ) 
{
  mtimer_counter = 0;

  MTIM0_TCR = 1<<1;			/* Reset counter */
  MTIM0_TCR &= ~(1<<1);
  
  install_irq( 14, (void *)MTimer0_Handler, HIGHEST_PRIORITY );
  EnableIntReq_IRQ( 14, ACTIVE_HIGH, HIGHEST_PRIORITY );

  /* divided by 80 to make it 1 second interval */
  /* please note, the PERIPH_CLK could be different for different
  peripherals, the UART clock is the same as SYS_CLK 80Mhz. */
  MTIM0_PR = (PERIPH_CLK / timer_interval) - 1;

  /* Reset counter if Match 1 counter equals to timer counter. */
  MTIM0_MCR = (0x01<<2);		/* For ADC1 on Match 1 */

  /* Set MATCHx register */
  MTIM0_MR1 = timer_interval/4 - 1;	/* For ADC1 */

  /* match 1 will generate logic 0 or 1. */
  MTIM0_EMR |= (0x02<<6);			/* For ADC1 */ 
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
