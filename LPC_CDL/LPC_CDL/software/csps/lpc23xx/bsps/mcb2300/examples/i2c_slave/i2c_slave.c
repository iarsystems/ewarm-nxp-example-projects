/***********************************************************************
 * i2c_slave.c 2008-10-29 --- HieuNguyen
 *
 * Project: I2C driver example
 *
 * Description:
 * 		This code is set I2C in SLAVE mode
 * 		This example is used to test I2C in master and slave mode together.
 * 		The master will first write 64 bytes of data to slave (in polling mode) 
 * and read 64 bytes of data from slave (in asynchronous mode).
 * 		The slave will first read 64 bytes of data from master (in polling mode) 
 * and transfer 64 bytes of data to a master (in polling mode).
 ***********************************************************************
 * Software that is described herein is for illustrative purposes only  
 * which provides customers with programming information regarding the  
 * products. This software is supplied "AS IS" without any warranties.  
 * NXP Semiconductors assumes no responsibility or liability for the 
 * use of the software, conveys no license or title under any patent, 
 * copyright, or mask work right to the product. NXP Semiconductors 
 * reserves the right to make changes in the software without 
 * notification. NXP Semiconductors also make no representation or 
 * warranty that such application will be suitable for the specified 
 * use without further testing or modification. 
 **********************************************************************/
#include "lpc23xx_i2c_driver.h"
#include "lpc23xx_pinsel_driver.h"
#include "mcb2300_init.h"
#include "lpc23xx_vic_driver.h"
#include "lpc23xx_uart_driver.h"

// Own I2C Slave address ORed with Genaral Call enable bit
#define OWNSLVADDR	(0x64 | 0x01)

CHAR menu1[] = 
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
"I2C demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Use I2C function in SLAVE mode \n\r"
" To communicate with the other that uses I2C function as MASTER mode \n\r"
" Read and write 64 bytes from/to I2C master \n\r"
"********************************************************************************\n\r";
STATIC S32 i2cdev, uartdev;
U8 wdata[64], rdata[64];
S32 volatile I2CFlag = 0;

// Init data before sending
void DataInit (void)
{
	U8 i;
	
	UART_Puts_ (uartdev, "Initilize data...");
	UART_Puts_ (uartdev, "i\tWriteData[i]\tReadData[i]");
	
	for (i = 0; i < 64; i++)
	{
		wdata[i] = i;
		rdata[i] = 0;
		
		/* Print information */
		UART_PutDec (uartdev, i);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, (U32) wdata[i]);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, (U32) rdata[i]);
    	UART_Puts_ (uartdev, "");
	}
}

/* Init UART0 */
S32 UART0_Init (void)
{
	// Set pins for UART0 function
    PINSEL_Set(PORT_0, 2, FUNC_1);
    PINSEL_Set(PORT_0, 3, FUNC_1);
    
    /* Open UART0 - 115200 bps, 8 databit, 0 parity, 1 stop bit */    
    if ((uartdev = UART_Init (UART_PORT_0, 115200, 8, 0, 1)) == 0x00)
    {
        return _ERROR;
    }
    
    return _NO_ERROR;
}

/* Read pressed key */
CHAR UART0Read(void)
{
	U32 len = 0;
	CHAR readkey[1];
	
     while (len == 0)
     {
         len = UART_ReadPoll (uartdev, readkey, sizeof(readkey));
     }
     
     return readkey[0];
}

/* User call back function */
void usr_cbs (void)
{
	I2CFlag = 1;
}

int c_entry (void)
{		
	U8 i;
	U32 monitor;
	
	mcb2300_init();
	
	/* Init UART0 */
	if (UART0_Init() == _ERROR)
	{
		return 0;
	}
	
	// Set pin function according to I2C1 controller
	PINSEL_Set (PORT_0, 19, FUNC_3);
	PINSEL_Set (PORT_0, 20, FUNC_3);
	
	/* Set internal pull-up resistor since these pin 
	 * are not open-drain in I2C function */
	PINSEL_SetResistorMode (PORT_0, 19, PINMODE_PULLUP);
	PINSEL_SetResistorMode (PORT_0, 20, PINMODE_PULLUP);
	
	// Init I2C1 device, slave mode, 100KHz - not necessary in slave mode
	if ((i2cdev = I2C_Init (I2C_PORT_1, I2C_SLAVE_MODE, OWNSLVADDR, 100000)) == 0x00)
	{
		return 0;
	}
	
	// Install IRQ for I2C device used in ASYNC mode
	if (VIC_InstallIRQ (I2C1_INT,(void *) I2C1_ISR, 0x01) == _ERROR)
	{
		return 0;
	}
	    
	// Config Call back function used in ASYNC mode 
	I2C_SetCBS (i2cdev, (void *)usr_cbs);
	
	/* Welcome screen */
	UART_WritePoll (uartdev, menu1, sizeof(menu1));
	UART_Puts_ (uartdev, "Press 1 to start!");

	// Wait key "1" pressed
	while (UART0Read() != '1');
	
	// Init data
	DataInit();
	
	// print infor..
	UART_Puts_ (uartdev, "Start reading when receiving WRITE command from MASTER...");
	
	// Read some data after receive "write" commmand from master
	// Read 64 bytes
	monitor = I2C_ReadPoll (i2cdev, 0x00, rdata, sizeof (rdata));

	// Print information
	UART_Puts_ (uartdev, "Reading complete!");
	UART_Puts_ (uartdev, "Start sending when receiving READ command from MASTER...");
	
	// Write 64 bytes
	monitor = I2C_WritePoll (i2cdev, 0x00, wdata, sizeof (wdata));

	// Print data content after transferring...
	UART_Puts_ (uartdev, "Read and write data content after transferring:");
	UART_Puts_ (uartdev, "i\tWriteData[i]\tReadData[i]");
	for (i = 0; i < 64; i++)
	{		
		/* Print information */
		UART_PutDec (uartdev, i);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, (U32) wdata[i]);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, (U32) rdata[i]);
    	UART_Puts_ (uartdev, "");
	}
	
	// Terminate program..
	UART_Puts_ (uartdev, "Demo program terminated!");
	
	/* Close UART */
	UART_Close (uartdev);
	
	/* Close I2C */
	I2C_Close (i2cdev);
	
	// Loop forever...
	while (1);
	
	return (0);
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}
