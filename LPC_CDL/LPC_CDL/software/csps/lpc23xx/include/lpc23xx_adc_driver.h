/***********************************************************************
* $Id: lpc23xx_adc_driver.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx ADC driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: ADC
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
**********************************************************************/
#ifndef LPC23XX_ADC_DRIVER_H_
#define LPC23XX_ADC_DRIVER_H_

#include "lpc23xx_adc.h"


/***********************************************************************
 * Public ADC enumeration
 **********************************************************************/
/* Type of start option */
typedef enum
{
	ADC_START_CONTINUOUS = 0,	/* Continuous mode */
	ADC_START_NOW,				/* Start conversion now */
	ADC_START_ON_EINT0,			/* Start conversion when the edge selected 
								by bit 27 occurs on P2.10/EINT0 */
	ADC_START_ON_CAP01,			/* Start conversion when the edge selected 
								by bit 27 occurs on P1.27/CAP0.1 */
	ADC_START_ON_MAT01,			/* Start conversion when the edge selected 
								by bit 27 occurs on MAT0.1 */
	ADC_START_ON_MAT03,			/* Start conversion when the edge selected 
								by bit 27 occurs on MAT0.3 */
	ADC_START_ON_MAT10,			/* Start conversion when the edge selected 
								by bit 27 occurs on MAT1.0 */
	ADC_START_ON_MAT11			/* Start conversion when the edge selected 
								by bit 27 occurs on MAT1.1 */
} ADC_START_OPT;

/* Type of edge when start conversion on the selected CAP/MAT signal */
typedef enum
{
	ADC_START_ON_RISING = 0,	/* Start conversion on a rising edge 
								on the selected CAP/MAT signal */
	ADC_START_ON_FALLING		/* Start conversion on a falling edge 
								on the selected CAP/MAT signal */
} ADC_START_ON_EDGE_OPT;

/***********************************************************************
 * ADC driver API functions
 **********************************************************************/
/* Init ADC device */
S32 ADC_Init(void);

/* Close ADC device */
S32 ADC_Close (S32 ADCdev);

/* Configure ADC device */
S32 ADC_Config (S32 ADCdev, U32 freq, U32 resolution);

/* Enable channel of ADC to be convert */
S32 ADC_EnableCH (S32 ADCdev, U32 ADC_channel);

/* Disable channel of ADC to be convert */
S32 ADC_DisableCH (S32 ADCdev, U32 ADC_channel);

/* Start AD conversion in each type of start */
S32 ADC_Start (S32 ADCdev, S32 starttype, S32 arg);

/* Stop AD conversion */
S32 ADC_Stop (S32 ADCdev);

/* Read AD value of each channel according to each type 
 * of start in polling mode */
U32 ADC_ReadPoll (S32 ADCdev, U32 ADC_channel);

/* Read AD AD value of each channel in interrupt mode,
 * after interrupt handler, current value of each channel will be
 * copied to AD buffer [0..7] */
U32 ADC_ReadAsync (S32 ADCdev, U32 ADC_channel);

/* Enable ADC interrupt */
S32 ADC_EnableInt (S32 ADCdev, U32 ADC_channel);

/* Enable ADC interrupt */
S32 ADC_DisableInt (S32 ADCdev, U32 ADC_channel);

/* Enable global interrupt on ADC device */
S32 ADC_EnableGlobalInt (S32 ADCdev);

/* Disable global interrupt on ADC device */
S32 ADC_DisableGlobalInt (S32 ADCdev);

/* Interrupt handler for ADC interrupt mode */
void ADC_ISR (void);

/* Set callback functon for ADC device */
S32 ADC_SetCBS (S32 ADCdev, void *pCBS);

#endif /*LPC23XX_ADC_DRIVER_H_*/
