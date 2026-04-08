/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * fmstrisr_test.c
 ******************************************************************************/
#include "drivers.h"

int counter = 0;

void main (void)
{
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* route system clock to PTF7                                               */
  SIM_SelClkout (CLKOUT_SRC1);
  PORT_Init (PORTF,PORT_MODULE_ALT3_MODE,PIN7);

  /* clock mode 2:1:1, 48MHz                                                  */
  SIM_SetClkMode (SYSCLK_MODE1);
  SIM_SetClkDiv  (SYSCLK_DIV1);
  FLL_Init  (FLL_MODULE_FEE_48MHZ_CONFIG);

  /* initialize UART1 (38400Bd) and FreeMASTER                                */
  PORT_Init (PORTI, PORT_MODULE_ALT2_MODE, PIN0|PIN1);
  UART_Init (UART1, UART_MODULE_INTRMODE_CONFIG(38400,48e6));
  UART_InstallCallback (UART0_UART1, PRI_LVL2, (UART_CALLBACK)FMSTR_Isr);

  FMSTR_Init();

  EnableInterrupts();

  while(1)
  {
    FMSTR_Recorder ();
    counter++;
  }
}
