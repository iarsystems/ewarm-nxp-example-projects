/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : 13, September 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *   This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for the IAR LPC-2468 board. It shows basic use of parallel
 *  I/O, timer, interrupt controller, ADC and interface to a LCD HD44780 compatible
 *  module.
 *   It show the position of the potentiometer (AIN0) in percents.
 *
 * Jumpers:
 *  J5      - depending of power source
 *  ISP     - unfilled
 *  nRESET  - unfilled
 *  EINT0   - filled
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 * The LEDs share pins with Trace port. If ETM is enabled the LEDs can not be used.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define NONPROT 0xFFFFFFFF
#define CRP1  0x12345678
#define CRP2  0x87654321
/*If CRP3 is selected, no future factory testing can be performed on the device*/
#define CRP3  0x43218765

#ifndef __RAM_DEBUG
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = NONPROT;
#endif

#define TIMER0_TICK_PER_SEC   20
#define DLY_100US             300

volatile Boolean Update = TRUE;

/*************************************************************************
 * Function Name: Timer0IntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void Timer0IntrHandler (void)
{
  // clear interrupt
  T0IR_bit.MR0INT = 1;
  Update = TRUE;
  VICADDRESS = 0;
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
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
char Str[17];
Int32U Pos;
Int32U AdcData;

  // MAM init
  MAMCR_bit.MODECTRL = 0;
  MAMTIM_bit.CYCLES  = 3;   // FCLK > 40 MHz
  MAMCR_bit.MODECTRL = 2;   // MAM functions fully enabled
  // Init clock
  InitClock();
  // Init GPIO
  GpioInit();
  // Init VIC
  VIC_Init();

  // ADC Init
  // Assign P0.23 to AIN0
  ANALOG_TRIM_CHANNEL_SEL = 1;
  PCONP_bit.PCAD = 1; // Enable ADC clk
  // Set ADC clk <4.5 MHz
  AD0CR_bit.CLKDIV = (SYS_GetFpclk(ADC_PCLK_OFFSET) / 45000000) + 1;
  // Select AIN0
  AD0CR_bit.SEL = 1<<ANALOG_TRIM_CHANNEL;
  // Disable all interrupts
  ADINTEN = 0;
  // Enable ADC
  AD0CR_bit.PDN = 1;
  // Start conversion
  AD0CR_bit.START = 1;

  // Init Time0
  PCONP_bit.PCTIM0 = 1; // Enable TMR0 clk
  T0TCR_bit.CE = 0;     // counting  disable
  T0TCR_bit.CR = 1;     // set reset
  T0TCR_bit.CR = 0;     // release reset
  T0CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T0MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T0MCR_bit.MR0R = 1;   // Enable reset on MR0
  T0MCR_bit.MR0S = 0;   // Disable stop on MR0
  // set timer 0 period
  T0PR = 0;
  T0MR0 = SYS_GetFpclk(TIMER0_PCLK_OFFSET)/(TIMER0_TICK_PER_SEC);
  // init timer 0 interrupt
  T0IR_bit.MR0INT = 1;  // clear pending interrupt
  VIC_SetVectoredIRQ(Timer0IntrHandler,0,VIC_TIMER0);
  VICINTENABLE |= 1UL << VIC_TIMER0;
  T0TCR_bit.CE = 1;     // counting Enable

  __enable_interrupt();

  HD44780_PowerUpInit();

  // Show message on the LCD
  HD44780_StrShow(1, 1,  "  IAR Systems   ");
  HD44780_StrShow(1, 2,  "    LPC2468     ");
  LCD_LIGHT_ON();
  DelayResolution100us(30000);
  while(1)
  {
    if(Update)
    {
      Update = FALSE;
      AdcData = AD0GDR_bit.RESULT;
      AD0CR_bit.START = 1;
      // Calculate position of the potentiometer in percents
      Pos = (AdcData*100)/1024;
      sprintf(Str,"AN_TR:%3d%%      ",Pos);
      HD44780_StrShow(1, 2,(pInt8S)Str);
    }
  }
}
