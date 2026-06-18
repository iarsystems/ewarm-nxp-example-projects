/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * systmsr_test.c
 ******************************************************************************/
#include "drivers.h"

static uint32 volatile ticks;

void main (void)
{
  /* initialize system Timer                                                  */
  SYST_Init (SYST_MODULE_FREERUN_TMR_DIV1_EN_CONFIG,PRI_LVL0,NULL);

  /* clear system Timer                                                       */
  SYST_ClrCntrVal  ();
  NOP();
  NOP();
  NOP();
  /* read value from the system Timer                                         */
  ticks = SYST_GetCntrVal ();  /* ticks = 3 core clock cycles                 */

  while(1);
}
