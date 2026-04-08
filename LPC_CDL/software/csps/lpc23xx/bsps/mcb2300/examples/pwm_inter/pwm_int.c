/***********************************************************************
 * ID: pwm_int.c 2008-10-31 HieuNguyen
 *
 * Project: PWM driver example
 *
 * Description:
 * This program illustrates the PWM signal on 6 Channel in single edge mode
 * Peripheral clock for PWM: PWM_PCLK = CCLK / 4 = 12MHz and there is no prescale for PWM
 * So that the PWM timer/counter clock is at 12MHz. The base rate is set to 256
 * So the base PWM frequency is at 12MHz/256 = 46.875 KHz (Period = ~21.34 microsecond)
 * Each PWM channel (1 to 6) will be configured as following:
 * + PWM1.2 = 20% (period = 4.250 microsecond)
 * + PWM1.3 = 40% (period = 8.502 microsecond)
 * + PWM1.4 = 50% (period = 10.67 microsecond)
 * + PWM1.5 = 70% (period = 14.92 microsecond)
 * + PWM1.6 = 90% (period = 19.17 microsecond)
 * Here, PWM1.1 value is not stable, it will increase by the time from 0 to 100% period
 * and restart. Match interrupt for channel 0 is set, when timer of PWM reach to 256
 * (value of channel 0 match), an interrupt for matching will generate and update the
 * value of PWM1.1, this value will be updated after each:
 * Period * 4096 = 21.34 * 4096 = 87,408 (microsecond)
 * And this value will be reset to 0 after:
 * Period * 4096 * 100 = 8,740,864 (microsecond) = ~8.740 (second)
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
#include "lpc23xx_vic_driver.h"
#include "mcb2300_init.h"

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
" + PWM1.2 = 20% (period = 4.250 microsecond)\n\r"
" + PWM1.3 = 40% (period = 8.502 microsecond)\n\r"
" + PWM1.4 = 50% (period = 10.67 microsecond)\n\r"
" + PWM1.5 = 70% (period = 14.92 microsecond)\n\r"
" + PWM1.6 = 90% (period = 19.17 microsecond)\n\r"
" Here, PWM1.1 value is not stable, it will increase by the time from 0 to 100% period\n\r"
" and restart. Match interrupt for channel 0 is set, when timer of PWM reach to 256\n\r"
" (value of channel 0 match), an interrupt for matching will generate and update the\n\r"
" value of PWM1.1, this value will be updated after each:\n\r"
" Period * 4096 = 21.34 * 4096 = 87,408 (microsecond)\n\r"
" And this value will be reset to 0 after:\n\r"
" Period * 4096 * 100 = 8,740,864 (microsecond) = ~8.740 (second)\n\r"
"********************************************************************************\n\r";
STATIC S32 uartdev = 0;
STATIC S32 pwmdev = 0;
U32 volatile match_cnt = 0;
U32 volatile long_duty = 0;

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

/* PWM Interrupt handler */
void PWM_ISR (void) 
{
    U32 regval;
    
    /* Read the value in interrupt register to know which channel 
     * interrupt and later will write this value to clear interrtup */
    regval = PWM_ReadMatchIntPending (pwmdev);
    
    if ( regval & PWM_INT_ON_MATCH(0))
    {
    	match_cnt++;	
    	if (match_cnt >= 0x1000)
    	{
    		match_cnt = 0;
    		long_duty++;
    		if (long_duty >= 101)
    		{
    			// Reset duty
    			long_duty = 0;
        		// Print info
        		UART_Puts_ (uartdev, "PWM1.1 is reset!");
    		}
    		
    		// Update PWM1.1 duty
    		PWM_ConfigChannel (pwmdev, 1, PWM_SINGLE_EDGE, 
    				PWM_WAV_DUTY_PER, long_duty, 0, 0);
    		PWM_UpdateChannel(pwmdev, 1);
    	}
    }
    
    /* Clear the interrtup flag */
    PWM_ClrMatchIntPending (pwmdev, regval);
    
    /* Ack and update VIC Vector Addr */
    VIC_Ack ();
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
	
	// Install IRQ interrupt handler for PWM
	if ((VIC_InstallIRQ (PWM1_INT, (void *) PWM_ISR, 0x01)) == _ERROR)
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
	
	/* Enable match 0 interrupt on pwm */
	PWM_EnableMatchInt (pwmdev, 0);
	
	/* And now, enable PWM match 0 interrupt */
	VIC_EnableInt (PWM1_INT);
	
	// Main loop forever
	while(1);
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}
