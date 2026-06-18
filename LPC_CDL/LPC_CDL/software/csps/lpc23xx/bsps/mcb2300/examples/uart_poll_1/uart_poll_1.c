/***********************************************************************
 * uart_poll_1.c 13 2008-09-11 --- editted by HieuNguyen
 * 
 * Project: UART driver demo
 *
 * Description:
 *     A UART driver (polling mode) example.
 * 
 * Note:
 * 		Using new HAL structure
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
#include "mcb2300_init.h"

/* UART device handle */
STATIC S32 uartdev;

CHAR menu1[] = "Hello NXP Semiconductors \n\r";
CHAR menu2[] = "UART polling mode demo \n\r\t MCU LPC2368 - ARM7TDMI-S \n\r\t UART1 - 115200bps \n\r";
CHAR menu3[] = "UART demo terminated!";
/* Print a menu */
void print_menu(void)
{
    UART_WritePoll(uartdev, menu1, sizeof(menu1));
    UART_WritePoll(uartdev, menu2, sizeof(menu2));
}

/***********************************************************************
 *
 * Function: c_entry
 *
 * Purpose: UART example code without interrupts
 *
 * Processing:
 *     Sends out a 'simulated' menu to UART 0 at 115.2K-N-8-1. Press the
 *     'r' key to display the menu again. Press ESC to end the example.
 *     Any other key is simply wrapped back around to the terminal.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Always returns 1
 *
 * Notes:
 *     Using polling mode is not as easy as using interrupt mode!
 *
 **********************************************************************/

int c_entry(void)
{
    CHAR buffer[10];
    S32 len, idx;
    BOOL32 volatile exitflag = FALSE;
    
    /* init MCB2300 component included PLL setting */
    mcb2300_init();
    
    /* Open UART0 - 115200 bps, 8 databit, 0 parity, 1 stop bit */    
    if ((uartdev = UART_Init (UART_PORT_1, 115200, 8, 0, 1)) == 0x00)
    {
        return 0;
    }

    /* Print a menu */  
    print_menu();
	
    /* Read some data from the buffer */
    while (exitflag == FALSE)
    {
       len = 0;
        while (len == 0)
        {
            len = UART_ReadPoll (uartdev, buffer, sizeof(buffer));
        }
        
        /* Got some data */
        idx = 0;
        while (idx < len)
        {
            if (buffer[idx] == 27)
            {
                /* ESC key, set exit flag */
            	UART_WritePoll(uartdev, menu3, sizeof(menu3));
                exitflag = TRUE;
            }
            else if (buffer[idx] == 'r')
            {
                print_menu();
            }
            else
            {
                /* Echo it back */
                UART_WritePoll (uartdev, &buffer[idx], 1);
            }
            idx++;
        }
    }

    /* Close UART */
    UART_Close (uartdev);
    
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
