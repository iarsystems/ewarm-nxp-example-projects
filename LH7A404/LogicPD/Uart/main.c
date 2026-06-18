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
 * for ARM to develop code for the Logic PD LH7A404 evaluation boards.
 *  It shows basic use of UART2 and interrupt controller.
 *
 * This application is intended to show how to set up the UART to transmit
 * and receive characters. The UART2 configuration is 115,200 baud, 8-n-1 mode
 *
 *    $Revision: 34 $
 **************************************************************************/
#include <intrinsics.h>
#include <stdio.h>
#include <string.h>
#include <NXP/iolh7A404.h>
#include "board.h"
#include "arm_comm.h"
#include "arm922t_cp15_drv.h"

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
"* for ARM to develop code for the Logic PD LH7A404 evaluation boards.   *\n\r"
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
void (*interrupt_function)();
unsigned int vector;
  vector = VIC_VECTADDR_1;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();    // Call vectored interrupt function
  VIC_VECTADDR_1 = 0;         // Clean interrupt in VIC
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
  UART_EIC_2 = 0xF;               // Clear all errors flags
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
 *               0 underflow
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
  GPIO_PEDD |= STAT_LED0 | STAT_LED1;   // PE6, PE7 outputs
  GPIO_LED_OFF();STAT_LED0_OFF();STAT_LED1_OFF();

  // Init VIC controller
  // Assign all interrupt channels to IRQ
  VIC_INTSEL_1 = VIC_INTSEL_2 = 0;
  // Disable all interrupts
  VIC_INTENCLR_1 = VIC_INTENCLR_2 = 0xFFFFFFFF;
  // Clear all software inerrutps
  VIC_SOFTINTCLR_1 = VIC_SOFTINTCLR_2 = 0xFFFFFFFF;
  // Clear interrupt
  VIC_VECTADDR_1 = VIC_VECTADDR_2 = 0;
  // Clear address of the Interrupt Service routine (ISR) for non-vectored IRQs.
  VIC_NVADDR_1 = VIC_NVADDR_2 = 0;

  // Clear address of the Interrupt Service routine (ISR) for vectored IRQs.
  VIC_VAD0_1  = 0;
  VIC_VAD1_1  = 0;
  VIC_VAD2_1  = 0;
  VIC_VAD3_1  = 0;
  VIC_VAD4_1  = 0;
  VIC_VAD5_1  = 0;
  VIC_VAD6_1  = 0;
  VIC_VAD7_1  = 0;
  VIC_VAD8_1  = 0;
  VIC_VAD9_1  = 0;
  VIC_VAD10_1 = 0;
  VIC_VAD11_1 = 0;
  VIC_VAD12_1 = 0;
  VIC_VAD13_1 = 0;
  VIC_VAD14_1 = 0;
  VIC_VAD15_1 = 0;
  VIC_VAD0_2  = 0;
  VIC_VAD1_2  = 0;
  VIC_VAD2_2  = 0;
  VIC_VAD3_2  = 0;
  VIC_VAD4_2  = 0;
  VIC_VAD5_2  = 0;
  VIC_VAD6_2  = 0;
  VIC_VAD7_2  = 0;
  VIC_VAD8_2  = 0;
  VIC_VAD9_2  = 0;
  VIC_VAD10_2 = 0;
  VIC_VAD11_2 = 0;
  VIC_VAD12_2 = 0;
  VIC_VAD13_2 = 0;
  VIC_VAD14_2 = 0;
  VIC_VAD15_2 = 0;

  // Disable all vectored IRQ slots
  VIC_VECTCNTL0_1  = 0;
  VIC_VECTCNTL1_1  = 0;
  VIC_VECTCNTL2_1  = 0;
  VIC_VECTCNTL3_1  = 0;
  VIC_VECTCNTL4_1  = 0;
  VIC_VECTCNTL5_1  = 0;
  VIC_VECTCNTL6_1  = 0;
  VIC_VECTCNTL7_1  = 0;
  VIC_VECTCNTL8_1  = 0;
  VIC_VECTCNTL9_1  = 0;
  VIC_VECTCNTL10_1 = 0;
  VIC_VECTCNTL11_1 = 0;
  VIC_VECTCNTL12_1 = 0;
  VIC_VECTCNTL13_1 = 0;
  VIC_VECTCNTL14_1 = 0;
  VIC_VECTCNTL15_1 = 0;
  VIC_VECTCNTL0_2  = 0;
  VIC_VECTCNTL1_2  = 0;
  VIC_VECTCNTL2_2  = 0;
  VIC_VECTCNTL3_2  = 0;
  VIC_VECTCNTL4_2  = 0;
  VIC_VECTCNTL5_2  = 0;
  VIC_VECTCNTL6_2  = 0;
  VIC_VECTCNTL7_2  = 0;
  VIC_VECTCNTL8_2  = 0;
  VIC_VECTCNTL9_2  = 0;
  VIC_VECTCNTL10_2 = 0;
  VIC_VECTCNTL11_2 = 0;
  VIC_VECTCNTL12_2 = 0;
  VIC_VECTCNTL13_2 = 0;
  VIC_VECTCNTL14_2 = 0;
  VIC_VECTCNTL15_2 = 0;

  // Init Uart
  CSC_PWRCNT_bit.UARTBAUD  = 1;   // Crystal oscillator output / 1
  // Control Register
  UART_CON_2_bit.UARTEN = 1;      // Enables UART operation
  UART_CON_2_bit.LBE    = 0;      // Disables loopback mode
  UART_CON_2_bit.TXP    = 0;      // UARTTXx active LOW and UARTIRTX1 active HIGH
  UART_CON_2_bit.RXP    = 0;      // UARTRXx and UARTIRRX1 active LOW
  // Init Baud rate
  UART_BRCON_2_bit.BAUDDIV = MAIN_OSC_FREQ/(16*UART_BAUD_115200)-1;
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
  UART_EIC_2 = 0xF;               // Clear all errors flags
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
  VIC_VAD0_2 = (unsigned long)Uart_handler;
  // UART2 interrupt line
  VIC_VECTCNTL0_2_bit.INTSOURCE = VIC2_UART2IN;
  // Enable slot
  VIC_VECTCNTL0_2_bit.EN = 1;
  // Enable UART interrupt
  VIC_INTEN_2_bit.UART2IN = 1;

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

