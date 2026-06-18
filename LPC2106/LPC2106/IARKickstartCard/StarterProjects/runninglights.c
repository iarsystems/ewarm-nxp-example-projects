
/*
 * $Revision: 28 $
 */


/*
  Running lights
  Timer interrupt and LED example for the Kickstart card.
  Displays running lights on the Kickstart Card LED:s.

  Jumper settings:
  ----------------
  For ROM-monitor:
  Set JP4 (enables UART0, used for ROM-monitor communication).
  Set jumpers p0.2-p0.14, p0.16, p0.17 and p0.31 in the LED jumper block.

  For J-Link or Standalone:
  Set JP6 to JTAG1.
  Set jumpers p0.0-p0.15 in the LED jumper block.
*/

#include <stdio.h>
#include <string.h>
#include <intrinsics.h>
#include "timer.h"
#include "lpc210x.h"

int main(void)
{
  unsigned int leds = 0x001f;
  
  // System initialization, this will map the exception vectors.
  LPC210xSystemInit();
  
  // Set up peripheral registers.
  LPC210xInitPIO();

  // First disable interrupts.
  __disable_interrupt();
  // Setup interrupt controller.
  LPC210xInitVIC();
  LPC210xInitTimerInterrupt(TimerBeat);
  
  // Periodic timer initialization.
  LPC210xInitTimer();
  // Enable interrupts.
  __enable_interrupt();

  // Start periodic timer.
  LPC210xStartTimer();

  // Loop forever.
  for (;;)
  {
    LPC210xLedSet(leds,0xffff);
    Sleep(65); // Display for 65 ms.
    leds <<= 1;
    leds |= leds>>16;
    leds &= 0xffff;
  }
}
