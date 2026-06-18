/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * lptmr_test.c
 ******************************************************************************/
#include "drivers.h"

static volatile uint16 counter;

void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* initialize LPTMR in freerun Timer mode                                   */
  LPTMR_Init(LPTMR_MODULE_TMR_FREERUN_MODE_CONFIG(0,LPTMR_LPOCLK),0);
  LPTMR_Enable();

  while (1)
  {
    counter = LPTMR_ReadCnt();
  }
}
