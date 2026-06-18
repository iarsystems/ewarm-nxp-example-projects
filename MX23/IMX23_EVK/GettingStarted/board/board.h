/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2011
 **
 **    $Revision: 5880 $
 **
 ***************************************************************************/
#include <NXP/iomcimx23.h>
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

/* OSC [MHz] */
#define FOSC1               24MHZ
#define FOSC2               32768HZ

/* Core clk [MHz]  266 MHz */
#define MPLL_CLK            266MHZ
#define FCLK                266MHZ
#define HCLK                137MHZ

/* GPT1 Tick per second*/
#define GPT1_TICK_PER_SEC   20


#endif /* __BOARD_H */
