/***********************************************************************
 * ID: pwm_single.c 2008-10-31 HieuNguyen
 *
 * Project: PWM driver example
 *
 * Description:
 * This program illustrates the PWM signal on 6 Channel in single edge mode
 * Peripheral clock for PWM: PWM_PCLK = CCLK / 4 = 12MHz and there is no prescale for PWM
 * So that the PWM timer/counter clock is at 12MHz. The base rate is set to 256
 * So the base PWM frequency is at 12MHz/256 = 46.875 KHz (Period = ~21.34 microsecond)
 * Each PWM channel (1 to 6) will be configured as following:
 * + PWM1.1 = 10% (period = 2.087 microsecond)
 * + PWM1.2 = 20% (period = 4.250 microsecond)
 * + PWM1.3 = 40% (period = 8.502 microsecond)
 * + PWM1.4 = 50% (period = 10.67 microsecond)
 * + PWM1.5 = 70% (period = 14.92 microsecond)
 * + PWM1.6 = 90% (period = 19.17 microsecond)
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
#include "lpc23xx_pwm_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_uart_driver.h"
#include "mcb2300_init.h"

S32 volatile uartdev, pwmdev;

CHAR menu1[] = 
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
"PWM demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" This program illustrates the PWM signal on 6 Channel in single edge mode\n\r"
" Peripheral clock for PWM: PWM_PCLK = CCLK / 4 = 12MHz and there is no prescale for PWM\n\r"
" So that the PWM timer/counter clock is at 12MHz. The base rate is set to 256\n\r"
" So the base PWM frequency is at 12MHz/256 = 46.875 KHz (Period = ~21.34 microsecond)\n\r"
" Each PWM channel (1 to 6) will be configured as following:\n\r"
" + PWM1.1 = 10% (period = 2.087 microsecond)\n\r"
" + PWM1.2 = 20% (period = 4.250 microsecond)\n\r"
" + PWM1.3 = 40% (period = 8.502 microsecond)\n\r"
" + PWM1.4 = 50% (period = 10.67 microsecond)\n\r"
" + PWM1.5 = 70% (period = 14.92 microsecond)\n\r"
" + PWM1.6 = 90% (period = 19.17 microsecond)\n\r"
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

int c_entry(void)
{
	U8 temp;
	
	mcb2300_init();
	
	if ((UART0_Init()) == _ERROR)
	{
		return 0;
	}

	// Welcome screen
	UART_WritePoll (uartdev, menu1, sizeof (menu1));
	
	/* Set these pins fuction for PWM channel 1, 2, 3, 4, 5,6 */
	for (temp = 0; temp < 6; temp++)
	{
		PINSEL_Set (PORT_2, temp, FUNC_1);
	}
	
	/* After int, PCLK divider is set to 4 for PWM clock input 
	 * from CCLK, it mean PCLK = CCLK/4 = 48MHz/4 = 12MHz */
	
	/* Open the pwm module beginning work with PWM function */
	if ((pwmdev = PWM_Init(0x00)) == 0)
	{
		 return 0;
	}
	
	/* Set the base rate for channel 0 = 256Hz
	 * prescale = 1 and update system immediately after config */
	PWM_Setup (pwmdev, 1, 256);
	PWM_Update (pwmdev);
	
	/* Channel 0 will be reset when match */
	PWM_EnableMatchResetTC (pwmdev, 0);
	
	/* Config each channel:
	 * - Single edge
	 * - Duty in percent mode (0..100%)
	 * - Remaining Offset Option and Duty value not care! */
	PWM_ConfigChannel (pwmdev, 1, PWM_SINGLE_EDGE, 
			PWM_WAV_DUTY_PER, 10, 0, 0);
	PWM_ConfigChannel (pwmdev, 2, PWM_SINGLE_EDGE, 
			PWM_WAV_DUTY_PER, 20, 0, 0);
	PWM_ConfigChannel (pwmdev, 3, PWM_SINGLE_EDGE, 
			PWM_WAV_DUTY_PER, 40, 0, 0);
	PWM_ConfigChannel (pwmdev, 4, PWM_SINGLE_EDGE, 
			PWM_WAV_DUTY_PER, 50, 0, 0);
	PWM_ConfigChannel (pwmdev, 5, PWM_SINGLE_EDGE, 
			PWM_WAV_DUTY_PER, 70, 0, 0);
	PWM_ConfigChannel (pwmdev, 6, PWM_SINGLE_EDGE, 
			PWM_WAV_DUTY_PER, 90, 0, 0);

	// Enable each PWM channel and update their value (1..6)
	for (temp = 1; temp < 7; temp++)
	{
		PWM_UpdateChannel (pwmdev, temp);
		PWM_EnableChannel (pwmdev, temp);
	}
	
	// Print info
	UART_Puts_ (uartdev, "PWM is now running... \n\r");
	
	// Start to run PWM
	PWM_Enable (pwmdev);
	
	// Main loop forever
	while(1);
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}
