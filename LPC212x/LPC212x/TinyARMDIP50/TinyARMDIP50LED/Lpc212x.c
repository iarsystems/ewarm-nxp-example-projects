//-----------------------------------------------------
// TinyARM DIP50 LPC2129 example code
//-----------------------------------------------------
// Modified by I.Paulik-PASAT for TinyARM board
// V0.02 13.03.04 Modified for LPC212x - PINSEL1 init
// V0.01 18.12.03 Initial version - LEDs on PIO24-31
// The original code is from IAR
//-----------------------------------------------------
// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

#include <intrinsics.h>
#include <NXP/iolpc2129.h>
#include "lpc212x.h"

#define VIC_TIMER0_bit (1 << VIC_TIMER0)

static void (*timer_function)();


//
// Interrupt handler.
//

static void TimerInterrupt()
{
  (*timer_function)(); // Call timer callback function.

  //__IR0 = 0xff; // Clear timer 0 interrupt line.
  T0IR = 0xff; // Clear timer 0 interrupt line.
}


// IRQ interrupt handler.
// Only the timer interrupt is used by this example.
__irq __arm void IRQ_Handler(void)
{
  void (*interrupt_function)();
  unsigned int vector;

  // Called at 1000 Hz rate.
  vector = VICVectAddr; // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)(); // Call vectored interrupt function.

  VICVectAddr = 0; // Clear interrupt in VIC.
}


//
// System initialization.
//
void LPC210xSystemInit()
{
  // Map lowest 64 bytes of the address space to bottom of internal RAM.
  // This will move the exception vectors in place.
#ifdef EXEC_FLASH
// Code placed in Flash
  MEMMAP = 1;
#else
// Code placed in RAM
  MEMMAP = 2;
#endif
}


//
// Interrupt controller initalization.
//

void LPC210xInitInterrupt(void(*timer_func)())
{
  // Setup timer callback function.
  timer_function = timer_func;

  // Setup interrupt controller.
  VICProtection = 0;
  // Disable all interrupts
  VICIntEnClear = 0xffffffff;

  VICIntSelect &= ~VIC_TIMER0_bit; // IRQ on timer 0 line.
  VICVectAddr0 = (unsigned int)&TimerInterrupt;
  VICVectCntl0 = 0x20 | VIC_TIMER0; // Enable vector interrupt for timer 0.
  VICIntEnable |= VIC_TIMER0_bit; // Enable timer 0 interrupt.
}


//
// Timer functions.
//

void LPC210xInitTimer()
{
  T0TCR = 0; // Disable timer 0.
  // The prescaler refuses any value other than zero!?
  T0PC = 0; // Prescaler is set to no division. Cclk is 10 MHz, pclk is 2.5 MHz.
  T0MR0 = 2500; // Count up to this value. Generate 1000 Hz interrupt.
  T0MCR = 3; // Reset and interrupt on MR0 (match register 0).
  T0CCR = 0; // Capture is disabled.
  T0EMR = 0; // No external match output.
}


void LPC210xStartTimer()
{
  T0TCR = 1; // Enable timer 0.
}


//
// Parallel I/O functions.
//

void LPC210xInitPIO()
{
  // Set LED pins on GPIO to output.
   IO0DIR |= 0x7BffffffL;
   IO1DIR |= 0xffff0000L;

   PINSEL0 = 0x00; // Enable I2C SCL and SDA pins.
   // LPC212x P0.27-P0.30 are set to AIN0..3 after RESET
   PINSEL1 = 0x00; // LPC212x port assignment
}


//
// LED output drivers for P0.0 to P0.31.
//
void LPC210xLedSet(unsigned long mask)
{
  // Set LED D1-D32.
  IO0CLR = 0xffffffffL;
  // Port 0.2,0.3 are open drain for I2C compatibility
  // LEDs on those pins are pulluped to Vcc
  IO0SET = mask ^ 0x0C;
}

//
// LED output drivers for P1.0 to P1.31.
//
void Port1_LedSet(unsigned long mask)
{
  // Set LED D1-D32.
  IO1CLR = 0xffff0000L;
  IO1SET = (mask << 16) & 0xffff0000L;
}

void LPC210xSevenSegmentDisplay(unsigned int value)
{
}


