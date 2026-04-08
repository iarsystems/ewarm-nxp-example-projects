/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : main.c
 *    DESCRIPTION
 *    ===========
 *      This example project shows how to use the IAR Embedded Workbench for ARM
 *     to develop code for IAR-LPC-122X-SK board. It shows basic use of I/O,
 *     timer, ADC and interrupt controllers.
 *     It shows the Trimer position no LCD and UART
 *
 *    COMPATIBILITY
 *    =============
 *
 *      The example project is compatible with IAR-LPC-122X-SK evaluation
 *       board. By default, the project is configured to use the J-Link SWD
 *     interface.
 *
 *    CONFIGURATION
 *    =============
 *
 *     Jumpers:
 *     RST_E   - unfilled
 *     ISP_E   - unfilled
 *
 *     The GettingStarted application is downloaded to the iFlash.
 *
 *    GETTING STARTED
 *     ===============
 *
 *       1) Start the IAR Embedded Workbench for ARM.
 *
 *       2) Select File->Open->Workspace...
 *        Open the following workspace:
 *
 *         <installation-root>\arm\examples\NXP\
 *          LPC12xx\IAR-LPC-122X-SK\GettingStarted\GettingStarted.eww
 *
 *       3) Run the program.
 *
 *    1. Date        : Octomber  2010
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US             550


#include <yfuns.h>

#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = CRP;

/*variable for clitical section entry control*/
Int32U CriticalSecCntr;
volatile Int32U Ticks;
/** external data **/

//** internal functions **/
static int MyLowLevelGetchar(void);
/** public functions **/
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

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

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

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

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

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;

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

  assert((NVIC_WAKE_UP0 <= IntNumber) && (NVIC_PIO_0 >= IntNumber));
  IntNumber -= NVIC_WAKE_UP0;
  pNVIC_IntPri += IntNumber;
  *pNVIC_IntPri = Priority;
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: clock, AHB devider
 * Return: void
 *
 * Description: Initialize PLL to desired clock and  AHB devider
 *              Sys clock is Sys PLL output
 *
 *
 *************************************************************************/
void InitClock(Int32U clock, Int32U ahbdiv)
{
  /*Sys Oscilator Enable*/
  SYSOSCCTRL = (MAIN_OSC_FREQ>(20MHZ))?(0x2):(0x0);
  /*Power Up SYS Oscilator*/
  PDRUNCFG_bit.SYSOSC_PD = 0;
  /*Enable Internal RC oscilator*/
  PDRUNCFG_bit.IRC_PD = 0;
  /*Select internal RC oscilator for
    Sys clock source*/
  MAINCLKUEN = 0;
  MAINCLKSEL = 0;
  MAINCLKUEN = 1;
  /*Configure SYS PLL*/
  /*Power Down SYS PLL*/
  PDRUNCFG_bit.SYSPLL_PD = 1;
  /*Select Sys Oscilator for
    SYS PLL source*/
  SYSPLLCLKUEN = 0;
  SYSPLLCLKSEL = 1;
  SYSPLLCLKUEN = 1;
  /*Calc M*/
  Int32U m = clock/MAIN_OSC_FREQ - 1;

  assert(m<32);
  /*Configure PLL frequency*/
  SYSPLLCTRL =  (m)    /*MSEL*/
             |  (0<<5) /*PSEL = 1*/
             |  (0<<7) /*DIRECT = 0*/
             |  (0<<8); /*BYPASS=0*/

  /*Power Up PLL*/
  PDRUNCFG_bit.SYSPLL_PD = 0;
  /*Set Sys AHB Clock devider*/
  SYSAHBCLKDIV_bit.DIV = ahbdiv;
  /*Wain until PLL locks*/
  while(!(SYSPLLSTAT_bit.LOCK));
  /*Select Sys PLL Output for
    Sys clock source*/
  MAINCLKUEN = 0;
  MAINCLKSEL = 3;
  MAINCLKUEN = 1;
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

      Clk *= (SYSPLLCTRL_bit.MSEL+1);
    break;

    default:
      Clk = 0;
    break;
  }
  return Clk;
}

/*************************************************************************
 * Function Name: UART_Init
 * Parameters: Int32U BaudRate - desired UART baud rate
 * Return: none
 *
 * Description: Inits UART0 at desired baud rate, 1 stop bit, no pority
 *              and no flow control
 *
 *************************************************************************/
void UART_Init (Int32U BaudRate)
{
Int32U Pclk;
  /*Enable Clocks*/
  UART0CLKDIV = 1;
  /*Set UART devider*/
  while((Pclk=SYS_GetMainClk()/UART0CLKDIV)> 70MHZ)
  {
    UART0CLKDIV ++;
  }
  /*Enable UART register clock*/
  SYSAHBCLKCTRL_bit.UART0 = 1;
  /*Enable I/O Configuration registers*/
  SYSAHBCLKCTRL_bit.IOCON = 1;

  U0FDR = 0x10;             //Fractional devider not used

  UART_TX_IOCON = 0x82;     //Uart ÒX function select
  UART_RX_IOCON = 0x82;     //Uart RX function select

  U0LCR_bit.WLS = 3;        //Select 8-Bits
  U0LCR_bit.SBS = 0;        //One Stop Bit
  U0LCR_bit.PE = 0;         //No parity

  U0LCR_bit.DLAB = 1;       //Devisor Latch Access ebabled
  //Set UART0 BAUDRATE
  U0DLL = (((Pclk/16)+BaudRate/2)/BaudRate)%256;  //
  U0DLM = (((Pclk/16)+BaudRate/2)/BaudRate)/256;  //
  U0LCR_bit.DLAB = 0;           //Clear Devisor Latch Access bit

  U0FCR = (1<<0);     //Enable UART0 FIFOs.
  U0TER = (1<<7);     //Enable Transmission
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
    while( !U0LSR_bit.THRE );  //Wait
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
  /*Enable IOCON Clock*/
  SYSAHBCLKCTRL_bit.IOCON = 1;
  /*Set all ports as inputs*/
  GPIO0DIR = 0;
  GPIO1DIR = 0;
  GPIO2DIR = 0;
  /* Reset all GPIO pins to default primary function
     pull ups disabled*/
  IOCON_PIO0_0 = 0x80;
  IOCON_PIO0_3 = 0x80;
  IOCON_PIO0_4 = 0x80;
  IOCON_PIO0_5 = 0x80;
  IOCON_PIO0_6 = 0x80;
  IOCON_PIO0_7 = 0x80;
  IOCON_PIO0_8 = 0x80;
  IOCON_PIO0_9 = 0x80;
  IOCON_PIO0_12 = 0x80;
  IOCON_PIO0_14 = 0x80;
  IOCON_PIO0_15 = 0x80;
  IOCON_PIO0_16 = 0x80;
  IOCON_PIO0_17 = 0x80;
  IOCON_PIO0_18 = 0x80;
  IOCON_PIO0_19 = 0x80;
  IOCON_PIO0_20 = 0x80;
  IOCON_PIO0_21 = 0x80;
  IOCON_PIO0_22 = 0x80;
  IOCON_PIO0_23 = 0x80;
  IOCON_PIO0_24 = 0x80;
  IOCON_PIO0_27 = 0x80;
  IOCON_PIO0_28 = 0x80;
  IOCON_PIO0_29 = 0x80;
  IOCON_PIO1_2 = 0x80;
  IOCON_PIO1_3 = 0x80;
  IOCON_PIO1_4 = 0x80;
  IOCON_PIO1_5 = 0x80;
  IOCON_PIO1_6 = 0x80;
  IOCON_PIO2_0 = 0x80;
  IOCON_PIO2_1 = 0x80;
  IOCON_PIO2_2 = 0x80;
  IOCON_PIO2_3 = 0x80;
  IOCON_PIO2_4 = 0x80;
  IOCON_PIO2_5 = 0x80;
  IOCON_PIO2_6 = 0x80;
  IOCON_PIO2_7 = 0x80;
  IOCON_PIO2_8 = 0x80;
  IOCON_PIO2_9 = 0x80;
  IOCON_PIO2_10 = 0x80;
  IOCON_PIO2_11 = 0x80;
  IOCON_PIO2_12 = 0x80;
  IOCON_PIO2_13 = 0x80;
  IOCON_PIO2_14 = 0x80;
  IOCON_PIO2_15 = 0x80;

  IOCON_PIO0_30 = 0x81;
  IOCON_PIO0_31 = 0x81;
  IOCON_PIO1_0 = 0x81;
  IOCON_PIO1_1 = 0x81;


  IOCON_PIO0_10 = 0x100;
  IOCON_PIO0_11 = 0x100;
}
/*************************************************************************
 * Function Name: TMR0_IRQHandler
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
  // clear interrupt
  TMR32B0IR_bit.MR0INT = 1;
  /**/
  NVIC_ClrPend(NVIC_CT32B0);
}

void CT32B0_Init(Int32U tps)
{
  Ticks = 0;
  // Enable TIM0 clocks
  SYSAHBCLKCTRL_bit.CT32B0 = 1;

  // Init Time0
  TMR32B0TCR_bit.CE = 0;     // counting  disable
  TMR32B0TCR_bit.CR = 1;     // set reset
  TMR32B0TCR_bit.CR = 0;     // release reset
  TMR32B0CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  TMR32B0MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  TMR32B0MCR_bit.MR0R = 1;   // Enable reset on MR0
  TMR32B0MCR_bit.MR0S = 0;   // Disable stop on MR0
  // set timer 0 period
  TMR32B0PR = 0;
  TMR32B0MR0 = (SYS_GetMainClk()/(SYSAHBCLKDIV))/(tps);
  // init timer 0 interrupt
  TMR32B0IR_bit.MR0INT = 1;  // clear pending interrupt
  TMR32B0TCR_bit.CE = 1;     // counting Enable
   /*Enable NVIC TMR0 Interrupt*/
  NVIC_IntEnable(NVIC_CT32B0);
  NVIC_IntPri(NVIC_CT32B0,16);
}

void CT32B0_Stop(void)
{
  TMR32B0TCR_bit.CE = 0;     // counting  disable

  NVIC_IntDisable(NVIC_CT32B0);

  SYSAHBCLKCTRL_bit.CT32B0 = 0; // disable clock
}

/*************************************************************************
 * Function Name: ADC_INIT
 * Parameters: none
 *
 * Return: none
 *
 * Description: ADC Initialzation.
 *
 *************************************************************************/
void ADC_Init(void)
{
  /*Enable ADC*/
  SYSAHBCLKCTRL_bit.ADC = 1;
  PDRUNCFG_bit.ADC_PD = 0;
  /*Selcet ADC pin function*/
  ANALOG_TRIM_IOCON = 2;
  /*Set ADC Devider*/
  AD0CR_bit.CLKDIV = ((SYS_GetMainClk()/(SYSAHBCLKDIV)) / 4500000) + 1;

  AD0CR_bit.SEL = 0;
  // Disable all interrupts
  AD0INTEN = 0;
}

static const Int8S _DegreeChar[] =
{
  0x06,
  0x09,
  0x09,
  0x06,
  0x00,
  0x00,
  0x00,
  0x00,
};
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
char buf[9];
  /*WDT disable*/
  WDMOD = 0;
  WDFEED = 0xAA;
  WDFEED = 0x55;
  /*Init clock 33MHz*/
  InitClock(132MHZ, 4);
  /*Enable GPIO Clock*/
  SYSAHBCLKCTRL_bit.GPIO0 = 1;
  /*Set all pins as input ports*/
  GpioInit();
   /*Init LED Ports*/
  LED1_DIR |= LED1_MASK;
  LED2_DIR |= LED2_MASK;
  /*Turn off LED2*/
  LED_OFF(LED2_OUT);
  /*Turn on LED1*/
  LED_ON(LED1_OUT);
  /*UART Init: 115200 bps, 8-bit,
    1-Stop bit, Parity - None*/
  UART_Init(115200);
  /*Enable Timer. Ticks 10 times per second*/
  CT32B0_Init(10);
  /*Init LCD*/
  HD44780_IO_Init();
  if(HD44780_OK != HD44780_PowerUpInit())
  {/*If LCD Init fails*/
    printf("LCD INIT ERROR\n\r");
    while(1)
    {
      if(Ticks)
      {
        Ticks = 0;
        LED1_OUT = ~LED1_PIN;
      }
    }
  }

  HD44780_WrCGRAM(_DegreeChar,1);
  /*Init ADC*/
  ADC_Init();

  printf("\n\r");
  printf("\n\r");
  printf("***************************\n\r");
  printf("*     IAR LPC122x SK      *\n\r");
  printf("* Getting Started example *\n\r");
  printf("***************************\n\r");

  HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)"  IAR   ");

  while(1)
  {
    AD0CR_bit.START = 0;
    /*Select ANALOG_TRIM_CHANNEL*/
    AD0CR_bit.SEL = 1<<ANALOG_TRIM_CHANNEL;
    /*Start AD conversion*/
    AD0CR_bit.START = 1;

    /**/
    while(!Ticks);

    Ticks = 0;

    sprintf(buf,"%d%%",(ANALOG_TRIM_RESULT*100)/(1024-1));
    /*Print result*/
    printf("Trim position %s   \r",buf);

    /*Clear LCD*/
    HD44780_StrShow(1,1,(const HD44780_STRING_DEF *) "        ");
    /*Get string length*/
    int strlength = strlen(buf);
    /*Display temperature on LCD*/
    HD44780_StrShow(1+8-strlength,1,(const HD44780_STRING_DEF *) buf);
    /*Toggle leds*/
    LED1_OUT = ~LED1_PIN;
    LED2_OUT = ~LED2_PIN;
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
