/******************************************************************************
Usage Notes:

This application is intended to run on the IAR LPC2103 evaluation board.

About every six seconds, LEDs 0-3 will slew in forward direction. This is the
execution of the Timer0 interrupt on MR0 interrupt service routine. EXTINT2,
which is connected to the rightmost button on the board (viewed with UART con-
nectors to the right) will cause execution of an isr which slews LEDs 11,10,9,
8 to the right. While Timer0 interrupt on MR0 isr is executing, push the
EXTINT2 button and observe that the Timer0 interrupt on MR0 isr will stop
executing, and the EXTINT2 isr will execute and when completed, the Timer0
interrupt on MR0 isr will complete execution.

Note: This application intended to run out of RAM with PLL disabled. PLL is
ensured to be disabled by feed sequence at start.


******************************************************************************/

#include "Fiq_Int_IrQ_Ex.h"

int main(void)
{
  UINT32 *ptr2VectCtrlBase = (unsigned int *)0xFFFFF200UL;
  UINT32 *ptr2VectAddrBase = (unsigned int *)0xFFFFF100UL;

  MAMCR = 2;                      // Per NXP advisory 7/11/05 for correct operation from
                                  // flash in stand alone mode
  __disable_interrupt();                         // PLL feed seq can't be interrupted
  PLLCON &= 0xFFFFFFFCUL;                        // Clear PLLE and PLLC
  PLLFEED = 0x0000000AAUL;                       // Feed PLL so PLLCON setting take
  PLLFEED = 0x000000055UL;
  APBDIV = 0;                                    // PCLKFREQ = XTAL/4
  PINSEL0 &= ~(0xFFF);                           // Pins 0-11 inclusive are GPIO
  IODIR  |= 0xFFF;                               // Pins 0-11 inclusive are outputs
  IOCLR  |= 0xFFF;                               // Turn off LEDs 0-3 which come on at pwr up

  PINSEL0_bit.P0_15=0x1;                         // Set Port pin P0.15 function to EINT2

  /* Initialize Timer 0 registers */
  T0TCR = 2;                                     // Reset & Freeze Timer 0 counter
  T0IR=0xFF;                                     // reset match and capture event interrupts
  T0PR = 1500;
  T0MR0 = 11520;                                 // intrp rate = 1501*11520/3686400=.21Hz
  T0MCR = 3;                                     // Reset Timer Counter & Interrupt on match
  T0TCR = 1;                                     // Counting enable

  for(UINT32 i=0; i<16; i++)
  {
    *ptr2VectCtrlBase++ = 0;                     // Disable all vectored IRQ slots
    *ptr2VectAddrBase++ = 0;                     // Clear address of the Interrupt Service routine (ISR) for vectored IRQs
  }

  VICIntSelect =  0;                             // Set all VIC interrupts to IRQ for now
  VICIntEnClear = 0xFFFFFFFFUL;                  // Diasable all interrupts
  VICProtection = 0;                             // VIC registers can be accessed in User or privileged mode
  VICVectAddr =   0;                             // Clear interrupt

  VICIntSelect &= ~(1<<VIC_TIMER0);              // Timer 0 intrpt is an IRQ (VIC_TIMER0 = 4)
  VICVectAddr0 = (unsigned int)&Tmr0IntOnMR0_isr;// Install ISR in VIC addr slot 0
  VICVectCntl0 = 0x20 | VIC_TIMER0;              // IRQ type, TIMER 0 int enabled
  VICIntEnable |= (1<<VIC_TIMER0);               // Turn on Timer0 Interrupt

  VICIntSelect |= (1<<VIC_EINT2);                // FIQ on external int 2
  VICIntEnable |= (1<<VIC_EINT2);                // Enable EINT 2 interrupt

  __enable_interrupt();                          // Global interrupt enable

  while(TRUE)                                    // Foreground "task"
  {}
}

__irq __arm void Tmr0IntOnMR0_isr(void)
{
  UINT32 us_intrptID;
  T0TC = 0x2;                                    // Freeze the counter
  us_intrptID = T0IR & 0xFF;
  T0IR = us_intrptID & 0xFF;                     // Clear timer interrupt
  for(UINT32 i=4; i>0; i--)
  {
    for(UINT32 ledid=0; ledid<4; ledid++)
    {
      IOSET |= (1<<ledid);
      for(UINT32 j=MANDELAY; j>0; j--){}
      IOCLR |= (1<<ledid);
      for(UINT32 j=MANDELAY; j>0; j--){}
    }
  }
  T0TC = 0x1;                                    // Start counting again
  VICVectAddr = 0;
}

__fiq __arm void fiq_handler(void)            // at 0x1C, the FIQ exception slot
{
  while(EXTINT_bit.EINT2)
  {
    EXTINT_bit.EINT2 = 1;                        // Try to reset external interrupt flag
  }
  for(UINT32 i=4; i>0; i--)
  {
    for(UINT32 ledid=11; ledid>7; ledid--)
    {
      IOSET |= (1<<ledid);
      for(UINT32 j=MANDELAY; j>0; j--)
      {}
      IOCLR |= (1<<ledid);
      for(UINT32 j=MANDELAY; j>0; j--)
      {}
    }
  }
}                                                // end EXT2Button_isr()

/* These handlers trap instruction prefetch abort, undefined instruction abort
   and data fetch abort exceptions
*/
__arm void undef_handler(void)
{
  while(1){}
}

__arm void prefetch_handler(void)
{
  while(1){}
}

__arm void data_handler(void)
{
  while(1){}
}
