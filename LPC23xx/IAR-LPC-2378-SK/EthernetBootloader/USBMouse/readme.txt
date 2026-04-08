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
 to develop code for IAR LPC-2378-SK board. It implements USB HID mouse.
  The joystick control up, down, left, right and left mouse button, B1
 is the left mouse button too and B2 is the right mouse button.
 Controls:
  Joystick    - left, right, up, down movement, center left button
  BUT1        - left button
  BUT2        - right button

  This example is linked at address 0x00002000 and should be use with
  LPC2300 Ethernet Bootloader.

COMPATIBILITY
=============

   The example project is compatible with, on IAR LPC-2378-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
   If Port 2 bits 0 - 9 are used by application PINSEL10 must be cleared and
  this will disable trace port too.

   Make sure that the following jumpers are correctly configured on the
  IAR LPC-2378-SK evaluation board:

  PWR_SEL     - depending of power source
  USBC/USBSC  - USBSC
  RST_E       - unfilled
  ISP_E       - unfilled

GETTING STARTED
===============

  1) Select File->Open->Workspace...
     Open the following workspace:

     My Documents\IAR Embedded Workbench\arm\examples\NXP\
     LPC24xx\IAR-LPC-2378\EthernetBootloader\EthernetBootloader.eww

  2) Select USBMouse projec.

  3) Press F7 or use make button to build the project.

  4) Switch to Bootloader project.

  5) Power on the board and connect the J-Linck.

  6) Press Ctrl+1 or use "Download active application" to build the project and 
     download it to the board.

  7) Reset the board and meanwhile hold the B1 pressed. The Bootloader is now 
     started and USB_LINK will be on.
  
  8) Connect a LAN cable to the board. The LED will blink.

  9) Start Ethernet Flash Utility program on PC connected to the same LAN.

 10) Configure the Ethernet Flash Utility program. Basically you should set the 
     MAC address and IP address (Please read NXP application note AN10744 for 
     more details).

 11) From flash loader section for the Utility program browse to 
        My Documents\IAR Embedded Workbench\arm\examples
        \NXP\LPC24xx\IAR-LPC-2378\EthernetBootloader\
        \USBMouse\Flash Debug\Exe\USBMouse.hex
 12) Press "Upload to Flash button". The program is now downloaded to the 
     microcontroller's flash.

 13) Reset the board. The USBMouse example starts.
