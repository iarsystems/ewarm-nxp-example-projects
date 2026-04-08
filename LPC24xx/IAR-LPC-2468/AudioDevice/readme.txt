########################################################################
#
#                           AudioDevice.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for IAR LPC-2468 board.
   It implements usb audio device with one output.

COMPATIBILITY
=============
   The example project is compatible with, on IAR LPC-2468
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============
   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

     The GettingStarted application is downloaded to the iFlash or iRAM memory
  depending of the project's configuration and executed.

   Make sure that the following jumpers are correctly configured on the
  IAR LPC-2468 evaluation board:

  Jumpers:
   J5      - depending of power source
   ISP     - unfilled
   nRESET  - unfilled
   EINT0   - filled

  The LEDs share pins with Trace port. If ETM is enabled the LEDs can not be used. 

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC24xx\IAR-LPC-2468\AudioDevice\AudioDevice.eww

  3) Connect a headset to HEADPHONE connectors of the IAR LPC-P2468
     board.

  4) Run the program.

  5) use the USB cable to connect the PC to USB of the Board
     The first time the device is connected to the computer, Windows will load the
     driver for identified device. The USB Audio Device driver will be automatically
     loaded.