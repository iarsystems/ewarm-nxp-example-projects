/****************************************************************************
 *   $Id: lpc13Axx_rit.h 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC17xx RIT example
 *
 *   Description:
 *     This file contains RIT code header definition.
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
#ifndef __RITTIMER_H 
#define __RITTIMER_H

#define TIME_INTERVAL	(12000000/100 - 1)

#define CTRL_RITINT				(0x1<<0)
#define CTRL_RITENCLR			(0x1<<1)
#define CTRL_RITENBR			(0x1<<2)
#define CTRL_RITEN				(0x1<<3)

extern void init_rit_timer( uint32_t debugger_flag, uint32_t timerInterval );
extern void enable_rit_timer( void );
extern void disable_rit_timer( void );
extern void reset_rit_timer( void );
extern void RIT_IRQHandler (void);

#endif /* end __RITTIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
