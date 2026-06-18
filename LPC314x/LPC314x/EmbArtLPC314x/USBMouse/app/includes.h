/***************************************************************************
 **
 **
 **    Master inlude file
 **
 **    Used with ARM IAR C/C++ Compiler
 **
 **    (c) Copyright IAR Systems 2007
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
#include <math.h>
#include <assert.h>

#include "arm_comm.h"
#include "board.h"

#include "arm926ej_cp15_drv.h"
#include "arm_comm.h"
#include "drv_cgu.h"
#include "drv_spi.h"
#include "drv_spinor.h"
#include "drv_intc.h"

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

#include "lpc314x_i2c.h"
#include "lpc314x_i2c_driver.h"
#include "lpc314x_pca9532_driver.h"

#include "buttons.h"

#endif  // __INCLUDES_H
