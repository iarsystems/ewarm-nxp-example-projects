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
 to develop code for Micro-Blox LPC2478 board.
   It implements usb audio device with one output.

COMPATIBILITY
=============
   The example project is compatible with, on Micro-Blox LPC2478 board.
   By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============
   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   The USB audio class device application is downloaded to the iFlash memory
  and executed.

   Make sure that the following jumpers are correctly configured on the
   Micro-Blox LPC2478 board:

  Jumpers:
    On Main board
     J13 - 2-3
     J14 - 2-3
     J16 - 7-8
     J16 - 9-10
     J16 - 11-12
     J16 - 13-14
     J16 - 15-16
     J16 - 17-18
     J16 - 19-20
     J17 - 2-3
     J18 - 1-2
     J18 - 3-4
     J18 - 5-6
     J18 - 7-8
     J48 - filled
     J49 - 3-4
     J50 - 5-6
     J58 - 1-2
     J59 - 1-2

    On CPU board
     J1  - 1-2
     J3  - 1-2
     J7  - filled
     J10 - 1-2
     J12 - 1-2
     J13 - 1-2
     J15 - 2-3
     J16 - 1-2
     J18 - 2-3
     J20 - 2-3
     J24 - 2-3
     J26 - 2-3
     J27 - 1-2
     J28 - 2-3
     J31 - 2-3
     J32 - 2-3
     J33 - 2-3
     J34 - 2-3
     J35 - 2-3
     J37 - filled

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC24xx\Micro-Blox-LPC2478\AudioDevice\AudioDevice.eww

  3) Run the program.

  5) Use a USB cable to connect the PC to the USB connector of the Board (J45)
     The first time the device is connected to the computer, Windows will load the
     driver for identified device. The USB Audio Device driver will be automatically
     loaded.