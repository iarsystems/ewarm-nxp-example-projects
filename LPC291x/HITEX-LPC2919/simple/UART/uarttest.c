/*****************************************************************************
 *   usbtest.c:  main C entry file for Philips LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.10.01  ver 1.00    Prelimnary version, first Release
 *
 ******************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "irq.h"
#include "uart.h"
#include <intrinsics.h> /* for interrupts iar */

extern volatile DWORD UART0Count;
extern volatile BYTE UART0Buffer[BUFSIZE];
extern volatile DWORD UART1Count;
extern volatile BYTE UART1Buffer[BUFSIZE];

/*****************************************************************************
**   Main Function  main()
*****************************************************************************/
int main (void)
{
  UART0Init(115200);	/* baud rate setting */
  UART1Init(115200);	/* baud rate setting */

  __enable_interrupt(); // iar
  
  while (1) 
  {				/* Loop forever */
	if ( UART0Count != 0 )
	{
	  U0IER = IER_THRE | IER_RLS;			/* Disable RBR */
	  UART0Send( (BYTE *)UART0Buffer, UART0Count );
	  UART0Count = 0;
	  U0IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
	}
	
	if ( UART1Count != 0 )
	{
	  U1IER = IER_THRE | IER_RLS;			/* Disable RBR */
	  UART1Send( (BYTE *)UART1Buffer, UART1Count );
	  UART1Count = 0;
	  U1IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
	}
  }
  return 0;
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/

