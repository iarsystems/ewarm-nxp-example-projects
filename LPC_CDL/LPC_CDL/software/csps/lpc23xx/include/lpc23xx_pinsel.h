/***********************************************************************
* $Id: lpc23xx_pinsel.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx PINSEL definition
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
#ifndef LPC23XX_PINSEL_H_
#define LPC23XX_PINSEL_H_

#include "lpc23xx.h"
#include "arm7tdmis_types.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/**********************************************************************
* PINSEL register structures
**********************************************************************/
typedef struct{
	__IO U32 PINSELn[11]; 		/* Pin function select register */
	__IO U32 RESERVED0[5]; 		/* Reserve */
	__IO U32 PINMODEn[10];		/* Pin mode select register */
} PINSEL_Type;

/* Macro pointing to PINSEL registers address */
#define PINSEL  ((PINSEL_Type*) (PINSEL_BASE))

#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_PINSEL_H_*/
