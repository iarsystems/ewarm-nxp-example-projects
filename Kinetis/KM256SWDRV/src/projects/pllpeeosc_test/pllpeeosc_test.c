/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * pllpeeosc_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* route core clock to PTF7 for monitoring                                  */
  SIM_SelClkout (CLKOUT_SRC1);
  PORT_Init (PORTF,PORT_MODULE_ALT3_MODE,PIN7);

  /* PLL settings                                                             */
  OSC_Init (OSC_MODULE_OSCERCLK_ON_STOPMODE_ON_CONFIG(OSC_SC0P));
  PLL_Init (PLL_MODULE_PEE_DIV256_OSC_CONFIG);

  while(1);
}
