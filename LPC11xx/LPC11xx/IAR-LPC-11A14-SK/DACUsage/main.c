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
 *     timer, DAC and interrupt controller.
 *      It produces ~13.7 Hz sine wave at AOUT processor pin - Analog Extensions
 *     Port (Pin 1 @ CON1). When the USER button is pressed, it produces square
 *     wave.
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
 *     The DACUsage application is downloaded to the internal Flash.
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

#define PI 3.14159265
#define STEP 5

volatile Int32U flags0=0;

#define flTick               (flags0 &   ( 1 << 0 ))
#define flTickSet()          (flags0 |=  ( 1 << 0 ))
#define flTickClear()        (flags0 &= ~( 1 << 0 ))

/** external data **/

/** internal functions **/

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
 * Function Name: DAC_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: DAC Initialzation.
 *
 *************************************************************************/
void DAC_Init(void)
{
  /* Enable DAC */
  SYSAHBCLKCTRL_bit.DAC = 1;
  PDRUNCFG_bit.DAC_PD = 0;
  /* Select FUNC value 2 (DAC) in IOCON register */
  DAC_IOCON = 2;
  /* Select BIAS value 0 - 1uS settling time, 1MHz max update rate  */
  DACR_bit.BIAS = 0;
  /* Triggers on register write */
  DACR_bit.TRIG = 0;
  /* Set 0V at output */
  DACR_bit.VALUE = 0;
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
float angle;
Int32U value;

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
  HD44780_StrShow(1,1,(const HD44780_STRING_DEF *) "DAC Demo");
  /* Init DAC */
  DAC_Init();
  /* Enable Timer. Ticks 1000 times per second */
  CT32B0_Init(1000);

  angle = 0;
  /* Main loop */
  while(1)
  if (flTick)
  {
    flTickClear();
    /* Calculate current sample of the sine wave */
    value = (Int32U)(512 + sin(angle*PI/180)*511);
    /* If button is pressed */
    if(!(USER_PORT & USER_MASK))
    {
      /* Convert sample to produce square wave */
      if (value > 511) value = 1023;
      else value = 0;
    }
    /* Write calculated sample value to DAC register */
    DACR_bit.VALUE = value;
    /* Prepare next sample */
    angle += STEP;
    if (angle > 360) angle = 0;

 }
}
