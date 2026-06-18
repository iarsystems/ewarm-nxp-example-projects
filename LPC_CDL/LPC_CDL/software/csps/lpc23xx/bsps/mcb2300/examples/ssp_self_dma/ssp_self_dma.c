/***********************************************************************
 * ssp_seft_dma.c 2008-11-10 --- HieuNguyen
 *
 * Project: SSP driver example
 *
 * Description:
 * Use SSP0 function in MASTER mode
 * MOSI and MISO pin on SSP pinout must be cross-connected together (self-test)
 * Transfer 64 bytes of data (in DMA mode for both Tx and Rx) and display result
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
#include "lpc23xx_dma_driver.h"
#include "mcb2300_init.h"

/* the number pin of port 0 */
#define SSP0_SCK_PIN	15
#define SSP0_MISO_PIN	17
#define SSP0_MOSI_PIN	18
#define SSP0_SSEL_PIN	16
#define SSP0_FUNCTION	FUNC_2

// For DMA controller
#define DMA_SRC			0x7FD00000
#define DMA_DST			0x7FD01000
#define DMA_DATA_SIZE	64
#define SSP0_DMA_FIFO	0xE0068008

CHAR menu0[] =
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
"SSP self-transferring (DMA mode) demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Use SSP0 function in MASTER mode \n\r"
" MOSI and MISO pin on SSP pinout must be cross-connected together (self-test)\n\r"
" Transfer 64 bytes of data (in DMA mode for both Tx and Rx) and display result\n\r"
"********************************************************************************\n\r";
S32 sspdev = 0, uart0dev = 0;
BOOL32 complete;

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
U8 UART0Read(void)
{
	U8 len = 0;
	U8 readkey[1];
	
     while (len == 0)
     {
         len = UART_ReadPoll (uart0dev, readkey, sizeof(readkey));
     }
     
     return readkey[0];
}

/* User call back function */
void user_callback (void)
{
	complete = TRUE;
}

int c_entry (void)
{
	U8 *src_addr, *dest_addr, i; 
	
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

    /* Install IRQ for DMA controller, Init GPDMA and its channel */
    if (VIC_InstallIRQ (GP_DMA_INT,(void *)GPDMA_ISR, 0x01) == _ERROR)
    {
    	return 0;
    }
    

	UART_Puts_ (uart0dev, "Press 1 to init data!");
	while (UART0Read() != '1');
	
	// Init boundary of DMA memory used for SSP tranferring
    UART_Puts_ (uart0dev, "Init data block...");
    UART_Puts_ (uart0dev, "   Source Addr\tSource Data\tDestination Addr\tDestination Data");
    
    /* Point to memory source and destination address */
    src_addr = (U8 *)DMA_SRC;
    dest_addr = (U8 *)DMA_DST;
    
    /* Init data in source and destination */
    for ( i = 0; i < DMA_DATA_SIZE; i++ )
    {
    	*src_addr = i;
    	*dest_addr = 0;
    	
    	/* Print information */
    	UART_Puts (uart0dev, "   "); UART_PutHex (uart0dev, (U32) src_addr);
    	UART_Puts (uart0dev, "\t"); UART_PutHex (uart0dev, *src_addr);
    	UART_Puts (uart0dev, "\t"); UART_PutHex (uart0dev, (U32) dest_addr);
    	UART_Puts (uart0dev, "\t"); UART_PutHex (uart0dev, *dest_addr);
    	UART_Puts_ (uart0dev, "");
    	
    	src_addr++;
    	dest_addr++;
    }

	UART_Puts_ (uart0dev, "Press 2 to transfer data!");
	while (UART0Read() != '2');
	
    // Init DMA control
    GPDMA_Init();
    
    /* On DMA channel 0, source is memory, destination is SSP0 Tx 
     * Use:
     * - Flow control = Memory to peripheral - DMA control
     * - DMA_DATA_SIZE = 64 bytes
     * - Source BURST size = 8, Destination BURST size = 4
     * - Source width = Destination width = BYTE 
     * - DMA source = DMA_SRC
     * - DMA destination = SSP0_DMA_FIFO 
     * - Source peripheral = 0 (do not care)
     * - Destination peripheral = GPDMA_PSRC_SSP0TX */
    GPDMA_Config (0, GPDMA_M2P, (DMA_DATA_SIZE), GPDMA_BSIZE_8, GPDMA_BSIZE_4,
    		GPDMA_WIDTH_BYTE, GPDMA_WIDTH_BYTE, DMA_SRC, SSP0_DMA_FIFO, 0, GPDMA_PSRC_SSP0TX);
    /* On DMA channel 1, source is SSP0 Rx, destination is memory
     * Use:
     * - Flow control = Peripheral to memory - DMA control
     * - DMA_DATA_SIZE = 64 bytes
     * - Source BURST size = 4, Destination BURST size = 8 
     * - Source width = Destination width = BYTE
     * - DMA source = SSP0_DMA_FIFO
     * - DMA destination = DMA_DST
     * - Source peripheral = GPDMA_PSRC_SSP0RX
     * - Destination peripheral = 0 (do not care)*/
    GPDMA_Config (1, GPDMA_P2M, (DMA_DATA_SIZE), GPDMA_BSIZE_4, GPDMA_BSIZE_8,
    		GPDMA_WIDTH_BYTE, GPDMA_WIDTH_BYTE, SSP0_DMA_FIFO, DMA_DST, GPDMA_PSRC_SSP0RX,0);
    
    // Enable interrupt for DMA
    VIC_EnableInt (GP_DMA_INT);
    
    // Enable Tx and Rx DMA on SSP0
    SSP_EnableDMA (sspdev, (SSP_DMA_RXDMA | SSP_DMA_TXDMA));
    
    UART_Puts_ (uart0dev, "Proccessing...");
    
    /* Wait for GPDMA proccessing complete */
    while (GPDMA_GetTC() < 2);
	
    // Print out infor...
    UART_Puts_ (uart0dev, "Data after transferring...");
    UART_Puts_ (uart0dev, "   Source Addr\tSource Data\tDestination Addr\tDestination Data");
    
    /* Point to memory source and destination address */
    src_addr = (U8 *)DMA_SRC;
    dest_addr = (U8 *)DMA_DST;
    
    /* Init data in source and destination */
    for ( i = 0; i < DMA_DATA_SIZE; i++ )
    {
    	/* Print information */
    	UART_Puts (uart0dev, "   "); UART_PutHex (uart0dev, (U32) src_addr);
    	UART_Puts (uart0dev, "\t"); UART_PutHex (uart0dev, *src_addr);
    	UART_Puts (uart0dev, "\t"); UART_PutHex (uart0dev, (U32) dest_addr);
    	UART_Puts (uart0dev, "\t"); UART_PutHex (uart0dev, *dest_addr);
    	UART_Puts_ (uart0dev, "");
    	
    	src_addr++;
    	dest_addr++;
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
