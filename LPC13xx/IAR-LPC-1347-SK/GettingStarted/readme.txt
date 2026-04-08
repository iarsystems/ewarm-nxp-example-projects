########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for IAR-LPC1347-SK board. It shows basic use of I/O,
 timer, ADC and interrupt controllers.
 It shows the Temperature and the Trimer position no LCD and UART
 Press B1 to see the temperature on the LCD.
 Press B2 to see the trimer position on the LCD

COMPATIBILITY
=============

   The example project is compatible with, on IAR-LPC1347-SK evaluation
  board. By default, the project is configured to use the J-Link SWD
  interface.

CONFIGURATION
=============

  Jumpers:
   RST_E   - unfilled
   ISP_E   - unfilled
   TMS/SWDIO - SWDIO (2-3)
   TDO/SWV   - SWV (2-3)

  The GettingStarted application is downloaded to the iFlash.

GETTING STARTED
===============

  1) Build and download the program.
