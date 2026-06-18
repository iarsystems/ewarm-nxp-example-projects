/***********************************************************************
 * ID: rtc_32k.c 2008-11-05 HieuNguyen
 *
 * Project: rtc driver example
 *
 * Description:
 *     A simple realtime clock driver example.
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
#include "lpc23xx_rtc_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_uart_driver.h"
#include "lpc23xx_vic_driver.h"
#include "mcb2300_init.h"

/* Timer device handles */
static S32 rtcdev = 0;
static S32 uart0dev = 0;

CHAR menu0[] = 
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
"RTC internal system clock demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Use RTC with internal system clock (Peripheral clock) \n\r"
" With PREINT and PREFRAC register to optimize RTC clock input \n\r"
" To generate interrupt in Second Counter Increment Interrupt (1s)\n\r"
" and Alarm Match Interrupt mode (10s) \n\r"
"********************************************************************************\n\r";

static S32 toggle = 0;

/* Init pin that used to drive LED */
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

/***********************************************************************
 *
 * Function: rtc_second_interrupt
 *
 * Purpose: rtc interrupt handler for each second increase in
 * second counter register.
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void rtc_second_interrupt(void)
{
	U32 regval = 0;
	U32 secval;
	
	regval = RTC_ReadIntPending (rtcdev);
		
	if (regval & RTC_IRL_CIF) 
	{
		/* This is increment counter interrupt*/
		if(toggle == 1) 
		{
			FIO_Clr32 (FIO_2, 0xFF);
			toggle = 0;
		}
		else
		{
			toggle = 1;
			FIO_Set32 (FIO_2, 0xFF);
		}

		secval = RTC_GetTime (rtcdev, RTC_TIMETYPE_SECOND);
		
		/* Send debug information */
		UART_Puts (uart0dev, "Second: "); UART_PutDec (uart0dev, secval); 
		UART_Puts_ (uart0dev, "");
	}

	/* Continue to check the Alarm match*/
	if (regval & RTC_IRL_ALF)
	{
		/* Send debug information */
		UART_Puts_ (uart0dev,"ALARM 10s matched!");
	}
	
	/* Erase the interrupt flags: RTC->rtc_ilr |=1*/
	RTC_ClrIntPending (rtcdev, regval);
	
    /* Ack and update VIC Vector Addr */
    VIC_Ack();
}

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

/***********************************************************************
 *
 * Function: c_entry
 *
 * Purpose: Application entry point from the startup code
 *
 * Processing:
 *     See function.
 * 	   real time clock will tick and second counter register will 
 * 		increase and generate the interrupt for each second. 
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Always returns 1
 *
 * Notes: None
 *
 **********************************************************************/
int c_entry(void)
{
	RTC_TIME_Type RTCFullTime;
	
	mcb2300_init();

	// Init LED port 
	LED_Init();
	
	if ((UART0_Init()) == _ERROR)
	{
		return 0;
	}
	
	// Print welcome screen
	UART_WritePoll (uart0dev, menu0, sizeof(menu0));
	
	/* Install interrupt for counter second increment */
	if (VIC_InstallIRQ (RTC_INT,(void *)rtc_second_interrupt, 0x01) == _ERROR)
	{
		return (0);
	}
	
	// Init RTC module 
	rtcdev = RTC_Init();
	
	// Select source for RTC: RTC peripheral clock 
	RTC_SetClockSource (rtcdev, RTC_CLKSRC_SYSCLK);
	
	// Print info...
	UART_Puts_ (uart0dev, "Clocksource selected from RTC peripheral clock \n\r"
			" CCLK = 48MHz \n\r"
			" RTC divider from CCLK = 4 \n\r"
			" --> PCLK(RTC) = 12MHz \n\r");
	UART_Puts (uart0dev, "Value of PREINT = (PCLK / 32768) - 1 = "); 
	UART_PutDec (uart0dev, RTC->PREINT); UART_Puts_ (uart0dev, "");
	UART_Puts (uart0dev, "Value of PREFRAC = PCLK - ((PREINT + 1) * 32768) = "); 
	UART_PutDec (uart0dev, RTC->PREFRAC); UART_Puts_ (uart0dev, "");
	
	// Disable Alarm interrupt for each type
	RTC_DisableALI (rtcdev, 0xFF);
	// Disable Counter Increment interrupt for each type
	RTC_DisableCII (rtcdev, 0xFF);
	
    /* Enable rtc (starts increase the tick counter and second counter register) */
	RTC_Reset (rtcdev);
	RTC_Start (rtcdev);
	
	/* Set current time for RTC */
	// Current time is 8:00:00AM, 2008-11-08
	RTC_SetTime (rtcdev, RTC_TIMETYPE_SECOND, 0);
	RTC_SetTime (rtcdev, RTC_TIMETYPE_MINUTE, 0);
	RTC_SetTime (rtcdev, RTC_TIMETYPE_HOUR, 8);
	RTC_SetTime (rtcdev, RTC_TIMETYPE_MONTH, 11);
	RTC_SetTime (rtcdev, RTC_TIMETYPE_YEAR, 2008);
	RTC_SetTime (rtcdev, RTC_TIMETYPE_DAYOFMONTH, 6);
	
	// Get and print current time
	RTC_GetFullTime (rtcdev, &RTCFullTime);
	UART_Puts (uart0dev, "Current time set to: ");
	UART_PutDec (uart0dev, (RTCFullTime.HOUR)); UART_Puts (uart0dev, ":");
	UART_PutDec (uart0dev, (RTCFullTime.MIN)); UART_Puts (uart0dev, ":");
	UART_PutDec (uart0dev, (RTCFullTime.SEC)); UART_Puts (uart0dev, "  ");
	UART_PutDec (uart0dev, (RTCFullTime.DOM)); UART_Puts (uart0dev, "/");
	UART_PutDec (uart0dev, (RTCFullTime.MONTH)); UART_Puts (uart0dev, "/");
	UART_PutDec (uart0dev, (RTCFullTime.YEAR)); UART_Puts_ (uart0dev, "");
	
	/* Set ALARM time for second */
	RTC_SetALTime (rtcdev, RTC_TIMETYPE_SECOND, 10);
	
	UART_Puts (uart0dev, "Second ALARM set to "); 
	UART_PutDec (uart0dev, RTC_GetALTime (rtcdev, RTC_TIMETYPE_SECOND));
	UART_Puts_ (uart0dev, "s");
	
	/* Set the CIIR for second counter interrupt*/
	RTC_EnableCII (rtcdev, RTC_TIMETYPE_SECOND);
	/* Set the AMR for 10s match alarm interrtupt */
	RTC_EnableALI (rtcdev, RTC_TIMETYPE_SECOND);
	
    /* Enable the global RTC interrupt */
    VIC_EnableInt (RTC_INT);
    
    /* Now loop to test RTC 32768Hz external interrupt */
	while(1);
	
	return (1);
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}
