/**********************************************************************
* $Id: Sct_SimpleCapture.c 28 2014-01-27 14:45:13Z danielru $		Sct_SimpleCapture.c			2011-06-02
*//**
* @file		Sct_SimpleCapture.c
* @brief	This example implements simple capture of SCT
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
#include "lpc18xx_sct.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_gpio.h"
#include "sct_fsm.h"
#include "lpc18xx_libcfg.h"
#include "debug_frmwrk.h"
#include "lpc18xx_cgu.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Sct_SimpleCapture	Sct_SimpleCapture
 * @ingroup SCT_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS **********************/
#define NUM_CAPTURE 4


/************************** PRIVATE VARIABLES *************************/
uint8_t menu[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
" SCT demo \n\r"
"\t - MCU: LPC18xx \n\r"
"\t - Core: ARM CORTEX-M3 \n\r"
"\t - Communicate via: UART1 - 115200 bps \n\r"
" A simple capture SCT example. \n\r"
"********************************************************************************\n\r";

volatile int wait = 1;
uint32_t sample_buffer[NUM_CAPTURE];


/************************** PRIVATE FUNCTION *************************/
void print_menu(void);
void SCT_IRQHandler (void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		RTC interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SCT_IRQHandler (void)
{
    /* Acknowledge the interrupt souce */
	SCT_EventFlagClear(SCT_IRQ_EVENT_capture_complete);

    /* Let 'main' know */
    wait = 0;
}

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print menu
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG(menu);
}


/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main SCT program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	int i,j;

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

	/* Pin configuration for SCT */
	scu_pinmux (4, 2, MD_PUP, FUNC1);   	/* P4_2:  SCTOUT0_0 */
	scu_pinmux (0xF,8, MD_PDN, FUNC2);   	/* PF_8:  SCTIN0_2 pin 20 X5 */

	/* Configure GPIO pin as pulse generator */
	scu_pinmux (7, 0, MD_PDN, FUNC0);   	/* P7_0: GPIO 3_8 pin 1 JP19 */
	GPIO_SetDir(3,(1<<8),1);				// output
	GPIO_ClearValue(3,(1<<8));				// Clear -> initiate low

	/* Global configuration of the SCT */
	SCT_Config(SCT_CONFIG_16BIT_COUNTER);
	SCT_ControlSet(SCT_CTRL_CLRCTR_L|SCT_CTRL_HALT_L|SCT_CTRL_PRE_L(100-1)
					|SCT_CTRL_HALT_H|SCT_CTRL_CLRCTR_H|SCT_CTRL_PRE_H(100-1),
					ENABLE);

	/* Now use the FSM code to configure the state machine */
	sct_fsm_init();

	_DBG("SCT start capturing ...");

	/* Start the SCT */
	SCT_ControlSet(SCT_CTRL_HALT_L,DISABLE);

	NVIC_EnableIRQ (SCT_IRQn);

	for(j=0;j<6;j++)
	{
		for(i=0;i<10000;i++);
		GPIO_SetValue(3,(1<<8));			// create rising edge
		for (i=0;i<10000;i++);
		GPIO_ClearValue(3,(1<<8));			// create falling edge
	}

	// run cpu in a loop waiting for complete
	while (wait);

	// put the results in the buffer
	sample_buffer[0] = SCT_CAPTURE_sample1;
	sample_buffer[1] = SCT_CAPTURE_sample2;
	sample_buffer[2] = SCT_CAPTURE_sample3;
	sample_buffer[3] = SCT_CAPTURE_sample4;

	_DBG("completed\n\r");
	_DBH32(sample_buffer[0]);_DBG("\n\r");
	_DBH32(sample_buffer[1]);_DBG("\n\r");
	_DBH32(sample_buffer[2]);_DBG("\n\r");
	_DBH32(sample_buffer[3]);_DBG("\n\r");

	while(1);


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

/*
 * @}
 */

