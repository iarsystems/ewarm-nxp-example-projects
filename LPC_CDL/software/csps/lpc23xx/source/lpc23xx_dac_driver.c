/***********************************************************************
* $Id: lpc23xx_dac_driver.c 28 2014-01-27 14:45:13Z danielru $
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

#include "lpc23xx_scb_driver.h"
#include "lpc23xx_dac_driver.h"

/***********************************************************************
 * DAC driver private data and types
 **********************************************************************/

/* DAC configuration type */
typedef struct
{
  BOOL32 	init;       			    	/* Device initialized flag */
  DAC_Type 	*regptr; 						/* Pointer to DAC registers */
} DAC_CFG_T;

/* DAC configuration data */
STATIC DAC_CFG_T daccfg = {
		FALSE, 
		(DAC_Type *) 0
};

/***********************************************************************
 * Public DAC API driver 
 **********************************************************************/

/**********************************************************************
 * Function: DAC_Init
 * Purpose: Init DAC device
 * Parameter: None
 * Return: 
 * 		Pointer to ADC device if success or 0 if error
 *********************************************************************/
S32 DAC_Init (void)
{
	S32 status = 0;
	
	if (daccfg.init == FALSE)
	{
		// Set init flag
		daccfg.init = TRUE;
		
		// Point to register address 
		daccfg.regptr = DAC;
		
		// Set clock divider for DAC module
		SCB_SetPCLKDiv (PCLK_DAC, CCLK_DIV_4);
		
		status = (S32) (&daccfg);
	}
	
	return status; 
}


/**********************************************************************
 * Function: DAC_Config
 * Purpose: Config DAC device
 * Parameter:
 * 		DACdev: Pointer to DAC device
 * 		maxcurrent: Max current option (selected from DAC_CURRENT_OPT enum)
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 DAC_Config (S32 DACdev, S32 maxcurrent)
{
	DAC_CFG_T *pDACdev = (DAC_CFG_T *) DACdev;
	
	if (pDACdev->init == FALSE)
	{
		return _ERROR;
	}
	
	if ((DAC_CURRENT_OPT) maxcurrent == DAC_MAX_CURRENT_700uA)
	{
		pDACdev->regptr->CR &= ~DAC_BIAS_EN;
	}
	else if ((DAC_CURRENT_OPT) maxcurrent == DAC_MAX_CURRENT_350uA)
	{
		pDACdev->regptr->CR |= DAC_BIAS_EN;
	}
	else
	{
		return _ERROR;
	}
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: DAC_Update
 * Purpose: Write/update value to DAC output
 * Parameter:
 * 		DACdev: Pointer to DAC device
 * 		DACValue: DAC value to be updated (must be less than 0x3FF)
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 DAC_Update (S32 DACdev, U32 DACValue)
{
	DAC_CFG_T *pDACdev = (DAC_CFG_T *) DACdev;
	
	if (pDACdev->init == FALSE)
	{
		return _ERROR;
	}
	
	if (DACValue > 0x3FF)
	{
		return _ERROR;
	}
	
	pDACdev->regptr->CR &= ~DAC_VALUE(0x3FF);
	pDACdev->regptr->CR |= DAC_VALUE(DACValue);
	
	return _NO_ERROR;
}
