/***************************************************************************
 **
 **    Master include file
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2006
 **
 **    $Revision: 28 $
 **
 ***************************************************************************/

#ifndef  __INCLUDES_H
#define  __INCLUDES_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <intrinsics.h>
#include <nxp/iolpc2468.h>

#include "arm_comm.h"
#include "board.h"
#include "sys.h"

#include "drv_hd44780_cnfg.h"
#include "drv_hd44780.h"
#include "drv_hd44780_l.h"

#endif // __INCLUDES_H
