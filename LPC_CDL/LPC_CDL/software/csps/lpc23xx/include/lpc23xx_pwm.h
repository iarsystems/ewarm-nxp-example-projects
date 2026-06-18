/***********************************************************************
* $Id: lpc23xx_pwm.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx PWM definitions
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: PWM
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

#ifndef LPC23XX_PWM_H
#define LPC23XX_PWM_H

#include "arm7tdmis_types.h"
#include "lpc23xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
* PWM register structures
**********************************************************************/

/* PWM module register structures */ 
typedef struct {
    __IO U32 IR;         	/* PWM Interrupt Register */
    __IO U32 TCR;        	/* PWM Timer Control Register */
    __IO U32 TC;         	/* PWM Timer Counter */
    __IO U32 PR;         	/* PWM Prescale Register */
    __IO U32 PC;         	/* PWM Prescale Counter */
    __IO U32 MCR;        	/* PWM Match Control Register */
    __IO U32 MR0;        	/* PWM Match Register 0 */
    __IO U32 MR1;        	/* PWM Match Register 1 */
    __IO U32 MR2;        	/* PWM Match Register 2 */
    __IO U32 MR3;        	/* PWM Match Register 3 */
    __IO U32 CCR;			/* Capture Control Register */
    __IO U32 CR0;			/* Capture Register 0 */
    __IO U32 CR1;			/* Capture Register 1 */
    __IO U32 CR2;			/* Capture Register 2 */
    __IO U32 CR3;			/* Capture Register 3 */
    __IO U32 RESERVED0;		/* Reserved */
    __IO U32 MR4;        	/* PWM Match Register 4 */
    __IO U32 MR5;        	/* PWM Match Register 5 */
    __IO U32 MR6;        	/* PWM Match Register 6 */
    __IO U32 PCR;        	/* PWM Control Register */
    __IO U32 LER;        	/* PWM Load Enable Register */
    __IO U32 RESERVED1[7];	/* Reserved */
    __IO U32 CTCR;			/* Count Control Register */
} PWM_Type;

/**********************************************************************
* IR register definitions
**********************************************************************/
/* Interrupt flag for PWM match channel for 6 channel */
#define PWM_INT_MATCH(n)    ((n) < 4 ? _BIT(n) : _BIT(n + 4))
/* Interrupt flag for capture input */
#define PWMCAP_INT(n)		_BIT(n + 4)

/**********************************************************************
* TCR register definitions
**********************************************************************/
#define PWM_COUNTER_ENABLE      _BIT(0) /* PWM Counter Enable */
#define PWM_COUNTER_RESET       _BIT(1) /* PWM Counter Reset */
#define PWM_ENABLE              _BIT(3) /* PWM Enable */

/**********************************************************************
* CTCR register definitions
**********************************************************************/
 /* PWM Counter-Timer Mode */
#define PWM_CT_MODE(n)          _SBF(0, (n & 0x03))
 /* PWM Capture input select */
#define PWM_CIN_SELECT(n)       _SBF(2, (n & 0x03))

/**********************************************************************
* MCR register definitions
**********************************************************************/
/* generate a PWM interrupt when a MATCHn occurs */
#define PWM_INT_ON_MATCH(n)     _BIT(((n & 0x7) << 1) + (n & 0x07))
/* reset the PWM when a MATCHn occurs */
#define PWM_RESET_ON_MATCH(n)   _BIT(((n & 0x7) << 1) + (n & 0x07) + 1)
/* stop the PWM when a MATCHn occurs */
#define PWM_STOP_ON_MATCH(n)    _BIT(((n & 0x7) << 1) + (n & 0x07) + 2)

/**********************************************************************
* CCR register definitions
**********************************************************************/
/* PCAPn is rising edge sensitive */
#define PWM_PCAP_RISING(n)  _BIT(((n & 0x2) << 1) + (n & 0x1))
/* PCAPn is falling edge sensitive */
#define PWM_PCAP_FALLING(n) _BIT(((n & 0x2) << 1) + (n & 0x1) + 1)
/* PWM interrupt is generated on a PCAP event */
#define PWM_INT_ON_PCAP(n)  _BIT(((n & 0x2) << 1) + (n & 0x1) + 2)

/**********************************************************************
* PCR register definitions
**********************************************************************/
/* PWM output n is a single edge controlled output */
#define PWM_SINGLE_EDGE(n)  _SBF(n & 0x7, 0)
/* PWM output n is a double edge controlled output */
#define PWM_DOUBLE_EDGE(n)  ((n & 0x7) < 2 ? 0 : _SBF((n & 0x7), 1))
/* enable PWM output n */
#define PWM_ENABLE_OUT(n)   ((n & 0x7) < 1 ? 0 : _SBF((n & 0x7) + 8, 1))

/**********************************************************************
* LER register definitions
**********************************************************************/
/* PWM MATCHn register update control */
#define PWM_EN_MATCHL(n)    (n < 7 ? _BIT(n) : 0)

/**********************************************************************
* CCR register definitions
**********************************************************************/
/* Interrupt on CAPn.x event */
#define PWM_ICAP_EN(x)		_BIT((x << 1) + x + 2)

/* Macro pointing to PWM registers */
#define PWM 	((PWM_Type *)(PWM_BASE))

#ifdef __cplusplus
}
#endif

#endif /* LPC23XX_PWM_H */ 
