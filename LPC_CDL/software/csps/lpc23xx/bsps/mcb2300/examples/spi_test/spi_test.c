/***********************************************************************
 * $Id: spi_test.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx driver
 *
 * Description:
 * Using SPI at mode SPI master/8bit on MCB2300 to communicate with
 * SC16IS750/760 Demo Board
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

// Define macro used in command when using SPI with SC16IS740
#define SC16IS740_WR_CMD(x)		((U8) (x << 3))
#define SC16IS740_RD_CMD(x)		((U8) ((x << 3) | 0x80))

// Define register address of SC16IS740
#define SC16IS740_IODIR_REG		0x0A
#define SC16IS740_IOSTATE_REG	0x0B
#define SC16IS740_IOCON_REG		0x0E

CHAR menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"SPI demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Communicate with SPI function on SC16IS750/760 Demo Board\n\r"
" Use IO function on SC16IS740/750/760 chip to turn ON/OFF LEDs\n\r"
"********************************************************************************\n\r";
STATIC S32 spidev, uartdev;
U8 iocon_cfg[2] = {SC16IS740_WR_CMD(SC16IS740_IOCON_REG), 0x00};
U8 iodir_cfg[2] = {SC16IS740_WR_CMD(SC16IS740_IODIR_REG), 0xFF};
U8 iostate_on[2] = {SC16IS740_WR_CMD(SC16IS740_IOSTATE_REG), 0x00};
U8 iostate_off[2] = {SC16IS740_WR_CMD(SC16IS740_IOSTATE_REG), 0xFF};
U8 spireadbuf[2];
S32 volatile SPIFlag = FALSE;

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
	U8 ReadKey;

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

	// Reset SPI complete flag
	SPIFlag = FALSE;
	SPI_ReadWriteAsync (spidev, &spireadbuf, &iocon_cfg, sizeof (iocon_cfg));
	// Wait for SPI complete
	while (SPIFlag == FALSE);

	// Reset SPI complete flag
	SPIFlag = FALSE;
	SPI_ReadWriteAsync (spidev, &spireadbuf, &iodir_cfg, sizeof (iodir_cfg));
	// Wait for SPI complete
	while (SPIFlag == FALSE);

	UART_Puts_ (uartdev, "Press 1 to turn ON, press 2 to turn OFF");

	// Loop forever, run in polling mode
	while (1)
	{
		// Enter value
		ReadKey = UART0Read();

		if (ReadKey == '1')
		{
			SPI_ReadWritePoll (spidev, &spireadbuf, &iostate_on, sizeof (iostate_on));
			UART_Puts_(uartdev, "LEDs are ON");
		}
		else if (ReadKey == '2')
		{
			SPI_ReadWritePoll (spidev, &spireadbuf, &iostate_off, sizeof (iostate_off));
			UART_Puts_(uartdev, "LEDs are OFF");
		}
		else if (ReadKey == 27)
		{
			UART_Puts_ (uartdev, "Demo program terminated!");
			break;
		}
		else
		{
			UART_Puts_ (uartdev, "Error input!");
		}
	}

	return 1;
}

int main (void)
{
    return (c_entry());
}
