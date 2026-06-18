/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : May, 17 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the Logic PD LH7A400 evaluation boards.
 * It shows basic use of MMU, parallel I/O, timer and the interrupt
 * controller.
 *
 *  It starts by showing different patterns on the LED's separated by
 * half second.
 *
 *    $Revision: 34 $
 **************************************************************************/

#include <intrinsics.h>
#include <stdio.h>
#include <string.h>

#include <NXP/iolh7A400.h>
#include "board.h"
#include "arm_comm.h"

// system timer counter
volatile unsigned int StTick;

VoidFpnt_t IntrHanders[32];

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ Handler
 *
 *************************************************************************/
__irq __arm void irq_handler(void)
{
unsigned long IntFlag = IC_INTSR, Mask = 1, Count = 0;
  while(IntFlag)
  {
    if(IntFlag & Mask)
    {
      IntFlag &= ~Mask;
      IntrHanders[Count]();
    }
    Mask <<= 1;
    ++Count;
  }
}

/*************************************************************************
 * Function Name: Timer_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer interrupt Handler
 *
 *************************************************************************/
void Timer_handler(void)
{
  // Clean interrupt
  TIMER_TCEOI1 = 0;  // Clear interrupt flag
  ++StTick;
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: Main subroutine
 *
 *************************************************************************/
int main(void)
{
  StTick = 0;

  // Init IO ports
  GPIO_LED_OFF();STAT_LED0_OFF();STAT_LED1_OFF();

  // Init VIC controller
  IC_INTENC = 0xFF;  // Diasable all interrupts
  for(int i = 0; i < sizeof(IntrHanders)/sizeof(VoidFpnt_t); ++i)
  {
    IntrHanders[i] = NULL;
  }

  // Init System timer 1ms
  TIMER_CONTROL1_bit.ENABLE = 0;  // Stop Timer 3 Count
  TIMER_CONTROL1_bit.CLKSEL = 1;  // Use Fast clock
  TIMER_CONTROL1_bit.MODE   = 1;  // Periodic
  // Set period
  TIMER_LOAD1_bit.LOAD      = TIMR12_FAST_CLK/(TICK_PER_SECOND);
  TIMER_TCEOI1 = 0;               // Clear interrupt flag
  TIMER_CONTROL1_bit.ENABLE = 1;  // Enable Timer 3 Count
  // registered the interrupt handler of Timer
  // Set address of the handler
  IntrHanders[IC_TC1OI] = Timer_handler;
  // Enable timer 1 interrupt
  IC_INTENS_bit.TC1OI = 1;

  // Enable interrupts.
  __enable_interrupt();

  while(1)
  {
    // ~ Tick 0.5 sec BLINK CPLD GPIO LED0
    if((StTick & 0x100) == 0)
    {
      GPIO_LED_OFF();
    }
    else
    {
      GPIO_LED_ON();
    }
    // ~ Tick 1 sec BLINK Status LED0
    if((StTick & 0x200) == 0)
    {
      STAT_LED0_OFF();
    }
    else
    {
      STAT_LED0_ON();
    }
    // ~ Tick 2 sec BLINK Status LED1
    if((StTick & 0x400) == 0)
    {
      STAT_LED1_OFF();
    }
    else
    {
      STAT_LED1_ON();
    }
  }	
}

