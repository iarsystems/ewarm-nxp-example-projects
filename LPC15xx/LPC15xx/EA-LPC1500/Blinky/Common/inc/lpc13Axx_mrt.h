/****************************************************************************
 *   $Id: lpc13Axx_mrt.h 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC13Axx software example
 *
 *   Description:
 *     This file contains definition and prototype for multi-rate timer
 *     configuration.
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
#ifndef __MRT_H
#define __MRT_H

#define TEST_REPEATED_MODE        1

#define NUM_MRT_CHANNELS          4

/* Interval register bit definition. */
#define MRT_INTERVAL				0x300
#define MRT_INTERVAL_LOAD   (0x1UL<<31)

/* Control register bit definition. */
#define MRT_INT_ENA					(0x1<<0)
#define MRT_REPEATED_MODE		(0x00<<1)
#define MRT_ONE_SHOT_INT		(0x01<<1)
#define MRT_ONE_SHOT_STALL	(0x02<<1)

/* Status register bit definition */
#define MRT_STAT_IRQ_FLAG		(0x1<<0)
#define MRT_STAT_RUN				(0x1<<1)


void delayMs(uint32_t delayInMs);
void MRT_IRQHandler(void);
void init_mrt(uint32_t ch_num, uint32_t mode, uint32_t timerInterval);
void enable_mrt_timer( uint32_t ch_num );
void disable_mrt_timer( uint32_t ch_num );

#endif /* end __MRT_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
