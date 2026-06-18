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
 *     timer, UART, I2C, on-board accelerometer sensor, and interrupt controller.
 *      It shows small pseudo-graphic square at the text LCD display. When the board
 *     is tilted around X and Y axes, the square moves. Accelerometer info is
 *     printed to UART at application startup.
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
 *   USER_E         - open
 *   RS_RST/USB_RST - open/open
 *   RS_ISP/USB_ISP - open/open
 *   TRIM_E         - open
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
 *     The Accelerometer application is downloaded to the internal Flash.
 *
 *    GETTING STARTED
 *     ===============
 *
 *    1) Build and download the example.
 *
 *    2) Run the program.
 *
 *
 *    History :
 *    1. Date        : September, 2011
 *       Author      : Atanas Uzunov
 *       Description : Create
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
float X = 20;
float Y =  3;
SMB380_Data_t XYZT;
float accx,accy;
unsigned char Smb380Id, Smb380Ver, delay;

  /* Init clock, PLL = 200MHz, AHB divider = 1 */
  InitClock(200MHZ, 1);
  /* Enable GPIO Clock */
  SYSAHBCLKCTRL_bit.GPIO = 1;
  /* Enable IOCON Clock */
  SYSAHBCLKCTRL_bit.IOCON = 1;
  /* Set all pins as input ports */
  GpioInit();
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
  /* Init UART: 115200 bps, 8-bit, 1-Stop bit, Parity - None */
  UART_Init(115200);
  /* Enable Timer. Ticks 20 times per second */
  CT32B0_Init(20);
  /* Init acceleration sensor*/
  SMB380_Init();

  /* Shows IAR text for 1s */
  HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)"  IAR   ");
  delay = 20;
  while(delay) if (flTick) { flTickClear(); delay--; }

  /* Print some info to UART terminal */
  printf("\n\r");
  printf("\n\r");
  printf("***************************\n\r");
  printf("*     IAR LPC11A14 SK     *\n\r");
  printf("*  Accelerometer example  *\n\r");
  printf("***************************\n\r");
  printf("\n\r");

  SMB380_GetID((unsigned char *)&Smb380Id, (unsigned char *)&Smb380Ver);
  printf("Acceleration sensor ID:%d, Ver:%d\n\r",Smb380Id,Smb380Ver);
  printf("\n\r");

  printf("Tilt the board around X and Y axes,\n\r");
  printf("to move the square on the board\n\r");
  printf("LCD screen. Enjoy! ;-)\n\r");
  printf("\n\r");

  /* Main loop */
  while(1)
  if (flTick)
  {
    flTickClear();
    /* Get data from accelerometer */
    SMB380_GetData(&XYZT);

    /* Check if X data is new */
    if (XYZT.AccX & 0x00001)
    {
      /* Update X acceleration */
      accx = ((Int16S)(XYZT.AccX >> 6));
      accx /= 70;
     }
    /* Check if Y data is new */
     if (XYZT.AccY & 0x00001)
     {
       /* Update Y acceleration */
       accy = ((Int16S)(XYZT.AccY >> 6));
       accy /= 70;
     }
     /* Update image position */
     X += accy;
     Y += accx;
     /* Check screen boundaries */
     if (X < 1) X = 1;
     if (X > ((CHAR_COUNT*CHAR_WIDTH) - IMAGE_WIDTH)) X = ((CHAR_COUNT*CHAR_WIDTH) - IMAGE_WIDTH);
     if (Y < 0) Y = 0;
     if (Y > (CHAR_HEIGHT - IMAGE_HEIGHT)) Y = (CHAR_HEIGHT - IMAGE_HEIGHT);
     /* Draw image on the screen */
     HD44780_DrawXY((Int16U)X,(Int16U)Y);
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
