/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * irtchibernate_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  /* Initialize IRTC module to allow controlling external LDO regulator. The  */
  /* WKUP/TAMPER0 pin transitions back to HiZ state when TAMPER1 pin changes  */
  /* to logic 0 (TAMPER1 interrupt is generated).                             */
  IRTC_WkupInit (IRTC_MODULE_COMP_OFF_CONFIG(LFSR_CLK_64HZ,4),
                 IRTC_TAMPER_PIN_PASSIVE_POL_LOW_CONFIG(TAMPER_FILT_CLK_64HZ,0),
                 IRTC_TAMPER_PIN_DI_CONFIG,
                 IRTC_TAMPER1_MASK);

  arch_delay(2000000);                  /* do some processing ...             */

  IRTC_Hibernate (FORCE_PIN_LOG0);      /* force WKUP/TAMPER0 pin to logic 0  */

  while(1);
}
