/***************************************************************************
 **
 **
 **    Master include file
 **
 **    Used with ARM IAR C/C++ Compiler
 **
 **    (c) Copyright IAR Systems 2011
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
#include <nxp/iolpc11u3x.h>
#include <math.h>

#include "arm_comm.h"
#include "board.h"

#include "glcd_api.h"

#include "usb_cnfg.h"
#include "usb_desc.h"
#include "usb_hw.h"
#include "usb_t9.h"
#include "usb_hooks.h"
#include "usb_dev_desc.h"
#include "hid.h"
#include "hid_mouse.h"
#include "usb_hooks.h"
#include "usb_buffer.h"

#endif  // __INCLUDES_H
