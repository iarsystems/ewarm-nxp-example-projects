/**
 * @file	: timer_capture.c
 * @purpose	: A simple driver to test TIMER capture function
 * @version	: 1.0
 * @date	: 27. Nov. 2009
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
#include "lpc13xx_timer.h"
#include "lpc13xx_uart.h"
#include "lpc13xx_libcfg.h"
#include "lpc13xx_nvic.h"
#include "debug_frmwrk.h"
#include "lpc13xx_gpio.h"
#include "lpc13xx_pinsel.h"



#define MAX_MATCH	1000000
#define START_MATCH	10000


/************************** PRIVATE TYPES *************************/

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"Timer Match interrupt demo \n\r"
"\t - MCU: LPC13xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART -  9600 bps \n\r"
" Use timer16 0 to take a snapshot of the timer value when an input signal \n\r"
" on CAP0 transitions \n\r"
"********************************************************************************\n\r";

// UART Configuration structure variable
UART_CFG_Type UARTConfigStruct;
uint32_t MatchCount;

//timer init
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
TIM_CAPTURECFG_Type TIM_CaptureConfigStruct;
uint8_t volatile timer0_flag = FALSE, timer1_flag = FALSE;
/************************** PRIVATE FUNCTION *************************/
void print_menu(void);
void TIMER16_0_IRQHandler(void);
void TIMER16_1_IRQHandler(void);
// Print Menu 1
void print_menu(void)
{
	_DBG(menu1);
}

void TIMER16_0_IRQHandler(void)
{
	static uint32_t ledstate = 0;
	if (TIM_GetIntCaptureStatus(LPC_TMR16B0,0))
	{
		TIM_ClearIntCapturePending(LPC_TMR16B0,0);
		_DBG("Time capture: ");
		_DBH32(TIM_GetCaptureValue(LPC_TMR16B0));_DBG_("");
		if(ledstate == 0){
			ledstate = 1;
			GPIO_SetValue(PORT3, 0);
		}else{
			ledstate = 0;
			GPIO_ClearValue(PORT3, 0);
		}
	}
}

int c_entry(void)
{
	SystemInit();

	// DeInit NVIC and SCBNVIC
	NVIC_DeInit();
	NVIC_SCBDeInit();

	/* Configure the NVIC Preemption Priority Bits:
	 * two (2) bits of preemption priority, six (6) bits of sub-priority.
	 * Since the Number of Bits used for Priority Levels is five (5), so the
	 * actual bit number of sub-priority is three (3)
	 */
	NVIC_SetPriorityGrouping(0x05);

	//  Set Vector table offset value
#if (__RAM_MODE__==1)
	NVIC_SetVTOR(0x10000000);
#else
	NVIC_SetVTOR(0x00000000);
#endif

	/* Init debug */
	debug_frmwrk_init();

	// print welcome screen
	print_menu();
	GPIO_SetDir( PORT3, 0, 1 ); //Green LED
	PINSEL_SetPinFunc(0,2,2); /* Timer0_16 CAP0 */

	// Initialize timer 0, prescale count time of 1000000uS = 1S
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1000000;

	// use channel 0, CT16B0_CAP0
	TIM_CaptureConfigStruct.CaptureChannel = 0;
	// Enable capture on CAPn.0 rising edge
	TIM_CaptureConfigStruct.RisingEdge = ENABLE;
	// Enable capture on CAPn.0 falling edge
	TIM_CaptureConfigStruct.FallingEdge = ENABLE;
	// Generate capture interrupt
	TIM_CaptureConfigStruct.IntOnCaption = ENABLE;


	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TMR16B0, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigCapture(LPC_TMR16B0, &TIM_CaptureConfigStruct);
	TIM_ResetCounter(LPC_TMR16B0);


	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER_16_0_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER_16_0_IRQn);
	// To start timer 0
	TIM_Cmd(LPC_TMR16B0,ENABLE);

	while (1);
	return 1;
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}
#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

