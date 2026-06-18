/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * irtc1hzpulse_test.c
 ******************************************************************************/
#include "drivers.h"
#include "freemaster.h"

static int8  int_comp_val = -5;
static uint8 fra_comp_val = 11;

/* FreeMASTER TSA table                                                       */
FMSTR_TSA_TABLE_BEGIN(first_table)
    FMSTR_TSA_RW_VAR(int_comp_val, FMSTR_TSA_SINT8)
    FMSTR_TSA_RW_VAR(fra_comp_val, FMSTR_TSA_UINT8)
FMSTR_TSA_TABLE_END()

FMSTR_TSA_TABLE_LIST_BEGIN()
    FMSTR_TSA_TABLE(first_table)
FMSTR_TSA_TABLE_LIST_END()
/* End of FreeMASTER TSA table                                                */

void main (void)
{
  /* initialize SIM module and enable MCGIRCLK for use by peripherals         */
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* enable MCGIRCLK for use by peripherals and change MCGIRCLK clock         */
  /* frequency from 2.097 MHz to 4.192 MHz                                    */
  FLL_CtrlIRCLKEN (TRUE);
  FLL_SetFastClkDiv(FIRC_DIV1);

  /* initialize IRTC with 1 Hz fine pulse output (routed to PTF2)             */
  IRTC_Init (IRTC_MODULE_COMP_FINE_CONFIG(LFSR_CLK_2HZ,0,int_comp_val,fra_comp_val),
             IRTC_TAMPER_PIN_DI_CONFIG,
             IRTC_TAMPER_PIN_DI_CONFIG,
             IRTC_TAMPER_PIN_DI_CONFIG,
             0);
  IRTC_SelRtcClkout (RTCCLKOUT_SRC1);
  PORT_Init (PORTF, PORT_MODULE_ALT3_MODE, PIN2);

  /* initialize UART and FreeMASTER                                           */
  PORT_Init (PORTI, PORT_MODULE_ALT2_MODE, PIN0|PIN1);
  UART_Init (UART1, UART_MODULE_POLLMODE_CONFIG(38400,4e6));

  FMSTR_Init();

  while(1)
  {
    IRTC_UpdateFineCompVal (int_comp_val,fra_comp_val);
    FMSTR_Poll             ();
  }
}
