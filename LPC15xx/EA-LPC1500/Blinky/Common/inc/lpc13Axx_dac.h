/****************************************************************************
 *   $Id: lpc13Axx_dac.h 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC13Axx DAC example
 *
 *   Description:
 *     This file contains DAC code header definition.
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
#ifndef __DAC_H 
#define __DAC_H

#define TIMER_TRIG_ENABLED	  	1
#define DAC_SHUTOFF_ENABLED     0

#define MAX_DAC_VALUE		      0xFFF
#define CTRL_INT              (0x1<<0)
#define CTRL_TRIGSRC(n)       ((n)<<1)
#define CTRL_POLARITY         (0x1<<4)
#define CTRL_SYNC_BYPASS      (0x1<<5)
#define CTRL_TIMER_ENA        (0x1<<6)
#define CTRL_DBLBUF_ENA       (0x1<<7)
#define CTRL_SHUTOFF_ENA      (0x1<<8)
#define CTRL_SHUTOFF_FLAG     (0x1<<9)
#define DAC_BIAS(n)			      ((n)<<10)

extern void DACInit( uint32_t dac_timer_val );
extern void DACDMAInit( uint32_t dac_timer_val );
void DAC_IRQHandler(void);

#endif /* end __DAC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
