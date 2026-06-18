/**********************************************************************
* $Id: lpc43xx_nvic.h 28 2014-01-27 14:45:13Z danielru $		lpc43xx_nvic.h		2011-06-02
*//**
* @file		lpc43xx_nvic.h
* @brief	Contains all macro definitions and function prototypes
* 			support for Nesting Vectored Interrupt firmware library
* 			on lpc43xx
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
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
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup NVIC NVIC (Nested Vector Interrupt Controller)
 * @ingroup LPC4300CMSIS_FwLib_Drivers
 * @{
 */

#ifndef lpc43xx_NVIC_H_
#define lpc43xx_NVIC_H_

/* Includes ------------------------------------------------------------------- */
#include "lpc43xx.h"
#include "lpc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif


/* Public Functions ----------------------------------------------------------- */
/** @defgroup NVIC_Public_Functions NVIC Public Functions
 * @{
 */

void NVIC_SetVTOR(uint32_t offset);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* lpc43xx_NVIC_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
