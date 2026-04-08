/*
 * All software, source code, included documentation, and any implied know-how
 * are property of Freescale Semiconductor and therefore considered
 * CONFIDENTIAL INFORMATION.
 * This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.
 *
 * All Confidential Information remains the property of Freescale Semiconductor
 * and will not be copied or reproduced without the express written permission
 * of the Discloser, except for copies that are absolutely necessary in order
 * to fulfill the Purpose.
 *
 * Services performed by FREESCALE in this matter are performed AS IS and
 * without any warranty.
 * CUSTOMER retains the final decision relative to the total design and
 * functionality of the end product.
 * FREESCALE neither guarantees nor will be held liable by CUSTOMER for the
 * success of this project.
 *
 * FREESCALE disclaims all warranties, express, implied or statutory
 * including, but not limited to, implied warranty of merchantability or
 * fitness for a particular purpose on any hardware, software ore advise
 * supplied to the project by FREESCALE, and or any product resulting from
 * FREESCALE services.
 * In no event shall FREESCALE be liable for incidental or consequential
 * damages arising out of this agreement. CUSTOMER agrees to hold FREESCALE
 * harmless against any and all claims demands or actions by anyone on account
 * of any damage,or injury, whether commercial, contractual, or tortuous,
 * rising directly or indirectly as a result of the advise or assistance
 * supplied CUSTOMER in connectionwith product, services or goods supplied
 * under this Agreement.
 */

/*
 * File:	tower.h
 * Purpose:	Kinetis tower CPU card definitions
 *
 * Notes:
 */

#ifndef __TOWER_H__
#define __TOWER_H__

#include "mcg.h"

/* Global defines to use for TWR_K64F120M */
#define DEBUG_PRINT

#if (defined(TWR_K64F120M))
#	define MCU_MK64FN1M0VCM12

#	define TWR_STRING			"TWR-K64F120M"
#	define TWR_SYSCLOCK			(((CLK0_FREQ_HZ/ PLL0_PRDIV) * PLL0_VDIV) / 1000000)

 /* Input Clock Info */
#	define CLK0_FREQ_HZ			50000000
#	define CLK0_TYPE			CANNED_OSC

  /* PLL Configuration Info */
//#	define NO_PLL_INIT

/*
 * The expected PLL output frequency is:
 * PLL out = (((CLKIN/PRDIV) x VDIV))
 * where the CLKIN can be either CLK0_FREQ_HZ or CLK1_FREQ_HZ.
 *
 * For more info on PLL initialization refer to the mcg driver files.
 */

#	define PLL0_PRDIV			20
#	define PLL0_VDIV			48

	/* System Bus Clock Info */
#	define REF_CLK				XTAL8
#	define CORE_CLK_MHZ			PLL120			/* system/core frequency in MHz */

	/* Serial Port Info */
#	define TERM_PORT			UART5_BASE_PTR	/* OSJTAG serial port or TWR-SER COM port */
#	define TERMINAL_BAUD		115200

#	undef UART0_ALT1		/* PTA_1/2 */
#	undef UART0_ALT2		/* PTA_14/15 */
#	undef UART0_ALT3		/* PTB_16/17 */
#	undef UART0_ALT4		/* PTD_6/7 */
#	undef HW_FLOW_CONTROL	/* UART0, PTA_0/1 */
#	undef HW_FLOW_CONTROL1	/* UART0, PTA_16/17 */
#	undef HW_FLOW_CONTROL2	/* UART0, PTB_2/3 */
#	undef HW_FLOW_CONTROL3	/* UART0, PTD_4/5 */

#	undef UART1_ALT1		/* PTC_3/4 */
#	undef UART1_ALT2		/* PTE_0/1 */
#	undef HW_FLOW_CONTROL	/* UART1, PTC_1/2 */
#	undef HW_FLOW_CONTROL1	/* UART1, PTE_2/3 */

#	undef UART3_ALT1		/* PTB10/11 */
#	undef UART3_ALT2		/* PTC16/17 */
#	undef UART3_ALT3		/* PTE4/5 */
#	undef HW_FLOW_CONTROL	/* UART3, PTB_8/9 */
#	undef HW_FLOW_CONTROL1	/* UART3, PTB_12/13 */
#	undef HW_FLOW_CONTROL2	/* UART3, PTC_18/19 */
#	undef HW_FLOW_CONTROL3	/* UART3, PTE_6/7 */

#	undef UART4_ALT1		/* PTC_14/15 */
#	undef UART4_ALT2		/* PTE_24/25 */
#	undef HW_FLOW_CONTROL	/* UART4, PT_12/13 */
#	undef HW_FLOW_CONTROL1	/* UART4, PT_26/27 */

#	undef UART5_ALT1		/* PTD_8/9 */
#	define UART5_ALT2		/* PTE_8/9 */
#	undef HW_FLOW_CONTROL	/* UART5, PTD_0/1 */
#	undef HW_FLOW_CONTROL1	/* UART5, PTD_10/11 */

#else
#	error "No valid tower CPU card defined"
#endif

#endif		/* __TOWER_H__ */
