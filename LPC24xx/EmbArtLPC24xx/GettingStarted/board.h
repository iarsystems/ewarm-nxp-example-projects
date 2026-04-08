/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2007
 **
 **    $Revision: 28 $
 **
 ***************************************************************************/
#include <intrinsics.h>
#include "arm_comm.h"
#include <nxp/iolpc2468.h>

#ifndef __BOARD_H
#define __BOARD_H

#define I_RC_OSC_FREQ   (4MHZ)
#define MAIN_OSC_FREQ   (12MHZ)
#define RTC_OSC_FREQ    (32768UL)
#define SYS_FREQ        (48MHZ)

#if defined(EA_LPC_24XX)

// LCD backlight
#define LCD_BLT_MASK  (1UL<<18)
#define LCD_BLT_DIR   IO1DIR
#define LCD_BLT_FDIR  FIO1DIR
#define LCD_BLT_SET   IO1SET
#define LCD_BLT_FSET  FIO1SET
#define LCD_BLT_CLR   IO1CLR
#define LCD_BLT_FCLR  FIO1CLR
#define LCD_BLT_IO    IO1PIN
#define LCD_BLT_FIO   FIO1PIN

#else
#error Define type of the board
#endif

// PCLK offset
#define WDT_PCLK_OFFSET     0
#define TIMER0_PCLK_OFFSET  2
#define TIMER1_PCLK_OFFSET  4
#define UART0_PCLK_OFFSET   6
#define UART1_PCLK_OFFSET   8
#define PWM0_PCLK_OFFSET    10
#define PWM1_PCLK_OFFSET    12
#define I2C0_PCLK_OFFSET    14
#define SPI_PCLK_OFFSET     16
#define RTC_PCLK_OFFSET     18
#define SSP1_PCLK_OFFSET    20
#define DAC_PCLK_OFFSET     22
#define ADC_PCLK_OFFSET     24
#define CAN1_PCLK_OFFSET    26
#define CAN2_PCLK_OFFSET    28
#define ACF_PCLK_OFFSET     30
#define BAT_RAM_PCLK_OFFSET 32
#define GPIO_PCLK_OFFSET    34
#define PCB_PCLK_OFFSET     36
#define I2C1_PCLK_OFFSET    38
//#define                   40
#define SSP0_PCLK_OFFSET    42
#define TIMER2_PCLK_OFFSET  44
#define TIMER3_PCLK_OFFSET  46
#define UART2_PCLK_OFFSET   48
#define UART3_PCLK_OFFSET   50
#define I2C2_PCLK_OFFSET    52
#define I2S_PCLK_OFFSET     54
#define MCI_PCLK_OFFSET     56
//#define                   58
#define PCLK_PCLK_OFFSET    60
//#define                   62

#endif /* __BOARD_H */
