/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * vlpsuart_test.c
 ******************************************************************************/
#include "drivers.h"

/* uart callback function prototype                                           */
static void uart_callback (UART_CALLBACK_SRC module, UART_CALLBACK_TYPE type,
                           int32 status);

/* static data definition                                                     */
static int volatile counter = 0;

/* device boot in VLPR mode - see appconfig.h                                 */
void main (void)
{
  /* Core:Bus:Flash = 2:1:1; Core clock = 0.4 MHz                             */
  SIM_SetClkDiv (SYSCLK_DIV5);

  /* enable clock to modules                                                  */
  SIM_EnableModule (PORTI);
  SIM_EnableModule (UART1);
  SIM_EnableModule (LPTMR);

  /* UART1 init 4800bd                                                        */
  PORT_Init   (PORTI, PORT_MODULE_ALT2_MODE, PIN0|PIN1);
  UART_Init   (UART1, UART_MODULE_INTRMODE_CONFIG(4800,0.4e6));
  UART_EdgIsrEnable    (UART1);
  UART_InstallCallback (UART0_UART1, PRI_LVL1, uart_callback);
  FMSTR_Init();

  /* initialize LPTMR in freerun mode                                         */
  LPTMR_Init(LPTMR_MODULE_TMR_FREERUN_MODE_CONFIG(0,LPTMR_LPOCLK),0);
  LPTMR_Enable();
  EnableInterrupts();

  while(1)
  {
    if (LPTMR_ReadCnt() >= 500) { LPTMR_Disable (); SMC_SetMode (VLPS); }
    FMSTR_Recorder ();
    counter++;
  }
}

/* uart callback function definition                                          */
static void uart_callback (UART_CALLBACK_SRC module, UART_CALLBACK_TYPE type,
                           int32 status)
{
  if (module == UART1_CALLBACK)
  {
    if  (type == UART_EDG_CALLBACK) { LPTMR_Enable(); }
    FMSTR_Isr   ();
    LPTMR_Reset ();
  }
}
