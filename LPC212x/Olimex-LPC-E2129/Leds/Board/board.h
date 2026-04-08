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
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

/* OSC [MHz] */
#define FOSC                (14745600HZ)

/* Core clk [MHz]  24 MHz
   When USB engine is used the Core clk must be more that 18MHz */
#define FCCLK               (24MHZ)

#define MAM_STATE           TRUE
#define MAM_CONFIG          MAM_FULL
#define MAM_CYCLES          MAMCycle3


#endif /* __BOARD_H */
