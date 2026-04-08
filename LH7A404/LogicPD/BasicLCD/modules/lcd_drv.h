/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lcd_drv.h
 *    Description : Prototypes for the LH7952x LCD controller driver
 *
 *    History :
 *    1. Date        : 7 May 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 34 $
 **************************************************************************/
#include "includes.h"

#ifndef __LCD_DRV_H
#define __LCD_DRV_H

#define LCD_WIDTH				240 	// Pixels per line
#define LCD_HEIGHT			320 	// Lines per panel

// LCD display types
typedef enum {
	TFT = 0,      // Panel type is standard TFT
	ADTFT,        // Panel type is advanced TFT
	HRTFT,        // Panel type is highly reflective TFT
	MONO_4BIT,    // Panel type is 4-bit mono
	MONO_8BIT,    // Panel type is 8-bit mono
	CSTN,         // Panel type is color STN
} LcdPanel_t;

/* NO_ERROR macro */
#define _NO_ERROR           (INT_32)(0)
/* ERROR macro */
#define _ERROR              (INT_32)(-1)

typedef enum
{
	BPP_1 = 0, BPP_2, BPP_4, BPP_8, BPP_16
} BitPerPix_t;

typedef enum
{
  // Enable the lcd controller
  LCD_ENABLE = 0,
  // Enable the lcd controller
  LCD_DISABLE,
  // Enable LCD power
  LCD_PW_ENABLE,
  // Disable LCD power
  LCD_PW_DISABLE,
  // Update the upper frame buffer pointer
  LCD_SET_UP_FB,
  // Update the lower frame buffer pointer
  LCD_SET_LOW_FB,
  // Get the state of the controller - Enable or disabled
  LCD_GET_STATUS,
  // Mask interrupts
  LCD_DISABLE_INT,
  // UnMask interrupts
  LCD_ENABLE_INT,
  // Enable Vertical compare interrupts
  LCD_ENABLE_VCOMP_INT,
  // Disable Vertical compare interrupts
  LCD_DISABLE_VCOMP_INT,
  // Set the controller into BGR mode
  LCD_SET_BGR,
  // Set the controller into RGB mode
  LCD_SET_RGB,
  // Bind a callback method to be used by the isr
  LCD_SET_CALLBACK,
  // Update LCD Palette RAM
  LCD_SET_PALETTE,
} LcdCommand_t;

// Structure containing the parameters for the LCD panel
typedef struct {
	Int8U       	h_BackPorch;         	/* Horizontal back porch in
	            	                         clocks (minimum of 1) */
	Int8U       	h_FrontPorch;        	/* Horizontal front porch in
	            	                         clocks (minimum of 1) */
	Int8U       	h_SyncPulseWidth;   	/* HSYNC pulse width in
	            	                         clocks (minimum of 1) */
	Int16U      	PixelsPerLine;      	/* Pixels per line (horizontal
	            	                         resolution) */
	Int8U       	v_BackPorch;         	/* Vertical back porch in
	            	                         clocks */
	Int8U       	v_FrontPorch;        	/* Vertical front porch in
	            	                         clocks */
	Int8U       	v_SyncPulseWidth;   	/* VSYNC pulse width in
	            	                         clocks (minimum 1 clock) */
	Int16U      	LinesPerPanel;      	/* Lines per panel (vertical
	            	                         resolution) */
  Int16U        ClocksPerLine;      	/* Clocks per line */
	Boolean     	InvertOutputEnable; 	/* Invert output enable, 1 =
	            	                         invert*/
	Boolean     	InvertPanelClock;   	/* Invert panel clock, 1 =
	            	                         invert*/
	Boolean       InvertHsync;         	/* Invert HSYNC, 1 = invert */
	Boolean       InvertVsync;         	/* Invert VSYNC, 1 = invert */
	Int8U       	AC_BiasFrequency;    	/* AC bias frequency in
	            	                         clocks (minimum 1) */
  Boolean       Clock14mhzSelect;   	/* 0 = use bus clock */
  Boolean       RgbNormalMode;      	/* 0 = swap red and blue */
  Boolean       DmaOn4Empty;       		/* Watermark (DMA) bit */
	Int8U       	BitsPerPixel;       	/* Maximum bits per pixel the
	            	                         display supports */
	Int32U      	OptimalClock;        	/* Optimal clock rate (Hz) */
	LcdPanel_t    LCD_PanelType;       	/* LCD panel type */
  Boolean       DualPanel;   					/* Single panel display */
	
	/* The following parameters are needed for ADTFT and HRTFT panels
	   only. For all other panels, these should be programmed to 0 */
	Boolean      	HrtftClsEnable;     	/* HRTFT CLS enable flag, 1 =
	                                       enable */
	Boolean       HrtftSpsEnable;     	/* HRTFT SPS enable flag, 1 =
	                                       enable */
	Int8U         HrtftLpToPsDelay; 		/* HRTFT LP to PS delay in
	                                       clocks */
	Int8U         HrtftPolarityDelay; 	/* HRTFT polarity delay in
	                                       clocks */
	Int8U         HrtftLpDelay;       	/* HRTFT LP delay in clocks */
	Int8U         HrtftSplDelay;      	/* HRTFT SPL delay in
	                                       clocks */
/* HRTFT SPL to CLKS delay */
  Int16U        HrtftSplToClsDelay;
  Int32U        PixelClkDivider;    	/* Pixel clock divider */
} LcdParam_t, * pLcdParam_t;

/*************************************************************************
 * Function Name: Lcd_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: The LCD Controller Interrupt handler
 *
 *************************************************************************/
void Lcd_Handler(void);

/*************************************************************************
 * Function Name: LcdInit
 * Parameters: const LcdParam_t * LcdPram, VicIrqSlots_t Slot
 *
 * Return: Boolean
 *
 * Description: Init LCD Controller
 *
 *************************************************************************/
Boolean LcdInit(const LcdParam_t * LcdPram, VicIrqSlots_t Slot);

/*************************************************************************
 * Function Name: LcdIoCtrl
 * Parameters: LcdCommand_t Command, Int32U Arg
 *
 * Return: Boolean
 *
 * Description: LCD configuration block  This function is a large case block.
 *              Based on the passed function and option values, set or get
 *              the appropriate real time clock parameter.
 *
 *************************************************************************/
Boolean LcdIoCtrl (LcdCommand_t Command, Int32U Arg);

#endif // __LCD_DRV_H
