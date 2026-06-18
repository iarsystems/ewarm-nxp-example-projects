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
 to develop code for IAR-LPC1768-SK board.
   It implements usb audio device class with one output and one input.


COMPATIBILITY
=============

   The example project is compatible with, on IAR-LPC1768-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect,In this case adaptive 
  clocking should be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

  Jumpers:
   PWR_SEL     - depending of power source
   USBD-       - USB Device (1-2)  
   USBD+       - USB Device (1-2)  
   RST_E       - unfilled
   ISP_E       - unfilled

   The AudioDevice application is downloaded to the iFlash memory and executed.

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC17xx\IAR-LPC-1768-SK\AudioDevice\AudioDevice.eww

  3) Connect a headset to MIC and HEADPHONE connectors of the IAR LPC-1768-SK
     board.

  4) Run the program.

  5) use the USB cable to connect the PC to USB of the Board
     The first time the device is connected to the computer, Windows will load the
     driver for identified device. The USB Audio Device driver will be automatically
     loaded.