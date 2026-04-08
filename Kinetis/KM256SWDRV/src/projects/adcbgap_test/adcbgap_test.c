/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * adcbgap_test.c
 ******************************************************************************/
#include "drivers.h"

static volatile uint16 ad10_value;

void main (void)
{
  /* enable PMC buffer operation also in VLPx modes and bandgap buffer in     */
  /* low power mode                                                           */
  PMC_Init (PMC_MODULE_LVDRE_ON_LVDINT_OFF_LVWINT_OFF_CONFIG(PMC_LVDL,PMC_LVW1),
            PMC_INTREG_BGEN_ON_BGBE_ON_CONFIG, PRI_LVL0, (PMC_CALLBACK)NULL);

  /* SMC and SIM modules                                                      */
  SMC_Init (SMC_MODULE_VLPS_ON_VLLS_ON_CONFIG);
  SIM_Init (SIM_MODULE_ALL_PERIPH_OFF_CONFIG);

  SIM_EnableModule (UART2);
  SIM_EnableModule (ADC);
  SIM_EnableModule (PORTI);

  /* switch to VLPR mode                                                      */
  SMC_SetMode(VLPR);

  /* ADC module                                                               */
  ADC_Init  (ADC_MODULE_16B_SWTRG_BGAP_LP_INTERNAL_CLK_CONFIG,
             HWAVG_OFF,
             ADC_CH_SE_POLL_CONFIG(ADC_SE8),
             ADC_CH_DISABLE_CONFIG,
             ADC_CH_DISABLE_CONFIG,
             ADC_CH_DISABLE_CONFIG,
             PRI_LVL0,
             (ADC_CALLBACK)NULL);

  /* initialize UART and FreeMASTER                                           */
  PORT_Init (PORTI, PORT_MODULE_ALT2_MODE, PIN6|PIN7);
  UART_Init (UART2, UART_MODULE_POLLMODE_CONFIG(38400,1e6));

  FMSTR_Init();

  while (1)
  {
    FMSTR_Poll ();
    if (ADC_Ready(CHA))
    {
      ad10_value = ADC_Read(CHA);
      ADC_Start(CHA, ADC_SE8);
    }
  }
}
