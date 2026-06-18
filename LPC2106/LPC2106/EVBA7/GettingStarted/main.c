
/*
 * $Revision: 28 $
 */

#include <stdio.h>
#include <string.h>
#include <intrinsics.h>
#include "timer.h"
#include "lpc210x.h"

int main(void)
{
  int number;
  
  // System initialization, this will map the exception vectors.
  LPC210xSystemInit();
  
  // Set up peripheral registers.
  LPC210xInitPIO();
  
  // First disable interrupts.
  __disable_interrupt();
  // Setup interrupt controller.
  LPC210xInitInterrupt(TimerBeat);
  // Periodic timer initialization.
  LPC210xInitTimer();
  // Enable interrupts.
  __enable_interrupt();

  // Start periodic timer.
  LPC210xStartTimer();

  // Loop forever.
  for (;;)
  {
    for (number = 0; number < 10; number++)
    {
      // Display number on 7-segment display.
      LPC210xSevenSegmentDisplay(number);
      LPC210xLedSet(number);
      Sleep(500); // Display for 500 ms.
    }
  }
}
