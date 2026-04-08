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
  /* to logic 1 (TAMPER1 interrupt is generated).                             */
  IRTC_WkupInit (IRTC_MODULE_COMP_OFF_CONFIG,
                 IRTC_TAMPER_PIN_POL_HIGH_CONFIG(TAMPER_FILT_CLK_64HZ,0),
                 IRTC_TAMPER_PIN_DI_CONFIG,
                 IRTC_TAMPER1_MASK);

  arch_delay(2000000);                  /* do some processing ...             */

  IRTC_Hibernate (FORCE_PIN_LOG1);      /* force WKUP/TAMPER0 pin to logic 1  */

  while(1);
}
