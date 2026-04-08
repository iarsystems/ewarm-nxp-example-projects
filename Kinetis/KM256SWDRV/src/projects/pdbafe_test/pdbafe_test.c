/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * pdbafe_test.c
 ******************************************************************************/
#include "drivers.h"
static int32 volatile tmp32[4];

void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  SIM_SetClkMode (SYSCLK_MODE1);
  SIM_SetClkDiv  (SYSCLK_DIV1);

  PLL_Enable (PLL32KREF_SRC1);

  /* VREF module must be initialized after SIM module                         */
  VREF_Init (VREF_MODULE_CHOP_EN_ICOMP_EN_REG_EN_VREFL_EN_LPWR_CONFIG,
             VREF_SWITCH_S1_L_S2_L_S3_L_BUFF_EN);
  VREFL_Trim( 3);                                 /* VREFL trimmed to 0.400 V */


  /* CMP init                                                                 */
  CMP_Init  (CMP2,
             CMP_MODULE_EN_FILTER7_SAMPLE_PER_255_DAC_EN_CONFIG(32),
             IN4,
             IN7_DAC);

  /* AFE init                                                                 */
  SIM_SelAfePllClk (SIM_MCG_PLL_CLK);
  AFE_ChInit (CH0, AFE_CH_HWTRG_SCM_PGAOFF_CONFIG(DEC_OSR64), 0, PRI_LVL0, NULL);
  AFE_ChInit (CH1, AFE_CH_HWTRG_SCM_PGAOFF_CONFIG(DEC_OSR64), 0, PRI_LVL0, NULL);
  AFE_ChInit (CH2, AFE_CH_HWTRG_SCM_PGAOFF_CONFIG(DEC_OSR64), 0, PRI_LVL0, NULL);
  AFE_ChInit (CH3, AFE_CH_HWTRG_SCM_PGAOFF_CONFIG(DEC_OSR64), 0, PRI_LVL0, NULL);
  AFE_Init   (AFE_MODULE_RJFORMAT_CONFIG(AFE_PLL_CLK, AFE_DIV2, 12288000));

  /* route the ADC conversion complete signals to the PDB pre-trigger acks    */
  /* inputs                                                                   */
  XBAR_Init (XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             0,
             NULL);

  XBAR_Path(XBAR_PDBPTRG0,XBAR_AFE0TRG);
  XBAR_Path(XBAR_PDBPTRG1,XBAR_AFE1TRG);
  XBAR_Path(XBAR_PDBPTRG2,XBAR_AFE2TRG);
  XBAR_Path(XBAR_PDBPTRG3,XBAR_AFE3TRG);

  /* PDB init - pre trigger channels 0,1,2,3 are configured.                  */
  PDB_PtrgInit (TRG0, PDB_PTRG_ON_DELAYED_CONFIG, 10000);
  PDB_PtrgInit (TRG1, PDB_PTRG_ON_DELAYED_CONFIG, 20000);
  PDB_PtrgInit (TRG2, PDB_PTRG_ON_DELAYED_CONFIG, 30000);
  PDB_PtrgInit (TRG3, PDB_PTRG_ON_DELAYED_CONFIG, 40000);
  PDB_Init(PDB_MODULE_HWTRG_ONESHOT_CONFIG(PDBTRG_CMP2,PDBCLK_DIV1,PDB_MULT1,60000,0),0,NULL);

  while (1)
  {
    if (AFE_ChReady(CH0))
    {
      tmp32[0] = AFE_ChRead(CH0);
    }
    if (AFE_ChReady(CH1))
    {
      tmp32[1] = AFE_ChRead(CH1);
    }
    if (AFE_ChReady(CH2))
    {
      tmp32[2] = AFE_ChRead(CH2);
    }
    if (AFE_ChReady(CH3))
    {
      tmp32[3] = AFE_ChRead(CH3);
    }
  }
}
