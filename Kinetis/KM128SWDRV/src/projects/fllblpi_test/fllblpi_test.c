/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * fllblpi_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  /* enable clocks to all peripherals - clock mode 2:1:1 (core:bus:flash)     */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* route core clock to PTF7 for monitoring                                  */
  SIM_SelClkout (CLKOUT_SRC1);
  PORT_Init (PORTF,PORT_MODULE_ALT3_MODE,PIN7);

  /* FLL settings                                                             */
  FLL_Init (FLL_MODULE_BLPI_4MHZ_CONFIG);

  while(1);
}
