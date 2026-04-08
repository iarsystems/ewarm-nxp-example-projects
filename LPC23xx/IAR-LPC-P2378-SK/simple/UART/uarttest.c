/*****************************************************************************
 *   uarttest.c:  main C entry file for NXP LPC23xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 ******************************************************************************/
#include <nxp/iolpc2378.h>
#include "type.h"
#include "irq.h"
#include "target.h"
#include "uart.h"

extern volatile DWORD UART0Count;
extern volatile BYTE UART0Buffer[BUFSIZE];

/*****************************************************************************
**   Main Function  main()
*****************************************************************************/
int main (void)
{
  TargetResetInit();

  UARTInit(115200); /* baud rate setting */

  while (1)
  {       /* Loop forever */
    if ( UART0Count != 0 )
    {
      U0IER = IER_THRE | IER_RLS;     /* Disable RBR */
      UARTSend( (BYTE *)UART0Buffer, UART0Count );
      UART0Count = 0;
      U0IER = IER_THRE | IER_RLS | IER_RBR; /* Re-enable RBR */
    }
  }
  return 0;
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/
