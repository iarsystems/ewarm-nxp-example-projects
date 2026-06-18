/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2005
 **
 **    $Revision: 16 $
 **
 ***************************************************************************/
#ifndef __BOARD_H
#define __BOARD_H
#include "arm_comm.h"

/* OSC [MHz] */
#define FOSC1               (16MHZ)
#define FOSC2               (327680HZ)

/* Core clk [MHz] */
#define _M_PD               (1UL)
#define _M_MFD              (64UL)
#define _M_MFI              (5UL)
#define _M_MFN              (55UL)

#define _S_PD               (2UL)
#define _S_MFD              (64UL)
#define _S_MFI              (5UL)
#define _S_MFN              (55UL)

#define SPLL_CLK            (    FOSC1*2*(_S_MFI+((float)_S_MFN/(float)_S_MFD))/(float)_S_PD)
#define MPLL_CLK            (512*FOSC2*2*(_M_MFI+((float)_M_MFN/(float)_M_MFD))/(float)_M_PD)

// FCLK
#define FLCK_DIV            (1UL)
#define FCLK                (MPLL_CLK/FLCK_DIV)

// HCLK
#define HCLK_DIV            (1UL)
#define HCLK                (SPLL_CLK/HCLK_DIV)

// Peripherals divider 1
#define PER1_DIV            (13UL)
#define PERCLK1             (Int32U)(SPLL_CLK/PER1_DIV)

// Peripherals divider 2
#define PER2_DIV            (13UL)
#define PERCLK2             (SPLL_CLK/PER2_DIV)

// Peripherals divider 3
#define PER3_DIV            (13UL)
#define PERCLK3             (SPLL_CLK/PER3_DIV)

/* Tick per second*/
#define TICK_PER_SEC        (100UL)

#endif /* __BOARD_H */
