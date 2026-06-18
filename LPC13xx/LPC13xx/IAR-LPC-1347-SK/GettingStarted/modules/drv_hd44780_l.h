/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : drv_hd44780_l.h
 *    Description : STM32 HD44780 low level I/O function driver
 *
 *    History :
 *    1. Date        : February 5, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : September 7, 2009
 *       Author      : Stoyan Choynev
 *       Description : Port for IAR-LPC1343-SK 
 *    3. Date        : January 30, 2012
 *       Author      : Atanas Uzunov
 *       Description : Port for IAR-LPC1347-SK 
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#ifndef  __DRV_HD44780_L_H
#define  __DRV_HD44780_L_H

#define LCD_RS                    (1UL << 4)
#define LCD_RS_DIR                GPIO_DIR1
#define LCD_RS_PORT               GPIO_B36

#define LCD_E                     (1UL << 5)
#define LCD_E_DIR                 GPIO_DIR1
#define LCD_E_PORT                GPIO_B37

#define LCD_RW                    (1UL << 7)
#define LCD_RW_DIR                GPIO_DIR1
#define LCD_RW_PORT               GPIO_B39

#define LCD_DATA_MASK             0xFUL
#define LCD_DATA_SHIFT            0
#define LDC_DATA_DIR              GPIO_DIR1
#define LCD_DATA_PORT             GPIO_MPIN1
#define LCD_DATA_PORT_MASK        GPIO_MASK1     

#define LCD_E_HIGH()              {LCD_E_PORT = 1;}
#define LCD_E_LOW()               {LCD_E_PORT = 0;}

#define LCD_RS_HIGH()             {LCD_RS_PORT = 1;}
#define LCD_RS_LOW()              {LCD_RS_PORT = 0;}

#define LCD_RW_HIGH()             {LCD_RW_PORT = 1;}
#define LCD_RW_LOW()              {LCD_RW_PORT = 0;}

/* Delay macro */
#define HD44780_BUS_DLY()         for(volatile int dly = 10;dly;--dly)

void HD44780_IO_Init (void);
void HD44780SetPD (void);
void HD44780SetPU (void);
void HD44780WrIO (Int32U);
#if HD4780_WR > 0
Int8U HD44780RdIO (void);
#endif // HD4780_WR > 0

#endif  /* __DRV_HD44780_L_H */
