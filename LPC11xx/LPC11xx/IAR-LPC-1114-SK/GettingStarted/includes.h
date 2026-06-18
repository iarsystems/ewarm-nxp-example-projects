/***************************************************************************
 **
 **
 **    Master include file
 **
 **    Used with ARM IAR C/C++ Compiler
 **
 **    (c) Copyright IAR Systems 2009
 **
 **    $Revision: 28 $
 **
 ***************************************************************************/

#ifndef __INCLUDES_H
#define __INCLUDES_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <intrinsics.h>
#include <assert.h>
#include <nxp/iolpc1114.h>
#include <math.h>

#include "arm_comm.h"
#include "board.h"

#include "drv_hd44780.h"
#include "drv_hd44780_cnfg.h"
#include "drv_hd44780_l.h"

#include "ntc.h"
#include "temperature.h"

#endif  // __INCLUDES_H
