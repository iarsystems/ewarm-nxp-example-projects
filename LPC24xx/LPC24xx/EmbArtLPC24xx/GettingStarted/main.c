/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 25, October 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Embedded Artists LPC24xx board. It shows basic use of
 * the I/O, the timer and the interrupt controllers.
 *  It starts by blinking LCD backlight.
 *
 * Jumpers: according board's manual
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <intrinsics.h>
#include <stdio.h>
#include <nxp/iolpc2468.h>

#include "board.h"
#include "sys.h"

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

#define TIMER0_TICK_PER_SEC   10

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
  // Toggle USB Link LED
  LCD_BLT_FIO ^= LCD_BLT_MASK;
  // clear interrupt
  T0IR_bit.MR0INT = 1;
  VICADDRESS = 0;
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
  // Init clock
  InitClock();
  // Init GPIO
  GpioInit();
  // Init VIC
  VIC_Init();

  // Init USB Link  LED
  LCD_BLT_FDIR = LCD_BLT_MASK;
  LCD_BLT_FSET = LCD_BLT_MASK;

  // Enable TIM0 clocks
  PCONP_bit.PCTIM0 = 1; // enable clock

  // Init Time0
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
  while(1)
  {
  }
}
