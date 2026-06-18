/***********************************************************************
 * ID: adc_poll.c 2008-11-09 HieuNguyen
 *
 * Project: ADC driver example
 *
 * Description:
 * 		Read ADC0.0 value in polling mode and display via UART0 
 * after each 1 second. Turn potentiometer to see ADC value updated
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
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_uart_driver.h"
#include "lpc23xx_vic_driver.h"
#include "lpc23xx_adc_driver.h"
#include "lpc23xx_tc_driver.h"
#include "mcb2300_init.h"


/* ADC, timer and UART device variable */
STATIC S32 adcdev = 0;
STATIC S32 uart0dev = 0;
STATIC S32 tcdev = 0;

CHAR menu0[] = 
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
" ADC demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Use ADC with 10-bit resolution at freq = 1MHz, read in polling mode\n\r"
" To get ADC0 value on channel 0 and display via UART0\n\r"
" Turn the potentiometer to see how ADC value changes\n\r"
"********************************************************************************\n\r";

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

int c_entry(void)
{	
	U32 ADCValue;
	
	mcb2300_init();
	
	// Init UART0
	if ((UART0_Init()) == _ERROR)
	{
		return 0;
	}
	
	// init timer 0
	if ((tcdev = TC_Init(0)) == 0)
	{
		return 0;
	}
	
	// Print welcome screen
	UART_WritePoll (uart0dev, menu0, sizeof(menu0));
	
	// Select pin that used as ADC function
	PINSEL_Set (PORT_0, 23, FUNC_1);
	
	if ((adcdev = ADC_Init()) == 0)
	{
		return 0;
	}
	
	// Configure ADC with 1MHz of freq, 10-bit of resolution
	ADC_Config (adcdev, 1000000, 10);
	
	// Enable channel 0 on ADC0
	ADC_EnableCH (adcdev, 0);
	
	// main loop
	while (1)
	{
		// Start ADC conversion
		ADC_Start (adcdev, ADC_START_NOW, 0);
		
		// Read ADC value on channel 0 of ADC0 in polling mode
		ADCValue = ADC_ReadPoll (adcdev, 0);
		
		// Print out via UART0
		UART_Puts (uart0dev, "ADC value on channel 0: ");
		UART_PutDec (uart0dev, ADCValue); UART_Puts_ (uart0dev, "");
		
		// Delay for a while 
		TC_Waitms (tcdev, 1000);
	}
	
	return 0;
}

int main (void)
{
    return (c_entry());
}
