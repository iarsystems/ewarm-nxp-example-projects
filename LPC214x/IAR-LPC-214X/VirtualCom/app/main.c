/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : June 28, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 * Jumpers:
 *  PWR_J   - depend of power source
 *  DBG     - present
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-LPC-214X evaluation boards. It implements USB
 * CDC (Communication Device Class) device and install it like a Virtual COM
 * port. The UART1 is used for physical implementation of the RS232 port.
 * This example can work standalone on the IAR-LPC-214X board.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define UART  UART_1

#define LCD_BACKLIGHT_ON()    (IO0SET_bit.P0_21 = 1)
#define LCD_BACKLIGHT_OFF()   (IO0CLR_bit.P0_21 = 1)
#define LCD_BACKLIGHT_INIT()  (PINSEL1_bit.P0_21 = 0, IO0DIR_bit.P0_21 = 1, IO0CLR_bit.P0_21 = 1)

/*************************************************************************
 * Function Name: fiq_handler
 * Parameters: none
 * Return: none
 *
 * Description: FIQ subroutine
 *
 *************************************************************************/
__fiq __arm void fiq_handler (void)
{
  while(1);
}

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler
 *
 *************************************************************************/
__irq __arm void irq_handler (void)
{
void (*interrupt_function)();
unsigned int vector;

  vector = VICVectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  if(interrupt_function != NULL)
  {
    interrupt_function();  // Call vectored interrupt function.
  }
  else
  {
    VICVectAddr = 0;      // Clear interrupt in VIC.
  }
}

/*************************************************************************
 * Function Name: SetTickFlag
 * Parameters: none
 *
 * Return: none
 *
 * Description: Set arg
 *
 *************************************************************************/
void SetTickFlag (void* arg)
{
Int32U* pFlag = arg;
  *pFlag = 1;
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Delay
 * Return: void
 *
 * Description: Timer1 CH0 subroutine - delay [100us]
 *		
 *************************************************************************/
void DelayResolution100us(Int32U Delay)
{
volatile Int32U Flag = 0;
  // Stop Timer 1
  TIMER_Stop(TIMER1);
  // Reset Timer 1 counter
  TIMER_Reset(TIMER1);
  // Set action - match module CH0
  TIMER_SetMatchAction(TIMER1, CH0, TimerAction_Interrupt | TimerAction_StopTimer,
  Delay usec_T1*100, SetTickFlag, (void*)&Flag, DONOTHING);
  // Start Timer 1
  TIMER_Start(TIMER1);
  // Wait expire of delay
  while(Flag == 0);
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
Int8U Buffer[100];
pInt8U pBuffer;
Int32U Size,TranSize;

Boolean CdcConfigureStateHold;

#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
CDC_LineCoding_t CDC_LineCoding;
UartLineCoding_t UartLineCoding;
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0

#if CDC_DEVICE_SUPPORT_LINE_STATE > 0

#if UART1_MODEM_STAT_ENA > 0
UartModemEvents_t     UartModemEvents;
UartModemLineState_t  UartModemLineState;
CDC_LineState_t CDC_LineState;
#endif // UART1_MODEM_STAT_ENA > 0

UartLineEvents_t      UartLineEvents;

SerialState_t   SerialState;
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0


  SYS_Init(FOSC, FCCLK, VPBDIV2, USER_FLASH, 0xB27E7FFF,0x80FFFFFF,0xFFFFFFFF,0);
  LCD_BACKLIGHT_INIT();
  VIC_Init();
  VIC_SetProtectionMode(UserandPrivilegedMode);

  // Init Dly 100 ms - Timer1
  PM_OpenPeripheral(PC_TIMER1);
  TIMER_Init(TIMER1, TIMER_PRECISION);
  VIC_SetVectoredIRQ(TIMER1_ISR,VIC_Slot2,VIC_TIMER1); //  Dly 100 ms interrupt registered
  VIC_EnableInt(1<<VIC_TIMER1);

  // Init UART 1
  UartInit(UART_1,VIC_Slot4);

  // CDC USB
  UsbCdcInit();

  // Enable interrupts
  __enable_interrupt();

  // Soft connection enable
  USB_ConnectRes(TRUE);

  // LCD Powerup init
  HD44780_PowerUpInit();
  // Backlight On
  LCD_BACKLIGHT_ON();

  // Show messages on LCD
  HD44780_StrShow(1, 1,  "  IAR Systems   ");

  CdcConfigureStateHold = !IsUsbCdcConfigure();

  while(1)
  {
    if (IsUsbCdcConfigure())
    {
      if(CdcConfigureStateHold == FALSE)
      {
        HD44780_StrShow(1, 2,  "Virtual COM Port");
        CdcConfigureStateHold = TRUE;
      }

      // Data from USB
      Size = UsbCdcRead(Buffer,sizeof(Buffer)-1);
      if(Size)
      {
    #ifdef DATA_LOGGING
        Buffer[Size] = 0;
        printf("> %s\n",Buffer);
    #endif // DATA_LOGGING
        TranSize = 0;
        pBuffer = Buffer;
        do
        {
          Size -= TranSize;
          pBuffer += TranSize;
          TranSize = UartWrite(UART,pBuffer,Size);
        }
        while(Size != TranSize);
      }

      // Data from UART
      Size = UartRead(UART,Buffer,sizeof(Buffer)-1);
      if(Size)
      {
    #ifdef DATA_LOGGING
        Buffer[Size] = 0;
        printf("< %s\n",Buffer);
    #endif  // DATA_LOGGING
        while(!UsbCdcWrite(Buffer,Size));
      }
      // Get line and modem events from UART
#if CDC_DEVICE_SUPPORT_LINE_STATE > 0
      // Get line events - BI, FE, PE, OE
      UartLineEvents = UartGetUartLineEvents(UART_1);
#if UART1_MODEM_STAT_ENA > 0
      // Get modem line events - RI, DCD and DSR
      UartModemEvents = Uart1GetUartModemEvents();
      if(UartLineEvents.Data || UartModemEvents.Data)
      {
        // Modem lines report - DCD, DSR and RI
        SerialState.bRxCarrier = UartModemEvents.bDCD;
        SerialState.bTxCarrier = UartModemEvents.bDSR;
        SerialState.bRingSignal = UartModemEvents.bDRI;
#else
      if(UartLineEvents.Data)
      {
        SerialState.Data = 0;
#endif // UART1_MODEM_STAT_ENA > 0
        // Line events report BI, PE, FE and OE
        SerialState.bBreak = UartLineEvents.bBI;
        SerialState.bFraming = UartLineEvents.bFE;
        SerialState.bOverRun = UartLineEvents.bOE;
        SerialState.bParity = UartLineEvents.bPE;
        // Send events
        UsbCdcReportSerialCommState(SerialState);
      }
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0
    }
    else
    {
      if(CdcConfigureStateHold == TRUE)
      {
        HD44780_StrShow(1, 2,  " Put USB cable  ");
        CdcConfigureStateHold = FALSE;
      }
    }

    // UART line coding - Baud rate, number of the stop bits,
    // number of bits of the data word and parity type
#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
    if(UsbCdcIsNewLineCodingSettings())
    {
      CDC_LineCoding = UsbCdcGetLineCodingSettings();
      // Update the baud rate
      UartLineCoding.dwDTERate = CDC_LineCoding.dwDTERate;
      // Update the stop bits number
      UartLineCoding.bStopBitsFormat = CDC_LineCoding.bCharFormat?UART_TWO_STOP_BIT:UART_ONE_STOP_BIT;
      // Update the parity type
      UartLineCoding.bParityType = (CDC_LineCoding.bParityType == 0)?UART_NO_PARITY:(UartParity_t)(CDC_LineCoding.bParityType-1);
      // Update the word width
      UartLineCoding.bDataBits = (UartWordWidth_t)(CDC_LineCoding.bDataBits - 5);
      // Set UART line coding
      UartSetLineCoding(UART_1,UartLineCoding);
    }
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0

    // Get line and modem events from USB
#if UART1_MODEM_STAT_ENA > 0 && CDC_DEVICE_SUPPORT_LINE_STATE > 0
    // Modem lines - DTR, RTS
    if(UsbCdcIsNewLineStateSettings())
    {
      CDC_LineState = UsbCdcGetLineStateSettings();
      UartModemLineState.bDTR = CDC_LineState.DTR_State;
      UartModemLineState.bRTS = CDC_LineState.RTS_State;
      Uart1SetModemLineState(UartModemLineState);
    }
#endif // UART1_MODEM_STAT_ENA > 0 && CDC_DEVICE_SUPPORT_LINE_STATE > 0

#if CDC_DEVICE_SUPPORT_BREAK > 0
    // Break event
    UartSetUartLineState(UART_1,UsbCdcGetBreakState());
#endif // CDC_DEVICE_SUPPORT_BREAK > 0
  }
}
