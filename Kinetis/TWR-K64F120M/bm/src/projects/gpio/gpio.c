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
 * File:		gpio.c
 * Purpose:		LED and Switch Example
 *
 *		Configures GPIO for the LED and push buttons on the TWR-K64F120M
 *		Blue LED - On
 *		Green LED - Toggles on/off
 *		Orange LED - On if SW3 pressed
 *		Yellow LED - On if SW1 pressed
 *
 *		Also configures push buttons for falling IRQ's. ISR
 *		configured in vector table in isr.h
 */

#define GPIO_PIN_MASK			0x1Fu
#define GPIO_PIN(x)				(((1)<<(x & GPIO_PIN_MASK)))

#include "common.h"
#include "lptmr.h"

/* Function declarations */
void portd_isr(void);
void porte_isr(void);
void init_gpio(void);

extern void lptmr_init(void);

void test_allgpios(void)
{
	unsigned long *port, *gpio, *tgl;
	int i, j;

	gpio = (unsigned long *)&GPIO_PDDR_REG(PTA_BASE_PTR);
	*gpio = 0xFFFFFFFF;
	for (i = 4; i < 32; i++) {
		if ((i == 18))
			continue;
		port = (unsigned long *)&PORT_PCR_REG(PORTA_BASE_PTR, i);
		*port = 0x100;
	}
	gpio = (unsigned long *)&GPIO_PDDR_REG(PTB_BASE_PTR);
	*gpio = 0xFFFFFFFF;
	for (i = 0; i < 32; i++) {
		port = (unsigned long *)&PORT_PCR_REG(PORTB_BASE_PTR, i);
		*port++ = 0x100;
	}
	gpio = (unsigned long *)&GPIO_PDDR_REG(PTC_BASE_PTR);
	*gpio = 0xFFFFFFFF;
	for (i = 0; i < 32; i++) {
		port = (unsigned long *)&PORT_PCR_REG(PORTC_BASE_PTR, i);
		*port++ = 0x100;
	}
	gpio = (unsigned long *)&GPIO_PDDR_REG(PTD_BASE_PTR);
	*gpio = 0xFFFFFFFF;
	for (i = 0; i < 32; i++) {
		port = (unsigned long *)&PORT_PCR_REG(PORTD_BASE_PTR, i);
		*port++ = 0x100;
	}
	gpio = (unsigned long *)&GPIO_PDDR_REG(PTE_BASE_PTR);
	*gpio = 0xFFFFFFFF;
	for (i = 0; i < 32; i++) {
		port = (unsigned long *)&PORT_PCR_REG(PORTE_BASE_PTR, i);
		*port++ = 0x100;
	}

	tgl = (unsigned long *)&GPIO_PTOR_REG(PTA_BASE_PTR);
	for (i = 0; i < 010; i++) {
		for (j = 0x10; j < 0x80; j++) {
			if (((j / 4) == 18) || ((j / 4) == 19))
				continue;
			*tgl = (1 << (j / 4));
		}
	}

	tgl = (unsigned long *)&GPIO_PTOR_REG(PTB_BASE_PTR);
	for (i = 0; i < 010; i++) {
		for (j = 0; j < 0x80; j++) {
			*tgl = (1 << (j / 4));
		}
	}

	tgl = (unsigned long *)&GPIO_PTOR_REG(PTC_BASE_PTR);
	for (i = 0; i < 010; i++) {
		for (j = 0; j < 0x80; j++) {
			*tgl = (1 << (j / 4));
		}
	}

	tgl = (unsigned long *)&GPIO_PTOR_REG(PTD_BASE_PTR);
	for (i = 0; i < 010; i++) {
		for (j = 0; j < 0x80; j++) {
			*tgl = (1 << (j / 4));
		}
	}

	tgl = (unsigned long *)&GPIO_PTOR_REG(PTE_BASE_PTR);
	for (i = 0; i < 010; i++) {
		for (j = 0; j < 0x80; j++) {
			*tgl = (1 << (j / 4));
		}
	}
}

void main(void)
{
	printf("%s GPIO Example!\n", TWR_STRING);

	/* Turn on all port clocks */
	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK |
				SIM_SCGC5_PORTB_MASK |
				SIM_SCGC5_PORTC_MASK |
				SIM_SCGC5_PORTD_MASK |
				SIM_SCGC5_PORTE_MASK;

	/* Enable GPIOC interrupts in NVIC */
	enable_irq(INT_PORTC - 16);		/* GPIOC Vector is 77. IRQ# is 77-16=61 */
	enable_irq(INT_PORTA - 16);		/* GPIOC Vector is 75. IRQ# is 75-16=59 */

	/* Initialize GPIO */
	init_gpio();

	lptmr_init();

	while (1) {
		/* Toggle the green LED on PTE6 */
		GPIOE_PTOR |= GPIO_PDOR_PDO(GPIO_PIN(6));

		/* Look at status of SW1 on PTC6 */
		if ((GPIOC_PDIR & GPIO_PDIR_PDI(GPIO_PIN(6))) == 0) {
			/*
			 * if pressed...
			 * Set PTE8 to 0 (turns on orange LED)
			 */
			GPIOE_PDOR &= ~GPIO_PIN(8);
		} else {
			/*
			 * else if SW1 not pressed...
			 * Set PTE8 to 1 (turns off orange LED)
			 */
			GPIOE_PDOR |= GPIO_PIN(8);
		}

		/* Look at status of SW3 on PTA4 */
		if ((GPIOA_PDIR & GPIO_PDIR_PDI(GPIO_PIN(4))) == 0) {
			/*
			 * If pressed...
			 * Set PTE7 to 0 (turns on Yellow LED)
			 */
			GPIOE_PDOR &= ~GPIO_PIN(7);
		} else {
			/*
			 * else if SW2 not pressed...
			 * Set PTE7 to 1 ((turns off Yellow LED)
			 */
			GPIOE_PDOR |= GPIO_PIN(7);
		}

		time_delay_ms(500);
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
void init_gpio()
{
	/*
	 * Set PTC6 and PTC7 (connected to SW1 and SW2) for GPIO functionality,
	 * falling IRQ, and to use internal pull-ups. (pin defaults to input state)
	 */
	PORTC_PCR6 =
	    PORT_PCR_MUX(1) | PORT_PCR_IRQC(0xA) | PORT_PCR_PE_MASK |
	    PORT_PCR_PS_MASK;
	PORTA_PCR4 =
	    PORT_PCR_MUX(1) | PORT_PCR_IRQC(0xA) | PORT_PCR_PE_MASK |
	    PORT_PCR_PS_MASK;

	/*
	 * Set PTE6, PTE7, and PTE8 (connected to LED's)
	 * for GPIO functionality
	 */
	PORTE_PCR6 = (0 | PORT_PCR_MUX(1));
	PORTE_PCR7 = (0 | PORT_PCR_MUX(1));
	PORTE_PCR8 = (0 | PORT_PCR_MUX(1));

	/* Change PTE6, PTE7, PTE8 to outputs */
	GPIOE_PDDR = GPIO_PDDR_PDD(GPIO_PIN(6) | GPIO_PIN(7) | GPIO_PIN(8));
}

/* ISR for PORTC interrupts */
void portc_isr(void)
{
	if (PORTC_ISFR & GPIO_PIN(6)) {
		PORTC_ISFR = 0x40;	/* Clear Port A ISR flags */
		printf("SW1 ");
	}
	if (PORTA_ISFR & GPIO_PIN(4)) {
		PORTA_ISFR = 0x10;	/* Clear Port A ISR flags */
		printf("SW3 ");
	}
	printf("Pressed\n");
}
