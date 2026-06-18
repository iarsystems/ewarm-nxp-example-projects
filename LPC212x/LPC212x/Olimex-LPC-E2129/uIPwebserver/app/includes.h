/***************************************************************************
 **
 **    Master inlude file
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2006
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

#include <intrinsics.h>
#include <NXP/iolpc2129.h>

#include "arm_comm.h"
#include "board.h"

#include "lpc_vic.h"
#include "lpc_sysctrl.h"
#include "cs8900a.h"


#include "timer.h"
#include "uip-conf.h"
#include "uipopt.h"
#include "uip_arp.h"
#include "uip.h"
#include "httpd.h"

#endif // __INCLUDES_H
