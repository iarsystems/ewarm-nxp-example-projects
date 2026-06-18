/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * uartisr_test.c
 ******************************************************************************/
#include "drivers.h"

/* uart callback function prototype                                           */
static void uart_callback (UART_CALLBACK_SRC module, UART_CALLBACK_TYPE type,
                           int32 status);

/* data type definitions                                                      */
volatile char c;

void main (void)
{
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
  UART_Init (UART2, UART_MODULE_INTRMODE_CONFIG(9600,24e6));
  UART_InstallCallback (PRI_LVL1, uart_callback);

  EnableInterrupts();

  UART_PutStr(UART2,"\n\rCharacters will be echoed on the screen:\n");

  while(1);
}

/* uart callback function definition                                          */
static void uart_callback (UART_CALLBACK_SRC module, UART_CALLBACK_TYPE type,
                           int32 status)
{
  if (module == UART2_CALLBACK)
  {
    if (type == UART_TX_CALLBACK)
    {
      UART_PutChar (UART2, c);
      UART_TxIsrDisable (UART2);
    }
    if (type == UART_RX_CALLBACK)
    {
      c = UART_GetChar (UART2);
      UART_TxIsrEnable (UART2);
    }
  }
}
