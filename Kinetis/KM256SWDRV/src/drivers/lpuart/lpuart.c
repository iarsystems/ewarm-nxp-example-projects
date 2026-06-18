/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      lpuart.h
 * @version   1.0.3.0
 * @date      Jan-07-2015
 * @brief     Low-Power Universal Asynchronous Receiver/Transmitter (LPUART)
 *            driver source code,
 ******************************************************************************/
#include "common.h"
#include "lpuart.h"

#if !defined(MCU_MKM34Z7)
  #error "The lpuart.c module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static LPUART_CALLBACK pCallbackLPUART;

/******************************************************************************
 * Public functions definitions                                               *
 ******************************************************************************/
void LPUART0_InstallCallback (uint8 ip, LPUART_CALLBACK pCallback)
{
  if (pCallback) { pCallbackLPUART = pCallback; NVIC_SetIsr(INT_LPUART0,ip); }
}

void LPUART0_Init (tLPUART sci)
{
  /* disable lpuart operation                                                 */
  LPUART0_CTRL &= ~(LPUART_CTRL_TE_MASK|LPUART_CTRL_RE_MASK );
  LPUART0_BAUD  = sci.BAUD;
  LPUART0_MATCH = sci.MATCH;
  LPUART0_STAT  = sci.STAT;

  /* enable uart operation                                                    */
  LPUART0_CTRL  = sci.CTRL;
  LPUART_ClrFlags (LPUART0, LPUART_ALL);
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
#if defined(__ICCARM__)
  #pragma diag_suppress=Pa082
#endif
weak void lpuart_isr (void)
{
  register uint32 tmp;

  tmp = LPUART0_STAT & LPUART_STAT_RXEDGIF_MASK;
  if((tmp != 0) && (LPUART0_BAUD & LPUART_BAUD_RXEDGIE_MASK))
  {
    LPUART0_STAT = LPUART0_STAT;  /* clear RXEDGIF interrupt flag             */
    pCallbackLPUART (LPUART0_CALLBACK, LPUART_EDG_CALLBACK,tmp);
  }

  tmp = LPUART0_STAT & (LPUART0_CTRL & (LPUART_CTRL_RIE_MASK|                 \
                        LPUART_CTRL_TCIE_MASK|LPUART_CTRL_TIE_MASK));
  if(tmp & LPUART_STAT_RDRF_MASK)
    pCallbackLPUART (LPUART0_CALLBACK, LPUART_RX_CALLBACK, tmp);

  if(tmp & (LPUART_STAT_TC_MASK|LPUART_STAT_TDRE_MASK))
    pCallbackLPUART (LPUART0_CALLBACK, LPUART_TX_CALLBACK, tmp);

  tmp = LPUART0_STAT & (LPUART0_CTRL & (LPUART_CTRL_PEIE_MASK|                \
                        LPUART_CTRL_FEIE_MASK|LPUART_CTRL_NEIE_MASK|          \
                        LPUART_CTRL_ORIE_MASK));
  if(tmp != 0)
    pCallbackLPUART (LPUART0_CALLBACK, LPUART_ER_CALLBACK, tmp);
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
