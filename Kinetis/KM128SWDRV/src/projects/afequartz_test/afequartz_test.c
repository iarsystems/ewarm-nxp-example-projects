/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * afequartz_test.c
 ******************************************************************************/
#include "drivers.h"

/* static data definitions                                                    */
static volatile int32 result;

void main (void)
{
  /* enable clocks to all peripherals - clock mode 2:1:1 (core:bus:flash)     */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* route bus clock to PTF7                                                  */
  SIM_SelClkout (CLKOUT_SRC2);
  PORT_Init (PORTF,PORT_MODULE_ALT3_MODE,PIN7);

  /* configures AFE_CLK pad to output mode and routes it to PTB7              */
  SIM_SelAfeClkPadDir (TRUE);
  PORT_Init (PORTB,PORT_MODULE_ALT2_MODE,PIN7);

  /* system oscillator and FLL settings                                       */
  OSC_Init (OSC_MODULE_OSCERCLK_ON_STOPMODE_ON_CONFIG(OSC_SC0P));
  FLL_Init (FLL_MODULE_FEI_40_50MHZ_OSC_RANGE_02_EN_CONFIG);

  /* VREF module must be initialized after SIM module                         */
  VREF_Init (VREF_MODULE_CHOP_EN_ICOMP_EN_REG_EN_VREFL_EN_LPWR_CONFIG,
             VREF_SWITCH_S1_L_S2_L_S3_L_BUFF_EN);
  VREFL_Trim( 3);                                 /* VREFL trimmed to 0.400 V */

  /* afe clocking directly from system oscillator                             */
  SIM_SelAfePllClk (SIM_OSC_CLK);
  AFE_ChInit (CH0, AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR64), 0, 0, NULL);
  AFE_Init   (AFE_MODULE_RJFORMAT_CONFIG(AFE_PLL_CLK, AFE_DIV2, 8000000));

  /* trigger AFE operation by SW                                              */
  AFE_SwTrigger (CH0);

  /* main loop                                                                */
  while (1)
  {
    if (AFE_ChReady(CH0))
    {
      AFE_Read (CH0, &result);
    }
  }
}
