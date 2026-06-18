/**********************************************************************
* $Id: atimer_wic.c 28 2014-01-27 14:45:13Z danielru $		Atimer_Wic.c		2011-06-02
*//**
* @file		Atimer_Wic.c
* @brief	This example describes how to use ATimer to generate Interrupt
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
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
**********************************************************************/
#include "lpc18xx_atimer.h"
#include "lpc18xx_cgu.h"
#include "lpc18xx_evrt.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_libcfg.h"
#include "debug_frmwrk.h"
#include "lpc18xx_gpio.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Atimer_Wic	Atimer_Wic
 * @ingroup ATIMER_Examples
 * @{
 */


/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"Timer delay demo \n\r"
"\t - MCU: LPC18xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART1 - 115200 bps \n\r"
" Using Alarm Timer to generate Interrupt and wake up system\n\r"
"********************************************************************************\n\r";
uint32_t count = 0;

/************************** PRIVATE FUNCTIONS *************************/
void print_menu(void);
void EVRT_IRQHandler(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		ATIMER interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EVRT_IRQHandler(void)
{
	/* Check if source interrupt is ATIMER */
	if(EVRT_IsSourceInterrupting(LPC_EVENTROUTER, EVRT_SRC_ATIMER))
	{
		/* clear ATIMER interrupt status */
		ATIMER_ClearIntStatus(LPC_ATIMER);
		/*Clear Alarm Timer interrupt flag */
		EVRT_ClrPendIntSrc(LPC_EVENTROUTER, EVRT_SRC_ATIMER);

		if(count & 1)
			FIO_ByteSetValue(4, 0, 0xFF);
		else
			FIO_ByteClearValue(4, 0, 0xFF);

		count++;
	}
}

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print menu
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG(menu1);
}


/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main ATIMER program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	SystemInit();
	CGU_Init();
	/* Initialize debug via UART1
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	// print welcome screen
	print_menu();

	/* Init LED Pins for output */
	scu_pinmux(0x8 ,7 , MD_PDN, FUNC0); 	// P8.7 : GPIO4_7: LED1
	scu_pinmux(0x8 ,6 , MD_PDN, FUNC0); 	// P8.6 : GPIO4_6: LED2
	scu_pinmux(0x8 ,5 , MD_PDN, FUNC0); 	// P8.5 : GPIO4_5: LED3
	scu_pinmux(0x8 ,4 , MD_PDN, FUNC0); 	// P8.4 : GPIO4_4: LED4
	scu_pinmux(0x8 ,3,  MD_PDN, FUNC0); 	// P8.3 : GPIO4_3: LED5
	scu_pinmux(0x8 ,2 , MD_PDN, FUNC0); 	// P8.2 : GPIO4_2: LED6
	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 	// P8.1 : GPIO4_1: LED7
	scu_pinmux(0x8 ,0 , MD_PDN, FUNC0); 	// P8.0 : GPIO4_0: LED8

	FIO_ByteSetDir(4, 0, 0xFF, 0xFF);

	/* Init Alarm Timer with Preset Count = 1024 ~ 1s */
	ATIMER_Init(LPC_ATIMER, 1024);
	/* Enable Alarm Timer */
	ATIMER_IntEnable(LPC_ATIMER);

	/* Init EVRT */
	EVRT_Init(LPC_EVENTROUTER);
	/*Clear Alarm Timer Source */
	EVRT_ClrPendIntSrc(LPC_EVENTROUTER, EVRT_SRC_ATIMER);
	/* Enable EVRT in order to be able to read the ATIMER interrupt */
	EVRT_ConfigIntSrcActiveType(LPC_EVENTROUTER, EVRT_SRC_ATIMER, EVRT_SRC_ACTIVE_RISING_EDGE);
	/* Enable Alarm Timer Source */
	EVRT_SetUpIntSrc(LPC_EVENTROUTER, EVRT_SRC_ATIMER, ENABLE);

	/* Enable NVIC */
	NVIC_EnableIRQ(EVENTROUTER_IRQn);

	while(1){
		__WFI();
		_DBG("Waked Up by Alarm Timer\r\n");
	}
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

/**
 * @}
 */
