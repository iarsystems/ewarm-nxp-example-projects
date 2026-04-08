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

// OSC [Hz]
#define MAIN_OSC_CLK		      (20000000UL)
#define HCLK_DIV              2
#define HCLK_FREQ             (MAIN_OSC_CLK * 7)/(HCLK_DIV*2)

// SRAM Base Address
#define EXT_FLASH 		        ((unsigned int *)0x40000000)
#define EXT_VIDEO 		        ((unsigned int *)0x44000000)
#define EXT_SRAM 		          ((unsigned int *)0x48000000)
#define EXT_CPLD 		          ((unsigned int *)0x4C000000)

// Tick
#define TICK_PER_SECOND       1000

// Leds
#define USR_LED               (1UL << 0)
#define STAT_LED0             (1UL << 1)
#define STAT_LED1             (1UL << 2)

#define USR_LED_ON()         *(unsigned char*)0x4C580000 = 0
#define USR_LED_OFF()        *(unsigned char*)0x4C580000 = USR_LED

#define STAT_LED0_ON()       GPIO_PFDR &= ~STAT_LED0
#define STAT_LED0_OFF()      GPIO_PFDR |=  STAT_LED0

#define STAT_LED1_ON()       GPIO_PFDR &= ~STAT_LED1
#define STAT_LED1_OFF()      GPIO_PFDR |=  STAT_LED1

#endif /* __BOARD_H */
