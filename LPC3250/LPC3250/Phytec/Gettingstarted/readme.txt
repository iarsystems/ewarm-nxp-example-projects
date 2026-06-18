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
  to develop code for the Phytec LPC3250 evaluation board. It shows
  basic use of CP15, I/O, timer, uart and the interrupt controllers.
  It starts by blinking LED_D15 and LED_D18. The button BTN1 change
  blinking speed while the button BTN2 change the pattern.  The button
  action will be displayed to the U5 BD9 connector at 15200-baud, 8-bit,
  1-stop and no parity/flow control.
   The jumpers settings are regarding Phytec LPC3250 user manual (default)

COMPATIBILITY
=============

   The LEDs example project is compatible with, on the Phytec LPC3250
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

  1) Make sure that the jumpers settings are regarding Phytec LPC3180
    user manual.

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC3250\Phytec\GettingStarted\GettingStarted.eww

  3) Run the program.

     The GettingStarted application is downloaded to the iRAM, xRAM or NAND memory
   on the evaluation board and executed.
  Buttons:
   B1 - Change LED blinking speed
   B2 - Change pattern of LED blinking
