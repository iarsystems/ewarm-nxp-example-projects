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
#include <nxp/iolpc11a1x.h>
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

#define I_RC_OSC_FREQ   (12MHZ)
#define XTAL_OSC_FREQ   (25MHZ)
#define WDT_OSC_FREQ    (0MHZ) 


#if defined(IAR_LPC_11A14_SK)

// LEDS
#define LED1_MASK   (1UL<<16)
#define LED1_DIR    P0DIR
#define LED1_PORT   P0PORT   

#define LED2_MASK   (1UL<<17)
#define LED2_DIR    P0DIR
#define LED2_PORT   P0PORT   

#define LED3_MASK   (1UL<<18)
#define LED3_DIR    P0DIR
#define LED3_PORT   P0PORT   

#define LED4_MASK   (1UL<<19)
#define LED4_DIR    P0DIR
#define LED4_PORT   P0PORT   

#define LED5_MASK   (1UL<<20)
#define LED5_DIR    P0DIR
#define LED5_PORT   P0PORT   

#define LED6_MASK   (1UL<<21)
#define LED6_DIR    P0DIR
#define LED6_PORT   P0PORT   

#define LED7_MASK   (1UL<<22)
#define LED7_DIR    P0DIR
#define LED7_PORT   P0PORT   

#define LED8_MASK   (1UL<<23)
#define LED8_DIR    P0DIR
#define LED8_PORT   P0PORT   

#define LED_SET     P0SET
#define LET_CLR     P0CLR
#define LED_TOGGLE  P0NOT

#define LEDS_MASK   (255UL<<16);
#define LEDS_PORT   P0PORT
#define LEDS_DIR    P0DIR
#define LEDS_SET    P0SET
#define LEDS_CLR    P0CLR

#define LEDS_OFF()                    LEDS_SET = LEDS_MASK; 
#define LEDS_ON(mask)                 LEDS_CLR = (mask<<16); 

#define LED_ON(port,mask)             port &= ~mask;
#define LED_OFF(port,mask)            port |= mask;

// Buttons
//USER
#define USER_MASK           (1UL<<31)
#define USER_DIR            P0DIR
#define USER_PORT           P0PORT
//WAKE-UP
#define WAKE_UP_MASK        (1UL<<11)
#define WAKE_UP_DIR         P0DIR
#define WAKE_UP_PORT        P0PORT
//ISP_E
#define ISP_E_MASK          (1UL<<1)
#define ISP_E_DIR           P0DIR
#define ISP_E_PORT          P0PORT

// Analog trim
#define ANALOG_TRIM_CHANNEL   7
#define ANALOG_TRIM_IOCON     IOCON_P0_15

// Accelerometer
#define ACC_INT_E_MASK     (1UL<<27)
#define ACC_INT_E_PORT     P0PORT
#define ACC_INT_E_DIR      P0DIR

//Buzzer
#define BUZZER_MASK   (1UL<<4)
#define BUZZER_DIR    P0DIR
#define BUZZER_PORT   P0PORT

//DAC
#define DAC_MASK   (1UL<<4)
#define DAC_DIR    P0DIR
#define DAC_PORT   P0PORT
#define DAC_IOCON  IOCON_P0_4

//RS232
#define RS232_TX_MASK      (1UL<<5)
#define RS232_TX_DIR       P1DIR
#define RS232_TX_PORT      P1PORT
#define RS232_TX_IOCON     IOCON_P1_5

#define RS232_RX_MASK      (1UL<<4)
#define RS232_RX_DIR       P1DIR
#define RS232_RX_PORT      P1PORT
#define RS232_RX_IOCON     IOCON_P1_4


#else
#error define type of the board
#endif

#endif /* __BOARD_H */
