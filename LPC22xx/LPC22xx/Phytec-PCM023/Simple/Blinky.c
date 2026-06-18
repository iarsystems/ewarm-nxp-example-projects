/*
  File: Blinky.c

  To blink the onboard LED (D3) of the Phytec PCM-023 board
  for every 65ms.

  Notes: Please check your board revision and uncomment appropriate
  clock definitions
   for 1179.6 revision ckock must be XTALFREQ - 10000000 and
   for 1179.5 revision ckock must be XTALFREQ - 60000000
*/

#include <intrinsics.h>
#include "timer.h"
#include "lpc2294.h"

/* Code Read Protection
CRP     0x87654321 - Read Memory is disabled.
									 - Write to RAM is disabled.
									 - Go command is disabled.
									 - Copy RAM to Flash is disabled.
									 - JTAG is disabled.
*/
#if !defined(xRAM) && !defined(iRAM)
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned int crp = 0xFFFFFFFF;
#endif

int main (void)
{
  // System initialization, this will map the exception vectors.
  LPC2294SystemInit();

  // Set up peripheral registers.
  LPC2294InitPIO();

  // First disable interrupts.
  __disable_interrupt();

  // Setup interrupt controller.
  LPC2294InitVIC();
  LPC2294InitTimerInterrupt(TimerBeat);

  // Periodic timer initialization.
  LPC2294InitTimer();

  // Enable interrupts.
  __enable_interrupt();

  // Start periodic timer.
  LPC2294StartTimer();

  // Loop forever.
  for (;;)
  {
    LPC2294LedSet();
    Sleep(200);      // Display for 65 ms.
    LPC2294LedClear();
    Sleep(200);      // Display for 65 ms.
  }
}
