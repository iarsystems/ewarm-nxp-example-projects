/***********************************************************************
 * ID: rtc_32k_pdown.c 2008-06-09 LinhNguyen
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
#include "lpc23xx_scb_driver.h"

/* Timer device handles */
static S32 rtcdev;
static S32 uart0dev = 0;

CHAR menu0[] = 
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
"RTC 32K - PowerDown demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Use RTC with external oscilator 32KHz \n\r"
" To generate interrupt in Second Counter Increment Interrupt (1s) in Normal Operation \n\r"
" and Alarm Match Interrupt mode (after each 10s) to toggle POWERDOWN operation mode \n\r"
" (enter to /wakeup from PowerDown mode of MCU operation) \n\r"
"********************************************************************************\n\r";
static S32 toggle = 0;
static S32 toggle_pd = 0;

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
	
	// Read Int pending
	regval = RTC_ReadIntPending (rtcdev);
	
	/* Check if waking-up from POWERDOWN mode
	 * Re-config PLL for system clock after waking-up from
	 * POWER DOWN */
	if (toggle_pd == 1)
	{
		// Reconfig system clock 
		SCB_SetupSYSCLK (12000000, 12, 1, 0x01, 6, 6);
	}
	
	if (regval & RTC_IRL_CIF)
	{
		/* Send debug information */
		UART_Puts_ (uart0dev, "RTC interrupt in normal operation");
		
		/* This is increment counter interrupt*/
		if (toggle == 1) 
		{
			FIO_Clr32 (FIO_2, 0xFF);
			toggle = 0;
		}
		else
		{
			toggle = 1;
			FIO_Set32 (FIO_2, 0xFF);
		}

		// Get current value of second
		secval = RTC_GetTime (rtcdev, RTC_TIMETYPE_SECOND);
		
		// Print infor..
		UART_Puts (uart0dev, "Second: "); UART_PutDec (uart0dev, secval); 
		UART_Puts_ (uart0dev, "");
	}
	
	/* Continue to check the Alarm match*/
	if (regval & RTC_IRL_ALF)
	{
		/* Set the AMR for next 10s match alarm interrupt */
		RTC_SetALTime (rtcdev, RTC_TIMETYPE_SECOND, 
				(RTC_GetTime (rtcdev, RTC_TIMETYPE_SECOND) + 10));
		
		// Enable POWER DOWN FLAG
		if(toggle_pd == 0)
		{
			// Toggle PD flag
			toggle_pd = 1;
			
			// Disable Second counter increment interrupt 
			RTC_DisableCII (rtcdev, RTC_TIMETYPE_SECOND);
			
			/* Send debug information */
			UART_Puts_ (uart0dev, "Second Alarm matched! Enable POWERDOWN now...!");

		    /* Wait for current UART transmission complete before init PD mode */
		    while (!(UART_GetStatus (uart0dev) & UART_LSR_THRE));
		    secval = 0xFFFF;
		    while (secval--);
		}
		else
		{
			/* Send debug information */
			UART_Puts_ (uart0dev, "RTC interrupt from POWERDOWN mode");
			// Toggle PD flag
			toggle_pd = 0;
			
			// Re-Enable Second counter increment interrupt for normal operation
			RTC_EnableCII (rtcdev, RTC_TIMETYPE_SECOND);
		}
	}
	
	/* Clear Int pending */
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
	
	// Select source for RTC: external OSC 32K
	RTC_SetClockSource (rtcdev, RTC_CLKSRC_EXT32K);
	
	// Disable Alarm interrupt for each type
	RTC_DisableALI (rtcdev, 0xFF);
	// Disable Counter Increment interrupt for each type
	RTC_DisableCII (rtcdev, 0xFF);
	
    /* Enable rtc (starts increase the tick counter and second counter register) */
	RTC_Reset (rtcdev);
	RTC_Start (rtcdev);

	RTC_SetTime (rtcdev, RTC_TIMETYPE_SECOND, 0);
	
	/* Set ALARM time for second */
	RTC_SetALTime (rtcdev, RTC_TIMETYPE_SECOND, 5);

	/* Set the CIIR for second counter interrupt*/
	RTC_EnableCII (rtcdev, RTC_TIMETYPE_SECOND);
	/* Set the AMR for 10s match alarm interrtupt */
	RTC_EnableALI (rtcdev, RTC_TIMETYPE_SECOND);
	
    /* Enable the global RTC interrupt */
    VIC_EnableInt (RTC_INT);
    
    // Set Wake-up source according to RTC
    SCB_SetWakeUpSource(RTCWAKE);
	
	while(1)
	{
		if (toggle_pd == 1)
		{
			// PDOWN init
			SCB_InitPDOWN();
		}
	}
	
	return (1);
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}
