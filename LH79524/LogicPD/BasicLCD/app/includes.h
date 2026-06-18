/***************************************************************************
 **
 **    Master inlude file
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2005
 **
 **    $Revision: 34 $
 **
 ***************************************************************************/

#ifndef __INCLUDES_H
#define __INCLUDES_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#include <intrinsics.h>
#include <NXP/iolh79524.h>

#include "arm_comm.h"
#include "board.h"
#include "arm720t_cp15_drv.h"

#include "lh7952x_vic.h"
#include "touch_scr.h"
#include "lcd_drv.h"
#include "lq035q7db02_lcd.h"
#include "paint_bmp.h"

#endif
