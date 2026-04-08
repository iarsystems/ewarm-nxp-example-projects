/**********************************************************************
* $Id: Pwr_PowerDown.c 28 2014-01-27 14:45:13Z danielru $		Pwr_PowerDown.c				2011-06-02
*//**
* @file		Pwr_PowerDown.c
* @brief	This example describes how to enter the system in power down
* 			mode and wake-up by using WAKEUP0 pin
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
#include "lpc18xx_evrt.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_gpio.h"
#include "lpc18xx_cgu.h"
#include "lpc18xx_pwr.h"
#include "lpc18xx_libcfg.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Pwr_PowerDown	Pwr_PowerDown
 * @ingroup PWR_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS *************************/

#define LED_CONTROL_PORT_NUM			4
#define LED_CONTROL_BYTE_NUM			0
#define LED_CONTROL_ALL_CONFIG_OUTPUT	0x2	/*P4.1*/
#define LED_CONTROL_CLEAR_ALL			0x2
#define OUTPUT							0x01
#define INPUT							0x00


/************************** PRIVATE FUNCTION *************************/
void EVRT_IRQHandler(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		EVRT interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EVRT_IRQHandler(void)
{
	if(EVRT_IsSourceInterrupting(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0))
	{
		EVRT_ClrPendIntSrc(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0);
	}

}


/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry (void)
{
	uint32_t j;

	SystemInit();
	CGU_Init();

	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 	// USB_IND1 LED

	FIO_ByteSetDir(LED_CONTROL_PORT_NUM,LED_CONTROL_BYTE_NUM,
					LED_CONTROL_ALL_CONFIG_OUTPUT,OUTPUT);

	// Initiate EVRT
	EVRT_Init(LPC_EVENTROUTER);
	// Configure interrupt signal from WAKEUP0 pin to EVRT
	EVRT_ConfigIntSrcActiveType(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0,EVRT_SRC_ACTIVE_RISING_EDGE);
	// Enable interrupt signal from WAKEUP0 pin to EVRT
	EVRT_SetUpIntSrc(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0,ENABLE);

    /* Disable EVRT interrupt */
    NVIC_DisableIRQ(EVENTROUTER_IRQn);
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(EVENTROUTER_IRQn, ((0x01<<3)|0x01));
    /* Enable RTC interrupt */
    NVIC_EnableIRQ(EVENTROUTER_IRQn);

	while (1)
	{
			for (j = 0; j < 5000000; j++);
			FIO_ByteClearValue(LED_CONTROL_PORT_NUM,LED_CONTROL_BYTE_NUM,LED_CONTROL_CLEAR_ALL);
			GPIO_SetValue(LED_CONTROL_PORT_NUM,(1<<1));
		PWR_PowerDown();
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
