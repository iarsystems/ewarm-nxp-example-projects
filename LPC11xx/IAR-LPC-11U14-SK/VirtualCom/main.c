/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR LPC11U14-SK board.
 *  It implements USB CDC (Communication Device Class) device and installs
 * it like a Virtual COM port.
 *
 *    Jumpers:
 *   RST_E     - unfilled
 *   ISP_E     - unfilled
 *   PIO1_LOW  - unfilled
 *   PIO3_HIGH - unfilled
 *   PIO3_E    - filled
 *   TMS/SWD   - SWD (2-3)
 *
 *    History :
 *    1. Date        : 19, March 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"
#include <yfuns.h>

#define DLY_100US             550

/*variable for critical section entry control*/
Int32U CriticalSecCntr;

volatile Int32U Ticks;

/** external data **/

/** internal functions **/

/** public functions **/
/*************************************************************************
 * Function Name: NVIC_IntEnable
 * Parameters: IntNumber - Interrupt number
 * Return: void
 *
 * Description: Enable interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntEnable(Int32U IntNumber)
{
volatile unsigned long * pNVIC_SetEn = &ISER;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_USB_WAKEUP >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

  pNVIC_SetEn += IntNumber/32;
  *pNVIC_SetEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_IntDisable
 * Parameters: IntNumber - Interrupt number
 * Return: void
 *
 * Description: Disables interrup at NVIC
 *
 *
 *************************************************************************/
void NVIC_IntDisable(Int32U IntNumber)
{
volatile unsigned long * pNVIC_ClrEn = &ICER;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_USB_WAKEUP >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

  pNVIC_ClrEn += IntNumber/32;
  *pNVIC_ClrEn = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrupt number
 * Return: void
 *
 * Description:Clear pending interrupt at NVIC
 *
 *
 *************************************************************************/
void NVIC_ClrPend(Int32U IntNumber)
{
volatile unsigned long * pNVIC_ClrPend = &ICPR;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_USB_WAKEUP >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

  pNVIC_ClrPend += IntNumber/32;
  *pNVIC_ClrPend = (1UL<<(IntNumber%32));

}

/*************************************************************************
 * Function Name: NVIC_IntPri
 * Parameters: IntNumber - Interrupt number, Interrupt Priority
 * Return: void
 *
 * Description:Sets Interrupt priority
 *
 *
 *************************************************************************/
void NVIC_IntPri(Int32U IntNumber, Int8U Priority)
{
volatile Int8U * pNVIC_IntPri = (Int8U *)&IP0;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_USB_WAKEUP >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;
  pNVIC_IntPri += IntNumber;
  *pNVIC_IntPri = Priority;
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: clock, AHB divider
 * Return: void
 *
 * Description: Initialize PLL to desired clock and  AHB divider
 *              Sys clock is Sys PLL output
 *
 *
 *************************************************************************/
void InitClock(Int32U clock, Int32U ahbdiv)
{
  /*Sys Oscilator Enable*/
  SYSOSCCTRL = (MAIN_OSC_FREQ>(20MHZ))?(0x2):(0x0);
  /*Power Up SYS Oscillator*/
  PDRUNCFG_bit.SYSOSC_PD = 0;
  /*Enable Internal RC oscillator*/
  PDRUNCFG_bit.IRC_PD = 0;
  /*Select internal RC oscillator for
    Sys clock source*/
  MAINCLKUEN = 0;
  MAINCLKSEL = 0;
  MAINCLKUEN = 1;
  /*Configure SYS PLL*/
  /*Power Down SYS PLL*/
  PDRUNCFG_bit.SYSPLL_PD = 1;
  /*Select Sys Oscillator for
    SYS PLL source*/
  SYSPLLCLKUEN = 0;
  SYSPLLCLKSEL = 1;
  SYSPLLCLKUEN = 1;
  /*Calc M*/
  Int32U m = clock/(MAIN_OSC_FREQ*(2*2)) - 1;

  assert(m<32);
  /*Configure PLL frequency*/
  SYSPLLCTRL =  (m)     /*MSEL*/
             |  (1<<5)  /*PSEL = 2*/
             ;
  /*Power Up PLL*/
  PDRUNCFG_bit.SYSPLL_PD = 0;
  /*Set Sys AHB Clock divider*/
  SYSAHBCLKDIV_bit.DIV = ahbdiv;
  /*Wain until PLL locks*/
  while(!(SYSPLLSTAT_bit.LOCK));
  /*Select Sys PLL Output for
    Sys clock source*/
  MAINCLKUEN = 0;
  MAINCLKSEL = 3;
  MAINCLKUEN = 1;
  /* Init USB clock system */
  /* Power down USBPLL */
  PDRUNCFG_bit.USBPLL_PD = 1;
  /* Power-up USB Phy*/
  PDRUNCFG_bit.USBPAD_PD = 0;
  /* Select system oscillator and update */
  USBPLLCLKUEN = 0;
  USBPLLCLKSEL = 1;
  USBPLLCLKUEN = 1;
  USBPLLCTRL = ((48MHZ / MAIN_OSC_FREQ) - 1)     /*MSEL*/
             |  (2<<5)  /*PSEL = 2*/
             ;
  /* Wake up USBPLL */
  PDRUNCFG_bit.USBPLL_PD = 0;
  /*Wain until PLL locks*/
  while(!(USBPLLSTAT_bit.LOCK));
  /* set usb clock - 48MHz*/
  USBCLKSEL = 0;
  USBCLKDIV = 1;
}

/*************************************************************************
 * Function Name: SYS_GetMainClk
 * Parameters: none
 * Return: Int32U
 *
 * Description: return Main Clock [Hz]
 *
 *************************************************************************/
Int32U SYS_GetMainClk(void)
{
Int32U Clk;
  switch(MAINCLKSEL_bit.SEL)
  {
    case 0:
       Clk = I_RC_OSC_FREQ;
    break;
    case 1:
      Clk = MAIN_OSC_FREQ;
    break;
    case 2:
      Clk = WDT_OSC_FREQ;
    break;
    case 3:
      switch(SYSPLLCLKSEL_bit.SEL)
      {
        case 0:
          Clk = I_RC_OSC_FREQ;
        break;
        case 1:
          Clk = MAIN_OSC_FREQ;
        break;
        case 2:
          Clk = WDT_OSC_FREQ;
        break;

        default:
          Clk = 0;
        break;
      }

      if(!SYSOSCCTRL_bit.BYPASS)
        Clk *= (SYSPLLCTRL_bit.MSEL+1);
    break;

    default:
      Clk = 0;
    break;
  }
  return Clk;
}

void USART_Init (Int32U BaudRate)
{
Int32U Pclk;
Int32U div;
  /*Disable Clock*/
  USARTCLKDIV = 0;
  /* Configure IOs*/
  /*Enable I/O Configuration registers*/
  SYSAHBCLKCTRL_bit.IOCON = 1;
  IOCON_PIO0_19 = 1;         /* Uart TX function select */
  IOCON_PIO0_18 = 1;         /* Uart RX function select */

  div = 1;
  while((Pclk=SYS_GetMainClk()/USARTCLKDIV) > 50MHZ)
  {
    div++;
  }
  /*Enable Clock*/
  USARTCLKDIV = div;

  /*Enable UART register clock*/
  SYSAHBCLKCTRL_bit.USART = 1;

  U0LCR_bit.WLS = 3;        /* Select 8-Bits */
  U0LCR_bit.SBS = 0;        /* One Stop Bit */
  U0LCR_bit.PE = 0;         /* No parity */

  U0LCR_bit.DLAB = 1;       /* Divisor Latch Access ebabled */
  /* Set UART0 BAUDRATE */
  U0DLL = ((((Pclk+8)/16)+BaudRate/2)/BaudRate)%256;
  U0DLM = ((((Pclk+8)/16)+BaudRate/2)/BaudRate)/256;
  U0LCR_bit.DLAB = 0;       /* Clear Divisor Latch Access bit */

  U0FCR = (1<<0);     /* Enable UART0 FIFOs. */
  U0TER = (1<<7);     /* Enable Transmission */
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
 * Function Name: GpioInit
 * Parameters: void
 * Return: void
 *
 * Description: Reset all GPIO pins to default: primary function
 *
 *************************************************************************/
void GpioInit(void)
{
  /* Set to inputs */
  GPIO_DIR0 = 0;
  GPIO_DIR1 = 0;

  /* Reset all GPIO pins to default primary function */
  IOCON_RESET_PIO0_0 = 0x90;
  IOCON_PIO0_1 = 0x90;
  IOCON_PIO0_2 = 0x90;
  IOCON_PIO0_3 = 0x90;
  IOCON_PIO0_4 = 0x80;
  IOCON_PIO0_5 = 0x80;
  IOCON_PIO0_6 = 0x90;
  IOCON_PIO0_7 = 0x90;
  IOCON_PIO0_8 = 0x88;
  IOCON_PIO0_9 = 0x90;
  IOCON_SWCLK_PIO0_10 = 0x90;
  IOCON_TDI_PIO0_11 = 0x90;
  IOCON_TMS_PIO0_12 = 0x90;
  IOCON_TDO_PIO0_13 = 0x90;
  IOCON_TRST_PIO0_14 = 0x90;
  IOCON_SWDIO_PIO0_15 = 0x90;
  IOCON_PIO0_16 = 0x90;
  IOCON_PIO0_17 = 0x90;
  IOCON_PIO0_20 = 0x90;
  IOCON_PIO0_21 = 0x90;
  IOCON_PIO0_22 = 0x90;
  IOCON_PIO0_23 = 0x90;
  IOCON_PIO1_5 = 0x90;
  IOCON_PIO1_13 = 0x90;
  IOCON_PIO1_14 = 0x90;
  IOCON_PIO1_15 = 0x90;
  IOCON_PIO1_16 = 0x90;
  IOCON_PIO1_19 = 0x90;
  IOCON_PIO1_20 = 0x90;
  IOCON_PIO1_21 = 0x90;
  IOCON_PIO1_22 = 0x90;
  IOCON_PIO1_23 = 0x90;
  IOCON_PIO1_24 = 0x90;
  IOCON_PIO1_25 = 0x90;
  IOCON_PIO1_26 = 0x90;
  IOCON_PIO1_27 = 0x90;
  IOCON_PIO1_28 = 0x90;
  IOCON_PIO1_29 = 0x90;
}
/*************************************************************************
 * Function Name: TIMER32_0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer32 0 interrupt handler
 *
 *************************************************************************/
void TIMER32_0_IRQHandler (void)
{
  ++Ticks;
  /* clear interrupt */
  TMR32B0IR_bit.MR0INT = 1;
  /**/
   NVIC_ClrPend(NVIC_CT32B0);
}

void CT32B0_Init(Int32U tps)
{
  Ticks = 0;
  /* Enable TIM0 clocks */
  SYSAHBCLKCTRL_bit.CT32B0 = 1;

  /* Init Time0 */
  TMR32B0TCR_bit.CE = 0;    /* counting  disable */
  TMR32B0TCR_bit.CR = 1;   /* set reset */
  TMR32B0TCR_bit.CR = 0;   /* release reset */

  TMR32B0MCR_bit.MR0I = 1;   /* Enable Interrupt on MR0 */
  TMR32B0MCR_bit.MR0R = 1;   /* Enable reset on MR0 */
  TMR32B0MCR_bit.MR0S = 0;   /* Disable stop on MR0 */
  /* set timer 0 period */
  TMR32B0PR = 0;
  TMR32B0MR0 = (SYS_GetMainClk()/(SYSAHBCLKDIV))/(tps);
  /* init timer 0 interrupt */
  TMR32B0IR_bit.MR0INT = 1;  /* clear pending interrupt */
  TMR32B0TCR_bit.CE = 1;    /* counting Enable */
   /*Enable NVIC TMR0 Interrupt*/
  NVIC_IntEnable(NVIC_CT32B0);
  NVIC_IntPri(NVIC_CT32B0,16);
}

void CT32B0_Stop(void)
{
  TMR32B0TCR_bit.CE = 0;       /* counting  disable */

  NVIC_IntDisable(NVIC_CT32B0);

  SYSAHBCLKCTRL_bit.CT32B0 = 0; /* disable clock */
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

UartLineEvents_t      UartLineEvents;

SerialState_t   SerialState;
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0

  /*Init clock*/
  InitClock(132MHZ, 2);
  /*Enable GPIO Clock*/
  SYSAHBCLKCTRL_bit.GPIO = 1;
  /*Enable IOCON Clock*/
  SYSAHBCLKCTRL_bit.IOCON = 1;
  /*Set all pins as input ports*/
  GpioInit();
   /*Init LED Ports*/
  LED1_DIR |= LED1_MASK;
  LED2_DIR |= LED2_MASK;
  /* Turn off LED2 */
  LED_OFF(LED2_PORT,LED2_MASK);
  /* Turn on LED1 */
  LED_ON(LED1_PORT,LED1_MASK);

  UartInit();

  /*Init LCD*/
  HD44780_IO_Init();

  if(HD44780_OK != HD44780_PowerUpInit())
  {/*If LCD Init fails*/
    printf("LCD INIT ERROR\n\r");
    while(1)
    {
      if(5>Ticks)
      {
        Ticks = 0;
        LED1_PORT = ~LED1_PORT;
      }
    }
  }

  HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)"USB CDC ");
  // Init USB
  UsbCdcInit();

  __enable_interrupt();
  // Soft connection enable
  USB_ConnectRes(TRUE);

  CdcConfigureStateHold = !IsUsbCdcConfigure();

  while(1)
  {
    if (IsUsbCdcConfigure())
    {
      // Data from USB
      Size = UsbCdcRead(Buffer,sizeof(Buffer)-1);
      if(Size)
      {
        LED_ON(LED1_PORT,LED1_MASK);
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
          TranSize = UartWrite(pBuffer,Size);
        }
        while(Size != TranSize);
      }

      // Data from UART1
      Size = UartRead(Buffer,sizeof(Buffer)-1);
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
      UartLineEvents = UartGetUartLineEvents();
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
      UartSetLineCoding(UartLineCoding);
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
    UartSetUartLineState(UsbCdcGetBreakState());
#endif // CDC_DEVICE_SUPPORT_BREAK > 0
  }

}
