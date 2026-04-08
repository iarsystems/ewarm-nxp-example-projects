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
 to develop code for IAR-LPC11U14-SK board. It implements USB HID mouse.
  The USER and WKUP button control horizontal movement and the AN_TR control
 vertical movement

 Controls:
  USER        - left
  WAKE-UP     - right
  AN_TRIM     - up/down

COMPATIBILITY
=============
   The example project is compatible with IAR-LPC11U14-SK evaluation board.
   By default, the project is configured to use the J-Link SWD interface.

CONFIGURATION
=============
   Make sure that the following jumpers are correctly configured on the
  IAR-LPC11U14-SK evaluation board:

  Jumpers:
   RST_E     - unfilled
   ISP_E     - unfilled
   PIO1_LOW  - unfilled
   PIO3_HIGH - unfilled
   PIO3_E    - filled
   TMS/SWD   - SWD (2-3)

  The USB HID mouse application is downloaded to the iFlash.

GETTING STARTED
===============

  1) Build and download the program.

  2) Run the program.

  3) Use a USB cable to connect the evaluation board to your PC. The first
     time a USB device is connected to your computer, Windows will
     automatically load the proper device driver. In this case, the USB
     HID mouse device driver will be loaded.
