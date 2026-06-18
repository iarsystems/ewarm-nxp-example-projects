/******************************************************************************
Usage Notes:

This application is intended to run on the IAR LPC2103 evaluation board.

Note: This application intended to run out of RAM with PLL disabled. PLL is
ensured to be disabled by feed sequence at start.

Pin P0.6 on the IAR LPC2103 board is on vertical row, left side across from
Pin P0.7

This application uses linker command, ddf and programming files located in
$PROJ_DIR$\..\..\Common_Files so folder Common_Files must be located at this
place relative to file ..\StarterProjects\LPC2148_TMR0Int_Example\Project_Files

******************************************************************************/

#include "Timer0.1.2.3_IntOnMR0.h"

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
#ifdef PRELIMINARY
  VPBDIV = 0;                     // PCLKFREQ = XTAL/4
#else
  APBDIV = 0;                     // PCLKFREQ = XTAL/4
#endif

  bl_TmrFlag = 0;                // No background tasks performed yet

  PINSEL0 = 0;                    // Make all pins including P0.4 GPIO function
#ifdef PRELIMINARY
  IO0DIR |= 0x3FFF;               // Make P0.0,P0.1,P0.4-P0.13 outputs
  IO0CLR |= 0X154;                // 0b0000000101010100 Turn off LEDs 2,4,6,8
#else
  IODIR |= 0x3FFF;               // Make P0.0,P0.1,P0.4-P0.13 outputs
  IOCLR |= 0X154;                // 0b0000000101010100 Turn off LEDs 2,4,6,8
#endif

  T0IR=0xFF;                      // reset match and capture event interrupts
  T0TC=0;                         // Clear timer counter
  T0PR= 0;                        // No Prescalar
  T0MR0=PCLKFREQ/100;             // Count up to 30,000 for 100Hz interrupt tau = .010s
  T0MCR = 3;                      // Reset Timer Counter & Interrupt on match
  T0TCR = 1;                      // Counting enable

  T1IR=0xFF;                      // reset match and capture event interrupts
  T1TC=0;                         // Clear timer counter
  T1PR= 0;                        // No Prescalar
  T1MR0=PCLKFREQ/143;             // Count up to 25,779 for 143Hz interrupt tau = .007s
  T1MCR = 3;                      // Reset Timer Counter & Interrupt on match
  T1TCR = 1;                      // Counting enable

  T2IR=0xFF;                      // reset match and capture event interrupts
  T2TC=0;                         // Clear timer counter
  T2PR= 0;                        // No Prescalar
  T2MR0=PCLKFREQ/91;              // Count up to 40,510 for 91Hz interrupt tau = .011s
  T2MCR = 3;                      // Reset Timer Counter & Interrupt on match
  T2TCR = 1;                      // Counting enable

  T3IR=0xFF;                      // reset match and capture event interrupts
  T3TC=0;                         // Clear timer counter
  T3PR= 0;                        // No Prescalar
  T3MR0=PCLKFREQ/333;             // Count up to 11,070 for 333Hz interrupt tau = .003s
  T3MCR = 3;                      // Reset Timer Counter & Interrupt on match
  T3TCR = 1;                      // Counting enable

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

  VICIntSelect &= ~(1<<VIC_TIMER1);                // Timer 1 intrpt is an IRQ (VIC_TIMER0 = 4)
  VICVectAddr2 = (unsigned int)&Tmr1_IntOnMR0_isr; // Install ISR in VIC addr slot 0
  VICVectCntl2 = 0x20 | VIC_TIMER1;                // IRQ type, TIMER 0 int enabled
  VICIntEnable |= (1<<VIC_TIMER1);                 // Turn on Timer0 Interrupt

  VICIntSelect &= ~(1<<VIC_TIMER2);                // Timer 2 intrpt is an IRQ (VIC_TIMER0 = 4)
  VICVectAddr3 = (unsigned int)&Tmr2_IntOnMR0_isr; // Install ISR in VIC addr slot 0
  VICVectCntl3 = 0x20 | VIC_TIMER2;                // IRQ type, TIMER 0 int enabled
  VICIntEnable |= (1<<VIC_TIMER2);                 // Turn on Timer0 Interrupt

  VICIntSelect &= ~(1<<VIC_TIMER3);                // Timer 2 intrpt is an IRQ (VIC_TIMER0 = 4)
  VICVectAddr4 = (unsigned int)&Tmr3_IntOnMR0_isr; // Install ISR in VIC addr slot 0
  VICVectCntl4 = 0x20 | VIC_TIMER3;                // IRQ type, TIMER 0 int enabled
  VICIntEnable |= (1<<VIC_TIMER3);                 // Turn on Timer0 Interrupt

  __enable_interrupt();                            // Global interrupt enable

  while(TRUE)                                      // Foreground "task"
  {
    if( bl_TmrFlag & (1<<PIN0_2) )
    {
      bl_TmrFlag &= ~(1<<PIN0_2);                  // Clear flag set by Timer0 isr
      TogglePortPin(PIN0_2);                       // Toggle port pin
    }
    if( bl_TmrFlag & (1<<PIN0_4) )
    {
      bl_TmrFlag &= ~(1<<PIN0_4);                  // Clear flag set by Timer1 isr
      TogglePortPin(PIN0_4);                       // Toggle port pin
    }
    if( bl_TmrFlag & (1<<PIN0_6) )
    {
      bl_TmrFlag &= ~(1<<PIN0_6);                  // Clear flag set by Timer2 isr
      TogglePortPin(PIN0_6);                       // Toggle port pin
    }
    if( bl_TmrFlag & (1<<PIN0_8) )
    {
      bl_TmrFlag &= ~(1<<PIN0_8);                  // Clear flag set by Timer3 isr
      TogglePortPin(PIN0_8);                       // Toggle port pin
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
  if(us_Ticks == TICKS_PER_500mSEC_at_100Hz)
  {
    bl_TmrFlag |= (1<<PIN0_2);                     // signal toggle for LED2
    us_Ticks = 0;
  }
  VICVectAddr = 0;                                 // dummy write to complete interrupt cycle
}

__irq __arm void Tmr1_IntOnMR0_isr(void)
{
  static unsigned int us_Ticks =0;
  unsigned int intrptID;
  intrptID = T1IR & 0xFF;                          // determine which channel
  T1IR = intrptID;                                 // interrupted & clear timer interrupt
  us_Ticks++;
  if(us_Ticks == TICKS_PER_700mSEC_at_143Hz)
  {
    bl_TmrFlag |= (1<<PIN0_4);                     // signal toggle for LED4
    us_Ticks = 0;
  }
  VICVectAddr = 0;                                 // dummy write to complete interrupt cycle
}

__irq __arm void Tmr2_IntOnMR0_isr(void)
{
  static unsigned int us_Ticks =0;
  unsigned int intrptID;
  intrptID = T2IR & 0xFF;                          // determine which channel
  T2IR = intrptID;                                 // interrupted & clear timer interrupt
  us_Ticks++;
  if(us_Ticks == TICKS_PER_1100mSEC_at_91Hz)
  {
    bl_TmrFlag |= (1<<PIN0_6);                     // signal toggle for LED6
    us_Ticks = 0;
  }
  VICVectAddr = 0;                                 // dummy write to complete interrupt cycle
}

__irq __arm void Tmr3_IntOnMR0_isr(void)
{
  static unsigned int us_Ticks =0;
  unsigned int intrptID;
  intrptID = T3IR & 0xFF;                          // determine which channel
  T3IR = intrptID;                                 // interrupted & clear timer interrupt
  us_Ticks++;
  if(us_Ticks == TICKS_PER_300mSEC_at_333Hz)
  {
    bl_TmrFlag |= (1<<PIN0_8);                     // signal toggle for LED8
    us_Ticks = 0;
  }
  VICVectAddr = 0;                                 // dummy write to complete interrupt cycle
}

void TogglePortPin(unsigned int portPinID)
{
  static unsigned int pinstate;
  pinstate = IOPIN & (1<<portPinID);

  if(pinstate)                                     // Is portPinID High?
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
