/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2007
 **
 **    $Revision: 5880 $
 **
 ***************************************************************************/
#include <NXP/iomc13260.h>
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

/* OSC [MHz] */
#define FOSC1               48MHZ
#define FOSC2               327680HZ

/* Core clk [MHz]  144 MHz */
#define DPLLCLK             288MHZ
#define APCLK               144MHZ
#define AHBCLK              72MHZ
#define IPCLK               36MHZ

/* GPT1 Tick per second*/
#define GPT_TICK_PER_SEC    5

#define LED0                (1UL<< 21)
#define LED1                (1UL<< 22)

#define LED_MASK            (LED0 | LED1)
#define LED_PORT            GPIO_DR
#define LED_DIR             GPIO_GDIR

#define LED0_ON()           (LED_PORT |=  LED0 )
#define LED0_OFF()          (LED_PORT &= ~LED0 )
#define LED0_TOGGLE()       (LED_PORT ^=  LED0 )

#define LED1_ON()           (LED_PORT |=  LED1 )
#define LED1_OFF()          (LED_PORT &= ~LED1 )
#define LED1_TOGGLE()       (LED_PORT ^=  LED1 )

#endif /* __BOARD_H */
