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
 to develop code for Keil MCB2300 board. It shows basic use of I/O,
 timer and the interrupt controllers.
  Displays running lights.

COMPATIBILITY
=============
   The example project is compatible with, on Keil MCB2300
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

  Jumpers:
   VBUS - filled

   The GettingStarted application is downloaded to the iFlash or iRAM memory
  depending of the project's configuration and executed.

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

      <installation-root>\arm\examples\NXP\
     LPC17xx\KEIL-MCB2300\GettingStarted\GettingStarted.eww
  3) Connect USB cable to the Type-B USB connector (used for power supplay)
  4) Run the program.
