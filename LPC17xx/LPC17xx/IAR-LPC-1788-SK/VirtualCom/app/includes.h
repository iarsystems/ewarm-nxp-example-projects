/***************************************************************************
 **
 **
 **    Master include file
 **
 **    Used with ARM IAR C/C++ Compiler
 **
 **    (c) Copyright IAR Systems 2010
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
#include <nxp/iolpc1788.h>
#include <math.h>

#include "arm_comm.h"
#include "board.h"

#include "drv_nvic.h"
#include "drv_clk.h"
#include "drv_glcd.h"
#include "sdram_64M_32bit_drv.h"

#include "iar_logo.h"

#include "usb_cnfg.h"
#include "usb_desc.h"
#include "usb_hw.h"
#include "usb_t9.h"
#include "usb_hooks.h"
#include "usb_dev_desc.h"
#include "usb_hooks.h"
#include "usb_buffer.h"

#include "cd_class.h"
#include "cdc_desc.h"
#include "cdc_cmd.h"

#include "uart.h"

#endif  // __INCLUDES_H
