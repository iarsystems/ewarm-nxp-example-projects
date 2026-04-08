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

#include "common.h"

extern uint32_t irq_sw;

void irq_demo(int a, char **b)
{
	printf("PIN IRQ Demo, press SW1 or SW3\n"
		   "- Press SW3 to exit...\n\n");

	/* init sw1 & sw3 */
	PORTA_PCR4 = PORT_PCR_IRQC(0x0A) | PORT_PCR_MUX(1) |
				 PORT_PCR_PFE_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTC_PCR6 = PORT_PCR_IRQC(0x0A) | PORT_PCR_MUX(1) |
				 PORT_PCR_PFE_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

	/* enable interrupt */
	enable_irq(61);		/* GPIOC Vector is 77. IRQ# is 77-16=61 */
	enable_irq(59);		/* GPIOC Vector is 75. IRQ# is 75-16=59 */

	while (1) {
		if (irq_sw == 0x01) {
			irq_sw &= ~0x01;
			printf("SW3\n");
			break;
		}
		if (irq_sw == 0x10) {
			irq_sw &= ~0x10;
			printf("SW1\n");
		}
	}

	printf("\n");
}

void irq_isr(void)
{
	if (PORTC_ISFR & 0x40) {
		PORTC_ISFR = 0x40;	/* Clear Port C ISR flags */
		irq_sw |= 0x10;
	}
	if (PORTA_ISFR & 0x10) {
		PORTA_ISFR = 0x10;	/* Clear Port A ISR flags */
		irq_sw |= 0x01;
	}
}
