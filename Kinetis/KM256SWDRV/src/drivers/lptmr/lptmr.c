/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      lptmr.c
 * @version   1.0.4.0
 * @date      Jan-16-2015
 * @brief     Low-Power Timer (LPTMR) driver source code.
 ******************************************************************************/
#include "common.h"
#include "lptmr.h"

#if !defined(MCU_MKM34Z7)
  #error "The lptmr.c module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static LPTMR_CALLBACK pCallbackLPTMR;

/******************************************************************************
 * public function definitions                                                *
 ******************************************************************************/
void LPTMR_InstallCallback (uint8 ip, LPTMR_CALLBACK pCallback)
{
  if (pCallback) { pCallbackLPTMR = pCallback; NVIC_SetIsr(INT_LPTMR0,ip); }
}

void LPTMR_Init (tLPTMR lptmr, uint16 val)
{
  LPTMR_PSR = lptmr.PSR;
  LPTMR_CMR = val;
  LPTMR_CSR = (lptmr.CSR|LPTMR_CSR_TCF_MASK);     /* Timer Compare Flag       */
}

/******************************************************************************
 * interrupt function definitions                                             *
 ******************************************************************************/
weak void lptmr_isr (void)
{
  iosetw (LPTMR_CSR,LPTMR_CSR_TCF_MASK);          /* Clear interrupt flag     */
  /* call interrupt function                                                  */
  pCallbackLPTMR ();
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
