/**
 * @file	: uart_interrupt_test.c
 * @purpose	: An example of UART using interrupt mode to test the UART driver
 * @version	: 1.0
 * @date	: 19. August. 2009
 * @author	: NguyenCao
 *----------------------------------------------------------------------------
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

#include "lpc13xx_uart.h"		/* Central include file */
#include "LPC13xx.h"
#include "lpc13xx_nvic.h"


extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];

/************************** PRIVATE VARIABLES *************************/
uint8_t menu[] =
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"UART interrupt demo \n\r"
	"\t - MCU: LPC13xx \n\r"
	"\t - ARM COTEX-M3 \n\r"
	"UART: 96000 bps - 8 data bit - No parity - 1 stop bit \n\r"
	"Print welcome screen first, then press any key to have it \n\r"
	"read in from the terminal and returned back to the terminal. \n\r"
	"********************************************************************************\n\r";
uint8_t menu1[] = "UART demo terminated!";

// Current Tx Interrupt enable state
FlagStatus TxIntStat;


/************************** PRIVATE FUNCTIONS *************************/
void print_menu(void);
void UART0_IRQHandler(void);
void UART0_IntTransmit(void);
void UART0_IntReceive(void);
uint32_t UARTReceive( uint8_t *rxbuf, uint8_t buflen);
uint32_t UARTSend(uint8_t *txbuf, uint8_t buflen);


/*********************************************************************//**
 * @brief	Print Welcome Screen Menu subroutine
 * @param	None
 * @return	None
 **********************************************************************/
void print_menu(void)
{
	UART_Send(menu, sizeof(menu), BLOCKING);
}

/*********************************************************************//**
 * @brief	Main UART testing example sub-routine
 * 			Print welcome screen first, then press any key to have it
 * 			read in from the terminal and returned back to the terminal.
 * 			- Press ESC to exit
 * 			- Press 'r' to print welcome screen menu again
 **********************************************************************/
int c_entry(void)
{
	SystemInit();

	// DeInit NVIC and SCBNVIC
	NVIC_DeInit();
	NVIC_SCBDeInit();

	/* Configure the NVIC Preemption Priority Bits:
	 * two (2) bits of preemption priority, six (6) bits of sub-priority.
	 * Since the Number of Bits used for Priority Levels is five (5), so the
	 * actual bit number of sub-priority is three (3)
	 */
	NVIC_SetPriorityGrouping(0x05);

	//  Set Vector table offset value
#if (__RAM_MODE__==1)
	NVIC_SetVTOR(0x10000000);
#else
	NVIC_SetVTOR(0x00000000);
#endif

	UART_Init();
	print_menu();

	/* Enable the UART Interrupt */
	NVIC_EnableIRQ(UART_IRQn);

	UART_IntConfig(UART_INTCFG_RBR,ENABLE);
	UART_IntConfig(UART_INTCFG_THRE,ENABLE);
	UART_IntConfig(UART_INTCFG_RLS,ENABLE);

	while (1)/* Loop forever */
	{
		if ( UARTCount != 0 )
		{
			UART_IntConfig(UART_INTCFG_RBR,DISABLE); /* Disable RBR interrupt */
		  UART_Send( (uint8_t *)UARTBuffer, UARTCount, BLOCKING );
		  UARTCount = 0;
		  UART_IntConfig(UART_INTCFG_RBR,ENABLE); /* Re-enable RBR interrupt */
		}
	}
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
