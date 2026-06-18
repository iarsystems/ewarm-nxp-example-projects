########################################################################
#
#                           USBMouse.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for IAR LPC2478-SK board. It implements USB HID mouse.
   The mouse cursor moves depending of slope of the board.


COMPATIBILITY
=============
   The example project is compatible with the IAR LPC2478-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============
   After power-up the controller gets it's clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   The UAB HID Mouse application is downloaded to the iFlash memory
  and executed.

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
     LPC24xx\IAR-LPC-2478\USBMouse\USBMouse.eww

  3) Run the program.

  4) Use a USB cable to connect the PC to the USB Device connector of the Board.
     The first time a USB device is connected to your computer, Windows will
     automatically load the proper device driver. In this case, the USB
     HID mouse device driver will be loaded.