/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * afeisr_test.c
 ******************************************************************************/
#include "drivers.h"

/* AFE callback declarations                                                  */
void afe_ch0_callback (AFE_CH_CALLBACK_TYPE type, int32 result);
void afe_ch1_callback (AFE_CH_CALLBACK_TYPE type, int32 result);

/* static data definitions                                                    */
volatile int32 value_ch0 = 0;
volatile int32 value_ch1 = 0;
volatile int16 delay_ch0 = 0;
volatile int16 delay_ch1 = 0;

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

  /* AFE channels init                                                        */
  SIM_SelAfePllClk (SIM_MCG_PLL_CLK);
  AFE_ChInit (CH0, AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR64), 0, PRI_LVL1,
             (AFE_CH_CALLBACK)afe_ch0_callback);
  AFE_ChInit (CH1, AFE_CH_SWTRG_CCM_PGAOFF_CONFIG(DEC_OSR64), 0, PRI_LVL1,
             (AFE_CH_CALLBACK)afe_ch1_callback);
  AFE_Init   (AFE_MODULE_RJFORMAT_CONFIG(AFE_PLL_CLK, AFE_DIV2, 12288000));

  /* initialize UART and FreeMASTER                                           */
  PORT_Init (PORTI, PORT_MODULE_ALT2_MODE, PIN0|PIN1);
  UART_Init (UART1, UART_MODULE_INTRMODE_CONFIG(38400,48e6));
  UART_InstallCallback (UART0_UART1, PRI_LVL2, (UART_CALLBACK)FMSTR_Isr);
  FMSTR_Init();

  /* diagnostic LED                                                           */
  PORT_Init (PORTF, PORT_MODULE_ALT1_MODE, PIN1);
  GPIO_Init (GPIOF, GPIO_OUT_LOGIC0_MODE, PIN1);
  PORT_Init (PORTE, PORT_MODULE_ALT1_MODE, PIN5);
  GPIO_Init (GPIOE, GPIO_OUT_LOGIC0_MODE, PIN5);

  /* enable interrupts on global level                                        */
  EnableInterrupts();

  /* trigger AFE conversions                                                  */
  AFE_SwTrigger (CH0|CH1);

  /* main loop                                                                */
  while (1)
  {
    /* update delay value                                                     */
    if (delay_ch1 != 0 || delay_ch0 != 0)
    {
      AFE_DlyWrite (CH0, delay_ch0);
      AFE_DlyWrite (CH1, delay_ch1);
      AFE_DlyOk ();
      delay_ch0 = 0;
      delay_ch1 = 0;
    }
  }
}

/* AFE callbacks definitions                                                  */
void afe_ch0_callback (AFE_CH_CALLBACK_TYPE type, int32 result)
{
  if (type == COC_CALLBACK)
  {
    value_ch0 = result;
    GPIO_Tgl (GPIOF, PIN1);
  }
}

void afe_ch1_callback (AFE_CH_CALLBACK_TYPE type, int32 result)
{
  if (type == COC_CALLBACK)
  {
    value_ch1 = result;
    FMSTR_Recorder();   /* recorder called at faster interrupt                */
    GPIO_Tgl (GPIOE, PIN5);
  }
}
