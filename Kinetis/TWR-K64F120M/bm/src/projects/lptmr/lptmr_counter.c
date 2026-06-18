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

/*
 *  Counting example using the LPO clock.
 *
 *  Sets compare value to 5000. Thus when using the 1Khz LPO clock,
 *   it will take 5 seconds for the Timer Compare Flag to be set.
 */
void lptmr_time_counter()
{
	int compare_value = 5000;	/* value must be less than 0xFFFF or 65535 */
	int value;

	printf("LPTMR Time Counting Example\n");

	/* Reset LPTMR module */
	lptmr_clear_registers();

	/* Configure LPTMR */
	LPTMR0_CMR = LPTMR_CMR_COMPARE(compare_value);			/* Set compare value */
	LPTMR0_PSR = LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK;	/* Use LPO clock and bypass prescale */

	printf("- LPTMR using LPO clock with no prescale, "
		   "and compare value=5000 (5 seconds)\n");
	printf("- Press a key to start counter\n");
	in_char();		/* wait for keyboard press */

	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;	/* Turn on LPTMR with default settings *//* */

	printf("-- Counter register (CNR) value is ");
	/* Wait for Timer Compare Flag to be set */
	while ((LPTMR0_CSR & LPTMR_CSR_TCF_MASK) == 0) {
		/*
		 * This may not get proper counter data if the CNR is
		 * read at the same time it is incremented
		 */
		value = get_counter_value();
		if (value < 10)
			printf("%d\b", value);
		else if (value < 100)
			printf("%d\b\b", value);
		else if (value < 1000)
			printf("%d\b\b\b", value);
		else if (value < 10000)
			printf("%d\b\b\b\b", value);

//		printf("-- Counter register (CNR) value is %d\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b",
//		       get_counter_value());
	}

	printf("- Waited for %4d counts\n", compare_value);
	printf("- End of Time Counting Example\n\n");
}

/*
 *  Counting example using the LPO clock and the prescale feature
 *
 *  Sets compare value to 250. Thus when using the 1Khz LPO clock with
 *    LPTMR0_PSR[PRESCALE]=0x4, it will take 8 seconds for Timer Compare Flag
 *    to be set. (1Khz clock/32=31.25Hz clock)
 *
 *  If prescaler was not used, then timer would only wait .25 seconds.
 *
 */
void lptmr_prescale()
{
	/* value must be less than 0xFFFF or 65535 */
	int compare_value = 250;
	int value;

	printf("LPTMR Time Counting Example with Prescaler\n");

	/* Reset LPTMR module */
	lptmr_clear_registers();

	/* Configure LPTMR */
	LPTMR0_CMR = LPTMR_CMR_COMPARE(compare_value);				/* Set compare value */
	LPTMR0_PSR = LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PRESCALE(0x4);	/* Use LPO clock and divide by 32 */

	printf("- LPTMR using LPO clock with PRESCALE=4 and "
		   "compare value=250 (8 seconds)\n");
	printf("- Press a key to start counter\n");
	in_char();		/* wait for keyboard press */

	/* Turn on LPTMR with default settings */
	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;

	printf("-- Counter register (CNR) value is ");

	/* Wait for Timer Compare Flag to be set */
	while ((LPTMR0_CSR & LPTMR_CSR_TCF_MASK) == 0) {
		/*
		 * This may not get proper counter data if the CNR is
		 * read at the same time it is incremented
		 */
		value = get_counter_value();
		if (value < 10)
			printf("%d\b", value);
		else if (value < 100)
			printf("%d\b\b", value);
		else if (value < 1000)
			printf("%d\b\b\b", value);
		else if (value < 10000)
			printf("%d\b\b\b\b", value);

//		printf("-- Counter register (CNR) value is %d\n",
//		       get_counter_value());
	}

	printf("- Waited for %d counts\n", compare_value);
	printf("- End of Time Counting Example with Prescale\n\n");
}
