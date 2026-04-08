/**
 * @file	: gpio_interrupt_test.c
 * @purpose	: A simple driver to test GPIO interrupt
 * @version	: 1.0
 * @date	: 15. Dec. 2009
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
#include "lpc13xx_uart.h"
#include "lpc13xx_nvic.h"


/************************** PRIVATE TYPES *************************/


/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"GPIO interrupt demo \n\r"
"\t - MCU: LPC13xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART -  9600bps \n\r"
" Use GPIO as a external interrupt source \n\r"
" Software triggers the GPIO interrupt on P0.1, P1.1, P2.1 and P3.1 \n\r"
"********************************************************************************\n\r";

uint8_t message0[] = "P0.1 interrupt \n\r";
uint8_t message1[] = "P1.1 interrupt \n\r";
uint8_t message2[] = "P2.1 interrupt \n\r";
uint8_t message3[] = "P3.1 interrupt \n\r";

uint32_t intPort = 4;


/*********************************************************************//**
 * @brief	Print Welcome Screen Menu subroutine
 **********************************************************************/
void print_menu(void)
{
	UART_Send(menu1, sizeof(menu1), BLOCKING);
}

/*****************************************************************************
 * @brief		Use one GPIO pin(port0 pin1) as interrupt source
*****************************************************************************/
void PIOINT0_IRQHandler(void)
{
  uint32_t regVal;

  regVal = GPIO_IntStatus( PORT0, 1 );
  if ( regVal )
  {
	GPIO_IntClear( PORT0, 1 );
	intPort = 0;
  }
  return;
}

/*****************************************************************************
 * @brief		Use one GPIO pin(port1 pin1) as interrupt source
*****************************************************************************/
void PIOINT1_IRQHandler(void)
{
  uint32_t regVal;

  regVal = GPIO_IntStatus( PORT1, 1 );
  if ( regVal )
  {
	GPIO_IntClear( PORT1, 1 );
	intPort = 1;
  }
  return;
}

/*****************************************************************************
 * @brief		Use one GPIO pin(port2 pin1) as interrupt source
*****************************************************************************/
void PIOINT2_IRQHandler(void)
{
  uint32_t regVal;

  regVal = GPIO_IntStatus( PORT2, 1 );
  if ( regVal )
  {
	GPIO_IntClear( PORT2, 1 );
	intPort = 2;
  }
  return;
}

/*****************************************************************************
 * @brief		Use one GPIO pin(port0 pin1) as interrupt source
*****************************************************************************/
void PIOINT3_IRQHandler(void)
{
  uint32_t regVal;

  regVal = GPIO_IntStatus( PORT3, 1 );
  if ( regVal )
  {
	GPIO_IntClear( PORT3, 1 );
	intPort = 3;
  }
  return;
}


/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  SystemInit();

  GPIO_Init();
  UART_Init();
  	// print welcome screen
  	print_menu();

  	NVIC_EnableIRQ(EINT0_IRQn);
  	NVIC_EnableIRQ(EINT1_IRQn);
  	NVIC_EnableIRQ(EINT2_IRQn);
  	NVIC_EnableIRQ(EINT3_IRQn);


  /* use port0_1 as input event, interrupt test. */
  GPIO_SetDir( PORT0, 1, 0 );
  /* port0_1, single trigger, active high. */
  GPIO_SetInterrupt( PORT0, 1, 0, 0, 0 );
  GPIO_IntEnable( PORT0, 1 );

  /* use port1_1 as input event, interrupt test. */
  GPIO_SetDir( PORT1, 1, 0 );
  /* port0_1, single edge trigger, active high. */
  GPIO_SetInterrupt( PORT1, 1, 0, 0, 0 );
  GPIO_IntEnable( PORT1, 1 );

  /* use port2_1 as input event, interrupt test. */
  GPIO_SetDir( PORT2, 1, 0 );
  /* port0_1, single edge trigger, active high. */
  GPIO_SetInterrupt( PORT2, 1, 0, 0, 0 );
  GPIO_IntEnable( PORT2, 1 );

  /* use port3_1 as input event, interrupt test. */
  GPIO_SetDir( PORT3, 1, 0 );
  /* port0_1, single edge trigger, active high. */
  GPIO_SetInterrupt( PORT3, 1, 0, 0, 0 );
  GPIO_IntEnable( PORT3, 1 );

  while( 1 )
  {
	  switch (intPort){
		 case 0:
			 UART_Send(message0, sizeof(message0), BLOCKING);
			 intPort = 0xFF;
			 break;
		 case 1:
			 UART_Send(message1, sizeof(message1), BLOCKING);
			 intPort = 0xFF;
		 	break;
		 case 2:
			 UART_Send(message2, sizeof(message2), BLOCKING);
			 intPort = 0xFF;
			 break;
		 case 3:
			 UART_Send(message3, sizeof(message3), BLOCKING);
			 intPort = 0xFF;
			 break;
		 default:
			 intPort = 0xFF;
			 break;

	  }
  }
}

/* --------------------------------- End Of File ------------------------------ */
