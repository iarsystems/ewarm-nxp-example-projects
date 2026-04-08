/*****************************************************************************
 *   target.h:  Header file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TARGET_H 
#define __TARGET_H

#ifdef __cplusplus
   extern "C" {
#endif

#define HITEX_BOARD		1	/* 0 is NXP SJA2510 board */

#if HITEX_BOARD
#define PLL_M_VALUE		10-1	/* OSC is 16Mhz, multiplier is 10 */
#else
#define PLL_M_VALUE		16-1	/* OSC is 10Mhz, multiplier is 16 */
#endif

/* The ring osc is 400Khz. The CCO clock is always 160Mhz, the Post divider 
 is always 2. So, the CPU_CLK is Clk_CCO/P = 80Mhz */
#define RING_OSC	400000		
#define CPU_CLK		80000000

/* Below PERIPH_CLK can be configured in different frequencies for 
different peripherals. This clock is used for TIMER block only. */ 
#define PERIPH_CLK	80000000

#define IRAM0_BASE_ADDR	0x80000000
#define IRAM0_SIZE	0x8000
#define IRAM1_BASE_ADDR	0x80008000
#define IRAM1_SIZE	0x4000

/*****************************************************************************
** Function name:		TargetInit
**
** Descriptions:		Initialize the target board; it is called in a 
**				necessary place, change it as needed
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern void TargetInit(void);
extern void TargetResetInit( void );
extern void GpioResetInit( void );
/* A RGU test module needs to be called in startup.s, move these
two modules to target.c to faciliate compatibility with all the
other modules. Only RGU test uses below two modules. */
void RGU_Status( void );
void RGU_SRC_Status( void );
/* The IRAM1 test prototyping */
extern void IRAM1SanityCheck( void );

#ifdef __cplusplus
   }
#endif
 
#endif /* end __TARGET_H */
/******************************************************************************
**                            End Of File
******************************************************************************/
