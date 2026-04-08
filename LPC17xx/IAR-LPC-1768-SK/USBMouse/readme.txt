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
 to develop code for IAR-LPC1768-SK board. It implements USB HID mouse.
  The joystick control up, down, left, right and left mouse button, B1
 is the left mouse button too and B2 is the right mouse button.
 Controls:
  Joystick    - left, right, up, down movement, center left button
  BUT1        - left button
  BUT2        - right button


COMPATIBILITY
=============

   The example project is compatible with IAR-LPC1768-SK evaluation board.
   By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============

   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect. In this case adaptive
  clocking should be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   Make sure that the following jumpers are correctly configured on the
  IAR-LPC1768-SK evaluation board:

  PWR_SEL     - depending of power source
  USBD-       - USB Device (1-2)  
  USBD+       - USB Device (1-2)  
  RST_E       - unfilled
  ISP_E       - unfilled

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC17xx\IAR-LPC-1768-SK\USBMouse\USBMouse.eww

  3) Run the program.

  4) Use a USB cable to connect the evaluation board to your PC. The first
     time a USB device is connected to your computer, Windows will
     automatically load the proper device driver. In this case, the USB
     HID mouse device driver will be loaded.
