/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : main.c
 *    DESCRIPTION
 *    ===========
 *     This example project shows how to use the IAR Embedded Workbench for ARM
 *    to develop code for IAR-LPC1347-SK board.
 *     It implements USB CDC (Communication Device Class) device and installs
 *    it like a Virtual COM port. The UART is used for physical implementation
 *    of the RS232 port. There is LED indication for data transfer from UART->
 *    ->Virtual COM and from Virtual COM->UART.
 *
 *    COMPATIBILITY
 *    =============
 *
 *      The example project is compatible with IAR-LPC1347-SK evaluation board.
 *    By default, the project is configured to use the J-Link SWD interface.
 *
 *    CONFIGURATION
 *    =============
 *    Jumpers:
 *    RST_E   - unfilled
 *    ISP_E   - unfilled
 *    TMS/SWDIO - SWDIO (2-3)
 *    TDO/SWV   - SWV (2-3)
 *
 *    GETTING STARTED
 *    ===============
 *
 *    1) Build and download the program.
 *
 *    2) Connect a serial cable between a Host COM port and the IAR LPC1347-SK
 *       UART connector and start a terminal emulator program on the Host.
 *
 *    3) Run the program.
 *
 *    4) The first time the device is connected to the computer, Windows will
 *       load the driver for identified device. The Virtual COM port driver is
 *       in the $PROJ_DIR$\VirCOM_Driver_XP\.
 *
 *    5) Open second instance of terminal emulator and open the Virtual COM Port.
 *       Configure the Virtual COM Port with the same settings as PC COM Port.
 *
 *    6) Open these two ports with the two instances of terminal program.
 *       When you type at Virtual COM Port window, you will see typed characters
 *       at PC COM Port window and vice versa. You will see LED1 indication when
 *       you send data from Virtual COM Port to PC COM Port, and LED2 indication
 *       when you send data from PC COM Port to Virtual COM Port.
 *
 *
 *    1. Date        : September 7, 2009
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *    2. Date        : January 30, 2012
 *       Author      : Atanas Uzunov
 *       Description : Ported to IAR LPC1347-SK evaluation board.
 *                     Added LED notification for USB->UART and UART->USB
 *                     transfers.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"
#include <yfuns.h>

#define DLY_100US             550

volatile uint32_t usartLedTicks;
volatile uint32_t usbLedTicks;
volatile uint32_t Ticks;

#define LED_HOLD_TIME         10

/** external data **/

/** internal functions **/

/** public functions **/
/*************************************************************************
 * Function Name: InitClock
 * Parameters:
 * Return: void
 *
 * Description: Initialize PLL to 72MHz
 *              Sys clock is Sys PLL output and  AHB divider is 1
 *              System runs at 72MHz
 *
 *************************************************************************/
void InitClock(void)
{
  /* Enable Internal RC oscillator */
  CLK_SetIRCOsc(CLK_ENABLE);
  /* Select internal RC oscillator for
     Sys clock source*/
  CLK_SetMainClk(MAINSEL_IRCOSC, 1);
  /* Sys Oscilator Enable, bypass disabled, set range */
  CLK_MainOscSet(CLK_ENABLE, CLK_DISABLE, (MOSC >(20MHZ))?(MOSCRNG_15_25MHZ):(MOSCRNG_1_20MHZ));
  /* Select Sys Oscillator for
     SYS PLL source*/
  CLK_SetPLLInClk(PLLINSEL_MOSC);
  /* Enable set PLL 72Hz (M = 6; P = 2) */
  CLK_SetPll(CLK_ENABLE, (72MHZ/MOSC)-1, 1);
  /* Select Sys PLL Output for
     Main clock source. SYSAHB clock divider 1:1. CPU runs at 72MHz */
  CLK_SetMainClk(MAINSEL_CLKPLL,1);
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
  IOCON_PIO0_4 = 0x00;
  IOCON_PIO0_5 = 0x00;
  IOCON_PIO0_6 = 0x90;
  IOCON_PIO0_7 = 0x90;
  IOCON_PIO0_8 = 0x90;
  IOCON_PIO0_9 = 0x93;
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
  IOCON_PIO1_0 = 0x90;
  IOCON_PIO1_1 = 0x90;
  IOCON_PIO1_2 = 0x90;
  IOCON_PIO1_3 = 0x90;
  IOCON_PIO1_4 = 0x90;
  IOCON_PIO1_5 = 0x90;
  IOCON_PIO1_7 = 0x90;
  IOCON_PIO1_8 = 0x90;
  IOCON_PIO1_10 = 0x90;
  IOCON_PIO1_11 = 0x90;
  IOCON_PIO1_13 = 0x90;
  IOCON_PIO1_14 = 0x90;
  IOCON_PIO1_15 = 0x90;
  IOCON_PIO1_16 = 0x90;
  IOCON_PIO1_17 = 0x90;
  IOCON_PIO1_18 = 0x90;
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
  IOCON_PIO1_31 = 0x90;
}

/*************************************************************************
 * Function Name: CT32B0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void CT32B0_IRQHandler (void)
{
  ++Ticks;
  if (usartLedTicks) usartLedTicks--;
  if (usbLedTicks) usbLedTicks--;
  /* Clear interrupt */
  TMR32B0IR_bit.MR0INT = 1;
  NVIC_ClrPend(NVIC_CT32B0);
}

/*************************************************************************
 * Function Name: CT32B0_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 initialization
 *
 *************************************************************************/
void CT32B0_Init(uint32_t tps)
{
  Ticks = 0;
  usartLedTicks = 0;
  usbLedTicks = 0;
  /* Enable TIM0 clocks */
  SYSAHBCLKCTRL_bit.CT32B0 = 1;

  /* Init Timer 0 */
  TMR32B0TCR_bit.CE = 0;     /* counting  disable */
  TMR32B0TCR_bit.CR = 1;     /* set reset */
  TMR32B0TCR_bit.CR = 0;     /* release reset */
  TMR32B0CTCR_bit.CTM = 0;   /* Timer Mode: every rising PCLK edge */
  TMR32B0MCR_bit.MR0I = 1;   /* Enable Interrupt on MR0 */
  TMR32B0MCR_bit.MR0R = 1;   /* Enable reset on MR0 */
  TMR32B0MCR_bit.MR0S = 0;   /* Disable stop on MR0 */
  /* set timer 0 period */
  TMR32B0PR = 0;
  TMR32B0MR0 = (CLK_GetClock(CLK_SYS))/(tps);
  /* init timer 0 interrupt */
  TMR32B0IR_bit.MR0INT = 1;  /* clear pending interrupt */
  TMR32B0TCR_bit.CE = 1;     /* counting Enable */
  /* Enable NVIC TMR0 Interrupt */
  NVIC_IntEnable(NVIC_CT32B0);
  NVIC_IntPri(NVIC_CT32B0,16);
}

/*************************************************************************
 * Function Name: CT32B0_Stop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Disable Timer 0
 *
 *************************************************************************/
void CT32B0_Stop(void)
{
  TMR32B0TCR_bit.CE = 0;        /* counting  disable */

  NVIC_IntDisable(NVIC_CT32B0);

  SYSAHBCLKCTRL_bit.CT32B0 = 0; /* disable clock */
}

#pragma section=".intvec"
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
#endif /* CDC_DEVICE_SUPPORT_LINE_CODING > 0 */

#if CDC_DEVICE_SUPPORT_LINE_STATE > 0
UartLineEvents_t      UartLineEvents;
SerialState_t   SerialState;
#endif /* CDC_DEVICE_SUPPORT_LINE_STATE > 0 */

int cpuclk;
  /* Flash accelerator save value */
  FLASHCFG_bit.FLASHTIM = 3;
  /* Clock Init */
  InitClock();
  /* Flash accelerator init */
  cpuclk = CLK_GetClock(CLK_SYS);

  if( cpuclk < 20000000 ){
    FLASHCFG_bit.FLASHTIM = 1;
  } else if( cpuclk < 40000000 ){
    FLASHCFG_bit.FLASHTIM = 2;
  } else {
    FLASHCFG_bit.FLASHTIM = 3;
  }
  /* Set vector table location */
  VTOR  = (unsigned int)__segment_begin(".intvec");
  /* Enable GPIO Clock */
  SYSAHBCLKCTRL_bit.GPIO = 1;
  /* Enable IOCON Clock */
  SYSAHBCLKCTRL_bit.IOCON = 1;
  /* Set all pins as input ports */
  GpioInit();
  /* Init LED Ports */
  LED1_DIR |= LED1_MASK;
  LED2_DIR |= LED2_MASK;
  /* Turn off LED1 */
  LED_OFF(LED1);
  /* Turn off LED2 */
  LED_OFF(LED2);
  /* Init Timer 0 */
  CT32B0_Init(100);

  UartInit();

  /* Init LCD */
  HD44780_IO_Init();

  if(HD44780_OK != HD44780_PowerUpInit())
  {/* If LCD Init fails */
    printf("LCD INIT ERROR\n\r");
    while(1)
    {
      if(500>Ticks)
      {
        Ticks = 0;
        LED1 = ~LED1;
      }
    }
  }

  HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)"USB CDC ");
  /* Init USB */
  UsbCdcInit();

  __enable_interrupt();

  /* Soft connection enable */
  USB_ConnectRes(TRUE);

  CdcConfigureStateHold = !IsUsbCdcConfigure();

  while(1)
  {
    /* If led2 timer is 0, turn off LED2 */
    if (!usartLedTicks) LED_OFF(LED2);
    /* If led1 timer is 0, turn off LED1 */
    if (!usbLedTicks) LED_OFF(LED1);
    if (IsUsbCdcConfigure())
    {
      /* Data from USB */
      Size = UsbCdcRead(Buffer,sizeof(Buffer)-1);
      if(Size)
      {
        /*Turn on LED1 and load led1 timer*/
        LED_ON(LED1);
        usbLedTicks = LED_HOLD_TIME;
#ifdef DATA_LOGGING
        Buffer[Size] = 0;
        printf("> %s\n",Buffer);
#endif /* DATA_LOGGING */
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

      /* Data from UART */
      Size = UartRead(Buffer,sizeof(Buffer)-1);
      if(Size)
      {
        /*Turn on LED2 and load led2 timer*/
        LED_ON(LED2);
        usartLedTicks = LED_HOLD_TIME;
#ifdef DATA_LOGGING
        Buffer[Size] = 0;
        printf("< %s\n",Buffer);
#endif  /* DATA_LOGGING */
        while(!UsbCdcWrite(Buffer,Size));
      }

      /* Get line and modem events from UART */
#if CDC_DEVICE_SUPPORT_LINE_STATE > 0
      /* Get line events - BI, FE, PE, OE */
      UartLineEvents = UartGetUartLineEvents();
#if UART1_MODEM_STAT_ENA > 0
      /* Get modem line events - RI, DCD and DSR */
      UartModemEvents = Uart1GetUartModemEvents();
      if(UartLineEvents.Data || UartModemEvents.Data)
      {
        /* Modem lines report - DCD, DSR and RI */
        SerialState.bRxCarrier = UartModemEvents.bDCD;
        SerialState.bTxCarrier = UartModemEvents.bDSR;
        SerialState.bRingSignal = UartModemEvents.bDRI;
#else
      if(UartLineEvents.Data)
      {
        SerialState.Data = 0;
#endif /* UART1_MODEM_STAT_ENA > 0 */
        /* Line events report BI, PE, FE and OE */
        SerialState.bBreak = UartLineEvents.bBI;
        SerialState.bFraming = UartLineEvents.bFE;
        SerialState.bOverRun = UartLineEvents.bOE;
        SerialState.bParity = UartLineEvents.bPE;
        /* Send events */
        UsbCdcReportSerialCommState(SerialState);
      }
#endif /* CDC_DEVICE_SUPPORT_LINE_STATE > 0 */
    }
    else
    {
      if(CdcConfigureStateHold == TRUE)
      {
        CdcConfigureStateHold = FALSE;
      }
    }

    /* UART line coding - Baud rate, number of the stop bits,
       number of bits of the data word and parity type */
#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
    if(UsbCdcIsNewLineCodingSettings())
    {
      CDC_LineCoding = UsbCdcGetLineCodingSettings();
      /* Update the baud rate */
      UartLineCoding.dwDTERate = CDC_LineCoding.dwDTERate;
      /* Update the stop bits number */
      UartLineCoding.bStopBitsFormat = CDC_LineCoding.bCharFormat?UART_TWO_STOP_BIT:UART_ONE_STOP_BIT;
      /* Update the parity type */
      UartLineCoding.bParityType = (CDC_LineCoding.bParityType == 0)?UART_NO_PARITY:(UartParity_t)(CDC_LineCoding.bParityType-1);
      /* Update the word width */
      UartLineCoding.bDataBits = (UartWordWidth_t)(CDC_LineCoding.bDataBits - 5);
      /* Set UART line coding */
      UartSetLineCoding(UartLineCoding);
    }
#endif /* CDC_DEVICE_SUPPORT_LINE_CODING > 0 */

    /* Get line and modem events from USB */
#if UART1_MODEM_STAT_ENA > 0 && CDC_DEVICE_SUPPORT_LINE_STATE > 0
    /* Modem lines - DTR, RTS */
    if(UsbCdcIsNewLineStateSettings())
    {
      CDC_LineState = UsbCdcGetLineStateSettings();
      UartModemLineState.bDTR = CDC_LineState.DTR_State;
      UartModemLineState.bRTS = CDC_LineState.RTS_State;
      Uart1SetModemLineState(UartModemLineState);
    }
#endif /* UART1_MODEM_STAT_ENA > 0 && CDC_DEVICE_SUPPORT_LINE_STATE > 0 */

#if CDC_DEVICE_SUPPORT_BREAK > 0
    /* Break event */
    UartSetUartLineState(UsbCdcGetBreakState());
#endif /* CDC_DEVICE_SUPPORT_BREAK > 0 */
  }

}
