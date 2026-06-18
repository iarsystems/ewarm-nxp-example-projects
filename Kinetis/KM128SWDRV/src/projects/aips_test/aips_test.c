/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * aips_test.c
 ******************************************************************************/
#include "drivers.h"

volatile int16 counter;

void main (void)
{
  /* enable clock to all peripherals                                          */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* set bus masters attribute to be controlled internally by the core        */
  MCM_SetMasterAttr (MCM_CM0_MASTER|MCM_DMA_MASTER,
                     MCM_MASTER_EN_PRIV_OR_USER_SECURE_OR_NONSEC,
                     TRUE);

  /* set LPTMR slot for RW access from privileged secure mode - slot opened   */
  AIPS_SetSlotAttr (AIPS_LPTMR_SLOT, AIPS_SLOT_NO_NO_RW, TRUE);

  LPTMR_Init(LPTMR_MODULE_TMR_FREERUN_MODE_CONFIG(0,LPTMR_LPOCLK),0);
  LPTMR_Enable();

  while(1)
  {
    counter = LPTMR_ReadCnt();  /* executes in privileged mode                */
    UserMode();
    counter = LPTMR_ReadCnt();  /* hangs up in user mode                      */
  }
}
