/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lq035q7db02_lcd.h
 *    Description : Prototypes for the display configuration settings
 *									used for the LQ035Q7DB02 HRTFT display
 *
 *    History :
 *    1. Date        : 7 May 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 34 $
 **************************************************************************/
#include "lcd_drv.h"

#ifndef __LQ035Q7DB02_LCD_H
#define __LQ035Q7DB02_LCD_H

#define LCD_WIDTH						240 	// Pixels per line
#define LCD_HEIGHT					320 	// Lines per panel
#define CLCDC_COLOR_DEPTH		BPP_8	// Bits per pix

extern const LcdParam_t NXPLQ035Q7DB02;

#endif // __LQ035Q7DB02_LCD_H
