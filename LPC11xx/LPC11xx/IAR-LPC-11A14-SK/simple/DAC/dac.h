/****************************************************************************
 *   $Id: dac.h 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC11Axx DAC example
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

#define sine_wave           0
#if !sine_wave
#define ATRG_ENABLED        1
/* In order to test timer trigger, ATRG_ENABLED needs to be set too. */
#define TIMER_TRIG_ENABLED  1
#endif
#define MAX_DAC_VALUE       0x3FF
#define DAC_BIAS           (0x1<<16)

#define NO_TRIGGER          0
#define CMP_TRIGGER         1
#define ATRG0_TRIGGER       2
#define ATRG1_TRIGGER       3
#define CT32B1_M0_TRIGGER   4
#define CT32B1_M1_TRIGGER   5
#define CT16B1_M0_TRIGGER   6
#define CT16B1_M1_TRIGGER   7

extern void DACInit( void );
void DAC_IRQHandler(void);

#endif /* end __DAC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
