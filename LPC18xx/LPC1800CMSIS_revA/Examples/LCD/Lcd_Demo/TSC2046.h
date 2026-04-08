/**********************************************************************
* $Id: TSC2046.h 28 2014-01-27 14:45:13Z danielru $		TSC2046.h			2012-04-24
*//**
* @file		TSC2046.h
* @brief	Contains all functions to control TSC2046 using SPI
* @version	1.0
* @date		24. April. 2012
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2012, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#ifndef TSC2046_H_
#define TSC2046_H_

#include "lpc_types.h"

/** @defgroup  Lcd_Demo	TSC2046
 * @ingroup LCD_Examples
 * @{
 */


#define TSC2046_CONVERSION_BITS     12

#define TSC2046_SSP_CLOCK		(2000000ul)

/* TSC2046 control byte definitions */

#define TSC_START                          (0x01 << 7)
#define TSC_CHANNEL_X                      (0x05 << 4)    /* differential */
#define TSC_CHANNEL_Y                      (0x01 << 4)    /* differential */
#define TSC_CHANNEL_Z1                     (0x03 << 4)    /* differential */
#define TSC_CHANNEL_Z2                     (0x04 << 4)    /* differential */
#define TSC_8BIT                           (0x01 << 3)
#define TSC_12BIT                          (0x00 << 3)
#define TSC_PD                        		0x00
#define TSC_ADC_ON                          0x01
#define TSC_REF_ON                          0x02
#if (TSC2046_CONVERSION_BITS == 12)
#define TSC_CONVERSION_MODE                 TSC_12BIT
#else
#define TSC_CONVERSION_MODE                 TSC_8BIT
#endif

#define TSC_SER_MODE					   (0x01 << 2) // Single-Ended Reference Mode
#define TSC_DFR_MODE					   (0x00 << 2) // Differential Reference Mode

#define X_MEASURE      (TSC_START | TSC_CHANNEL_X  | TSC_CONVERSION_MODE | TSC_DFR_MODE | TSC_ADC_ON)
#define Y_MEASURE      (TSC_START | TSC_CHANNEL_Y  | TSC_CONVERSION_MODE | TSC_DFR_MODE | TSC_ADC_ON)
#define Z1_MEASURE     (TSC_START | TSC_CHANNEL_Z1 | TSC_CONVERSION_MODE | TSC_DFR_MODE | TSC_ADC_ON)
#define Z2_MEASURE     (TSC_START | TSC_CHANNEL_Z2 | TSC_CONVERSION_MODE | TSC_DFR_MODE | TSC_ADC_ON)
#define PWRDOWN 	   (TSC_START | TSC_CHANNEL_Y  | TSC_CONVERSION_MODE | TSC_DFR_MODE | TSC_PD)

typedef struct
{
    int16_t ad_left;						// left margin
    int16_t ad_right;						// right margin
    int16_t ad_top;							// top margin
    int16_t ad_bottom;						// bottom margin
    int16_t lcd_width;						// lcd horizontal size
    int16_t lcd_height;						// lcd vertical size
    uint8_t swap_xy;						// 1: swap x-y coords
} TSC2046_Init_Type;

/* Initialize TSC2046 */
void TSC2046_Init(TSC2046_Init_Type* pConfig);
/* Get current Touch Coordinates */
Bool GetTouchPos(int16_t *pX, int16_t* pY);
/**
 * @}
 */

#endif /* TSC2046_H_ */
