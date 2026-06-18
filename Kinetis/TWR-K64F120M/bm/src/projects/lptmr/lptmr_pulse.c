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
 * Counts pulses found on LPTMR0_ALT1, LPTMR0_ALT2, or LPTMR0_ALT3
 */
void lptmr_pulse_counter(char pin_select)
{
	unsigned int compare_value = 1000;
	int value;
	char input;
	printf("LPTMR Pulse Counting Example on LPTMR_ALT%d\n", pin_select);

	/*Reset LPTMR module */
	lptmr_clear_registers();

	/*Set up GPIO */
	if (pin_select == LPTMR_ALT2) {
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;	/* Turn on PORTC clock */
		PORTC_PCR5 = PORT_PCR_MUX(3);		/* Use ALT3 */

		printf("- Testing ALT2 pin on PORTC5\n");
		printf("- \t%s: ALT2 is conected A70 on TWR-ELEV\n", TWR_STRING);
	} else {
		printf("- Invalid pin selected\n\n");
		return;
	}

	/*
	 * Test requires external hardware. Need to confirm
	 * if want to run test or not
	 */
	printf("- This test requires a function generator, or another way of\n"
		   "  producing a pulse signal on the pin specified above. \n");
	printf("- Please connect that device to the specified pin\n\n");
	printf("- If you would like to continue with this example, press \"y\".\n"
		   "  To skip press any other key\n");
	input = in_char();	/*wait for keyboard press */
	printf("\n");
	if (input != 'y' && input != 'Y') {
		printf("- Exiting LPTMR Pulse Counting Example on LPTMR_ALT%d\n",
		       pin_select);
		return;
	}

	/* Use LPO clock but bypass glitch filter */
	LPTMR0_PSR = LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK;
	/* Set compare value */
	LPTMR0_CMR = LPTMR_CMR_COMPARE(compare_value);
	/*
	 * Set LPT to use the pin selected, and put in pulse count mode,
	 * on rising edge (default)
	 */
	LPTMR0_CSR = LPTMR_CSR_TPS(pin_select) | LPTMR_CSR_TMS_MASK;

	printf("- Press any key to start pulse counter\n");
	in_char();		/* wait for keyboard press */

	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;	/* Turn on LPT */

	printf("Pulse count register (PCR) value is ");
	/* Wait for compare flag to be set */
	while ((LPTMR0_CSR & LPTMR_CSR_TCF_MASK) == 0) {
		/*
		 * This may not get proper counter data if the CNR is read
		 * at the same time it is incremented
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

//		printf("Pulse count register (PCR) value is %d\n",
//		       get_counter_value());
	}

	printf("- Detected %d pulses on LPTMR_ALT%d\n", compare_value,
	       pin_select);
	printf("- End of Pulse Counting Example\n\n");
}
