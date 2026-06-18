/***********************************************************************
 * ID: wdt_int.c 2008-11-07 HieuNguyen
 *
 * Project: wdt driver example
 *
 * Description:
 *     A simple wdt driver example.
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
#include "lpc23xx_wdt_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_uart_driver.h"
#include "lpc23xx_vic_driver.h"
#include "mcb2300_init.h"
#include "lpc23xx_tc_driver.h"

#define WDT_FEED_VALUE	0x003FFFFF

CHAR menu0[]=
"********************************************************************************\n\r"
"This Welcome Screen below will executive after reset event \n\r" 
"Hello NXP Semiconductors \n\r"
" Watch dog timer reset when timeout demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Use WDT with Interrnal RC OSC, reset mode, timeout = 2000000 microsecond \n\r"
" To reset MCU when time out. After reset, program will determine what cause of "
" last reset time (external reset or WDT time-out)\n\r"
"********************************************************************************\n\r";
/* Timer device handles */
STATIC S32 uart0dev = 0, tcdev = 0;
STATIC S32 volatile LED_toggle = FALSE;

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


/* Init GPIO that drive LEDs */
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
	U64 ti;
	
	mcb2300_init();
	
	// init LED
	LED_Init();
	
	if ((UART0_Init()) == _ERROR)
	{
		return 0;
	}
	
	// Init timer 0
	if ((tcdev = TC_Init (0x00)) == 0)
	{
		return 0;
	}
	
	// Print welcome screen
	UART_WritePoll (uart0dev, menu0, sizeof(menu0));
	
	// Read back TimeOut flag to determine previous timeout reset
	if ((WDT_ReadFlagPending() & WDT_TOF_FLAG) != 0)
	{
		UART_Puts_ (uart0dev, "Last MCU reset caused by WDT TimeOut!");
		// Clear WDT TimeOut
		WDT_ClrTimeOutFlag();
	}
	else
	{
		UART_Puts_ (uart0dev, "Last MCU reset caused by external reset!");
	}
	
	// Init WDT, IRC OSC, interrupt mode, timeout = 2000000 microsecond
	WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_RESET, 2000000);
	
	/* Enable the Watch dog */
    VIC_EnableInt (WDT_INT);
    
    // Start WDT...
    WDT_Start ();
    
	while(1)
	{
		if (LED_toggle == FALSE)
		{
			FIO_Set32 (FIO_2, 0xFF);
			UART_Puts_ (uart0dev, "Tick ON");
			LED_toggle = TRUE;
		}
		else
		{
			FIO_Clr32 (FIO_2, 0xFF);
			UART_Puts_ (uart0dev, "Tick OFF");
			LED_toggle = FALSE;
		}
		
		// Wait for 1000 millisecond
		TC_Waitms (tcdev, 500);		
	}

	return (1);
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}
