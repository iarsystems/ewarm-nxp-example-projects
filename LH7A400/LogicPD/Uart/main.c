/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : May, 14 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the Logic PD LH7A400 evaluation boards.
 *  It shows basic use of UART2 and interrupt controller.
 *
 * This application is intended to show how to set up the UART to transmit
 * and receive characters. The UART2 configuration is 115,200 baud, 8-n-1 mode
 *
 *    $Revision: 34 $
 **************************************************************************/
#include <stdio.h>
#include <intrinsics.h>
#include <NXP/iolh7A400.h>
#include "board.h"
#include "arm_comm.h"

typedef enum
{
  UART_REC_CH = 0, UART_ERROR,
} Event_t;

typedef enum
{
  UART_FE = 0, UART_OE, UART_PE, UART_BD, UART_UE
} UartErrorType_t;

#pragma pack(1)
typedef struct
{
  Event_t   Event;
  unsigned char Value;
} EventBuf_t, *pEventBuf_t;
#pragma pack()


const char Startup [] =
"\n\r*************************************************************************\n\r"
"*  This example project shows how to use the IAR Embedded Workbench     *\n\r"
"* for ARM to develop code for the Logic PD LH7A400 evaluation boards.   *\n\r"
"* It shows basic use of UART2 and interrupt controller.                 *\n\r"
"*                                                                       *\n\r"
"*  This application is intended to show how to set up the UART2 to      *\n\r"
"* transmit and receive characters. The UART2 configuration is           *\n\r"
"* 115,200 baud, 8-n-1 mode                                              *\n\r"
"*************************************************************************\n\r";

const char String [] = "\n\rReceived character - %c [ASCII], %d [Dec]\n\r";
const char FErr [] = "\n\rFraming error\n\r";
const char OErr [] = "\n\rOverrun errror\n\r";
const char PErr [] = "\n\rParity errror\n\r";
const char BErr [] = "\n\rBreak detect\n\r";
const char UErr [] = "\n\rUnknow error\n\r";

char TxBuffer[200];
EventBuf_t EventBuf[200];

volatile char * volatile pTxData;

int EventPopInd, EventPushInd;
int EventBufferSize;
pEventBuf_t pEventBuf;

// function prototypes
void EventFifoInit(pEventBuf_t Event, int size);
int EventPush(Event_t Event, unsigned char Value);
int EventPop(pEventBuf_t Event);

VoidFpnt_t IntrHanders[32];

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ Handler
 *
 *************************************************************************/
__irq __arm void irq_handler(void)
{
unsigned long IntFlag = IC_INTSR, Mask = 1, Count = 0;
  while(IntFlag)
  {
    if(IntFlag & Mask)
    {
      IntFlag &= ~Mask;
      IntrHanders[Count]();
    }
    Mask <<= 1;
    ++Count;
  }
}

/*************************************************************************
 * Function Name: Uart_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: The UART interrupt Handler
 *
 *************************************************************************/
void Uart_handler(void)
{
Int32U Data;
  if (UART_ISR_2_bit.TIM)
  {
    // Fill Tx FIFO
    do
    {
      UART_DATA_2 = *pTxData++;
      if(*pTxData == 0)
      {
        pTxData = (char *)0;
        // Disable Tx FIFO interrupt
        UART_INTEN_2_bit.TEN  = 0;
        STAT_LED1_OFF();
        break;
      }
    }
    while(!UART_STATUS_2_bit.TXFF);
  }
  else
  {
    do
    {
      Data = UART_DATA_2;
      if (Data & (1UL<<10))
      {
        // Overrun Error Received
        EventPush(UART_ERROR,UART_OE);
        // Overrun Error Interrupt Clear
        UART_EIC_2_bit.OEIC = 1;
      }
      if (Data & (1UL<<11))
      {
        // Break Error Received
        EventPush(UART_ERROR,UART_BD);
        // Break Error Interrupt Clear
        UART_EIC_2_bit.BEIC = 1;
        // Framing Error Interrupt Clear
        UART_EIC_2_bit.FEIC = 1;
      } else if (Data & (1UL<<8))
      {
        // Framing Error Received
        EventPush(UART_ERROR,UART_FE);
        // Framing Error Interrupt Clear
        UART_EIC_2_bit.FEIC = 1;
      }
      if (Data & (1UL<<9))
      {
        // Parity Error Received
        EventPush(UART_ERROR,UART_PE);
        // Parity Error Interrupt Clear
        UART_EIC_2_bit.PEIC = 1;
      }
      if(!(Data & 0xF00))
      {
        // Character push
        EventPush(UART_REC_CH,Data);
      }
    }
    while(!UART_STATUS_2_bit.RXFE);
  }
  UART_EIC_2 = 0xF;               // Clear all erros flags
}

/*************************************************************************
 * Function Name: UartTranString
 * Parameters: const char * pStr
 *
 * Return: none
 *
 * Description: Init UART transmit
 *
 *************************************************************************/
void UartTranString (const char * pStr)
{
  if((pStr == NULL) || (*pStr == 0))
  {
    return;
  }
  // Wait until the Tx FIFO isn't full
  while(UART_STATUS_2_bit.TXFF);
  // Fill Tx FIFO
  do
  {
    UART_DATA_2 = *pStr++;
    if(*pStr == 0)
    {
      return;
    }
  } while (!UART_STATUS_2_bit.TXFF);
  if(*pStr != 0)
  {
    STAT_LED1_ON();
    pTxData = (char *)pStr;
    // Enable Tx interrupt
    UART_INTEN_2_bit.TEN  = 1;
  }
}

/*************************************************************************
 * Function Name: EventFifoInit
 * Parameters: pEventBuf_t pEvent, int Size
 *
 * Return: none
 *
 * Description: Init the events' FIFO
 *
 *************************************************************************/
void EventFifoInit(pEventBuf_t pEvent, int Size)
{
  EventPopInd = EventPushInd = 0;
  EventBufferSize = Size;
  pEventBuf = pEvent;
}

/*************************************************************************
 * Function Name: EventPush
 * Parameters: none
 *
 * Return: int - 1 pass
 *               0 overflow
 *
 * Description: Push event in the events' FIFO
 *
 *************************************************************************/
int EventPush(Event_t Event, unsigned char Value)
{
int PushTmp = EventPushInd + 1;
  // Avoid disable-enable interrupt in EventPop function
  if(EventPopInd == PushTmp)
  {
    return (0);
  }
  else if (PushTmp >= EventBufferSize)
  {
    PushTmp = 0;
    if(EventPopInd == PushTmp)
    {
      return (0);
    }
  }
  (pEventBuf+EventPushInd)->Event = Event;
  (pEventBuf+EventPushInd)->Value = Value;
  EventPushInd = PushTmp;
  return (1);
}

/*************************************************************************
 * Function Name: EventPush
 * Parameters: pEventBuf_t pEvent
 *
 * Return: int - 1 pass
 *               0 unedrflow
 *
 * Description: Pop event from the events' FIFO
 *
 *************************************************************************/
int EventPop(pEventBuf_t pEvent)
{
  if(EventPopInd == EventPushInd)
  {
    // The FIFO is empty
    return(0);
  }
  *pEvent = *(pEventBuf+EventPopInd);
  // Boundary check
  if(++EventPopInd >= EventBufferSize)
  {
    EventPopInd = 0;
  }
  return (1);
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: Main subroutine
 *
 *************************************************************************/
int main(void)
{
EventBuf_t Event;
unsigned char ch;
int NewData;
volatile int Dummy;

  // Init IO ports
  GPIO_LED_OFF();STAT_LED0_OFF();STAT_LED1_OFF();

  // Init VIC controller
  // Init VIC controller
  IC_INTENC = 0xFF;  // Diasable all interrupts
  for(int i = 0; i < sizeof(IntrHanders)/sizeof(VoidFpnt_t); ++i)
  {
    IntrHanders[i] = NULL;
  }

  // Init Uart
  CSC_PWRCNT_bit.UARTBAUD  = 0;   // Crystal oscillator output / 2
  // Control Register
  UART_CON_2_bit.UARTEN = 1;      // Enables UART operation
  UART_CON_2_bit.LBE    = 0;      // Disables loopback mode
  UART_CON_2_bit.TXP    = 0;      // UARTTXx active LOW and UARTIRTX1 active HIGH
  UART_CON_2_bit.RXP    = 0;      // UARTRXx and UARTIRRX1 active LOW
  // Init Baud rate
  UART_BRCON_2_bit.BAUDDIV = (MAIN_OSC_FREQ/(16*2*UART_BAUD_115200))-1;
  // FIFO and Framing Control Register
  UART_FCON_2_bit.PEN   = 0;      // Disables parity checking and generation.
  UART_FCON_2_bit.STP2  = 0;      // Transmit one stop bits
  UART_FCON_2_bit.FEN   = 1;      // Enable FIFO operation
  UART_FCON_2_bit.WLEN  = 3;      // 8-bit data
  // DMA Control Register
  UART_DMACR_2_bit.RDE  = 0;      // DMA for the receive direction is disabled
  UART_DMACR_2_bit.TDE  = 0;      // DMA for the transmit direction is disabled
  // Clear pending interrupts
  while(!UART_STATUS_2_bit.TXFE); // Tx FIFO is empty
  while(!UART_STATUS_2_bit.RXFE)
  {
    Dummy = UART_DATA_2;          // Drain the Rx FIFO
  }
  UART_EIC_2 = 0xF;               // Clear all erros flags
  // Init Interrupt Enable Register
  UART_INTEN_2_bit.REN  = 1;      // Receive Interrupt Enable
  UART_INTEN_2_bit.TEN  = 0;      // Transmit Interrupt Disable
  UART_INTEN_2_bit.RTEN = 1;      // Receive Timeout Interrupt Enable
  UART_INTEN_2_bit.FEEN = 1;      // Frame Error Interrupt Enable
  UART_INTEN_2_bit.BEEN = 1;      // Break Error Interrupt Enable
  UART_INTEN_2_bit.OEEN = 1;      // Overrun Error Interrupt Enable
  UART_INTEN_2_bit.PEEN = 0;      // Parity Error Interrupt Disable

  // registered the interrupt handler of UART
  // Set address of the handler
  IntrHanders[IC_UART2INTR] = Uart_handler;
  // Enable timer 1 interrupt
  IC_INTENS_bit.UART2INTR = 1;

  NewData = 0;
  EventFifoInit(EventBuf,sizeof(EventBuf)/sizeof(EventBuf_t));

  // Enable interrupts.
  __enable_interrupt();
  UartTranString(Startup);
  while(1)
  {
    if((pTxData == NULL) && (EventPop(&Event)))
    {
      if(Event.Event == UART_REC_CH)
      {
        if(Event.Value < ' ')
        {
          ch = ' ';
        }
        else
        {
          ch = Event.Value;
        }
        sprintf(TxBuffer,String,ch,Event.Value);
        NewData = 1;
      }
      else if (Event.Event == UART_ERROR)
      {
        switch (Event.Value)
        {
        case UART_FE:
          sprintf(TxBuffer,FErr);
          NewData = 1;
          break;
        case UART_OE:
          sprintf(TxBuffer,OErr);
          NewData = 1;
          break;
        case UART_PE:
          sprintf(TxBuffer,PErr);
          NewData = 1;
          break;
        case UART_BD:
          sprintf(TxBuffer,BErr);
          NewData = 1;
          break;
        case UART_UE:
          sprintf(TxBuffer,UErr);
          NewData = 1;
        }
      }
      if(NewData)
      {
        NewData = 0;
        // Send a string
        UartTranString(TxBuffer);
      }
    }
  }
}

