/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * afevlprpll_test.c
 ******************************************************************************/
#include "drivers.h"

volatile int32 result;

void main (void)
{
  /* enable clocks to selected on chip peripherals                            */
  SIM_Init (SIM_MODULE_LP_PLL_VLP_EN_CONFIG);

  /* Required by VREF and PLL in VLPR mode                                    */
  PMC_SetBGEN ();

  /* enable PLL module                                                        */
  PLL_Enable (PLL32KREF_SRC1);

  /* VREF module must be initialized after SIM module                         */
  VREF_Init (VREF_MODULE_CHOP_EN_ICOMP_EN_REG_EN_VREFL_EN_LPWR_CONFIG,
             VREF_SWITCH_S1_L_S2_L_S3_L_BUFF_EN);
  VREFL_Trim( 3);                                 /* VREFL trimmed to 0.400 V */

  /* afe clocking directly from system oscillator                             */
  SIM_SelAfePllClk (SIM_MCG_PLL_CLK);
  AFE_ChInit (CH0, AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR64), 0, 0, NULL);
  AFE_Init   (AFE_MODULE_LPM_RJFORMAT_CONFIG(AFE_PLL_CLK, AFE_DIV8, 12288000));

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
