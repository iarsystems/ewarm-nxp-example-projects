/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * osc_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* route core clock to PTF7                                                 */
  SIM_SelClkout (CLKOUT_SRC1);
  PORT_Init (PORTF,PORT_MODULE_ALT3_MODE,PIN7);

  /* clock mode 1:1:1                                                         */
  SIM_SetClkMode (SYSCLK_MODE0);
  SIM_SetClkDiv  (SYSCLK_DIV1);

  /* System oscillator and FLL settings                                       */
  OSC_Init (OSC_MODULE_OSCERCLK_ON_STOPMODE_ON_CONFIG(OSC_SC0P));
  FLL_Init (FLL_MODULE_BLPE_OSC_CONFIG);

  while(1);
}
