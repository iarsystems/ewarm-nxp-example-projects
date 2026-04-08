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
#include "hal_i2c.h"

#define MMA8451_I2C_ADDRESS (0x1c<<1)
#define I2C1_B I2C1_BASE_PTR

/* this delay is very important, it may cause w-r operation failure. */
static void pause(void)
{
	int n;
	for (n = 0; n < 40; n++)
		asm("nop");
}

uint8 hal_dev_mma8451_read_reg(uint8 addr)
{
	uint8 result;

	i2c_start(I2C1_B);
	i2c_write_byte(I2C1_B, MMA8451_I2C_ADDRESS | I2C_WRITE);

	i2c_wait(I2C1_B);
	i2c_get_ack(I2C1_B);

	i2c_write_byte(I2C1_B, addr);
	i2c_wait(I2C1_B);
	i2c_get_ack(I2C1_B);

	i2c_repeated_start(I2C1_B);
	i2c_write_byte(I2C1_B, MMA8451_I2C_ADDRESS | I2C_READ);
	i2c_wait(I2C1_B);
	i2c_get_ack(I2C1_B);

	i2c_set_rx_mode(I2C1_B);

	i2c_give_nack(I2C1_B);
	result = i2c_read_byte(I2C1_B);
	i2c_wait(I2C1_B);

	i2c_stop(I2C1_B);
	result = i2c_read_byte(I2C1_B);
	pause();
	return result;
}

void hal_dev_mma8451_write_reg(uint8 addr, uint8 data)
{
	i2c_start(I2C1_B);

	i2c_write_byte(I2C1_B, MMA8451_I2C_ADDRESS | I2C_WRITE);
	i2c_wait(I2C1_B);
	i2c_get_ack(I2C1_B);

	i2c_write_byte(I2C1_B, addr);
	i2c_wait(I2C1_B);
	i2c_get_ack(I2C1_B);

	i2c_write_byte(I2C1_B, data);
	i2c_wait(I2C1_B);
	i2c_get_ack(I2C1_B);

	i2c_stop(I2C1_B);
	pause();
}
