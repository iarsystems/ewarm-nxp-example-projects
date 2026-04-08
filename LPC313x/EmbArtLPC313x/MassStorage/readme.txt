############################################################################
#
#                           MassStorage.eww
#
# $Revision: 28 $
#
############################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for for a Embedded Artsists LPC313x evaluation board.
  It implements a RAM based 32MB drive. The first free drive letters will be
 used. For example, if your PC configuration includes two hard disk partitions
 (in C:\ and D:\) and a CD-ROM drive (in E:\), the memory card drive will
 appear as F:\.
  The GPIO_BOOT2 LED will indicate drive activity.

COMPATIBILITY
=============
   The USB Mass storage example project is compatible with Embedded Artsists
  LPC313x evaluation board. By default the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

The Project contains the following configurations:

  Debud_SDRAM: This configuration is intended for debugging in the external
               SDRAM.

 Debug_SPINOR_Boot_SDRAM: This confuguration will download the example in the
                          SPI NOR flash. The application is linked to run in
                          the external SDRAM and a second level bootloader is
                          linked to run in the internal RAM. The IROM
                          bootloader copies the secon level bootloader from
                          the SPI NOR to the interan RAM and starts it.
                          The second level bootloader copies the rest of the
                          application to SDRAM. The configuration uses
                          FlashEmbArtLPC313x_SPINOR_BOOT.flash flash loader to
                          program the secon level bootloader and
                          FlashEmbArtLPC313x_SPINOR_IMG.flash flash loader to
                          program the application.

                          Note: This configuration uses a Hardware reset with
                          delay to start IROM bootloader and allow it to copy
                          the second level bootloader. The boot jumpers should
                          be configured for SPI NOR flash boot.

 Jumpers:
  (Boot from SPI Flash)
  BOOT0      - L
  BOOT1      - L
  BOOT2      - H
                          
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\LPC313x\EmbArtLPC313x\
     MassStorage\MassStorage.eww

  3) Select configuration.

  4) Power the board and connect the J-Link.

  5) Press CTRL+D or use Download and Debug button to start a debug session.

  6) Use a USB cable to connect the PC to the USB-dev connector of the Board.
     The first time a USB device is connected to your computer, Windows will
     automatically load the proper device driver. In this case, the USB
     mass storage device driver will be loaded.