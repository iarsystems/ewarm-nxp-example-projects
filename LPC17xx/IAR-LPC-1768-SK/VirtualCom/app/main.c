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
 *    1. Date        : February 3, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : February 12, 2009
 *       Author      : Stoyan Choynev
 *       Description : Port for IAR-LPC1768-SK
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-LPC1768-SK board.
 *  It implements USB CDC (Communication Device Class) device and install
 * it like a Virtual COM port. The UART1 is used for physical implementation
 * of the RS232 port.
 *
 * Jumpers:
 *  PWR_SEL     - depending of power source
 *  USBD-       - USB Device (1-2)
 *  USBD+       - USB Device (1-2)
 *  RST_E       - unfilled
 *  ISP_E       - unfilled
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect. In this case adaptive clocking
 * should be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US             500

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

/*variable for clitical section entry control*/
Int32U CriticalSecCntr;
/*************************************************************************
 * Function Name: NVIC_IntEnable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Enable interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntEnable(Int32U IntNumber)
{
volatile unsigned long * pNVIC_SetEn = &SETENA0;

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;

  pNVIC_SetEn += IntNumber/32;
  *pNVIC_SetEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_IntDisable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Disables interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntDisable(Int32U IntNumber)
{
volatile unsigned long * pNVIC_ClrEn = &CLRENA0;

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;

  pNVIC_ClrEn += IntNumber/32;
  *pNVIC_ClrEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description:Clear pending interrupt at NVIC
 *
 *
 *************************************************************************/
void NVIC_ClrPend(Int32U IntNumber)
{
volatile unsigned long * pNVIC_ClrPend = &CLRPEND0;

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;

  pNVIC_ClrPend += IntNumber/32;
  *pNVIC_ClrPend = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number, Interrupt Priority
 * Return: void
 *
 * Description:Sets Interrupt priority
 *
 *
 *************************************************************************/
void NVIC_IntPri(Int32U IntNumber, Int8U Priority)
{
volatile Int8U * pNVIC_IntPri = (Int8U *)&IP0;

  assert((NVIC_WDT <= IntNumber) && (NVIC_PLL1 >= IntNumber));
  IntNumber -= NVIC_WDT;
  pNVIC_IntPri += IntNumber;
  *pNVIC_IntPri = Priority;
}

#define FCCLK_FREQ 96000000
/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
  * Description: Initialize PLL and clocks' dividers. Hclk - 288MHz,
 *              Fcckl = 96MHz
 *
 *************************************************************************/
void InitClock(void)
{
  // 1. Init OSC
  SCS_bit.OSCRANGE = 0;
  SCS_bit.OSCEN = 1;
  // 2.  Wait for OSC ready
  while(!SCS_bit.OSCSTAT);
  // 3. Disconnect PLL
  PLL0CON_bit.PLLC = 0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 4. Disable PLL
  PLL0CON_bit.PLLE = 0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 5. Select source clock for PLL
  CLKSRCSEL_bit.CLKSRC = 1;   // Selects the main oscillator as a PLL clock source.
  PCLKSEL0 = PCLKSEL1  = 0;   // other peripherals 96/4 = 24MHz
  // 6. Set PLL settings 288 MHz
  PLL0CFG_bit.MSEL = 24-1;
  PLL0CFG_bit.NSEL = 2-1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 7. Enable PLL
  PLL0CON_bit.PLLE = 1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 8. Wait for the PLL to achieve lock
  while(!PLL0STAT_bit.PLOCK);
  // 9. Set clk divider settings
  CCLKCFG   = 3-1;            // 1/3 Fpll
  // 10. Connect the PLL
  PLL0CON_bit.PLLC = 1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
}

/*************************************************************************
 * Function Name: GpioInit
 * Parameters: void
 * Return: void
 *
 * Description: Reset all GPIO pins to default: primary function
 *
 *************************************************************************/
void GpioInit(void)
{
  // Set to inputs
  FIO0DIR = 0;
  FIO1DIR = 0;
  FIO2DIR = 0;
  FIO3DIR = 0;
  FIO4DIR = 0;

  // clear mask registers
  FIO0MASK = 0;
  FIO1MASK = 0;
  FIO2MASK = 0;
  FIO3MASK = 0;
  FIO4MASK = 0;

  // Reset all GPIO pins to default primary function
  PINSEL0 = 0;
  PINSEL1 = 0;
  PINSEL2 = 0;
  PINSEL3 = 0;
  PINSEL4 = 0;
  PINSEL7 = 0;
  PINSEL8 = 0;
  PINSEL9 = 0;
}

/*************************************************************************
 * Function Name: SYS_GetFsclk
 * Parameters: none
 * Return: Int32U
 *
 * Description: return Sclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFsclk(void)
{
Int32U Mul = 1, Div = 1, Osc, Fsclk;
  if(PLL0STAT_bit.PLLC)
  {
    // when PLL is connected
    Mul = PLL0STAT_bit.MSEL + 1;
    Div = PLL0STAT_bit.NSEL + 1;
  }

  // Find clk source
  switch(CLKSRCSEL_bit.CLKSRC)
  {
  case 0:
    Osc = I_RC_OSC_FREQ;
    break;
  case 1:
    Osc = MAIN_OSC_FREQ;
    break;
  case 2:
    Osc = RTC_OSC_FREQ;
    break;
  default:
    Osc = 0;
  }
  // Calculate system frequency
  Fsclk = Osc*Mul*2;
  Fsclk /= Div*(CCLKCFG+1);
  return(Fsclk);
}

/*************************************************************************
 * Function Name: SYS_GetFpclk
 * Parameters: Int32U Periphery
 * Return: Int32U
 *
 * Description: return Pclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFpclk(Int32U Periphery)
{
Int32U Fpclk;
pInt32U pReg = (pInt32U)((Periphery < 32)?&PCLKSEL0:&PCLKSEL1);

  Periphery  &= 0x1F;   // %32
  Fpclk = SYS_GetFsclk();
  // find peripheral appropriate periphery divider
  switch((*pReg >> Periphery) & 3)
  {
  case 0:
    Fpclk /= 4;
    break;
  case 1:
    break;
  case 2:
    Fpclk /= 2;
    break;
  default:
    Fpclk /= 8;
  }
  return(Fpclk);
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
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

  // Flash accelerator init
  FLASHCFG = (0x5UL<<12) | 0x3AUL;
  // Init clock
  InitClock();
#if FCCLK_FREQ < 20000000
  FLASHCFG = (0x0UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 40000000
  FLASHCFG = (0x1UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 60000000
  FLASHCFG = (0x2UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 80000000
  FLASHCFG = (0x3UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 100000000
  FLASHCFG = (0x4UL<<12) | 0x3AUL;
#endif

  // Init GPIO
  GpioInit();
  // Init UART 1
  UartInit(UART_1,4,NORM);

  // Init USB
  UsbCdcInit();

  __enable_interrupt();
  // Soft connection enable
  USB_ConnectRes(TRUE);

  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);

  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(7,105,131,131);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fCommunication Class\r\n\t\tDevice\r");

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
