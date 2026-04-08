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
 to develop code for Embedded Artists LPC24xx board. It shows basic use of I/O,
 timer and the interrupt controllers.
  It starts by blinking LCD backlight.

COMPATIBILITY
=============

   The example project is compatible with, on Embedded Artists LPC24xx
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   The GettingStarted application is downloaded to the iFlash, iRAM and external
  NOR memory depending of the project's configuration and executed.

   Make sure that all jumpers are set according board's manual

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

      <installation-root>\arm\examples\NXP\
     LPC24xx\EmbArtLPC24xx\GettingStarted\GettingStarted.eww

  3) Run the program.