/******************************************************************************
This example shows how to create a 1Hz interrupt using the real time clock
interrupt

Pin0.4 is connected to the LED that is fifth from the right on the bank of LEDs
at the bottom of the 2103 evaluation board.
Pin 0.5 is located on the vertical double column of pins, 4th from the bottom
on the right hand column

******************************************************************************/

#include "RTC_Interrupt.h"

int main (void)
{
  unsigned int *ptr2VectCtrlBase = (unsigned int *)0xFFFFF200;
  unsigned int *ptr2VectAddrBase = (unsigned int *)0xFFFFF100;
  us_secondsCount = 0;                         // No seconds ticked off yet
  __disable_interrupt();                       // PLL feed seq can't be interrupted
  PLLCON &= 0xFFFFFFFC;                        // Clear PLLE and PLLC
  PLLFEED = 0xAA;                              // Feed PLL so PLLCON setting take
  PLLFEED = 0x55;
  APBDIV = 0;                                  // PCLKFREQ = XTAL/4

  SCS_bit.GPIO0M = 0;                          // 0 for access via APB (traditional) 1 for fast

  PINSEL0_bit.P0_5 =  2;                       // Port Pin P0.5 is PWM output
  T0PWMCON_bit.PWM1ENA =   1;                  // Match register 1 is source of toggle
  T0PWMCON_bit.PWM3ENA =   1;                  // Match register 1 is source of toggle
  T0MR3 = PCLKFREQ/350;                        // 350Hz square wave
  T0MR1 = PCLKFREQ/500;                        // 30% duty cycle
  T0MCR_bit.MR3RES =  1;                       //
  T0TCR = 1;

  bl_TimerFlag = FALSE;                        // No background tasks performed yet
  PINSEL0_bit.P0_4=0;                          // select Pin P0.4 as GPIO
  IODIR |= (1<<PIN0_4);                        // Pin 4 Port 0 is output
  PCONP = ALL_PERIPHERALS_ON;                  // All peripherals turned on

  VICIntSelect  =  0;                          // Set all VIC interrupts to IRQ for now
  VICIntEnClear = 0xFFFFFFFF;                  // Diasable all interrupts
  VICProtection = 0;                           // Accessed in User or privileged mode
  VICVectAddr = 0;                             // Clear interrupt

  AMR =  0xFF;                                 // disable all alarm compares
  CCR =  0x11;                                 // enable the clock and use RTCX and clock source
  CIIR = 0x01;                                 // interrupt if the 'seconds' register increments

  for(unsigned int i=0; i<16; i++)
  {
    *ptr2VectCtrlBase++ = 0;                   // Disable all vectored IRQ slots
    *ptr2VectAddrBase++ = 0;                   // Clear address of the Interrupt Service routine (ISR)
  }                                            // for vectored IRQs

  VICIntSelect &= ~(1<<VIC_RTC);               // Make sure that RTC is an IRQ
  VICVectAddr10 = (unsigned long)RTC_INT_isr;  // set interrupt vector in 10
  VICVectCntl10 = 0x0000002D;                  // use RTC as source (D=13 decimal)
  VICIntEnable |=  (1<<VIC_RTC);               // Enable RTC interrupt

  __enable_interrupt();

  while(TRUE)                                  // foreground task
  {
    if(bl_TimerFlag)
    {
      if(IOPIN & (1<<PIN0_4))
        IOCLR |= (1<<PIN0_4);
      else
        IOSET |= (1<<PIN0_4);
      bl_TimerFlag = FALSE;
    }
  }
}                                              // end main()

__irq __arm void RTC_INT_isr(void)
{
  ILR =0x01;                                   // clear the 'seconds' interrupt flag
  us_secondsCount++;
  bl_TimerFlag = TRUE;                         // signal foreground task to toggle state of pin
  VICVectAddr =0x00000000;                     // Complete Interrupt cycle by dummy write to VIC
}

/* These handlers trap instruction fiq, prefetch abort, undefined instruction abort
   and data fetch abort exceptions
*/
__fiq __arm void fiq_handler(void)
{
  while(1){}
}

__irq __arm void undef_handler(void)
{
  while(1);
}

__irq __arm void prefetch_handler(void)
{
  while(1);
}

__irq __arm void data_handler(void)
{
  while(1);
}
