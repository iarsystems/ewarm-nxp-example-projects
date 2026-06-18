/*****************************************************************************
 *   mtimer.h:  Header file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __MTIMER_H 
#define __MTIMER_H

#define TIMER_MS_TICKS	1000000

extern void enable_mtimer0( void );
extern void disable_mtimer0( void );
extern void init_mtimer0(DWORD timer_interval);

#endif /* end __MTIMER_H */

/*****************************************************************************
**                            End Of File
******************************************************************************/
