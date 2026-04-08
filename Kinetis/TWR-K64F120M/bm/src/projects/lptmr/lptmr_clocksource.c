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
 * Internal Reference Clock (PSC=0x0)
 *   The Internal Reference Clock can come from two clock sources.
 *   If MCG_C2[IRCS]=0, then uses slow internal clock (32kHz)
 *   If MCG_C2[IRCS]=1, then uses fast internal clock (2Mhz)
 *
 *  This example uses fast internal clock. It is pre-scaled to wait for 4 seconds.
 *    Because of trim values, it may be slightly faster or slower than this.
 */
void lptmr_internal_ref_input()
{
	/* 4 seconds with prescale=8 and 2Mhz fast clock */
	unsigned int compare_value = 15625;

	printf("Internal Reference Clock Example\n");

	/* Reset LPTMR module */
	lptmr_clear_registers();

	/* Ensure Internal Reference Clock is Enabled */
	MCG_C1 |= MCG_C1_IRCLKEN_MASK;

	/*
	 * Enable fast internal ref clock by setting MCG_C2[IRCS]=1
	 * If wanted to use 32Khz slow mode, set MCG_C2[IRCS]=0 instead
	 */
	MCG_C2 |= MCG_C2_IRCS_MASK;

	/* Configure LPTMR */
	LPTMR0_CMR = LPTMR_CMR_COMPARE(compare_value);		/* Set compare value */
	/* Use internal clock prescaled by 512 */
	LPTMR0_PSR = LPTMR_PSR_PCS(0x0) | LPTMR_PSR_PRESCALE(0x8);

	printf("- LPTMR using fast internal ref clock with PRESCALE=0x8,\n"
		   "  and compare value=15625 (4 seconds)\n");
	printf("- Press a key to start counter\n");
	in_char();

	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;	/* Turn on LPT with default settings */

	printf("-- Counting...\n");

	// Wait for Timer Compare Flag to be set
	while ((LPTMR0_CSR & LPTMR_CSR_TCF_MASK) == 0) {
		/*
		 * This may not get proper counter data if the CNR is
		 * read at the same time it is incremented
		 */
		//printf("Current value of counter register CNR is %d\n",get_counter_value());
	}

	printf("-- 4 seconds should have passed\n");
	printf("- End of Internal reference Clock Source Example\n\n");
}

/*
 * LPO Clock Example (PSC=0x1)
 *
 * Test is pre-scaled to wait for 10 seconds. In reality it might be
 * slightly off because of the trim values for the LPO clock.
 */
void lptmr_lpo_input()
{
	/* 4 second delay with the 1khz LPO clock */
	unsigned int compare_value = 4000;

	printf("LPO Clock Source Example\n");

	/* Reset LPTMR module */
	lptmr_clear_registers();

	/* Configure LPTMR */
	LPTMR0_CMR = LPTMR_CMR_COMPARE(compare_value);			// Set compare value
	LPTMR0_PSR = LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK;	// Use LPO clock with bypass enabled

	printf("- LPTMR using LPO clock with no prescale, "
		   "and compare value=4000 (4 seconds)\n");
	printf("- Press a key to start counter\n");
	in_char();

	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;	//Turn on LPT with default settings

	printf("-- Counting...\n");

	/* Wait for Timer Compare Flag to be set */
	while ((LPTMR0_CSR & LPTMR_CSR_TCF_MASK) == 0) {
		/*
		 * This may not get proper counter data if the CNR is
		 * read at the same time it is incremented
		 */
		//printf("Current value of counter register CNR is %d\n",get_counter_value());
	}

	printf("- 4 seconds should have passed\n");
	printf("- End of LPO Clock Source Example\n\n");
}

/*
 * 32kHz Input Clock Test (PSC=0x2)
 *   The 32kHz clock (ERCLK32K) can come from two clock sources.
 *   If SOPT1[OSC32KSEL]=1, then it uses 32kHz RTC crystal connected to XTAL32
 *      This is what the code below tests
 *   If SOPT1[OSC32KSEL]=0, then it uses 32kHz System oscillator, and reguires
 *      that the main system clock be a 32kHz crystal. The tower board does not
 *      support that feature.
 *
 *  Test is pre-scaled to wait for 8 seconds.
 */
void lptmr_32khz_input()
{
	/* 4 second delay with prescale = 1 */
	unsigned int compare_value = 32768;

	printf("32 Khz Clock Source Example\n");

	/* Reset LPTMR module */
	lptmr_clear_registers();

	/* Configure to use 32Khz clock from RTC clock */
	SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;		// Enable RTC registers
	RTC_CR |= RTC_CR_OSCE_MASK;				// Turn on RTC oscillator
	SIM_SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK;	// Clear RTC OSC as source for ERCLK32K
	SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(2);	// Select RTC OSC as source for ERCLK32K

	/* Configure LPTMR */
	/* Set compare value */
	LPTMR0_CMR = LPTMR_CMR_COMPARE(compare_value);
	/* Use 32khz clock (ERCLK32K) and divide source by 4 */
	LPTMR0_PSR = LPTMR_PSR_PCS(0x2) | LPTMR_PSR_PRESCALE(0x1);

	printf("- LPTMR using 32Khz clock with PRESCALE=0x1, "
		   "and compare value=32768 (4 seconds)\n");
	printf("- Press a key to start counter\n");
	in_char();

	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;	//Turn on LPT with default settings

	printf("-- Counting...\n");

	GPIOB_PTOR |= GPIO_PDDR_PDD(1 << 7);
	/* Wait for Timer Compare Flag to be set */
	while ((LPTMR0_CSR & LPTMR_CSR_TCF_MASK) == 0) {
		/*
		 * This may not get proper counter data if the CNR is
		 * read at the same time it is incremented
		 */
		//printf("Current value of counter register CNR is %d\n",get_counter_value());
	}
	GPIOB_PTOR |= GPIO_PDDR_PDD(1 << 7);

	printf("- 4 seconds should have passed\n");
	printf("- End of 32 Khz Clock Source Example\n\n");
}

/*
 * External Reference Clock Example(PSC=0x3)
 *
 * TWR-K70F120M uses a 50MHz external clock
 *
 *  Test is pre-scaled to wait for 10 seconds in both cases by adjusting
 *    the compare value.
 */
void lptmr_external_clk_input()
{
	unsigned int compare_value;

	printf("External Clock Source Example\n");

	/* Reset LPTMR module */
	lptmr_clear_registers();

	/* Turn on external reference clock */
	SIM_SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK;
	OSC_CR |= OSC_CR_ERCLKEN_MASK;	// selects EXTAL to drive  XOSCxERCLK

	//Set compare value
	compare_value = 7630;	//~10 seconds with prescale=0xF and 50MHz clock

	/* Configure LPTMR */
	/* Set compare value */
	LPTMR0_CMR = LPTMR_CMR_COMPARE(compare_value);
	/* Use divided external clock */
	LPTMR0_PSR = LPTMR_PSR_PCS(0x3) | LPTMR_PSR_PRESCALE(0xF);

	printf("- LPTMR using external clock with PRESCALE=0xF,\n"
		   "  and compare value=%d (10 seconds)\n", compare_value);
	printf("- Press a key to start counter\n");
	in_char();

	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;	//Turn on LPT with default settings

	printf("-- Counting...\n");

	//Wait for Timer Compare Flag to be set
	while ((LPTMR0_CSR & LPTMR_CSR_TCF_MASK) == 0) {
		/*
		 * This may not get proper counter data if the CNR is
		 * read at the same time it is incremented
		 */
		//printf("Current value of counter register CNR is %d\n",get_counter_value());
	}

	printf("- Timer should have waited for 10 seconds\n");
	printf("- End of External Clock Example\n\n");
}
