/**
 * @file	: timer_interrupt_test.c
 * @purpose	: A simple timer driver example uses timer16 0 in interrupt mode
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
#include "lpc13xx_gpio.h"
#include "lpc13xx_pinsel.h"
#include "lpc13xx_nvic.h"
#include "debug_frmwrk.h"


/************************** PRIVATE TYPES *************************/

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"Timer Match interrupt demo \n\r"
"\t - MCU: LPC13xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART0 -  9600 bps \n\r"
" Use timer 0 to turn on/off P2.0 and toggle MAT0.0, pin P1.28 at frequency 10Hz\n\r"
"********************************************************************************\n\r";
uint8_t	info1[] ="Timer 0 Tick ON \n\r";
uint8_t info2[] ="Timer 0 Tick OFF \n\r";
// UART Configuration structure variable
UART_CFG_Type UARTConfigStruct;

//timer init
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
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
	if (TIM_GetIntStatus(LPC_TMR16B0,0))
	{
		TIM_ClearIntPending(LPC_TMR16B0,0);
		/* toggle led status */
		if (timer0_flag)
		{
			GPIO_SetValue(PORT3, 0);
			UART_Send(info1, sizeof(info1), BLOCKING);
		}

		else
		{
			GPIO_ClearValue(PORT3, 0);
			UART_Send(info2, sizeof(info1), BLOCKING);
		}

		timer0_flag =! timer0_flag;
	}
}


int c_entry(void)
{
	// Initialize System clock
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

	GPIO_SetDir( PORT3, 0, 1 );
	LPC_IOCON->PIO0_8 = PINSEL_PIO_FUNC(2);

	// Initialize timer 0, prescale count time of 100uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 100;
	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR1 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//Toggle MR1 pin if MR1 matches it
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
	// Set Match value, count value of 1000 (1000 * 100uS = 100mS --> 10 Hz)
	TIM_MatchConfigStruct.MatchValue   = 1000;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TMR16B0, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TMR16B0,&TIM_MatchConfigStruct);

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER_16_0_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 1 */
	NVIC_EnableIRQ(TIMER_16_0_IRQn);
	// To start timer 1
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

