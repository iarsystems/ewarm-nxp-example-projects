/**********************************************************************
* $Id: Gpio_ExtInt.c 28 2014-01-27 14:45:13Z danielru $		Gpio_ExtInt.c	2012-02-17
*//**
* @file		Gpio_ExtInt.c
* @brief	This example describes how to use GPIO as source of an
* 			external interrupt
* @version	1.0
* @date		17. Feb. 2012
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2012, NXP Semiconductor
* All rights reserved.
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include "lpc18xx_libcfg.h"
#include "lpc18xx_gpio.h"
#include "lpc18xx_scu.h"
#include "debug_frmwrk.h"
#include "lpc18xx_cgu.h"


/* Example group ----------------------------------------------------------- */
/** @defgroup Gpio_ExtInt	Gpio_ExtInt
 * @ingroup GPIO_Examples
 * @{
 */


/************************** PRIVATE DEFINITIONS *************************/
#define INT_LED_BIT				1  //LEDUSB0
#define INT_LED_PORT			4
#define POLL_LED_BIT			15 //LEDUSB1
#define POLL_LED_PORT			4

#define PIN_INT_PORT			7
#define PIN_INT_BIT				23

#define PIN_INT_NUM				0
#define INT_ID					PIN_INT0_IRQn


/************************** PRIVATE VARIABLES *************************/
uint8_t intMode=0;
uint32_t msec;
uint8_t menu[]=
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"GPIO External Interrupt demo \n\r"
	"\t - MCU: LPC18xx \n\r"
	"\t - Core: ARM CORTEX-M3 \n\r"
	"\t - Communicate via: UART0 - 115200 bps \n\r"
	"This example used to configure the\n\r "
	"GPIO as source of external Interrupt \n\r"
	"********************************************************************************\n\r";
uint8_t mode[]=
	"\t - 0: Rising Edge Interrupt \n\r"
	"\t - 1: Falling Edge Interrupt \n\r"
	"\t - 2: Active-HIGH Level Interrupt \n\r"
	"\t - 3: Active-LOW Level Interrupt \n\r";

/************************** PRIVATE FUNCTIONS *************************/
void print_menu(void);
void print_mode(void);
void GPIO0_IRQHandler(void);
void delay (void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		GPIO interrupt 0 handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void GPIO0_IRQHandler(void)
{
	int j;
	if(GPIO_GetIntStatus(PIN_INT_NUM))
	{
		GPIO_ClearInt(intMode, PIN_INT_NUM);
		for (j= 0; j<5; j++)
		{
		/*blink LED P8.1 when GPIO0 INT occur*/
			GPIO_SetValue(INT_LED_PORT,(1<<INT_LED_BIT));
			delay();
			GPIO_ClearValue(INT_LED_PORT,(1<<INT_LED_BIT));
			delay();
		}
	}
}

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print Welcome menu
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG(menu);
}

/*********************************************************************//**
 * @brief		Print Interrupt Mode Description
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void print_mode(void)
{
	_DBG(mode);
}

/*********************************************************************//**
 * @brief		Delay function
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void delay (void) {
  unsigned int i;

  for (i = 0; i < 0x100000; i++) {
  }
}
/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
uint32_t tempx, tempy;

int c_entry (void) {                       /* Main Program                       */

	uint8_t flag;
	SystemInit();
	CGU_Init();

	/* Initialize debug via UART0
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	print_menu();

	/* Configure pin function */
	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 					// P8.1 : USB0_IND1 LED (INT_LED)
	scu_pinmux(0x9 ,3 , MD_PDN, FUNC0); 					// P9.3 : USB1_IND1 LED (POL_LED)
	scu_pinmux(0xF ,9 , (MD_PLN|MD_EZI|MD_ZI), FUNC4);		//Potentiometer - R26

	/* Set direction for GPIO port */
	GPIO_SetDir(INT_LED_PORT,(1<<INT_LED_BIT), 1); 			//USB0_IND1 LED (INT_LED)
	GPIO_SetDir(POLL_LED_PORT,(1<<POLL_LED_BIT), 1);		//USB1_IND1 LED (POL_LED)
	GPIO_SetDir(PIN_INT_PORT,(1<<PIN_INT_BIT), 0);			//Potentiometer - R26

	_DBG("\n\rPlease choose one of the following interrupt modes: \n\r");
	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(INT_ID, ((0x01<<3)|0x01));

	print_mode();

	/* Get the interrupt mode */
	flag = 1;
	while (flag)
	{
		intMode = _DG;
		switch (intMode)
		{
			case '0':
			case '1':
			case '2':
			case '3':
				_DBC(intMode);
				flag = 0;
				break;
			default:
				_DBG("\n\r...Please input digit from 0 to 3 only! \n\r");
				break;
		}
	}
	intMode = intMode - 0x30;
	/* Configure GPIO interrupt */
	GPIO_IntCmd(PIN_INT_NUM,PIN_INT_PORT,PIN_INT_BIT,intMode); //GPIO7[23] : PF_9
	
	_DBG("\n\rPlease turn the potentiometer to generate an external interrupt... ");
	/* Enable interrupt for Pin Interrupt */
	NVIC_EnableIRQ(INT_ID);
	
	while (1)
	{                           					// Loop forever
		/* Blink LED P9.3 */
		delay();
		GPIO_ClearValue(POLL_LED_PORT,(1<<POLL_LED_BIT));
		delay();
		GPIO_SetValue(POLL_LED_PORT,(1<<POLL_LED_BIT));
	}
}


/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
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

/**
 * @}
 */
