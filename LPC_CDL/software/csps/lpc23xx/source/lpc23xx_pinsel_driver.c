/***********************************************************************
* $Id: lpc23xx_pinsel_driver.c 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx pinsel driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: PINSEL
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

#include "lpc23xx_pinsel_driver.h"

/***********************************************************************
 *
 * Function: PINSEL_Set 
 *
 * Purpose: Setup the pin selection function
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 * 		portnum: PORT number (must be 0..4)
 *	    pinnum:  Pin number (must be 0..31)
 *  	funcnum: Function number (must be 0..3)
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void PINSEL_Set ( U32 portnum, U32 pinnum, U32 funcnum)
{
	U32 pinnum_t = pinnum;
	U32 pinselreg_idx = 2 * portnum;
	
	if (pinnum_t >= 16)
	{
		pinnum_t -= 16;
		pinselreg_idx++ ;
	}
	
	PINSEL->PINSELn[pinselreg_idx] &= ~((U32)(3 << (pinnum_t * 2)));
	PINSEL->PINSELn[pinselreg_idx] |= (U32)(funcnum << (pinnum_t * 2));
}

/***********************************************************************
 *
 * Function: PINSEL_EnableTraceFunc 
 *
 * Purpose: Enable trace function
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void PINSEL_EnableTraceFunc(void)
{
	PINSEL->PINSELn[10] |= (1 << 3); 
}

/***********************************************************************
 *
 * Function: PINSEL_DisableTraceFunc 
 *
 * Purpose: Disable trace function
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void PINSEL_DisableTraceFunc(void)
{
	PINSEL->PINSELn[10] &= ~(1 << 3); 
}

/***********************************************************************
 *
 * Function: PINSEL_SetResistorMode 
 *
 * Purpose: Setup resistor mode for each pin
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 * 		portnum: PORT number (must be 0..4)
 *	    pinnum:  Pin number (must be 0..31)
 *  	modenum: Mode number (must be 0, 2 or 3)
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void PINSEL_SetResistorMode ( U32 portnum, U32 pinnum, U32 modenum)
{
	U32 pinnum_t = pinnum;
	U32 pinmodereg_idx = 2 * portnum;
	
	if (pinnum_t >= 16)
	{
		pinnum_t -= 16;
		pinmodereg_idx++ ;
	}
	
	PINSEL->PINMODEn[pinmodereg_idx] &= ~((U32)(3 << (pinnum_t * 2)));
	PINSEL->PINMODEn[pinmodereg_idx] |= (U32)(modenum << (pinnum_t * 2));
}
