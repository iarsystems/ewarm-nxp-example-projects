/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * rcm_test.c
 ******************************************************************************/
#include "drivers.h"

#if defined(__ICCARM__)
  #pragma diag_suppress=Pa082
#endif
void main (void)
{
  register uint16 tmp16;

  /* reset pin filtered in all modes by a filter clocked by LPO               */
  RCM_Init  (RCM_MODULE_FILT_CLOCKED_BY_LPO_EN_IN_ALL_MODES_CONFIG);

  /* read sources of the most recent reset                                    */
  tmp16 = RCM_GetResetSrc();
  if (tmp16 & RCM_SACKERR) { NOP(); }
  if (tmp16 & RCM_MDM_AP ) { NOP(); }
  if (tmp16 & RCM_SW     ) { NOP(); }
  if (tmp16 & RCM_LOCKUP ) { NOP(); }
  if (tmp16 & RCM_POR    ) { NOP(); }
  if (tmp16 & RCM_PIN    ) { NOP(); }
  if (tmp16 & RCM_LOL    ) { NOP(); }
  if (tmp16 & RCM_LOC    ) { NOP(); }
  if (tmp16 & RCM_LVD    ) { NOP(); }
  if (tmp16 & RCM_WAKEUP ) { NOP(); }

  while(1);
}
