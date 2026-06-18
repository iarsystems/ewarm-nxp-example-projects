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
 to develop code for IAR-LPC-11U24-SK board. It shows basic use of I/O,
 timer, ADC and interrupt controllers.
 It shows the Temperature and the Trimer position on LCD and UART
 Press B1 to see the temperature on the LCD.
 Press B2 to see the trimer position on the LCD

COMPATIBILITY
=============

   The example project is compatible with, on IAR-LPC-11U24-SK evaluation
  board. By default, the project is configured to use the J-Link SWD
  interface.

CONFIGURATION
=============

   Make sure that the following jumpers are correctly configured on the
  IAR-LPC11U24-SK evaluation board:

  Jumpers:
   RST_E     - unfilled
   ISP_E     - unfilled
   PIO1_LOW  - unfilled
   PIO3_HIGH - unfilled
   PIO3_E    - filled
   TMS/SWD   - SWD (2-3)

  The GettingStarted application is downloaded to the iFlash.

GETTING STARTED
===============

  1) Build and download the program.

  2) Run the program.
