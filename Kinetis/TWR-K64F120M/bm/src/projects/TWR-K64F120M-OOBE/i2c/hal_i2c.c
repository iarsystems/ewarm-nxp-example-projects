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

void i2c_set_tx_mode(I2C_MemMapPtr p)
{
	p->C1 |= I2C_C1_TX_MASK;
}

void i2c_set_rx_mode(I2C_MemMapPtr p)
{
	p->C1 &= ~I2C_C1_TX_MASK;
}

void i2c_set_slave_mode(I2C_MemMapPtr p)
{
	p->C1 &= ~I2C_C1_MST_MASK;
}

void i2c_set_master_mode(I2C_MemMapPtr p)
{
	p->C1 |= I2C_C1_MST_MASK;
}

/* i2c general */

void i2c_give_nack(I2C_MemMapPtr p)
{
	p->C1 |= I2C_C1_TXAK_MASK;
}

void i2c_give_ack(I2C_MemMapPtr p)
{
	p->C1 &= ~I2C_C1_TXAK_MASK;
}

void i2c_repeated_start(I2C_MemMapPtr p)
{
	p->C1 |= 0x04;
}

void i2c_write_byte(I2C_MemMapPtr p, uint8 data)
{
	p->D = data;
}

uint8 i2c_read_byte(I2C_MemMapPtr p)
{
	return p->D;
}

void i2c_start(I2C_MemMapPtr p)
{
	i2c_set_master_mode(p);
	i2c_set_tx_mode(p);
}

void i2c_stop(I2C_MemMapPtr p)
{
	i2c_set_slave_mode(p);
	i2c_set_rx_mode(p);
}

void i2c_wait(I2C_MemMapPtr p)
{
	/* wait flag */
	while ((p->S & I2C_S_IICIF_MASK) == 0) ;
	/* clear flag */
	p->S |= I2C_S_IICIF_MASK;
}

uint16 i2c_get_ack(I2C_MemMapPtr p)
{
	if ((p->S & I2C_S_RXAK_MASK) == 0)
		return TRUE;
	else
		return FALSE;
}

void hal_i2c_init(void)
{
	SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;

	/* configure GPIO for I2C function */
	PORTC_PCR10 = PORT_PCR_MUX(2);
	PORTC_PCR11 = PORT_PCR_MUX(2);

	I2C1_F = 0x14;		/* baudrate */
	I2C1_C1 = 0x80;		/* enable IIC */
}

void hal_i2c_deinit(void)
{
	I2C1_C1 = 0x00;
	SIM_SCGC4 &= ~SIM_SCGC4_I2C1_MASK;
}
