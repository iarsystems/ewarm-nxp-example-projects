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
#include "i2c.h"

signed char result[20];
unsigned char MasterTransmission;
unsigned char SlaveID;

/*
 * I2C Initialization
 * Set Baud Rate and turn on I2C0
 */
void init_I2C(void)
{
	SIM_SCGC5 =
	    SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK |
	    SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;	//Turn on clock to I2C` module

	/* configure GPIO for I2C0 function */
	PORTC_PCR10 = PORT_PCR_MUX(2);
	PORTC_PCR11 = PORT_PCR_MUX(2);

	I2C1_F = 0x14;					/* set MULT and ICR */
	I2C1_C1 = I2C_C1_IICEN_MASK;	/* enable IIC */
}

/*
 * Start I2C Transmision
 * @param SlaveID is the 7 bit Slave Address
 * @param Mode sets Read or Write Mode
 */
void IIC_StartTransmission(unsigned char SlaveID, unsigned char Mode)
{
	if (Mode == MWSR) {
		/* set transmission mode */
		MasterTransmission = MWSR;
	} else {
		/* set transmission mode */
		MasterTransmission = MRSW;
	}

	/* shift ID in right possition */
	SlaveID = (unsigned char)ACCEL_I2C_ADDRESS << 1;

	/* Set R/W bit at end of Slave Address */
	SlaveID |= (unsigned char)MasterTransmission;

	/* send start signal */
	i2c_Start();

	/* send ID with W/R bit */
	i2c_write_byte(SlaveID);
}

/* Pause Routine */
void Pause(void)
{
	int n;
	for (n = 1; n < 50; n++) {
		asm("nop");
	}
}

/*
 * Read a register from the MMA7660
 * @param u8RegisterAddress is Register Address
 * @return Data stored in Register
 */
unsigned char I2CReadRegister(unsigned char u8RegisterAddress)
{
	unsigned char result;

	/* Send Slave Address */
	IIC_StartTransmission(SlaveID, MWSR);
	i2c_Wait();

	/* Write Register Address */
	I2C1_D = u8RegisterAddress;
	i2c_Wait();

	/* Do a repeated start */
	I2C1_C1 |= I2C_C1_RSTA_MASK;

	/* Send Slave Address */
	I2C1_D = (ACCEL_I2C_ADDRESS << 1) | 0x01;	//read address
	i2c_Wait();

	/* Put in Rx Mode */
	I2C1_C1 &= (~I2C_C1_TX_MASK);

	/* Turn off ACK since this is second to last byte being read */
	I2C1_C1 |= I2C_C1_TXAK_MASK;

	/* Dummy read */
	result = I2C1_D;
	i2c_Wait();

	/* Send stop since about to read last byte */
	i2c_Stop();

	/* Read byte */
	result = I2C1_D;

	return result;
}

/*
 * Write a byte of Data to specified register on MMA7660
 * @param u8RegisterAddress is Register Address
 * @param u8Data is Data to write
 */
void I2CWriteRegister(unsigned char u8RegisterAddress, unsigned char u8Data)
{
	/* send data to slave */
	IIC_StartTransmission(SlaveID, MWSR);
	i2c_Wait();

	I2C1_D = u8RegisterAddress;
	i2c_Wait();

	I2C1_D = u8Data;
	i2c_Wait();

	i2c_Stop();

	Pause();
}

/*
 * Read first three registers from the MMA7660
 * @param u8RegisterAddress is Register Address
 * @return Data stored in Register
 */
unsigned char I2CReadMultiRegisters(unsigned char u8RegisterAddress,
									unsigned char bytes)
{
	unsigned char n = bytes;
	int i;

	/* Send Slave Address */
	IIC_StartTransmission(SlaveID, MWSR);
	i2c_Wait();

	/* Write Register Address */
	I2C1_D = u8RegisterAddress;
	i2c_Wait();

	/* Do a repeated start */
	I2C1_C1 |= I2C_C1_RSTA_MASK;

	/* Send Slave Address */
	I2C1_D = (ACCEL_I2C_ADDRESS << 1) | 0x01;	//read address
	i2c_Wait();

	/* Put in Rx Mode */
	I2C1_C1 &= (~I2C_C1_TX_MASK);

	/* Ensure TXAK bit is 0 */
	I2C1_C1 &= ~I2C_C1_TXAK_MASK;

	/* Dummy read */
	result[0] = I2C1_D;
	i2c_Wait();

	for (i = 0; i < n - 2; i++) {
		/* Read first byte */
		result[i] = I2C1_D;
		i2c_Wait();
	}
	/* Turn off ACK since this is second to last read */
	I2C1_C1 |= I2C_C1_TXAK_MASK;

	/* Read second byte */
	result[i++] = I2C1_D;
	i2c_Wait();

	/* Send stop */
	i2c_Stop();

	/* Read third byte */
	result[i++] = I2C1_D;

	return result[0];
}
