/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      swisr.c
 * @version   1.0.2.0
 * @date      May-14-2012
 * @brief     Software Interrupt (SWISR) driver source code.
 ******************************************************************************/
#include "common.h"
#include "swisr.h"

#if !defined(MCU_MKM34Z7)
  #error "The swisr.c module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * public data type definitions                                               *
 ******************************************************************************/
uint16 swisrcnt[] = {0,0,0};

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static SWISR_CALLBACK pCallback0;
static SWISR_CALLBACK pCallback1;
static SWISR_CALLBACK pCallback2;

/******************************************************************************
 * public function definitions                                                *
 ******************************************************************************/
void SWISR0_HandlerInit  (uint16 ip, SWISR_CALLBACK pCallback)
{
  if (pCallback) { pCallback0 = pCallback; NVIC_SetIsr(SWISR_VECTOR_0,ip); }
}

void SWISR1_HandlerInit  (uint16 ip, SWISR_CALLBACK pCallback)
{
  if (pCallback) { pCallback1 = pCallback; NVIC_SetIsr(SWISR_VECTOR_1,ip); }
}

void SWISR2_HandlerInit  (uint16 ip, SWISR_CALLBACK pCallback)
{
  if (pCallback) { pCallback2 = pCallback; NVIC_SetIsr(SWISR_VECTOR_2,ip); }
}

/******************************************************************************
 * interrupt function definitions                                             *
 ******************************************************************************/
void slcd_isr     (void) { pCallback0(); }
void wdogewm_isr  (void) { pCallback1(); }
void mcg_isr      (void) { pCallback2(); }
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
