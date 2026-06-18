/******************************************************************************
Usage Notes:

This application is intended to run on the IAR LPC2103 evaluation board.

Note: This application intended to run out of RAM with PLL disabled. PLL is
ensured to be disabled by feed sequence at start.

Pin P0.6 on the IAR LPC2103 board is on vertical row, left side across from
Pin P0.7

******************************************************************************/

#include "Timer0_IntOnMR0.h"

int main(void)
{
  unsigned int *ptr2VectCtrlBase = (unsigned int *)0xFFFFF200;
  unsigned int *ptr2VectAddrBase = (unsigned int *)0xFFFFF100;
  MAMCR = 2;                      // Per NXP advisory 7/11/05 for correct operation
                                  // from flash in stand alone mode
  __disable_interrupt();          // PLL feed seq can't be interrupted
  PLLCON &= 0xFFFFFFFC;           // Clear PLLE and PLLC
  PLLFEED = 0xAA;                 // Feed PLL so PLLCON setting take
  PLLFEED = 0x55;
  APBDIV = 0;                     // PCLKFREQ = XTAL/4

  bl_Tmr0Flag = 0;                // No background tasks performed yet

  PINSEL0 = 0;                    // Make all pins including P0.4 GPIO function
  IODIR |= 0x3FF3;               // Make P0.0,P0.1,P0.4-P0.13 outputs
  PINSEL0_bit.P0_14=0x1;          // Set Port pin P01.4 function to EINT1
  PINSEL0_bit.P0_15=0x1;          // Set Port pin P0.15 function to EINT2

  T0IR=0xFF;                      // reset match and capture event interrupts
  T0TC=0;                         // Clear timer counter
  T0PR= 0;                        // No Prescalar
  T0MR0=PCLKFREQ/100;             // Count up to 30,000 for 100Hz interrupt
  T0MCR = 3;                      // Reset Timer Counter & Interrupt on match
  T0TCR = 1;                      // Counting enable

  VICIntSelect  =  0;             // Set all VIC interrupts to IRQ for now
  VICIntEnClear = 0xFFFFFFFF;     // Diasable all interrupts
  VICProtection = 0;              // Accessed in User or privileged mode
  VICVectAddr = 0;                // Clear interrupt

  for(unsigned int i=0; i<16; i++)
  {
    *ptr2VectCtrlBase++ = 0;      // Disable all vectored IRQ slots
    *ptr2VectAddrBase++ = 0;      // Clear address of the Interrupt Service routine (ISR)
  }                               // for vectored IRQs

  VICIntSelect &= ~(1<<VIC_TIMER0);                // Timer 0 intrpt is an IRQ (VIC_TIMER0 = 4)
  VICVectAddr1 = (unsigned int)&Tmr0_IntOnMR0_isr; // Install ISR in VIC addr slot 0
  VICVectCntl1 = 0x20 | VIC_TIMER0;                // IRQ type, TIMER 0 int enabled
  VICIntEnable |= (1<<VIC_TIMER0);                 // Turn on Timer0 Interrupt

  /* Set up the button pressed interrupt on EINT1 */
  VICIntSelect &= ~(1<<VIC_EINT1);                 // IRQ on external int 2.
  VICVectAddr2 = (unsigned int)&EINT1BtnDown_isr;  // Install EINT2 in VIC addr slot
  VICVectCntl2 = 0x20 | VIC_EINT1;                 // Enable vect int for EINT2.
  VICIntEnable |= (1<<VIC_EINT1);                  // Enable EINT 2 interrupt.

  /* Set up the button pressed interrupt on EINT2 */
  VICIntSelect &= ~(1<<VIC_EINT2);                 // IRQ on external int 2.
  VICVectAddr3 = (unsigned int)&EINT2BtnDown_isr;  // Install EINT2 in VIC addr slot
  VICVectCntl3 = 0x20 | VIC_EINT2;                 // Enable vect int for EINT2.
  VICIntEnable |= (1<<VIC_EINT2);                  // Enable EINT 2 interrupt.

  __enable_interrupt();                            // Global interrupt enable

  while(TRUE)                                      // Foreground "task"
  {
    if(bl_Tmr0Flag)
    {
      bl_Tmr0Flag = FALSE;                         // Clear flag set by isr
      TogglePortPin(PIN0_6);                       // Toggle port pin6
    }
  }                                                // end foreground loop
}                                                  // end main()

__irq __arm void Tmr0_IntOnMR0_isr(void)
{
  static unsigned int us_Ticks =0;
  unsigned int intrptID;
  intrptID = T0IR & 0xFF;                          // determine which channel
  T0IR = intrptID;                                 // interrupted & clear timer interrupt
  us_Ticks++;
  if(us_Ticks == TICKS_PER_FIVEHUNDRED_MSEC)
  {
    bl_Tmr0Flag = TRUE;                            // The background "task"
    us_Ticks = 0;
  }
  VICVectAddr = 0;                                 // dummy write to complete interrupt cycle
}

__irq __arm void EINT1BtnDown_isr(void)
{
  EXTINT_bit.EINT1 = 1;                            // Try to reset external interrupt flag.
  if(!EXTINT_bit.EINT1)                            // Check if flag was reset (button not pressed).
  {
    us_EINT1Count++;
    TogglePortPin(PIN0_8);                         // Toggle port pin8
  }
  VICVectAddr = 0;                                 // dummy write to complete interrupt cycle
}

__irq __arm void EINT2BtnDown_isr(void)
{
  EXTINT_bit.EINT2 = 1;                            // Try to reset external interrupt flag.
  if(!EXTINT_bit.EINT2)                            // Check if flag was reset (button not pressed).
  {
    us_EINT2Count++;
    TogglePortPin(PIN0_A);                         // Toggle port pin8
  }
  VICVectAddr = 0;                                 // dummy write to complete interrupt cycle
}

void TogglePortPin(unsigned int portPinID)
{

  if( IOPIN & (1<<portPinID) )                     // Is portPinID High?
    IOCLR |= (1<<portPinID);                       // Yes, make it low
  else
    IOSET |= (1<<portPinID);                       // No, set pin high
}

/* These handlers trap instruction fiq, prefetch abort, undefined instruction abort
   and data fetch abort exceptions
*/
__fiq __arm void fiq_handler(void)
{
  while(1){}
}

__arm void undef_handler(void)
{
  while(1);
}

__arm void prefetch_handler(void)
{
  while(1);
}

__arm void data_handler(void)
{
  while(1);
}
