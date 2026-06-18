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
#ifndef __HW_TRIG_TEST_H
#define __HW_TRIG_TEST_H

#define POT				18
#define ADC1_CHANA		POT		/* set to desired ADC0 channel trigger A */
#define ADC1_CHANB		POT		/* set to desired ADC0 channel trigger B */

#define ADC1_DLYA		0x4000	/* ADC1 trigger A delay */
#define ADC1_DLYB		0x8000	/* ADC1 trigger B delay */

/* PIN signal (GREEN) */
#define LED0_TOGGLE		GPIOE_PTOR = 1 << 6;	/* xor - toggle */

/* PIN1 signal (BLUE) */
#define LED1_LOW		GPIOE_PCOR = 1 << 9;	/* clear */
#define LED1_HIGH		GPIOE_PSOR = 1 << 9;	/* set */

/* PIN2 signal (YELLOW) */
#define LED2_LOW		GPIOE_PCOR = 1 << 7;	/* clear */
#define LED2_HIGH		GPIOE_PSOR = 1 << 7;	/* set */

/* PIN2 signal (RED) */
#define LED3_LOW		GPIOE_PCOR = 1 << 8;	/* clear */
#define LED3_HIGH		GPIOE_PSOR = 1 << 8;	/* set */

#define ADC1A_DONE		0x01
#define ADC1B_DONE		0x02

/* function prototypes */

uint8_t Hw_Trig_Test(void);

#endif		/*__HW_TRIG_TEST_H */
