/**********************************************************************
* $Id: Lcd_Demo.c 28 2014-01-27 14:45:13Z danielru $		Lcd_Demo.c				2011-06-02
*//**
* @file		Lcd_Demo.c
* @brief	This example describes how to use LCD on Hitex board
* 			and LogicPD 4.3
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include "lpc18xx_lcd.h"
#include "lpc18xx_gpio.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_libcfg.h"
#include "lpc18xx_cgu.h"
#include "lcd_qvga_tft.h"
#include "Cursor.h"
#include "TSC2046.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Lcd_Demo	Lcd_Demo
 * @ingroup LCD_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS *************************/
#define LCD_WIDTH		240
#define LCD_HEIGHT		320
#define LOGO_WIDTH		110
#define LOGO_HEIGHT		42

#define TCS_USED 1
#define FRAMEBUFFER_ADDR        0x28000000

/* Touch Screen Config */
/************************** PRIVATE VARIABLES *************************/
uint32_t msec;

extern void SDRAM_Init (void);
extern const unsigned short image[];
extern const unsigned char Cursor[];
const TSC2046_Init_Type TSC2046_320240_12bit = {
		3686, 205, 3842, 267, 240, 320, 1
};
const TSC2046_Init_Type TSC2046_320240_8bit = {
		240, 10, 240, 16, 240, 320, 1
};
/* LCD configured struct */
const LCD_CFG_Type EA320x240 = {
	LCD_WIDTH,
	LCD_HEIGHT,
	{10,28},
	{1,2},
	2,
	2,
	1,
	LCD_SIGNAL_ACTIVE_LOW,
	LCD_SIGNAL_ACTIVE_LOW,
	LCD_CLK_FALLING,
	LCD_SIGNAL_ACTIVE_HIGH,
	10,
	LCD_BPP16_565,
	LCD_TFT,
	LCD_COLOR_FORMAT_BGR,
	FALSE,
	0
};

/* pointer to frame buffer */
uint16_t *framebuffer = (uint16_t *)FRAMEBUFFER_ADDR;
/************************** PRIVATE FUNCTIONS *************************/

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		SysTick handler sub-routine (1ms)
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SysTick_Handler (void) 					// SysTick Interrupt Handler @ 1000Hz
{
	if(msec)msec--;
}
/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		put a pixel
 * @param[in]	x,y postion of pixel
 * @param[in]	val color value of pixel in 16bit
 * @return 		None
 **********************************************************************/

void putpixel(uint32_t x, uint32_t y, uint16_t val){
	framebuffer[x+y*240] = val;
}

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
uint32_t tempx, tempy;

int c_entry (void) {                       /* Main Program                       */
	uint32_t i,j;
	int cursor_x = 150, cursor_y = 200;
	Bool Sts = FALSE;

	SystemInit();
	CGU_Init();
	CGU_SetDIV(CGU_CLKSRC_IDIVE, 1);
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_CLKSRC_IDIVE);
	CGU_EntityConnect(CGU_CLKSRC_IDIVE,CGU_BASE_LCD);
	SDRAM_Init();
	/*pin set up for LCD */
	scu_pinmux(0x07, 7, MD_PUP, FUNC3); 	/* LCD_PWR @ P7.7 */

	scu_pinmux(0x04, 7, MD_PUP, FUNC0);		/* LCD_DCLK @ P4.7 */
	scu_pinmux(0x04, 5, MD_PUP, FUNC2);		/* LCD_FP @ P4.5 */
	scu_pinmux(0x04, 6, MD_PUP, FUNC2); 	/* LCD_ENAB_M @ P4.6 */
	scu_pinmux(0x07, 6, MD_PUP, FUNC3);		/* LCD_LP @ P7.6 */
	scu_pinmux(0x04, 1, MD_PUP, FUNC2);		/* LCD_VD_0 @ P4.1 */
	scu_pinmux(0x04, 4, MD_PUP, FUNC2);		/* LCD_VD_1 @ P4.4 */
//	scu_pinmux(0x04, 3, MD_PUP, FUNC2);		/* LCD_VD_2 @ P4.3 */
	scu_pinmux(0x04, 2, MD_PUP, FUNC2);		/* LCD_VD_3 @ P4.2 */
	scu_pinmux(0x08, 7, MD_PUP, FUNC3);		/* LCD_VD_4 @ P8.7 */
	scu_pinmux(0x08, 6, MD_PUP, FUNC3);		/* LCD_VD_5 @ P8.6 */
	scu_pinmux(0x08, 5, MD_PUP, FUNC3);		/* LCD_VD_6 @ P8.5 */
	scu_pinmux(0x08, 4, MD_PUP, FUNC3);		/* LCD_VD_7 @ P8.4 */
	scu_pinmux(0x07, 5, MD_PUP, FUNC3);		/* LCD_VD_8 @ P7.5 */
	scu_pinmux(0x04, 8, MD_PUP, FUNC2);		/* LCD_VD_9 @ P4.8 */
	scu_pinmux(0x04, 10, MD_PUP, FUNC2);	/* LCD_VD_10 @ P4.10 */
	scu_pinmux(0x04, 9, MD_PUP, FUNC2); 	/* LCD_VD_11 @ P4.9 */
	scu_pinmux(0x08, 3, MD_PUP, FUNC3); 	/* LCD_VD_12 @ P8.3 */
	scu_pinmux(0x0B, 6, MD_PUP, FUNC2); 	/* LCD_VD_13 @ PB.6 */
	scu_pinmux(0x0B, 5, MD_PUP, FUNC2); 	/* LCD_VD_14 @ PB.5 */
	scu_pinmux(0x0B, 4, MD_PUP, FUNC2); 	/* LCD_VD_15 @ PB.4 */
	scu_pinmux(0x07, 4, MD_PUP, FUNC3);		/* LCD_VD_16 @ P7.4 */
	scu_pinmux(0x07, 2, MD_PUP, FUNC3); 	/* LCD_VD_18 @ P7.2 */
	scu_pinmux(0x07, 1, MD_PUP, FUNC3); 	/* LCD_VD_19 @ P7.1 */
	scu_pinmux(0x0B, 3, MD_PUP, FUNC2); 	/* LCD_VD_20 @ PB.3 */
	scu_pinmux(0x0B, 2, MD_PUP, FUNC2); 	/* LCD_VD_21 @ PB.2 */
	scu_pinmux(0x0B, 1, MD_PUP, FUNC2); 	/* LCD_VD_22 @ PB.1 */
	scu_pinmux(0x0B, 0, MD_PUP, FUNC2); 	/* LCD_VD_23 @ PB.0 */

	scu_pinmux(0x07, 0, MD_PUP, FUNC3);		/* LCD_LE @ P7.0 */
	scu_pinmux(0x04, 4, MD_PUP, FUNC0);		/* LCD_VD_2 @ P4.3 */
	scu_pinmux(0x07, 3, MD_PUP, FUNC0);		/* @ P7.3 */
	scu_pinmux(0x04, 1, MD_PUP, FUNC0);		/* LCDVDEN */


	GPIO_SetDir(2,1<<4, 1);
	GPIO_SetDir(2,1<<1, 1);
	GPIO_SetDir(3,1<<8, 1);	/*LCDLE*/
	GPIO_SetDir(3,1<<11, 1);

	GPIO_ClearValue(2, 1<<1);
	GPIO_ClearValue(3, 1<<8);
	GPIO_SetValue(2,1<<4);
	// M3Frequency is automatically set when SetClock(BASE_M3_CLK... was called.
	SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M3CORE)/1000);  				// Generate interrupt @ 1000 Hz
	msec=5;while(msec);

	LPC_RGU->RESET_CTRL0 = (1UL << 16);
	msec=5;while(msec);

	/*Fill Colorbar only*/
	for(i=0;i<LCD_WIDTH*LCD_HEIGHT/4;i++){
		framebuffer[i] = 0x1F;
		}
	for(i=LCD_WIDTH*LCD_HEIGHT/4;i<LCD_WIDTH*LCD_HEIGHT*2/4;i++){
		framebuffer[i] = 0x3F<<5;
		}
	for(i=LCD_WIDTH*LCD_HEIGHT*2/4;i<LCD_WIDTH*LCD_HEIGHT*3/4;i++){
		framebuffer[i] = 0x1F<<11;
		}
	for (i=LCD_WIDTH*LCD_HEIGHT*3/4; i < LCD_WIDTH*LCD_HEIGHT; i++) {
		framebuffer[i] = 0xFFFF;
	}
	/* Fill NXP logo */
#if 1
	for(j=0;j<LOGO_HEIGHT;j++){
		for(i=0;i<LOGO_WIDTH;i++){
			putpixel(i,j, image[(i+j*LOGO_WIDTH)]);
		}
	}
#endif
	GPIO_SetValue(3, 1<<11);
	LCD_Init(LPC_LCD, (LCD_CFG_Type*)&EA320x240);
	InitLcdController();

	TSC2046_Init((TSC2046_Init_Type *)&TSC2046_320240_12bit);

	LCD_SetFrameBuffer(LPC_LCD, (void*)framebuffer);
	LCD_Power(LPC_LCD, ENABLE);
	msec=100;while(msec);
	LCD_Cursor_Enable(LPC_LCD, 0, DISABLE);
	LCD_Cursor_Config(LPC_LCD, LCD_CURSOR_32x32, 1);
	LCD_Cursor_WriteImage(LPC_LCD, 0, (void *)Cursor);
	LCD_Cursor_SetPos(LPC_LCD,cursor_x,cursor_y);
	LCD_Cursor_Enable(LPC_LCD, 0, ENABLE);

	msec=20;while(msec);
	/* Enable Backlight */
	GPIO_SetValue(3,1<<8);
	while (1)
	{
		Sts = GetTouchPos((int16_t*)&cursor_x, (int16_t*)&cursor_y);
		if (Sts)
			LCD_Cursor_SetPos(LPC_LCD, cursor_x, cursor_y);
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
