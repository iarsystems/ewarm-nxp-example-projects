Project Start: 6/6/05

Project LPC2148_Simple_UART

Intent:
This application is intended to show how to set up the UART to transmit charac-
ters by a simple polling routine.

Specifications:
1. Send a character out of serial port UART1 every 50 milliseconds
2. 50msec time interval set by software timer
3. Timer0 generates master interrupt at 100Hz by compare to match register 0

UART1 uses the following settings: 9600 baud, 8 data bits, 1 stop bit, no parity.

Author: Mark Moran
Last Date Modified: 9 Jun 05
Status: Works According to specification

Copyright: IAR Systems Software Inc. 2005
Disclaimer: This code is furnished "as is" to illustrate how to use the IAR
Embedded Workbench integrated development environment to create and debugcode
for NXP LPC2xxx ARM7 microcontrollers. Code in this example may be used
freely for any purpose. Every effort has been made to ensure compliance
with above specification. Anyone using this code for any purpose whatsoever
agrees to accept all pursuant risks in use and hold IAR harmless for any and all
damages. In other words, if this code smokes your board, it is not our fault.