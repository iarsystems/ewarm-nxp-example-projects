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
 to develop code for IAR-LPC-P2378-SK board. It shows basic use of I/O,
 timer and the interrupt controllers.
  It starts by blinking USB Link LED.

COMPATIBILITY
=============

   The example project is compatible with, on IAR-LPC-P2378-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   Make sure that the following jumpers are correctly configured on the
  IAR LPC-P2378-SK evaluation board:
  PWR_SEL     - depending of power source
  USBC/USBSC  - USBSC
  RST_E       - unfilled
  ISP_E       - unfilled

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

      <installation-root>\arm\examples\NXP\
     LPC23xx\IAR-LPC-P2378-SK\GettingStarted\GettingStarted.eww

  3) Run the program.
