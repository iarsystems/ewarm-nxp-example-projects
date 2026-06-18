########################################################################
#
#                           MassStorage.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use IAR Embedded Workbench for ARM
 to develop code for the Micro-Blox LPC2478 board.
  It implements a MMC/SD card drive. The first free drive letters will be
 used. For example, if your PC configuration includes two hard disk partitions
 (in C:\ and D:\) and a CD-ROM drive (in E:\), the memory card drive will
 appear as F:\.


COMPATIBILITY
=============
   The example project is compatible with, on Micro-Blox LPC2478
  board. By default, the project is configured to use the J-Link JTAG 
  interface.

CONFIGURATION
=============
   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   The USB mass storage device class application is downloaded to the iFlash memory
  and executed.

   Make sure that the following jumpers are correctly configured on the
  Micro-Blox LPC2478 board:

  Jumpers:
   On Main board
    J13 - 2-3
    J14 - 2-3
    J48 - filled
    J49 - 3-4
    J50 - 5-6
    J58 - 1-2
    J59 - 1-2

   On CPU board
    J1  - 1-2
    J3  - 1-2
    J7  - filled
    J24 - 2-3
    J26 - 2-3
    J31 - 2-3
    J37 - filled

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC24xx\Micro-Blox-LPC2478\MassStorage\MassStorage.eww

  3) Run the program.

  4) Use a USB cable to connect the PC to the USB-dev connector of the Board.
     The first time a USB device is connected to your computer, Windows will
     automatically load the proper device driver. In this case, the USB
     mass storage device driver will be loaded.

     The MassStorage application is downloaded to the flash memory on the
     evaluation board and executed. 
