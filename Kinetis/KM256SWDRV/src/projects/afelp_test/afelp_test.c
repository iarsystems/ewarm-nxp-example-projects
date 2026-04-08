/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * afelp_test.c
 ******************************************************************************/
#include "drivers.h"

/* static data definitions                                                    */
volatile int32 result[2];

#if defined(__ICCARM__)
  #pragma required=result
#endif
void main (void)
{
  /* initialize SIM module                                                    */
  SMC_Init (SMC_MODULE_VLPS_ON_VLLS_OFF_CONFIG);
  SIM_Init (SIM_MODULE_LP_CONFIG);

  /* clock mode 1:1:1, 1.0 MHz                                                */
  SIM_SetClkMode (SYSCLK_MODE0);
  SIM_SetClkDiv  (SYSCLK_DIV2);
  FLL_Init  (FLL_MODULE_BLPI_2MHZ_CONFIG);
  PMC_SetBGEN ();                                 /* enable VREF in VLPR mode */

  /* switch to VLPR mode                                                      */
  SMC_SetMode(VLPR);

  /* VREF module must be initialized after SIM module                         */
  VREF_Init (VREF_MODULE_CHOP_EN_ICOMP_EN_REG_EN_VREFL_EN_LPWR_CONFIG,
             VREF_SWITCH_S1_L_S2_L_S3_L_BUFF_DI);
  VREFL_Trim( 3);                                 /* VREFL trimmed to 0.400 V */

  /* AFE_CH0 & AFE_CH2 init                                                   */
  AFE_ChInit (CH0, AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR256),0,PRI_LVL0,NULL);
  AFE_ChInit (CH2, AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR256),0,PRI_LVL0,NULL);
  AFE_Init   (AFE_MODULE_LPM_RJFORMAT_CONFIG(AFE_IRC_CLK, AFE_DIV1, 1000000));

  /* initialize UART and FreeMASTER                                           */
  PORT_Init (PORTI, PORT_MODULE_ALT2_MODE, PIN6|PIN7);
  UART_Init (UART2, UART_MODULE_POLLMODE_CONFIG(19200,1.0e6));
  FMSTR_Init();

  /* trigger AFE conversions                                                  */
  AFE_SwTrigger (CH0|CH2);

  /* main loop                                                                */
  while (1)
  {
    if ((AFE_ChReady(CH0)) && (AFE_ChReady(CH2)))
    {
      AFE_Read (CH0, &result[0]);
      AFE_Read (CH2, &result[1]);
      FMSTR_Recorder ();
    }
    FMSTR_Poll ();
  }
}
