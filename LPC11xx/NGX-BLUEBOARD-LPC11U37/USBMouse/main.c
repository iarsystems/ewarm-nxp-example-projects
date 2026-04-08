/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : main.c
 *
 *    DESCRIPTION
 *    ===========
 *     This example project shows how to use the IAR Embedded Workbench for ARM
 *    to develop code for NGX-BLUEBOARD-LPC11U37 board. It implements USB HID
 *    mouse.
 *     The EXTERNAL and WAKEUP buttons control the horizontal movement and the
 *    R14 controls the vertical movement of mouse cursor.
 *
 *    Controls:
 *    EXTERNAL  - left
 *    WAKEUP    - right
 *    R14       - up/down
 *
 *
 *    CONFIGURATION
 *    =============
 *
 *      The USB HID mouse application is downloaded to the iFlash.
 *
 *
 *    COMPATIBILITY
 *    =============
 *
 *      The example project is compatible with NGX-BLUEBOARD-LPC11U37 board.
 *    By default, the project is configured to use the J-Link SWD interface.
 *
 *
 *    GETTING STARTED
 *    ===============
 *
 *    1) Build and download the program.
 *
 *    2) Run the program.
 *
 *    3) Use a USB cable to connect the evaluation board to your PC. The first
 *       time a USB device is connected to your computer, Windows will
 *       automatically load the proper device driver. In this case, the USB
 *       HID mouse device driver will be loaded.
 *
 *    1. Date        : May 12, 2011
 *       Author      : Stanimir Bonev
 *       Description : Initial Revision
 *
 *    2. Date        : June 15, 2012
 *       Author      : Atanas Uzunov
 *       Description : Adapted for NGX-BLUEBOARD-LPC11U37
 *
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"
#include <yfuns.h>

#define DLY_100US             550

/* variable for critical section entry control */
Int32U CriticalSecCntr;

volatile Int32U Ticks;
volatile Int32U AdcVal;

/** external data **/

/** internal functions **/
static int MyLowLevelGetchar(void);

/** public functions **/
/*************************************************************************
 * Function Name: NVIC_IntEnable
 * Parameters: IntNumber - Interrupt number
 * Return: void
 *
 * Description: Enable interrupt at NVIC
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
 * Description: Disables interrupt at NVIC
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
  /* Sys Oscillator Enable */
  SYSOSCCTRL = (MAIN_OSC_FREQ>(20MHZ))?(0x2):(0x0);
  /* Power Up SYS Oscillator */
  PDRUNCFG_bit.SYSOSC_PD = 0;
  /* Enable Internal RC oscillator */
  PDRUNCFG_bit.IRC_PD = 0;
  /* Select internal RC oscillator for
    Sys clock source */
  MAINCLKUEN = 0;
  MAINCLKSEL = 0;
  MAINCLKUEN = 1;
  /* Configure SYS PLL */
  /* Power Down SYS PLL */
  PDRUNCFG_bit.SYSPLL_PD = 1;
  /* Select Sys Oscillator for
    SYS PLL source */
  SYSPLLCLKUEN = 0;
  SYSPLLCLKSEL = 1;
  SYSPLLCLKUEN = 1;
  /* Calculate M */
  Int32U m = clock/(MAIN_OSC_FREQ*(2*2)) - 1;

  assert(m<32);
  /* Configure PLL frequency */
  SYSPLLCTRL =  (m)     /* MSEL */
             |  (1<<5)  /* PSEL = 2 */
             ;
  /* Power Up PLL */
  PDRUNCFG_bit.SYSPLL_PD = 0;
  /* Set Sys AHB Clock divider */
  SYSAHBCLKDIV_bit.DIV = ahbdiv;
  /* Wain until PLL locks */
  while(!(SYSPLLSTAT_bit.LOCK));
  /* Select Sys PLL Output for
     Sys clock source */
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
             |  (2<<5)                           /*PSEL = 2*/
             ;
  /* Wake up USBPLL */
  PDRUNCFG_bit.USBPLL_PD = 0;
  /* Wain until PLL locks */
  while(!(USBPLLSTAT_bit.LOCK));
  /* Set usb clock - 48MHz */
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

/*************************************************************************
 * Function Name: USART_Init
 * Parameters: Int32U BaudRate - the baud rate.
 *
 * Return:
 *
 * Description: Initialize the USART module.
 *
 *************************************************************************/
void USART_Init (Int32U BaudRate)
{
Int32U Pclk;
Int32U div;
  /* Disable Clock */
  USARTCLKDIV = 0;
  /* Configure IOs */
  /* Enable I/O Configuration registers */
  SYSAHBCLKCTRL_bit.IOCON = 1;
  IOCON_PIO0_19 = 1;             /* Uart TX function select */
  IOCON_PIO0_18 = 1;             /* Uart RX function select */

  div = 1;
  while((Pclk=SYS_GetMainClk()/USARTCLKDIV) > 50MHZ)
  {
    div++;
  }
  /* Enable Clock */
  USARTCLKDIV = div;

  /* Enable UART register clock */
  SYSAHBCLKCTRL_bit.USART = 1;

  U0LCR_bit.WLS = 3;        /* Select 8-Bits */
  U0LCR_bit.SBS = 0;        /* One Stop Bit */
  U0LCR_bit.PE = 0;         /* No parity */

  U0LCR_bit.DLAB = 1;       /* Divisor Latch Access ebabled */
  /* Set UART0 BAUDRATE */
  U0DLL = ((((Pclk+8)/16)+BaudRate/2)/BaudRate)%256;
  U0DLM = ((((Pclk+8)/16)+BaudRate/2)/BaudRate)/256;
  U0LCR_bit.DLAB = 0;       /* Clear Divisor Latch Access bit */

  U0FCR = (1<<0);           /* Enable UART0 FIFOs. */
  U0TER = (1<<7);           /* Enable Transmission */
}

/*************************************************************************
 * Function Name: __write
 * Parameters: Low Level character output
 *
 * Return:
 *
 * Description:
 *
 *************************************************************************/
size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize)
{
size_t nChars = 0;

  for (/* Empty */; Bufsize > 0; --Bufsize)
  {
    while( !U0LSR_bit.THRE );  /* Wait */
    U0THR = * Buf++;
    ++nChars;
  }
  return nChars;
}
/*************************************************************************
 * Function Name: __read
 * Parameters: Low Level character input
 *
 * Return:
 *
 * Description:
 *
 *************************************************************************/
size_t __read(int handle, unsigned char * buffer, size_t size)
{
int nChars = 0;

  /* This template only reads from "standard in", for all other file
   * handles it returns failure. */
  if (handle != _LLIO_STDIN)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size > 0; --size)
  {
    int c = MyLowLevelGetchar();
    if (c < 0)
      break;

    *buffer++ = c;
    ++nChars;
  }

  return nChars;
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
  AdcVal =  AD0GDR_bit.RESULT;

   NVIC_ClrPend(NVIC_CT32B0);
}

/*************************************************************************
 * Function Name: CT32B0_Init
 * Parameters: Int32U tps - ticks per second
 *
 * Return: none
 *
 * Description: CT32B0 interrupt handler
 *
 *************************************************************************/
void CT32B0_Init(Int32U tps)
{
  Ticks = 0;
  /* Enable TIM0 clocks */
  SYSAHBCLKCTRL_bit.CT32B0 = 1;
  /* Init Timer 0 */
  TMR32B0TCR_bit.CE = 0;    /* counting  disable */
  TMR32B0TCR_bit.CR = 1;   /* set reset */
  TMR32B0TCR_bit.CR = 0;   /* release reset */
  TMR32B0MCR_bit.MR0I = 1;   /* Enable Interrupt on MR0 */
  TMR32B0MCR_bit.MR0R = 1;   /* Enable reset on MR0 */
  TMR32B0MCR_bit.MR0S = 0;   /* Disable stop on MR0 */
  /* Set Timer 0 period */
  TMR32B0PR = 0;
  TMR32B0MR0 = (SYS_GetMainClk()/(SYSAHBCLKDIV))/(tps);
  /* Init Timer 0 interrupt */
  TMR32B0IR_bit.MR0INT = 1;  /* clear pending interrupt */
  TMR32B0TCR_bit.CE = 1;    /* counting Enable */
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
 * Description: Stops the CT32B0 timer.
 *
 *************************************************************************/
void CT32B0_Stop(void)
{
  TMR32B0TCR_bit.CE = 0;       /* counting  disable */

  NVIC_IntDisable(NVIC_CT32B0);

  SYSAHBCLKCTRL_bit.CT32B0 = 0; /* disable clock */
}

/*************************************************************************
 * Function Name: ADC_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: ADC Initialzation.
 *
 *************************************************************************/
void ADC_Init(void)
{
  /* ADC Init */
  /* Enable ADC */
  SYSAHBCLKCTRL_bit.ADC = 1;
  PDRUNCFG_bit.ADC_PD = 0;
  /**/
  ANALOG_TRIM_IOCON = 1;

  AD0CR_bit.CLKDIV = ((SYS_GetMainClk()/(SYSAHBCLKDIV)) / 4500000) + 1;

  AD0CR_bit.SEL = 0;
  /* Disable all interrupts */
  AD0INTEN = 0;
  AD0CR_bit.START = 0;

  /* Select ANALOG_TRIM_CHANNEL */
  AD0CR_bit.SEL = 1<<ANALOG_TRIM_CHANNEL;
  AD0CR_bit.BURST = 1;
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
Int32U ADC_Hold;

  /* Init clock */
  InitClock(132MHZ, 2);
  /* Enable GPIO Clock */
  SYSAHBCLKCTRL_bit.GPIO = 1;
  /* Enable IOCON Clock */
  SYSAHBCLKCTRL_bit.IOCON = 1;
  /* Set all pins as input ports */
  GpioInit();
  /* Init LED Ports */
  LED1_DIR |= LED1_MASK;
  LED2_DIR |= LED2_MASK;
  /* Turn off LED2 */
  LED_OFF(LED2_PORT,LED2_MASK);
  /* Turn on LED1 */
  LED_ON(LED1_PORT,LED1_MASK);
  /* UART Init: 115200 bps, 8-bit,
    1-Stop bit, Parity - None */
  USART_Init(115200);
  /* Enable Timer. Ticks 50 times per second */
  CT32B0_Init(50);
  /* Init LCD */
  init_glcd();
  glcd_clear();
  init_xy_axis(PAGE0,COL0 + 27);
  back_light_on();

  /* Init */
  HidInit();

  lcd_display_string((unsigned char *)"IAR SYSTEMS");
  init_xy_axis(PAGE0 + 2,COL0 + 14);
  lcd_display_string((unsigned char *)"NGX-BB-LPC11U37");
  init_xy_axis(PAGE0 + 4,COL0 + 30);
  lcd_display_string((unsigned char *)"BLUEBOARD");
  init_xy_axis(PAGE0 + 6,COL0 + 2);
  lcd_display_string((unsigned char *)"USB HID Device Example");

  printf("\n\r");
  printf("\n\r");
  printf("***************************\n\r");
  printf("*   BLUEBOARD-LPC11U37    *\n\r");
  printf("* USB HID device example  *\n\r");
  printf("***************************\n\r");

  __enable_interrupt();

  /* Init ADC */
  ADC_Init();

  /* Soft connection enable */
  USB_ConnectRes(TRUE);
  ADC_Hold = AdcVal;

  while(1)
  {
    if(Ticks)
    {
      Ticks = FALSE;
      if(UsbCoreReq(UsbCoreReqDevState) == UsbDevStatusConfigured)
      {
        Int8S h = 0, v = 0;
        int tmp;
        if(!USER_PORT)
        {
          h -= 8;
        }
        else
        {
          if(!WAKE_UP_PORT)
          {
            h += 8;
          }
        }

        tmp = ADC_Hold - AdcVal;
        tmp >>= 1;
        ADC_Hold = AdcVal;
        if(tmp > 127)
        {
          tmp = 127;
        }
        else if (tmp < -128)
        {
          tmp = -128;
        }
        v = tmp;
        if(h || v)
        {
          UsbWakeUp();
          HidMouseSendReport(h,v,0);
        }
      }
    }
  }
}

/** private functions **/
static int MyLowLevelGetchar(void)
{
int ch;

  if(U0LSR_bit.DR)
  {
    ch = U0RBR;
    if(U0LSR_bit.OE || U0LSR_bit.PE || U0LSR_bit.FE )
    {
      return (ch | 0x10000000);
    }
    return (ch & 0xff );
  }
  return -1;
}
