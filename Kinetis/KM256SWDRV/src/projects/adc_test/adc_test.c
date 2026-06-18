/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * adc_test.c
 ******************************************************************************/
#include "drivers.h"

static volatile uint16 tmp16;

void main (void)
{
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);
  ADC_Init (ADC_MODULE_16B_SWTRG_XREF_CONFIG,
            HWAVG_32,
            ADC_CH_SE_POLL_CONFIG(ADC_SE8),
            ADC_CH_DISABLE_CONFIG,
            ADC_CH_DISABLE_CONFIG,
            ADC_CH_DISABLE_CONFIG,
            PRI_LVL0, NULL);
  while (1)
  {
    if (ADC_Ready(CHA))
    {
      tmp16 = ADC_Read(CHA);
      ADC_Start(CHA, ADC_SE8);
    }
  }
}
