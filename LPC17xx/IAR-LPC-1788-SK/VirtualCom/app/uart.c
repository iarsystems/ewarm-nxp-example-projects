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
#define UART_GLOBAL
#include "uart.h"

extern void NVIC_IntEnable(Int32U IntNumber);
extern void NVIC_IntDisable(Int32U IntNumber);
extern void NVIC_ClrPend(Int32U IntNumber);
extern void NVIC_IntPri(Int32U IntNumber, Int8U Priority);

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
  #define UART0_TX_IOCON      IOCON_P0_02
#endif /*UART0_TX_IOCON*/
#ifndef UART0_TX_IOCON_INIT
  #define UART0_TX_IOCON_INIT 0x21 
#endif /*UART0_TX_IOCON_INIT*/
#ifndef UART0_RX_IOCON      
  #define UART0_RX_IOCON      IOCON_P0_03
#endif /*UART0_TX_IOCON*/
#ifndef UART0_RX_IOCON_INIT
  #define UART0_RX_IOCON_INIT 0x21
#endif /*UART0_RX_IOCON_INIT*/

/*UART 1 default pin map*/
#ifndef UART1_TX_IOCON      
  #define UART1_TX_IOCON      IOCON_P0_15
#endif /*UART1_TX_IOCON*/
#ifndef UART1_TX_IOCON_INIT
  #define UART1_TX_IOCON_INIT 0x21 
#endif /*UART1_TX_IOCON_INIT*/
#ifndef UART1_RX_IOCON      
  #define UART1_RX_IOCON      IOCON_P0_16
#endif /*UART1_TX_IOCON*/
#ifndef UART1_RX_IOCON_INIT
  #define UART1_RX_IOCON_INIT 0x21
#endif /*UART1_RX_IOCON_INIT*/

/*UART 2 default pin map*/
#ifndef UART2_TX_IOCON      
  #define UART2_TX_IOCON      IOCON_P0_10
#endif /*UART2_TX_IOCON*/
#ifndef UART2_TX_IOCON_INIT
  #define UART2_TX_IOCON_INIT 0x21 
#endif /*UART2_TX_IOCON_INIT*/
#ifndef UART2_RX_IOCON      
  #define UART2_RX_IOCON      IOCON_P0_11
#endif /*UART2_TX_IOCON*/
#ifndef UART2_RX_IOCON_INIT
  #define UART2_RX_IOCON_INIT 0x21
#endif /*UART2_RX_IOCON_INIT*/

/*UART 3 default pin map*/
#ifndef UART3_TX_IOCON      
  #define UART3_TX_IOCON      IOCON_P0_00
#endif /*UART3_TX_IOCON*/
#ifndef UART3_TX_IOCON_INIT
  #define UART3_TX_IOCON_INIT 0x22 
#endif /*UART3_TX_IOCON_INIT*/
#ifndef UART3_RX_IOCON      
  #define UART3_RX_IOCON      IOCON_P0_01
#endif /*UART3_TX_IOCON*/
#ifndef UART3_RX_IOCON_INIT
  #define UART3_RX_IOCON_INIT 0x22
#endif /*UART3_RX_IOCON_INIT*/

/*UART 4 default pin map*/
#ifndef UART4_TX_IOCON      
  #define UART4_TX_IOCON      IOCON_P5_04
#endif /*UART4_TX_IOCON*/
#ifndef UART4_TX_IOCON_INIT
  #define UART4_TX_IOCON_INIT 0x34 
#endif /*UART4_TX_IOCON_INIT*/
#ifndef UART4_RX_IOCON      
  #define UART4_RX_IOCON      IOCON_P5_03
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
 * Parameters: pUartFifo_t Fifo, Int8U Data
 *
 * Return: Boolean
 *
 * Description: Push a char in a FIFO. Return TRUE when push is successful
 *  or FALSE when the FIFO is full.
 *
 *************************************************************************/
static Boolean FifoPush(pUartFifo_t Fifo, Int8U Data)
{
Int32U IndxTmp;

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
 * Parameters: pUartFifo_t Fifo, Int8U Data
 *
 * Return: Boolean
 *
 * Description: Pop a char from a FIFO. Return TRUE when pop is successful
 *  or FALSE when the FIFO is empty.
 *
 *************************************************************************/
static Boolean FifoPop(pUartFifo_t Fifo, pInt8U pData)
{
Int32U IndxTmp;

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
Int32U UartIntId = U0IIR, LineStatus, Counter;
Int8U Data;
  // Recognize the interrupt event
  switch (UartIntId & 0xF)
  {
  case RLS_INTR_ID: // Receive Line Status
  case CDI_INTR_ID: // Character Time-out Indicator
  case RDA_INTR_ID: // Receive Data Available
    // Read the line state of the UART
    LineStatus = U0LSR;
    do
    {
      if(LineStatus & RLS_OverrunError)
      {
        // Overrun Error
        Uart0LineEvents.bOE = TRUE;
      }
      Data = U0RBR;
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
      LineStatus = U0LSR;
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
      U0THR = Data;
    }
    break;
  }
  NVIC_ClrPend(NVIC_UART0);  // Clear interrupt in NVIC.
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
Int32U UartIntId = U1IIR, LineStatus, Counter;
Int8U Data;
  // Same like interrupt routine of UART0
  switch (UartIntId & 0xF)
  {
  case RLS_INTR_ID:
  case CDI_INTR_ID:
  case RDA_INTR_ID:
    LineStatus = U1LSR;
    do
    {
      if(LineStatus & RLS_OverrunError)
      {
        Uart1LineEvents.bOE = TRUE;
      }
      Data = U1RBR;
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
      LineStatus = U1LSR;
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
      U1THR = Data;
    }
    break;
#if UART1_MODEM_STAT_ENA > 0
  case MODEM_INTR_ID: // Modem Interrupt
    // Read the modem lines status
    Uart1ModemEvents.Data = U1MSR;
    break;
#endif // UART1_MODEM_STAT_ENA > 0
  }
  NVIC_ClrPend(NVIC_UART1);  // Clear interrupt in NVIC.
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
Int32U UartIntId = U2IIR, LineStatus, Counter;
Int8U Data;
  // Recognize the interrupt event
  switch (UartIntId & 0xF)
  {
  case RLS_INTR_ID: // Receive Line Status
  case CDI_INTR_ID: // Character Time-out Indicator
  case RDA_INTR_ID: // Receive Data Available
    // Read the line state of the UART
    LineStatus = U2LSR;
    do
    {
      if(LineStatus & RLS_OverrunError)
      {
        // Overrun Error
        Uart2LineEvents.bOE = TRUE;
      }
      Data = U2RBR;
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
      LineStatus = U2LSR;
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
      U2THR = Data;
    }
    break;
  }
  NVIC_ClrPend(NVIC_UART2);  // Clear interrupt in NVIC.
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
Int32U UartIntId = U3IIR, LineStatus, Counter;
Int8U Data;
  // Recognize the interrupt event
  switch (UartIntId & 0xF)
  {
  case RLS_INTR_ID: // Receive Line Status
  case CDI_INTR_ID: // Character Time-out Indicator
  case RDA_INTR_ID: // Receive Data Available
    // Read the line state of the UART
    LineStatus = U3LSR;
    do
    {
      if(LineStatus & RLS_OverrunError)
      {
        // Overrun Error
        Uart3LineEvents.bOE = TRUE;
      }
      Data = U3RBR;
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
      LineStatus = U3LSR;
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
      U3THR = Data;
    }
    break;
  }
  NVIC_ClrPend(NVIC_UART3);  // Clear interrupt in NVIC.
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
Int32U UartIntId = U4IIR, LineStatus, Counter;
Int8U Data;
  // Recognize the interrupt event
  switch (UartIntId & 0xF)
  {
  case RLS_INTR_ID: // Receive Line Status
  case CDI_INTR_ID: // Character Time-out Indicator
  case RDA_INTR_ID: // Receive Data Available
    // Read the line state of the UART
    LineStatus = U4LSR;
    do
    {
      if(LineStatus & RLS_OverrunError)
      {
        // Overrun Error
        Uart4LineEvents.bOE = TRUE;
      }
      Data = U4RBR;
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
      LineStatus = U4LSR;
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
      U4THR = Data;
    }
    break;
  }
  NVIC_ClrPend(NVIC_UART4);  // Clear interrupt in NVIC.
}

/*************************************************************************
 * Function Name: UartInit
 * Parameters: UartNum_t Uart, Int32U IrqSlot, UartMode_t UartMode
 *
 * Return: Boolean
 *
 * Description: Init UART
 *
 *************************************************************************/
Boolean UartInit(UartNum_t Uart,Int32U IrqSlot, UartMode_t UartMode)
{
volatile Int8U Tmp;
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
    PCONP_bit.PCUART0 = 1;
    // Assign Port to UART0
    UART0_TX_IOCON = UART0_TX_IOCON_INIT;     //Uart TX function select
    UART0_RX_IOCON = UART0_RX_IOCON_INIT;     //Uart RX function select

    U0LCR = 0x03; // Word Length =8, no parity , 1 stop
    U0FCR = 0x7;  // Enable and Clear the UART0 FIFO, Set RX FIFO interrupt level - 1 char
    // Transmit enable
    U0TER_bit.TXEN = 1;
    Tmp = U0IER;  // Clear pending interrupts
    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    U0IER = 0x07;

    NVIC_IntEnable(NVIC_UART0);
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
    PCONP_bit.PCUART1 = 1;
    // Assign Port to UART1
    UART1_TX_IOCON = UART1_TX_IOCON_INIT;     //Uart TX function select
    UART1_RX_IOCON = UART1_RX_IOCON_INIT;     //Uart RX function select

    U1LCR = 0x03; // Word Length =8, no parity , 1 stop
    U1MCR = 0;    // Word Length =8, no parity , 1 stop
    U1FCR = 0x7;  // Enable and Clear the UART1 FIFO, Set RX FIFO interrupt level - 1 char
    // Transmit enable
    U1TER_bit.TXEN = 1;
    Tmp = U1IER;  // Clear pending interrupts

    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    // Modem Status Interrupt, CTS Interrupt Enable
    U1IER = 0x07 | (UART1_MODEM_STAT_ENA?0x88:0);

    NVIC_IntEnable(NVIC_UART1);
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
    PCONP_bit.PCUART2 = 1;
    // Assign Port to UART2
    UART2_TX_IOCON = UART2_TX_IOCON_INIT;     //Uart TX function select
    UART2_RX_IOCON = UART2_RX_IOCON_INIT;     //Uart RX function select

    U2LCR = 0x03; // Word Length =8, no parity , 1 stop
    U2FCR = 0x7;  // Enable and Clear the UART2 FIFO, Set RX FIFO interrupt level - 1 char
    // Transmit enable
    U2TER_bit.TXEN = 1;
    Tmp = U2IER;  // Clear pending interrupts
    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    U2IER = 0x07;

    NVIC_IntEnable(NVIC_UART2);
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
    PCONP_bit.PCUART3 = 1;
    // Assign Port to UART3
    UART3_TX_IOCON = UART3_TX_IOCON_INIT;     //Uart TX function select
    UART3_RX_IOCON = UART3_RX_IOCON_INIT;     //Uart RX function select

    U3LCR = 0x03; // Word Length =8, no parity , 1 stop
    U3FCR = 0x7;  // Enable and Clear the UART3 FIFO, Set RX FIFO interrupt level - 1 char
    // Transmit enable
    U3TER_bit.TXEN = 1;
    Tmp = U3IER;  // Clear pending interrupts
    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    U3IER = 0x07;

    NVIC_IntEnable(NVIC_UART3);
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
    PCONP_bit.PCUART4 = 1;
    // Assign Port to UART4
    UART4_TX_IOCON = UART4_TX_IOCON_INIT;     //Uart TX function select
    UART4_RX_IOCON = UART4_RX_IOCON_INIT;     //Uart RX function select

    U4LCR = 0x03; // Word Length =8, no parity , 1 stop
    U4FCR = 0x7;  // Enable and Clear the UART4 FIFO, Set RX FIFO interrupt level - 1 char
    // Enable/Disable IrDA mode
    if(UartMode == IRDA)
    {
      U4ICR_bit.FIXPULSEEN = 0;
      U4ICR_bit.IRDAINV = 1;
      U4ICR_bit.IRDAEN = 1;
    }
    else
    {
      U4ICR_bit.IRDAEN = 0;
    }
    // Transmit enable
    U4TER_bit.TXEN = 1;
    Tmp = U4IER;  // Clear pending interrupts
    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    U4IER = 0x07;

    NVIC_IntEnable(NVIC_UART4);
    break;
  }
  return(TRUE);
}

/*************************************************************************
 * Function Name: UartCalcDivider
 * Parameters:  Int32U Freq, Int32U Baud
 *              pInt32U pDiv, pInt32U pAddDiv, pInt32U pMul
 *
 * Return: None
 *
 * Description: Calculate the coefficients of the UART baudrate generator
 *
 *************************************************************************/
static
void UartCalcDivider(Int32U Freq, Int32U Baud,
                     pInt32U pDiv, pInt32U pAddDiv, pInt32U pMul)
{
Int32U Temp, Error = (Int32U)-1;
Int32U K1, K2, K3, Baudrate;
Int32U DivTemp, MulTemp, AddDivTemp;

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
Int32U Mul, Div, AddDiv, Freq;

  // Check parameters
  if ((UartCoding.dwDTERate == 0) || (UartCoding.dwDTERate > UART_MAX_BAUD_RATE))
  {
    return;
  }
  switch (Uart)
  {
  case UART_0:
    Freq = CLK_GetClock(CLK_PERIPH);
    UartCalcDivider(Freq,UartCoding.dwDTERate,&Div,&AddDiv,&Mul);
    U0LCR_bit.WLS = UartCoding.bDataBits;
    U0LCR_bit.SBS = UartCoding.bStopBitsFormat;
    U0LCR_bit.PE  =(UartCoding.bParityType == UART_NO_PARITY)?0:1;
    U0LCR_bit.PS  = UartCoding.bParityType;
    U0LCR_bit.DLAB = 1;
    U0DLL = Div & 0xFF;
    U0DLM = (Div >> 8) & 0xFF;
    U0FDR = AddDiv + (Mul << 4);
    U0LCR_bit.DLAB = 0;
    break;
  case UART_1:
    Freq = CLK_GetClock(CLK_PERIPH);
    UartCalcDivider(Freq,UartCoding.dwDTERate,&Div,&AddDiv,&Mul);
    U1LCR_bit.WLS = UartCoding.bDataBits;
    U1LCR_bit.SBS = UartCoding.bStopBitsFormat;
    U1LCR_bit.PE  =(UartCoding.bParityType == UART_NO_PARITY)?0:1;
    U1LCR_bit.PS  = UartCoding.bParityType;
    U1LCR_bit.DLAB = 1;
    U1DLL = Div & 0xFF;
    U1DLM = (Div >> 8) & 0xFF;
    U1FDR = AddDiv + (Mul << 4);
    U1LCR_bit.DLAB = 0;
    break;
  case UART_2:
    Freq = CLK_GetClock(CLK_PERIPH);
    UartCalcDivider(Freq,UartCoding.dwDTERate,&Div,&AddDiv,&Mul);
    U2LCR_bit.WLS = UartCoding.bDataBits;
    U2LCR_bit.SBS = UartCoding.bStopBitsFormat;
    U2LCR_bit.PE  =(UartCoding.bParityType == UART_NO_PARITY)?0:1;
    U2LCR_bit.PS  = UartCoding.bParityType;
    U2LCR_bit.DLAB = 1;
    U2DLL = Div & 0xFF;
    U2DLM = (Div >> 8) & 0xFF;
    U2FDR = AddDiv + (Mul << 4);
    U2LCR_bit.DLAB = 0;
    break;
  case UART_3:
    Freq = CLK_GetClock(CLK_PERIPH);
    UartCalcDivider(Freq,UartCoding.dwDTERate,&Div,&AddDiv,&Mul);
    U3LCR_bit.WLS = UartCoding.bDataBits;
    U3LCR_bit.SBS = UartCoding.bStopBitsFormat;
    U3LCR_bit.PE  =(UartCoding.bParityType == UART_NO_PARITY)?0:1;
    U3LCR_bit.PS  = UartCoding.bParityType;
    U3LCR_bit.DLAB = 1;
    U3DLL = Div & 0xFF;
    U3DLM = (Div >> 8) & 0xFF;
    U3FDR = AddDiv + (Mul << 4);
    U3LCR_bit.DLAB = 0;
    break;
  case UART_4:
    Freq = CLK_GetClock(CLK_PERIPH);
    UartCalcDivider(Freq,UartCoding.dwDTERate,&Div,&AddDiv,&Mul);
    U4LCR_bit.WLS = UartCoding.bDataBits;
    U4LCR_bit.SBS = UartCoding.bStopBitsFormat;
    U4LCR_bit.PE  =(UartCoding.bParityType == UART_NO_PARITY)?0:1;
    U4LCR_bit.PS  = UartCoding.bParityType;
    U4LCR_bit.DLAB = 1;
    U4DLL = Div & 0xFF;
    U4DLM = (Div >> 8) & 0xFF;
    U4FDR = AddDiv + (Mul << 4);
    U4LCR_bit.DLAB = 0;
    break;
  }
}

/*************************************************************************
 * Function Name: UartRead
 * Parameters:  UartNum_t Uart, pInt8U pBuffer, Int32U BufferSize
 *
 * Return: Int32U
 *
 * Description: Read received data from UART.
 *              Return number of readied characters
 *
 *************************************************************************/
Int32U UartRead(UartNum_t Uart, pInt8U pBuffer, Int32U BufferSize)
{
Int32U Count;
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
 * Parameters:  UartNum_t Uart, pInt8U pBuffer, Int32U BufferSize
 *
 * Return: Int32U
 *
 * Description: Write a data to UART. Return number of successful
 *  transmitted bytes
 *
 *************************************************************************/
Int32U UartWrite(UartNum_t Uart, pInt8U pBuffer, Int32U BufferSize)
{
Int32U Count = 0;
pUartFifo_t pUartFifo;
Int32U save;

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
    volatile pInt8U pUartTxReg;
    save = __get_interrupt_state();
    __disable_interrupt();
    if((pUartFifo->PushIndx == pUartFifo->PopIndx))
    {
      // The Tx FIFO is empty
      switch (Uart)
      {
      case UART_0:
        pUartTxReg = (pInt8U)&U0THR;
        if(U0LSR_bit.THRE)
        {
          *pUartTxReg = *pBuffer;
          ++Count;
        }
        break;
      case UART_1:
        pUartTxReg = (pInt8U)&U1THR;
        if(U1LSR_bit.THRE)
        {
          *pUartTxReg = *pBuffer;
          ++Count;
        }
        break;
      case UART_2:
        pUartTxReg = (pInt8U)&U2THR;
        if(U2LSR_bit.THRE)
        {
          *pUartTxReg = *pBuffer;
          ++Count;
        }
        break;
      case UART_3:
        pUartTxReg = (pInt8U)&U3THR;
        if(U3LSR_bit.THRE)
        {
          *pUartTxReg = *pBuffer;
          ++Count;
        }
        break;
      case UART_4:
        pUartTxReg = (pInt8U)&U4THR;
        if(U4LSR_bit.THRE)
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
    U0LCR_bit.BC  = Break;
    break;
  case UART_1:
    U1LCR_bit.BC  = Break;
    break;
  case UART_2:
    U2LCR_bit.BC  = Break;
    break;
  case UART_3:
    U3LCR_bit.BC  = Break;
    break;
  case UART_4:
    U4LCR_bit.BC  = Break;
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
  U1MCR_bit.DTR = UartModemLineState.bDTR;
  U1MCR_bit.RTS = UartModemLineState.bRTS;
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
  ModemEvents.Data = AtomicExchange(ModemEvents.Data,(pInt32U)&Uart1ModemEvents);
  return(ModemEvents);
}
#endif // UART1_MODEM_STAT_ENA > 0

