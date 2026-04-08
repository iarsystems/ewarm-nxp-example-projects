/***********************************************************************
* $Id: lpc23xx_vic_driver.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx VIC driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: VIC
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
#ifndef LPC23XX_VIC_DRIVER_H_
#define LPC23XX_VIC_DRIVER_H_

#include "lpc23xx_vic.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initilize and refresh VIC module */
void VIC_Init(void);

/* Install IRQ interrupt for interrupt source */
S32 VIC_InstallIRQ (U32 int_source, 
						void *handler_addr,
						U32 priority);

/* Uninstall IRQ interrupt for interrupt source */
S32 VIC_UninstallIRQ (U32 int_source);

/* Install FIQ interrupt for interrupt source */
S32 VIC_InstallFIQ (U32 int_source);

/* Uninstall FIQ interrupt for interrupt source */
S32 VIC_UninstallFIQ (U32 int_source);

/* Enable interrupt for interrupt source */
void VIC_EnableInt (U32 int_source);

/* Disable interrupt for interrupt source */
void VIC_DisableInt (U32 int_source);

/* Get current IRQ interrupt status */
U32 VIC_GetIRQPending (void);

/* Get current FIQ interrupt status */
U32 VIC_GetFIQPending (void);

/* Set SW interrupt */
void VIC_SetSWInt (U32 int_source);

/* Set SW interrupt */
void VIC_ClrSWInt (U32 int_source);

/* Acknowledge by updating VIC addrr */
void VIC_Ack(void);

#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_VIC_DRIVER_H_*/
