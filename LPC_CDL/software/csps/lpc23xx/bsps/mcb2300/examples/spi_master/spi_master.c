/***********************************************************************
 * $Id: spi_master.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx driver
 *
 * Description:	
 * 		This example will set SPI in MASTER mode
 * 		This example is used to test SPI in master and slave mode together.
 * 		The master will transfer 100 bytes of data with slave (in polling mode) 
 * and display the result of write/read data.
 * 		The slave will transfer 100 bytes of data with master (in asynchronous mode) 
 * and display the result of write/read data.
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
#include "lpc23xx_spi_driver.h"
#include "lpc23xx_pinsel_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_vic_driver.h"
#include "mcb2300_init.h"
#include "lpc23xx_uart_driver.h"

CHAR menu1[] = 
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
"SPI demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Use SPI function in MASTER mode \n\r"
" To communicate with the other that uses SPI function as SLAVE mode \n\r"
" Transfer 100 bytes of data with slave \n\r"
"********************************************************************************\n\r";
STATIC S32 spidev, uartdev;
U8 wdata[100], rdata[100];
S32 volatile SPIFlag = FALSE;

void DataInit (void)
{
	U8 i;
	
	UART_Puts_ (uartdev, "Initilize data...");
	UART_Puts_ (uartdev, "i\tWriteData[i]\tReadData[i]");
	
	for (i = 0; i < 100; i++)
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

S32 UART0_Init (void)
{
    /* Open UART0 - 115200 bps, 8 databit, 0 parity, 1 stop bit */    
    if ((uartdev = UART_Init (UART_PORT_0, 115200, 8, 0, 1)) == 0x00)
    {
        return _ERROR;
    }
    return _NO_ERROR;
}

/* Read pressed key */
U8 UART0Read(void)
{
	U8 len = 0;
	U8 readkey[1];
	
     while (len == 0)
     {
         len = UART_ReadPoll (uartdev, readkey, sizeof(readkey));
     }
     
     return readkey[0];
}


void usr_cbs (void)
{
	U32 tmp;
	// Check status of latest SPI operation
	tmp = SPI_GetStatus (spidev);
	
	// if error occur, display information about the error via UART
	if ((tmp & 0x01) != 0)
	{
		UART_Puts_ (uartdev, "SPI operation error! Current data will be abort!");
		UART_Puts (uartdev, "Return error code: "); UART_PutHex (uartdev, (tmp & 0x01));
		UART_Puts_ (uartdev, "");
	}
	
	// Turn on SPI complete flag
	SPIFlag = TRUE;
}


int c_entry (void)
{
	U8 tmp;
	
	mcb2300_init();
	
	if ((UART0_Init()) == _ERROR)
	{
		return 0;
	}
	
	// Install SPI interrupt handler used in ASYNC mode
	if (VIC_InstallIRQ (SPI0_INT, (void *) SPI_ISR, 0x01) == _ERROR)
	{
		return 0;
	}
	
	/* Set these function pins for SPI peripheral except of SS pin
	 * that will be configured when set SPI mode (master or slave) */
	PINSEL_Set (PORT_0, SPI_PIN_FUNC_SCK, FUNC_3);
	PINSEL_Set (PORT_0, SPI_PIN_FUNC_MISO, FUNC_3);
	PINSEL_Set (PORT_0, SPI_PIN_FUNC_MOSI, FUNC_3);
	
	if ((spidev = SPI_Init (0, SPI_MASTER, 100000, SPI_DATABIT_8, SPI_FIRST_CLK_EDGE,
			SPI_CLK_HI, SPI_ORDER_MSB)) == 0x00)
	{
		return _ERROR;
	}
	
	// Start to run in asyn mode, callback function mus be set first
	if (SPI_SetCBS (spidev, (void *) usr_cbs) == _ERROR)
	{
		return 0;
	}
	
	// Print welcome screen
	UART_WritePoll (uartdev, menu1, sizeof(menu1));
	UART_Puts_ (uartdev, "Press 1 to start!");
	while (UART0Read() != '1');
	
	// init data
	DataInit();
	
	// Transfer data...
	UART_Puts_ (uartdev, "Transferring data...");
	SPI_ReadWritePoll (spidev, &rdata, &wdata, sizeof(wdata));
	
	
	// Print data after transfer...
	UART_Puts_ (uartdev, "Transfer completed! Data received: ");
	UART_Puts_ (uartdev, "i\tWriteData[i]\tReadData[i]");
	for (tmp = 0; tmp < 100; tmp ++)
	{
		/* Print information */
		UART_PutDec (uartdev, tmp);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, (U32) wdata[tmp]);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, (U32) rdata[tmp]);
    	UART_Puts_ (uartdev, "");
	}
	
	UART_Puts_ (uartdev, "Demo terminated!");
	
	// Loop forever...
	while (1);

	return 1;
}

int main (void)
{
    return (c_entry());
}
