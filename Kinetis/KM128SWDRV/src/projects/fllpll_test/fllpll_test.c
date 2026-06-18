/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * fllpll_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  /* enable clocks to all peripherals - clock mode 2:1:1 (core:bus:flash)     */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* route core clock to PTF7                                                 */
  SIM_SelClkout (CLKOUT_SRC2);
  PORT_Init (PORTF,PORT_MODULE_ALT3_MODE,PIN7);

  /* clock mode 1:1:1                                                         */
  SIM_SetClkMode (SYSCLK_MODE0);
  SIM_SetClkDiv  (SYSCLK_DIV1);

  /* FLL settings                                                             */
  FLL_Init  (FLL_MODULE_FEE_24MHZ_CONFIG);

  /* route PLL clock output from MCG to PTF7                                  */
  SIM_SelClkout (CLKOUT_SRC7);

  /* Set 32 KHz RTC Oscillator as the source of the PLL and enable PLL        */
  PLL_Enable (PLL32KREF_SRC1);

  PLL_CtrlPLLSTEN (FALSE);  /* PLL will be disabled in Normal Stop Mode       */

  while(1);
}
