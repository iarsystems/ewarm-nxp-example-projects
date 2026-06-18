/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : May, 12 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the Logic PD LH79524 evaluation boards.
 *  It shows basic use of the UART0 and the interrupt controller.
 *
 * This application is intended to show how to set up the UART to transmit
 * and receive characters. The UART0 configuration is 115,200 baud, 8-n-1 mode
 *
 *    $Revision: 34 $
 **************************************************************************/
#include <intrinsics.h>
#include <stdio.h>
#include <string.h>
#include <NXP/iolh79524.h>
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
"* for ARM to develop code for the Logic PD LH79524 evaluation boards.   *\n\r"
"* It shows basic use of the UART0 and the interrupt controller.         *\n\r"
"*                                                                       *\n\r"
"*  This application is intended to show how to set up the UART2 to      *\n\r"
"* transmit and receive characters. The UART0 configuration is           *\n\r"
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
  vector = VIC_VectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();  // Call vectored interrupt function
  VIC_VectAddr = 0;         // Clean interrupt in VIC
}

/*************************************************************************
 * Function Name: Uart_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: The UART2 interrupt Handler
 *
 *************************************************************************/
void Uart_handler(void)
{
Int32U Data;
  if (UART0_UARTMIS_bit.TXMIS)
  {
    // Transmit Interrupt
    UART0_UARTICR_bit.TXIC = 1;
    // Fill Tx FIFO
    do
    {
      UART0_UARTDR = *pTxData++;
      if(*pTxData == 0)
      {
        pTxData = (char *)0;
        // Disable Tx FIFO interrupt
        UART0_UARTIMSC_bit.TXIM  = 0;
        STAT_LED1_OFF();
        break;
      }
    }
    while(!UART0_UARTFR_bit.TXFF);
  }
  else
  {
    do
    {
      Data = UART0_UARTDR;
      if(UART0_UARTRIS_bit.OERIS)
      {
        // Data Overrun Error
        EventPush(UART_ERROR,UART_OE);
        // Overrun Error Interrupt Clear
        UART0_UARTICR_bit.OEIC = 1;
      }
      else if (UART0_UARTRIS_bit.BERIS)
      {
        // Break Error
        EventPush(UART_ERROR,UART_BD);
        // Overrun Error Interrupt Clear
        UART0_UARTICR_bit.BEIC = 1;
      }
      else if (UART0_UARTRIS_bit.PEARIS)
      {
        // Parity Error Received
        EventPush(UART_ERROR,UART_PE);
        // Overrun Error Interrupt Clear
        UART0_UARTICR_bit.PEARIC = 1;
      }
      else if (UART0_UARTRIS_bit.FERIS)
      {
        // Parity Error Received
        EventPush(UART_ERROR,UART_FE);
        // Overrun Error Interrupt Clear
        UART0_UARTICR_bit.FEIC = 1;
      }
      else
      {
        // Character push
        EventPush(UART_REC_CH,Data);
      }
    }
    while(!UART0_UARTFR_bit.RXFE);
    // Receive Interrupt
    UART0_UARTICR_bit.RXIC = 1;
    // Receive Timeout Interrupt Clear
    UART0_UARTICR_bit.RTIC = 1;
  }
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
  while(UART0_UARTFR_bit.TXFF);
  // Fill Tx FIFO
  do
  {
    UART0_UARTDR = *pStr++;
    if(*pStr == 0)
    {
      return;
    }
  } while (!UART0_UARTFR_bit.TXFF);
  if(*pStr != 0)
  {
    STAT_LED1_ON();
    pTxData = (char *)pStr;
    // Enable Tx interrupt
    UART0_UARTIMSC_bit.TXIM  = 1;
  }
  UART0_UARTCR_bit.TXE     = 1;   // Transmit section enabled
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
  CPLD_GPIO_DIR = 0;          // CPLD GPOI1,2 outputs
  GPIO_LED_OFF();STAT_LED0_OFF();STAT_LED1_OFF();

  // Init VIC controller
  // Assign all interrupt channels to IRQ
  VIC_IntSelect  =  0;
  // Disable all interrupts
  VIC_IntEnClear = 0xFFFFFFFF;
  // Clear all software interrupts
  VIC_SoftIntClear = 0xFFFFFFFF;
  // Clear interrupt
  VIC_VectAddr = 0;
  // Clear address of the Interrupt Service routine (ISR) for non-vectored IRQs.
  VIC_DefVectAddr = 0;

  // Clear address of the Interrupt Service routine (ISR) for vectored IRQs.
  VIC_VectAddr0  = 0;
  VIC_VectAddr1  = 0;
  VIC_VectAddr2  = 0;
  VIC_VectAddr3  = 0;
  VIC_VectAddr4  = 0;
  VIC_VectAddr5  = 0;
  VIC_VectAddr6  = 0;
  VIC_VectAddr7  = 0;
  VIC_VectAddr8  = 0;
  VIC_VectAddr9  = 0;
  VIC_VectAddr10 = 0;
  VIC_VectAddr11 = 0;
  VIC_VectAddr12 = 0;
  VIC_VectAddr13 = 0;
  VIC_VectAddr14 = 0;
  VIC_VectAddr15 = 0;

  // Disable all vectored IRQ slots
  VIC_VectCtrl0  = 0;
  VIC_VectCtrl1  = 0;
  VIC_VectCtrl2  = 0;
  VIC_VectCtrl3  = 0;
  VIC_VectCtrl4  = 0;
  VIC_VectCtrl5  = 0;
  VIC_VectCtrl6  = 0;
  VIC_VectCtrl7  = 0;
  VIC_VectCtrl8  = 0;
  VIC_VectCtrl9  = 0;
  VIC_VectCtrl10 = 0;
  VIC_VectCtrl11 = 0;
  VIC_VectCtrl12 = 0;
  VIC_VectCtrl13 = 0;
  VIC_VectCtrl14 = 0;
  VIC_VectCtrl15 = 0;

  // Init Uart
  PCLKCTRL0_bit.U0   = 1;
  PCLKSEL0_bit.UART0 = 0;   // Crystal oscillator output
  // Init Baud rate
  UART0_UARTIBRD_bit.BAUDDIVINT = MAIN_OSC_FREQ/(16*UART_BAUD_115200);
  UART0_UARTFBRD_bit.FRAC = ((MAIN_OSC_FREQ/16)%UART_BAUD_115200)/(UART_BAUD_115200>>6);
  // Enable the UART0 clock.
  // !!! The Baud rate must be set before UART clock enable !!!
  PCLKCTRL0_bit.U0   = 0;
  // Assign pins to UART
  MUXCTL5_bit.PB6    = 2;   // UART0 Rx
  MUXCTL5_bit.PB7    = 2;   // UART0 Tx
  // Init UART Control Register
  UART0_UARTCR_bit.UARTEN  = 1;   // Enables the UART
  UART0_UARTCR_bit.SIREN   = 0;   // SIR Disabled
  UART0_UARTCR_bit.LBE     = 0;   // Loopback is disabled
  UART0_UARTCR_bit.RTSEN   = 0;   // Disable flow control
  UART0_UARTCR_bit.CTSEN   = 0;
  UART0_UARTCR_bit.RXE     = 1;   // Receive Section
  UART0_UARTCR_bit.TXE     = 1;   // Transmit section enabled
  // Init Line Control Register
  UART0_UARTLCR_H_bit.PEN  = 0;   // Parity disable
  UART0_UARTLCR_H_bit.STP2 = 0;   // 1 stop bit
  UART0_UARTLCR_H_bit.FEN  = 1;   // Enables transmit and receive FIFO buffers
  UART0_UARTLCR_H_bit.WLEN = 3;   // Word Length 8 bits
  UART0_UARTLCR_H_bit._9BIT= 0;   // Nine-bit Mode disabled
  // FIFO Interrupt level
  UART0_UARTIFLS_bit.RXIFLSEL = 0;  // Receive FIFO becomes > 1/8 full
  UART0_UARTIFLS_bit.TXIFLSEL = 0;  // Receive FIFO becomes < 1/8 full
  // DMA Disable
  UART0_DMACTRL_bit.RXDMAEN = 0;  // Disables the DMA for the receive FIFO
  UART0_DMACTRL_bit.TXDMAEN = 0;  // Disables the DMA for the transmit FIFO

  UART0_UARTRSR_UARTECR = 0;      // Clear pending errors
  UART0_UARTICR = 0x000007F2;     // Clear pending interrupts
  // Init Interrupt Mask Register
  UART0_UARTIMSC_bit.RXIM  = 1;   // Enable the RXI interrupt
  UART0_UARTIMSC_bit.TXIM  = 0;   // Mask the TXI interrupt
  UART0_UARTIMSC_bit.FEIM  = 1;   // Enable the FEI interrupt
  UART0_UARTIMSC_bit.BEIM  = 1;   // Enable the BEI interrupt
  UART0_UARTIMSC_bit.OEIM  = 1;   // Enable the OEI interrupt
  UART0_UARTIMSC_bit.RTIM  = 1;   // Enable the RTI interrupt

  // registered the interrupt handler of UART
  // Set address of the handler
  VIC_VectAddr0 = (unsigned long)Uart_handler;
  // UART2 interrupt line
  VIC_VectCtrl0_bit.IntSource = UART0_IRQ;
  // Enable slot
  VIC_VectCtrl0_bit.E = 1;
  // Enable UART interrupt
  VIC_IntEnable_bit.UART0INTR = 1;

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

