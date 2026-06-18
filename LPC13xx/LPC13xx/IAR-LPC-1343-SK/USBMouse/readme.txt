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
 to develop code for IAR-LPC1343-SK board. It implements USB HID mouse.
 The mouse slowly moves, B1 is the left mouse button and B2 is the right
 mouse button.

 Controls:
  BUT1        - left button
  BUT2        - right button


COMPATIBILITY
=============
  The example project is compatible with IAR-LPC1343-SK evaluation board.
  By default, the project is configured to use the J-Link SWD interface.

CONFIGURATION
=============
  Jumpers:
   RST_E   - unfilled
   ISP_E   - unfilled
   TMS/SWD - SWD (2-3)
   TDO/SWO - SWO (2-3)

   J-Link_D - filled - to use with an external J-Link
            - unfilled - to use with the on board J-Link

GETTING STARTED
===============

  1) Build and download the program.

  2) Run the program.

  3) Use a USB cable to connect the evaluation board to your PC. The first
     time a USB device is connected to your computer, Windows will
     automatically load the proper device driver. In this case, the USB
     HID mouse device driver will be loaded.
