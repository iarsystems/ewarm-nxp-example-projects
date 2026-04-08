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

#define NONPROT 0xFFFFFFFF
#define CRP1  0x12345678
#define CRP2  0x87654321
/*If CRP3 is selected, no future factory testing can be
performed on the device*/
#define CRP3  0x43218765
#define CRP   NONPROT

#define PIN_LOW(pin)   pin = 0;
#define PIN_HIGH(pin)  pin = 1;

#if defined(IAR_LPC_122X_SK)

// LED1
#define LED1_MASK   (1UL<<5)
#define LED1_DIR    GPIO1DIR
#define LED1_OUT    GPIO1OUT_bit.P1_5
#define LED1_PIN    GPIO1PIN_bit.P1_5
// LED2
#define LED2_MASK   (1UL<<4)
#define LED2_DIR    GPIO1DIR
#define LED2_OUT    GPIO1OUT_bit.P1_4
#define LED2_PIN    GPIO1PIN_bit.P1_4

#define LED_OFF(led) PIN_HIGH(led)
#define LED_ON(led)  PIN_LOW(led)

// Buttons
//B1
#define USER_MASK        (1UL<<1)
#define USER_DIR          GPIO1DIR
#define USER_PIN          GPIO1PIN_bit.P1_1
//B2
#define WAKE_UP_MASK      (1UL<<3)
#define WAKE_UP_DIR        GPIO1DIR
#define WAKE_UP_PIN        GPIO1PIN_bit.P1_3

// Analog trim
#define ANALOG_TRIM_CHANNEL   2
#define ANALOG_TRIM_IOCON     IOCON_PIO1_0
#define ANALOG_TRIM_RESULT    AD0DR2_bit.RESULT

//Buzzer
#define BUZZER_MASK   (1UL<<6)
#define BUZZER_DIR    GPIO1DIR
#define BUZZER_OUT    GPIO1OUT_bit.P1_6
//UART
#define UART_TX_MASK    (1UL<<2)
#define UART_TX_DIR     GPIO0DIR
#define UART_TX_OUT     GPIO0OUT_bit.P0_2
#define UART_TX_IOCON   IOCON_PIO0_2

#define UART_RX_MASK    (1UL<<1)
#define UART_RX_DIR     GPIO0DIR
#define UART_RX_PIN     GPIO0PIN_bit.P0_1
#define UART_RX_IOCON   IOCON_PIO0_1

#define UART_RTS_MASK   (1UL<<0)
#define UART_RTS_DIR    GPIO0DIR
#define UART_RTS_OUT    GPIO0OUT_bit.P0_0
#define UART_RTS_IOCON  IOCON_PIO0_0

#define UART_DTR_MASK   (1UL<<3)
#define UART_DTR_DIR    GPIO0DIR
#define UART_DTR_OUT    GPIO0OUT_bit.P0_3
#define UART_DTR_IOCON  IOCON_PIO0_3

#define UART_DSR_MASK   (1UL<<4)
#define UART_DSR_DIR    GPIO0DIR
#define UART_DSR_PIN    GPIO0PIN_bit.P0_4
#define UART_DSR_IOCON  IOCON_PIO0_4

#define UART_CD_MASK    (1UL<<5)
#define UART_CD_DIR     GPIO0DIR
#define UART_CD_PIN     GPIO0PIN_bit.P0_5
#define UART_CD_IOCON   IOCON_PIO0_5

#define UART_RI_MASK    (1UL<<6)
#define UART_RI_DIR     GPIO0DIR
#define UART_RI_PIN     GPIO0PIN_bit.P0_6
#define UART_RI_IOCON   IOCON_PIO0_6

#define UART_CTS_MASK    (1UL<<7)
#define UART_CTS_DIR     GPIO0DIR
#define UART_CTS_PIN     GPIO0PIN_bit.P0_7
#define UART_CTS_IOCON   IOCON_PIO0_7

#else
#error define type of the board
#endif

// PCLK offset

#endif /* __BOARD_H */
