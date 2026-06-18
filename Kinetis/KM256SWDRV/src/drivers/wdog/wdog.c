/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      wdog.c
 * @version   1.0.2.0
 * @date      Aug-31-2012
 * @brief     Watchdog Timer (WDOG) driver source code.
 ******************************************************************************/
#include "common.h"
#include "wdog.h"

#if !defined(MCU_MKM34Z7)
  #error "The wdog.c module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * interrupt function definitions                                             *
 ******************************************************************************/
weak void wdogewm_isr (void) { for (;;) {} }      /* overriden in swisr.c     */
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
