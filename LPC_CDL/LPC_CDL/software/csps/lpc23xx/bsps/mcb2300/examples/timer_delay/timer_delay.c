/***********************************************************************
 * ID: timer_delay.c 2008-03-11 HieuNguyen
 *
 * Project: LPC23xx Timer driver example
 *
 * Description:
 *     A simple timer driver example uses timer 0 in polling mode
 *
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
#include "lpc23xx_pinsel_driver.h"
#include "lpc23xx_gpio_driver.h"
#include "lpc23xx_tc_driver.h"
#include "lpc23xx_scb_driver.h"
#include "mcb2300_init.h"
#include "lpc23xx_uart_driver.h"

CHAR menu1[] = 
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
"Timer delay demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Use timer 0 in polling mode \n\r"
" Wait 1 second to turn ON/OFF LED in sequense \n\r"
"********************************************************************************\n\r";
STATIC S32 tcdev = 0, uartdev = 0;

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

void LED_Init (void)
{
	U8 temp;
	
	for (temp = 0; temp < 8; temp++)
	{
		// Set function mode on P2.0 to P2.7 pin to GPIO 
		PINSEL_Set (PORT_2, temp, FUNC_0);
		// Set internal resistor pull-up
		PINSEL_SetResistorMode (PORT_2, temp, PINMODE_PULLUP);
	}
	
	// Set direction to output
	FIO_SetDirOutput32 (FIO_2, 0xFF);
	
	/* Turn off all LEDs */
	FIO_Clr32 (FIO_2, 0xFF);
}


int c_entry(void)
{
	mcb2300_init();

	// Init LED port 
	LED_Init();
	
	if ((UART0_Init()) == _ERROR)
	{
		return 0;
	}
	
	// Print welcome screen
	UART_WritePoll (uartdev, menu1, sizeof(menu1));
	
	// Init timer
	if ((tcdev = TC_Init (0x00)) == 0)
	{
		return 0;
	}
	
	while (1)
	{
		// Wait for 1000 millisecond
		TC_Waitms (tcdev, 1000);
		FIO_Set32 (FIO_2, 0xFF);
		UART_Puts_ (uartdev, "Tick ON");
		// Wait for 1000 millisecond
		TC_Waitms (tcdev, 1000);
		FIO_Clr32 (FIO_2, 0xFF);
		UART_Puts_ (uartdev, "Tick OFF");
	}
	
	return (1);
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}
