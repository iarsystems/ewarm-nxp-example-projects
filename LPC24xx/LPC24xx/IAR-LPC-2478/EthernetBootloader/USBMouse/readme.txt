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

  This example is linked at address 0x00002000 and should be use with
  LPC2300 Ethernet Bootloader.

COMPATIBILITY
=============
   The example project is compatible with the IAR LPC2478-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

   Make sure that the following jumpers are correctly configured on the
  IAR LPC2478-SK evaluation board:

  Jumpers:
   EXT/JLINK  - depending of power source
   ISP_E      - unfilled
   RST_E      - unfilled
   BDS_E      - unfilled
   C/SC       - SC

GETTING STARTED
===============

  
  1) Select File->Open->Workspace...
     Open the following workspace:

     My Documents\IAR Embedded Workbench\arm\examples\NXP\
     LPC24xx\IAR-LPC-2478\EthernetBootloader\EthernetBootloader.eww

  2) Select USBMouse projec.

  3) Press F7 or use make button to build the project.

  4) Switch to Bootloader project.

  5) Power on the board and connect the J-Linck.

  6) Press Ctrl+1 or use "Download active application" to build the project and 
     download it to the board.

  7) Reset the board and meanwhile hold the B1 pressed. The Bootloader is now 
     started and USB_UP_LED1 will be on.
  
  8) Connect a LAN cable to the board. The LED will blink.

  9) Start Ethernet Flash Utility program on PC connected to the same LAN.

 10) Configure the Ethernet Flash Utility program. Basically you should set the 
     MAC address and IP address (Please read NXP application note AN10744 for 
     more details).

 11) From flash loader section for the Utility program browse to 
        My Documents\IAR Embedded Workbench\arm\examples
        \NXP\LPC24xx\IAR-LPC-2478\EthernetBootloader\
        \USBMouse\Flash Debug\Exe\USBMouse.hex
 12) Press "Upload to Flash button". The program is now downloaded to the 
     microcontroller's flash.

 13) Reset the board. The USBMouse example starts.
