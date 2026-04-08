/**
 * @file	: LedBlinky.c
 * @purpose	: A simple timer driver example that configures and toggles port pins with a timer
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
#include "lpc13xx_gpio.h"


/************************** PRIVATE TYPES *************************/

/************************** PRIVATE VARIABLES *************************/


//timer init
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct;
/************************** PRIVATE FUNCTION *************************/


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

	// print welcome screen
	GPIO_SetDir( PORT3, 0, 1 ); //Green LED
	GPIO_SetDir( PORT1, 9, 1 ); //Red LED

	// Initialize timer 0, prescale count time of 100uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 100;
	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Disable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//Toggle MR0.0 pin if MR0 matches it
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
	// Set Match value, count value of 10000 (10000 * 100uS = 1S --> 1Hz)
	TIM_MatchConfigStruct.MatchValue   = 10000;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TMR32B0, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TMR32B0,&TIM_MatchConfigStruct);
	TIM_Cmd(LPC_TMR32B0,ENABLE);
	while (1)
	{

		// Wait for 1000 millisecond
		while ( !(TIM_GetIntStatus(LPC_TMR32B0,0)));

		{
		TIM_ClearIntPending(LPC_TMR32B0,0);
		GPIO_SetValue(PORT3, 0);
		GPIO_ClearValue(PORT1, 9);
		}

		// Wait for 1000 millisecond
		while ( !(TIM_GetIntStatus(LPC_TMR32B0,0)));
		{
		TIM_ClearIntPending(LPC_TMR32B0,0);
		GPIO_ClearValue(PORT3, 0);
		GPIO_SetValue(PORT1, 9);
		}
	}
	TIM_DeInit(LPC_TMR32B0);
	return (1);
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

