/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2006
 **
 **    $Revision: 34 $
 **
 ***************************************************************************/
#ifndef __BOARD_H
#define __BOARD_H

// OSCs [Hz]
#define MAIN_OSC_FREQ	       (11289600UL)
#define LF_OSC_FREQ		       (32768UL)

// PLL
#define PRE_DIV               1
#define FB_DIV                27

#define PLL_FREQ             ((MAIN_OSC_FREQ*FB_DIV)/PRE_DIV)

// FCLK
#define FCLK_DIV              4
#define FCLK_FREQ            (PLL_FREQ)/(FCLK_DIV)

// HCLK
#define HCLK_DIV              6
#define HCLK_FREQ            (PLL_FREQ)/(HCLK_DIV)

// HCLK to SDCLK Ratio
#define HCLK2SDCLK            0

// Tick
#define TICK_PER_SECOND       1000

// UART Baud rate
#define UART_BAUD_115200      115200UL

// Memory map
#define SMEM_CS0_BASE_ADDR  (0x40000000UL) // nCS0 static memory base address
#define SMEM_CS1_BASE_ADDR  (0x44000000UL) // nCS1 static memory base address
#define SMEM_CS2_BASE_ADDR  (0x48000000UL) // nCS2 static memory base address
#define SMEM_CS3_BASE_ADDR  (0x4C000000UL) // nCS3 static memory base address
#define DMEM_CS0_BASE_ADDR  (0x20000000UL) // nCS0 dynamic memory base address
#define DMEM_CS1_BASE_ADDR  (0x22000000UL) // nCS1 dynamic memory base address

// MT48LC16A2-75 SDRAM
#define SDRAM_TRP            (20)
#define SDRAM_TRAS           (60)
#define SDRAM_TREX           (75)
#define SDRAM_TARP           (100)
#define SDRAM_TDAL           (100)
#define SDRAM_TWR            (30)
#define SDRAM_TRC            (70)
#define SDRAM_TRFC           (70)
#define SDRAM_TXSR           (80)
#define SDRAM_TRRD           (20)
#define SDRAM_TMRD           (40)
#define SDRAM_OPER_REFRESH   (15630)

// 28F128-PTL12 NOR Flash
#define FLASH_WAIT_WEN       (40)
#define FLASH_WAIT_OEN       (100)
#define FLASH_WAIT_RD        (150)
#define FLASH_WAIT_PAGE      (40)
#define FLASH_WAIT_WR        (100)
#define FLASH_WAIT_TURN      (40)

// CPLD
#define CPLD_WAIT_WEN        (40)
#define CPLD_WAIT_OEN        (40)
#define CPLD_WAIT_RD         (100)
#define CPLD_WAIT_WR         (100)
#define CPLD_WAIT_TURN       (40)

// Leds
#define GPIO_LED             (1UL << 0)
#define STAT_LED1            (1UL << 1)
#define STAT_LED0            (1UL << 2)

#define CPLD_LED_IO          (*(unsigned char*)0x4CB00000)
#define CPLD_GPIO_DIR        (*(unsigned char*)0x4CD00000)
#define CPLD_GPIO_IO         (*(unsigned char*)0x4CC00000)

#define GPIO_LED_ON()         CPLD_LED_IO &= ~GPIO_LED
#define GPIO_LED_OFF()        CPLD_LED_IO |=  GPIO_LED

#define STAT_LED0_ON()        CPLD_LED_IO &= ~STAT_LED0
#define STAT_LED0_OFF()       CPLD_LED_IO |=  STAT_LED0

#define STAT_LED1_ON()        CPLD_LED_IO &= ~STAT_LED1
#define STAT_LED1_OFF()       CPLD_LED_IO |=  STAT_LED1

#endif /* __BOARD_H */
