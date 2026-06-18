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

#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <stdbool.h>
#include    <assert.h>

#include    <intrinsics.h>
#include 		<nxp/iolpc1768.h>

#include    "arm_comm.h"
#include    "board.h"

#include    "drv_glcd_cnfg.h"
#include    "drv_glcd.h"
#include    "glcd_ll.h"

#include    "iar_logo.h"
#include    "i2c_drv.h"
#include    "smb380_drv.h"

#endif // __INCLUDES_H
