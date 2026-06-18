/***********************************************************************
 * ID: dac_ex.c 2008-11-09 HieuNguyen
 *
 * Project: DAC driver example
 *
 * Description:
 *     This example is to test DAC peripheral through speaker on MCB2300 board. 
 * The DAC peripheral will generate the sin tone, in which will increase 
 * step by step to max and reduce to 0 and continue to increase to max again 
 * in the same frequency continuously. We are able to hear the tone from the 
 * speaker for many cycles.
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
#include "lpc23xx_uart_driver.h"
#include "lpc23xx_dac_driver.h"
#include "lpc23xx_tc_driver.h"
#include "mcb2300_init.h"


/* ADC, timer and UART device variable */
STATIC S32 dacdev = 0;
STATIC S32 uart0dev = 0;
STATIC S32 tcdev = 0;

CHAR menu0[] = 
"********************************************************************************\n\r" 
"Hello NXP Semiconductors \n\r"
" DAC demo \n\r"
"\t - MCU: LPC2368 \n\r"
"\t - Core: ARM7TDMI-S \n\r"
"\t - Comunicate via: UART0 - 115.2 kbps \n\r"
" Write new DAC value that encreased by the time and output to speaker \n\r"
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
	U32 time;
	float rate;
	
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
	
	// Select pin that used as DAC function
	PINSEL_Set (PORT_0, 26, FUNC_2);
	
	if ((dacdev = DAC_Init()) == 0)
	{
		return 0;
	}
	
	// Config DAC with DAC_MAX_CURRENT_700uA
	DAC_Config (dacdev, DAC_MAX_CURRENT_700uA);
	
	// Reset times
	time = 0;
	
	// Print out infor...
	UART_Puts_ (uart0dev, "Starting DAC demo...");
	
	// main loop
	while (1)
	{
		for(time = 1; time < 0x3FF; time++)
		{
			DAC_Update (dacdev, (U32)(time*rate));
		}
		
		rate += 0.001;
		
		if(rate >= 1)
		{
			rate = 0;
		}
	}
	
	return 0;
}

int main (void)
{
    return (c_entry());
}
