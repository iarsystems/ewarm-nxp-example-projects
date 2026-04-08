/**********************************************************************
* $Id: lcd_qvga_tft.c 28 2014-01-27 14:45:13Z danielru $		lcd_qvga_tft.c			2012-04-24
*//**
* @file		lcd_qvga_tft.c
* @brief	Contains all functions to control LCD controller using SPI
* @version	1.0
* @date		24. April. 2012
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
#include "lpc18xx_ssp.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_timer.h"
#include "lpc18xx_gpio.h"
#include "lcd_qvga_tft.h"

#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc18xx_libcfg.h"
#else
#include "lpc18xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */

/** @defgroup Lcd_Demo	LCD QVGA TFT
 * @ingroup LCD_Examples
 * @{
 */

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
#define DC_CMD   (GPIO_ClearValue(7, 1<<8))
#define DC_DATA  (GPIO_SetValue(7, 1<<8))

#define SSP_PORT (LPC_SSP0)
#define SSP_CLOCK 1000000

/******************************************************************************
 * Local Functions
 *****************************************************************************/

/*********************************************************************//**
 * @brief 		Pin configuration to communicate with LCD Controller.
 * @param[in]	None
 * @return 		None
 **********************************************************************/
static void pinConfig(void)
{
#if !defined(HITEX_LCD_TERM)
	scu_pinmux(0xC,11,MD_PLN,FUNC4);	// PC.11 connected to GPIO = SSEL_MUX_A
	scu_pinmux(0xC,12,MD_PLN,FUNC4);	// PC.12 connected to GPIO = SSEL_MUX_B
	GPIO_SetDir(0x6,(1<<10), 1);
	GPIO_SetDir(0x6,(1<<11), 1);
#endif
	/* Configure SSP0 pins*/
	scu_pinmux(0xF,0,MD_PLN_FAST,FUNC0);	// PF.0 connected to SCL/SCLK	func2=SSP0 SCK0
 	scu_pinmux(0xF,1,MD_PLN_FAST,FUNC2);	// PF.1 connected to nCS		func2=SSP0 SSEL0
 	scu_pinmux(0xF,2,MD_PLN_FAST,FUNC2);	// PF.2 connected to SO			func2=SSP0 MISO0
 	scu_pinmux(0xF,3,MD_PLN_FAST,FUNC2);	// PF.3 connected to nSI		func2=SSP0 MOSI0

	scu_pinmux(0x0E, 8, MD_PUP, FUNC4);//DC PIN
  /*  (CS) Touch Pad*/
//  GPIO_SetDir(7,(1<<16),1);

  /* (DC) */
  GPIO_SetDir(7,(1<<8),1);

}

/*********************************************************************//**
 * @brief 		Write to a LCD register using SPI.
 * @param[in]	address   Register address
 * @param[in]	data      Data which will be written to the given register
 * @return 		None
 **********************************************************************/

static void
writeToReg(uint16_t addr, uint16_t data)
{
	uint8_t buf[2];
	SSP_DATA_SETUP_Type sspCfg;

	GPIO_SetValue(6,(1<<10));
	GPIO_ClearValue(6,(1<<11));
  
	DC_CMD;

	buf[0] = 0;
	buf[1] = (addr & 0xff);

	sspCfg.tx_data = buf;
	sspCfg.rx_data = NULL;
	sspCfg.length	= 2;

	SSP_ReadWrite (SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);

	DC_DATA;
	buf[0] = (data >> 8);
	buf[1] = (data & 0xff);
	SSP_ReadWrite (SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);

	DC_CMD;

	buf[0] = (0);
	buf[1] = (0x22);
	SSP_ReadWrite (SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);
}
/*********************************************************************//**
 * @brief 		Initialize SSD1289 LCD Controller.
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void ssd1289_init(void)
{
	writeToReg (0x00,0x0001);
	TIM_Waitms(15);
	writeToReg (0x03,0x6E3E); //0xAEAC
	writeToReg (0x0C,0x0007);
	writeToReg (0x0D,0x000E); //0x000F
	writeToReg (0x0E,0x2C00); //0x2900
	writeToReg (0x1E,0x00AE); //0x00B3
	TIM_Waitms(15);
	writeToReg (0x07,0x0021);
	TIM_Waitms(50);
	writeToReg (0x07,0x0023);
	TIM_Waitms(50);
	writeToReg (0x07,0x0033);
	TIM_Waitms(50);

	writeToReg (0x01,0x2B3F);
	writeToReg (0x02,0x0600);
	writeToReg (0x10,0x0000);
	TIM_Waitms(15);
	writeToReg (0x11,0xC5B0); //0x65b0
	TIM_Waitms(20);
	writeToReg (0x05,0x0000);
	writeToReg (0x06,0x0000);
	writeToReg (0x16,0xEF1C);
	writeToReg (0x17,0x0003);
	writeToReg (0x07,0x0233);
	writeToReg (0x0B,0x5312);
	writeToReg (0x0F,0x0000);
	writeToReg (0x25,0xE000);
	TIM_Waitms(20);
	writeToReg (0x41,0x0000);
	writeToReg (0x42,0x0000);
	writeToReg (0x48,0x0000);
	writeToReg (0x49,0x013F);
	writeToReg (0x44,0xEF00);
	writeToReg (0x45,0x0000);
	writeToReg (0x46,0x013F);
	writeToReg (0x4A,0x0000);
	writeToReg (0x4B,0x0000);
	TIM_Waitms(20);
	writeToReg (0x30,0x0707);
	writeToReg (0x31,0x0704);
	writeToReg (0x32,0x0005); //0x0204
	writeToReg (0x33,0x0402); //0x0201
	writeToReg (0x34,0x0203);
	writeToReg (0x35,0x0204);
	writeToReg (0x36,0x0204);
	writeToReg (0x37,0x0401); //0x0502
	writeToReg (0x3A,0x0302);
	writeToReg (0x3B,0x0500);
	TIM_Waitms(20);
	writeToReg (0x22,0x0000);
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

/*********************************************************************//**
 * @brief 		Initialize LCD Controller.
 * @param[in]	None
 * @return 	None
 **********************************************************************/

void InitLcdController (void)
{
	SSP_CFG_Type SSP_ConfigStruct;
	TIM_TIMERCFG_Type TIM_ConfigStruct;

	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIMER0, TIM_TIMER_MODE,&TIM_ConfigStruct);

	pinConfig();

	// initialize SSP configuration structure to default
	SSP_ConfigStructInit(&SSP_ConfigStruct);

	// set clock rate
	SSP_ConfigStruct.ClockRate = SSP_CLOCK;

	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(SSP_PORT, &SSP_ConfigStruct);

	// Enable SSP peripheral
	SSP_Cmd(SSP_PORT, ENABLE);

	TIM_Waitms(200);

	/* initialize LCD controller */
	ssd1289_init();

	SSP_Cmd(SSP_PORT, DISABLE);
	SSP_DeInit(SSP_PORT);
	TIM_DeInit(LPC_TIMER0);
}

/**
 * @}
 */

