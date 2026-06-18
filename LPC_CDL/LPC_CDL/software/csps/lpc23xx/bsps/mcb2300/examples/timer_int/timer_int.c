/***********************************************************************
 * ID: timer_int.c 2008-11-03 HieuNguyen
 *
 * Project: LPC23xx Timer driver example
 *
 * Description:
 *     A simple timer driver example uses timer 0 in interrupt mode
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
#include "lpc23xx_vic_driver.h"

CHAR menu1[] = 
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
"Timer interrupt demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Use timer 0 ticks at speed of  10Hz, LED P2.0 blinks at speed of 5Hz \n\r"
" Use timer 1 ticks at speed of  1Hz, LED P2.1 blinks at speed of 0.5Hz \n\r"	
"********************************************************************************\n\r";
STATIC S32 tc0dev = 0, tc1dev = 0, uartdev = 0;
S32 volatile timer0_flag = FALSE, timer1_flag = FALSE;

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

/***********************************************************************
 *
 * Function: timer0_user_interrupt
 *
 * Purpose: Timer 0 interrupt handler
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
void timer0_user_interrupt(void)
{
    /* Clear latched timer interrupt */
	TC_ClrMatchIntPending (tc0dev, 0);

	UART_Puts_ (uartdev, "T0 tick!");
	
    /* toggle led status */
    if (timer0_flag == TRUE)
    {
    	FIO_Set32 (FIO_2, 0x01);
    	timer0_flag = FALSE;
    }
    else
    {
    	FIO_Clr32 (FIO_2, 0x01);
    	timer0_flag = TRUE;
    }
    
    /* Ack and update VIC Vector Addr */
    VIC_Ack();
}

/***********************************************************************
 *
 * Function: timer1_user_interrupt
 *
 * Purpose: Timer 1 interrupt handler
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
void timer1_user_interrupt(void)
{	
	/* Clear latched timer interrupt */
	TC_ClrMatchIntPending (tc1dev, 0);

	UART_Puts_ (uartdev, "T1 tick!");
	
    /* toggle led status */
    if (timer1_flag == TRUE)
    {
    	FIO_Set32 (FIO_2, 0x02);
    	timer1_flag = FALSE;
    }
    else
    {
    	FIO_Clr32 (FIO_2, 0x02);
    	timer1_flag = TRUE;
    }
    
    /* Ack and update VIC Vector Addr */
    VIC_Ack();
}

/***********************************************************************
 *
 * Function: c_entry
 *
 * Purpose: Application entry point from the startup code
 *
 * Processing:
 *     See function.
 * 	   Two LEDs will blink at different speeds: 1Hz and 10Hz
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
	UART_WritePoll (uartdev, menu1, sizeof(menu1));
	
	/* Install interrupt match value timer */
	if (VIC_InstallIRQ (TIMER0_INT,(void *)timer0_user_interrupt, 0x01) == _ERROR)
	{
		return (0);
	}
	
	if (VIC_InstallIRQ(TIMER1_INT,(void *)timer1_user_interrupt, 0x02) == _ERROR)
	{
		return (0);
	} 

	// Init timer 0
	if ((tc0dev = TC_Init (0x00)) == 0)
	{
		return 0;
	}
	// Init timer 1
	if ((tc1dev = TC_Init (0x01)) == 0)
	{
		return 0;
	}
	
	/* Setup timer 0 for a 10Hz match rate                            */
	/* Use a prescale count time of 100uS                             */
	TC_SetupTimer (tc0dev, TC_PRESCALE_USVAL, 100);

	/* Use a match count value of 1000 (1000 * 100uS = 100mS --> 10Hz)*/
	/* Use match register set 0 (of 0..3) */
	/* Generate match interrupt on match */
	/* Do not stop timer on match */
	/* Reset timer counter on match */
	/* Match is when timer count is 1000 */
	TC_EnableMatchInt (tc0dev, 0);
	TC_DisableMatchStop (tc0dev, 0);
	TC_EnableMatchReset (tc0dev, 0);
	TC_SetMatch (tc0dev, 0, 1000);

	/* Setup timer 1 for a 1Hz match rate                           */
	/* Use a prescale count time of 1000uS                          */
	/* Use ps_us_val value, 1000uS */
    TC_SetupTimer (tc1dev, TC_PRESCALE_USVAL, 1000);
    
	/* Use a match value of 1000 (1000 * 1000uS = 1s --> 1Hz)       */
	/* Use match register set 0 (of 0..3) */
	/* Generate match interrupt on match */
	/* Do not stop timer on match */
	/* Reset timer counter on match */
	TC_EnableMatchInt (tc1dev, 0);
	TC_DisableMatchStop (tc1dev, 0);
	TC_EnableMatchReset (tc1dev, 0);
	TC_SetMatch (tc1dev, 0, 1000);

	/******************************************************************/

	/* Enable timers (starts counting) */
	TC_Start (tc0dev);
	TC_Start (tc1dev);

    VIC_EnableInt (TIMER0_INT);
    VIC_EnableInt (TIMER1_INT);
    
	while(1);
	return (1);
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}
