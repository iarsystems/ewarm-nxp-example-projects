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

#define I_RC_OSC_FREQ   (12MHZ)
#define MAIN_OSC_FREQ   (12MHZ)
#define WDT_OSC_FREQ    (0MHZ)

#define PIN_LOW(pin)   pin = 0;
#define PIN_HIGH(pin)  pin = 0x7FF;

#if defined(IAR_LPC_1114_SK)

// LED1
#define LED1_MASK   (1UL<<0)
#define LED1_DIR    GPIO3DIR
#define LED1_PIN   (*((volatile unsigned int *)(0x50030000+(LED1_MASK<<2))))
// LED2
#define LED2_MASK   (1UL<<9)
#define LED2_DIR    GPIO1DIR
#define LED2_PIN   (*((volatile unsigned int *)(0x50010000+(LED2_MASK<<2))))
// LED3
#define LED3_MASK   (1UL<<1)
#define LED3_DIR    GPIO3DIR
#define LED3_PIN   (*((volatile unsigned int *)(0x50030000+(LED3_MASK<<2))))

#define LED_OFF(pin) PIN_HIGH(pin)
#define LED_ON(pin)  PIN_LOW(pin)
// Buttons
//B1
#define B1_MASK            (1UL<<7)
#define B1_DIR             GPIO0DIR
#define B1_PIN            (*((volatile unsigned int *)(0x50000000+ (B1_MASK<<2))))
//B2
#define B2_MASK            (1UL<<4)
#define B2_DIR             GPIO1DIR
#define B2_PIN            (*((volatile unsigned int *)(0x50010000+ (B2_MASK<<2))))

// Analog trim
#define ANALOG_TRIM_CHANNEL   6
#define ANALOG_TRIM_IOCON     IOCON_PIO1_10

//TEMP
#define TEMP_CHANNEL          7
#define TEMP_PIN_IOCON        IOCON_PIO1_11

//Buzzer
#define BUZZER_MASK   (1UL<<8)
#define BUZZER_DIR    GPIO0DIR
#define BUZZER_PIN   (*((volatile unsigned int *)(0x50000000+(BUZZER_MASK<<2))))
//UART
#define UART_TX_MASK   (1UL<<7)
#define UART_TX_DIR    GPIO1DIR
#define UART_TX_PIN   (*((volatile unsigned int *)(0x50010000+(UART_TX_MASK<<2))))
#define UART_TX_IOCON        IOCON_PIO1_7


#define UART_RX_MASK   (1UL<<6)
#define UART_RX_DIR    GPIO1DIR
#define UART_RX_PIN   (*((volatile unsigned int *)(0x50010000+(UART_RX_MASK<<2))))
#define UART_RX_IOCON        IOCON_PIO1_6

#else
#error define type of the board
#endif

// PCLK offset

#endif /* __BOARD_H */
