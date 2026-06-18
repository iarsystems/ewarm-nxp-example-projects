/**
 * @file	: pmu_test.c
 * @purpose	: Simple example to test the power management unit
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
#include "LPC13xx.h"			/* LPC13xx Peripheral Registers */
#include "lpc13xx_gpio.h"
#include "lpc13xx_clkpwr.h"
#include "lpc13xx_timer.h"
#include "lpc13xx_uart.h"
#include "lpc13xx_nvic.h"


#define BOD_PD				(0x1<<3)
#define ADC_PD				(0x1<<4)
#define SYS_OSC_PD			(0x1<<5)
#define WDT_OSC_PD			(0x1<<6)
#define SYS_PLL_PD			(0x1<<7)
#define USB_PLL_PD			(0x1<<8)

/************************** PRIVATE TYPES *************************/

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"PMU demo \n\r"
"\t - MCU: LPC13xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART -  9600bps \n\r"
" IC will go to deep sleep mode and wake up by the P0.1 pin\n\r"
"********************************************************************************\n\r";

uint8_t menu2[] = "Demo terminated! \n\r";
uint8_t message1[] = "IC now going to deep sleep mode    \n\r";
uint8_t message2[] = "IC is woken up by the pin P0.1     \n\r";


//timer init
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct;

/*********************************************************************//**
 * @brief	Print Welcome Screen Menu subroutine
 **********************************************************************/
void print_menu(void)
{
	UART_Send(menu1, sizeof(menu1), BLOCKING);
}

/*********************************************************************//**
 * @brief		Initialize PMU and setup wakeup source.
				For Sleep and deepsleep, any of the I/O pins can be
				used as the wakeup source.
				For Deep Powerdown, only pin P1.4 can be used as
				wakeup source from deep powerdown.
 **********************************************************************/
void PMU_Init( void )
{
  /* Enable all clocks, even those turned off at power up. */
  LPC_SYSCON->PDRUNCFG &= ~(WDT_OSC_PD|SYS_OSC_PD|ADC_PD);

  NVIC_EnableIRQ((IRQn_Type)(WAKEUP1_IRQn));

  /* use port0_1 as wakeup source, i/o pin */
  LPC_IOCON->PIO0_1 &= ~0x07;
  LPC_IOCON->PIO0_1 |= 0x20;
  GPIO_SetDir( PORT0, 1, 0 );	/* Input P0.1 */
  /* Only edge trigger. activation polarity on P0.1 is FALLING EDGE. */
  LPC_SYSCON->STARTAPRP0 = 0x00000000;
  /* Clear all wakeup source */
  LPC_SYSCON->STARTRSRP0CLR = 0xFFFFFFFF;
  LPC_SYSCON->STARTRSRP1CLR = 0xFFFFFFFF;
  /* Enable Port 0.1 as wakeup source. */
  LPC_SYSCON->STARTERP0 = 0x1<<1;
  return;
}


/*****************************************************************************
 * @brief		WAKEUP Interrupt Handler
*****************************************************************************/
void WAKEUP_IRQHandler(void)
{
  uint32_t regVal;

  /* This handler takes care all the port pins if they
  are configured as wakeup source. */
  regVal = LPC_SYSCON->STARTSRP0;
  if ( regVal != 0 )
  {
	LPC_SYSCON->STARTRSRP0CLR = regVal;
  }
  regVal = LPC_SYSCON->STARTSRP1;
  if ( regVal != 0 )
  {
	LPC_SYSCON->STARTRSRP1CLR = regVal;
  }
  /* See tracker for bug report. */
  __NOP();
  return;
}


/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  uint32_t regVal;

  SystemInit();
  NVIC_DeInit();
  NVIC_SCBDeInit();

  /* Enable AHB clock to the GPIO domain. */
  GPIO_Init();

  /* Setup wakeup source before sleep or deep sleep. Current,
  port0.1 is configured as wakeup source, falling edge trigger. */
  PMU_Init();
  UART_Init();
  	// print welcome screen
  	print_menu();
  /* Put X/Y/Z into sleep mode. */
  UART_Send(message1, sizeof(message1), BLOCKING);
  /*wait Uart*/
  for(int i = 0; i < 1000000; i++);
  regVal = USB_PLL_PD|SYS_PLL_PD|WDT_OSC_PD|ADC_PD|BOD_PD;
  PMU_Sleep( 1, regVal );
  /* Set port 3_0 to output as LED control */
  GPIO_SetDir( 3, 0, 1 );
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
	UART_Send(message2, sizeof(message2), BLOCKING);

  while (1)                                /* Loop forever */
  {
	  // Wait for 1000 millisecond
	  		while ( !(TIM_GetIntStatus(LPC_TMR32B0,0)));

	  		{
	  		TIM_ClearIntPending(LPC_TMR32B0,0);
	  		GPIO_SetValue(PORT3, 0);
	  		}

	  		// Wait for 1000 millisecond
	  		while ( !(TIM_GetIntStatus(LPC_TMR32B0,0)));
	  		{
	  		TIM_ClearIntPending(LPC_TMR32B0,0);
	  		GPIO_ClearValue(PORT3, 0);
	  		}
  }
}

/* --------------------------------- End Of File ------------------------------ */
