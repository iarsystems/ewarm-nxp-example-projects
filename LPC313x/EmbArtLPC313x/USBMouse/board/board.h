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
#include <nxp/iolpc3131.h>

#ifndef __BOARD_H
#define __BOARD_H

#define MAIN_OSC_FREQ   (12MHZ)
#define RTC_OSC_FREQ    (32768UL)

#if defined(EA_LPC_313x)

#else
#error define type of the board
#endif

#endif /* __BOARD_H */
