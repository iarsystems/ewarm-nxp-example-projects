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
#include "arm_comm.h"
#include <NXP/iomcimx27.h>
#ifndef __BOARD_H
#define __BOARD_H

/* OSC [MHz] */
#define FOSC1               26MHZ
#define FOSC2               327680HZ

/* Core clk [MHz]  266 MHz */
#define MPLL_CLK            266MHZ
#define FCLK                266MHZ
#define HCLK                133MHZ
#define SPLL_CLK            300MHZ

/* GPT1 Tick per second*/
#define GPT1_TICK_PER_SEC   100

#define LED1_ON()           (PTC_DR_bit.PIN25 = 0)
#define LED1_OFF()          (PTC_DR_bit.PIN25 = 1)

#define LED2_ON()           (PTC_DR_bit.PIN26 = 0)
#define LED2_OFF()          (PTC_DR_bit.PIN26 = 1)

#endif /* __BOARD_H */
