/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * fmstrtsa_test.c
 ******************************************************************************/
#include "drivers.h"

static uint16 ro_counter = 0;
static uint16 rw_counter = 0;

/* FreeMASTER TSA Functionality                                               */
FMSTR_TSA_TABLE_BEGIN(first_table)
  FMSTR_TSA_RO_VAR(ro_counter , FMSTR_TSA_UINT16)
  FMSTR_TSA_RW_VAR(rw_counter , FMSTR_TSA_UINT16)
FMSTR_TSA_TABLE_END()

FMSTR_TSA_TABLE_LIST_BEGIN()
  FMSTR_TSA_TABLE(first_table)
FMSTR_TSA_TABLE_LIST_END()
/* End of FreeMASTER TSA Functionality                                        */

void main (void)
{
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* clock mode 2:1:1, 48MHz                                                  */
  SIM_SetClkMode (SYSCLK_MODE1);
  SIM_SetClkDiv  (SYSCLK_DIV1);
  FLL_Init  (FLL_MODULE_FEE_48MHZ_CONFIG);

  /* initialize UART1 (38400Bd) and FreeMASTER                                */
  PORT_Init (PORTI, PORT_MODULE_ALT2_MODE, PIN0|PIN1);
  UART_Init (UART1, UART_MODULE_POLLMODE_CONFIG(38400,48e6));
  FMSTR_Init();

  while(1)
  {
    FMSTR_Poll ();
    FMSTR_Recorder ();
    ro_counter++;
    rw_counter++;
  }
}
