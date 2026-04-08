/***********************************************************************
* $Id: lpc23xx_dac.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx DAC driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: DAC
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
#ifndef LPC23XX_DAC_H_
#define LPC23XX_DAC_H_

#include "arm7tdmis_types.h"
#include "lpc23xx.h"

#ifdef __cplusplus
extern "C"
{
#endif


/********************************************************************
 * DAC Module Register Structure
 ********************************************************************/
typedef struct
{
	__IO U32 CR;	/* D/A Converter Register */
} DAC_Type;

/***********************************************************************
 * D/A Control Register defines
 **********************************************************************/
/* After the selected settling time after this field is written with a
new VALUE, the voltage on the AOUT pin (with respect to VSSA)
is VALUE/1024 × VREF */
#define DAC_VALUE(n) 		_SBF(6,(n&0x3FF))
/* If this bit = 0: The settling time of the DAC is 1 microsecond max, 
 * and the maximum current is 700 microAmpere
 * If this bit = 1: The settling time of the DAC is 2.5 microsecond 
 * and the maximum current is 350 microAmpere */
#define DAC_BIAS_EN			_BIT(16)


/* Macros pointing to ADC0 registers */
#define DAC ((DAC_Type *) (DAC_BASE))

#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_DAC_H_*/
