6/4/05

Project LPC2148_Simplify_3
Spec:
  1. Press BTN 1 vector into EINT1 ISR
  2. Press BTN 2 vector into EINT0 ISR
  3. Software timers chained to TIMER0_ISR should signal events at 100mSec,
  One Sec, and Five Sec intervals
  4. Provide a dummy foreground loop for the user to use as template for
  installing event handlers for timer and button driven events.

Author: Mark Moran
Last Date Modified: 6 Jun 05
Status: Works OK

Note: See usage notes at top of Demo.c to see how to use watchpoints to cause
breaks in __irq __arm void irq_handler (void) when BTN1 and BTN2 pressed

Copyright: IAR Systems Software Inc. 2005
Disclaimer: This code is furnished "as is" to illustrate how to use the IAR
Embedded Workbench integrated development environment to create and debugcode
for NXP LPC2xxx ARM7 microcontrollers. Code in this example may be used
freely for any purpose. Every effort has been made to ensure compliance
with above specification. Anyone using this code for any purpose whatsoever
agrees to accept all pursuant risks in use and hold IAR harmless for any and all
damages. In other words, if this code smokes your board, it is not our fault
