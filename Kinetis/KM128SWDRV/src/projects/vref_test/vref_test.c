/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * vref_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* voltage reference sourced from the internal 1.2V bangap reference        */
  VREF_Init (VREF_MODULE_CHOP_EN_ICOMP_EN_REG_EN_VREFL_EN_LPWR_CONFIG,
             VREF_SWITCH_S1_L_S2_L_S3_L_BUFF_EN);
  VREFL_Trim(3);

  /* voltage reference sourced from the external 1.2V reference               */
  VREF_Init (VREF_MODULE_CHOP_DI_ICOMP_DI_REG_DI_VREFL_EN_LPWR_CONFIG,
             VREF_SWITCH_S1_L_S2_L_S3_H_BUFF_DI);
  VREFL_Trim(3);

  while(1);
}
