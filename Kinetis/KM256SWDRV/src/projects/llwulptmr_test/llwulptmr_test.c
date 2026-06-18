/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * llwulptmr_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  /* reset pin filtered in all modes and filter clocked by LPO                */
  RCM_Init (RCM_MODULE_FILT_CLOCKED_BY_LPO_EN_IN_ALL_MODES_CONFIG);

  /* enable low voltage reset detection (1.6V), disable interrupts and bandgap*/
  PMC_Init (PMC_MODULE_LVDRE_ON_LVDINT_OFF_LVWINT_OFF_CONFIG(PMC_LVDL,PMC_LVW1),
            PMC_INTREG_BGEN_OFF_BGBE_OFF_CONFIG, PRI_LVL0, (PMC_CALLBACK)NULL);

  /* initialize LPTMR and IRTC at POR; release I/O states otherwise           */
  SIM_EnableModule (LPTMR);
  if (PMC_GetACKISO()) { PMC_SetACKISO(); }
  else
  {
    LPTMR_Init(LPTMR_MODULE_TMR_CNT_RST_ONTCF_MODE_CONFIG(0,LPTMR_LPOCLK),10000);
    LPTMR_Enable   ();
    OSC32K_Disable ();
  }
  LPTMR_ClrCmpFlag ();

  /* LLWU initialize                                                          */
  LLWU_Init (LLWU_MODULE_PERIPH_ONLY_DETECT_CONFIG(LLWU_LPTMR), PRI_LVL0, NULL);
  arch_delay (10000000l);       /* VLPR=258uA @ 2:1:1 2MHz core clock         */

  /* main programme loop                                                      */
  while (1)
  {
    /* enter low power mode                                                   */
    //SMC_SetMode(VLLS3);         /* VLLS3 = 1.8uA                            */
    SMC_SetMode(VLLS2);           /* VLLS2 = 1.6uA                            */
    //SMC_SetMode(VLLS1);         /* VLLS1 = 1.1uA                            */
  }
}
