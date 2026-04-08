/******************************************************************************
 * (c) Copyright 2014, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      mmau.c
 * @version   1.0.0.0
 * @date      Jul-23-2014
 * @brief     Memory Mapped Arithmetic Unit (MMAU) software driver source file
 ******************************************************************************/
#include "common.h"
#include "mmau.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static MMAU_CALLBACK pCallbackMMAU;

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
void MMAU_InstallCallback (tMMAU cfg, uint8 ip, MMAU_CALLBACK pCallback)
{
  if (pCallback) { pCallbackMMAU = pCallback; NVIC_SetIsr(INT_MMAU,ip); }
  MMAU_CSR = cfg.CR;
}

/******************************************************************************
 * interrupt function definitions                                             *
 ******************************************************************************/
weak void mmau_isr (void)
{
  /* read interrupt flags                                                     */
  register uint32 tmp = MMAU_CSR;

  /* process callback function                                                */
  if (tmp & MMAU_DZIF) { pCallbackMMAU (DZIF_CALLBACK); }
  if (tmp &  MMAU_VIF) { pCallbackMMAU ( VIF_CALLBACK); }
  if (tmp &  MMAU_QIF) { pCallbackMMAU ( QIF_CALLBACK); }

  /* clear interrupt flags while preserving instruction flags                 */
  MMAU_CSR_IF_CLR = tmp;
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
