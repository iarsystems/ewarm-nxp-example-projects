/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    DESCRIPTION
 *    ===========
 *      This example project shows how to use the IAR Embedded Workbench for ARM
 *     to develop code for IAR-LPC-11A14-SK board. It shows basic use of I/O,
 *     timer, ADC and interrupt controller.
 *      It shows the Trimer position on the LCD and UART, and lights up the eight
 *     board leds in "running light" mode.
 *      Press the USER button to invert leds state.
 *
 *    COMPATIBILITY
 *    =============
 *
 *      The example project is compatible with, on IAR-LPC-11A14-SK evaluation
 *     board.
 *
 *    CONFIGURATION
 *    =============
 *
 *     Jumpers:
 *   USER_E         - closed
 *   RS_RST/USB_RST - open/open
 *   RS_ISP/USB_ISP - open/open
 *   TRIM_E         - closed
 *   BUZ_E          - open
 *   ACMPO_INT_E    - open
 *   ACC_INT_E      - open
 *   LEDx_E         - all closed
 *   3.3V_CORE_E    - closed
 *   3.3V_IO_E      - closed
 *   RS_RX/USB_RX   - RS_RX
 *   RS_TX/USB_TX   - RS_TX
 *   REG/BAT        - depends on power source
 *
 *     The GettingStarted application is downloaded to the internal Flash.
 *
 *    GETTING STARTED
 *     ===============
 *
 *       1) Build and download the example.
 *
 *       2) Run the program.
 *
 *
 *    History :
 *    1. Date        : September 2011
 *       Author      : Atanas Uzunov
 *       Description : create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US             1000
#include <yfuns.h>

volatile Int32U flags0=0;

#define flTick               (flags0 &   ( 1 << 0 ))
#define flTickSet()          (flags0 |=  ( 1 << 0 ))
#define flTickClear()        (flags0 &= ~( 1 << 0 ))

/** external data **/

/** internal functions **/
static int MyLowLevelGetchar(void);

/** public functions **/
/*************************************************************************
 * Function Name: InitClock
 * Parameters: pllclock - PLL clock, ahbdiv - AHB divider
 * Return: void
 *
 * Description: Initialize PLL to desired clock and AHB divider
 *              Sys clock is Sys PLL output
 *
 *************************************************************************/
void InitClock(Int32U pllclock, Int32U ahbdiv)
{
  /* Enable XTAL Oscilator */
  PDRUNCFG_bit.XTAL_PD = 0;
  /* Enable Internal RC oscilator */
  PDRUNCFG_bit.IRC_PD = 0;
  /* Select internal RC oscilator for SYS clock source */
  MAINCLKUEN = 0;
  MAINCLKSEL = 0;
  MAINCLKUEN = 1;
  /* Configure System PLL */
  /* Power-down System PLL */
  PDRUNCFG_bit.SYSPLL_PD = 1;
  /* Select XTAL Oscilator for SYS PLL input */
  SYSPLLCLKUEN = 0;
  SYSPLLCLKSEL = 1;
  SYSPLLCLKUEN = 1;
  /* Calculate M */
  Int32U m = pllclock/(XTAL_OSC_FREQ*(2*2)) - 1;
  assert(m<32);
  /* Configure PLL frequency */
  SYSPLLCTRL =  (m)      /* MSEL         */
             |  (1<<5);  /* PSEL=1 / P=2 */
  /* Power-up PLL */
  PDRUNCFG_bit.SYSPLL_PD = 0;
  /* Set System AHB Clock divider */
  SYSAHBCLKDIV_bit.DIV = ahbdiv;
  /* Wait until PLL locks */
  while(!(SYSPLLSTAT_bit.LOCK));
  /* Select System PLL Output for SYS clock source */
  MAINCLKUEN = 0;
  MAINCLKSEL = 3;
  MAINCLKUEN = 1;
}

/*************************************************************************
 * Function Name: SYS_GetSourceClk
 * Parameters: none
 * Return: Int32U
 *
 * Description: return Main Clock [Hz]
 *
 *************************************************************************/
Int32U SYS_GetSourceClk(void)
{
Int32U Clk;
  switch(MAINCLKSEL_bit.SEL)
  {
    case 0:
      Clk = I_RC_OSC_FREQ;
    break;
    case 1:
      Clk = XTAL_OSC_FREQ;
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
          Clk = XTAL_OSC_FREQ;
        break;
        case 2:
          Clk = WDT_OSC_FREQ;
        break;
        default:
          Clk = 0;
        break;
      }
    break;
    default:
      Clk = 0;
    break;
  }
  return Clk;
}

/*************************************************************************
 * Function Name: SYS_GetPCLK
 * Parameters: none
 *
 * Return: int
 *
 * Description: Returns Peripheral Clock Frequency in MHz
 *
 *************************************************************************/
Int32U SYS_GetPCLK()
{
   int m = (SYSPLLCTRL & 0x1F) + 1;
   return (m*XTAL_OSC_FREQ)/SYSAHBCLKDIV_bit.DIV;
}

/*************************************************************************
 * Function Name: UART_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Initializes UART module with
 *              baud 115200, no parity, 1 stop bit
 *
 *************************************************************************/
void UART_Init(Int32U BaudRate)
{
Int32U Pclk;
Int32U div;
  /* Disable Clock */
  UARTCLKDIV = 0;
  /* Configure IOs */
  /* Enable I/O Configuration registers */
  SYSAHBCLKCTRL_bit.IOCON = 1;
  /* Uart TX function select */
  IOCON_P0_12_bit.FUNC = 1;
  /* Uart RX function select */
  IOCON_P0_13_bit.FUNC = 1;
  /* Calculate divisor */
  div = 1;
  while((Pclk=SYS_GetPCLK()/div) > 50MHZ)
  {
    div++;
  }
  /* Set divisor */
  UARTCLKDIV = div;
  /* Enable UART clock */
  SYSAHBCLKCTRL_bit.UART = 1;
  /* Select 8-Bits */
  UART_LCR_bit.WLS = 3;
  /* One Stop Bit */
  UART_LCR_bit.SBS = 0;
  /* No parity */
  UART_LCR_bit.PE = 0;
  /* Divisor Latch Access enabled */
  UART_LCR_bit.DLAB = 1;
  /* Set UART BAUDRATE */
  UART_DLL = ((((Pclk+8)/16)+BaudRate/2)/BaudRate)%256;
  UART_DLM = ((((Pclk+8)/16)+BaudRate/2)/BaudRate)/256;
  /* Clear Divisor Latch Access bit */
  UART_LCR_bit.DLAB = 0;
  /* Enable UART0 FIFOs. */
  UART_FCR = (1<<0);
  /* Enable Transmission */
  UART_TER = (1<<7);
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

  for (/*Empty */; Bufsize > 0; --Bufsize)
  {
    while( !UART_LSR_bit.THRE );
    UART_THR = * Buf++;
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
  P0DIR = 0;
  P1DIR = 0;

  /* Reset all GPIO pins to default primary function */
  IOCON_RESET_P0_0 = 0x90;
  IOCON_P0_1 = 0x90;
  IOCON_P0_2 = 0x90;
  IOCON_P0_3 = 0x90;
  IOCON_P0_4 = 0x90;
  IOCON_TCK_P0_5 = 0x90;
  IOCON_TDI_P0_6 = 0x90;
  IOCON_TMS_P0_7 = 0x90;
  IOCON_TDO_P0_8 = 0x90;
  IOCON_TRST_P0_9 = 0x90;
  IOCON_SWDIO_P0_10 = 0x90;
  IOCON_P0_11 = 0x90;
  IOCON_P0_12 = 0x90;  // URXD
  IOCON_P0_13 = 0x90;  // UTXD
  IOCON_P0_14 = 0x90;
  IOCON_P0_15 = 0x90;
  IOCON_P0_16 = 0x90;
  IOCON_P0_17 = 0x90;
  IOCON_P0_18 = 0x90;
  IOCON_P0_19 = 0x90;
  IOCON_P0_20 = 0x90;
  IOCON_P0_21 = 0x90;
  IOCON_P0_22 = 0x90;
  IOCON_P0_23 = 0x90;
  IOCON_P0_24 = 0x90;
  IOCON_P0_25 = 0x90;
  IOCON_P0_26 = 0x90;
  IOCON_P0_27 = 0x90;
  IOCON_P0_28 = 0x90;
  IOCON_P0_29 = 0x90;
  IOCON_P0_30 = 0x90;
  IOCON_P0_31 = 0x90;
  IOCON_P1_0 = 0x90;
  IOCON_P1_1 = 0x90;
  IOCON_P1_2 = 0x90;
  IOCON_P1_3 = 0x90;
  IOCON_P1_4 = 0x90;
  IOCON_P1_5 = 0x90;
  IOCON_P1_6 = 0x90;
  IOCON_P1_7 = 0x90;
  IOCON_P1_8 = 0x90;
  IOCON_P1_9 = 0x90;
}

/*************************************************************************
 * Function Name: TIMER32_B0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer32B0 interrupt handler
 *
 *************************************************************************/
void TIMER32_B0_IRQHandler (void)
{
  flTickSet();
  /* clear interrupt */
  TMR32B0IR_bit.MR0INT = 1;
  NVIC_ClearPendingIRQ(NVIC_CT32B0);
}

/*************************************************************************
 * Function Name: CT32B0_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer32B0 initialization
 *
 *************************************************************************/
void CT32B0_Init(Int32U tps)
{
  /* Enable timer B0 clocks */
  SYSAHBCLKCTRL_bit.CT32B0 = 1;
  /* Init Timer B0 */
  /* Disable counting */
  TMR32B0TCR_bit.CEN = 0;
  /* Set reset */
  TMR32B0TCR_bit.CRST = 1;
  /* Release reset */
  TMR32B0TCR_bit.CRST = 0;
  /* Enable Interrupt on MR0 */
  TMR32B0MCR_bit.MR0I = 1;
  /* Enable reset on MR0 */
  TMR32B0MCR_bit.MR0R = 1;
  /* Disable stop on MR0 */
  TMR32B0MCR_bit.MR0S = 0;
  /* Set timer B0 period */
  TMR32B0PR = 0;
  TMR32B0MR0 = (SYS_GetPCLK()/(SYSAHBCLKDIV))/(tps);
  /* Init timer B0 interrupt */
  /* clear pending interrupt */
  TMR32B0IR_bit.MR0INT = 1;
  /* Enable counting */
  TMR32B0TCR_bit.CEN = 1;
  /* Enable NVIC TMR0 Interrupt */
  NVIC_EnableIRQ(NVIC_CT32B0);
  NVIC_SetPriority(NVIC_CT32B0,16);
}

/*************************************************************************
 * Function Name: CT32B0_Stop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Stops Timer32B0
 *
 *************************************************************************/
void CT32B0_Stop(void)
{
  /* Disable counting */
  TMR32B0TCR_bit.CEN = 0;
  /* Disable interrupt */
  NVIC_DisableIRQ(NVIC_CT32B0);
  /* Disable clock */
  SYSAHBCLKCTRL_bit.CT32B0 = 0;
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
  /* Enable ADC */
  SYSAHBCLKCTRL_bit.ADC = 1;
  PDRUNCFG_bit.ADC_PD = 0;
  /* Select FUNC value 2 (AD7) in IOCON register */
  ANALOG_TRIM_IOCON = 2;
  /* ADC Clock should be less than 4.5MHz,        */
  /* so we calc the CLKDIV value for ~3MHz clock  */
  AD0CR_bit.CLKDIV = ((SYS_GetPCLK()/(SYSAHBCLKDIV)) / 3000000) - 1;
  /* Disable all interrupts */
  AD0INTEN = 0;
  /* Stops any ADC conversions */
  AD0CR_bit.START = 0;
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
char buf[9];
char lcd_buf[9];
Int8U counter=0;

  /* Init clock, PLL = 200MHz, AHB divider = 1 */
  InitClock(200MHZ, 1);
  /* Enable GPIO Clock */
  SYSAHBCLKCTRL_bit.GPIO = 1;
  /* Enable IOCON Clock */
  SYSAHBCLKCTRL_bit.IOCON = 1;
  /* Set all pins as input ports */
  GpioInit();
  /* Init LED Ports */
  LEDS_OFF();
  LEDS_DIR |= LEDS_MASK;
  /* Init LCD */
  HD44780_IO_Init();
  if(HD44780_OK != HD44780_PowerUpInit())
  {/* If LCD Init fails */
    printf("LCD INIT ERROR\n\r");
    while(1)
    {
      if(flTick)
      {
        flTickClear();
        LED_TOGGLE |= LEDS_MASK;
      }
    }
  }
  /* Init ADC */
  ADC_Init();
  /* Init UART: 115200 bps, 8-bit, 1-Stop bit, Parity - None */
  UART_Init(115200);
  /* Enable Timer. Ticks 10 times per second */
  CT32B0_Init(10);

  printf("\n\r");
  printf("\n\r");
  printf("***************************\n\r");
  printf("*     IAR LPC11A14 SK     *\n\r");
  printf("* Getting Started example *\n\r");
  printf("***************************\n\r");
  printf("\n\r");

  /* Main loop */
  while(1)
  if (flTick)
  {
    flTickClear();

    /* Lights-up next LED                     */
    /* if USER button is pressed, invert LEDs */
    LEDS_OFF();
    if(!(USER_PORT & USER_MASK))
    {
      LEDS_ON(~(1 << (counter++%8)));
    }
    else
    {
      LEDS_ON(1 << (counter++%8));
    }

    /* Start measure of ANALOG_TRIM_CHANNEL */
    AD0CR_bit.START = 0;
    AD0CR_bit.SEL = 1<<ANALOG_TRIM_CHANNEL;
    AD0CR_bit.START = 2;
    /* Wait to complete AD conversion */
    while(!AD0DR7_bit.DONE);

    /* Save ADC value to buf as string 0-100% */
    sprintf(buf,"%d%%",(AD0DR7_bit.V_VREF*100)/(1024-1));

    /* Print result - UART */
    printf("\rTrim position %s  ",buf);
    /* Print result - LCD  */
    sprintf(lcd_buf,"ADC:%s",buf);
    HD44780_StrShow(1,1,(const HD44780_STRING_DEF *) "        ");
    HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)lcd_buf);
 }
}

/** private functions **/
static int MyLowLevelGetchar(void)
{
int ch;

  if(UART_LSR_bit.RDR)
  {
    ch = UART_RBR;
    if(UART_LSR_bit.OE || UART_LSR_bit.PE || UART_LSR_bit.FE )
    {
      return (ch | 0x10000000);
    }
    return (ch & 0xff );
  }
  return -1;
}
