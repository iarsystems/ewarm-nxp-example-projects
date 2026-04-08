/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * uart_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  char c;

  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* route core clock to PTF7                                                 */
  SIM_SelClkout (CLKOUT_SRC2);
  PORT_Init (PORTF,PORT_MODULE_ALT3_MODE,PIN7);

  /* clock mode 1:1:1, 24MHz                                                  */
  SIM_SetClkMode (SYSCLK_MODE0);
  SIM_SetClkDiv  (SYSCLK_DIV1);
  FLL_Init  (FLL_MODULE_FEE_24MHZ_CONFIG);

  /* UART2 init 9600bd                                                        */
  PORT_Init (PORTI, PORT_MODULE_ALT2_MODE, PIN6|PIN7);
  UART_Init (UART2, UART_MODULE_POLLMODE_CONFIG(9600,24e6));

  UART_PutStr (UART2,"\n\rCharacters will be echoed on the screen:\n");

  while(1)
  {
    /* read and echo character                                                */
    while (!UART_RxFull(UART2));
    c = UART_GetChar (UART2);
    while (!UART_TxIdle(UART2));
    UART_PutChar (UART2,c);
  }
}
