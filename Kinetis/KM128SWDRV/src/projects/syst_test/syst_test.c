/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * syst_test.c
 ******************************************************************************/
#include "drivers.h"

static uint32 volatile temp32;
static uint32 volatile counter;

/* System Timer callback prototype                                            */
static void syst_callback(void);

void main (void)
{
  /* Initialize system Timer module - generates exception every 1000 system   */
  /* clock cycles                                                             */
  SYST_Init (SYST_MODULE_RELOAD_TMR_DIV1_EN_CONFIG(1000), PRI_LVL0, syst_callback);
  EnableInterrupts ();

  while(1)
  {
    temp32 = SYST_GetCntrVal();
  }
}

/* System Timer callback definition                                           */
static void syst_callback(void)
{
  counter++;
}
