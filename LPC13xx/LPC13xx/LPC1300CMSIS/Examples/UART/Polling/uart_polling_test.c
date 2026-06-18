/**
 * @file	: uart_polling_test.c
 * @purpose	: An example of UART using polling mode to test the UART driver
 * @version	: 1.0
 * @date	: 18. Mar. 2009
 * @author	: HieuNguyen
 * @modify	: 18. Aug. 2009 by NguyenCao
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
#include "system_LPC13xx.h"
#include "lpc13xx_nvic.h"

/************************** PRIVATE VARIABLES *************************/
uint8_t menu[] =
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"UART polling demo \n\r"
	"\t - MCU: LPC13xx \n\r"
	"\t - ARM COTEX-M3 \n\r"
	"UART: 96000 bps - 8 data bit - No parity - 1 stop bit \n\r"
	"Print welcome screen first, then press any key to have it \n\r"
	"read in from the terminal and returned back to the terminal.\n\r"
	"- Press ESC to exit.\n\r"
	"- Press 'r' to print welcome screen menu again.\n\r"
	"********************************************************************************\n\r";
uint8_t menu1[] = "UART demo terminated!";

/************************** PRIVATE FUNCTIONS *************************/
void print_menu(void);

/*********************************************************************//**
 * @brief		Print Welcome menu
 * @param[in]	none
 * @return 		None
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
int c_entry(void) {
	uint32_t idx, len;
	FlagStatus exitflag;
	uint8_t buffer[10];

	// Initialize System clock
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

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_Init();
	print_menu();

	//Config trigger FIFO level = 0;
	UART_FIFOConfig(UART_FIFO_TRGLEV0);

	// Enable UART Transmit
	UART_TXCmd(ENABLE);

	// Reset exit flag
	exitflag = RESET;

    /* Read some data from the buffer */
    while (exitflag == RESET)
    {
       len = 0;
        while (len == 0)
        {
            len = UART_Receive(buffer, sizeof(buffer), NONE_BLOCKING);
        }

        /* Got some data */
        idx = 0;
        while (idx < len)
        {
            if (buffer[idx] == 27)
            {
                /* ESC key, set exit flag */
            	UART_Send(menu1, sizeof(menu1), BLOCKING);
                exitflag = SET;
            }
            else if (buffer[idx] == 'r')
            {
                print_menu();
            }
            else
            {
                /* Echo it back */
            	UART_Send(&buffer[idx], 1, BLOCKING);
            }
            idx++;
        }
    }

    // wait for current transmission complete - THR must be empty
    while (UART_CheckBusy() == SET);

    // DeInitialize UART0 peripheral
    UART_DeInit();

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

