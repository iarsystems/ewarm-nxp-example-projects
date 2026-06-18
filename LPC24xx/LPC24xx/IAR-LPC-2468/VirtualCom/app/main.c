/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : September 14, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR LPC-2468 board.
 *  It implements USB CDC (Communication Device Class) device and install
 * it like a Virtual COM port. The UART1 is used for physical implementation
 * of the RS232 port.
 *
 * Jumpers:
 *  J5      - depending of power source
 *  ISP     - unfilled
 *  nRESET  - unfilled
 *  EINT0   - filled
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 * The LEDs share pins with Trace port. If ETM is enabled the LEDs can not be used. 
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define NONPROT 0xFFFFFFFF
#define CRP1  0x12345678
#define CRP2  0x87654321
/*If CRP3 is selected, no future factory testing can be performed on the device*/
#define CRP3  0x43218765

#ifndef __RAM_DEBUG
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = NONPROT;
#endif

#define DLY_100US             500

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
  }
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

  // MAM init
  MAMCR_bit.MODECTRL = 0;
  MAMTIM_bit.CYCLES = 3;    // FCLK > 40 MHz
  MAMCR_bit.MODECTRL = 2;   // MAM functions fully enabled

  // Init clock
  InitClock();
  // Init GPIO
  GpioInit();
  // Init VIC
  VIC_Init();
  // Init UART 1
  UartInit(UART_1,4,NORM);

  // Init USB
  UsbCdcInit();

  __enable_interrupt();
  // Soft connection enable
  USB_ConnectRes(TRUE);

  HD44780_PowerUpInit();
  // Show message on the LCD
  HD44780_StrShow(1, 1,  "  IAR Systems   ");
  HD44780_StrShow(1, 2,  "   CD Class     ");
  LCD_LIGHT_ON();

  CdcConfigureStateHold = !IsUsbCdcConfigure();

  while(1)
  {
    if (IsUsbCdcConfigure())
    {
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
          TranSize = UartWrite(UART_1,pBuffer,Size);
        }
        while(Size != TranSize);
      }

      // Data from UART1
      Size = UartRead(UART_1,Buffer,sizeof(Buffer)-1);
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
