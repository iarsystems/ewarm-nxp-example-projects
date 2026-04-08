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

#ifndef __LED_H__
#define __LED_H__

#include "common.h"

void LEDs_toggle(void);

#define ENABLE_GPIO_CLOCKS (SIM_SCGC5 |= (SIM_SCGC5_PORTC_MASK))

/* TWR-K64F120M GPIO for LED configuration
 *            Diode D5 - PTE6
 *            Diode D6 - PTE7
 *            Diode D7 - PTE8
 *            Diode D9 - PTE9
 */
#define LED0_EN (PORTE_PCR6 = PORT_PCR_MUX(1))
#define LED1_EN (PORTE_PCR7 = PORT_PCR_MUX(1))
#define LED2_EN (PORTE_PCR8 = PORT_PCR_MUX(1))
#define LED3_EN (PORTE_PCR9 = PORT_PCR_MUX(1))

#define LED0_TOGGLE (GPIOE_PTOR = (1<<6))
#define LED1_TOGGLE (GPIOE_PTOR = (1<<7))
#define LED2_TOGGLE (GPIOE_PTOR = (1<<8))
#define LED3_TOGGLE (GPIOE_PTOR = (1<<9))

#endif		/* __LED_H__ */
