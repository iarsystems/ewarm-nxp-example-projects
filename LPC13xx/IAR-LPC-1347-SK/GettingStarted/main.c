/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : main.c
 *    DESCRIPTION
 *    ===========
 *      This example project shows how to use the IAR Embedded Workbench for ARM
 *     to develop code for IAR-LPC1347-SK board. It shows basic use of I/O,
 *     timer, ADC and interrupt controllers.
 *     It shows the Temperature and the Trimer position no LCD and UART
 *     Press B1 to see the temperature on the LCD.
 *     Press B2 to see the trimer position on the LCD
 *
 *    COMPATIBILITY
 *    =============
 *
 *      The example project is compatible with, on IAR-LPC1347-SK evaluation
 *       board. By default, the project is configured to use the J-Link SWD
 *     interface.
 *
 *    CONFIGURATION
 *    =============
 *
 *     Jumpers:
 *     RST_E   - unfilled
 *     ISP_E   - unfilled
 *     TMS/SWDIO - SWDIO (2-3)
 *     TDO/SWV   - SWV (2-3)
 *
 *     The GettingStarted application is downloaded to the iFlash.
 *
 *    GETTING STARTED
 *     ===============
 *
 *   1) Build and download the program.
 *
 *
 *    1. Date        : September 7, 2009
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 *    2. Date        : January 30, 2012
 *       Author      : Atanas Uzunov
 *       Description : Ported to IAR-LPC1347-SK evaluation board.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"
#include <yfuns.h>

#define DLY_100US             550

volatile uint32_t Ticks;

/** external data **/

/** internal functions **/
static int MyLowLevelGetchar(void);
/** public functions **/
/*************************************************************************
 * Function Name: InitClock
 * Parameters:
 * Return: void
 *
 * Description: Initialize PLL to 72MHz
 *              Sys clock is Sys PLL output and AHB divider is 1
 *              System runs at 72MHz
 *
 *************************************************************************/
void InitClock(void)
{
  /* Enable Internal RC oscillator */
  CLK_SetIRCOsc(CLK_ENABLE);
  /* Select internal RC oscillator for
     Sys clock source */
  CLK_SetMainClk(MAINSEL_IRCOSC, 1);
  /* Sys Oscillator Enable, bypass disabled, set range */
  CLK_MainOscSet(CLK_ENABLE, CLK_DISABLE, (MOSC >(20MHZ))?(MOSCRNG_15_25MHZ):(MOSCRNG_1_20MHZ));
  /* Select Sys Oscillator for
     SYS PLL source */
  CLK_SetPLLInClk(PLLINSEL_MOSC);
  /* Enable set PLL 72Hz (M = 6; P = 2) */
  CLK_SetPll(CLK_ENABLE, (72MHZ/MOSC)-1, 1);
  /* Select Sys PLL Output for
     Main clock source. SYSAHB clock divider 1:1. CPU runs at 72MHz */
  CLK_SetMainClk(MAINSEL_CLKPLL,1);
}

/*************************************************************************
 * Function Name: UART_Init
 * Parameters: uint32_t BaudRate
 * Return: void
 *
 * Description: Initialize Uart Module
 *
 *************************************************************************/
void UART_Init (uint32_t BaudRate)
{
uint32_t Pclk;

  /* Enable I/O Configuration registers */
  SYSAHBCLKCTRL_bit.IOCON = 1;
  UART_RX_IOCON = UART_RX_IOCON_INIT;  /* Uart RX function select */
  UART_TX_IOCON = UART_TX_IOCON_INIT;  /* Uart RX function select */
  /* Enable Clocks */
  UARTCLKDIV = 1;
  /**/
  while((Pclk=CLK_GetClock(CLK_MAIN)/UARTCLKDIV)> 70MHZ)
  {
    UARTCLKDIV ++;
  }
  /* Enable UART register clock */
  SYSAHBCLKCTRL_bit.USART = 1;

  U0LCR_bit.WLS = 3;                  /* Select 8-Bits */
  U0LCR_bit.SBS = 0;                  /* One Stop Bit */
  U0LCR_bit.PE = 0;                   /* No parity */

  U0LCR_bit.DLAB = 1;                 /* Divisor Latch Access enabled */
  /* Set UART0 BAUDRATE */
  U0DLL = ((((Pclk+8)/16)+BaudRate/2)/BaudRate)%256;
  U0DLM = ((((Pclk+8)/16)+BaudRate/2)/BaudRate)/256;
  U0LCR_bit.DLAB = 0;                 /* Clear Divisor Latch Access bit*/

  U0FCR = (1<<0);                     /* Enable UART0 FIFOs */
  U0TER = (1<<7);                     /* Enable Transmission */
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
 * Parameters: uint32_t Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
 *
 *************************************************************************/
void DelayResolution100us(uint32_t Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile uint32_t j = DLY_100US; j; j--)
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
  /* Enable TIM0 clocks */
  SYSAHBCLKCTRL_bit.CT32B0 = 1;

  /* Init Timer0 */
  TMR32B0TCR_bit.CE = 0;     /* counting  disable */
  TMR32B0TCR_bit.CR = 1;     /* set reset */
  TMR32B0TCR_bit.CR = 0;     /* release reset */
  TMR32B0CTCR_bit.CTM = 0;   /* Timer Mode: every rising PCLK edge */
  TMR32B0MCR_bit.MR0I = 1;   /* Enable Interrupt on MR0 */
  TMR32B0MCR_bit.MR0R = 1;   /* Enable reset on MR0 */
  TMR32B0MCR_bit.MR0S = 0;   /* Disable stop on MR0 */
  /* Set Timer0 period */
  TMR32B0PR = 0;
  TMR32B0MR0 = (CLK_GetClock(CLK_SYS))/(tps);
  /* Init Timer0 interrupt */
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
  /* Enable ADC */
  SYSAHBCLKCTRL_bit.ADC = 1;
  PDRUNCFG_bit.ADC_PD = 0;
  /* Select ADC pin function */
  ANALOG_TRIM_IOCON = 1;
  TEMP_PIN_IOCON = 1;
  /* Set ADC Divider */
  AD0CR_bit.CLKDIV = (CLK_GetClock(CLK_SYS)/ 4500000) + 1;

  AD0CR_bit.SEL = 0;
  /* Disable all interrupts */
  AD0INTEN = 0;
  /* 12 bit mode ADC */
  AD0CR_bit.MODE10BIT = 0;

  AD0CR_bit.START = 0;
  /* Select ANALOG_TRIM_CHANNEL */
  AD0CR_bit.SEL = 1<<ANALOG_TRIM_CHANNEL;
  /* Start ADC */
  AD0CR_bit.START = 1;
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
char buf[9];
int32_t cpuclk;
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
  /* Turn off LED2 */
  LED_OFF(LED2);
  /* Turn on LED1 */
  LED_ON(LED1);
  /* UART Init: 115200 bps, 8-bit,
     1-Stop bit, Parity - None */
  UART_Init(115200);
  /* Enable Timer. Ticks 10 times per second */
  CT32B0_Init(10);
  /* Init LCD */
  HD44780_IO_Init();
  if(HD44780_OK != HD44780_PowerUpInit())
  {/* If LCD Init fails */
    printf("LCD INIT ERROR\n\r");
    while(1)
    {
      if(Ticks)
      {
        Ticks = 0;
        LED1 = ~LED1;
      }
    }
  }

  HD44780_WrCGRAM(_DegreeChar,1);
  /* Init ADC */
  ADC_Init();

  printf("\n\r");
  printf("\n\r");
  printf("***************************\n\r");
  printf("*     IAR LPC1347 SK      *\n\r");
  printf("* Getting Started example *\n\r");
  printf("***************************\n\r");

  printf("\n\rTrim position 0%% : ");

  while(1)
  {
    if(!BUT1)
    {/* If Button 1 is pressed */
      LED_ON(LED1);
      LED_OFF(LED2);
    }
    else
    {
      if(!BUT2)
      {/* If Button 2 is pressed */
        LED_OFF(LED1);
        LED_ON(LED2);
      }
    }

    AD0CR_bit.START = 0;
    /* Select ANALOG_TRIM_CHANNEL */
    AD0CR_bit.SEL = 1<<ANALOG_TRIM_CHANNEL;

    AD0CR_bit.START = 1;

    sprintf(buf,"%.1f\260C",
             TempConverter(NtcRatioToTemperature((float)AD0DR7_bit.V_VREF/(4096-1)),KELVIN,CELSIUS));
    /* Print result */
    printf("Temp %s       \r",buf);

    if(!LED1)
    {
      /* Clear LCD */
      HD44780_StrShow(1,1,(const HD44780_STRING_DEF *) "        ");
      /* Get string length */
      int strlength = strlen(buf);
      /* Correct degree symbol */
      buf[strlength-2] = '\1';
      /* Display temperature on LCD */
      HD44780_StrShow(1+8-strlength,1,(const HD44780_STRING_DEF *)buf);
    }

    while(!Ticks);

    Ticks = 0;

    AD0CR_bit.START = 0;
    /* Select AIN5 */
    AD0CR_bit.SEL = 1<<TEMP_CHANNEL;

    AD0CR_bit.START = 1;

    sprintf(buf,"%3d%%",(AD0DR6_bit.V_VREF*100)/(4096-1));
    /* Print result */
    printf("Trim position %s : ",buf);

    if(!LED2)
    {
      /* Clear LCD */
      HD44780_StrShow(1,1,(const HD44780_STRING_DEF *) "        ");
      /* Get string length */
      int strlength = strlen(buf);
     /* Display temperature on LCD */
      HD44780_StrShow(1+8-strlength,1,(const HD44780_STRING_DEF *) buf);
    }

    while(!Ticks);

    Ticks = 0;
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
