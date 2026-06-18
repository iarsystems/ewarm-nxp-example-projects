/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : drv_hd44780_l.h
 *    Description : STM32 HD44780 low level I/O function driver
 *
 *    History :
 *    1. Date        : February 5, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : April 18, 2011
 *       Author      : Stanimir Bonev
 *       Description : Port for IAR-LPC11U14-SK
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#ifndef  __DRV_HD44780_L_H
#define  __DRV_HD44780_L_H

#define LCD_RS                    (1UL << 24)
#define LCD_RS_DIR                P0DIR
#define LCD_RS_PORT               P0PORT

#define LCD_E                     (1UL << 26)
#define LCD_E_DIR                 P0DIR
#define LCD_E_PORT                P0PORT

#define LCD_RW                    (1UL << 25)
#define LCD_RW_DIR                P0DIR
#define LCD_RW_PORT               P0PORT


#define LCD_DATA_MASK             0xFUL
#define LCD_DATA_SHIFT            6
#define LDC_DATA_DIR              P1DIR
#define LCD_DATA_PORT_IN          P1PORT
#define LCD_DATA_PORT_SET         P1SET
#define LCD_DATA_PORT_CLR         P1CLR

#define LCD_E_HIGH()              LCD_E_PORT  |=  LCD_E;
#define LCD_E_LOW()               LCD_E_PORT  &=  ~LCD_E;

#define LCD_RS_HIGH()             LCD_RS_PORT |=  LCD_RS;
#define LCD_RS_LOW()              LCD_RS_PORT &=  ~LCD_RS;

#define LCD_RW_HIGH()             LCD_RW_PORT |=  LCD_RW;
#define LCD_RW_LOW()              LCD_RW_PORT &=  ~LCD_RW;

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
