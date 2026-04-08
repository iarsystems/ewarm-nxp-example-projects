/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * rmga_test.c
 ******************************************************************************/
#include "drivers.h"

static volatile uint32 random_value;

#if defined(__ICCARM__)
  #pragma required=random_value
#endif
void main (void)
{
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);
  RNGA_Init (RNGA_MODULE_NORMAL_MODE_CONFIG, PRI_LVL0, NULL);
  while (1)
  {
    RNGA_WaitAndGetVal (&random_value);
  }
}
