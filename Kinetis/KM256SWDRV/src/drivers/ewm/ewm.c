/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      ewm.c
 * @version   1.0.0.0
 * @date      Jan-16-2015
 * @brief     External Watchdog Monitor (EWM) driver source code.
 ******************************************************************************/
#include "common.h"
#include "ewm.h"

#if !defined(MCU_MKM34Z7)
  #error "The ewm.c module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static EWM_CALLBACK pCallbackEWM;

/******************************************************************************
 * Public functions definitions                                               *
 ******************************************************************************/
void EWM_Init (tEWM cfg, uint8 ip, EWM_CALLBACK pCallback)
{
  register uint8 tmp = 0;

  if (pCallback)
  {
    pCallbackEWM = pCallback;
    NVIC_SetIsr(INT_WDOG_EWM,ip);
    tmp = EWM_CTRL_INTEN_MASK;
  }

  EWM_CMPL         = cfg.CMPL;
  EWM_CMPH         = cfg.CMPH;
  EWM_CLKCTRL      = cfg.CLKCTRL;
  EWM_CLKPRESCALER = cfg.CLKPRESCALER;
  EWM_CTRL         = cfg.CTRL|tmp;
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
weak void ewm_isr (void)
{
  ioclrb (EWM_CTRL,EWM_CTRL_INTEN_MASK);
  pCallbackEWM();
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
