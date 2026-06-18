/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : 6, November 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the NOHAU LPC2800 evaluation board. It shows
 * basic use of I/O, timer and the interrupt controllers.
 *  It starts by blinking D9.
 *  The jumpers settings are regarding  Nohau LPC2800 user manual
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <intrinsics.h>
#include <stdio.h>
#include <NXP/iolpc2888.h>

#include "lpc288x_low_init.h"

#if defined(XFLASH_BOOT)
#define TIMER0_TICK_PER_SEC   20
#elif defined(IFLASH_BOOT)
#define TIMER0_TICK_PER_SEC   10
#else
#define TIMER0_TICK_PER_SEC   5
#endif

#define TIMER0_IN_FREQ        12000000

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
  T0CLR = 0;              // clear timer interrupt
  MODE0_2_bit.P2_1 ^= 1;  // toggle P2.1
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
  // Init led D9 pin (P2.1)
  MODE1_2_bit.P2_1 = 1;
  MODE0_2_bit.P2_1 = 1;
  // Init Time0
  T0CTRL_bit.TENAB = 0;     // disable counting
  T0CTRL_bit.TMODE = 1;     // enable auto reload from T0LOAD register
  T0CTRL_bit.PRESCALE = 2;  // prescaler = CGU clock rate / 256
  // set timer 0 period
  T0LOAD = (TIMER0_IN_FREQ)/(256 * TIMER0_TICK_PER_SEC);
  T0CLR = 0;                // clear timer pending interrupt
  // init timer 0 interrupt
  IrqTable[VIC_TIMER0].pIntrFunc    = Timer0IntrHandler;
  IrqTable[VIC_TIMER0].IntrPriority = 1;
  INT_REQ5 = (1UL << 29) |  // clear software interrupt
             (1UL << 28) |  // Update Priority
             (1UL << 27) |  // Update target
             (1UL << 26) |  // Update interrupt enable
             (1UL << 25) |  // Update interrupt active level
             (1UL << 16) |  // INTEN
             1;             // PRIO
  T0CTRL_bit.TENAB = 1;     // enable timer 0 counting
  __enable_interrupt();
  while(1)
  {
  }
}
