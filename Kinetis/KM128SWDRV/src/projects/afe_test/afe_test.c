/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * afe_test.c
 ******************************************************************************/
#include "drivers.h"

/* static function prototype                                                  */
void diagnostic_setup (void);

/* static data definitions                                                    */
static int32 volatile result;

#if defined(__ICCARM__)
  #pragma required=result
#endif
void main(void)
{
  /* initialize SIM module                                                    */
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* clock mode 2:1:1, 48MHz                                                  */
  SIM_SetClkMode (SYSCLK_MODE1);
  SIM_SetClkDiv  (SYSCLK_DIV1);
  FLL_Init  (FLL_MODULE_FEE_48MHZ_CONFIG);

  /* sets 32 KHz RTC Oscillator as the source of the PLL and enable PLL       */
  PLL_Enable (PLL32KREF_SRC1);

  /* VREF module must be initialized after SIM module                         */
  VREF_Init (VREF_MODULE_CHOP_EN_ICOMP_EN_REG_EN_VREFL_EN_LPWR_CONFIG,
             VREF_SWITCH_S1_L_S2_L_S3_L_BUFF_EN);
  VREFL_Trim( 3);                                 /* VREFL trimmed to 0.400 V */

  /* AFE_CH1 init                                                             */
  SIM_SelAfePllClk (SIM_MCG_PLL_CLK);
  AFE_ChInit (CH0, AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR64), 0, PRI_LVL0, NULL);
  AFE_Init   (AFE_MODULE_RJFORMAT_CONFIG(AFE_PLL_CLK, AFE_DIV2, 12288000));

  /* initialize UART and FreeMASTER                                           */
  PORT_Init (PORTI, PORT_MODULE_ALT2_MODE, PIN0|PIN1);
  UART_Init (UART1, UART_MODULE_POLLMODE_CONFIG(38400,48e6));
  FMSTR_Init();

  diagnostic_setup ();

  /* trigger AFE conversions                                                  */
  AFE_SwTrigger (CH0);

  /* main loop                                                                */
  while (1)
  {
    if (AFE_ChReady(CH0))
    {
      result = AFE_ChRead (CH0);
      FMSTR_Recorder ();
    }
    FMSTR_Poll ();
  }
}

/* static function definition                                                 */
void diagnostic_setup (void)
{
  /* Route AFE_CH0 modulator & clock outputs to PTE0 & PTA7 for monitoring    */
  XBAR_Init (XBAR_MODULE_NO_EDGE_DETECT_CONFIG, PRI_LVL0, NULL);
  PORT_Init (PORTE,PORT_MODULE_ALT3_MODE,PIN0);
  PORT_Init (PORTA,PORT_MODULE_ALT2_MODE,PIN7);
  SIM_SelXbarAfeModOut (SIM_MOD_AFE_CH1);
  XBAR_Path (XBAR_AFEMDAT, XBAR_OUT4);
  XBAR_Path (XBAR_AFEMCLK, XBAR_OUT0);

  /* Route PLL clock output from MCG to PTF7 for monitoring                   */
  PORT_Init (PORTF,PORT_MODULE_ALT3_MODE,PIN7);
  SIM_SelClkout (CLKOUT_SRC2);
}
