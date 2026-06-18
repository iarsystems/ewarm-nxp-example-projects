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
#include "adc16.h"		/* ADC driver defines */
#include "fpu.h"


extern void rtc_init(uint32 seconds, uint32 alarm, uint8 c_interval,
					 uint8 c_value, uint8 interrupt);
extern void led_test(int a, char **b);
extern void i2c_demo(int a, char **b);
extern void i2c_oobe_demo(int a, char **b);
extern void adc_pdb_demo(int a, char **b);
extern void irq_demo(int a, char **b);
extern void usb_device_demo(void);
extern void usb_host_demo(void);
extern void esdhc_test(void);

/* Defines */
#define __ADC_PDB_H		1
#define GPIO_PIN_MASK		0x1Fu
#define GPIO_PIN(x)			(((1)<<(x & GPIO_PIN_MASK)))

#define USEC				10		/* 10 usec for PIT */

#define HW_FPU

/* global variable */
uint32_t irq_sw;
uint32_t delay_ms;

/* This is the structure that contains the desired ADC/PGA configuration. */
tADC_Config Master_Adc_Config;
extern uint8_t Hw_Trig_Test(void);

void msec_delay(uint32_t ms)
{
	uint32_t start = 0, ticks_msec;

	ticks_msec = 1000 / USEC;		/* ticks per msec */
	ticks_msec *= ms;

	start = delay_ms;

	while (1) {
		if ((delay_ms - start) >= ticks_msec)
			break;
	}
}

void usec_delay(uint32_t us)
{
	uint32_t start = 0, ticks_usec;

	if (us > USEC) {
		ticks_usec = us / USEC;		/* ticks per usec */

		start = delay_ms;

		while (1) {
			if ((delay_ms - start) >= ticks_usec)
				break;
		}
	} else {
		printf("Value must be greater than %d\n", USEC);
	}
}

/*
 * Initialize GPIO for Tower switches and LED's
 *
 * PTA4 - SW3
 * PTC6 - SW1
 *
 * PTE6 - Green (D5)
 * PTE7 - Yellow (D6)
 * PTE8 - Orange (D8) - not used, UART5
 * PTE9 - Blue (D9) - not used, UART5
 */
void init_gpio(void)
{
	/* Turn on all port clocks */
	SIM_SCGC5 |=
	    SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK |
	    SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	/* Initialize GPIO connected to LEDs */
#ifndef UART5_ALT2
	PORTE_PCR9 = PORT_PCR_MUX(1);
	PORTE_PCR8 = PORT_PCR_MUX(1);
#endif
	PORTE_PCR7 = PORT_PCR_MUX(1);
	PORTE_PCR6 = PORT_PCR_MUX(1);

	/*
	 * Initialize GPIO connected to pushbutton switches
	 * SW1 and SW3 and enable internal pull-up
	 */
	PORTC_PCR6 = (PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
	PORTA_PCR4 = (PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);

	/* Set as an output */
	GPIOE_PDDR = ((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
}

void pit_isr(void)
{
	PIT_TFLG0 = 0x01;	/* clear flag */
	PIT_TCTRL0;			/* dummy read to ensure the interrupt, */
	delay_ms++;
}

void pit_init(unsigned long uSec)
{
	uint32_t bus_clk, temp;
	float bus_clk_ns;

	delay_ms = 0;

	bus_clk = (CLK0_FREQ_HZ / ((MCG_C5 & 0x1F) + 1));
	bus_clk = bus_clk * ((MCG_C6 & 0x1F) + 24);
	bus_clk >>= 1;

	bus_clk_ns = ((float)1000000000 / (float)bus_clk);
	uSec *= 1000;		/* make it nS */
	bus_clk_ns = (float)uSec / bus_clk_ns;
	temp = (uint32_t)bus_clk_ns;

	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;	/* enable PIT module */

	PIT_TCTRL0 = 0x00;					/* disable PIT0 */
	PIT_LDVAL0 = temp;
	PIT_TCTRL0 = PIT_TCTRL_TIE_MASK;	/* enable PIT0 and interrupt */
	PIT_TFLG0 = 0x01;					/* clear flag */
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;

	enable_irq(INT_PIT0 - 16);

	PIT_MCR = 0x00;
}

void main(void)
{
	irq_sw = 0;

#ifdef HW_FPU
	__enable_FPU();
#endif

	init_gpio();
	rtc_init(0, 10, 0, 0, INT_RTC);
	pit_init(USEC);		/* 100 us */

	printf("\nOOBE demo for the %s %d MHz family\n\n",
		   TWR_STRING, TWR_SYSCLOCK);

	GPIOE_PDOR |= ((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));	/* clear LEDs */

	irq_demo(0, 0);

	led_test(0, 0);
	Hw_Trig_Test();

	i2c_demo(0, 0);

	esdhc_test();

	usb_device_demo();
	usb_host_demo();

	// Must use TWR-SER1 or TWR-SER2
	//ethernet_demo();

	while (1) {
	}
}
