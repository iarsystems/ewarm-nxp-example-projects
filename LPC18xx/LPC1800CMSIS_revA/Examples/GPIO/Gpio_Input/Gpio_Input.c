/**********************************************************************
* $Id: Gpio_Input.c 28 2014-01-27 14:45:13Z danielru $		Gpio_Input.c	2012-02-17
*//**
* @file		Gpio_Input.c
* @brief	This example describes how to configure GPIO to read the
* 			state of a particular pin
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
#include "lpc18xx_gpio.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_libcfg.h"
#include "debug_frmwrk.h"
#include "lpc18xx_cgu.h"



/* Example group ----------------------------------------------------------- */
/** @defgroup Gpio_Input	Gpio_Input
 * @ingroup GPIO_Examples
 * @{
 */


/************************** PRIVATE DEFINITIONS *************************/
#define LED1_BIT			1 //LEDUSB
#define LED1_PORT			4
#define DIR_IN				0 //Direction out
#define DIR_OUT				1 //Direction in

/************************** PRIVATE VARIABLES *************************/
uint32_t msec;
uint8_t menu[]=
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"GPIO External Interrupt demo \n\r"
	"\t - MCU: LPC18xx \n\r"
	"\t - Core: ARM CORTEX-M3 \n\r"
	"\t - Communicate via: UART0 - 115200 bps \n\r"
	"This example used to configure the\n\r "
	"GPIO to read the state of a particular pin \n\r"
	"********************************************************************************\n\r";

/************************** PRIVATE FUNCTIONS *************************/
void print_menu(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/

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
/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
uint32_t tempx, tempy;

int c_entry (void) {                       /* Main Program                       */

	uint8_t flag;
	uint8_t port, pin, gpio_port, gpio_bit, function, tmp;
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
	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 	// P8.1 : USB1_IND1 LED

	/* Set direction for P8.1 */
	GPIO_SetDir(LED1_PORT,(1<<LED1_BIT), DIR_OUT);

	/* Get some inputs */
	_DBG("\n\rPlease enter PORT number (from 0 to F) \n\r");
    port = 0;
	flag = 1;
	while (flag)
	{
		port = _DG;
		switch (port)
		{
			case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
				_DBC(port);
				port = port - 0x30;
				flag = 0;
				break;
			case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':
				_DBC(port);
				port = port - 0x37;
				flag = 0;
				break;
			default:
				_DBG("\n\r...Please input character from 0 to 9 and from A to F only! \n\r");
				break;
		}
	}
	flag = 0;
	tmp = 0;
    pin = 0;
	while(flag < 2)
	{
		if(flag==0)
		_DBG("\n\rPlease enter PIN number (from 00 to 20):\n\r");
		tmp = _DG;
		switch(tmp)
		{
		case '0':case'1':case '2':case '3':case '4':case '5':case '6':case '7':case'8':case '9':
			tmp = tmp - 0x30;
			flag++;
			if(flag==1) tmp = tmp * 10;
			pin = pin + tmp;
			if(flag==2)
				_DBD(pin);
			tmp = 0;
			break;
		default:
			_DBG("\n\r...Please input digits from 0 to 9 only! \n\r");
			flag = 0; pin = 0; tmp = 0;
			break;
		}
	}
	_DBG("\n\rPlease enter FUNCTION number (from 0 to 7) \n\r");
    function = 0;
	flag = 1;
	while (flag)
	{
		function = _DG;
		switch (function)
		{
			case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':
				_DBC(function);
				function = function - 0x30;
				flag = 0;
				break;
			default:
				_DBG("\n\r...Please input character from 0 to 7 only! \n\r");
				break;
		}
	}
	/* Configure pin function */
	scu_pinmux(port ,pin , (MD_PLN|MD_EZI|MD_ZI), function);
        
	_DBG("\n\rPlease enter GPIO port number (from 0 to 7) \n\r");
    gpio_port = 0;
	flag = 1;
	while (flag)
	{
		gpio_port = _DG;
		switch (gpio_port)
		{
			case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':
				_DBC(gpio_port);
				gpio_port = gpio_port - 0x30;
				flag = 0;
				break;
			default:
				_DBG("\n\r...Please input digit from 0 to 7 only! \n\r");
				break;
		}
	}
    gpio_bit = 0;
	flag = 0;
	tmp = 0;
	while(flag < 2)
	{
		if(flag==0)
		_DBG("\n\rPlease enter GPIO bit number (from 00 to 30):\n\r");
		tmp = _DG;
		switch(tmp)
		{
		case '0':case'1':case '2':case '3':case '4':case '5':case '6':case '7':case'8':case '9':
			tmp = tmp - 0x30;
			flag++;
			if(flag==1) tmp = tmp * 10;
			gpio_bit = gpio_bit + tmp;
			if(flag==2) _DBD(gpio_bit);
			tmp = 0;
			break;
		default:
			_DBG("\n\r...Please input digits from 0 to 9 only! \n\r");
			flag = 0; gpio_bit = 0; tmp = 0;
			break;
		}
	}

	/* Set direction as input for reading status of a particular pin */
	GPIO_SetDir(gpio_port,(1<<gpio_bit), DIR_IN);

	while (1)
	{                           					// Loop forever
		/* Turn LED P8.1 according to state of particular pin */
		if ((GPIO_ReadValue(gpio_port) & (1<<gpio_bit)))
			GPIO_ClearValue(LED1_PORT,(1<<LED1_BIT));
		else GPIO_SetValue(LED1_PORT,(1<<LED1_BIT));
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
