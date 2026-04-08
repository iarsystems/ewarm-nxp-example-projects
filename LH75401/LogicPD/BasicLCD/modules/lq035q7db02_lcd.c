/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lq035q7db02_lcd.c
 *    Description : The display configuration settings used for
 *     							the LQ035Q7DB02 HRTFT display
 *
 *    History :
 *    1. Date        : 7 May 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 34 $
 **************************************************************************/
#include "lq035q7db02_lcd.h"

// NXP LQ035Q7DB02 portrait mode ADTFT display parameters
const LcdParam_t NXPLQ035Q7DB02 =
{
    21,       					// Horizontal back porch
    11,       					// Horizontal front porch
    13,       					// HSYNC pulse width
    LCD_WIDTH,					// Pixels per line
    7,        					// Vertical back porch
    4,        					// Vertical front porch
    2,        					// VSYNC pulse width
    LCD_HEIGHT,					// Lines per panel
    LCD_WIDTH-1,       	// Clocks per line (minus 1)
    0,        					// Do not invert output enable
    1,        					// Invert panel clock
    1,        					// Invert HSYNC
    0,        					// Do not invert VSYNC
    1,        					// AC bias frequency (not used)
    0,        					// 0 = use bus clock
    0,        					// Red and blue are not swapped
    0,        					// Watermark (DMA) bit DMA request on 8 empty
    CLCDC_COLOR_DEPTH, 	// Bits per pixel
    5213000,  					// Optimal clock rate (Hz)
    HRTFT,    					// LCD panel type
    0,        					// Single panel display
    1,        					// HRTFT CLS enable flag
    1,        					// HRTFT SPS enable flag
    9,        					// HRTFT LP to PS delay
    3,        					// HRTFT polarity delay
    14,       					// HRTFT LP delay
    34,       					// HRTFT SPL delay
    209,       					// HRTFT SPL to CLKS delay
    8,         					// Pixel clock divider
};
