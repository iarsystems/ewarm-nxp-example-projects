/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * lpuartisr_test.c
 ******************************************************************************/
#include "drivers.h"

/* uart callback function prototype                                           */
static void lpuart_callback (LPUART_CALLBACK_SRC module, LPUART_CALLBACK_TYPE type,
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

  /* BUS clock selected as the LPUART clock source                            */
  SIM_SelLpuartClk(SIM_LPUARTCLK_SRC4);

  /* LPUART init 9600bd                                                       */
  PORT_Init  (PORTJ, PORT_MODULE_ALT2_MODE, PIN5|PIN6);
  LPUART_Init (LPUART0, LPUART_MODULE_INTRMODE_CONFIG(9600,24e6));
  LPUART_InstallCallback (PRI_LVL1, lpuart_callback);

  EnableInterrupts();

  LPUART_PutStr(LPUART0,"\n\rCharacters will be echoed on the screen:\n");

  while(1);
}

/* lpuart callback function definition                                        */
static void lpuart_callback (LPUART_CALLBACK_SRC module, LPUART_CALLBACK_TYPE type,
                             int32 status)
{
  if (module == LPUART0_CALLBACK)
  {
    if (type == LPUART_TX_CALLBACK)
    {
      LPUART_PutChar (LPUART0, c);
      LPUART_TxIsrDisable (LPUART0);
    }
    if (type == LPUART_RX_CALLBACK)
    {
      c = LPUART_GetChar (LPUART0);
      LPUART_TxIsrEnable (LPUART0);
    }
  }
}
