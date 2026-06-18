/**********************************************************************
* $Id: CGU_measureFreq.c 28 2014-01-27 14:45:13Z danielru $		CGU_measureFreq.c	2011-12-26
*//**
* @file		CGU_measureFreq.c
* @brief	This example describes how to use CGU to setting clock and determine Clock' freq
* @version	1.0
* @date		26. Dec. 2011
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include "lpc_types.h"
#include "lpc18xx_libcfg.h"
#include "lpc18xx_cgu.h"
#include "debug_frmwrk.h"


/* Example group ----------------------------------------------------------- */
/** @defgroup CGU_measureFreq	CGU_measureFreq
 * @ingroup CGU_Examples
 * @{
 */

/************************** PRIVATE MACRO *************************/
/* Assumed Xtal Frequency */
#define XTAL_FREQ	12000000

/************************** PRIVATE VARIABLES *************************/
uint8_t menu[]=
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"CGU demo \n\r"
	"\t - MCU: LPC18xx \n\r"
	"\t - Core: ARM CORTEX-M3 \n\r"
	"\t - Communicate via: UART1 - 115200 bps \n\r"
	"Use CGU to setup and show source base clock frequencies \n\r"
	"********************************************************************************\n\r";
/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry (void) {                       /* Main Program                       */
	
	CGU_ENTITY_T ClockSource;
	uint32_t SourceFreq, mul, div;
	
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
	
	/* print menu */
	_DBG(menu);
	
	/* setup PLL0 USB */
	_DBG("Setting USB PLL...\n\r");
	/* Disable PLL first */
	CGU_EnableEntity(CGU_CLKSRC_PLL0, DISABLE);
	/* the usb core require output clock = 480MHz */
	if(CGU_SetPLL0(XTAL_FREQ, 480000000, 0.98, 1.02) != CGU_ERROR_SUCCESS)
		while(1);
	CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_CLKSRC_PLL0);
	/* Enable PLL after all setting is done */
	CGU_EnableEntity(CGU_CLKSRC_PLL0, ENABLE);
	
	/* setup PLL0 Audio */
	lpc_printf("Setting Audio PLL...\r\n");
	/* Disable PLL first */
	CGU_EnableEntity(CGU_CLKSRC_PLL0_AUDIO, DISABLE);
	/* 24576000 is 48k x 512 for audio sysclk */
	if(CGU_SetPLL0_Audio(XTAL_FREQ, 24576000, 0.98, 1.02) != CGU_ERROR_SUCCESS)
		while(1);
	CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_CLKSRC_PLL0_AUDIO);
	/* Enable PLL after all setting is done */
	CGU_EnableEntity(CGU_CLKSRC_PLL0_AUDIO, ENABLE);
	
	/* Set up Divider */
	lpc_printf("Setting All Divider's divisors to 4...\r\n");
	for(ClockSource = CGU_CLKSRC_IDIVA; ClockSource <= CGU_CLKSRC_IDIVE; ClockSource++){
		/* Disable DIV first */
		CGU_EnableEntity(ClockSource, DISABLE);
		CGU_SetDIV(ClockSource, 4);
		CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, ClockSource);
		/* Enable DIV after all setting is done */
		CGU_EnableEntity(ClockSource, ENABLE);
	}
	
	lpc_printf("All Settings Done! Continue to measure Clock Freq ...\r\n");
	/* Measure and display Source Clock freq */
	lpc_printf("Measuring IRC Clock Freq ...");
	CGU_RealFrequencyCompare(CGU_CLKSRC_IRC, CGU_CLKSRC_XTAL_OSC, &mul, &div);
	SourceFreq = (XTAL_FREQ/1000)*(uint64_t)mul/div;
	lpc_printf("=%d kHz\r\n", SourceFreq);
	
	lpc_printf("Measuring PLL0 Clock Freq ...");
	CGU_RealFrequencyCompare(CGU_CLKSRC_PLL0, CGU_CLKSRC_XTAL_OSC, &mul, &div);
	SourceFreq = (XTAL_FREQ/1000)*(uint64_t)mul/div;
	lpc_printf("=%ld kHz\r\n", SourceFreq);
	
	lpc_printf("Measuring PLL0 Audio Clock Freq ...");
	CGU_RealFrequencyCompare(CGU_CLKSRC_PLL0_AUDIO, CGU_CLKSRC_XTAL_OSC, &mul, &div);
	SourceFreq = (XTAL_FREQ/1000)*(uint64_t)mul/div;
	lpc_printf("=%ld kHz\r\n", SourceFreq);
	
	for(ClockSource = CGU_CLKSRC_IDIVA; ClockSource <= CGU_CLKSRC_IDIVE; ClockSource++){
		lpc_printf("Measuring Divider %c Clock Freq ...", ClockSource - CGU_CLKSRC_IDIVA + 'A');
		CGU_RealFrequencyCompare(ClockSource, CGU_CLKSRC_XTAL_OSC, &mul, &div);
		SourceFreq = (XTAL_FREQ/1000)*(uint64_t)mul/div;
		lpc_printf("=%ld kHz\r\n", SourceFreq);
	}
	
	lpc_printf("Measure finished! Demo End!\r\n");
	while (1);
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
