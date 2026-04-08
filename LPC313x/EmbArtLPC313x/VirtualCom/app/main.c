/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : main.c
 *    Description :
 *
 *   This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for a Embedded Artsists LPC313x evaluation board.
 *   It implements USB CDC (Communication Device Class) device and install
 *  it like a Virtual COM port.
 *   The projec also shows how the system can boot from the SPI NOR Flash.
 *
 *COMPATIBILITY
 *=============
 *
 *   The Virtual COM port example project is compatible with Embedded Artsists
 *  LPC313x evaluation board. By default the project is configured to use the
 *  J-Link JTAG interface.
 *
 *CONFIGURATION
 *=============
 *
 *  The Project contains the following configurations:
 *
 *  Debug_iRAM: This configuration is intended for debugging in the
 *              microcontroller's internal RAM.
 *
 *  Debud_SDRAM: This configuration is intended for debugging in the external
 *               SDRAM.
 *
 *  Debug_SPINOR_Boot_iRAM: This confuguration will download the example in the
 *                          SPI NOR flash. The application is linked to run in
 *                          the internal RAM and is loaded by LPC IROM
 *                          bootloader. The configuration uses
 *                          FlashEmbArtLPC313x_boot.xml flash loader.
 *                          Note: This configuration uses a Hardware reset with
 *                          delay to start IROM bootloader and allow it to copy
 *                          the application. The boot jumpers should be
 *                          configured for SPI NOR flash boot.
 *
 * Debug_SPINOR_Boot_SDRAM: This confuguration will download the example in the
 *                          SPI NOR flash. The application is linked to run in
 *                          the external SDRAM and a second level bootloader is
 *                          linked to run in the internal RAM. The IROM
 *                          bootloader copies the secon level bootloader from
 *                          the SPI NOR to the interan RAM and starts it.
 *                          The second level bootloader copies the rest of the
 *                          application to SDRAM. The configuration uses
 *                          FlashEmbArtLPC313x_boot.xml flash loader to program
 *                          the secon level bootloader and
 *                          FlashEmbArtLPC313x_img.xml flash loader to program
 *                          the application.
 *
 *                          Note: This configuration uses a Hardware reset with
 *                          delay to start IROM bootloader and allow it to copy
 *                          the second level bootloader. The boot jumpers should
 *                          be configured for SPI NOR flash boot.
 *  Jumpers:
 *   (Boot from SPI Flash)
 *    BOOT0      - L
 *    BOOT1      - L
 *    BOOT2      - H
 *
 *    USB/UART
 *
 *    History :
 *    1. Date        : 25.8.2009
 *       Author      : Stanimir Bonev
 *       Description : initial revision.
 *
 *    $Revision: 28 $
 **************************************************************************/

/** include files **/
#include "includes.h"

/** local definitions **/
#define ISROM_MMU_TTBL  0x1201C000
#define SDRAM_BASE_ADDR 0x30000000
#define IRAM_BASE_ADDR  0x11028000

#define GPIO2_BOOT_SET  {IOCONF_GPIO_M1_SET = 4;IOCONF_GPIO_M0_SET  = 4;}
#define GPIO2_BOOT_CLR  {IOCONF_GPIO_M1_SET = 4;IOCONF_GPIO_M0_CLR  = 4;}


#define   TIMER1_IN_FREQ       6MHZ
#define   TIMER1_TICK_MAX      1000   //in miliseconds
#define   TIMER1_TICK_MIN      20     //in miliseconds

#define   TMR1TICK(tick)       ((TIMER1_IN_FREQ/1000)*tick)/256 //Convert miliseconds in Tmr ticks

#define   MOUSE_DELTA          1

/** external functions **/
extern void InitSDRAM(void);
extern void Dly_us(Int32U Dly);

/** external data **/
Int32U Tmr1Tick = TMR1TICK(2);

volatile Boolean Tick;

/** internal functions **/
/*************************************************************************
 * Function Name: Timer1IntrHandler
 * Parameters: None
 *
 * Return: None
 *
 * Description: Timer 1 Interrupt service routine. Reload Timer
 *              with Tmr1Tick and toggle the GPIO2_BOOT LED
 *
 *************************************************************************/
void Timer1IntrHandler (void)
{

  Timer1Load = Tmr1Tick;

  Timer1Clear = 0;              // clear timer interrupt

  Tick = TRUE;
}

/*************************************************************************
 * Function Name: main
 * Parameters: None
 *
 * Return: None
 *
 * Description: Getting Started main loop
 *
 *************************************************************************/
void main(void)
{
Int32U blink_led_div = 0;
Boolean led_state = FALSE;

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

  CP15_Mmu(FALSE);            // Disable MMU
  CP15_ICache(TRUE);          // Enable ICache

 // Init MMU
 // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_SetTtb((Int32U *)ISROM_MMU_TTBL);  //Set translation table base address
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache

#ifndef DBG_SDRAM
  /*Init Clocks only if debug is not in SDRAM.
    if DBG_SDRAM the Clock is init by bootloader
    or mac file*/
  InitClock();
  /*Init SDRAM only if debug is not in SDRAM.
    if DBG_SDRAM the SDRAM is init by bootloader
    or mac file*/
  InitSDRAM();
#endif /*DBG_SDRAM*/

  /*Init SPI Nor Flash*/
  InitSPINOR();
  /*Init Interrupt Controller.
    Arm Vector Copy to beginnint of the IRAM*/
  INTC_Init((Int32U *)IRAM_BASE_ADDR);
  /*Remap IRAM at address 0*/
  SYSCREG_ARM926_901616_LP_SHADOW_POINT = IRAM_BASE_ADDR;
  /*Enable Timer 1 Clock*/
  CGU_Run_Clock(TIMER1_PCLK);
  /*Init Timer 1*/
  Timer1Ctrl_bit.Enable = 0;    // disable counting
  Timer1Ctrl_bit.Mode = 1;      // reload mode
  Timer1Ctrl_bit.PreScale = 2;  // prescaler = CGU clock rate / 256
  Timer1Load = Tmr1Tick;
  // set timer 0 period
  Timer1Clear = 0;              // clear timer pending interrupt
  /*Install Interrupt Service Routine,
    Priority 3*/
  INTC_IRQInstall(Timer1IntrHandler, TIMER0_INTCT1,
                  3,0);
  /*Enable Timer 1 interrupt*/
  INTC_IntEnable(TIMER0_INTCT1, 1);

  // Init UART 0
  UartInit(UART_0,4);
  // Init USB
  UsbCdcInit();

  __enable_irq();

  /* Start Timer 1*/
  Timer1Ctrl_bit.Enable = 1;

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
          TranSize = UartWrite(UART_0,pBuffer,Size);
        }
        while(Size != TranSize);
      }

      // Data from UART1
      Size = UartRead(UART_0,Buffer,sizeof(Buffer)-1);
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
      UartLineEvents = UartGetUartLineEvents(UART_0);
      if(UartLineEvents.Data)
      {
        SerialState.Data = 0;
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
      UartSetLineCoding(UART_0,UartLineCoding);
    }
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0

#if CDC_DEVICE_SUPPORT_BREAK > 0
    // Break event
    UartSetUartLineState(UART_0,UsbCdcGetBreakState());
#endif // CDC_DEVICE_SUPPORT_BREAK > 0
    if(Tick)
    {
      Tick = FALSE;
      if(100 < ++blink_led_div)
      {
        blink_led_div = 0;
        if(led_state ^= TRUE)
        {
          // LED ON
          GPIO2_BOOT_CLR;
        }
        else
        {
          GPIO2_BOOT_SET;
        }
      }
    }
  }
}


