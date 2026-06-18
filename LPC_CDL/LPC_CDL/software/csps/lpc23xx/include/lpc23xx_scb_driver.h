/***********************************************************************
* $Id: lpc23xx_scb_driver.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx System control block driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: SCB
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
#ifndef LPC23XX_SYSCTRLBLK_DRIVER_H_
#define LPC23XX_SYSCTRLBLK_DRIVER_H_

#include "lpc23xx_scb.h"

#ifdef __cplusplus
extern "C" {
#endif
	

/***********************************************************************
 *  System control block  driver public functions
 **********************************************************************/
	
/* Read status of external interrupt flag source */
U32 SCB_ReadEXTINTPending(void);

/* Clear external interrupt flag source */ 
void SCB_ClrEXTINTPending(U32 source);

/* Set up external interrupt mode */
void SCB_SetEXTINTMode(U32 source, U32 mode);

/* Setup external interrupt polar */
void SCB_SetEXTINTPolar(U32 source, U32 polar);

/* Select GPIO access mode */
void SCB_SetGPIOMode (U32 gpiomode);

/* MCI power active level setup */
void SCB_SetMCIPWR (U32 activelevel);

/* Memmory acceleration module setup */
void SCB_SetMAM(U32 cycle_val);

/* System clock setting up */
S32 SCB_SetupSYSCLK (U32 Fosc, U32 MSel, U32 NSel, 
					U32 ClkSrcSel, U32 CCLKDiv, U32 USBCLKDiv);

/* Get current value of each system clock type */
S32 SCB_GetSYSCLK (U32 sysclktype);

/* Set value of each Peripheral Clock Selection */
void SCB_SetPCLKDiv (U32 clktype, U32 divval);

/* Get current value of each Peripheral Clock */
U32 SCB_GetPCLK (U32 clktype);

/* Set up wake-up source */
void SCB_SetWakeUpSource (U32 source);

/* Clear wake up source */
void SCB_ClrWakeUpSource (U32 source);

/* Enter power down mode */
void SCB_InitPDOWN (void);

/* Enter sleep mode */
void SCB_InitSLEEP (void);

/* Enter idle mode */
void SCB_InitIDLE (void);

/* Enable Power for peripheral */
void SCB_EnablePPWR (U32 ppwrsrc);

/* Disable Power for Peripheral */
void SCB_DisablePPWR (U32 ppwrsrc);

#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_SYSCTRLBLK_DRIVER_H_*/
