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

#if defined(BB_LPC11U37)

// LED1
#define LED1_MASK   (1UL<<22)
#define LED1_DIR    GPIO_DIR0
#define LED1_PORT   GPIO_B22

#define LED2_MASK   (1UL<<23)
#define LED2_DIR    GPIO_DIR0
#define LED2_PORT   GPIO_B23

#define LED_ON(port,mask)  port = 0;
#define LED_OFF(port,mask) port = 1;

// Buttons
//EXTERNAL
#define USER_MASK           (1UL<<7)
#define USER_DIR            GPIO_DIR0
#define USER_PORT           GPIO_B7
//WAKE-UP
#define WAKE_UP_MASK        (1UL<<16)
#define WAKE_UP_DIR         GPIO_DIR0
#define WAKE_UP_PORT        GPIO_B16

// Analog trim
#define ANALOG_TRIM_CHANNEL   0
#define ANALOG_TRIM_IOCON     IOCON_TDI_PIO0_11

//Buzzer
#define BUZZER_MASK   (1UL<<17)
#define BUZZER_DIR    GPIO_DIR0
#define BUZZER_PORT   GPIO_B17

//UART
#define UART_TX_MASK   (1UL<<19)
#define UART_TX_DIR    GPIO_DIR0
#define UART_TX_PORT   GPIO_B19
#define UART_TX_IOCON  IOCON_PIO0_19


#define UART_RX_MASK   (1UL<<18)
#define UART_RX_DIR    GPIO_DIR0
#define UART_RX_PORT   GPIO_B18
#define UART_RX_IOCON  IOCON_PIO0_18

#else
#error define type of the board
#endif

#endif /* __BOARD_H */
