/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * wdog_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  volatile uint16 i=0;

  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* Watchdog timeout is of 1.5 sec approximately                             */
  WDOG_Init(WDOG_MODULE_NORMAL_STOP_DI_CONFIG(WDOG_LPO_CLK,WDOG_DIV1,1500));
  while(1)
  {
   WDOG_WriteRefreshSeq();
   /*
    * Adding the delay of approximately 2 ms because watchdog is running
    * at 1 KHz clock. Commenting out this delay will cause watchdog reset.
    */
   for(i=0;i<0xf0;i++);
  }
}
