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
 to develop code for IAR-LP1768-SK board.
  It implements a MMC/SD card drive. The first free drive letters will be
 used. For example, if your PC configuration includes two hard disk partitions
 (in C:\ and D:\) and a CD-ROM drive (in E:\), the memory card drive will
 appear as F:\.
  The LCD backlight will indicate drive activity.
  The example handles Standard SD and SDHD cards, but not SDXC cards.

COMPATIBILITY
=============

  The MassStorage example project is compatible with, and works as a
  standalone project, on IAR-LP1768-SK evaluation board. By default,
  the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============

   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect. In this canse adaptive
  clocking should be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   Make sure that the following jumpers are correctly configured on the
  IAR LPC-1768-SK evaluation board:

  PWR_SEL     - depending of power source
  USBD-       - USB Device (1-2)  
  USBD+       - USB Device (1-2)  
  RST_E       - unfilled
  ISP_E       - unfilled

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\LPC17xx\IAR-LPC-1768-SK\
     MassStorage\MassStorage.eww

  3) Run the program.

  4) Use a USB cable to connect the evaluation board to your PC. The first
     time a USB device is connected to your computer, Windows will
     automatically load the proper device driver. In this case, the USB
     mass storage device driver will be loaded.

     The MassStorage application is downloaded to the flash memory on the
     evaluation board and executed. Also note that, during board operation,
     the LCD backlight will indicate drive activity.
