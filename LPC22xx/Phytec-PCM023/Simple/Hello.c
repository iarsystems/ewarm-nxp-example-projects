/*
  File: Hello.c

  To prompt the user for name and respond.

  Notes: Please check your board revision and uncomment appropriate
  clock definitions
   for 1179.6 revision ckock must be XTALFREQ - 10000000 and
   for 1179.5 revision ckock must be XTALFREQ - 60000000

*/

#include <intrinsics.h>
#include <stdio.h>
#include "lpc2294.h"
#include "serial.h"

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

char name[20];

int main (void)
{
  // System initialization, this will map the exception vectors.
  LPC2294SystemInit();

  //Set up UART
  LPC2294InitUART0();

  // First disable interrupts.
  __disable_interrupt();

  // Setup interrupt controller.
  LPC2294InitVIC();
  LPC2294InitUART0Interrupt(byteReceived, txBufEmpty);

  // Enable interrupts.
  __enable_interrupt();

  // Loop forever.
  for(;;)
  {
    printf("Hello, May I have your name\?\n\r>");
    scanline((char*)&name);
    printf("\nHello %s.  Welcome to IAR Systems.\n\n\n\r", name);
  }
}
