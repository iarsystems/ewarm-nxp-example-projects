/*****************************************************************************
 *   timer.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TIMER_H
#define __TIMER_H

#define TIME_INTERVAL Fpclk/100 - 1

extern DWORD init_timer( DWORD timerInterval );
extern void enable_timer( BYTE timer_num );
extern void disable_timer( BYTE timer_num );
extern void reset_timer( BYTE timer_num );

#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
