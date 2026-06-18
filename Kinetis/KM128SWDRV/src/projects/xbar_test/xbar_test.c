/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * xbar_test.c
 ******************************************************************************/
#include "drivers.h"

void main(void)
{
  SIM_EnableModule (XBAR);
  XBAR_Init (XBAR_MODULE_NO_EDGE_DETECT_CONFIG, PRI_LVL0, NULL);
  XBAR_Path (XBAR_LOGIC0, XBAR_DMA);
  while(1);
}
