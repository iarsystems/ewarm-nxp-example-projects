########################################################################
#
#                           MassStorage.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for IAR LPC2478-SK board.
  It implements a MMC/SD card drive. The first free drive letter will be
 used. For example, if your PC configuration includes two hard disk partitions
 (in C:\ and D:\) and a CD-ROM drive (in E:\), the memory card drive will
 appear as F:\.
  The USB Host LED will indicate drive activity.


COMPATIBILITY
=============
   The example project is compatible with the IAR LPC2478-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============
   After power-up the controller gets it's clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   Make sure that the following jumpers are correctly configured on the
  IAR LPC2478-SK evaluation board:

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
     LPC24xx\IAR-LPC-2478\MassStorage\MassStorage.eww

  3) Run the program.

  4) Use a USB cable to connect the PC to the USB-dev connector of the board.
     The first time a USB device is connected to your computer, Windows will
     automatically load the proper device driver. In this case, the USB
     mass storage device driver will be loaded.

     The MassStorage application is downloaded to the memory on the
     evaluation board and executed. Also note that, during board operation,
     the USB Host LED will indicate drive activity.
