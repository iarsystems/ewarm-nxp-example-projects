/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * oscvlpr_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  /* enable clocks to all peripherals - clock mode 2:1:1 (core:bus:flash)     */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);
  SMC_Init (SMC_MODULE_VLPS_ON_VLLS_ON_CONFIG);

  /* route bus clock to PTF7                                                  */
  SIM_SelClkout (CLKOUT_SRC2);
  PORT_Init (PORTF,PORT_MODULE_ALT3_MODE,PIN7);

  /* system oscillator and FLL settings                                       */
  OSC_Init (OSC_MODULE_OSCERCLK_ON_STOPMODE_ON_CONFIG(OSC_SC0P));
  SIM_SetClkDiv (SYSCLK_DIV10);

  /* switch to VLPR mode                                                      */
  SMC_SetMode(VLPR);

  while(1);
}
