/**********************************************************************
* $Id: TSC2046.c 28 2014-01-27 14:45:13Z danielru $		TSC2046.c			2012-04-24
*//**
* @file		TSC2046.c
* @brief	Contains all functions to control TSC2046 using SPI
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
#include "lpc18xx.h"
#include "lpc18xx_ssp.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_gpio.h"
#include "TSC2046.h"

#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc18xx_libcfg.h"
#else
#include "lpc18xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */

#if (TSC2046_CONVERSION_BITS == 12)
#define TSC2046_COORD_MAX           (0xFFF)
#define TSC2046_DELTA_VARIANCE      (0x50)
#else
#define TSC2046_COORD_MAX           (0xFF)
#define TSC2046_DELTA_VARIANCE      (0x03)
#endif
#define COORD_GET_NUM                 (10)

/** SSP Configuration */
#define TSC2046_SSP_PORT	            (LPC_SSP0)

/** Local variables */
static TSC2046_Init_Type TSC_Config;

static void TSC2046_ReadWrite(uint8_t command, uint16_t* data);

/*********************************************************************//**
 * @brief 		Initialize TSC2046.
 * @param[in]	pConfig  TSC Configuration
 * @return 		None
 **********************************************************************/
void TSC2046_Init(TSC2046_Init_Type *pConfig)
{
	SSP_CFG_Type SSP_ConfigStruct;
	uint16_t dummy_data;

#if !defined(HITEX_LCD_TERM)
	scu_pinmux(0xC,11,MD_PLN,FUNC4);	// PC.11 connected to GPIO = SSEL_MUX_A
	scu_pinmux(0xC,12,MD_PLN,FUNC4);	// PC.12 connected to GPIO = SSEL_MUX_B
	GPIO_SetDir(0x6,(1<<10), 1);
	GPIO_SetDir(0x6,(1<<11), 1);
#endif

	/* Configure SSP0 pins*/
	scu_pinmux(0xF,0,MD_PLN_FAST,FUNC0);	// PF.0 connected to SCL/SCLK	func2=SSP0 SCK0
// 	scu_pinmux(0xF,1,MD_PLN_FAST,FUNC2);	// PF.1 connected to nCS		func2=SSP0 SSEL0
	scu_pinmux(0xF,1,MD_PUP,FUNC4);	// PF.1 connected to nCS		func2=SSP0 SSEL0
 	scu_pinmux(0xF,2,MD_PLN_FAST,FUNC2);	// PF.2 connected to SO			func2=SSP0 MISO0
 	scu_pinmux(0xF,3,MD_PLN_FAST,FUNC2);	// PF.3 connected to nSI		func2=SSP0 MOSI0

 	GPIO_SetDir(0x7,(1<<16), 1);

 	GPIO_ClearValue(6,(1<<10));
	GPIO_SetValue(6,(1<<11));

	// initialize SSP configuration structure to default
	SSP_ConfigStructInit(&SSP_ConfigStruct);

	// set clock rate
	SSP_ConfigStruct.ClockRate = TSC2046_SSP_CLOCK;
	SSP_ConfigStruct.CPHA = SSP_CPHA_SECOND;
	SSP_ConfigStruct.CPOL = SSP_CPOL_LO;

	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(TSC2046_SSP_PORT, &SSP_ConfigStruct);

	// Enable SSP peripheral
	SSP_Cmd(TSC2046_SSP_PORT, ENABLE);

	// Enable Touch Screen Controller
	TSC2046_ReadWrite(PWRDOWN, &dummy_data);

	TSC_Config = *pConfig;

}
/*********************************************************************//**
 * @brief 		Send/Receive data to/from TSC2046.
 * @param[in]	command     It should be
 *                          X_MEASURE
 *                          Y_MEASURE
 *                          Z1_MEASURE
 *                          Z2_MEASURE
 *                          PWRDOWN
 * @param[out]	data       Received data
 * @return 		None
 **********************************************************************/
static void TSC2046_ReadWrite(uint8_t command, uint16_t* data)
{
	uint32_t tmp;
	uint8_t rx_data[3], tx_data[3] = {0x00, 0x00, 0x00};
	SSP_DATA_SETUP_Type sspCfg;

	tx_data[0] = command;
	GPIO_ClearValue(6,(1<<10));
	GPIO_SetValue(6,(1<<11));

	GPIO_ClearValue(7,(1<<16));

	for(tmp = 0x100; tmp ;tmp--);

	sspCfg.tx_data = &tx_data;
	sspCfg.rx_data = &rx_data;
	sspCfg.length  = 3;
	SSP_ReadWrite (TSC2046_SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);
	for(tmp = 0x100; tmp;tmp--);

#if (TSC2046_CONVERSION_BITS == 8)
	*data = (((rx_data[1]<<8) | (rx_data[2])) >> 7)&0xFF;
#else
    *data = (((rx_data[1]<<8) | rx_data[2]) >> 3)&0xFFF;
#endif
    GPIO_SetValue(7,(1<<16));
}

/*********************************************************************//**
 * @brief 		Evaluate the coordinates received from TSC.
 * @param[in]	command    TSC command, should be:
 *                          X_MEASURE
 *                          Y_MEASURE
 *                          Z1_MEASURE
 *                          Z2_MEASURE
 * @param[out]	coord   the coordinate after evaluate
 * @return 		Status, should be: SUCCESS or ERROR.
 **********************************************************************/
static Status EvalCoord(uint8_t command, uint16_t* coord)
{
	uint32_t i;
	uint16_t Tmp=0, previousTmp = *coord;
	int16_t diff = 0;

	for(i = 0; i < COORD_GET_NUM; i++)
	{
		TSC2046_ReadWrite(command,&Tmp);
		if(Tmp > TSC2046_COORD_MAX)
			return ERROR;
		if(i > 0)
			diff = Tmp - previousTmp;
		if(diff < 0)
			diff = 0 - diff;
		if(diff > TSC2046_DELTA_VARIANCE)
			return ERROR;
		*coord += Tmp;
		previousTmp = Tmp;
	}
	*coord /= (COORD_GET_NUM+1);
	return SUCCESS;
}
/*********************************************************************//**
 * @brief 		convert the coord received from TSC to a value on truly LCD.
 * @param[in]	Coord       received coord
 * @param[in]	MinVal    the minimum value of a coord
 * @param[in]	MaxVal     the maximum value of a coord
 * @param[in]	TrueSize   the size on LCD
 * @return 		the coord after converting.
 **********************************************************************/
static int16_t TSCCalibrate(int16_t Coord, int16_t MinVal, int16_t MaxVal, int16_t TrueSize)
{
	 int16_t tmp;
	 int16_t ret;
	 uint8_t convert = 0;

	 if(MinVal > MaxVal)		  // Swap value
	 {
		  tmp = MaxVal;
		  MaxVal = MinVal;
		  MinVal = tmp;
		  convert = 1;
	 }

	 ret = (Coord - MinVal)*TrueSize/(MaxVal-MinVal);
	 if(convert)
		ret = TrueSize - ret;

	 return ret;
}
/*********************************************************************//**
 * @brief 		Get Touch coordinates.
 * @param[out]	pX	  X-Coord
 * @param[out]	pY	  Y-Coord
 * @return 		None
 **********************************************************************/
Bool GetTouchPos(int16_t *pX, int16_t* pY)
{
	uint16_t tmp;
	uint16_t x, y, z1, z2, z;
	Status Sts = SUCCESS;

	TSC2046_ReadWrite(X_MEASURE,&x);
	TSC2046_ReadWrite(Y_MEASURE,&y);
	TSC2046_ReadWrite(Z1_MEASURE,&z1);
	TSC2046_ReadWrite(Z2_MEASURE,&z2);
	if (z1 != 0) {
		z = x * ((z2 / z1) - 1);
	}
	if (z <= 0 || z > 35000)
		return FALSE;
	// Get X-Coordinate
	Sts = EvalCoord(X_MEASURE,&x);
	if(Sts == ERROR)
		return FALSE;
	// Get Y-Coordinate
	Sts = EvalCoord(Y_MEASURE,&y);
	if(Sts == ERROR)
		return FALSE;
	// Get Z1-Value
	Sts = EvalCoord(Z1_MEASURE,&z1);
	if(Sts == ERROR)
		return FALSE;
	// Get Z2-Value
	Sts = EvalCoord(Z2_MEASURE,&z2);
	if(Sts == ERROR)
		return FALSE;

	z = x * ((z2 / z1) - 1);
	if (z <= 0 || z > 35000)
		return FALSE;
	else// Swap, adjust to truly size of LCD
	{
		if(TSC_Config.swap_xy)
		{
			*pY = TSCCalibrate(x,TSC_Config.ad_top,TSC_Config.ad_bottom,TSC_Config.lcd_height);
			*pX = TSCCalibrate(y,TSC_Config.ad_left,TSC_Config.ad_right,TSC_Config.lcd_width);
		}
		else
		{
			*pX = TSCCalibrate(x,TSC_Config.ad_top,TSC_Config.ad_bottom,TSC_Config.lcd_width);
			*pY = TSCCalibrate(y,TSC_Config.ad_left,TSC_Config.ad_right,TSC_Config.lcd_height);
		}
	}
	TSC2046_ReadWrite(PWRDOWN, &tmp);
	return TRUE;
}


