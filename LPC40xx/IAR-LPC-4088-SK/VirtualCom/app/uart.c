/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : uart.c
 *    Description : UARTs module
 *
 *    History :
 *    1. Date        : July 6, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <stdlib.h>
#include "LPC407x_8x.h"
#include "arm_comm.h"

#define UART_GLOBAL
#include "uart.h"

pUartFifo_t pUart0RxFifo; // Pointer to a FIFO Buffer of the UART0 Receive
pUartFifo_t pUart0TxFifo; // Pointer to a FIFO Buffer of the UART0 Transmit
pUartFifo_t pUart1RxFifo; // Pointer to a FIFO Buffer of the UART1 Receive
pUartFifo_t pUart1TxFifo; // Pointer to a FIFO Buffer of the UART1 Transmit
pUartFifo_t pUart2RxFifo; // Pointer to a FIFO Buffer of the UART2 Receive
pUartFifo_t pUart2TxFifo; // Pointer to a FIFO Buffer of the UART2 Transmit
pUartFifo_t pUart3RxFifo; // Pointer to a FIFO Buffer of the UART3 Receive
pUartFifo_t pUart3TxFifo; // Pointer to a FIFO Buffer of the UART3 Transmit
pUartFifo_t pUart4RxFifo; // Pointer to a FIFO Buffer of the UART4 Receive
pUartFifo_t pUart4TxFifo; // Pointer to a FIFO Buffer of the UART4 Transmit

/*UART 0 default pin map*/
#ifndef UART0_TX_IOCON      
  #define UART0_TX_IOCON      LPC_IOCON->P0_2
#endif /*UART0_TX_IOCON*/
#ifndef UART0_TX_IOCON_INIT
  #define UART0_TX_IOCON_INIT 0x21 
#endif /*UART0_TX_IOCON_INIT*/
#ifndef UART0_RX_IOCON      
  #define UART0_RX_IOCON      LPC_IOCON->P0_3
#endif /*UART0_TX_IOCON*/
#ifndef UART0_RX_IOCON_INIT
  #define UART0_RX_IOCON_INIT 0x21
#endif /*UART0_RX_IOCON_INIT*/

/*UART 1 default pin map*/
#ifndef UART1_TX_IOCON      
  #define UART1_TX_IOCON      LPC_IOCON->P0_15
#endif /*UART1_TX_IOCON*/
#ifndef UART1_TX_IOCON_INIT
  #define UART1_TX_IOCON_INIT 0x21 
#endif /*UART1_TX_IOCON_INIT*/
#ifndef UART1_RX_IOCON      
  #define UART1_RX_IOCON      LPC_IOCON->P0_16
#endif /*UART1_TX_IOCON*/
#ifndef UART1_RX_IOCON_INIT
  #define UART1_RX_IOCON_INIT 0x21
#endif /*UART1_RX_IOCON_INIT*/

/*UART 2 default pin map*/
#ifndef UART2_TX_IOCON      
  #define UART2_TX_IOCON      LPC_IOCON->P0_10
#endif /*UART2_TX_IOCON*/
#ifndef UART2_TX_IOCON_INIT
  #define UART2_TX_IOCON_INIT 0x21 
#endif /*UART2_TX_IOCON_INIT*/
#ifndef UART2_RX_IOCON      
  #define UART2_RX_IOCON      LPC_IOCON->P0_11
#endif /*UART2_TX_IOCON*/
#ifndef UART2_RX_IOCON_INIT
  #define UART2_RX_IOCON_INIT 0x21
#endif /*UART2_RX_IOCON_INIT*/

/*UART 3 default pin map*/
#ifndef UART3_TX_IOCON      
  #define UART3_TX_IOCON      LPC_IOCON->P0_0
#endif /*UART3_TX_IOCON*/
#ifndef UART3_TX_IOCON_INIT
  #define UART3_TX_IOCON_INIT 0x22 
#endif /*UART3_TX_IOCON_INIT*/
#ifndef UART3_RX_IOCON      
  #define UART3_RX_IOCON      LPC_IOCON->P0_1
#endif /*UART3_TX_IOCON*/
#ifndef UART3_RX_IOCON_INIT
  #define UART3_RX_IOCON_INIT 0x22
#endif /*UART3_RX_IOCON_INIT*/

/*UART 4 default pin map*/
#ifndef UART4_TX_IOCON      
  #define UART4_TX_IOCON      LPC_IOCON->P5_4
#endif /*UART4_TX_IOCON*/
#ifndef UART4_TX_IOCON_INIT
  #define UART4_TX_IOCON_INIT 0x34 
#endif /*UART4_TX_IOCON_INIT*/
#ifndef UART4_RX_IOCON      
  #define UART4_RX_IOCON      LPC_IOCON->P5_3
#endif /*UART4_TX_IOCON*/
#ifndef UART4_RX_IOCON_INIT
  #define UART4_RX_IOCON_INIT 0x04
#endif /*UART4_RX_IOCON_INIT*/

// Hold UART0 Evens (PE, BI, FE, OE)
UartLineEvents_t Uart0LineEvents;

// Hold UART1 Evens (PE, BI, FE, OE)
UartLineEvents_t Uart1LineEvents;

// Hold UART2 Evens (PE, BI, FE, OE)
UartLineEvents_t Uart2LineEvents;

// Hold UART3 Evens (PE, BI, FE, OE)
UartLineEvents_t Uart3LineEvents;

// Hold UART4 Evens (PE, BI, FE, OE)
UartLineEvents_t Uart4LineEvents;

#if UART1_MODEM_STAT_ENA > 0
// Hold the Modem Lines States of UART1
// ( Delta CTS, Delta DSR, Delta RI, Delta DCD,
//  CTS, DSR, RI and DCD )
UartModemEvents_t Uart1ModemEvents;
#endif // UART1_MODEM_STAT_ENA > 0

/*************************************************************************
 * Function Name: FifoPush
 * Parameters: pUartFifo_t Fifo, uint8_t Data
 *
 * Return: Boolean
 *
 * Description: Push a char in a FIFO. Return TRUE when push is successful
 *  or FALSE when the FIFO is full.
 *
 *************************************************************************/
static Boolean FifoPush(pUartFifo_t Fifo, uint8_t Data)
{
uint32_t IndxTmp;

  // calculate next push index
  IndxTmp = Fifo->PushIndx + 1;
  IndxTmp = IndxTmp % UART_FIFO_SIZE;

  // Check FIFO state
  if (IndxTmp == Fifo->PopIndx)
  {
    // The FIFO is full
    return(FALSE);
  }
  // Push the data
  Fifo->Buffer[Fifo->PushIndx] = Data;
  // Updating the push's index
  Fifo->PushIndx = IndxTmp;
  return(TRUE);
}

/*************************************************************************
 * Function Name: FifoPop
 * Parameters: pUartFifo_t Fifo, uint8_t Data
 *
 * Return: Boolean
 *
 * Description: Pop a char from a FIFO. Return TRUE when pop is successful
 *  or FALSE when the FIFO is empty.
 *
 *************************************************************************/
static Boolean FifoPop(pUartFifo_t Fifo, uint8_t * pData)
{
uint32_t IndxTmp;

  // Check FIFO state
  if (Fifo->PushIndx == Fifo->PopIndx)
  {
    // The FIFO is empty
    return(FALSE);
  }
  // Calculate the next pop index
  IndxTmp = Fifo->PopIndx + 1;
  IndxTmp = IndxTmp % UART_FIFO_SIZE;
  // Pop the data
  *pData = Fifo->Buffer[Fifo->PopIndx];
  // Updating of the pop's index
  Fifo->PopIndx = IndxTmp;
  return(TRUE);
}


/*************************************************************************
 * Function Name: UART0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: UART 0 interrupt routine
 *
 *************************************************************************/
void UART0_IRQHandler(void)
{
uint32_t UartIntId = LPC_UART0->IIR, LineStatus, Counter;
uint8_t Data;
  // Recognize the interrupt event
  switch (UartIntId & 0xF)
  {
  case RLS_INTR_ID: // Receive Line Status
  case CDI_INTR_ID: // Character Time-out Indicator
  case RDA_INTR_ID: // Receive Data Available
    // Read the line state of the UART
    LineStatus = LPC_UART0->LSR;
    do
    {
      if(LineStatus & RLS_OverrunError)
      {
        // Overrun Error
        Uart0LineEvents.bOE = TRUE;
      }
      Data = LPC_UART0->RBR;
      if (LineStatus & RLS_BreakInterruptr)
      {
        // Break Indicator
        Uart0LineEvents.bBI = TRUE;
      }
      else if (LineStatus & RLS_FramingError)
      {
        // Framing Error
        Uart0LineEvents.bFE = TRUE;
      }
      else if (LineStatus & RLS_ParityError)
      {
        // Parity Error
        Uart0LineEvents.bPE = TRUE;
      }
      // Push a new data into the receiver buffer
      if(!FifoPush(pUart0RxFifo,Data))
      {
        // the FIFO is full
        Uart0LineEvents.bOE = TRUE;
        break;
      }
      // Read the line state of the UART
      LineStatus = LPC_UART0->LSR;
    }
    while(LineStatus & RLS_ReceiverDataReady); // Is the hardware FIFO is empty?
    break;
  case THRE_INTR_ID:  // THRE Interrupt
    // Tx UART FIFO size - 1
    // Fill whole hardware transmit FIFO
    for (Counter = 15; Counter; --Counter)
    {
      // Pop a data from the transmit buffer
      if(!FifoPop(pUart0TxFifo,&Data))
      {
        // The tx software FIFO is empty
        break;
      }
      LPC_UART0->THR = Data;
    }
    break;
  }
  /*Data synchronization*/
  __DSB();
}

/*************************************************************************
 * Function Name: UART1_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: UART 1 interrupt routine
 *
 *************************************************************************/
void UART1_IRQHandler(void)
{
uint32_t UartIntId = LPC_UART1->IIR, LineStatus, Counter;
uint8_t Data;
  // Same like interrupt routine of UART0
  switch (UartIntId & 0xF)
  {
  case RLS_INTR_ID:
  case CDI_INTR_ID:
  case RDA_INTR_ID:
    LineStatus = LPC_UART1->LSR;
    do
    {
      if(LineStatus & RLS_OverrunError)
      {
        Uart1LineEvents.bOE = TRUE;
      }
      Data = LPC_UART1->RBR;
      if (LineStatus & RLS_BreakInterruptr)
      {
        Uart1LineEvents.bBI = TRUE;
      }
      else if (LineStatus & RLS_FramingError)
      {
        Uart1LineEvents.bFE = TRUE;
      }
      else if (LineStatus & RLS_ParityError)
      {
        Uart1LineEvents.bPE = TRUE;
      }
      if(!FifoPush(pUart1RxFifo,Data))
      {
        Uart1LineEvents.bOE = TRUE;
        break;
      }
      LineStatus = LPC_UART1->LSR;
    }
    while(LineStatus & RLS_ReceiverDataReady);
    break;
  case THRE_INTR_ID:
    // Tx UART FIFO size - 1
    for (Counter = 15; Counter; --Counter)
    {
      if(!FifoPop(pUart1TxFifo,&Data))
      {
        // The tx software FIFO is empty
        break;
      }
      LPC_UART1->THR = Data;
    }
    break;
#if UART1_MODEM_STAT_ENA > 0
  case MODEM_INTR_ID: // Modem Interrupt
    // Read the modem lines status
    Uart1ModemEvents.Data = LPC_UART1->MSR;
    break;
#endif // UART1_MODEM_STAT_ENA > 0
  }
  /*Data synchronization*/
  __DSB();
}

/*************************************************************************
 * Function Name: UART2_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: UART2 interrupt routine
 *
 *************************************************************************/
void UART2_IRQHandler(void)
{
uint32_t UartIntId = LPC_UART2->IIR, LineStatus, Counter;
uint8_t Data;
  // Recognize the interrupt event
  switch (UartIntId & 0xF)
  {
  case RLS_INTR_ID: // Receive Line Status
  case CDI_INTR_ID: // Character Time-out Indicator
  case RDA_INTR_ID: // Receive Data Available
    // Read the line state of the UART
    LineStatus = LPC_UART2->LSR;
    do
    {
      if(LineStatus & RLS_OverrunError)
      {
        // Overrun Error
        Uart2LineEvents.bOE = TRUE;
      }
      Data = LPC_UART2->RBR;
      if (LineStatus & RLS_BreakInterruptr)
      {
        // Break Indicator
        Uart2LineEvents.bBI = TRUE;
      }
      else if (LineStatus & RLS_FramingError)
      {
        // Framing Error
        Uart2LineEvents.bFE = TRUE;
      }
      else if (LineStatus & RLS_ParityError)
      {
        // Parity Error
        Uart2LineEvents.bPE = TRUE;
      }
      // Push a new data into the receiver buffer
      if(!FifoPush(pUart2RxFifo,Data))
      {
        // the FIFO is full
        Uart2LineEvents.bOE = TRUE;
        break;
      }
      // Read the line state of the UART
      LineStatus = LPC_UART2->LSR;
    }
    while(LineStatus & RLS_ReceiverDataReady); // Is the hardware FIFO is empty?
    break;
  case THRE_INTR_ID:  // THRE Interrupt
    // Tx UART FIFO size - 1
    // Fill whole hardware transmit FIFO
    for (Counter = 15; Counter; --Counter)
    {
      // Pop a data from the transmit buffer
      if(!FifoPop(pUart2TxFifo,&Data))
      {
        // The tx software FIFO is empty
        break;
      }
      LPC_UART2->THR = Data;
    }
    break;
  }
  /*Data synchronization*/
  __DSB();
}

/*************************************************************************
 * Function Name: UART3_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: UART3 interrupt routine
 *
 *************************************************************************/
void UART3_IRQHandler(void)
{
uint32_t UartIntId = LPC_UART3->IIR, LineStatus, Counter;
uint8_t Data;
  // Recognize the interrupt event
  switch (UartIntId & 0xF)
  {
  case RLS_INTR_ID: // Receive Line Status
  case CDI_INTR_ID: // Character Time-out Indicator
  case RDA_INTR_ID: // Receive Data Available
    // Read the line state of the UART
    LineStatus = LPC_UART3->LSR;
    do
    {
      if(LineStatus & RLS_OverrunError)
      {
        // Overrun Error
        Uart3LineEvents.bOE = TRUE;
      }
      Data = LPC_UART3->RBR;
      if (LineStatus & RLS_BreakInterruptr)
      {
        // Break Indicator
        Uart3LineEvents.bBI = TRUE;
      }
      else if (LineStatus & RLS_FramingError)
      {
        // Framing Error
        Uart3LineEvents.bFE = TRUE;
      }
      else if (LineStatus & RLS_ParityError)
      {
        // Parity Error
        Uart3LineEvents.bPE = TRUE;
      }
      // Push a new data into the receiver buffer
      if(!FifoPush(pUart3RxFifo,Data))
      {
        // the FIFO is full
        Uart3LineEvents.bOE = TRUE;
        break;
      }
      // Read the line state of the UART
      LineStatus = LPC_UART3->LSR;
    }
    while(LineStatus & RLS_ReceiverDataReady); // Is the hardware FIFO is empty?
    break;
  case THRE_INTR_ID:  // THRE Interrupt
    // Tx UART FIFO size - 1
    // Fill whole hardware transmit FIFO
    for (Counter = 15; Counter; --Counter)
    {
      // Pop a data from the transmit buffer
      if(!FifoPop(pUart3TxFifo,&Data))
      {
        // The tx software FIFO is empty
        break;
      }
      LPC_UART3->THR = Data;
    }
    break;
  }
  /*Data synchronization*/
  __DSB();
}

/*************************************************************************
 * Function Name: UART4_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: UART4 interrupt routine
 *
 *************************************************************************/
void UART4_IRQHandler(void)
{
uint32_t UartIntId = LPC_UART4->IIR, LineStatus, Counter;
uint8_t Data;
  // Recognize the interrupt event
  switch (UartIntId & 0xF)
  {
  case RLS_INTR_ID: // Receive Line Status
  case CDI_INTR_ID: // Character Time-out Indicator
  case RDA_INTR_ID: // Receive Data Available
    // Read the line state of the UART
    LineStatus = LPC_UART4->LSR;
    do
    {
      if(LineStatus & RLS_OverrunError)
      {
        // Overrun Error
        Uart4LineEvents.bOE = TRUE;
      }
      Data = LPC_UART4->RBR;
      if (LineStatus & RLS_BreakInterruptr)
      {
        // Break Indicator
        Uart4LineEvents.bBI = TRUE;
      }
      else if (LineStatus & RLS_FramingError)
      {
        // Framing Error
        Uart4LineEvents.bFE = TRUE;
      }
      else if (LineStatus & RLS_ParityError)
      {
        // Parity Error
        Uart4LineEvents.bPE = TRUE;
      }
      // Push a new data into the receiver buffer
      if(!FifoPush(pUart4RxFifo,Data))
      {
        // the FIFO is full
        Uart4LineEvents.bOE = TRUE;
        break;
      }
      // Read the line state of the UART
      LineStatus = LPC_UART4->LSR;
    }
    while(LineStatus & RLS_ReceiverDataReady); // Is the hardware FIFO is empty?
    break;
  case THRE_INTR_ID:  // THRE Interrupt
    // Tx UART FIFO size - 1
    // Fill whole hardware transmit FIFO
    for (Counter = 15; Counter; --Counter)
    {
      // Pop a data from the transmit buffer
      if(!FifoPop(pUart4TxFifo,&Data))
      {
        // The tx software FIFO is empty
        break;
      }
      LPC_UART4->THR = Data;
    }
    break;
  }
  /*Data synchronization*/
  __DSB();
}

/*************************************************************************
 * Function Name: UartInit
 * Parameters: UartNum_t Uart, uint32_t IrqSlot, UartMode_t UartMode
 *
 * Return: Boolean
 *
 * Description: Init UART
 *
 *************************************************************************/
Boolean UartInit(UartNum_t Uart,uint32_t IrqSlot, UartMode_t UartMode)
{
volatile uint8_t Tmp;
  switch (Uart)
  {
  case UART_0:
    pUart0RxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart0RxFifo == NULL)
    {
      return(FALSE);
    }
    pUart0TxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart0TxFifo == NULL)
    {
      free(pUart0RxFifo);
      return(FALSE);
    }
    // Init receive and transmit FIFOs
    pUart0RxFifo->PopIndx = pUart0RxFifo->PushIndx = \
    pUart0TxFifo->PopIndx = pUart0TxFifo->PushIndx = 0;

    // Enable UART0
    LPC_SC->PCONP |= (1<<3);
    // Assign Port to UART0
    UART0_TX_IOCON = UART0_TX_IOCON_INIT;     //Uart TX function select
    UART0_RX_IOCON = UART0_RX_IOCON_INIT;     //Uart RX function select

    LPC_UART0->LCR = 0x03; // Word Length =8, no parity , 1 stop
    LPC_UART0->FCR = 0x7;  // Enable and Clear the UART0 FIFO, Set RX FIFO interrupt level - 1 char
    // Transmit enable
    LPC_UART0->TER = (1<<7);
    Tmp = LPC_UART0->IER;  // Clear pending interrupts
    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    LPC_UART0->IER = 0x07;

    NVIC_EnableIRQ(UART0_IRQn);
    break;
  case UART_1:
    pUart1RxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart1RxFifo == NULL)
    {
      return(FALSE);
    }
    pUart1TxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart1TxFifo == NULL)
    {
      free(pUart1RxFifo);
      return(FALSE);
    }
    // Init receive and transmit FIFOs
    pUart1RxFifo->PopIndx = pUart1RxFifo->PushIndx = \
    pUart1TxFifo->PopIndx = pUart1TxFifo->PushIndx = 0;

    // Enable UART1
    LPC_SC->PCONP |= (1<<4);
    // Assign Port to UART1
    UART1_TX_IOCON = UART1_TX_IOCON_INIT;     //Uart TX function select
    UART1_RX_IOCON = UART1_RX_IOCON_INIT;     //Uart RX function select

    LPC_UART1->LCR = 0x03; // Word Length =8, no parity , 1 stop
    LPC_UART1->MCR = 0;    // Word Length =8, no parity , 1 stop
    LPC_UART1->FCR = 0x7;  // Enable and Clear the UART1 FIFO, Set RX FIFO interrupt level - 1 char
    // Transmit enable
    LPC_UART1->TER = (1<<7);
    Tmp = LPC_UART1->IER;  // Clear pending interrupts

    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    // Modem Status Interrupt, CTS Interrupt Enable
    LPC_UART1->IER = 0x07 | (UART1_MODEM_STAT_ENA?0x88:0);

    NVIC_EnableIRQ(UART1_IRQn);
    break;
  case UART_2:
    pUart2RxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart2RxFifo == NULL)
    {
      return(FALSE);
    }
    pUart2TxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart2TxFifo == NULL)
    {
      free(pUart2RxFifo);
      return(FALSE);
    }
    // Init receive and transmit FIFOs
    pUart2RxFifo->PopIndx = pUart2RxFifo->PushIndx = \
    pUart2TxFifo->PopIndx = pUart2TxFifo->PushIndx = 0;

    // Enable UART2
    LPC_SC->PCONP |= (1<<24);
    // Assign Port to UART2
    UART2_TX_IOCON = UART2_TX_IOCON_INIT;     //Uart TX function select
    UART2_RX_IOCON = UART2_RX_IOCON_INIT;     //Uart RX function select

    LPC_UART2->LCR = 0x03; // Word Length =8, no parity , 1 stop
    LPC_UART2->FCR = 0x7;  // Enable and Clear the UART2 FIFO, Set RX FIFO interrupt level - 1 char
    // Transmit enable
    LPC_UART2->TER = (1<<7);
    Tmp = LPC_UART2->IER;  // Clear pending interrupts
    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    LPC_UART2->IER = 0x07;

    NVIC_EnableIRQ(UART2_IRQn);
    break;
  case UART_3:
    pUart3RxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart3RxFifo == NULL)
    {
      return(FALSE);
    }
    pUart3TxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart3TxFifo == NULL)
    {
      free(pUart3RxFifo);
      return(FALSE);
    }
    // Init receive and transmit FIFOs
    pUart3RxFifo->PopIndx = pUart3RxFifo->PushIndx = \
    pUart3TxFifo->PopIndx = pUart3TxFifo->PushIndx = 0;

    // Enable UART3
    LPC_SC->PCONP |= (1<<25);
    // Assign Port to UART3
    UART3_TX_IOCON = UART3_TX_IOCON_INIT;     //Uart TX function select
    UART3_RX_IOCON = UART3_RX_IOCON_INIT;     //Uart RX function select

    LPC_UART3->LCR = 0x03; // Word Length =8, no parity , 1 stop
    LPC_UART3->FCR = 0x7;  // Enable and Clear the UART3 FIFO, Set RX FIFO interrupt level - 1 char
    // Transmit enable
    LPC_UART3->TER = (1<<7);
    Tmp = LPC_UART3->IER;  // Clear pending interrupts
    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    LPC_UART3->IER = 0x07;

    NVIC_EnableIRQ(UART3_IRQn);
    break;
  case UART_4:
    pUart4RxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart4RxFifo == NULL)
    {
      return(FALSE);
    }
    pUart4TxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart4TxFifo == NULL)
    {
      free(pUart4RxFifo);
      return(FALSE);
    }
    // Init receive and transmit FIFOs
    pUart4RxFifo->PopIndx = pUart4RxFifo->PushIndx = \
    pUart4TxFifo->PopIndx = pUart4TxFifo->PushIndx = 0;

    // Enable UART4
    LPC_SC->PCONP |= (1<<8);
    // Assign Port to UART4
    UART4_TX_IOCON = UART4_TX_IOCON_INIT;     //Uart TX function select
    UART4_RX_IOCON = UART4_RX_IOCON_INIT;     //Uart RX function select

    LPC_UART4->LCR = 0x03; // Word Length =8, no parity , 1 stop
    LPC_UART4->FCR = 0x7;  // Enable and Clear the UART4 FIFO, Set RX FIFO interrupt level - 1 char
    // Enable/Disable IrDA mode
    if(UartMode == IRDA)
    {
      LPC_UART4->ICR &= ~(1<<2);
      LPC_UART4->ICR |= (1<<1);
      LPC_UART4->ICR |= (1<<0);
    }
    else
    {
      LPC_UART4->ICR &= ~(1<<0);
    }
    // Transmit enable
    LPC_UART4->TER = (1<<7);
    Tmp = LPC_UART4->IER;  // Clear pending interrupts
    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    LPC_UART4->IER = 0x07;

    NVIC_EnableIRQ(UART4_IRQn);
    break;
  }
  return(TRUE);
}

/*************************************************************************
 * Function Name: UartCalcDivider
 * Parameters:  uint32_t Freq, uint32_t Baud
 *              uint32_t * pDiv, uint32_t * pAddDiv, uint32_t * pMul
 *
 * Return: None
 *
 * Description: Calculate the coefficients of the UART baudrate generator
 *
 *************************************************************************/
static
void UartCalcDivider(uint32_t Freq, uint32_t Baud,
                     uint32_t * pDiv, uint32_t * pAddDiv, uint32_t * pMul)
{
uint32_t Temp, Error = (uint32_t)-1;
uint32_t K1, K2, K3, Baudrate;
uint32_t DivTemp, MulTemp, AddDivTemp;

  //
  for(MulTemp = 1; MulTemp < 16; ++MulTemp)
  {
    K1 = Freq*MulTemp;
    for(AddDivTemp = 1; AddDivTemp < 16; ++AddDivTemp)
    {
      K3 = (MulTemp + AddDivTemp)<<4;
      K2 =  K3 * Baud;
      DivTemp = K1/K2;
      // if DIVADDVAL>0, UnDL must be UnDL >= 0x0002 or the UART will
      // not operate at the desired baud-rate!
      if(DivTemp < 2)
      {
        continue;
      }
      Baudrate = DivTemp * K3;
      Baudrate = K1/Baudrate;
      Temp = (Baudrate > Baud)? \
                 (Baudrate - Baud): \
                 (Baud - Baudrate);
      if (Temp < Error)
      {
        Error = Temp;
        *pDiv = DivTemp;
        *pMul = MulTemp;
        *pAddDiv = AddDivTemp;
        if(Error == 0)
        {
          return;
        }
      }
    }
  }
}

/*************************************************************************
 * Function Name: UartSetLineCoding
 * Parameters:  UartNum_t Uart,UartLineCoding_t pUartCoding
 *
 * Return: None
 *
 * Description: Init UART Baud rate, Word width, Stop bits, Parity type
 *
 *************************************************************************/
void UartSetLineCoding(UartNum_t Uart,UartLineCoding_t UartCoding)
{
uint32_t Mul, Div, AddDiv, Freq;
uint8_t tmp;
  // Check parameters
  if ((UartCoding.dwDTERate == 0) || (UartCoding.dwDTERate > UART_MAX_BAUD_RATE))
  {
    return;
  }
  switch (Uart)
  {
  case UART_0:
    Freq = PeripheralClock;
    UartCalcDivider(Freq,UartCoding.dwDTERate,&Div,&AddDiv,&Mul);
    tmp = (UartCoding.bDataBits & 0x3) | ((UartCoding.bStopBitsFormat&0x1)<<2) |
          ((UartCoding.bParityType == UART_NO_PARITY)?0:(1<<3)) |
          ((UartCoding.bParityType&0x3)<<4);
    LPC_UART0->LCR = tmp;
    LPC_UART0->LCR |= (1<<7);
    LPC_UART0->DLL = Div & 0xFF;
    LPC_UART0->DLM = (Div >> 8) & 0xFF;
    LPC_UART0->FDR = AddDiv + (Mul << 4);
    LPC_UART0->LCR &= ~(1<<7);
    break;
  case UART_1:
    Freq = PeripheralClock;
    UartCalcDivider(Freq,UartCoding.dwDTERate,&Div,&AddDiv,&Mul);
    tmp = (UartCoding.bDataBits & 0x3) | ((UartCoding.bStopBitsFormat&0x1)<<2) |
          ((UartCoding.bParityType == UART_NO_PARITY)?0:(1<<3)) |
          ((UartCoding.bParityType&0x3)<<4);
    LPC_UART1->LCR = tmp;
    LPC_UART1->LCR |= (1<<7);
    LPC_UART1->DLL = Div & 0xFF;
    LPC_UART1->DLM = (Div >> 8) & 0xFF;
    LPC_UART1->FDR = AddDiv + (Mul << 4);
    LPC_UART1->LCR &= ~(1<<7);
    break;
  case UART_2:
    Freq = PeripheralClock;
    UartCalcDivider(Freq,UartCoding.dwDTERate,&Div,&AddDiv,&Mul);
    tmp = (UartCoding.bDataBits & 0x3) | ((UartCoding.bStopBitsFormat&0x1)<<2) |
          ((UartCoding.bParityType == UART_NO_PARITY)?0:(1<<3)) |
          ((UartCoding.bParityType&0x3)<<4);
    LPC_UART2->LCR = tmp;
    LPC_UART2->LCR |= (1<<7);
    LPC_UART2->DLL = Div & 0xFF;
    LPC_UART2->DLM = (Div >> 8) & 0xFF;
    LPC_UART2->FDR = AddDiv + (Mul << 4);
    LPC_UART2->LCR &= ~(1<<7);
    break;
  case UART_3:
    Freq = PeripheralClock;
    UartCalcDivider(Freq,UartCoding.dwDTERate,&Div,&AddDiv,&Mul);
    tmp = (UartCoding.bDataBits & 0x3) | ((UartCoding.bStopBitsFormat&0x1)<<2) |
          ((UartCoding.bParityType == UART_NO_PARITY)?0:(1<<3)) |
          ((UartCoding.bParityType&0x3)<<4);
    LPC_UART3->LCR = tmp;
    LPC_UART3->LCR |= (1<<7);
    LPC_UART3->DLL = Div & 0xFF;
    LPC_UART3->DLM = (Div >> 8) & 0xFF;
    LPC_UART3->FDR = AddDiv + (Mul << 4);
    LPC_UART3->LCR &= ~(1<<7);
    break;
  case UART_4:
    Freq = PeripheralClock;
    UartCalcDivider(Freq,UartCoding.dwDTERate,&Div,&AddDiv,&Mul);
    tmp = (UartCoding.bDataBits & 0x3) | ((UartCoding.bStopBitsFormat&0x1)<<2) |
          ((UartCoding.bParityType == UART_NO_PARITY)?0:(1<<3)) |
          ((UartCoding.bParityType&0x3)<<4);
    LPC_UART4->LCR = tmp;
    LPC_UART4->LCR |= (1<<7);
    LPC_UART4->DLL = Div & 0xFF;
    LPC_UART4->DLM = (Div >> 8) & 0xFF;
    LPC_UART4->FDR = AddDiv + (Mul << 4);
    LPC_UART4->LCR &= ~(1<<7);
    break;
  }
}

/*************************************************************************
 * Function Name: UartRead
 * Parameters:  UartNum_t Uart, uint8_t * pBuffer, uint32_t BufferSize
 *
 * Return: uint32_t
 *
 * Description: Read received data from UART.
 *              Return number of readied characters
 *
 *************************************************************************/
uint32_t UartRead(UartNum_t Uart, uint8_t * pBuffer, uint32_t BufferSize)
{
uint32_t Count;
pUartFifo_t pUartFifo;
  switch (Uart)
  {
  case UART_0:
    pUartFifo= pUart0RxFifo;
    break;
  case UART_1:
    pUartFifo= pUart1RxFifo;
    break;
  case UART_2:
    pUartFifo= pUart2RxFifo;
    break;
  case UART_3:
    pUartFifo= pUart3RxFifo;
    break;
  case UART_4:
    pUartFifo= pUart4RxFifo;
    break;
  default:
    return(0);
  }

  for (Count = 0; Count < BufferSize; ++Count)
  {
    if(!FifoPop(pUartFifo,pBuffer+Count))
    {
      break;
    }
  }
  return(Count);
}

/*************************************************************************
 * Function Name: UartWrite
 * Parameters:  UartNum_t Uart, uint8_t * pBuffer, uint32_t BufferSize
 *
 * Return: uint32_t
 *
 * Description: Write a data to UART. Return number of successful
 *  transmitted bytes
 *
 *************************************************************************/
uint32_t UartWrite(UartNum_t Uart, uint8_t * pBuffer, uint32_t BufferSize)
{
uint32_t Count = 0;
pUartFifo_t pUartFifo;
uint32_t save;

  switch (Uart)
  {
  case UART_0:
    pUartFifo= pUart0TxFifo;
    break;
  case UART_1:
    pUartFifo= pUart1TxFifo;
    break;
  case UART_2:
    pUartFifo= pUart2TxFifo;
    break;
  case UART_3:
    pUartFifo= pUart3TxFifo;
    break;
  case UART_4:
    pUartFifo= pUart4TxFifo;
    break;
  default:
    return(0);
  }

  if(BufferSize != 0)
  {
    volatile uint8_t * pUartTxReg;
    save = __get_interrupt_state();
    __disable_interrupt();
    if((pUartFifo->PushIndx == pUartFifo->PopIndx))
    {
      // The Tx FIFO is empty
      switch (Uart)
      {
      case UART_0:
        pUartTxReg = (uint8_t *)&LPC_UART0->THR;
        if(LPC_UART0->LSR & (1<<5))
        {
          *pUartTxReg = *pBuffer;
          ++Count;
        }
        break;
      case UART_1:
        pUartTxReg = (uint8_t *)&LPC_UART1->THR;
        if(LPC_UART1->LSR & (1<<5))
        {
          *pUartTxReg = *pBuffer;
          ++Count;
        }
        break;
      case UART_2:
        pUartTxReg = (uint8_t *)&LPC_UART2->THR;
        if(LPC_UART2->LSR & (1<<5))
        {
          *pUartTxReg = *pBuffer;
          ++Count;
        }
        break;
      case UART_3:
        pUartTxReg = (uint8_t *)&LPC_UART3->THR;
        if(LPC_UART3->LSR & (1<<5))
        {
          *pUartTxReg = *pBuffer;
          ++Count;
        }
        break;
      case UART_4:
        pUartTxReg = (uint8_t *)&LPC_UART4->THR;
        if(LPC_UART4->LSR & (1<<5))
        {
          *pUartTxReg = *pBuffer;
          ++Count;
        }
        break;
      }
    }
    for ( ; Count < BufferSize; ++Count)
    {
      if(!FifoPush(pUartFifo,*(pBuffer+Count)))
      {
        break;
      }
    }
    __set_interrupt_state(save);
  }
  return(Count);
}

/*************************************************************************
 * Function Name: UartGetUartEvents
 * Parameters:  UartNum_t Uart
 *
 * Return: UartLineEvents_t
 *
 * Description: Get Uart Line events (PE,OE, FE, BI)
 *
 *************************************************************************/
UartLineEvents_t UartGetUartLineEvents (UartNum_t Uart)

{
UartLineEvents_t  LineEvents;
  LineEvents.Data = 0;

  switch (Uart)
  {
  case UART_0:
    AtomicExchange(LineEvents.Data,&Uart0LineEvents.Data);
    break;
  case UART_1:
    AtomicExchange(LineEvents.Data,&Uart1LineEvents.Data);
    break;
  case UART_2:
    AtomicExchange(LineEvents.Data,&Uart2LineEvents.Data);
    break;
  case UART_3:
    AtomicExchange(LineEvents.Data,&Uart3LineEvents.Data);
    break;
  case UART_4:
    AtomicExchange(LineEvents.Data,&Uart4LineEvents.Data);
    break;
  default:
    LineEvents.Data = 0;
  }
  return(LineEvents);
}

/*************************************************************************
 * Function Name: UartSetUartLineState
 * Parameters:  UartNum_t Uart UartNum_t Uart, Boolean Break
 *
 * Return: none
 *
 * Description: Set Uart Break Event
 *
 *************************************************************************/
void UartSetUartLineState (UartNum_t Uart, Boolean Break)
{
  switch (Uart)
  {
  case UART_0:
    if(Break == TRUE)
    {
      LPC_UART0->LCR  |= (1<<6);
    }
    else
    {
      LPC_UART0->LCR  &= ~(1<<6);
    }  
    break;
  case UART_1:
    if(Break == TRUE)
    {
      LPC_UART1->LCR  |= (1<<6);
    }
    else
    {
      LPC_UART1->LCR  &= ~(1<<6);
    }  
    break;
  case UART_2:
    if(Break == TRUE)
    {
      LPC_UART2->LCR  |= (1<<6);
    }
    else
    {
      LPC_UART2->LCR  &= ~(1<<6);
    }  
    break;
  case UART_3:
    if(Break == TRUE)
    {
      LPC_UART3->LCR  |= (1<<6);
    }
    else
    {
      LPC_UART3->LCR  &= ~(1<<6);
    }  
    break;
  case UART_4:
    if(Break == TRUE)
    {
      LPC_UART4->LCR  |= (1<<6);
    }
    else
    {
      LPC_UART4->LCR  &= ~(1<<6);
    }  
    break;
  }
}

#if UART1_MODEM_STAT_ENA > 0
/*************************************************************************
 * Function Name: Uart1SetModemLineState
 * Parameters:  UartModemLineState_t UartModemLineState
 *
 * Return: none
 *
 * Description: Init UART1 Modem lines state (RTS, DTR)
 *
 *************************************************************************/
void Uart1SetModemLineState(UartModemLineState_t UartModemLineState)
{
uint8_t tmp = LPC_UART1->MCR & ~(0x3);

  LPC_UART1->MCR = tmp | UartModemLineState.Data;    
}

/*************************************************************************
 * Function Name: Uart1GetUartModemEvents
 * Parameters:  none
 *
 * Return: UartModemEvents_t
 *
 * Description: Get Uart1 Modem lines events (DCD,DSR,CTS,RI)
 *
 *************************************************************************/
UartModemEvents_t Uart1GetUartModemEvents (void)
{
UartModemEvents_t  ModemEvents;
  ModemEvents.Data = 0;
  ModemEvents.Data = AtomicExchange(ModemEvents.Data,(uint32_t *)&Uart1ModemEvents);
  return(ModemEvents);
}
#endif // UART1_MODEM_STAT_ENA > 0

