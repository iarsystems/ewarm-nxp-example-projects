########################################################################
#
#                           USB_HostDemo.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for IAR LPC2478-SK board. It shows basic use of I/O,
 timer, interrupt and LCD controllers and USB Host Mass storage class 
 framework.

  This example demonstrates how to access the files on a "USB Flash drive" 
 connected to the USB Host connector on the IAR LPC2478-SK board. 
 
COMPATIBILITY
=============

   The USB_HostDemo project is compatible with IAR LPC2478SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
  
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
     LPC24xx\IAR-LPC-2478\USB_Host_Demo\USB_HostDemo.eww

  3) Run the program.
  
  4) Connect a USB flash drive to the USB host connector. The flash drive should be FAT16 or
     FAT32 formatted and a file named MSREAD.TXT must be present on the device (usb flash drive).
     
     The application creates a copy of MSREAD.TXT named MSWRITE.TXT.
