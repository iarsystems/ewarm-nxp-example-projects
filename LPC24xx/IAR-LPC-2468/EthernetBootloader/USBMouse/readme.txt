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
  to develop code for IAR LPC-2468 board. It implements USB HID mouse.
   When host install needed driver a mouse's cursor begin moved
  in rectangle shape. The WAKE-UP button is used for USB resume when device
  is suspended.

   Controls:
    K1 - USB resume, when device is suspended


  This example is linked at address 0x00002000 and should be use with
  LPC2300 Ethernet Bootloader.

COMPATIBILITY
=============
   The example project is compatible with, on IAR LPC-2468
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

   Make sure that the following jumpers are correctly configured on the
  IAR LPC-2468 evaluation board:

  Jumpers:
   J5      - depending of power source
   ISP     - unfilled
   nRESET  - unfilled
   EINT0   - filled

GETTING STARTED
===============

  1) Select File->Open->Workspace...
     Open the following workspace:

     My Documents\IAR Embedded Workbench\arm\examples\NXP\
     LPC24xx\IAR-LPC-2468\EthernetBootloader\EthernetBootloader.eww

  2) Select USBMouse projec.

  3) Press F7 or use make button to build the project.

  4) Switch to Bootloader project.

  5) Power on the board and connect the J-Linck.

  6) Press Ctrl+1 or use "Download active application" to build the project and 
     download it to the board.

  7) Reset the board and meanwhile hold the K4 pressed. The Bootloader is now 
     started and the LED connected to P2.0 will be on.
  
  8) Connect a LAN cable to the board. The LED will blink.

  9) Start Ethernet Flash Utility program on PC connected to the same LAN.

 10) Configure the Ethernet Flash Utility program. Basically you should set the 
     MAC address and IP address (Please read NXP application note AN10744 for 
     more details).

 11) From flash loader section for the Utility program browse to 
        My Documents\IAR Embedded Workbench\arm\examples
        \NXP\LPC24xx\IAR-LPC-2468\EthernetBootloader\
        \USBMouse\Flash Debug\Exe\USBMouse.hex
 12) Press "Upload to Flash button". The program is now downloaded to the 
     microcontroller's flash.

 13) Reset the board. The USBMouse example starts.
