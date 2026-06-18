############################################################################
#
#                           USBMouse.eww
#
# $Revision: 28 $
#
############################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for a Embedded Artsists LPC314x evaluation board. It implements
 USB HID mouse.
  The joystick control up, down, left, right and left mouse button.
  The projec also shows how the system can boot from the SPI NOR Flash.

COMPATIBILITY
=============

   The USB Mouse example project is compatible with Embedded Artsists
  LPC314x evaluation board. By default the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

The Project contains the following configurations:

  Debug_iRAM: This configuration is intended for debugging in the
              microcontroller's internal RAM.

  Debud_SDRAM: This configuration is intended for debugging in the external
               SDRAM.

  Debug_SPINOR_Boot_iRAM: This confuguration will download the example in the
                          SPI NOR flash. The application is linked to run in
                          the internal RAM and is loaded by LPC IROM
                          bootloader. The configuration uses
                          FlashEmbArtLPC313x_SPINOR_BOOT.flash flash loader.

                          Note: This configuration uses a Hardware reset with
                          delay to start IROM bootloader and allow it to copy
                          the application. The boot jumpers should be
                          configured for SPI NOR flash boot.

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
  BOOT0 - L
  BOOT1 - L
  BOOT2 - H

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\LPC314x\EmbArtLPC314x\USBMouse\USBMouse.eww

  3) Select configuration.

  4) Power the board and connect the J-Link.

  5) Press CTRL+D or use Download and Debug button to start a debug session.