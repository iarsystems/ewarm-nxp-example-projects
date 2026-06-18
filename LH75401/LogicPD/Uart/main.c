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
 *    1. Date        : May, 4 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the Logic PD LH75401 evaluation boards.
 *  It shows basic use of the UART2 and the interrupt controller.
 *
 * This application is intended to show how to set up the UART to transmit
 * and receive characters. The UART2 configuration is 115,200 baud, 8-n-1 mode
 *
 *    $Revision: 34 $
 **************************************************************************/
#include <intrinsics.h>
#include <stdio.h>
#include <string.h>

#include <NXP/iolh75401.h>
#include "board.h"

// Definitions
#define UART2_BAUD_RATE   112500

typedef enum
{
  TxFIFOIntr = 1, RxFIFOIntr, RxIntr, TxIntr, TimerIntr
} UART_IntrType_t;

typedef enum
{
  UART_REC_CH = 0, UART_ERROR,
} Event_t;

typedef enum
{
  UART_FE = 0, UART_OE, UART_PE, UART_BD, UART_UE
} UartErrorType_t;

typedef void (*VoidFpnt_t)(void);

#pragma pack(1)
typedef struct
{
  Event_t   Event;
  unsigned char Value;
} EventBuf_t, *pEventBuf_t;

#pragma pack()

// Constants
const char Startup [] =
"\n\r*************************************************************************\n\r"
"*  This example project shows how to use the IAR Embedded Workbench     *\n\r"
"* for ARM to develop code for the Logic PD LH75401 evaluation boards.   *\n\r"
"* It shows basic use of the UART2 and the interrupt controller.         *\n\r"
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

// function prototypes
void EventFifoInit(pEventBuf_t Event, int size);
int EventPush(Event_t Event, unsigned char Value);
int EventPop(pEventBuf_t Event);

// Variables
char TxBuffer[200];
EventBuf_t EventBuf[200];

volatile char * volatile pTxData;

int EventPopInd, EventPushInd;
int EventBufferSize;
pEventBuf_t pEventBuf;


/*************************************************************************
 * Function Name: IRQ_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ Handler
 *
 *************************************************************************/
__irq __arm void IRQ_Handler(void)
{
void (*interrupt_function)();
unsigned int vector;
  vector = VIC_VectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();  // Call vectored interrupt function
  VIC_VectAddr = 0;         // Clean interrupt in VIC
}

/*************************************************************************
 * Function Name: Uart2_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: The UART2 interrupt Handler
 *
 *************************************************************************/
void Uart2_handler(void)
{
int LineStat;
unsigned char ch;
unsigned int BankHold;
  switch(UART2_GIR_bit.BI)
  {
  case TxFIFOIntr:  // Tx FIFO interrupt
    // Fill Tx FIFO
    do
    {
      UART2_TXD = *pTxData++;
      if(*pTxData == 0)
      {
        pTxData = (char *)0;
        // Get current registers' bank
        BankHold = UART2_GIR_bit.BANK;
        // Set a bank 0
        UART2_GIR_bit.BANK  = 0;
        // Disable Tx FIFO interrupt
        UART2_GER_bit.TFIE = 0;
        // Restore the bank
        UART2_GIR_bit.BANK  = BankHold;
        USR_LED_OFF();
        break;
      }
    }
    while(UART2_FLR_bit.TFL < 3);
    break;
  case RxFIFOIntr:  // Rx FIFO interrupt
    // Drain Rx FIFO
    do
    {
      ch = UART2_RXD;
      EventPush(UART_REC_CH,ch);
    }
    while (UART2_FLR_bit.RFL != 0);
    break;
  case RxIntr:   // Rx Line status interrupt
    LineStat = UART2_RST;
    // Overrun Error
    if(LineStat & (1UL<<1))
    {
      EventPush(UART_ERROR,UART_OE);
    }
    // Parity Error
    if(LineStat & (1UL<<2))
    {
      EventPush(UART_ERROR,UART_PE);
    }
    // Framing Error
    if(LineStat & (1UL<<3))
    {
      EventPush(UART_ERROR,UART_FE);
    }
    // Break detect
    if(LineStat & (1UL<<4))
    {
      EventPush(UART_ERROR,UART_BD);
    }
    break;
  case TxIntr:  // Tx status interrupt
    BankHold = UART2_GIR_bit.BANK;
    UART2_GIR_bit.BANK  = 0;
    // Disable Tx interrupt
    UART2_GER_bit.TxIE   = 0;
    UART2_GIR_bit.BANK  = BankHold;
    break;
  case TimerIntr: // UART timers interrupt
    BankHold = UART2_GIR_bit.BANK;
    UART2_GIR_bit.BANK  = 0;
    // Disable Timer interrupt
    UART2_GER_bit.TIE   = 0;
    UART2_GIR_bit.BANK  = BankHold;
    break;
  default:
    EventPush(UART_ERROR,UART_UE);
  }
}


/*************************************************************************
 * Function Name: Uart2TranString
 * Parameters: const char * pStr
 *
 * Return: none
 *
 * Description: Init UART2 transmit
 *
 *************************************************************************/
void Uart2TranString (const char * pStr)
{
unsigned int BankHold;
  if((pStr == NULL) || (*pStr == 0))
  {
    return;
  }
  // Wait until the Tx FIFO isn't full
  while(UART2_FLR_bit.TFL > 3);
  // Fill Tx FIFO
  do
  {
    UART2_TXD = *pStr++;
    if(*pStr == 0)
    {
      return;
    }
  } while (UART2_FLR_bit.TFL < 3);
  if(*pStr != 0)
  {
    USR_LED_ON();
    pTxData = (char *)pStr;
    BankHold = UART2_GIR_bit.BANK;
    UART2_GIR_bit.BANK  = 0;
    // Enable Tx FIFO interrupt
    UART2_GER_bit.TFIE  = 1;
    UART2_GIR_bit.BANK  = BankHold;
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
  // avoid disable-enable interrupt in EventPop function
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
int NewData;
unsigned char ch;

  // Init IO ports
  IOCON_PE_MUX_bit.UARTRX2 = 1;
  IOCON_PE_MUX_bit.UARTTX2 = 1;   // Assign PE0,PE1 to UART
  USR_LED_OFF();

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

  // Init UART2
  // UART 2 clk enable
  RCPC_APBPeriphClkCtrl0_bit.U2 = 1;
  // Baud Rate Generation Configuration Bank
  UART2_GIR_bit.BANK  = 3;
  UART2_CLCF_bit.RXCS = 1;  // Receiver Clock Source BRGA
  UART2_CLCF_bit.TXCS = 1;  // Transmitter Clock Source BRGA
  UART2_BACF_bit.BAM  = 1;  // Baud Rate Generator Mode
  UART2_TMIE_bit.TAIE = 0;
  UART2_TMIE_bit.TBIE = 0;  // Timer A,B interrupts disable

  // General Configuration Bank
  UART2_GIR_bit.BANK  = 2;
  UART2_RMD_bit.SSM   = 0;  // Majority voting for start bit
  UART2_RMD_bit.SWM   = 0;  // Small window, 3/16 sampling
  UART2_RMD_bit.DPD   = 0;  // Enable DPLL
  UART2_RMD_bit.uCM   = 0;  // No standard set Control Characters
  UART2_RIE_bit.OEE   = 1;  // enable OE interrupt
  UART2_RIE_bit.PEE   = 1;  // enable PE interrupt
  UART2_RIE_bit.FEE   = 1;  // enable FE interrupt
  UART2_RIE_bit.BKDE  = 1;  // enable Break Detect
  UART2_IMD_bit.RFD   = 0;  // Enable FIFOs
  UART2_IMD_bit.uLM   = 0;  // Normal Mode
  UART2_IMD_bit.LEM   = 0;  // No echo
  UART2_TMD_bit.NBCL  = 0;  // Eight-bit Length
  UART2_TMD_bit.SBL   = 0;  // Stop bit length b2,b1
  UART2_FMD_bit.TFT   = 0;  // Transmit FIFO Threshold >= 1 word
  UART2_FMD_bit.RFT   = 0;  // Receive FIFO Threshold >= 1 word

  // 8250A/16450 Compatible Bank
  UART2_GIR_bit.BANK  = 0;
  UART2_MCTRL_bit.LC  = 0;  // Loopback is disabled
  // Enable Baud Register access, Data width 8 bit, 1 stop bit
  UART2_LCR          = 0x83;
  // Set baud
  UART2_BAL =  (HCLK_FREQ / (UART2_BAUD_RATE * 16))      & 0xFF;
  UART2_BAH = ((HCLK_FREQ / (UART2_BAUD_RATE * 16)) >> 8)& 0xFF;
  UART2_LCR_bit.DLAB = 0;
  UART2_GER_bit.RFIE = 1;
  UART2_GER_bit.RxIE = 1;   // Enable Rx interrupts enable

  // General Work Bank
  UART2_GIR_bit.BANK  = 1;
  UART2_ICM_bit.INTA  = 0;  // Interrupt Acknowledge - Manual
  UART2_TCM = 0x0E;         // Transmit Enable
  UART2_RCM = 0xB4;         // Receive Enable
  UART2_TMCTRL_bit.TGB = 0;
  UART2_TMCTRL_bit.TGA = 0; // Disables counting

  // registered the interrupt handler of UART2
  // Set address of the handler
  VIC_VectAddr0 = (unsigned long)Uart2_handler;
  // UART2 interrupt line
  VIC_VectCtrl0_bit.IntSource = VIC_UART2;
  // Enable slot
  VIC_VectCtrl0_bit.E = 1;
  // Enable UART2 interrupt
  VIC_IntEnable_bit.UART2_Interrupt = 1;

  NewData = 0;
  EventFifoInit(EventBuf,sizeof(EventBuf)/sizeof(EventBuf_t));

  // Enable interrupts.
  __enable_interrupt();
  Uart2TranString(Startup);

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
        Uart2TranString(TxBuffer);
      }
    }
  }
}
