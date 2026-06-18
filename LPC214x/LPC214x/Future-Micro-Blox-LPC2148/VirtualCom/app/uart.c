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

pUartFifo_t pUart0RxFifo; // Pointer to a FIFO Buffer of the UART0 Receive
pUartFifo_t pUart0TxFifo; // Pointer to a FIFO Buffer of the UART0 Transmit
pUartFifo_t pUart1RxFifo; // Pointer to a FIFO Buffer of the UART1 Receive
pUartFifo_t pUart1TxFifo; // Pointer to a FIFO Buffer of the UART1 Transmit

// Hold UART0 Evens (PE, BI, FE, OE)
UartLineEvents_t Uart0LineEvents;

// Hold UART1 Evens (PE, BI, FE, OE)
UartLineEvents_t Uart1LineEvents;

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
 * Function Name: Uart0Isr
 * Parameters: none
 *
 * Return: none
 *
 * Description: UART 0 interrupt routine
 *
 *************************************************************************/
void Uart0Isr(void)
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
  VICVectAddr = 0;  // Clear interrupt in VIC.
}

/*************************************************************************
 * Function Name: Uart1Isr
 * Parameters: none
 *
 * Return: none
 *
 * Description: UART 1 interrupt routine
 *
 *************************************************************************/
void Uart1Isr(void)
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
  VICVectAddr = 0;  // Clear interrupt in VIC.
}

/*************************************************************************
 * Function Name: UartInit
 * Parameters: UartNum_t Uart,LPC_VicIrqSlots_t IrqSlot
 *
 * Return: Boolean
 *
 * Description: Init UART
 *
 *************************************************************************/
Boolean UartInit(UartNum_t Uart,LPC_VicIrqSlots_t IrqSlot)
{
volatile Int8U Tmp;
  if (Uart == UART_0)
  {
    pUart0RxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart0RxFifo == NULL)
    {
      return(FALSE);
    }
    pUart0TxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart0TxFifo == NULL)
    {
      return(FALSE);
    }
    // Init receive and transmit FIFOs
    pUart0RxFifo->PopIndx = pUart0RxFifo->PushIndx = \
    pUart0TxFifo->PopIndx = pUart0TxFifo->PushIndx = 0;

    // Assign Port 0,1 to UART0
    PINSEL0 &= ~0xF;
    PINSEL0 |=  0x5;

    U0LCR = 0x03; // Word Length =8, no parity , 1 stop
    U0FCR = 0x7;  // Enable and Clear the UART0 FIFO, Set RX FIFO interrupt level - 1 char
    // Transmit enable
    U0TER_bit.TxEn = 1;
    Tmp = U0IER;  // Clear pending interrupts
    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    U0IER = 0x07;

    VIC_SetVectoredIRQ(Uart0Isr,IrqSlot,VIC_UART0);
    VIC_EnableInt(1<<VIC_UART0);
  }
  else
  {
    pUart1RxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart1RxFifo == NULL)
    {
      return(FALSE);
    }
    pUart1TxFifo = (pUartFifo_t)malloc(sizeof(UartFifo_t));
    if(pUart1TxFifo == NULL)
    {
      return(FALSE);
    }
    // Init receive and transmit FIFOs
    pUart1RxFifo->PopIndx = pUart1RxFifo->PushIndx = \
    pUart1TxFifo->PopIndx = pUart1TxFifo->PushIndx = 0;

    // Assign Port 8,9 and 10, 11, 12, 13, 14, 15 to UART1
    PINSEL0 &= (UART1_MODEM_STAT_ENA?~0xFFFF0000:~0xF0000);
    PINSEL0 |= (UART1_MODEM_STAT_ENA? 0x55550000: 0x50000);

    U1LCR = 0x03; // Word Length =8, no parity , 1 stop
    U1MCR = 0;    // Word Length =8, no parity , 1 stop
    U1FCR = 0x7;  // Enable and Clear the UART0 FIFO, Set RX FIFO interrupt level - 1 char
    // Transmit enable
    U1TER_bit.TxEn = 1;
    Tmp = U1IER;  // Clear pending interrupts

    // enable RBR Interrupt, THRE Interrupt, RX Line Status Interrupt
    // Modem Status Interrupt, CTS Interrupt Enable
    U1IER = 0x07 | (UART1_MODEM_STAT_ENA?0x88:0);

    VIC_SetVectoredIRQ(Uart1Isr,IrqSlot,VIC_UART1);
    VIC_EnableInt(1<<VIC_UART1);
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
Int32U Div, Mul, AddDiv, Frequency;
  // Check parameters
  if ((UartCoding.dwDTERate == 0) || (UartCoding.dwDTERate > UART_MAX_BAUD_RATE))
  {
    return;
  }
  Frequency = SYS_GetFpclk();
  UartCalcDivider(Frequency,UartCoding.dwDTERate,&Div,&AddDiv,&Mul);
  if (Uart == UART_0)
  {
    U0LCR_bit.WLS = UartCoding.bDataBits;
    U0LCR_bit.SBS = UartCoding.bStopBitsFormat;
    U0LCR_bit.PE  =(UartCoding.bParityType == UART_NO_PARITY)?0:1;
    U0LCR_bit.PS  = UartCoding.bParityType;
    U0LCR_bit.DLAB = 1;
    U0DLL = Div & 0xFF;
    U0DLM = (Div >> 8) & 0xFF;
    U0FDR = AddDiv + (Mul << 4);
    U0LCR_bit.DLAB = 0;
  }
  else
  {
    U1LCR_bit.WLS = UartCoding.bDataBits;
    U1LCR_bit.SBS = UartCoding.bStopBitsFormat;
    U1LCR_bit.PE  =(UartCoding.bParityType == UART_NO_PARITY)?0:1;
    U1LCR_bit.PS  = UartCoding.bParityType;
    U1LCR_bit.DLAB = 1;
    U1DLL = Div & 0xFF;
    U1DLM = (Div >> 8) & 0xFF;
    U1FDR = AddDiv + (Mul << 4);
    U1LCR_bit.DLAB = 0;
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
pUartFifo_t pUartFifo = (Uart == UART_0)?pUart0RxFifo:pUart1RxFifo;
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
pUartFifo_t pUartFifo = (Uart == UART_0)?pUart0TxFifo:pUart1TxFifo;
  if(BufferSize != 0)
  {
    if (pUartFifo->PopIndx == pUartFifo->PushIndx)
    {
      // The Tx FIFO is empty
      pInt8U pUartTxReg = (pInt8U)((Uart == UART_0)?&U0THR:&U1THR);
      *pUartTxReg = *pBuffer;
      ++Count;
    }
    for ( ; Count < BufferSize; ++Count)
    {
      if(!FifoPush(pUartFifo,*(pBuffer+Count)))
      {
        break;
      }
    }
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
  LineEvents.Data = AtomicExchange(LineEvents.Data,
                                   (Uart == UART_0)?
                                   &Uart0LineEvents.Data:
                                   &Uart1LineEvents.Data);
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
  if(Uart == UART_0)
  {
    U0LCR_bit.BC  = Break;
  }
  else
  {
    U1LCR_bit.BC  = Break;
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

