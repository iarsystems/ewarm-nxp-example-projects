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
#include "llwu.h"
#include "mcg.h"
#include "uart.h"
#include "rcm.h"

extern int re_init_clk;
extern int mcg_clk_hz;
extern int mcg_clk_khz;
extern int core_clk_khz;

void llwu_reset_enable(void)
{
	printf(" LLWU Reset pin enabled as wakeup source from Low power modes \n");
	LLWU_RST = LLWU_RST_LLRSTE_MASK;	/*no reset filter for now */
}

/*
 * function: llwu_configure
 *
 * description: Set up the LLWU for wakeup the MCU from LLS and VLLSx modes
 * from the selected pin or module.
 *
 * inputs:
 * pin_en - unsigned integer, bit position indicates the pin is enabled.
 *			More than one bit can be set to enable more than one pin at a time.
 *
 * rise_fall - 0x00 = External input disabled as wakeup
 *			   0x01 - External input enabled as rising edge detection
 *			   0x02 - External input enabled as falling edge detection
 *			   0x03 - External input enablge as any edge detection
 * module_en - unsigned char, bit position indicates the module is enabled.
 *			   More than one bit can be set to enabled more than one module
 *
 * for example:	if bit 0 and 1 need to be enabled as rising edge detect call
 *				this  routine with pin_en = 0x0003 and rise_fall = 0x02
 *
 * Note: to set up one set of pins for rising and another for falling, 2 calls
 *		 to this function are required, 1st for rising then the second for
 *		 falling.
 */
void llwu_configure(unsigned int pin_en,
					unsigned char rise_fall,
					unsigned char module_en)
{
	uint8 temp;

	temp = LLWU_PE1;
	if (pin_en & 0x0001) {
		temp |= LLWU_PE1_WUPE0(rise_fall);
		printf(" LLWU configured pins PTE1/UART1_RX/I2C1_SCL /SPI1_SIN "
			   "to be an LLWU wakeup source\n");
		LLWU_F1 |= LLWU_F1_WUF0_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x0002) {
		temp |= LLWU_PE1_WUPE1(rise_fall);
		printf(" LLWU configured pins PTE2/SPI1_SCK/SDHC0_DCLK to be "
			   "an LLWU wakeup source\n");
		LLWU_F1 |= LLWU_F1_WUF1_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x0004) {
		temp |= LLWU_PE1_WUPE2(rise_fall);
		printf(" LLWU configured pins PTE4/SPI1_PCS0/SDHC0_D3 to be an "
			   "LLWU wakeup source\n");
		LLWU_F1 |= LLWU_F1_WUF2_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x0008) {
		temp |= LLWU_PE1_WUPE3(rise_fall);
		printf(" LLWU configured pins PTA4/FTM0_CH1/NMI/EZP_CS to be an "
			   "LLWU wakeup source\n");
		LLWU_F1 |= LLWU_F1_WUF3_MASK;	/* write one to clear the flag */
	}
	LLWU_PE1 = temp;

	temp = LLWU_PE2;
	if (pin_en & 0x0010) {
		temp |= LLWU_PE2_WUPE4(rise_fall);
		printf(" LLWU configured pins PTA13/FTM1_CH1 /FTM1_QD_PHB to be an "
			   "LLWU wakeup source\n");
		LLWU_F1 |= LLWU_F1_WUF4_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x0020) {
		temp |= LLWU_PE2_WUPE5(rise_fall);
		printf(" LLWU configured pins PTB0/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA to "
			   "be an LLWU wakeup source\n");
		LLWU_F1 |= LLWU_F1_WUF5_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x0040) {
		temp |= LLWU_PE2_WUPE6(rise_fall);
		printf(" LLWU configured pins PTC1/UART1_RTS/FTM0_CH0 to be an LLWU "
			   "wakeup source\n");
		LLWU_F1 |= LLWU_F1_WUF6_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x0080) {
		temp |= LLWU_PE2_WUPE7(rise_fall);
		printf(" LLWU configured pins PTC3/UART1_RX/FTM0_CH2 to be an LLWU "
			   "wakeup source\n");
		LLWU_F1 |= LLWU_F1_WUF7_MASK;	/* write one to clear the flag */
	}
	LLWU_PE2 = temp;

	temp = LLWU_PE3;
	if (pin_en & 0x0100) {
		temp |= LLWU_PE3_WUPE8(rise_fall);
		printf(" LLWU configured pins PTC4/SPI0_PCS0/FTM0_CH3 to be an LLWU "
			   "wakeup source\n");
		LLWU_F2 |= LLWU_F2_WUF8_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x0200) {
		temp |= LLWU_PE3_WUPE9(rise_fall);
		printf(" LLWU configured pins PTC5/SPI0_SCK/I2S0_RXD0 to be an LLWU "
			   "wakeup source\n");
		LLWU_F2 |= LLWU_F2_WUF9_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x0400) {
		temp |= LLWU_PE3_WUPE10(rise_fall);
		printf(" LLWU configured pins PTC6/PDB0_EXTRG to be an LLWU "
			   "wakeup source\n");
		LLWU_F2 |= LLWU_F2_WUF10_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x0800) {
		temp |= LLWU_PE3_WUPE11(rise_fall);
		printf(" LLWU configured pins PTC11/I2S0_RXD1 to be an LLWU "
			   "wakeup source\n");
		LLWU_F2 |= LLWU_F2_WUF11_MASK;	/* write one to clear the flag */
	}
	LLWU_PE3 = temp;

	temp = LLWU_PE4;
	if (pin_en & 0x1000) {
		temp |= LLWU_PE4_WUPE12(rise_fall);
		printf(" LLWU configured pins PTD0/SPI0_PCS0/UART2_RTS to be an "
			   "LLWU wakeup source\n");
		LLWU_F2 |= LLWU_F2_WUF12_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x2000) {
		temp |= LLWU_PE4_WUPE13(rise_fall);
		printf(" LLWU configured pins PTD2/UART2_RX to be an LLWU wakeup "
			   "source \n");
		LLWU_F2 |= LLWU_F2_WUF13_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x4000) {
		temp |= LLWU_PE4_WUPE14(rise_fall);
		printf(" LLWU configured pins PTD4/UART0_RTS/FTM0_CH4/EWM_IN to "
			   "be an LLWU wakeup source\n");
		LLWU_F2 |= LLWU_F2_WUF14_MASK;	/* write one to clear the flag */
	}
	if (pin_en & 0x8000) {
		temp |= LLWU_PE4_WUPE15(rise_fall);
		printf(" LLWU configured pins PTD6/UART0_RX/FTM0_CH6/FTM0_FLT0 to "
			   "be an LLWU wakeup source \n");
		LLWU_F2 |= LLWU_F2_WUF15_MASK;	/* write one to clear the flag */
	}
	LLWU_PE4 = temp;

	LLWU_ME = module_en;		/*Set up modules to wakeup up */

	printf("LLWU PE1   = 0x%02X,    ", (LLWU_PE1));
	printf("LLWU PE2   = 0x%02X\n", (LLWU_PE2));
	printf("LLWU PE3   = 0x%02X,    ", (LLWU_PE3));
	printf("LLWU PE4   = 0x%02X\n", (LLWU_PE4));
	printf("LLWU ME    = 0x%02X,    ", (LLWU_ME));
	printf("LLWU F1    = 0x%02X\n", (LLWU_F1));
	printf("LLWU F2    = 0x%02X,    ", (LLWU_F2));
	printf("LLWU F3    = 0x%02X\n", (LLWU_F3));
	printf("LLWU FILT1 = 0x%02X,    ", (LLWU_FILT1));
	printf("LLWU FILT2 = 0x%02X\n", (LLWU_FILT2));
	printf("LLWU RST   = 0x%02X\n", (LLWU_RST));
	/*function ends */
}

void llwu_configure_filter(unsigned int wu_pin_num,
						   unsigned char filter_en,
						   unsigned char rise_fall)
{
	/*
	 * wu_pin_num is the pin number to be written to FILTSEL.
	 * wu_pin_num is not the same as pin_en.
	 */
	uint8 temp;

	printf("\nEnabling Filter %x on WU Pin %x for WU sense %x \n",
		   filter_en, wu_pin_num, rise_fall);

	temp = 0;
	/*first clear filter values and clear flag by writing a 1 */
	LLWU_FILT1 = LLWU_FILT1_FILTF_MASK;
	LLWU_FILT2 = LLWU_FILT2_FILTF_MASK;

	if (filter_en == 1) {
		/*clear the flag bit and set the others */
		temp |= (LLWU_FILT1_FILTF_MASK) |
				(LLWU_FILT1_FILTE(rise_fall) |
				 LLWU_FILT1_FILTSEL(wu_pin_num));
		LLWU_FILT1 = temp;
	} else if (filter_en == 2) {
		/*clear the flag bit and set the others */
		temp |= (LLWU_FILT2_FILTF_MASK) |
				(LLWU_FILT2_FILTE(rise_fall) |
				 LLWU_FILT2_FILTSEL(wu_pin_num));
		LLWU_FILT2 = temp;
	} else {
		printf("\nError - invalid filter number %x\n", filter_en);
	}
}

void llwu_isr(void)
{
	char ch;

//	GPIOE_PSOR = 0x04000000;		/* set Port E 26 indicate wakeup - set it in llwu_isr */
//	GPIOE_PCOR = 0x04000000;		/* clear Port E 26 indicating sleep */
//	GPIOE_PSOR = 0x04000000;		/* set Port E 26 indicate wakeup - set it in llwu_isr */
//	GPIOE_PCOR = 0x04000000;		/* clear Port E 26 indicating  wakeup from sleep */
//	GPIOB_PSOR = 0x00080000;		/* set Port B19 */
//	GPIOB_PCOR = 0x00080000;		/* clear Port B19 */
//	GPIOB_PSOR = 0x00080000;		/* set Port B19 */
//	GPIOB_PCOR = 0x00080000;		/* clear Port B19 */

//	outsrs();
	if (LLWU_F1 & LLWU_F1_WUF0_MASK) {
		LLWU_F1 |= LLWU_F1_WUF0_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F1 & LLWU_F1_WUF1_MASK) {
		LLWU_F1 |= LLWU_F1_WUF2_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F1 & LLWU_F1_WUF2_MASK) {
		LLWU_F1 |= LLWU_F1_WUF2_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F1 & LLWU_F1_WUF3_MASK) {
		LLWU_F1 |= LLWU_F1_WUF3_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F1 & LLWU_F1_WUF4_MASK) {
		LLWU_F1 |= LLWU_F1_WUF4_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F1 & LLWU_F1_WUF5_MASK) {
		LLWU_F1 |= LLWU_F1_WUF5_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F1 & LLWU_F1_WUF6_MASK) {
		LLWU_F1 |= LLWU_F1_WUF6_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F1 & LLWU_F1_WUF7_MASK) {
		LLWU_F1 |= LLWU_F1_WUF7_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F2 & LLWU_F2_WUF8_MASK) {
		LLWU_F2 |= LLWU_F2_WUF8_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F2 & LLWU_F2_WUF9_MASK) {
		LLWU_F2 |= LLWU_F2_WUF9_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F2 & LLWU_F2_WUF10_MASK) {
		LLWU_F2 |= LLWU_F2_WUF10_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F2 & LLWU_F2_WUF11_MASK) {
		LLWU_F2 |= LLWU_F2_WUF11_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F2 & LLWU_F2_WUF12_MASK) {
		LLWU_F2 |= LLWU_F2_WUF12_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F2 & LLWU_F2_WUF13_MASK) {
		LLWU_F2 |= LLWU_F2_WUF13_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F2 & LLWU_F2_WUF14_MASK) {
		SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;	/* turn on port D ajj per alister */
		LLWU_F2 |= LLWU_F2_WUF14_MASK;		/* write one to clear the flag */
		PORTD_PCR4 |= PORT_PCR_ISF_MASK;	/*  clear Flag if there */
	}
	if (LLWU_F2 & LLWU_F2_WUF15_MASK) {
		SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;	/* turn on port D ajj per alister */
		ch = uart_getchar(UART0_BASE_PTR);
		uart_putchar(UART0_BASE_PTR, ch);
		LLWU_F2 |= LLWU_F2_WUF15_MASK;		/* write one to clear the flag */
		PORTD_PCR6 |= PORT_PCR_ISF_MASK;	/*  clear Flag if there */
	}
	if (LLWU_F3 & LLWU_F3_MWUF0_MASK) {
		LLWU_F3 |= LLWU_F3_MWUF0_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F3 & LLWU_F3_MWUF1_MASK) {
		LLWU_F3 |= LLWU_F3_MWUF1_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F3 & LLWU_F3_MWUF2_MASK) {
		LLWU_F3 |= LLWU_F3_MWUF2_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F3 & LLWU_F3_MWUF3_MASK) {
		LLWU_F3 |= LLWU_F3_MWUF3_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F3 & LLWU_F3_MWUF4_MASK) {
		LLWU_F3 |= LLWU_F3_MWUF4_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F3 & LLWU_F3_MWUF5_MASK) {
		LLWU_F3 |= LLWU_F3_MWUF5_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F3 & LLWU_F3_MWUF6_MASK) {
		LLWU_F3 |= LLWU_F3_MWUF6_MASK;		/* write one to clear the flag */
	}
	if (LLWU_F3 & LLWU_F3_MWUF7_MASK) {
		LLWU_F3 |= LLWU_F3_MWUF7_MASK;		/* write one to clear the flag */
	}

	if (LLWU_FILT1 & LLWU_FILT1_FILTF_MASK) {
		LLWU_FILT1 |= LLWU_FILT1_FILTF_MASK;
	}
	if (LLWU_FILT2 & LLWU_FILT2_FILTF_MASK) {
		LLWU_FILT2 |= LLWU_FILT2_FILTF_MASK;
	}
}
