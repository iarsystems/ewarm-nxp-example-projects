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

#include "lptmr.h"

char LPTMR_INTERRUPT;		/*Global flag variable */

/* Timer will trigger interrupt after 5 seconds */
void lptmr_interrupt(void)
{
	int compare_value = 5000;	/* value must be less than 0xFFFF */
	LPTMR_INTERRUPT = 0;		/* Clear global variable */

	/* Reset LPTMR module */
	lptmr_clear_registers();

	printf("LPTMR Interrupt Example\n");

	/* Enable LPT Interrupt in NVIC */
	enable_irq(58);		/* LPTMR Vector is 74. IRQ# is 74-16=58 */

	/* Configure LPT */
	LPTMR0_CMR = LPTMR_CMR_COMPARE(compare_value);	/* Set compare value */
	/* Use LPO clock and bypass prescale */
	LPTMR0_PSR = LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK;
	LPTMR0_CSR = LPTMR_CSR_TIE_MASK;	/* Enable LPT interrupt */

	printf("- LPTMR using LPO clock with no prescale, "
		   "and compare value=5000 (5 seconds)\n");
	printf("- Press a key to start counter\n");
	in_char();		/* wait for keyboard press */

	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;	/* Turn on LPTMR and start counting */

	printf("-- Counting...\n");
	/* Wait for the global variable to be set in LPTMR ISR */
	while (LPTMR_INTERRUPT == 0) {
	}

	printf("- Timer should have waited for 5 seconds\n");

	LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;	/* Turn off LPT to avoid more interrupts */
	printf("- End of LPTMR Interrupt Example\n\n");

	/* Reset LPTMR module */
	lptmr_clear_registers();
}

void lptmr_isr_example(void)
{
	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;	/* Clear LPT Compare flag */
	LPTMR_INTERRUPT = 1;				/* Set global variable */
}
