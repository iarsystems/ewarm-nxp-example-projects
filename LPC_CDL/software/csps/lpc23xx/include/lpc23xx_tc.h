/***********************************************************************
* ID: lpc23xx_tc.h 2008-06-09 HieuNguyen
* Project: LPC23xx Timer/counter definitions
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: Timer/counter
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
#ifndef LPC23XX_TC_H_
#define LPC23XX_TC_H_

#include "arm7tdmis_types.h"
#include "lpc23xx.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/**********************************************************************
* Timer/counter register structures
**********************************************************************/
typedef struct
{
	__IO U32 IR; 			/* Timer interrupt register */
	__IO U32 TCR; 			/* Timer control register */
	__IO U32 TC; 			/* Timer counter */
	__IO U32 PR; 			/* Timer prescale register */ 
	__IO U32 PC; 			/* Timer prescale counter */
	__IO U32 MCR; 			/* Timer match control register */
	__IO U32 MR[4]; 		/* Timer match register 0..3 */
	__IO U32 CCR; 			/* Timer capture control register */
	__IO U32 CR[4]; 		/* Timer capture register */
	__IO U32 EMR; 			/* Timer external match register */
	__IO U32 RESERVED[12]; 	/* Reserved */
	__IO U32 CTCR; 			/* Timer count control register */
} TC_Type;

/**********************************************************************
* IR definitions
**********************************************************************/
/* Macro for getting a timer match interrupt bit */
#define TC_MATCH_INT(n)		_BIT((n & 0x03))
/* Macro for getting a capture event interrupt bit */
#define TC_CAP_INT(n)     	_BIT(((n & 0x03) + 4))

/**********************************************************************
* CR definitions
**********************************************************************/
/* Timer/counter enable bit */
#define TC_ENABLE			0x01
/* Timer/counter reset bit */
#define TC_RESET			0x02

/**********************************************************************
* MCR definitions
**********************************************************************/
/* Bit location for interrupt on MRx match, n = 0 to 3 */
#define TC_INT_ON_MATCH(n)      	_BIT((n * 3))
/* Bit location for reset on MRx match, n = 0 to 3 */
#define TC_RESET_ON_MATCH(n)    	_BIT(((n * 3) + 1))
/* Bit location for stop on MRx match, n = 0 to 3 */
#define TC_STOP_ON_MATCH(n)     	_BIT(((n * 3) + 2))

/**********************************************************************
* CCR definitions
**********************************************************************/
/* Bit location for CAP.n on CRx rising edge, n = 0 to 3 */
#define TC_CAP_RISING(n)   		_BIT((n * 3))
/* Bit location for CAP.n on CRx falling edge, n = 0 to 3 */
#define TC_CAP_FALLING(n)   	_BIT(((n * 3) + 1))
/* Bit location for CAP.n on CRx interrupt enable, n = 0 to 3 */
#define TC_INT_ON_CAP(n)    	_BIT(((n * 3) + 2))
/* Maskbit for rising and falling edge bit */
#define TC_EDGE_MASK(n)			_SBF((n * 3), 0x03)

/**********************************************************************
* EMR definitions
**********************************************************************/
/* Bit location for output state change of MAT.n when external match
   happens, n = 0 to 3 */
#define TC_EM(n)    		_BIT(n)
/* Output state change of MAT.n when external match happens: no change */
#define TC_EM_NOTHING    	0x0
/* Output state change of MAT.n when external match happens: low */
#define TC_EM_LOW         	0x1
/* Output state change of MAT.n when external match happens: high */
#define TC_EM_HIGH        	0x2
/* Output state change of MAT.n when external match happens: toggle */
#define TC_EM_TOGGLE      	0x3
/* Macro for setting for the MAT.n change state bits */
#define TC_EM_SET(n,s) 	_SBF(((n << 1) + 4), (s & 0x03))
/* Mask for the MAT.n change state bits */
#define TC_EM_MASK(n) 		_SBF(((n << 1) + 4), 0x03)

/**********************************************************************
* CTCR definitions
**********************************************************************/
/* Mask to get the Counter/timer mode bits */
#define TC_CTCR_MODE_MASK  0x3
/* Mask to get the count input select bits */
#define TC_CTCR_INPUT_MASK 0xC

/* Macros pointing to timer/counter 0 registers */
#define TC0 ((TC_Type *) (TC0_BASE))
/* Macros pointing to timer/counter 1 registers */
#define TC1 ((TC_Type *) (TC1_BASE))
/* Macros pointing to timer/counter 2 registers */
#define TC2 ((TC_Type *) (TC2_BASE))
/* Macros pointing to timer/counter 3 registers */
#define TC3 ((TC_Type *) (TC3_BASE))


#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_TC_H_*/
