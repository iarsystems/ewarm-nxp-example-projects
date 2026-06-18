#include "lpc23xx_pinsel_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_vic_driver.h"
#include "mcb2300_init.h"
#include "lpc23xx_uart_driver.h"
#include "lpc23xx_mci_driver.h"
#include "lpc23xx_dma_driver.h"

#define BLOCK_NUM	0x80
#define DATA_BUFFER_SIZE 2048
STATIC S32 uartdev;

S8 menu1[] = "Hello NXP Semiconductors \n\r";
S8 menu2[] = "Testing for MCI Driver \n\r";

S8 WriteBlockBuffer[BLOCK_LENGTH];
S8 ReadBlockBuffer[BLOCK_LENGTH];

S32 puts_(const void *s) {
	UART_Puts_(uartdev, s);
	return 1;
}

U32 uart_init() {
	PINSEL_Set(PORT_0, 15, FUNC_1);
	PINSEL_Set(PORT_0, 16, FUNC_1);

	/* Open UART0 */
	/* Check ioctrl function... */
	/* Set UART 1 to 115.2K-N-8-1 */
	if ((uartdev = UART_Init(UART_PORT_1, 115200, 8, 0, 1)) == 0x00) {
		return 0;
	}
	return 0;
}

int c_entry() {
	U32 i = 0;
	U32 j = 0;
	U32 cardType;

	mcb2300_init();
	uart_init();

	for (i = 0; i < (BLOCK_LENGTH); i++) {
		WriteBlockBuffer[i] = i;
		ReadBlockBuffer[i] = 0;
	}

#if MCI_DMA_ENABLED
	/* on DMA channel 0, source is memory, destination is MCI FIFO. */
	/* On DMA channel 1, source is MCI FIFO, destination is memory. */
	GPDMA_Init();
#endif

	puts_(menu1);
	puts_(menu2);

	MCI_Start();
	cardType = MCI_CardInit();
	switch (cardType) {
	case 0:
		puts_("Card Unknow");
		break;
	case 1:
		puts_("MMC Card");
		break;
	case 2:
		puts_("SD Card");
		break;
	}
	if (MCI_CheckCID() == TRUE) {
		puts_("CID good");
	}
	if (MCI_SetAddress() == TRUE) {
		puts_("Set address good");
	}

	if (MCI_SendCSD(&i) == TRUE) {
		puts_("Send CSD good");
	}

	if (MCI_SelectCard() == TRUE) {
		puts_("Select card good");
	}

	if (MCI_CardType == SD_CARD) {
		MCI_SetClockRate(MCI_NORMAL_RATE);
		if (MCI_SetBusWidth(BUS_WIDTH_4BITS) == TRUE) {
			puts_("Clock Rate and Bus Width are set");
		}
	}

	if (MCI_SetBlockLength(BLOCK_LENGTH) == TRUE) {
		puts_("Set Block Length OK");
	}

	/***************************************************************/
	/* TEST - Write and Read number of blocks defined by BLOCK_NUM */
	/***************************************************************/
	for (i = 0; i < BLOCK_NUM; i++) {
		if (MCI_WriteBlock(i, WriteBlockBuffer, 1) != TRUE) {
			while (1)
				; /* Fatal error */
		}
		/* When MCI_Block_End_Flag is clear, it indicates
		 Write_Block is complete, next task, Read_Block to check write */
		while (MCI_Block_End_Flag == 1)
			;
		puts_("Write block is finished");
		if (MCI_ReadBlock(i, ReadBlockBuffer, 1) != TRUE) {
			while (1)
				; /* Fatal error */
		}
		/* When MCI_Block_End_Flag is clear, it indicates RX is done
		 with Read_Block,  validation of RX and TX buffers next. */
		while (MCI_Block_End_Flag == 1)
			;
		puts_("Read block is finished");
		for (j = 0; j < (BLOCK_LENGTH); j++) {
			if (WriteBlockBuffer[j] != ReadBlockBuffer[j]) {
				while (1)
					; /* Data comparison failure, fatal error */
			}
		}
		puts_("Verify Write block and Read block is finished");
		/*  clear read buffer for next block comparison */
		for (j = 0; j < (BLOCK_LENGTH); j++) {
			ReadBlockBuffer[j] = 0;
		}
	}
	return 0;

}

int main() {
	c_entry();
}

