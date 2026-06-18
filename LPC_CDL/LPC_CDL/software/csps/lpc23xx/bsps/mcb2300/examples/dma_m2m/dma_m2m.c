/***********************************************************************
 * dma_m2m_ex.c		2008-10-22	HieuNguyen
 * 
 * Project: GPDMA driver example
 *
 * Description:
 *     This example will transfer 2 block of data from memory boundary
 * to the other memory boundary on RAM using GPDMA module with interrupt.
 * 
 * Note:
 *
 ***********************************************************************
 * Software that is described herein is for illustrative purposes only  
 * which provides customers with programming information regarding the  
 * products. This software is supplied "AS IS" without any warranties.  
 * NXP Semiconductors assumes no responsibility or liability for the 
 * use of the software, conveys no license or title under any patent, 
 * copyright, or mask work right to the product. NXP Semiconductors 
 * reserves the right tll
 * o make changes in the software without 
 * notification. NXP Semiconductors also make no representation or 
 * warranty that such application will be suitable for the specified 
 * use without further testing or modification. 
 **********************************************************************/
#include "arm7tdmis_types.h"
#include "lpc23xx_pinsel_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_uart_driver.h"
#include "lpc23xx_vic_driver.h"
#include "lpc23xx_dma_driver.h"
#include "mcb2300_init.h"

#define DMA_SRC			0x7FD00000
#define DMA_DST			0x7FD01000
#define DMA_SIZE		0x100

/* UART device handle */
STATIC S32 uartdev;

CHAR menu1[] = 
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
"General Purpose DMA demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
"Transfer a block of data on DMA On-chip RAM \n\r"
"\t - Source Addr: 0x7FD00000 \n\r"
"\t - Destination Addr: 0x7FD01000 \n\r"
"\t - Size of block data: 256 bytes \n\r"
"********************************************************************************\n\r";

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

int c_entry(void)
{    
	U32 *src_addr, *dest_addr, i; 
	
    /* init MCB2300 component included PLL setting */
    mcb2300_init();
    
    /* Open UART0 - 115200 bps, 8 databit, 0 parity, 1 stop bit */    
    if ((uartdev = UART_Init (UART_PORT_0, 115200, 8, 0, 1)) == 0x00)
    {
        return 0;
    }

    UART_WritePoll (uartdev, menu1, sizeof(menu1));
    UART_Puts_ (uartdev, "Press '1' to initilize data block!");
    
    // Wait for key pressed 
    while (UART0Read() != '1');
    
    UART_Puts_ (uartdev, "Init data block...");
    UART_Puts_ (uartdev, "   Source Addr\tSource Data\tDestination Addr\tDestination Data");
    
    /* Point to memory source and destination address */
    src_addr = (U32 *)DMA_SRC;
    dest_addr = (U32 *)DMA_DST;
    
    /* Init data in source and destination */
    for ( i = 0; i < DMA_SIZE/4; i++ )
    {
    	*src_addr = i;
    	*dest_addr = 0;
    	
    	/* Print information */
    	UART_Puts (uartdev, "   "); UART_PutHex (uartdev, (U32) src_addr);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, *src_addr);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, (U32) dest_addr);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, *dest_addr);
    	UART_Puts_ (uartdev, "");
    	
    	src_addr++;
    	dest_addr++;
    }

    /* Install IRQ for DMA controller, Init GPDMA and its channel */
    if (VIC_InstallIRQ (GP_DMA_INT,(void *)GPDMA_ISR, 0x01) == _ERROR)
    {
    	return 0;
    }
    
    UART_Puts_ (uartdev, "Press '2' to transfer data block!");
    
    // Wait for key pressed 
    while (UART0Read() != '2');
    
    GPDMA_Init();
    GPDMA_Config (0, GPDMA_M2M, (DMA_SIZE/4), GPDMA_BSIZE_32, GPDMA_BSIZE_32,
    		GPDMA_WIDTH_WORD, GPDMA_WIDTH_WORD, DMA_SRC, DMA_DST, 0,0);
    
    VIC_EnableInt (GP_DMA_INT);    
    
    UART_Puts_ (uartdev, "Proccessing...");
    
    /* Wait for GPDMA proccessing complete */
    while (!(GPDMA_GetTC()));

    UART_Puts_ (uartdev, "Complete!!!");
    UART_Puts_ (uartdev, "Verifying data...");
    UART_Puts_ (uartdev, "   Source Addr\tSource Data\tDestination Addr\tDestination Data");
    
    /* Verify result */
    src_addr = (U32 *)DMA_SRC;
    dest_addr = (U32 *)DMA_DST;
    for ( i = 0; i < DMA_SIZE/4; i++ )
    {
    	/* Print information */
    	UART_Puts (uartdev, "   "); UART_PutHex (uartdev, (U32) src_addr);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, *src_addr);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, (U32) dest_addr);
    	UART_Puts (uartdev, "\t"); UART_PutHex (uartdev, *dest_addr);
    	UART_Puts_ (uartdev, "");
    	
    	if ( *src_addr != *dest_addr )
    	{
    		UART_Puts_ (uartdev, "Error occur, GPDMA does not completed!");
    		break;
    	}
    	src_addr++;
    	dest_addr++;
    }
    
    if (i == (DMA_SIZE/4))
    {
    	UART_Puts_ (uartdev, "Verifying complete!!!");
    }
    
    UART_Puts_ (uartdev, "GPDMA demo terminated!");
    
    /* Loop forever */
    while(1);
    return 1;
}	

/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}
