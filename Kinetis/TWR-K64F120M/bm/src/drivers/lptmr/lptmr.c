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
 * File:	lptmr.c
 * Purpose:	Provide common low power timer functions
 *
 * Notes:	Right now only function provided is used
 *			to generate a delay in ms. This driver
 *			could be expanded to include more functions
 *			in the future.
 */

#include "common.h"
#include "lptmr.h"

/*
 * Initialize the low power time to provide a delay measured in ms.
 *
 * Parameters:
 *	count_val	number of ms to delay
 *
 * Returns:
 * None
 */
void lptmr_init(void)
{
	/* Make sure the clock to the LPTMR is enabled */
	SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

	LPTMR0_CSR = 0;
	LPTMR0_CMR = 0;

	/* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
	LPTMR0_PSR = LPTMR_PSR_PCS(1) | 0;

	//enable_irq(INT_LPTimer - 16);
}

void time_delay_ms(uint32 count_val)
{

	/* Set the compare value to the number of ms to delay */
	LPTMR0_CMR = count_val;


	/* Start the timer */
//	LPTMR0_CSR = LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK;
	LPTMR0_CSR = LPTMR_CSR_TEN_MASK;

	/* Wait for counter to reach compare value */
	while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)) ;

	/* Clear Timer Compare Flag */
	LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;

	return;
}

void lptmr_isr(void)
{
	/* write 1 to TCF to clear the LPT timer compare flag */
	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
	LPTMR0_CSR = (LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK);
}
