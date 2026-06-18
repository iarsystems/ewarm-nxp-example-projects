/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 19, November 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Keil MCB2300 board. It shows basic use of the I/O,
 * the timer and the interrupt controllers.
 *  Displays running lights.
 *
 * Jumpers:
 *  VBUS - filled
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
#include <nxp/iolpc1766.h>

#include "board.h"

#pragma section=".intvec"
#define FCCO_FREQ               (300UL MHZ)
#define CCKL_DIV                (3UL)
#define FCCLK_FREQ              (FCCO_FREQ/CCKL_DIV)
#define FPCLK_FREQ              (FCCLK_FREQ/4)

#define SYS_TIMER_TICK_PER_SEC   10
#define SYS_TIMER_IN_FREQ        FCCLK_FREQ

volatile Int8U LedsState;

/*************************************************************************
 * Function Name: SysTick_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void SysTick_Handler(void)
{
  volatile Int32U Dummy = SYSTICKCSR; //clear interrupt flag
  // Leds Off
  LEDS_CLR = LEDS_MASK;
  // On next LED
  LEDS_SET = LedsState;
  if(0 == (LedsState <<= 1))
  {
    LedsState = 1;
  }
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
 * Description: Initialize PLL and clocks' dividers. Hclk - 300MHz,
 *              Fcckl = 100MHz
 *
 *************************************************************************/
void InitClock(void)
{
  // 1. Init OSC
  SCS_bit.OSCRANGE = 0;
  SCS_bit.OSCEN = 1;
  // 2.  Wait for OSC ready
  while(!SCS_bit.OSCSTAT);
  // 3. Disconnect PLL
  PLL0CON_bit.PLLC = 0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 4. Disable PLL
  PLL0CON_bit.PLLE = 0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 5. Select source clock for PLL
  CLKSRCSEL_bit.CLKSRC = 1;   // Selects the main oscillator as a PLL clock source.
  PCLKSEL0 = PCLKSEL1  = 0;   // other peripherals 100/4 = 25MHz
  // 6. Set PLL settings 300 MHz
  PLL0CFG_bit.MSEL = 25-1;
  PLL0CFG_bit.NSEL = 2-1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 7. Enable PLL
  PLL0CON_bit.PLLE = 1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 8. Wait for the PLL to achieve lock
  while(!PLL0STAT_bit.PLOCK);
  // 9. Set clk divider settings
  CCLKCFG   = 3-1;            // 1/3 Fpll
  // 10. Connect the PLL
  PLL0CON_bit.PLLC = 1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
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
  // Set to inputs
  FIO0DIR = 0;
  FIO1DIR = 0;
  FIO2DIR = 0;
  FIO3DIR = 0;
  FIO4DIR = 0;

  // clear mask registers
  FIO0MASK = 0;
  FIO1MASK = 0;
  FIO2MASK = 0;
  FIO3MASK = 0;
  FIO4MASK = 0;

  // Reset all GPIO pins to default primary function
  PINSEL0 = 0;
  PINSEL1 = 0;
  PINSEL2 = 0;
  PINSEL3 = 0;
  PINSEL4 = 0;
  PINSEL7 = 0;
  PINSEL8 = 0;
  PINSEL9 = 0;
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
  // Flash accelerator init
  FLASHCFG = (0x5UL<<12) | 0x3AUL;
  // Init clock
  InitClock();
#if FCCLK_FREQ < 20000000
  FLASHCFG = (0x0UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 40000000
  FLASHCFG = (0x1UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 60000000
  FLASHCFG = (0x2UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 80000000
  FLASHCFG = (0x3UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 100000000
  FLASHCFG = (0x4UL<<12) | 0x3AUL;
#endif

  // Init GPIO
  GpioInit();

  // Init LEDs
  LEDS_DIR |= LEDS_MASK;
  LEDS_CLR  = LEDS_MASK;

  // Init System Timer
  SYSTICKCSR_bit.ENABLE    = 0; // stop counting
  SYSTICKCSR_bit.CLKSOURCE = 1; // select core clock = 100MHz
  SYSTICKCSR_bit.TICKINT   = 1; // enable system tick timer interrupt
  SYSTICKCVR               = 0; // reset counter
  // Set timer period
  SYSTICKRVR_bit.RELOAD = (SYS_TIMER_IN_FREQ/SYS_TIMER_TICK_PER_SEC) - 1;
  SYSTICKCSR_bit.ENABLE    = 1; // enable counting

  LedsState = 1;

  __enable_interrupt();
  while(1)
  {
  }
}
