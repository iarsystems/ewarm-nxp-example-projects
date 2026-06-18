/****************************************************************************
 *   $Id: lpc13Axx_rit.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC13Axx RIT Timer example
 *
 *   Description:
 *     This file contains RIT Timer code example which include RIT 
 *     initialization, RIT interrupt handler, and APIs for RIT Timer.
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
#include "lpc13Axx.h"
#include "type.h"
#include "lpc13Axx_rit.h"

volatile uint32_t rit_timer_counter = 0;

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		RIT timer interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void RIT_IRQHandler (void) 
{
  if ( LPC_RIT->CTRL & CTRL_RITINT )
  {		
    LPC_RIT->CTRL |= CTRL_RITINT;		/* clear interrupt flag */
    rit_timer_counter++;
  }
  return;
}

/******************************************************************************
** Function name:		enable_rit_timer
**
** Descriptions:		Enable RIT timer
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void enable_rit_timer( void )
{
  LPC_RIT->CTRL |= (CTRL_RITENCLR | CTRL_RITEN);
  return;
}

/******************************************************************************
** Function name:		disable_rit_timer
**
** Descriptions:		Disable RIT timer
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void disable_rit_timer( void )
{
  LPC_RIT->CTRL &= ~(CTRL_RITEN);
  return;
}

/******************************************************************************
** Function name:		reset_rit_timer
**
** Descriptions:		Reset RIT timer
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void reset_rit_timer( void )
{
  LPC_RIT->CTRL |= CTRL_RITENCLR;
  return;
}

/******************************************************************************
** Function name:		init_rit_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**									install timer interrupt handler. If debugger_flag is 1
**									turn on timer for debugger.
**
** parameters:			debugger_flag, timer interval
** Returned value:		None
** 
******************************************************************************/
void init_rit_timer ( uint32_t debugger_flag, uint32_t TimerInterval ) 
{
	LPC_SYSCON->SYSAHBCLKCTRL1 |= EN1_RIT;
  LPC_SYSCON->PRESETCTRL1 |= EN1_RIT;
  LPC_SYSCON->PRESETCTRL1 &= ~EN1_RIT;

  disable_rit_timer();
	if ( debugger_flag == 1 )
    LPC_RIT->CTRL |= CTRL_RITENBR;
	else
    LPC_RIT->CTRL &= ~CTRL_RITENBR;
  rit_timer_counter = 0;
  LPC_RIT->COMPVAL = TimerInterval;
	LPC_RIT->COMPVAL_H = 0x00000000;
  NVIC_EnableIRQ(RIT_IRQn);
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
