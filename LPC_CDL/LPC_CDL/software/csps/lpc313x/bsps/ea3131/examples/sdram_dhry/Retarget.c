/******************************************************************************/
/* RETARGET.C: 'Retarget' layer for target-dependent low level functions      */
/******************************************************************************
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP LPC3xxx microcontroller devices only. Nothing else gives
 *      you the right to use this software.
 *
 *      Copyright (c) 2005-2009 Keil Software.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include "lpc313x_cgu_driver.h"
#include "lpc313x_uart.h"

void init_serial(void)
{                 /* Initialize Serial Interface     */
  /* enable UART clock */
  cgu_clk_en_dis(CGU_SB_UART_APB_CLK_ID, TRUE);
  cgu_clk_en_dis(CGU_SB_UART_U_CLK_ID, TRUE);

  /* Setup UART for 115.2K, 8 data bits, no parity, 1 stop bit */
  UART->dlm_ie = 0;
  /* Set the `Divisor Latch Access Bit` and enable so the DLL/DLM access*/
  UART->lcr |= UART_LCR_DIVLATCH_EN;
  /* Initialise the `Divisor latch LSB` and `Divisor latch MSB` registers */
  UART->dll_fifo = UART_LOAD_DLL(3);
  UART->dlm_ie = UART_LOAD_DLM(3);
  UART->lcr &= ~ UART_LCR_DIVLATCH_EN;

  /* Initialise the Fractional Divider Register */
  UART->fdr = UART_FDR_MUL_SET(6) | UART_FDR_DIVADD_SET(7);
  UART->lcr = UART_LCR_WLEN_8BITS;
  /* Clear FIFOs, set FIFO level, and pending interrupts */
  UART->iir_fcr = (UART_FCR_RXFIFO_TL16 |
                   UART_FCR_FIFO_EN | UART_FCR_TXFIFO_FLUSH |
                   UART_FCR_RXFIFO_FLUSH);
}

int MyLowLevelPutchar(int ch)
{

 /* wait until TX FIFO & THR are empty*/
  while ((UART->lsr & UART_LSR_TEMT) == 0);
  /* now write */
  UART->dll_fifo = ch;

  return(ch);
}

size_t __write(int handle, const unsigned char * buffer, size_t size)
{
  /* Remove the #if #endif pair to enable the implementation */
  size_t nChars = 0;

  if (buffer == 0)
  {
    /*
     * This means that we should flush internal buffers.  Since we
     * don't we just return.  (Remember, "handle" == -1 means that all
     * handles should be flushed.)
     */
    return 0;
  }

  /* This template only writes to "standard out" and "standard err",
   * for all other file handles it returns failure. */

  for (/* Empty */; size != 0; --size)
  {
    MyLowLevelPutchar(*buffer++);

    ++nChars;
  }

  return nChars;

}

int MyLowLevelGetchar()
{
  /* wait until data is recived */
  while ((UART->lsr & UART_LSR_RDR) == 0);

  return(MyLowLevelPutchar(UART->dll_fifo));
}

size_t __read(int handle, unsigned char * buffer, size_t size)
{
  /* Remove the #if #endif pair to enable the implementation */

  int nChars = 0;

  for (/* Empty */; size > 0; --size)
  {
    int c = MyLowLevelGetchar();
    if (c < 0)
      break;

    *buffer++ = c;
    ++nChars;
  }

  return nChars;
}
