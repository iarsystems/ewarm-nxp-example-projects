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
#include <nxp/iolpc1850.h>

#ifndef __BOARD_H
#define __BOARD_H

#define I_RC_OSC_FREQ   (12MHZ)
#define MAIN_OSC_FREQ   (12MHZ)
#define RTC_OSC_FREQ    (32768UL)

#if defined(HITEX_LPC_1800)

#define BOARD_A2  1
#define BOARD_A4  2

#define PCA9502_LED_ADDR    0x4D
#define PCA9673PW_LED_ADDR  0x24

#else
#error define type of the board
#endif

#endif /* __BOARD_H */
