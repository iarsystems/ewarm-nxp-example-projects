/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_lcd.h
 *    Description : LCD driver include file
 *
 *    History :
 *    1. Date        : May, 3 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "includes.h"

#ifndef __DRV_LCD_H
#define __DRV_LCD_H

#define IMAGE_WIDTH         (240UL)
#define IMAGE_HEIGHT        (320UL)
#define IMAGE_BPP           (16UL)
#define LCD_WIDTH           (240UL)
#define LCD_HEIGHT          (320UL)
#define LCD_BPP             (16UL)

extern Int8U LcdBuff[LCD_WIDTH*LCD_HEIGHT*(LCD_BPP/8)];

/*************************************************************************
 * Function Name: LcdSetup
 * Parameters: none
 *
 * Return: none
 *
 * Description: Set up LCD panel to display a video RAM data
 *
 *************************************************************************/
void LcdSetup(void);

#endif // __DRV_LCD_H
