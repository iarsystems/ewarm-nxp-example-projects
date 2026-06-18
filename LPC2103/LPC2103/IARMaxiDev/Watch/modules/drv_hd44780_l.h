/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_hd44780_l.h
 *    Description : LCD low level I/O function inlcude file
 *
 *    History :
 *    1. Date        : 25, May 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#ifndef  __DRV_HD44780_L_H
#define  __DRV_HD44780_L_H

/* Delay macro */
#define HD44780_BUS_DLY()      for(volatile int dly = 10;dly;--dly)

/*************************************************************************
 * Function Name: HD44780_IO_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init GPIO and clear shift register
 *
 *************************************************************************/
void HD44780_IO_Init (void);

/*************************************************************************
 * Function Name: HD44780SetRS
 * Parameters: Boolean Data
 * Return: none
 * Description: Set RS signal
 *
 *************************************************************************/
void HD44780SetRS (Boolean Data);

/*************************************************************************
 * Function Name: HD44780SetE
 * Parameters: Boolean Data
 * Return: none
 * Description: Set E signal
 *
 *************************************************************************/
void HD44780SetE (Boolean Data);

/*************************************************************************
 * Function Name: HD44780WrIO
 * Parameters: Int8U Data
 * Return: none
 * Description: Write to HD44780 I/O
 *
 *************************************************************************/
void HD44780WrIO (Int8U Data);

/*************************************************************************
 * Function Name: HD44780SetBackLight
 * Parameters: Boolean Data
 * Return: none
 * Description: Set backlight
 *
 *************************************************************************/
void HD44780SetBackLight (Boolean Data);
#endif // __DRV_HD44780_L_H
