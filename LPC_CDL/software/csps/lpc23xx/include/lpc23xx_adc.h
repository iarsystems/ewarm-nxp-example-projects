/***********************************************************************
* $Id: lpc23xx_adc.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx ADC definition
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
#ifndef LPC23XX_ADC_H_
#define LPC23XX_ADC_H_

#include "arm7tdmis_types.h"
#include "lpc23xx.h"

#ifdef __cplusplus
extern "C"
{
#endif


/********************************************************************
 * ADC Module Register Structure
 ********************************************************************/
typedef struct
{
	__IO U32 CR;			/* A/D Control Register */
	__IO U32 GDR;			/* A/D Global Data Register */
	__IO U32 RESERVED;		/* Reserved */
	__IO U32 INTEN;			/* A/D Interrupt Enable Register */
	__IO U32 DR[8];			/* A/D Channel 0..7 Data Register */
	__IO U32 STAT;			/* A/D Status Register */
} ADC_Type;

/***********************************************************************
 * A/D Control Register defines
 **********************************************************************/
/* Selects which of the AD0.0:7 pins is (are) to be sampled and converted */
#define ADC_CR_CH_SEL(n)	_BIT(n&0x7)
/* The APB clock (PCLK) is divided by (this value plus one) 
 * to produce the clock for the A/D */
#define ADC_CR_CLKDIV(n)	_SBF(8,n&0xFF)
/* Repeated conversions A/D enable bit */
#define ADC_CR_BURST		_BIT(16)
/* Number of clocks used for each conversion in Burst mode 
 * n is from range 10..3 present for 10..3 bit ADC */
#define ADC_CR_BITS(n)		_SBF(17,((10-n)&0x7))
/* ADC convert in power down mode */
#define ADC_CR_PDN			_BIT(21)
/* Start conversion now */
#define ADC_CR_START_NOW	_SBF(24,1)
/* Start conversion when the edge selected by bit 27 occurs on P2.10/EINT0 */
#define ADC_CR_START_EINT0	_SBF(24,2)
/* Start conversion when the edge selected by bit 27 occurs on P1.27/CAP0.1 */
#define ADC_CR_START_CAP01	_SBF(24,3)
/* Start conversion when the edge selected by bit 27 occurs on MAT0.1 */
#define ADC_CR_START_MAT01	_SBF(24,4)
/* Start conversion when the edge selected by bit 27 occurs on MAT0.3 */
#define ADC_CR_START_MAT03	_SBF(24,5)
/* Start conversion when the edge selected by bit 27 occurs on MAT1.0 */
#define ADC_CR_START_MAT10	_SBF(24,6)
/* Start conversion when the edge selected by bit 27 occurs on MAT1.1 */
#define ADC_CR_START_MAT11	_SBF(24,7)
/* Start conversion on a falling edge on the selected CAP/MAT signal */
#define ADC_CR_EDGE			_BIT(27)


/***********************************************************************
 * A/D Global Data Register defines
 **********************************************************************/
/* When DONE is 1, this field contains result value of ADC conversion */
#define ADC_GDR_RESULT(n)		(U32)((n>>6)&0x3FF)
/* These bits contain the channel from which the LS bits were converted */
#define ADC_GDR_CH(n)			(U32)((n>>24)&0x7)
/* This bit is 1 in burst mode if the results of one or 
 * more conversions was (were) lost */
#define ADC_GDR_OVERRUN_FLAG	_BIT(30)
/* This bit is set to 1 when an A/D conversion completes */
#define ADC_GDR_DONE_FLAG		_BIT(31)


/***********************************************************************
 * A/D Status Register defines
 **********************************************************************/
/* These bits mirror the DONE status flags that appear in the result 
 * register for each A/D channel */
#define ADC_STAT_CH_DONE_FLAG(n)		(U32)(n&0xFF)
/* These bits mirror the OVERRRUN status flags that appear in the 
 * result register for each A/D channel */
#define ADC_STAT_CH_OVERRUN_FLAG(n)		(U32)((n>>8)&0xFF)
/* This bit is the A/D interrupt flag */
#define ADC_STAT_INT_FLAG				_BIT(16)


/***********************************************************************
 * A/D Interrupt Enable Register defines
 **********************************************************************/
/* These bits allow control over which A/D channels generate
 * interrupts for conversion completion */
#define ADC_INTEN_CH(n)			_BIT(n&0x7)
/* When 1, enables the global DONE flag in ADDR to generate an interrupt */
#define ADC_INTEN_GLOBAL		_BIT(8)


/***********************************************************************
 * A/D Data Registers defines
 **********************************************************************/
/* When DONE is 1, this field contains result value of ADC conversion */
#define ADC_DR_RESULT(n)		(U32)((n>>6)&0x3FF)
/* These bits mirror the OVERRRUN status flags that appear in the 
 * result register for each A/D channel */
#define ADC_DR_OVERRUN_FLAG		_BIT(30)
/* This bit is set to 1 when an A/D conversion completes. It is cleared
 * when this register is read */
#define ADC_DR_DONE_FLAG		_BIT(31)

/* Macros pointing to ADC0 registers */
#define ADC0 ((ADC_Type *) (ADC0_BASE))


#ifdef __cplusplus
}
#endif


#endif /*LPC23XX_ADC_H_*/
