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
 to develop code for IAR-LPC-1768-SK board. It shows basic use of I/O,
 timer, interrupt and LCD controllers and USB Host Masstorage class 
 framework.

  This example demonstrates how to access the files on "USB Flash drive" 
 connected to USB Host port on IAR-LPC-1768-SK board. 
 
COMPATIBILITY
=============
   The USB_HostDemo project is compatible with IAR-LPC-1768-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   The USB_HostDemo application is downloaded to the iFlash.
  
  Jumpers:
    Power Select - depending of power source
    USB D-       - pin 3-4
    USB D+       - pin 3-4
    Debug Enable - filled
    ISP Enable   - unfilled
    Reset Enable - unfilled
  
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

      <installation-root>\arm\examples\NXP\
     LPC17xx\IAR-LPC-1768-SK\USB_Host_Demo\USB_HostDemo.eww

  3) Run the program.
  
  4) Connect a USB flash drive at USB host port. The flash drive should be FAT16 or
     FAT32 formatted and MSREAD.TXT must be present on the device (usb flash drive).
     