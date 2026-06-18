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

void main(void)
{
	printf("LPTMR Examples\n\n");

	/* Enable Port clocks */
	SIM_SCGC5 |=
	    SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK |
	    SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	/* Enable LPT Module */
	SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

	PORTB_PCR7 |= PORT_PCR_MUX(1);
	GPIOB_PDDR |= GPIO_PDDR_PDD(1 << 7);

	/* Time counting using LPO clock */
	lptmr_time_counter();

	/* Time counting using LPO clock with Prescaling */
	lptmr_prescale();

	/* Time counting using different clock sources */
	lptmr_internal_ref_input();
	lptmr_lpo_input();
	lptmr_32khz_input();

	lptmr_external_clk_input();

	/* LPTMR Interrupt Example */
	lptmr_interrupt();

	/* Basic Pulse Counting */
	lptmr_pulse_counter(LPTMR_ALT1);	/* LPMR_ALT1 not supported on TWR-K64F120M board */
	lptmr_pulse_counter(LPTMR_ALT2);

	printf("End of LPTMR Examples\n");
}

/*
 * Get the current LPTMR Counter Value.
 *
 * On each read of the LPTMR counter register, software must first write to the
 * LPTMR counter register with any value. This will synchronize and register the
 * current value of the LPTMR counter register into a temporary register. The
 * contents of the temporary register are returned on each read of the LPTMR
 * counter register.
 */
uint16 get_counter_value()
{
	LPTMR0_CNR = 0x1;
	return LPTMR0_CNR;
}

/* Zero out all registers. */
void lptmr_clear_registers()
{
	LPTMR0_CSR = 0x00;
	LPTMR0_PSR = 0x00;
	LPTMR0_CMR = 0x00;
}
