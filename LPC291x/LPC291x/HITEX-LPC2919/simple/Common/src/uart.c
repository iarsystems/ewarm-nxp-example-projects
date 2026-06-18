/*****************************************************************************
 *   uart.c:  UART API file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.07.12  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"
#include "type.h"
#include "target.h"
#include "irq.h"
#include "uart.h"

volatile DWORD UART0Status, UART1Status;
volatile BYTE UART0TxEmpty = 1, UART1TxEmpty = 1;
volatile BYTE UART0Buffer[BUFSIZE], UART1Buffer[BUFSIZE];
volatile DWORD UART0Count = 0, UART1Count = 0;
volatile DWORD FrameErrorCount = 0;
volatile DWORD BreakErrorCount = 0;
volatile DWORD RXTimeoutErrorCount = 0;
volatile DWORD RXOverflowErrorCount = 0;

/*****************************************************************************
** Function name:		UARTxHandler
**
** Descriptions:		UARTx interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void UART0Handler (void)
{
  BYTE IIRValue, LSRValue;
  BYTE Dummy = Dummy;

  IIRValue = U0IIR;
    
  IIRValue >>= 1;			/* skip bit 0 in IIR */
  IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
  if ( IIRValue == IIR_RLS ) {		/* Receive Line Status */
	LSRValue = U0LSR;
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) ) {
	  /* There are errors or break interrupt */
	  /* Read LSR will clear the interrupt */
	  Dummy = U0RBR;	/* Dummy read to clear interrupt, then bail out */
	  return;
	}
	if ( LSRValue & LSR_RDR ) {	/* Receive Data Ready */			
	  /* If no error on RLS, normal ready, save into the data buffer. */
	  /* Note: read RBR will clear the interrupt */
	  if ( UART0Count == BUFSIZE ) {
		UART0Count = 0;		/* buffer overflow */
	  }
	  UART0Buffer[UART0Count++] = U0RBR;	
	}
  }
  else if ( IIRValue == IIR_RDA ) {	/* Receive Data Available */
	/* Receive Data Available */
	if ( UART0Count == BUFSIZE ) {
	  UART0Count = 0;		/* buffer overflow */
	}
	UART0Buffer[UART0Count++] = U0RBR;
  }
  else if ( IIRValue == IIR_CTI ) {	/* Character timeout indicator */
	/* The mini. RX trigger level is 16, so, whenever RX FIFO is
	not empty, this CTI will be set after 4 character time, simply 
	read data from the FIFO when this interrupt occurs. */
	if ( UART0Count == BUFSIZE ) {
	  UART0Count = 0;		/* buffer overflow */
	}
	UART0Buffer[UART0Count++] = U0RBR;
  }
  else if ( IIRValue == IIR_THRE ) {/* THRE, transmit holding register empty */
	LSRValue = U0LSR;	/* Check status in the LSR to see if
								valid data in UxTHR or not */
	if ( LSRValue & LSR_THRE ) {
	  UART0TxEmpty = 1;
	}
	else {
	  UART0TxEmpty = 0;
	}
  }
  return;
}

/*****************************************************************************
** Function name:		UARTInit
**
** Descriptions:		Initialize UART0 port, setup pin select,
**						clock, parity, stop bits, FIFO, etc.
**
** parameters:			portNum(0 or 1) and UART baudrate
** Returned value:		true or false, return false only if the 
**						interrupt handler can't be installed to the 
**						VIC table
** 
*****************************************************************************/
void UART0Init( DWORD baudrate )
{
  DWORD Fdiv;

#if HITEX_BOARD
  SFSP1_22 = (0x01<<2)|(0x01<<0);	/* Digital no PU and PD, func. 1. */
  SFSP1_23 = (0x01<<2)|(0x01<<0);
#else
  SFSP0_16 = (0x01<<2)|(0x02<<0);	/* Digital no PU and PD, func. 2. */
  SFSP0_17 = (0x01<<2)|(0x02<<0);
#endif

  U0LCR &= ~(1<<7);		/* clear DLAB to disable IER */
  U0IER = 0x00;			/* Diable all UART0 interrupts */

  U0LCR |= (1<<7);			/* DLAB = 1 */
  U0LCR |= 0x03;		/* 8 bits, no Parity, 1 Stop bit */
  Fdiv = ( PERIPH_CLK / 16 ) / baudrate ;	/*baud rate */
  U0DLM = Fdiv / 256;							
  U0DLL = Fdiv % 256;
  U0LCR &= ~(1<<7);			/* DLAB = 0 */

#if UART_DMA_ENABLE
  U0FCR |= (1<<3)|(1<<0);	/* Enable DMA and FIFO */		
#else
  U0FCR |= (1<<0);			/* Enable FIFO */
#endif
  U0FCR |= (1<<2)|(1<<1);	/* reset TX and RX FIFO. */

  install_irq( 6, (void *)UART0Handler, HIGHEST_PRIORITY );
  EnableIntReq_IRQ( 6, ACTIVE_HIGH, HIGHEST_PRIORITY );
  U0IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART interrupt */
  return;
}

/*****************************************************************************
** Function name:		UARTSend
**
** Descriptions:		Send a block of data to the UART port based
**						on the data length
**
** parameters:			portNum, buffer pointer, and data length
** Returned value:		None
** 
*****************************************************************************/
void UART0Send(BYTE *BufferPtr, DWORD Length )
{
  while ( Length != 0 )
  {
	/* THRE status, contain valid data */
	while ( !(UART0TxEmpty & 0x01) );	
	U0THR = *BufferPtr;
	UART0TxEmpty = 0;	/* not empty in the THR until it shifts out */
	BufferPtr++;
	Length--;
  }
  return;
}

/*****************************************************************************
** Function name:		UARTxHandler
**
** Descriptions:		UARTx interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void UART1Handler (void)
{
  BYTE IIRValue, LSRValue;
  BYTE Dummy = Dummy;

  IIRValue = U1IIR;
    
  IIRValue >>= 1;			/* skip pending bit in IIR */
  IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
  if ( IIRValue == IIR_RLS ) {		/* Receive Line Status */
	LSRValue = U1LSR;
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) ) {
	  /* There are errors or break interrupt */
	  /* Read LSR will clear the interrupt */
	  Dummy = U1RBR;	/* Dummy read to clear interrupt, then bail out */
	  return;
	}
	if ( LSRValue & LSR_RDR ) {	/* Receive Data Ready */			
	  /* If no error on RLS, normal ready, save into the data buffer. */
	  /* Note: read RBR will clear the interrupt */
	  if ( UART1Count == BUFSIZE ) {
		UART1Count = 0;		/* buffer overflow */
	  }
	  UART1Buffer[UART1Count++] = U1RBR;	
	}
  }
  else if ( IIRValue == IIR_RDA ) {	/* Receive Data Available */
	/* Receive Data Available */
	if ( UART1Count == BUFSIZE ) {
	  UART1Count = 0;		/* buffer overflow */
	}
	UART1Buffer[UART1Count++] = U1RBR;
  }
  else if ( IIRValue == IIR_CTI ) {	/* Character timeout indicator */
	/* The mini. RX trigger level is 16, so, whenever RX FIFO is
	not empty, this CTI will be set after 4 character time, simply 
	read data from the FIFO when this interrupt occurs. */
	if ( UART1Count == BUFSIZE ) {
	  UART1Count = 0;		/* buffer overflow */
	}
	UART1Buffer[UART1Count++] = U1RBR;
  }
  else if ( IIRValue == IIR_THRE ) {/* THRE, transmit holding register empty */
	LSRValue = U1LSR;	/* Check status in the LSR to see if
								valid data in UxTHR or not */
	if ( LSRValue & LSR_THRE ) {
	  UART1TxEmpty = 1;
	}
	else {
	  UART1TxEmpty = 0;
	}
  }
  return;
}

/*****************************************************************************
** Function name:		UARTInit
**
** Descriptions:		Initialize UART0 port, setup pin select,
**						clock, parity, stop bits, FIFO, etc.
**
** parameters:			portNum(0 or 1) and UART baudrate
** Returned value:		true or false, return false only if the 
**						interrupt handler can't be installed to the 
**						VIC table
** 
*****************************************************************************/
void UART1Init( DWORD baudrate )
{
  DWORD Fdiv;

#if HITEX_BOARD
  SFSP2_16 = (0x01<<2)|(0x01<<0);	/* Digital no PU and PD, func. 1. */
  SFSP2_17 = (0x01<<2)|(0x01<<0);
#else
  /* Port2.16~17 for UART1 */
  SFSP2_16 = (0x01<<2)|(0x01<<0);	/* Digital no PU and PD, func. 1. */
  SFSP2_17 = (0x01<<2)|(0x01<<0);
#endif

  U1LCR &= ~(1<<7);		/* clear DLAB to disable IER */
  U1IER = 0x00;			/* Diable all UART0 interrupts */

  U1LCR |= (1<<7);		/* DLAB = 1 */
  U1LCR |= 0x03;	/* 8 bits, no Parity, 1 Stop bit */
  Fdiv = ( PERIPH_CLK / 16 ) / baudrate ;	/*baud rate */
  U1DLM = Fdiv / 256;							
  U1DLL = Fdiv % 256;
  U1LCR &= ~(1<<7);		/* DLAB = 0 */

#if UART_DMA_ENABLE
  U1FCR |= (1<<3)|(1<<0);	/* Enable DMA and FIFO */		
#else
  U1FCR |= (1<<0);			/* Enable FIFO */
#endif
  U1FCR |= (1<<2)|(1<<1);	/* reset TX and RX FIFO. */

  install_irq( 7, (void *)UART1Handler, HIGHEST_PRIORITY );
  EnableIntReq_IRQ( 7, ACTIVE_HIGH, HIGHEST_PRIORITY );
  U1IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART interrupt */
  return;
}

/*****************************************************************************
** Function name:		UARTSend
**
** Descriptions:		Send a block of data to the UART port based
**						on the data length
**
** parameters:			portNum, buffer pointer, and data length
** Returned value:		None
** 
*****************************************************************************/
void UART1Send(BYTE *BufferPtr, DWORD Length )
{
  while ( Length != 0 )
  {
	/* THRE status, contain valid data */
	while ( !(UART1TxEmpty & 0x01) );	
	U1THR = *BufferPtr;
	UART1TxEmpty = 0;	/* not empty in the THR until it shifts out */
	BufferPtr++;
	Length--;
  }
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
