//-----------------------------------------------------
// MAIN.C - TinyARM DIP50 LPC2129 example code
//-----------------------------------------------------
// Modified by I.Paulik-PASAT for TinyARM board
// The original code is from IAR
//-----------------------------------------------------

// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------


#include <stdio.h>
#include <string.h>
#include <intrinsics.h>
#include "timer.h"
#include "lpc212x.h"

/* Code Read Protection
CRP     0x87654321 - Read Memory is disabled.
									 - Write to RAM is disabled.
									 - Go command is disabled.
									 - Copy RAM to Flash is disabled.
									 - JTAG is disabled.
*/
#ifndef __RAM_DEBUG
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned int crp = 0xFFFFFFFF;
#endif

int main(void)
{
  unsigned long number,Var,Var2;

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
    Var=1; Var2=1;
    for (number = 0; number < 32; number++)
    {
      // Display number on 7-segment display.
      if (number==16) Var2=1;
      LPC210xSevenSegmentDisplay(number);
      LPC210xLedSet(Var);
      Port1_LedSet(Var2);
      Var <<= 1; Var2 <<= 1;
      Sleep(300); // Display for 300 ms.
    }

//    Var=1;
//    for (number = 0; number < 16; number++)
//    {
//      Port1_LedSet(Var);
//      Var <<= 1;
//      Sleep(300); // Display for 300 ms.
//    }
  }
}
