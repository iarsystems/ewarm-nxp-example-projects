/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2006
 **
 **    $Revision: 28 $
 **
 ***************************************************************************/
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

/* OSC [MHz] */
#define FOSC                (14.7456MHZ)
/* Core clk [MHz] */
#define FCCLK               (14.7456MHZ)
/* Per clk [MHz] */
#define PCCLK               (14.7456MHZ/4)

#endif /* __BOARD_H */
