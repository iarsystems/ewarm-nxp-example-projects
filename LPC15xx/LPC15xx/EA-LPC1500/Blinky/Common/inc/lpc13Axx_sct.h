/**********************************************************************
* $Id: lpc13Axx_sct.h 28 2014-01-27 14:45:13Z danielru $		lpc13Axx_sct.h		2011-06-02
*//**
* @file		lpc13Axx_sct.h
* @brief	Contains all macro definitions and function prototypes
* 			support for SCT firmware library on lpc8xx
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors’
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup SCT SCT (State Configurable Timer)
 * @ingroup LPC800CMSIS_FwLib_Drivers
 * @{
 */

#ifndef __SCT_H_
#define __SCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Private macros ------------------------------------------------------------- */
/** @defgroup SCT_Private_Macros SCT Private Macros
 * @{
 */

/* -------------------------- BIT DEFINITIONS ----------------------------------- */
/*********************************************************************//**
 * Macro defines for SCT  configuration register
 **********************************************************************/
/**  Selects 16/32 bit counter */
#define SCT_CONFIG_16BIT_COUNTER		0x00000000
#define SCT_CONFIG_32BIT_COUNTER		0x00000001

/*********************************************************************//**
 * Macro defines for SCT control register
 **********************************************************************/
/**  Stop low counter */
#define SCT_CTRL_STOP_L					(1<<1)
/**  Halt low counter */
#define SCT_CTRL_HALT_L					(1<<2)
/**  Clear low or unified counter */
#define SCT_CTRL_CLRCTR_L				(1<<3)
/**  Direction for low or unified counter */
#define COUNTUP_TO_LIMIT_THEN_CLEAR_TO_ZERO		0
#define COUNTUP_TO LIMIT_THEN_COUNTDOWN_TO_ZERO	1
#define SCT_CTRL_BIDIR_L(x)				(((x)&0x01)<<4)
/**  Prescale clock for low or unified counter */
#define SCT_CTRL_PRE_L(x)				(((x)&0xFF)<<5)

/**  Stop high counter */
#define SCT_CTRL_STOP_H					(1<<17)
/**  Halt high counter */
#define SCT_CTRL_HALT_H					(1<<18)
/**  Clear high counter */
#define SCT_CTRL_CLRCTR_H				(1<<19)
/**  Direction for high counter */
#define COUNTUP_TO_LIMIT_THEN_CLEAR_TO_ZERO		0
#define COUNTUP_TO LIMIT_THEN_COUNTDOWN_TO_ZERO	1
#define SCT_CTRL_BIDIR_H(x)				(((x)&0x01)<<20)
/**  Prescale clock for high counter */
#define SCT_CTRL_PRE_H(x)				(((x)&0xFF)<<21)
/*********************************************************************//**
 * Macro defines for SCT Conflict resolution register
**********************************************************************/
/**  Define conflict solution */
#define SCT_RES_NOCHANGE				(0)
#define SCT_RES_SET_OUTPUT				(1)
#define SCT_RES_CLEAR_OUTPUT			(2)
#define SCT_RES_TOGGLE_OUTPUT			(3)

#define LPC_SCT_CONFIG_RESET_VALUE 					0x1e00UL
#define LPC_SCT_CTRL_L_RESET_VALUE 					0x04UL
#define LPC_SCT_CTRL_H_RESET_VALUE 					0x04UL
#define LPC_SCT_LIMIT_L_RESET_VALUE 				0x0UL
#define LPC_SCT_LIMIT_H_RESET_VALUE 				0x0UL
#define LPC_SCT_HALT_L_RESET_VALUE 					0x0UL
#define LPC_SCT_HALT_H_RESET_VALUE 					0x0UL
#define LPC_SCT_STOP_L_RESET_VALUE 					0x0UL
#define LPC_SCT_STOP_H_RESET_VALUE 					0x0UL
#define LPC_SCT_START_L_RESET_VALUE 				0x0UL
#define LPC_SCT_START_H_RESET_VALUE 				0x0UL
#define LPC_SCT_COUNT_L_RESET_VALUE 				0x0UL
#define LPC_SCT_COUNT_H_RESET_VALUE 				0x0UL
#define LPC_SCT_STATE_L_RESET_VALUE 				0x0UL
#define LPC_SCT_STATE_H_RESET_VALUE 				0x0UL
#define LPC_SCT_REGMODE_L_RESET_VALUE 			0x0UL
#define LPC_SCT_REGMODE_H_RESET_VALUE 			0x0UL
#define LPC_SCT_OUTPUT_RESET_VALUE 					0x0UL
#define LPC_SCT_OUTPUTDIRCTRL_RESET_VALUE 	0x0UL
#define LPC_SCT_RES_RESET_VALUE 						0x0UL
#define LPC_SCT_DMA0REQUEST_RESET_VALUE 		0x0UL
#define LPC_SCT_DMA1REQUEST_RESET_VALUE 		0x0UL
#define LPC_SCT_EVEN_RESET_VALUE 						0x0UL
#define LPC_SCT_EVFLAG_RESET_VALUE 					0x0UL
#define LPC_SCT_CONEN_RESET_VALUE 					0x0UL
#define LPC_SCT_CONFLAG_RESET_VALUE 				0x0UL

#define LPC_SCT_MATCH_L_RESET_VALUE 				0x0UL
#define LPC_SCT_MATCH_H_RESET_VALUE 				0x0UL

#define LPC_SCT_MATCHREL_L_RESET_VALUE 			0x0UL
#define LPC_SCT_MATCHREL_H_RESET_VALUE 			0x0UL

#define LPC_SCT_EVENT_STATE_RESET_VALUE 		0x0UL
#define LPC_SCT_EVENT_CTRL_RESET_VALUE 			0x0UL
#define LPC_SCT_OUT_SET_RESET_VALUE 				0x0UL
#define LPC_SCT_OUT_CLR_RESET_VALUE 				0x0UL
#define LPC_SCT_MODULE_CONTENT_RESET_VALUE 	0x10862902UL

/* Public Functions ----------------------------------------------------------- */
/** @defgroup SCT_Public_Functions SCT Public Functions
 * @{
 */

void SCT_IRQHandler (void);
void SCT_Config(uint32_t value);
void SCT_ControlSet(uint32_t value, uint32_t ena);
void SCT_ConflictResolutionSet(uint8_t outnum, uint8_t value);
void SCT_EventFlagClear(uint8_t even_num);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* __SCT_H_ */

/**
 * @}
 */

