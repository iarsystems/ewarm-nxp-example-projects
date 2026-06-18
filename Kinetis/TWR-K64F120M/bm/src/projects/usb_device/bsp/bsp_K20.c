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
#include "bsp_k20.h"

void vfnfll_init(void);
void fll_init(void);

void vfnInitUSBClock(uint8 u8ClkOption)
{

	switch (u8ClkOption) {
	case MCGPLL0:
		{
			/*
			 * PLL reference
			 * USB fractional divider like USB reference clock
			 */
			SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK |
						 SIM_SOPT2_USBSRC_MASK;

			/* Divide reference clock by two to obtain 48MHz */
			SIM_CLKDIV2 |= (USB_FRAC | USB_DIV);
			break;
		}

	case MCGFLL:
		{
			/* Configure FLL to generate a 48MHz core clock */
			fll_init();

			/* FLL reference */
			SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;
			/* USB fractional divider like USB reference clock */
			SIM_SOPT2 |= SIM_SOPT2_USBSRC_MASK;
			/* Divide reference clock by one to obtain 48MHz */
			SIM_CLKDIV2 &= ~(SIM_CLKDIV2_USBDIV_MASK |
							 SIM_CLKDIV2_USBFRAC_MASK);
			break;
		}

	case CLKIN:
		{
			/* PTA5 selected as USBFS CLK source */
			SIM_SOPT2 &= (uint32) (~SIM_SOPT2_USBSRC_MASK);
			FLAG_SET(SIM_SCGC5_PORTA_SHIFT, SIM_SCGC5);
			/* Enabling PTA5 as CLK input */
			PORTA_PCR25 = (0 | PORT_PCR_MUX(2));
			break;
		}

	case IRC48M:
		{
			SIM_SCGC4 |= (SIM_SCGC4_USBOTG_MASK);	/* USB Clock Gating */

			USB0_USBTRC0 |= USB_USBTRC0_USBRESET_MASK;
			while (FLAG_CHK(USB_USBTRC0_USBRESET_SHIFT, USB0_USBTRC0)) {
			};

			SIM_SOPT2  |= SIM_SOPT2_PLLFLLSEL(3);
			SIM_SOPT2  |= SIM_SOPT2_USBSRC_MASK;

			USB0_CLK_RECOVER_IRC_EN |= USB_CLK_RECOVER_IRC_EN_IREC_EN_MASK;
			USB0_CLK_RECOVER_CTRL |= USB_CLK_RECOVER_CTRL_CLOCK_RECOVER_EN_MASK;
		}
	}

	SIM_SCGC4 |= (SIM_SCGC4_USBOTG_MASK);	/* USB Clock Gating */
}

void fll_init(void)
{
/*
	pee_pbe(CLK0_FREQ_HZ);
	pbe_fbe(CLK0_FREQ_HZ);
	fbe_fee(CLK0_FREQ_HZ);
*/
}

/*
FLL output like bus clock source, divide reference by 256 (8MHz /256 = 31.25kHz)
	MCG_C1 = MCG_C1_CLKS(0) | MCG_C1_FRDIV(3);

	while (!(MCG_S & MCG_S_OSCINIT_MASK)){};	// wait for oscillator to initialize
	while (MCG_S & MCG_S_IREFST_MASK){};		// wait for Reference clock Status bit to clear
*/

/*
Fixed DCO multiplier of 1464, MCGOUT = 32.768 kHz * 1464 / 1 = 48 MHz
	MCG_C4 = MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(1);
	while (MCG_S & MCG_S_CLKST_MASK){};			// Wait for output of FLL
*/
