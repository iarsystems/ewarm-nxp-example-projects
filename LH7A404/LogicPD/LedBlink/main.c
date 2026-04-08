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
 *    1. Date        : May, 14 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the Logic PD LH7A404 evaluation boards.
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

#include <NXP/iolh7A404.h>
#include "board.h"
#include "arm_comm.h"
#include "arm922t_cp15_drv.h"

//* system timer counter
volatile unsigned int StTick;

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
void (*interrupt_function)();
unsigned int vector;
  vector = VIC_VECTADDR_1;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();    // Call vectored interrupt function
  VIC_VECTADDR_1 = 0;         // Clean interrupt in VIC
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
  GPIO_PEDD |= STAT_LED0 | STAT_LED1;   // PE6, PE7 outputs
  GPIO_LED_OFF();STAT_LED0_OFF();STAT_LED1_OFF();

  // Init VIC controller
  // Assign all interrupt channels to IRQ
  VIC_INTSEL_1 = VIC_INTSEL_2 = 0;
  // Disable all interrupts
  VIC_INTENCLR_1 = VIC_INTENCLR_2 = 0xFFFFFFFF;
  // Clear all software interrupts
  VIC_SOFTINTCLR_1 = VIC_SOFTINTCLR_2 = 0xFFFFFFFF;
  // Clear interrupt
  VIC_VECTADDR_1 = VIC_VECTADDR_2 = 0;
  // Clear address of the Interrupt Service routine (ISR) for non-vectored IRQs.
  VIC_NVADDR_1 = VIC_NVADDR_2 = 0;

  // Clear address of the Interrupt Service routine (ISR) for vectored IRQs.
  VIC_VAD0_1  = 0;
  VIC_VAD1_1  = 0;
  VIC_VAD2_1  = 0;
  VIC_VAD3_1  = 0;
  VIC_VAD4_1  = 0;
  VIC_VAD5_1  = 0;
  VIC_VAD6_1  = 0;
  VIC_VAD7_1  = 0;
  VIC_VAD8_1  = 0;
  VIC_VAD9_1  = 0;
  VIC_VAD10_1 = 0;
  VIC_VAD11_1 = 0;
  VIC_VAD12_1 = 0;
  VIC_VAD13_1 = 0;
  VIC_VAD14_1 = 0;
  VIC_VAD15_1 = 0;
  VIC_VAD0_2  = 0;
  VIC_VAD1_2  = 0;
  VIC_VAD2_2  = 0;
  VIC_VAD3_2  = 0;
  VIC_VAD4_2  = 0;
  VIC_VAD5_2  = 0;
  VIC_VAD6_2  = 0;
  VIC_VAD7_2  = 0;
  VIC_VAD8_2  = 0;
  VIC_VAD9_2  = 0;
  VIC_VAD10_2 = 0;
  VIC_VAD11_2 = 0;
  VIC_VAD12_2 = 0;
  VIC_VAD13_2 = 0;
  VIC_VAD14_2 = 0;
  VIC_VAD15_2 = 0;

  // Disable all vectored IRQ slots
  VIC_VECTCNTL0_1  = 0;
  VIC_VECTCNTL1_1  = 0;
  VIC_VECTCNTL2_1  = 0;
  VIC_VECTCNTL3_1  = 0;
  VIC_VECTCNTL4_1  = 0;
  VIC_VECTCNTL5_1  = 0;
  VIC_VECTCNTL6_1  = 0;
  VIC_VECTCNTL7_1  = 0;
  VIC_VECTCNTL8_1  = 0;
  VIC_VECTCNTL9_1  = 0;
  VIC_VECTCNTL10_1 = 0;
  VIC_VECTCNTL11_1 = 0;
  VIC_VECTCNTL12_1 = 0;
  VIC_VECTCNTL13_1 = 0;
  VIC_VECTCNTL14_1 = 0;
  VIC_VECTCNTL15_1 = 0;
  VIC_VECTCNTL0_2  = 0;
  VIC_VECTCNTL1_2  = 0;
  VIC_VECTCNTL2_2  = 0;
  VIC_VECTCNTL3_2  = 0;
  VIC_VECTCNTL4_2  = 0;
  VIC_VECTCNTL5_2  = 0;
  VIC_VECTCNTL6_2  = 0;
  VIC_VECTCNTL7_2  = 0;
  VIC_VECTCNTL8_2  = 0;
  VIC_VECTCNTL9_2  = 0;
  VIC_VECTCNTL10_2 = 0;
  VIC_VECTCNTL11_2 = 0;
  VIC_VECTCNTL12_2 = 0;
  VIC_VECTCNTL13_2 = 0;
  VIC_VECTCNTL14_2 = 0;
  VIC_VECTCNTL15_2 = 0;

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
  VIC_VAD0_1 = (unsigned long)Timer_handler;
  // Timer interrupt line
  VIC_VECTCNTL0_1_bit.INTSOURCE = VIC1_T1UI;
  VIC_VECTCNTL0_1_bit.EN = 1;     // Enable slot
  // Enable timer 0 interrupt
  VIC_INTEN_1_bit.T1UI = 1;

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

