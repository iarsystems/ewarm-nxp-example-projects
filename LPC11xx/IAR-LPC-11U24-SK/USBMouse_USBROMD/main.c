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
 *    to develop code for IAR-LPC11U24-SK board. It implements USB HID mouse
 *    using on-chip USB drivers.
 *     The USER and WAKE-UP buttons control horizontal movement and the AN_TRIM
 *    controls vertical movement.
 *
 *    Controls:
 *    USER        - left
 *    WAKE-UP     - right
 *    AN_TRIM     - up/down
 *
 *
 *    COMPATIBILITY
 *    =============
 *
 *      The example project is compatible with IAR-LPC11U24-SK evaluation board.
 *    By default, the project is configured to use the J-Link SWD interface.
 *
 *    CONFIGURATION
 *    =============
 *    Jumpers:
 *   RST_E     - unfilled
 *   ISP_E     - unfilled
 *   IOCON_PIO1_LOW  - unfilled
 *   PIO3_HIGH - unfilled
 *   PIO3_E    - filled
 *   TMS/SWD   - SWD (2-3)
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
 *    2. Date        : July 20, 2012
 *       Author      : Atanas Uzunov
 *       Description : Changed to use the on-chip USB drivers.
 *
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"
#include <yfuns.h>

#define DLY_100US             550

/*variable for critical section entry control*/
uint32_t CriticalSecCntr;

USBD_API_T* pUsbApi;

volatile uint32_t Ticks;
volatile uint32_t AdcVal;

/**********************************************************************
 ** Global data
**********************************************************************/
USBD_API_T* pUsbApi;

/* local data */
static USBD_HANDLE_T hUsb;

/** internal functions **/
static int MyLowLevelGetchar(void);

/** public functions **/
/*************************************************************************
 * Function Name: USB_IRQHandler
 * Parameters: none
 * Return: void
 *
 * Description: USB_IRQ Interrupt Handler
 *
 *************************************************************************/
void USB_IRQHandler(void)
{
  pUsbApi->hw->ISR(hUsb);
}

/*************************************************************************
 * Function Name: NVIC_IntEnable
 * Parameters: IntNumber - Interrupt number
 * Return: void
 *
 * Description: Enable interrupt at NVIC
 *
 *************************************************************************/
void NVIC_IntEnable(uint32_t IntNumber)
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
 *************************************************************************/
void NVIC_IntDisable(uint32_t IntNumber)
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
 *************************************************************************/
void NVIC_ClrPend(uint32_t IntNumber)
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
 *************************************************************************/
void NVIC_IntPri(uint32_t IntNumber, Int8U Priority)
{
volatile uint8_t * pNVIC_IntPri = (Int8U *)&IP0;

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_USB_WAKEUP >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;
  pNVIC_IntPri += IntNumber;
  *pNVIC_IntPri = Priority;
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
 * Function Name: InitClock
 * Parameters: clock, AHB divider
 * Return: void
 *
 * Description: Initialize PLL to desired clock and  AHB divider
 *              Sys clock is Sys PLL output
 *
 *************************************************************************/
void InitClock(uint32_t clock, uint32_t ahbdiv)
{
  /*Sys Oscillator Enable*/
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
  uint32_t m = clock/(MAIN_OSC_FREQ*(2*2)) - 1;

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
  /*Init USB clock system*/
  /*Power down USBPLL*/
  PDRUNCFG_bit.USBPLL_PD = 1;
  /*Power-up USB Phy*/
  PDRUNCFG_bit.USBPAD_PD = 0;
  /*Select system oscillator and update*/
  USBPLLCLKUEN = 0;
  USBPLLCLKSEL = 1;
  USBPLLCLKUEN = 1;
  USBPLLCTRL = ((48MHZ / MAIN_OSC_FREQ) - 1)     /*MSEL*/
             |  (2<<5)  /*PSEL = 2*/
             ;
  /*Wake up USBPLL*/
  PDRUNCFG_bit.USBPLL_PD = 0;
  /*Wain until PLL locks*/
  while(!(USBPLLSTAT_bit.LOCK));
  /*Set USB clock - 48MHz*/
  USBCLKSEL = 0;
  USBCLKDIV = 1;
}

/*************************************************************************
 * Function Name: SYS_GetMainClk
 * Parameters: none
 * Return: uint32_t
 *
 * Description: return Main Clock [Hz]
 *
 *************************************************************************/
uint32_t SYS_GetMainClk(void)
{
uint32_t Clk;
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
 * Parameters: uint32_t BaudRate
 * Return: none
 *
 * Description: Configures the USART Module
 *
 *************************************************************************/
void USART_Init (uint32_t BaudRate)
{
uint32_t Pclk;
uint32_t div;
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
 * Function Name: __write
 * Parameters: Low Level cahracter output
 *
 * Return:
 *
 * Description:
 *
 *************************************************************************/
size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize)
{
size_t nChars = 0;

  for (/*Empty */; Bufsize > 0; --Bufsize)
  {
    while( !U0LSR_bit.THRE );  /* Wait */
    U0THR = * Buf++;
    ++nChars;
  }
  return nChars;
}
/*************************************************************************
 * Function Name: __read
 * Parameters: Low Level cahracter input
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
 * Function Name: CT32B0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer32 0 interrupt handler
 *
 *************************************************************************/
void CT32B0_IRQHandler (void)
{
  ++Ticks;
  /* clear interrupt */
  TMR32B0IR_bit.MR0INT = 1;
  AdcVal =  AD0GDR_bit.RESULT;
  NVIC_ClrPend(NVIC_CT32B0);
}

/*************************************************************************
 * Function Name: CT32B0_Init
 * Parameters: uint32_t tps
 *
 * Return: none
 *
 * Description: Timer32 0 interrupt handler
 *
 *************************************************************************/
void CT32B0_Init(uint32_t tps)
{
  Ticks = 0;
  /* Enable Timer CT32B0 clocks */
  SYSAHBCLKCTRL_bit.CT32B0 = 1;

  /* Init Timer CT32B0 */
  TMR32B0TCR_bit.CE   = 0;   /* counting  disable */
  TMR32B0TCR_bit.CR   = 1;   /* set reset */
  TMR32B0TCR_bit.CR   = 0;   /* release reset */

  TMR32B0MCR_bit.MR0I = 1;   /* Enable Interrupt on MR0 */
  TMR32B0MCR_bit.MR0R = 1;   /* Enable reset on MR0 */
  TMR32B0MCR_bit.MR0S = 0;   /* Disable stop on MR0 */

  /* Set Timer CT32B0 period */
  TMR32B0PR = 0;
  TMR32B0MR0 = (SYS_GetMainClk()/(SYSAHBCLKDIV))/(tps);

  /* Init Timer CT32B0 interrupt */
  TMR32B0IR_bit.MR0INT = 1;  /* clear pending interrupt */
  TMR32B0TCR_bit.CE    = 1;  /* counting Enable */

  /*Enable NVIC TMR0 Interrupt */
  NVIC_IntEnable(NVIC_CT32B0);
  NVIC_IntPri(NVIC_CT32B0,16);
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
  //TEMP_PIN_IOCON = 1;

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
 * Function Name: USB_Gpio_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB hardware initialization. Init GPIO and clock source
 *              of USB RAM.
 *
 *************************************************************************/
void USB_Gpio_Init(void)
{
  /* Enable AHB clock to the GPIO domain. */
  SYSAHBCLKCTRL |= (1<<6);

  /* Enable AHB clock to the USB block and USB RAM. */
  SYSAHBCLKCTRL |= ((0x1<<14)|(0x1<<27));

  /* Pull-down is needed, or internally, VBUS will be floating. This is to
  address the wrong status in VBUSDebouncing bit in CmdStatus register (applies to rev A) */
  IOCON_PIO0_3   &= ~0x1F;

  IOCON_PIO0_3   |= (0x01<<0);			/* Secondary function VBUS */
  IOCON_PIO0_6   &= ~0x07;
  IOCON_PIO0_6   |= (0x01<<0);			/* Secondary function SoftConn */
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
uint32_t ADC_Hold;
USBD_API_INIT_PARAM_T usb_param;
ErrorCode_t ret;
USB_CORE_DESCS_T desc;
static USB_CORE_CTRL_T* pCtrl;

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
  LED_OFF(LED1_PORT,LED1_MASK);
  /* UART Init: 115200 bps, 8-bit,
    1-Stop bit, Parity - None */
  USART_Init(115200);
  /* Enable Timer. Ticks 50 times per second */
  CT32B0_Init(50);
  /* Init LCD */
  HD44780_IO_Init();

  if(HD44780_OK != HD44780_PowerUpInit())
  {/* If LCD Init fails */
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

  /* Get USB API table pointer */
  pUsbApi = (USBD_API_T*)((*(ROM **)(0x1FFF1FF8))->pUSBD);

  /* Init USB Gpio */
  USB_Gpio_Init();

  /* Initialize callback structures */
  memset((void*)&usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
  usb_param.usb_reg_base = LPC_USB_BASE;
  usb_param.mem_base = LPC_USB_RAM_BEGIN;
  usb_param.mem_size = LPC_USB_RAM_SIZE;
  usb_param.max_num_ep = 2;
  usb_param.USB_Configure_Event = USB_Configure_Event;

  /* Initialize Descriptor pointers */
  memset((void*)&desc, 0, sizeof(USB_CORE_DESCS_T));
  desc.device_desc = (uint8_t *)&USB_DeviceDescriptor[0];
  desc.string_desc = (uint8_t *)&USB_StringDescriptor[0];
  desc.full_speed_desc = (uint8_t *)&USB_FsConfigDescriptor[0];
  desc.high_speed_desc = (uint8_t *)&USB_FsConfigDescriptor[0];

  /* USB Initialization */
  /* Get needed USB memory size */
  uint32_t res = pUsbApi->hw->GetMemSize(&usb_param);

  /* If there is no enough USB memory */
  if (res > LPC_USB_RAM_SIZE) {
    /* show an error message */
    printf("\n\rNot enough USB RAM!");
    HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)"RAM Err ");
    while(1);
  }

  /* Init USB hardware */
  ret = pUsbApi->hw->Init(&hUsb, &desc, &usb_param);
  if (ret == LPC_OK)
  {
    /* Init HID API */
    ret = USB_HID_Init(hUsb, (USB_INTERFACE_DESCRIPTOR *)&USB_FsConfigDescriptor[sizeof(USB_CONFIGURATION_DESCRIPTOR)], &usb_param.mem_base, &usb_param.mem_size);
    if (ret == LPC_OK) {
      /* Enable USB interrupt */
      NVIC_IntEnable(NVIC_USB_IRQ);
      /* USB connect */
      pUsbApi->hw->Connect(hUsb, 1);
    }
    else
    {
      printf("\n\rUSB Init error!");
      HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)"Init Err");
      while(1);
    }
  }

  HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)"ROMD HID");

  printf("\n\r");
  printf("\n\r");
  printf("***************************\n\r");
  printf("*     IAR LPC11U24 SK     *\n\r");
  printf("*   USBROMD HID example   *\n\r");
  printf("***************************\n\r");

  __enable_interrupt();

  /* Init ADC */
  ADC_Init();

  /* Soft connection enable */
  ADC_Hold = AdcVal;

  while(1)
  {
    if(Ticks)
    {
      Ticks = FALSE;
      /* check if USB device is configured */
      pCtrl = (USB_CORE_CTRL_T*)hUsb;
      if (pCtrl->config_value)
      {
        int8_t h = 0, v = 0;
        int tmp;
        if(!USER_PORT)
        {
          h -= 8;
          LED_ON(LED1_PORT,LED1_MASK);
          LED_OFF(LED2_PORT,LED2_MASK);
        }
        else if(!WAKE_UP_PORT)
        {
          h += 8;
          LED_ON(LED2_PORT,LED2_MASK);
          LED_OFF(LED1_PORT,LED1_MASK);
        }
        else
        {
          LED_OFF(LED1_PORT,LED1_MASK);
          LED_OFF(LED2_PORT,LED2_MASK);
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
          pUsbApi->hw->WakeUp(hUsb);
          MouseReport.X = h;
          MouseReport.Y = v;
          MouseReport.Buttons = 0;
          report_changed = 1;
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
