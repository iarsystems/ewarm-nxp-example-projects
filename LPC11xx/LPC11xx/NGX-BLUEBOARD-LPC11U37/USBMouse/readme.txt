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
 to develop code for NGX-BLUEBOARD-LPC11U37 board. It implements USB HID mouse.
  The EXTERNAL and WAKEUP buttons control the horizontal movement and the R14
 controls the vertical movement of mouse cursor.

 Controls:
  EXTERNAL  - left
  WAKEUP    - right
  R14       - up/down

COMPATIBILITY
=============
   The example project is compatible with NGX-BLUEBOARD-LPC11U37 board.
   By default, the project is configured to use the J-Link SWD interface.

CONFIGURATION
=============

  The USB HID mouse application is downloaded to the iFlash.

GETTING STARTED
===============

  1) Build and download the program.

  2) Run the program.

  3) Use a USB cable to connect the evaluation board to your PC. The first
     time a USB device is connected to your computer, Windows will
     automatically load the proper device driver. In this case, the USB
     HID mouse device driver will be loaded.
