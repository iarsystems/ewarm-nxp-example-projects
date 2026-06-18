/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2012
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

#if defined(IAR_LPC_11U24_SK)

// LED1
#define LED1_MASK   (1UL<<15)
#define LED1_DIR    GPIO_DIR1
#define LED1_PORT   GPIO_B47

#define LED2_MASK   (1UL<<21)
#define LED2_DIR    GPIO_DIR0
#define LED2_PORT   GPIO_B21

#define LED_ON(port,mask)  port = 0;
#define LED_OFF(port,mask) port = 1;

// Buttons
//USER
#define USER_MASK           (1UL<<7)
#define USER_DIR            GPIO_DIR0
#define USER_PORT           GPIO_B7
//WAKE-UP
#define WAKE_UP_MASK        (1UL<<16)
#define WAKE_UP_DIR         GPIO_DIR0
#define WAKE_UP_PORT        GPIO_B16

// Analog trim
#define ANALOG_TRIM_CHANNEL   6
#define ANALOG_TRIM_IOCON     IOCON_PIO0_22

//TEMP
#define TEMP_CHANNEL          7
#define TEMP_PIN_IOCON        IOCON_PIO0_23

//Buzzer
#define BUZZER_MASK   (1UL<<8)
#define BUZZER_DIR    GPIO_DIR0
#define BUZZER_PORT   GPIO_B8

//UART
#define UART_TX_MASK   (1UL<<13)
#define UART_TX_DIR    GPIO_DIR1
#define UART_TX_PORT   GPIO_B45
#define UART_TX_IOCON  IOCON_PIO1_13


#define UART_RX_MASK   (1UL<<14)
#define UART_RX_DIR    GPIO_DIR1
#define UART_RX_PORT   GPIO_B46
#define UART_RX_IOCON  IOCON_PIO1_14

#else
#error define type of the board
#endif

// PCLK offset

// PCLK offset

#endif /* __BOARD_H */
