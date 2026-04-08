/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2011
 **
 **    $Revision: 16 $
 **
 ***************************************************************************/
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

/* OSC [MHz] */
#define FOSC1               24MHZ
#define FOSC2               24576KHZ

/* Core clk [MHz]  532 MHz */
#define MCUPLL              (532MHZ)
#define MAX_CLK             (133MHZ)
#define IPG_CLK             (66500KHZ)
#define HSP_CLK             (133MHZ)
#define CKIL                (32KHZ)

/* CPLD */
#define CPLD_VER            (*(unsigned short *)0xB6020050)
#define BOAR_REV()          ( CPLD_VER    &0xFF)
#define CPLD_REV()          ((CPLD_VER>>8)&0xFF)

#define LED_CTRL            (*(unsigned short *)0xB6020000)

#define LED1                (1UL<<0)
#define LED2                (1UL<<1)
#define LED3                (1UL<<2)
#define LED4                (1UL<<3)
#define LED5                (1UL<<4)
#define LED6                (1UL<<5)
#define LED7                (1UL<<6)
#define LED8                (1UL<<7)

#define LED_ON(led)         (LED_CTRL |= (led))
#define LED_OFF(led)        (LED_CTRL &= ~(led))

#define LED_TOGGLE(led)     (LED_CTRL ^= led)


#endif /* __BOARD_H */
