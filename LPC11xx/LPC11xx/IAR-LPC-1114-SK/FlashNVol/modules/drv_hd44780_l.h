/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
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
 *    3. Date        : November 27, 2009
 *       Author      : Stanimir Bonev
 *       Description : Port for IAR-LPC1114-SK
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <stdint.h>
#include "LPC11xx.h"
#include "drv_hd44780_cnfg.h"


#ifndef  __DRV_HD44780_L_H
#define  __DRV_HD44780_L_H

#define LCD_RS                    (1UL << 8)
#define LCD_RS_DIR                LPC_GPIO2->DIR
#define LCD_RS_PORT               (*((volatile unsigned int *)(0x50020000+ (LCD_RS<<2))))

#define LCD_E                     (1UL << 9)
#define LCD_E_DIR                 LPC_GPIO2->DIR
#define LCD_E_PORT                (*((volatile unsigned int *)(0x50020000+ (LCD_E<<2))))

#define LCD_RW                    (1UL << 10)
#define LCD_RW_DIR                LPC_GPIO2->DIR
#define LCD_RW_PORT               (*((volatile unsigned int *)(0x50020000+ (LCD_RW<<2))))


#define LCD_DATA_2_MASK           0x3UL
#define LCD_DATA_2_SHIFT          6
#define LDC_DATA_2_DIR            LPC_GPIO2->DIR
#define LCD_DATA_2_PORT           (*((volatile unsigned int *)(0x50020000+((LCD_DATA_2_MASK<<LCD_DATA_2_SHIFT)<<2))))

#define LCD_DATA_3_MASK           0x3UL
#define LCD_DATA_3_SHIFT          4
#define LDC_DATA_3_DIR            LPC_GPIO3->DIR
#define LCD_DATA_3_PORT           (*((volatile unsigned int *)(0x50030000+((LCD_DATA_3_MASK<<LCD_DATA_3_SHIFT)<<2))))

#define LCD_E_HIGH()              LCD_E_PORT  =  LCD_E
#define LCD_E_LOW()               LCD_E_PORT  = ~LCD_E

#define LCD_RS_HIGH()             LCD_RS_PORT =  LCD_RS
#define LCD_RS_LOW()              LCD_RS_PORT = ~LCD_RS

#define LCD_RW_HIGH()             LCD_RW_PORT =  LCD_RW
#define LCD_RW_LOW()              LCD_RW_PORT = ~LCD_RW

/* Delay macro */
#define HD44780_BUS_DLY()         for(volatile int dly = 10;dly;--dly)

void HD44780_IO_Init (void);
void HD44780SetPD (void);
void HD44780SetPU (void);
void HD44780WrIO (uint32_t);
#if HD4780_WR > 0
uint8_t HD44780RdIO (void);
#endif // HD4780_WR > 0

#endif  /* __DRV_HD44780_L_H */
