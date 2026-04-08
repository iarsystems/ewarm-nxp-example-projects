/**
 * @file	: adc_polling_test.c
 * @purpose	: A simple adc driver example, used to test AD6
 * @version	: 1.0
 * @date	:06. Nov. 2009
 * @author	: ThieuTrinh
 *----------------------------------------------------------------------------
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
#include "lpc13xx_adc.h"
#include "lpc13xx_pinsel.h"
#include "lpc13xx_timer.h"
#include "debug_frmwrk.h"


/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
" ADC demo \n\r"
"\t - MCU: LPC13xx \n\r"
"\t - ARM COTEX \n\r"
"\t - Communicate via: UART0 - 9600 bps \n\r"
" Use ADC with 10-bit resolution at freq = 1MHz, read in polling mode\n\r"
" To get ADC value on channel 6 and display via UART\n\r"
" Turn the potentiometer to see how ADC value changes\n\r"
"********************************************************************************\n\r";
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
/************************** PRIVATE FUNCTION *************************/
void print_menu(void);
void Timer_Wait(uint32_t time);

/**
 * @brief Print menu
 */
void print_menu(void)
{
	_DBG(menu1);
}
volatile unsigned long SysTickCnt;      /* SysTick Counter  */


void SysTick_Handler (void) {           /* SysTick Interrupt Handler (1ms)    */
  SysTickCnt++;
}

void Delay (unsigned long tick) {       /* Delay Function                     */
  unsigned long systickcnt;

  systickcnt = SysTickCnt;
  while ((SysTickCnt - systickcnt) < tick);
}


/**
 * @brief Main Program body
 */
int c_entry(void)
{
	uint32_t adc_value;

	// Initialize System clock
	SystemInit();
	SysTick_Config(SystemCoreClock/1000 - 1);

// Initialize timer 0, prescale count time of 1ms
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1000;
	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will not reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = FALSE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = TRUE;
	//do no thing for external output
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;
	// Set Match value, count value is time (timer * 1000uS =timer mS )
	TIM_MatchConfigStruct.MatchValue   = 1000;


	// Init timer
	TIM_Init(LPC_TMR16B0, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TMR16B0,&TIM_MatchConfigStruct);
	NVIC_EnableIRQ(TIMER_16_0_IRQn);

	// Set pin connect block and pin mode AD7 on PIO1_11
	//LPC_IOCON->PIO1_11 = PINSEL_PIO_FUNC(1);
	// Set pin connect block and pin mode AD6 on PIO1_10
		LPC_IOCON->PIO1_10 = PINSEL_PIO_FUNC(1);

	SystemInit();

	debug_frmwrk_init();
	// print welcome screen
	print_menu();
	/* Configuration for ADC :
	 * 	Frequency at 1Mhz, 10 bit of resolution
	 *  ADC channel 6, no Interrupt
	 */
	ADC_Init(1000000, 10);
	ADC_IntConfig(ADC_CHANNEL_6,DISABLE);
	ADC_ChannelCmd(ADC_CHANNEL_6,ENABLE);

	while(1)
	{
		// Start conversion
		ADC_StartCmd(ADC_START_NOW);
		//Wait conversion complete
		while (!(ADC_ChannelGetStatus(ADC_CHANNEL_6,ADC_DATA_DONE)));
		adc_value = ADC_ChannelGetData(ADC_CHANNEL_6);
		_DBG("ADC value on channel 6: ");
		_DBD32(adc_value);
		_DBG_("");
		Delay(1000);

	}

	return 0;
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}

