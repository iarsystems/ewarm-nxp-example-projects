This example shows a "bare bones" program to set up an interrupt on Timer0 match compare on
channel 0, and how to install the ISR, and configure the VIC for it to interrupt as an IRQ.

Specifications:
1. Configure Timer0 to interrupt on matching value 3,686 in MR0
2. increment a counter in timer isr
3. when counter in isr = 100, set a flag in foreground loop to signal 1 second elapsed
4. clear flag in foreground loop when set

Author: Mark Moran
Last Date Modified: 6 Jun 05
Status: Works OK

Usage Notes: Set breakpoint at statement bl_TimerFlag = FALSE; on line 60 to be see that one-
second intervals are being signalled.

Copyright: IAR Systems Software Inc. 2005
Disclaimer: This code is furnished "as is" to illustrate how to use the IAR Embedded Workbench
integrated development environment to create code for NXP LPC2xxx ARM7 microcontrollers.
Code in this example may be used freely with the understanding that IAR Systems Software does
not guarantee any aspect of it. If decision is made to use this code in applications, user
takes full responsibility for code at that time.
