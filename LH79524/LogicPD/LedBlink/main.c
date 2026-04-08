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
 *    1. Date        : May, 10 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the Logic PD LH79524 evaluation boards.
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

#include <NXP/iolh79524.h>
#include "board.h"
#include "arm_comm.h"

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
  vector = VIC_VectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();  // Call vectored interrupt function
  VIC_VectAddr = 0;         // Clean interrupt in VIC
}

/*************************************************************************
 * Function Name: Timer0_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt Handler
 *
 *************************************************************************/
void Timer0_handler(void)
{
  // Clean interrupt
  TIMER_STATUS0_bit.CMP1_ST = 1;
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
  CPLD_GPIO_DIR = 0;          // CPLD GPOI1,2 outputs
  GPIO_LED_OFF();STAT_LED0_OFF();STAT_LED1_OFF();

  // Init VIC controller
  // Assign all interrupt channels to IRQ
  VIC_IntSelect  =  0;
  // Disable all interrupts
  VIC_IntEnClear = 0xFFFFFFFF;
  // Clear all software interrupts
  VIC_SoftIntClear = 0xFFFFFFFF;
  // Clear interrupt
  VIC_VectAddr = 0;
  // Clear address of the Interrupt Service routine (ISR) for non-vectored IRQs.
  VIC_DefVectAddr = 0;

  // Clear address of the Interrupt Service routine (ISR) for vectored IRQs.
  VIC_VectAddr0  = 0;
  VIC_VectAddr1  = 0;
  VIC_VectAddr2  = 0;
  VIC_VectAddr3  = 0;
  VIC_VectAddr4  = 0;
  VIC_VectAddr5  = 0;
  VIC_VectAddr6  = 0;
  VIC_VectAddr7  = 0;
  VIC_VectAddr8  = 0;
  VIC_VectAddr9  = 0;
  VIC_VectAddr10 = 0;
  VIC_VectAddr11 = 0;
  VIC_VectAddr12 = 0;
  VIC_VectAddr13 = 0;
  VIC_VectAddr14 = 0;
  VIC_VectAddr15 = 0;

  // Disable all vectored IRQ slots
  VIC_VectCtrl0  = 0;
  VIC_VectCtrl1  = 0;
  VIC_VectCtrl2  = 0;
  VIC_VectCtrl3  = 0;
  VIC_VectCtrl4  = 0;
  VIC_VectCtrl5  = 0;
  VIC_VectCtrl6  = 0;
  VIC_VectCtrl7  = 0;
  VIC_VectCtrl8  = 0;
  VIC_VectCtrl9  = 0;
  VIC_VectCtrl10 = 0;
  VIC_VectCtrl11 = 0;
  VIC_VectCtrl12 = 0;
  VIC_VectCtrl13 = 0;
  VIC_VectCtrl14 = 0;
  VIC_VectCtrl15 = 0;

  // Init System timer 1ms
  TIMER_CTRL0_bit.CS        = 0; // Stops counter
  TIMER_CTRL0_bit.CCL       = 1; // Counter Clear
  TIMER_CTRL0_bit.SEL       = 6; // HCLK/128
  // Clears counter when CNT0 for Timer 0 and T0CMP1 for Timer 0 match
  CMP_CAP_CTRL_bit.TC       = 1;
  // Set Timer 0 period
  T0CMP1 = HCLK_FREQ/(128*TICK_PER_SECOND);
  TIMER_STATUS0_bit.CMP1_ST = 1; // Clear pending interrupt
  INTEN0_bit.CMP1_EN        = 1; // Enable interrupt request for compare 1
  TIMER_CTRL0_bit.CS        = 1; // Enable counter
  // registered the interrupt handler of Timer1
  // Set address of the handler
  VIC_VectAddr0 = (unsigned long)Timer0_handler;
  // Timer 0 interrupt line
  VIC_VectCtrl0_bit.IntSource = Timer0_IRQ;
  VIC_VectCtrl0_bit.E = 1;          // Enable slot
  // Enable timer 0 interrupt
  VIC_IntEnable_bit.TIMER0INTR = 1;

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

