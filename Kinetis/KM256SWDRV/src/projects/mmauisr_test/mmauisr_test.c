/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * mmauisr_test.c
 ******************************************************************************/
#include "drivers.h"

/* callback prototypes                                                        */
static void pit_callback  (PIT_CALLBACK_TYPE  type);

/* static variables                                                           */
static int64 volatile var_main = 0ll;
static int64 volatile var_isr  = 0ll;

void main (void)
{
  /* initialize sim module                                                    */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* initialize pit module                                                    */
  PIT_InstallCallback (PRI_LVL0,pit_callback);
  PIT_Init  (PIT0, CH0, PIT_CH_TMR_EN_CONFIG, 1000);

  /* initialize UART and FreeMASTER                                           */
  PORT_Init (PORTI, PORT_MODULE_ALT2_MODE, PIN6|PIN7);
  UART_Init (UART2, UART_MODULE_POLLMODE_CONFIG(38400,1e6));
  FMSTR_Init();

  /* enable interrupts                                                        */
  EnableInterrupts();

  while(1)
  {
    /* mmau calculation                                                       */
    var_main = d_smac_ll (100l,200l);    /* A10=A10+100*200; var_main = A10   */
    FMSTR_Poll ();
  }
}

/* pit callback definition                                                    */
static void pit_callback  (PIT_CALLBACK_TYPE  type)
{
  /* store registers state to stack                                           */
  MMAU_StoreState();

  /* mmau calculation                                                         */
  lda_d (var_isr);                       /* A10 = var_isr                     */
  var_isr = d_smac_ll (200l,500l);       /* A10=A10+200*500; var_isr = A10    */

  /* restore registers state from stack                                       */
  MMAU_RestoreState();
}
