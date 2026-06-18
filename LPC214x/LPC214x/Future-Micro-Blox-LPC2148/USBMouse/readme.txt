########################################################################
#
#                           USBMouse.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR Future-Micro-Blox-LPC2148 board. It 
  implements USB HID mouse. When the host install the needed driver the 
  mouse cursor begins to move in rectangle shape. The WAKE-UP button is 
  used for USB resume when device is suspended.

COMPATIBILITY
=============

   The example project is compatible with Future-Micro-Blox-LPC2148 evaluation 
 board.
   By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============

   Make sure that the following jumpers are correctly configured on the
  Future-Micro-Blox-LPC2148 evaluation board:

  On Main board
  J13 - 2-3
  J14 - 2-3
  J48 - filled
  J49 - 3-4
  J50 - 5-6
  J58 - 1-2
  J59 - 1-2
 
  On CPU board
  J3  - 1-2
  J4  - 1-2
  J24 - filled
 
GETTING STARTED
===============
  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC214x\Future-Micro-Blox-LPC2148\USBMouse\USBMouse.eww

  3) Run the program.

  4) Use a USB cable to connect the evaluation board to your PC. The first
     time a USB device is connected to your computer, Windows will
     automatically load the proper device driver. In this case, the USB
     HID mouse device driver will be loaded.
