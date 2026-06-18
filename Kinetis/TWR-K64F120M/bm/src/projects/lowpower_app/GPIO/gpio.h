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

#ifndef GPIO_H_
#define GPIO_H_

#include "common.h"

#define BIT_MASK(x)		(1 << x)

#define LED1
#define LED1_PORT		E		/* Orange LED */
#define LED1_BIT		7

/* Not use for this demo */
//#define LED2
//#define LED2_PORT		D		/* Yellow LED */
//#define LED2_BIT		7

/* Compilation time Definitions */

#define PCR(PTO,BIT)	PORT##PTO##_PCR##BIT
#define PDDR(PTO)		GPIO##PTO##_PDDR
#define PSOR(PORT)		GPIO##PORT##_PSOR
#define PCOR(PORT)		GPIO##PORT##_PCOR
#define PTOR(PORT)		GPIO##PORT##_PTOR
#define PDOR(PORT)		GPIO##PORT##_PDOR
#define PDIR(PORT)		GPIO##PORT##_PDIR

/*Pin configured as GPIO */
#define PCR_OUTPUT_CONFIG	(PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK )
/*Pin configured as GPIO Input with pull up enable */
#define PCR_INPUT_CONFIG	(PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK)

#ifdef  LED1_PORT
#define LED1_PCR_OUTPUT(LED1_PORT,LED1_BIT)	PCR(LED1_PORT,LED1_BIT)
#define LED1_DDR_OUTPUT(LED1_PORT,LED1_BIT)	PDDR(LED1_PORT) |= (1<<LED1_BIT)
#define LED1_DDR_INPUT(LED1_PORT,LED1_BIT)	PDDR(LED1_PORT) &= ~(1<<LED1_BIT)
#define LED1_PSOR(LED1_PORT,LED1_BIT)		PSOR(LED1_PORT) |= (1<<LED1_BIT)
#define LED1_PCOR(LED1_PORT,LED1_BIT)		PCOR(LED1_PORT) |= (1<<LED1_BIT)
#define LED1_PTOR(LED1_PORT,LED1_BIT)		PTOR(LED1_PORT) |= (1<<LED1_BIT)

#define LED1_PCR		LED1_PCR_OUTPUT(LED1_PORT,LED1_BIT)
#define LED1_OUTPUT_EN	LED1_DDR_OUTPUT(LED1_PORT,LED1_BIT)
#define LED1_INPUT_EN	LED1_DDR_INPUT(LED1_PORT,LED1_BIT)

#define LED1_SET		LED1_PSOR(LED1_PORT,LED1_BIT)
#define LED1_CLR		LED1_PCOR(LED1_PORT,LED1_BIT)

#define LED1_ON			LED1_PCOR(LED1_PORT,LED1_BIT)
#define LED1_OFF		LED1_PSOR(LED1_PORT,LED1_BIT)
#define LED1_TOGGLE		LED1_PTOR(LED1_PORT,LED1_BIT)
#endif

#ifdef  LED2_PORT
#define LED2_PCR_OUTPUT(LED2_PORT,LED2_BIT)	PCR(LED2_PORT,LED2_BIT)
#define LED2_DDR_OUTPUT(LED2_PORT,LED2_BIT)	PDDR(LED2_PORT) |= (1<<LED2_BIT)
#define LED2_DDR_INPUT(LED2_PORT,LED2_BIT)	PDDR(LED2_PORT) &= ~(1<<LED2_BIT)
#define LED2_PSOR(LED2_PORT,LED2_BIT)		PSOR(LED2_PORT) |= (1<<LED2_BIT)
#define LED2_PCOR(LED2_PORT,LED2_BIT)		PCOR(LED2_PORT) |= (1<<LED2_BIT)
#define LED2_PTOR(LED2_PORT,LED2_BIT)		PTOR(LED2_PORT) |= (1<<LED2_BIT)

#define LED2_PCR		LED2_PCR_OUTPUT (LED2_PORT,LED2_BIT)
#define LED2_OUTPUT_EN	LED2_DDR_OUTPUT (LED2_PORT,LED2_BIT)
#define LED2_INPUT_EN	LED2_DDR_INPUT(LED2_PORT,LED2_BIT)

#define LED2_SET		LED2_PSOR(LED2_PORT,LED2_BIT)
#define LED2_CLR		LED2_PCOR(LED2_PORT,LED2_BIT)

#define LED2_ON			LED2_PCOR(LED2_PORT,LED2_BIT)
#define LED2_OFF		LED2_PSOR(LED2_PORT,LED2_BIT)
#define LED2_TOGGLE		LED2_PTOR(LED2_PORT,LED2_BIT)
#endif

void vfnLEDsOutPutConfig(void);
void vfnLEDsInPutConfig(void);

#endif		/* GPIO_H_ */
