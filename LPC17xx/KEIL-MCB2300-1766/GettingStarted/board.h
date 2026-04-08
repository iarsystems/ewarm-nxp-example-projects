/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2005
 **
 **    $Revision: 28 $
 **
 ***************************************************************************/
#include <intrinsics.h>
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

#define I_RC_OSC_FREQ   (4MHZ)
#define MAIN_OSC_FREQ   (12MHZ)
#define RTC_OSC_FREQ    (32768UL)

#if defined(KEIL_MCB2300)

// LEDs
#define LEDS_MASK           (0xFFUL<<0)
#define LEDS_DIR            FIO2DIR
#define LEDS_SET            FIO2SET
#define LEDS_CLR            FIO2CLR
#define LEDS_FIO            FIO2PIN

// Buttons
#define B1_MASK             (1UL<<10)
#define B1_DIR              FIO2DIR
#define B1_FIO              FIO2PIN

// Analog trim
#define ANALOG_TRIM_CHANNEL 0
#define ANALOG_TRIM_CHANNEL_SEL   PINSEL1_bit.P0_23

#else
#error define type of the board
#endif

// PCLK offset
#define WDT_PCLK_OFFSET     0
#define TIMER0_PCLK_OFFSET  2
#define TIMER1_PCLK_OFFSET  4
#define UART0_PCLK_OFFSET   6
#define UART1_PCLK_OFFSET   8
#define PWM1_PCLK_OFFSET    12
#define I2C0_PCLK_OFFSET    14
#define SPI_PCLK_OFFSET     16
#define SSP1_PCLK_OFFSET    20
#define DAC_PCLK_OFFSET     22
#define ADC_PCLK_OFFSET     24
#define CAN1_PCLK_OFFSET    26
#define CAN2_PCLK_OFFSET    28
#define ACF_PCLK_OFFSET     30
#define PCLK_QEI_OFFSET     32
#define PCB_PCLK_OFFSET     36
#define I2C1_PCLK_OFFSET    38
#define SSP0_PCLK_OFFSET    42
#define TIMER2_PCLK_OFFSET  44
#define TIMER3_PCLK_OFFSET  46
#define UART2_PCLK_OFFSET   48
#define UART3_PCLK_OFFSET   50
#define I2C2_PCLK_OFFSET    52
#define PCLK_I2S_OFFSET     54
#define PCLK_RIT_OFFSET     58
#define PCLK_SYSCON_OFFSET  60
#define PCLK_MC_OFFSET      62

#endif /* __BOARD_H */
