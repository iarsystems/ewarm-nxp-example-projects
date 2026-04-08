/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * lpuart_test.c
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

  /* BUS clock selected as the LPUART clock source                            */
  SIM_SelLpuartClk(SIM_LPUARTCLK_SRC4);

  /* LPUART init 9600bd                                                       */
  PORT_Init  (PORTJ, PORT_MODULE_ALT2_MODE, PIN5|PIN6);
  LPUART_Init(LPUART0, LPUART_MODULE_POLLMODE_CONFIG(9600,24e6));

  LPUART_PutStr (LPUART0,"\n\rCharacters will be echoed on the screen:\n");

  while(1)
  {
    /* read and echo character                                                */
    while (!LPUART_RxFull(LPUART0));
    c = LPUART_GetChar (LPUART0);
    while (!LPUART_TxIdle(LPUART0));
    LPUART_PutChar (LPUART0,c);
  }
}
