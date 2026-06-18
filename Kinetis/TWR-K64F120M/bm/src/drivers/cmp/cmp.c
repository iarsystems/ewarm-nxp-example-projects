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
#include "cmp.h"
#include "arm_cm4.h"

/*
 * Initialize the CMP for operation, interrupts, clock select, input select
 *
 * NOTE: Since the Comparitors are pinned out in multiple locations on most
 *		 Kinetis devices, this driver does not enable CMP pin functions.
 *		 The desired pins should be enabled before calling this init function.
 *
 * Parameters:
 *	cmpch			- CMP channel to initialize
 *	rising_falling	- equal to bit mask in the CMP_SCR Register
 *					  CMP_SCR_IER_MASK or CMP_SCR_IEF_MASK
 *
 * the reset of these inputs  are not yet implemented
 *	clk		- CMP module Clock
 *	inp		- cmp pos input selection
 *	inn		- cmp neg input selection
 *	outen	- cmp output enable
 *	dacval	- cmp dac input setting.
 */
void cmp_init(CMP_MemMapPtr cmpch, unsigned char rising_falling)
{
	/* Filter and digital hysteresis disabled */
	CMP_CR0_REG(cmpch) = 0x00;
	/*
	 * no sample(continuous), no window, -, HS mode,
	 * no invert, COUTA(unfiltered), output pin enable, moduledisable
	 */
	CMP_CR1_REG(cmpch) = CMP_CR1_PMODE_MASK |
						 CMP_CR1_COS_MASK |
						 CMP_CR1_OPE_MASK;			/*0x16; */
	/* Filter disabled */
	CMP_FPR_REG(cmpch) = 0x00;
	/* Enable rising or falling edge interrupts, clear flags */
	CMP_SCR_REG(cmpch) = rising_falling |
						 CMP_SCR_CFR_MASK |
						 CMP_SCR_CFF_MASK;			/*0x1E */
	/* 6-bit DAC enabled, output set to 1/2-scale, Vdd reference */
	CMP_DACCR_REG(cmpch) = CMP_DACCR_DACEN_MASK |
						   CMP_DACCR_VRSEL_MASK |
						   CMP_DACCR_VOSEL(0x20);	/*0xE0; */
	/* P-input as external channel 0, M-input as 6b DAC */
	CMP_MUXCR_REG(cmpch) = CMP_MUXCR_PSEL(0) | CMP_MUXCR_MSEL(7);	/*0x07; */

	/* enable CMP */
	CMP_CR1_REG(cmpch) |= CMP_CR1_EN_MASK;
}

void cmp0_isr(void)
{
	GPIOB_PSOR = 0x00080000;	/* set Port B 19 indicate wakeup */

	printf("\n[CMP0_ISR]****ISR entered*****\r\n\n");
	printf("[CMP0_ISR]CMP0_SCR   = %#02X ", (CMP0_SCR));
	if (CMP0_SCR & CMP_SCR_CFR_MASK) {
		CMP0_SCR |= CMP_SCR_CFR_MASK;
		printf(" CMP0_CFR set and is now clear \n");
		printf("[CMP0_ISR]CMP0_SCR   = %#02X ", (CMP0_SCR));
	}
	if (CMP0_SCR & CMP_SCR_CFF_MASK) {
		CMP0_SCR |= CMP_SCR_CFF_MASK;
		printf(" CMP0_CFF set and is now clear \n");
		printf("[CMP0_ISR]CMP0_SCR   = %#02X ", (CMP0_SCR));
	}
	return;
}

void cmp1_isr(void)
{
	GPIOB_PSOR = 0x00080000;	/* set Port B 19 indicate wakeup */

	printf("\n[CMP1_ISR]****ISR entered*****\r\n\n");
	printf("[CMP1_ISR]CMP1_SCR   = %#02X ", (CMP1_SCR));
	if (CMP1_SCR & CMP_SCR_CFR_MASK) {
		CMP1_SCR |= CMP_SCR_CFR_MASK;
		printf("[CMP1_ISR] CMP0_CFR set and is now clear \n");
		printf("[CMP1_ISR]CMP1_SCR   = %#02X ", (CMP1_SCR));
	}
	if (CMP1_SCR & CMP_SCR_CFF_MASK) {
		CMP1_SCR |= CMP_SCR_CFF_MASK;
		printf("[CMP1_ISR] CMP0_CFF set and is now clear \n");
		printf("[CMP1_ISR]CMP1_SCR   = %#02X ", (CMP1_SCR));
	}
	return;
}

void cmp2_isr(void)
{
	GPIOB_PSOR = 0x00080000;	/* set Port B 19 indicate wakeup */

	printf("\n[CMP2_ISR]****ISR entered*****\r\n\n");
	printf("[CMP2_ISR]CMP2_SCR   = %#02X ", (CMP2_SCR));

	if (CMP2_SCR & CMP_SCR_CFR_MASK) {
		CMP2_SCR |= CMP_SCR_CFR_MASK;
		printf("[CMP2_ISR] CMP0_CFR set and is now clear \n");
		printf("[CMP2_ISR]CMP2_SCR   = %#02X ", (CMP2_SCR));
	}
	if (CMP2_SCR & CMP_SCR_CFF_MASK) {
		CMP2_SCR |= CMP_SCR_CFF_MASK;
		printf("[CMP2_ISR] CMP0_CFF set and is now clear \n");
		printf("[CMP2_ISR]CMP2_SCR   = %#02X ", (CMP2_SCR));
	}
	return;
}
