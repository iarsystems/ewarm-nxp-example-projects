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
#define MAIN_OSC_FREQ	       (14745600UL)
#define LF_OSC_FREQ		       (32768UL)

// PLL
#define PRE_DIV               14
#define MAIN_DIV1             12
#define MAIN_DIV2             29
#define POST_DIV              2

#define PLL_FREQ             (((unsigned long long)MAIN_OSC_FREQ*(MAIN_DIV1+2)*(MAIN_DIV2+2))/((PRE_DIV+2)*(POST_DIV)))

// FCLK
#define FCLK_FREQ            (PLL_FREQ)

// HCLK
#define HCLK_DIV              2
#define HCLK_FREQ            (FCLK_FREQ/HCLK_DIV)

// PCLK
#define PCLK_DIV              2
#define PCLK_FREQ            (FCLK_FREQ/PCLK_DIV)

// Timer 1,2 clk
#define TIMR12_SLOW_CLK      (2 KHZ)
#define TIMR12_FAST_CLK      (508 KHZ)

// Timer 3 clk
#define TIMR3_INTR_CLK       (3.6864 MHZ)

// Tick
#define TICK_PER_SECOND       1000

// Touchscreen measure per second
#define TC_MEASURE_PER_SECOND 500

// UART Baud rate
#define UART_BAUD_115200      115200UL

// Memory map
#define SMEM_CS0_BASE_ADDR  (0x00000000UL) // nCS0 static memory base address
#define SMEM_CS1_BASE_ADDR  (0x10000000UL) // nCS1 static memory base address
#define SMEM_CS2_BASE_ADDR  (0x20000000UL) // nCS2 static memory base address
#define SMEM_CS3_BASE_ADDR  (0x30000000UL) // nCS3 static memory base address
#define SMEM_CS6_BASE_ADDR  (0x60000000UL) // nCS6 static memory base address
#define SMEM_CS7_BASE_ADDR  (0x70000000UL) // nCS7 static memory base address
#define DMEM_CS0_BASE_ADDR  (0xC0000000UL) // nCS0 dynamic memory base address
#define DMEM_CS1_BASE_ADDR  (0xD0000000UL) // nCS1 dynamic memory base address
#define DMEM_CS2_BASE_ADDR  (0xE0000000UL) // nCS2 dynamic memory base address

// MT48LCxx-75 SDRAM
#define SDRAM_OPER_REFRESH   (7812)

// NOR Flash
#define FLASH_WAIT_RD        (150)
#define FLASH_WAIT_PAGE      (40)
#define FLASH_WAIT_TURN      (40)

// CPLD
#define CPLD_WAIT_RD         (100)
#define CPLD_WAIT_TURN       (40)

// Leds
#define GPIO_LED             (1UL << 0)
#define STAT_LED1            (1UL << 1)
#define STAT_LED0            (1UL << 2)

#define CPLD_SPI_DATA        (*(volatile unsigned char*)0x70600000)
#define CPLD_SPI_CTRL        (*(volatile unsigned char*)0x70800000)

#define SPI_SCCS             (1UL << 0)
#define SPI_STCS             (1UL << 1)
#define SPI_SPRW             (1UL << 2)
#define SPI_SPDN             (1UL << 3)
#define SPI_SPST             (1UL << 4)
#define SPI_SPLD             (1UL << 5)

#define CPLD_INTR_MASK       (*(volatile unsigned char*)0x70C00000)
#define CPLD_LED_IO          (*(volatile unsigned char*)0x71600000)

#define GPIO_LED_ON()         CPLD_LED_IO &= ~GPIO_LED
#define GPIO_LED_OFF()        CPLD_LED_IO |=  GPIO_LED

#define STAT_LED0_ON()        CPLD_LED_IO &= ~STAT_LED0
#define STAT_LED0_OFF()       CPLD_LED_IO |=  STAT_LED0

#define STAT_LED1_ON()        CPLD_LED_IO &= ~STAT_LED1
#define STAT_LED1_OFF()       CPLD_LED_IO |=  STAT_LED1

#endif /* __BOARD_H */
