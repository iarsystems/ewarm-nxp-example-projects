/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * sim_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  SIM_Init (SIM_MODULE_ALL_PERIPH_OFF_CONFIG);
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG );

  while(1);
}
