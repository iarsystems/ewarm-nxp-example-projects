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
 to develop code for IAR-LPC1347-SK board. It implements USB HID mouse.
 The mouse slowly moves, BUT1 is the left mouse button and BUT2 is the right
 mouse button.
 Controls:
  BUT1        - left button
  BUT2        - right button


COMPATIBILITY
=============

   The example project is compatible with IAR-LPC1347-SK evaluation board.
   By default, the project is configured to use the J-Link SWD interface.

CONFIGURATION
=============
  Jumpers:
   RST_E   - unfilled
   ISP_E   - unfilled
   TMS/SWV   - SWV (2-3) 
   TDO/SWDIO - SWDIO (2-3) 

GETTING STARTED
===============

  1) Build and download the program.

  3) Run the program.

  4) Use a USB cable to connect the evaluation board to your PC. The first
     time a USB device is connected to your computer, Windows will
     automatically load the proper device driver. In this case, the USB
     HID mouse device driver will be loaded.
