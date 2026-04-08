########################################################################
#
#                           USBMassStorage.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
=============
  This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the Embedded Artists LPC3250 evaluation board.
  It implements a RAM based 15MB drive. The first available drive letter
  will be used. For example, if your PC configuration includes two hard disk
  partitions (C: and D:) and a CD-ROM drive (E:), the memory card drive will
  appear as F:.
  
  A LED will indicate drive activity.


COMPATIBILITY
=============
  This example project is compatible with Embedded Artsists LPC3250 
  evaluation board. By default the project is configured to use the 
  IAR J-Link JTAG interface.


CONFIGURATION
=============
  Connect a USB cable between the USB host port (not the USB device port!)
  on the board and a USB host port on the computer.
  The first time a USB device is connected to your computer, Windows will
  automatically load the proper device driver. In this case, the USB
  mass storage device driver will be loaded.

  The Project contains the following configurations:

  Debug_xRAM: This configuration is intended for debugging in the
              external SDRAM.

  Release_xRAM_boot_from_NAND:
              This configuration will download the example and the bootloader
              in the NAND flash. It is intended for example stand alone
              execution.

              Note: This will overwrite the S1L and Kickstart bootloaders
              that are shipped with the board.
 
 
GETTING STARTED
===============
  1) Follow the steps in the Getting Started Guide to open the example project from Information Center.

  2) Select proper configuration.

  3) Build and download the program.

  4) Run the program 
