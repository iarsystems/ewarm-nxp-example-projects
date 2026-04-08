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


#if defined(IAR_LPC1347_SK)
#define MOSC   (12MHZ)
/*UART DEF*/
#define UART_TX_MASK       (1UL<<19)
#define UART_TX_IOCON      IOCON_PIO0_19
#define UART_TX_IOCON_INIT 0x81 

#define UART_RX_MASK       (1UL<<18)
#define UART_RX_IOCON      IOCON_PIO0_18
#define UART_RX_IOCON_INIT 0x81 

/*Buzzer*/
#define BUZZER_MASK        (1UL<<21)
#define BUZZER_IOCON        IOCON_PIO0_21
#define BUZZER_IOCON_INIT   0x80
#define BUZZER_DIR         GPIO_DIR0
#define BUZZER_TGL_PORT    GPIO_NOT0

/*LEDs*/
#define USB_CONNECT_LED_MASK   (1UL<<6)
#define USB_CONNECT_LED_DIR    GPIO_DIR0
#define USB_CONNECT_LED        GPIO_B6

#define LED1_MASK   (1UL<<25)
#define LED1_DIR    GPIO_DIR1
#define LED1        GPIO_B57

#define LED2_MASK   (1UL<<24)
#define LED2_DIR    GPIO_DIR1
#define LED2        GPIO_B56

#define LED_ON(led) led = 0;
#define LED_OFF(led) led = 1;

/* Buttons*/
#define BUT1_MASK      (1UL<<7)
#define BUT1_DIR       GPIO_DIR0
#define BUT1           GPIO_B7

#define BUT2_MASK      (1UL<<16)
#define BUT2_DIR       GPIO_DIR0
#define BUT2           GPIO_B16

/*Analog trim*/
#define ANALOG_TRIM_CHANNEL   6
#define ANALOG_TRIM_IOCON     IOCON_PIO0_22

/*TEMP*/
#define TEMP_CHANNEL          7
#define TEMP_PIN_IOCON        IOCON_PIO0_23

/*USB*/
#define USB_CONNECT_MASK        (1UL<<6)
#define USB_CONNECT_IOCON       IOCON_PIO0_6
#define USB_CONNECT_IOCON_INIT  0x81 

#define USB_VBUS_MASK           (1UL<<3)
#define USB_VBUS_IOCON          IOCON_PIO0_3
#define USB_VBUS_IOCON_INIT     0x81 

#else
#error define type of the board
#endif

#endif /* __BOARD_H */
