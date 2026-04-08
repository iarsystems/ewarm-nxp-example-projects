/***********************************************************************
 * ssp_seft_poll.c 2008-11-10 --- HieuNguyen
 *
 * Project: SSP driver example
 *
 * Description:
 * Use SSP0 function in MASTER mode
 * MOSI and MISO pin on SSP pinout must be connected together
 * Transfer 64 bytes of data (in polling mode) and display result
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
#include "lpc23xx_ssp_driver.h"
#include "lpc23xx_pinsel_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_vic_driver.h"
#include "lpc23xx_uart_driver.h"
#include "mcb2300_init.h"

/* define SPI function on lpc2148
 * the number pin of port 0 */
#define SSP0_SCK_PIN	15
#define SSP0_MISO_PIN	17
#define SSP0_MOSI_PIN	18
#define SSP0_SSEL_PIN	16
#define SSP0_FUNCTION	FUNC_2

CHAR menu0[] =
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
"SSP self-transferring demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Use SSP0 function in MASTER mode \n\r"
" MOSI and MISO pin on SSP pinout must be connected together (self-test)\n\r"
" Transfer 64 bytes of data (in polling mode) and display result\n\r"
"********************************************************************************\n\r";
S32 sspdev = 0, uart0dev = 0;
U8 readdata[64];
U8 writedata[64];

BOOL32 volatile complete = FALSE;

/* Init UART0 */
S32 UART0_Init (void)
{
	// Set pins for UART0 function
    PINSEL_Set(PORT_0, 2, FUNC_1);
    PINSEL_Set(PORT_0, 3, FUNC_1);
    
    /* Open UART0 - 115200 bps, 8 databit, 0 parity, 1 stop bit */    
    if ((uart0dev = UART_Init (UART_PORT_0, 115200, 8, 0, 1)) == 0x00)
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
         len = UART_ReadPoll (uart0dev, readkey, sizeof(readkey));
     }
     
     return readkey[0];
}

void DataInit (void)
{
	U8 i;
	
	UART_Puts_ (uart0dev, "Initilize data...");
	UART_Puts_ (uart0dev, "i\tWriteData[i]\tReadData[i]");
	
	for (i = 0; i < 64; i++)
	{
		writedata[i] = i;
		readdata[i] = 0;
		
		/* Print information */
		UART_PutDec (uart0dev, i);
    	UART_Puts (uart0dev, "\t"); UART_PutHex (uart0dev, (U32) writedata[i]);
    	UART_Puts (uart0dev, "\t"); UART_PutHex (uart0dev, (U32) readdata[i]);
    	UART_Puts_ (uart0dev, "");
	}
}

/* User call back function */
void user_callback (void)
{
	complete = TRUE;
}

int c_entry (void)
{
	U8 i;
	
	mcb2300_init();
	
	// Init UART0
	if ((UART0_Init()) == _ERROR)
	{
		return 0;
	}

	/* Set these pin of port 0 to SSP function */
	PINSEL_Set(PORT_0, SSP0_SCK_PIN, SSP0_FUNCTION);
	PINSEL_Set(PORT_0, SSP0_MISO_PIN, SSP0_FUNCTION);
	PINSEL_Set(PORT_0, SSP0_MOSI_PIN, SSP0_FUNCTION);
	PINSEL_Set(PORT_0, SSP0_SSEL_PIN, SSP0_FUNCTION);

	// Print welcome screen
	UART_WritePoll (uart0dev, menu0, sizeof(menu0));
	
	// Init SSP device
	if ((sspdev = SSP_Init(0, SSP_MASTER, SSP_FRM_MODE_SPI, 100000, 
			8, SSP_FIRST_CLK_EDGE, SSP_CLK_HI)) == 0)
	{
		/* Error */
		return 0;
	}

	UART_Puts_ (uart0dev, "Press '1' to initlized data");
	while (UART0Read() != '1');
	
	/* Refresh data */
	DataInit();
	
	UART_Puts_ (uart0dev, "Press '2' to transfer data");
	while (UART0Read() != '2');
	UART_Puts_ (uart0dev, "Data transferring...");
	
	/* Transfer some data in POLLING mode */
	SSP_ReadWritePoll (sspdev, readdata, writedata, sizeof (writedata));

	UART_Puts_ (uart0dev, "Data transferring completed!");
	
	// Print data after transferring
	UART_Puts_ (uart0dev, "Data after transferring...");
	UART_Puts_ (uart0dev, "i\tWriteData[i]\tReadData[i]");
	for (i = 0; i < 64; i++)
	{
		/* Print information */
		UART_PutDec (uart0dev, i);
    	UART_Puts (uart0dev, "\t"); UART_PutHex (uart0dev, (U32) writedata[i]);
    	UART_Puts (uart0dev, "\t"); UART_PutHex (uart0dev, (U32) readdata[i]);
    	UART_Puts_ (uart0dev, "");
	}

	// Terminate program
	UART_Puts_ (uart0dev, "Demo terminated!");
	
	/* Close SSP device */
	SSP_Close(sspdev);

	/* Loop forever */
	while (1);

	return 1;
}

int main (void)
{
    return (c_entry());
}
