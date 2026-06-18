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

unsigned char MasterTransmission;
unsigned char SlaveID;

/*
 * I2C Initialization
 * Set Baud Rate and turn on I2C
 */
void init_I2C(void)
{
	SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;

	/* configure GPIO for I2C function */
	PORTC_PCR10 = PORT_PCR_MUX(2);
	PORTC_PCR11 = PORT_PCR_MUX(2);

	I2C1_F = 0x14;		/* set MULT and ICR */
	I2C1_C1 = 0x80;		/* enable IIC */
}

void deinit_I2C(void)
{
	I2C1_C1 = 0x00;
	SIM_SCGC4 &= ~SIM_SCGC4_I2C1_MASK;
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
	SlaveID = (unsigned char)MMA7660_I2C_ADDRESS << 1;

	/* Set R/W bit at end of Slave Address */
	SlaveID |= (unsigned char)MasterTransmission;

	/* send start signal */
	i2c_Start();

	/* send ID with W/R bit */
	i2c_write_byte(SlaveID);
}

/*
 * Read a register from the MMA7660
 * @param u8RegisterAddress is Register Address
 * @return Data stored in Register
 */
unsigned char u8MMA7660ReadRegister(unsigned char u8RegisterAddress)
{
	unsigned char result;
	unsigned int j;

	/* Send Slave Address */
	IIC_StartTransmission(SlaveID, MWSR);
	i2c_Wait();

	/* Write Register Address */
	I2C1_D = u8RegisterAddress;
	i2c_Wait();

	/* Do a repeated start */
	I2C1_C1 |= I2C_C1_RSTA_MASK;

	/* Send Slave Address */
	I2C1_D = (MMA7660_I2C_ADDRESS << 1) | 0x01;	/*read address */
	i2c_Wait();

	/* Put in Rx Mode */
	//I2C0_C1 &= (~I2C_C1_TX_MASK);
	I2C1_C1 &= (~I2C_C1_TX_MASK);

	/* Turn off ACK */
	I2C1_C1 |= I2C_C1_TXAK_MASK;

	/* Dummy read */
	result = I2C1_D;
	for (j = 0; j < 5000; j++) {
	};
	i2c_Wait();

	/* Send stop */
	i2c_Stop();
	result = I2C1_D;
	Pause();
	return result;
}

/*
 * Write a byte of Data to the MMA7660
 * @param u8RegisterAddress is Register Address
 * @param u8Data is Data to write
 */
void MMA7660WriteRegister(unsigned char u8RegisterAddress, unsigned char u8Data)
{
	/* send data to slave */
	IIC_StartTransmission(SlaveID, MWSR);
	i2c_Wait();

	/* Send I2C address */
	I2C1_D = u8RegisterAddress;
	i2c_Wait();

	/* Send data */
	I2C1_D = u8Data;
	i2c_Wait();

	i2c_Stop();

	Pause();
}
