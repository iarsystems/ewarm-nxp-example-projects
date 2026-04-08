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
 * File:	k64_i2c_demo.c
 * Purpose:	Demo Code for TWR-K64F120M
 */

#include "common.h"
#include "hal_i2c.h"
#include "hal_dev_mma8451.h"

/* Defines for GPIO */
#define GPIO_PIN_MASK			0x1Fu
#define GPIO_PIN(x)				(((1)<<(x & GPIO_PIN_MASK)))

extern uint32_t irq_sw;
extern void msec_delay(uint32_t ms);
extern void usec_delay(uint32_t ms);

void deinit_i2c_demo(void)
{
	hal_i2c_deinit();
	GPIOE_PDOR |= ((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
}

void i2c_demo(int a, char **b)
{
	unsigned char tmp;
	signed short resultx, resulty, resultz;

	printf("Kinetis %s I2C Demo!\n"
		   "- Rotate board either potentiometer or Battery holder side up\n"
		   "- In each case, red LED D8 will be lit\n"
		   "- Press SW3 to exit...\n",
		   TWR_STRING);

	/* clear all LEDs */
	GPIOE_PDOR |= ((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
	/* Set to 0 (turns on blue LED) */
	GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));

	/* Initialize I2C modules */
	hal_i2c_init();

	/* Configure MMA8451 sensor */
	tmp = hal_dev_mma8451_read_reg(0x2a);
	hal_dev_mma8451_write_reg(0x2a, tmp | 0x01);

	while (1) {
		if ((hal_dev_mma8451_read_reg(0x00) & 0xf) != 0) {
			resultx = hal_dev_mma8451_read_reg(0x01) << 8;
			resultx |= hal_dev_mma8451_read_reg(0x02);
			resultx >>= 2;

			resulty = hal_dev_mma8451_read_reg(0x03) << 8;
			resulty |= hal_dev_mma8451_read_reg(0x04);
			resulty >>= 2;

			resultz = hal_dev_mma8451_read_reg(0x05) << 8;
			resultz |= hal_dev_mma8451_read_reg(0x06);
			resultz >>= 2;
		} else {
			continue;
		}

		/* Read x-axis register */
		printf("%5d   %5d   %5d\r", resultx, resulty, resultz);

		/* If tilted, turn on orange LED */
		if ((resultx > 200 && resultx < 4000) ||
		    (resultx < -200 && resultx > -4000)) {
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(8));	/* Set to 0 (turns on red LED) */
		} else {
			/* If not tilted, turn off orange LED */
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(8));	/* Set to 1 (turns off red LED) */
		}

		/* Toggle the green LED on PTC9 */
		GPIOE_PTOR |= GPIO_PDOR_PDO(GPIO_PIN(6));
		if (irq_sw == 0x01) {
			irq_sw &= ~0x01;		/* clear for next use */
			/* if SW3 (PTA4) pressed..., exit this demo */
			break;
		}

		usec_delay(50);
	}

	deinit_i2c_demo();
	printf("Exiting I2C acceleromter demo\n\n");
}

void i2c_oobe_demo(int a, char **b)
{
	unsigned char tmp;
	signed short resultx, resulty, resultz;

	printf("Kinetis %s OOBE I2C Demo!\n"
		   "- Tilt board so that primary or secondary side faces up\n"
		   "- In each case, LEDs will light up in waterfall fashion\n"
		   "- press SW3 to exit\n",
		   TWR_STRING);

	/* Initialize I2C modules */
	hal_i2c_init();

	/* Configure MMA8451 sensor */
	tmp = hal_dev_mma8451_read_reg(0x2a);
	hal_dev_mma8451_write_reg(0x2a, tmp | 0x01);

	while (1) {

		if ((hal_dev_mma8451_read_reg(0x00) & 0xf) != 0) {
			resultx = hal_dev_mma8451_read_reg(0x01) << 8;
			resultx |= hal_dev_mma8451_read_reg(0x02);
			resultx >>= 2;

			resulty = hal_dev_mma8451_read_reg(0x03) << 8;
			resulty |= hal_dev_mma8451_read_reg(0x04);
			resulty >>= 2;

			resultz = hal_dev_mma8451_read_reg(0x05) << 8;
			resultz |= hal_dev_mma8451_read_reg(0x06);
			resultz >>= 2;
		} else
			continue;

		/* Read x-axis register */
		printf("%5d   %5d   %5d\r", resultx, resulty, resultz);

		/* If tilted, turn on orange/red LED and turn on all LCD segments */
		if ((resulty > -500 && resulty < 200) &&
		    (resultz < 4200 && resultz > 4000)) {
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(6));	/* Set to 0 (turns on green LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(7));	/* Set to 0 (turns on yellow LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(8));	/* Set to 0 (turns on red LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));	/* Set to 0 (turns on blue LED) */
		} else if ((resulty > -1500 && resulty < -500) &&
				   (resultz < 4000 && resultz > 3800)) {
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(6));	/* Set to 0 (turns off green LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(7));	/* Set to 0 (turns on yellow LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(8));	/* Set to 0 (turns on red LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));	/* Set to 0 (turns on blue LED) */
		} else if ((resulty > -2500 && resulty < -1500) &&
				   (resultz < 3800 && resultz > 3400)) {
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(6));	/* Set to 0 (turns off green LED) */
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(7));	/* Set to 0 (turns off yellow LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(8));	/* Set to 0 (turns on red LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));	/* Set to 0 (turns on blue LED) */
		} else if ((resulty > -4100 && resulty < -2500) &&
				   (resultz < 3400 && resultz > -200)) {
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(6));	/* Set to 0 (turns off green LED) */
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(7));	/* Set to 0 (turns off yellow LED) */
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(8));	/* Set to 0 (turns off red LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));	/* Set to 0 (turns on blue LED) */
		} else if ((resulty > 500 && resulty < 1500) &&
				   (resultz < 4000 && resultz > 3800)) {
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(6));	/* Set to 0 (turns on green LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(7));	/* Set to 0 (turns on yellow LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(8));	/* Set to 0 (turns on red LED) */
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(9));	/* Set to 0 (turns off blue LED) */
		} else if ((resulty > 1500 && resulty < 2500) &&
				   (resultz < 3800 && resultz > 3400)) {
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(6));	/* Set to 0 (turns on green LED) */
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(7));	/* Set to 0 (turns on yellow LED) */
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(8));	/* Set to 0 (turns off red LED) */
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(9));	/* Set to 0 (turns off blue LED) */
		} else if ((resulty > 2500 && resulty < 4100) &&
				   (resultz < 3400 && resultz > -200)) {
			GPIOE_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(6));	/* Set to 0 (turns on green LED) */
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(7));	/* Set to 0 (turns off yellow LED) */
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(8));	/* Set to 0 (turns off red LED) */
			GPIOE_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(9));	/* Set to 0 (turns off blue LED) */
		}

		if (irq_sw == 0x01) {
			irq_sw &= ~0x01;		/* clear for next use */
			/* if SW3 (PTA4) pressed..., exit this demo */
			break;
		}

		usec_delay(50);
	}

	deinit_i2c_demo();
	printf("Exiting I2C acceleromter demo\n\n");
}
