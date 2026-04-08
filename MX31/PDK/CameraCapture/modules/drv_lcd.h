/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_lcd.c
 *    Description : iMX31 PDK LCD driver module
 *
 *    History :
 *    1. Date        : 04.11.2008 
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *    $Revision: 16 $
 **************************************************************************/
#ifndef __DRV_LCD_H
#define __DRV_LCD_H

#define LCD_WIDTH           (480UL)
#define LCD_HEIGHT          (640UL)
#define LCD_H_SYNC_WIDTH    (20UL)        /*PCLK*/
#define LCD_V_SYNC_WIDTH    (10UL)        /*Line*/
#define LCD_H_WAIT_START    (41UL)
#define LCD_H_WAIT_END      (60UL)
#define LCD_V_WAIT_START    (5UL)
#define LCD_V_WAIT_END      (10UL)
#define LCD_BPP             (16UL)
#define LCD_FPS             (60UL)
#define LCD_PPL             (LCD_WIDTH /*+LCD_H_WAIT_START*/+LCD_H_WAIT_END+LCD_H_SYNC_WIDTH)
#define LCD_LPF             (LCD_HEIGHT/*+LCD_V_WAIT_START*/+LCD_V_WAIT_END+LCD_V_SYNC_WIDTH)
#define LCD_PXL_CLK         (LCD_LPF*LCD_PPL*LCD_FPS)

#define PICTURE_HEIGHT      300
#define PICTURE_WIDTH       400

extern __no_init Int16U LcdBgBuff[LCD_WIDTH*LCD_HEIGHT];
extern __no_init Int16U LcdFgBuff[PICTURE_WIDTH*PICTURE_HEIGHT];

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
