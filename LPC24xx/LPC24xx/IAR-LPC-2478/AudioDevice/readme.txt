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
 to develop code for the IAR LPC2478-SK board.
   It implements a usb audio device with one output, touch screen and sound
 effects loudness and wide stereo.

COMPATIBILITY
=============
   The example project is compatible with the IAR LPC2478-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============
   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   Make sure that the following jumpers are correctly configured on the
  IAR LPC2478-SK evaluation board:

  Jumpers:
   EXT/JLINK  - depending of power source
   ISP_E      - unfilled
   RST_E      - unfilled
   BDS_E      - unfilled
   C/SC       - SC

   The LCD shares pins with Trace port. If ETM is enabled the LCD will not work.

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC24xx\IAR-LPC-2478\AudioDevice\AudioDevice.eww

  3) Run the program.

  4) Use a USB cable to connect the PC to the USB Device connector of the board.
     The first time the device is connected to the computer, Windows will load the
     driver for identified device. The USB Audio Device driver will be automatically
     loaded.