/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * pdbb2b_test.c
 ******************************************************************************/
#include "drivers.h"
static uint16 volatile tmp16[3];

void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* PDB pre-trigger outputs are connected with the ADC's HW trigger inputs   */
  SIM_SelAdcTrg(TRUE);

  /* route the ADC conversion complete signals to the PDB pre-trigger acks    */
  /* inputs                                                                   */
  XBAR_Init (XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             0,
             NULL);
  XBAR_Path(XBAR_ADCCOCCHA,XBAR_PDBPTRG1A);
  XBAR_Path(XBAR_ADCCOCCHB,XBAR_PDBPTRG2A);
  XBAR_Path(XBAR_ADCCOCCHC,XBAR_PDBPTRG3A);

  /* ADC init - HW trigger is selected                                        */
  ADC_Init (ADC_MODULE_16B_HWTRG_XREF_CONFIG,
            HWAVG_OFF,
            ADC_CH_SE_POLL_CONFIG(ADC_SE8),
            ADC_CH_SE_POLL_CONFIG(ADC_SE15),
            ADC_CH_SE_POLL_CONFIG(ADC_SE14),
            ADC_CH_DISABLE_CONFIG,
            PRI_LVL0, NULL);

  /* PDB init - pre trigger channels 0,1,2 are configured.                    */
  PDB_PtrgInit(TRG0, PDB_PTRG_ON_DELAYED_CONFIG,  5000);
  PDB_PtrgInit(TRG1, PDB_PTRG_ON_B2BBYPASSED_CONFIG, 0);
  PDB_PtrgInit(TRG2, PDB_PTRG_ON_B2BBYPASSED_CONFIG, 0);
  PDB_Init(PDB_MODULE_SWTRG_CONT_CONFIG(PDBCLK_DIV1,PDB_MULT1,60000,0),0,NULL);

  /* Trigger the PDB counter                                                  */
  PDB_SWTrg();

  while (1)
  {
    if (ADC_Ready(CHA))
    {
      tmp16[0] = ADC_Read(CHA);
    }
    if (ADC_Ready(CHB))
    {
      tmp16[1] = ADC_Read(CHB);
    }
    if (ADC_Ready(CHC))
    {
      tmp16[2] = ADC_Read(CHC);
    }
  }
}
