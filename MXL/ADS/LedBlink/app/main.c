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
 *    1. Date        : 7 Feb. 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Freescale MXL ADS evaluation board based on
 * MC9328MXL. It shows basic use of I/O, timer and the interrupt controller.
 *  It blinks the LEDs PA2 and PA23.
 *
 *    $Revision: 16 $
**************************************************************************/
#include "includes.h"

#define LED_PA2           (1UL<<2)
#define LED_PA23          (1UL<<23)

Int32U Gpt1Counter = 0;

/*************************************************************************
 * Function Name: Gpt2_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT2 interrupt handler
 *
 *************************************************************************/
void Gpt2_Handler (void)
{
  // Clear interrupt flag
  TSTAT2_bit.COMP = 1;
  ++Gpt1Counter;
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: int
 *
 * Description: Main subroutine
 *
 *************************************************************************/
int main(void)
{
Int32U LedState;

  DbgUartPrint ("\n\rEnter in main() section\n\r");

  // GPIO init
  GIUS_A |= LED_PA2 | LED_PA23;   // set PA2, PA23 as GPIO
  DDIR_A |= LED_PA2 | LED_PA23;   // set PA2, PA23 as oputs
  OCR1_A_bit.PIN2 = 3;            // PA2 Output Configuration — Data Register
  OCR2_A_bit.PIN23= 3;            // PA23 Output Configuration — Data Register

  // Init GPT1
  TCTL2 = 0;                      // Clear GPT1 ctrl register
  TCTL2_bit.CLKSOURCE = 1;        // Clock Source — 001 = PERCLK1 to prescaler
  TCTL2_bit.IRQEN     = 1;        // Enable the compare interrupt
  TPRER2_bit.PRESCALER = 0;       // PERCLK1/1
  TCMP2 = (Int32U)(PERCLK1 / TICK_PER_SEC); // Init the Timer's period
  TCTL2_bit.TEN = 1;              // Enable GPT2

  // Init AITC PGT2 interrupt
  AitcEnableInt(Gpt2_Handler,TIMER2_INT,5,FALSE);

  // Enable interrupts
  __enable_interrupt();

  while(1)
  {
    if(Gpt1Counter > 20)
    {
      Gpt1Counter = 0;
      if(++LedState & 1)
      {
        // Led STAT1 ON
        DR_A |= LED_PA2;
      }
      else
      {
        // Led STAT1 OFF
        DR_A &= ~LED_PA2;
      }
      if(LedState & 2)
      {
        // Led STAT2 ON
       DR_A |= LED_PA23;
      }
      else
      {
        // Led STAT2 OFF
        DR_A &= ~LED_PA23;
      } // if(LedState & 2)
    } // if(Gpt1Counter > 20)
  } // while(1)
} // main
