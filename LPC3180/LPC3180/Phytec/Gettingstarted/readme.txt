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
  to develop code for a Phytec LPC3180 evaluation board. It shows
  basic use of CP15, I/O, timer and the interrupt controllers.
   It starts by blinking LED_D305 and LED_D310. The button S302 change
  blinking speed.
   The jumpers settings are regarding Phytec LPC3180 user manual (default)

   For Relase iRAM boot from NAND and xRAM boot from NAND is used
  FlashPhytecLPC3180_NAND.flash flashloader. The flashlodare is in
  <installation-root>\arm\config\flashloader\NXP.

COMPATIBILITY
=============

   The LEDs example project is compatible with, on Phytec LPC3180
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
     LPC3180\Phytec\GettingStarted\GettingStarted.eww

  3) Run the program.

     The GettingStarted application is downloaded to the iRAM, xRAM or NAND memory
   on the evaluation board and executed.
  Buttons:
   S302 - Change LED blinking speed
