/***********************************************************************
* $Id: lpc23xx_pinsel_driver.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx PINSEL driver
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
#ifndef LPC23XX_PINSEL_DRIVER_H_
#define LPC23XX_PINSEL_DRIVER_H_

#include "lpc23xx_pinsel.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Macro for PORT selection argument
 **********************************************************************/
#define	PORT_0  0 	/* PORT 0 */
#define	PORT_1  1 	/* PORT 1 */
#define	PORT_2  2 	/* PORT 2 */
#define	PORT_3  3 	/* PORT 3 */
#define	PORT_4  4  	/* PORT 4 */

/***********************************************************************
 * Macro for Pin Function selection argument
 **********************************************************************/
#define	FUNC_0  0 	/* default function */
#define	FUNC_1  1 	/* first alternate funtion */
#define	FUNC_2  2	/* second alternate funtion */
#define	FUNC_3  3 	/* third or reserved alternate funtion */


/***********************************************************************
 * Structure for PORT selection 
 **********************************************************************/
#define	PINMODE_PULLUP  	0	/* Internal pull-up resistor */
#define	PINMODE_TRISTATE  	2	/* Tri-state */
#define	PINMODE_PULLDOWN  	3	/* Internal pull-down resistor */

/* Function setting for pin on PORT */
void PINSEL_Set ( U32 portnum, U32 pinnum, U32 funcnum);

/* Set up trace function */
void PINSEL_EnableTraceFunc(void);

/* Set up trace function */
void PINSEL_DisableTraceFunc(void);

/* Select pull up/down resistor for pin on PORT */
void PINSEL_SetResistorMode ( U32 portnum, U32 pinnum, U32 modenum);

#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_PINSEL_DRIVER_H_*/
