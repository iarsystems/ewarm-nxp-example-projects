/*****************************************************************************
 *   timer.h:  Header file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TIMER_H 
#define __TIMER_H

#define TIMER_TICKS	80

extern void delayMs(DWORD Interval );
extern void enable_timer( void );
extern void disable_timer( void );
extern void init_timer(DWORD timer_interval);

#endif /* end __TIMER_H */

/*****************************************************************************
**                            End Of File
******************************************************************************/
