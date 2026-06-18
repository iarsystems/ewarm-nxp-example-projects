/***********************************************************************
* $Id: lpc23xx_dac_driver.h 28 2014-01-27 14:45:13Z danielru $
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
#ifndef LPC23XX_DAC_DRIVER_H_
#define LPC23XX_DAC_DRIVER_H_

#include "lpc23xx_dac.h"

#ifdef __cplusplus
extern "C"
{
#endif

/***********************************************************************
 * Public ADC enumeration
 **********************************************************************/
/* Current option in ADC confuration option */
typedef enum
{
	DAC_MAX_CURRENT_700uA = 0, 	/* The settling time of the DAC is 1 us max, 
								and the maximum	current is 700 uA */
	DAC_MAX_CURRENT_350uA		/* The settling time of the DAC is 2.5 us 
								and the maximum current is 350 uA */
} DAC_CURRENT_OPT;

	
/***********************************************************************
 * DAC driver API functions
 **********************************************************************/
/* Init DAC device */
S32 DAC_Init (void);

/* Config DAC device */
S32 DAC_Config (S32 DACdev, S32 maxcurrent);

/* Write/update value to DAC output */
S32 DAC_Update (S32 DACdev, U32 DACValue);


#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_DAC_DRIVER_H_*/
