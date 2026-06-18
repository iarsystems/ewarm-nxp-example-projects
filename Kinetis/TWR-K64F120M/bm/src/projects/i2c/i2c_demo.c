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
 * File:		i2c_demo.c
 * Purpose:		I2C example of talking to MMA8451Q on the Tower board.
 *
 *				This demo uses a single byte read, but there is also a
 *				u8ReadThreeRegisters() function available to show
 *				an example of doing multiple reads back to back.
 */

#include "common.h"
#include "lptmr.h"
#include "i2c.h"

extern void lptmr_init(void);
extern void time_delay_ms(uint32 count_val);

extern signed char result[20];
unsigned int counter = 0;
unsigned short ix = 0;

void main(void)
{
	printf("Kinetis %s I2C Demo\n", TWR_STRING);

	lptmr_init();

	/* Initialize I2C */
	init_I2C();

	/* Configure accelemoter sensor */
	I2CWriteRegister(0x2A, 0x01);

	printf("   X     Y     Z\n");

	while (1) {
		/* Look at status of accelerometer */
		I2CReadMultiRegisters(0x01, 6);

		printf("  %3d  %3d  %3d", result[0], result[2], result[4]);

		/* Delay for 250ms */
		time_delay_ms(100);
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
	}
}
