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
 * File:	sysinit.c
 * Purpose:	Kinetis Configuration
 *			Initializes processor to a default state
 *
 * Notes:
 */

#include "common.h"
#include "sysinit.h"
#include "uart.h"
#include "mcg.h"

/* Actual system clock frequency */
int mcg_clk_hz;
int mcg_clk_khz;
int core_clk_khz;
int core_clk_mhz;
int periph_clk_khz;
int flash_clk_khz;

void sysinit(void)
{
	/*
	 * Enable all of the port clocks. These have to be enabled to configure
	 * pin muxing options, so most code will need all of these on anyway.
	 */
	SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK |
				  SIM_SCGC5_PORTB_MASK |
				  SIM_SCGC5_PORTC_MASK |
				  SIM_SCGC5_PORTD_MASK |
				  SIM_SCGC5_PORTE_MASK);

	/*
	 * Ramp up the system clock
	 * Set the system dividers
	 * NOTE: The PLL init will not configure the system clock dividers,
	 * so they must be configured appropriately before calling the PLL
	 * init function to ensure that clocks remain in valid ranges.
	 */
	if (PMC_REGSC & PMC_REGSC_ACKISO_MASK)
		PMC_REGSC |= PMC_REGSC_ACKISO_MASK;

	SIM_CLKDIV1 = (0 | SIM_CLKDIV1_OUTDIV1(0) |	/* Core/system	120 Mhz */
				   SIM_CLKDIV1_OUTDIV2(1) |		/* Busclk		60 Mhz */
				   SIM_CLKDIV1_OUTDIV3(2) |		/* FlexBus		40 Mhz */
				   SIM_CLKDIV1_OUTDIV4(4));		/* Flash		24 Mhz */

	/*
	 * Initialize PLL
	 * PLL will be the source for MCG CLKOUT so the core, system,
	 * and flash clocks are derived from it
	 */
	mcg_clk_hz = pll_init(CLK0_FREQ_HZ,	/* CLKIN0 frequency */
						  LOW_POWER,	/* Set the oscillator for low power mode */
						  CLK0_TYPE,	/* Crystal or canned oscillator clock input */
						  PLL0_PRDIV,	/* PLL predivider value */
						  PLL0_VDIV,	/* PLL multiplier */
						  MCGOUT);		/* Use the output from this PLL as the MCGOUT */

	/*
	 * Check the value returned from pll_init() to make sure
	 * there wasn't an error
	 */
	if (mcg_clk_hz < 0x100)
		while (1) ;

	/*
	 * Use the value obtained from the pll_init function to define variables
	 * for the core clock in kHz and also the peripheral clock. These
	 * variables can be used by other functions that need awareness of the
	 * system frequency.
	 */
	mcg_clk_khz = mcg_clk_hz / 1000;
	core_clk_khz =
		mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> 28) + 1);
	periph_clk_khz =
		mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24) + 1);
	/*
	 * For debugging purposes, enable the trace clock and/or FB_CLK so that
	 * we'll be able to monitor clocks and know the PLL is at the frequency
	 * that we expect.
	 */

	trace_clk_init();
	clkout_init();

	/*
	 * Enable pin interrupt for the abort button - PTA4
	 * This pin could also be used as the NMI interrupt, but since the NMI
	 * is level sensitive each button press will cause multiple interrupts.
	 * Using the GPIO interrupt instead means we can configure for an edge
	 * sensitive interrupt instead = one interrupt per button press.
	 */
	//enable_abort_button();

	/* Enable the pins for the selected UART */
	if (TERM_PORT == UART0_BASE_PTR) {
#ifdef UART0_ALT1
		PORTA_PCR1 = PORT_PCR_MUX(2);	/* RX */
		PORTA_PCR2 = PORT_PCR_MUX(2);	/* TX */
#endif
#ifdef UART0_ALT2
		PORTA_PCR14 = PORT_PCR_MUX(3);	/* TX */
		PORTA_PCR15 = PORT_PCR_MUX(3);	/* RX */
#endif
#ifdef UART0_ALT3
		PORTB_PCR16 = PORT_PCR_MUX(3);	/* TX */
		PORTB_PCR17 = PORT_PCR_MUX(3);	/* RX */
#endif
#ifdef UART0_ALT4
		PORTD_PCR6 = PORT_PCR_MUX(3);	/* TX */
		PORTD_PCR7 = PORT_PCR_MUX(3);	/* RX */
#endif

#ifdef HW_FLOW_CONTROL
		PORTA_PCR0 = PORT_PCR_MUX(2);	/* CTS */
		PORTA_PCR3 = PORT_PCR_MUX(2);	/* RTS */
#endif
#ifdef HW_FLOW_CONTROL1
		PORTA_PCR16 = PORT_PCR_MUX(3);	/* CTS */
		PORTA_PCR17 = PORT_PCR_MUX(3);	/* RTS */
#endif
#ifdef HW_FLOW_CONTROL2
		PORTB_PCR2 = PORT_PCR_MUX(3);	/* RTS */
		PORTB_PCR3 = PORT_PCR_MUX(3);	/* CTS */
#endif
#ifdef HW_FLOW_CONTROL3
		PORTD_PCR4 = PORT_PCR_MUX(3);	/* RTS */
		PORTD_PCR5 = PORT_PCR_MUX(3);	/* CTS */
#endif
	}

	if (TERM_PORT == UART1_BASE_PTR) {
#ifdef UART1_ALT1
		PORTC_PCR3 = PORT_PCR_MUX(3);	/* RX */
		PORTC_PCR4 = PORT_PCR_MUX(3);	/* TX */
#endif
#ifdef UART1_ALT2
		PORTE_PCR0 = PORT_PCR_MUX(3);	/* TX */
		PORTE_PCR1 = PORT_PCR_MUX(3);	/* RX */
#endif
#ifdef HW_FLOW_CONTROL
		PORTC_PCR1 = PORT_PCR_MUX(3);	/* CTS */
		PORTC_PCR2 = PORT_PCR_MUX(3);	/* RTS */
#endif
#ifdef HW_FLOW_CONTROL1
		PORTE_PCR2 = PORT_PCR_MUX(3);	/* CTS */
		PORTE_PCR3 = PORT_PCR_MUX(3);	/* RTS */
#endif
	}

	if (TERM_PORT == UART2_BASE_PTR) {
		PORTD_PCR2 = PORT_PCR_MUX(3);	/* RX */
		PORTD_PCR3 = PORT_PCR_MUX(3);	/* TX */
#ifdef HW_FLOW_CONTROL
		PORTD_PCR0 = PORT_PCR_MUX(2);	/* RTS */
		PORTD_PCR1 = PORT_PCR_MUX(2);	/* CTS */
#endif
}

	if (TERM_PORT == UART3_BASE_PTR) {
#ifdef UART3_ALT1
		PORTB_PCR10 = PORT_PCR_MUX(3);	/* RX */
		PORTB_PCR11 = PORT_PCR_MUX(3);	/* TX */
#endif
#ifdef UART3_ALT2
		PORTC_PCR16 = PORT_PCR_MUX(3);	/* RX */
		PORTC_PCR17 = PORT_PCR_MUX(3);	/* TX */
#endif
#ifdef UART3_ALT3
		PORTE_PCR4 = PORT_PCR_MUX(3);	/* TX */
		PORTE_PCR5 = PORT_PCR_MUX(3);	/* RX */
#endif
#ifdef HW_FLOW_CONTROL
		PORTB_PCR8 = PORT_PCR_MUX(3);	/* RTS */
		PORTB_PCR9 = PORT_PCR_MUX(3);	/* CTS */
#endif
#ifdef HW_FLOW_CONTROL1
		PORTB_PCR12 = PORT_PCR_MUX(2);	/* RTS */
		PORTB_PCR13 = PORT_PCR_MUX(2);	/* CTS */
#endif
#ifdef HW_FLOW_CONTROL2
		PORTC_PCR18 = PORT_PCR_MUX(3);	/* RTS */
		PORTC_PCR19 = PORT_PCR_MUX(3);	/* CTS */
#endif
#ifdef HW_FLOW_CONTROL3
		PORTE_PCR6 = PORT_PCR_MUX(3);	/* CTS */
		PORTE_PCR7 = PORT_PCR_MUX(3);	/* RTS */
#endif
	}

	if (TERM_PORT == UART4_BASE_PTR) {
#ifdef UART4_ALT1
		PORTC_PCR14 = PORT_PCR_MUX(3);	/* RX */
		PORTC_PCR15 = PORT_PCR_MUX(3);	/* TX */
#endif
#ifdef UART4_ALT2
		PORTE_PCR24 = PORT_PCR_MUX(3);	/* TX */
		PORTE_PCR25 = PORT_PCR_MUX(3);	/* RX */
#endif
#ifdef HW_FLOW_CONTROL
		PORTC_PCR12 = PORT_PCR_MUX(3);	/* RTS */
		PORTC_PCR13 = PORT_PCR_MUX(3);	/* CTS */
#endif
#ifdef HW_FLOW_CONTROL1
		PORTE_PCR26 = PORT_PCR_MUX(3);	/* CTS */
		PORTE_PCR27 = PORT_PCR_MUX(3);	/* RTS */
#endif
	}

	if (TERM_PORT == UART5_BASE_PTR) {
#ifdef UART5_ALT1
		PORTD_PCR8 = PORT_PCR_MUX(3);	/* RX */
		PORTD_PCR9 = PORT_PCR_MUX(3);	/* TX */
#endif
#ifdef UART5_ALT2
		PORTE_PCR8 = PORT_PCR_MUX(3);	/* TX */
		PORTE_PCR9 = PORT_PCR_MUX(3);	/* RX */
#endif
#ifdef HW_FLOW_CONTROL
		PORTD_PCR10 = PORT_PCR_MUX(3);	/* RTS */
		PORTD_PCR11 = PORT_PCR_MUX(3);	/* CTS */
#endif
#ifdef HW_FLOW_CONTROL1
		PORTE_PCR10 = PORT_PCR_MUX(3);	/* CTS */
		PORTE_PCR11 = PORT_PCR_MUX(3);	/* RTS */
#endif
	}

	/*
	 * UART0 and UART1 are clocked from the core clock, but all other UARTs are
	 * clocked from the peripheral clock. So we have to determine which clock
	 * to send to the uart_init function.
	 */

	if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
		uart_init(TERM_PORT, core_clk_khz, TERMINAL_BAUD);
	else
		uart_init(TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
}

void trace_clk_init(void)
{
	/* Set the trace clock to the core clock frequency */
	SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;

	/*
	 * Enable the TRACE_CLKOUT pin function on PTE0 (alt5 function)
	 * enable high drive strength to support high toggle rate
	 */
	PORTE_PCR0 = (PORT_PCR_MUX(0x5) | PORT_PCR_DSE_MASK);
}

void clkout_init(void)
{
	/* Enable the clock to the FlexBus module */
	SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

	/*
	 * Enable the FB_CLKOUT function on PTC3 (alt5 function)
	 * enable high drive strength to support high toggle rate
	 */
	PORTC_PCR3 = (PORT_PCR_MUX(0x5)
				  | PORT_PCR_DSE_MASK);
}

void enable_abort_button(void)
{
	/* Configure the PTA4 pin for its GPIO function */
	PORTA_PCR4 = PORT_PCR_MUX(0x1);	/* GPIO is alt1 function for this pin */

	/* Configure the PTA4 pin for rising edge interrupts */
	PORTA_PCR4 |= PORT_PCR_IRQC(0x9);

	/* Enable the associated IRQ in the NVIC */
	enable_irq(87);
}
