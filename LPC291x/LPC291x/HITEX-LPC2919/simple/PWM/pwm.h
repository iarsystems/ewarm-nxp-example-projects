/*****************************************************************************
 *   pwm.h:  Header file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.10.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __PWM_H 
#define __PWM_H

#define PWM_CYCLE		120
#define PWM_OFFSET		20

#define PWM_MATCH0_5	0x3F	/* Bit 0 through 5 */

#define CO_INT			(1 << 0)
#define TD_INT			(1 << 1)
#define UD_INT			(1 << 2)
#define EMGY_INT		(1 << 3)

#define CNT_ENA			(1 << 0)
#define CNT_RESET		(1 << 1) 
#define RUN_ONCE		(1 << 2)
#define SYNC_OUT_ENA	(1 << 3)
#define SYNC_SEL		(1 << 4)
#define TRANS_ENA_SEL	(1 << 5)
#define TRANS_ENA		(1 << 6)
#define UPD_ENA			(1 << 7)

extern void PWM0_Init( DWORD cycle );
extern void PWM0_Set( DWORD cycle, DWORD offset );
extern void PWM0_Start( void );
extern void PWM0_Stop( void );

#endif /* end __PWM_H */
/****************************************************************************
**                            End Of File
****************************************************************************/
