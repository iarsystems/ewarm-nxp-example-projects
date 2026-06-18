/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * mcmr_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  /* Set bus masters attribute to be controlled internally by the core        */
  MCM_SetMasterAttr (MCM_CM0_MASTER|MCM_DMA_MASTER,
                     MCM_MASTER_EN_PRIV_OR_USER_SECURE_OR_NONSEC,
                     TRUE);
  /* Set round-robin arbitration for bus masters                              */
  MCM_MasterArbCtrl (TRUE);

  /* Switch to user mode                                                      */
  UserMode ();

  while(1);
}

