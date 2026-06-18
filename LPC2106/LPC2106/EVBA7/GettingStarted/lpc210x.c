
/*
 * $Revision: 28 $
 */

#include <intrinsics.h>
#include <NXP/iolpc2106.h>
#include "lpc210x.h"

#define VIC_TIMER0_bit (1 << VIC_TIMER0)

static void (*timer_function)();


//
// Interrupt handler.
//

static void TimerInterrupt()
{
  (*timer_function)(); // Call timer callback function.

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
  MEMMAP = 2;
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
  T0TCR = 2; // Disable timer 0.
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
  IODIR |= 0xf0;

  // Setup I2C controller.
  // The 7-segment LED is controlled by the I2C controller.
  I2CONCLR = 0x6C; // Clear AA, SI, STA and I2EN.

   PINSEL0 = 0x50; // Enable I2C SCL and SDA pins.

   I2CONSET = 0x40; // Set I2EN - I2C interface enable.
   I2CONSET = 0x24; // Set AA and STA.
   I2DAT = 0x42; // Slave address.
   I2CONCLR = 0x08; // Clear SI.
   I2CONCLR = 0x20; // Clear STA.
}


//
// LED output drivers.
//

void LPC210xLedSet(unsigned int mask)
{
  // Set LED D2, D3, D4 and D5.
  IOCLR = 0xf0;
  IOSET = (mask << 4) & 0xf0;
}


void LPC210xSevenSegmentDisplay(unsigned int value)
{
  // Display a hex number on the 7-segment display connected to the I2C bus.
  switch (value)
  {
  default:
  case 0x0:
    I2DAT = 0xC0;          // 0
    break;
  case 0x1 :
    I2DAT = 0xF9;          // 1
    break;
  case 0x2:
    I2DAT = 0xA4;          // 2
    break;
  case 0x3:
    I2DAT = 0xB0;          // 3
    break;
  case 0x4:
    I2DAT = 0x99;          // 4
    break;
  case 0x5:
    I2DAT = 0x92;          // 5
    break;
  case 0x6:
    I2DAT = 0x82;          // 6
    break;
  case 0x7:
    I2DAT = 0xF8;          // 7
    break;
  case 0x8:
    I2DAT = 0x80;          // 8
    break;
  case 0x9:
    I2DAT = 0x90;          // 9
    break;
  case 0xa:
    I2DAT = 0x88;          // A
    break;
  case 0xb:
    I2DAT = 0x83;          // B
    break;
  case 0xc:
    I2DAT = 0xC6;          // C
    break;
  case 0xd:
    I2DAT = 0xA1;          // D
    break;
  case 0xe:
    I2DAT = 0x86;          // E
    break;
  case 0xf:
    I2DAT = 0x8E;          // F
    break;
  }
  I2CONCLR = 0x08;  // Clear SI.
}


