############################################################################
#
#                           LPC3131.eww
#
# $Revision: 28 $
#
############################################################################

DESCRIPTION
===========
   This example projects shows how to use the IAR Embedded Workbench for ARM
  and the NXP Common Driver Library (CDL) to develop code for 
  Embedded Artsists LPC313x evaluation board. 
   
COMPATIBILITY
=============

   The examples are compatible with Embedded Artists LPC313x evaluation board.
   By default, the project is configured to use the
   J-Link JTAG interface.

CONFIGURATION
=============

Each example has the following configurations

  Debug_iRAM: This configuration is intended for debugging in the
              microcontroller's internal RAM.

  Debug_SPINOR_Boot_iRAM: This confuguration will download the example in the
                          SPI NOR flash. The application is linked to run in
                          the internal RAM and is loaded by LPC IROM
                          bootloader. The configuration uses
                          FlashEmbArtLPC313x_SPINOR_BOOT.flash flash loader.
                          Note: This configuration uses a Hardware reset with
                          delay to start IROM bootloader and allow it to copy
                          the application. The boot jumpers should be
                          configured for SPI NOR flash boot.
                                                    
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>arm\examples\NXP\LPC_CDL\software\
     csps\lpc313x\bsps\ea3131\examples\LPC313x.eww

  3) Select configuration.

  4) Power the board and connect the J-Link.
 
  5) Press CTRL+D or use Download and Debug button to start a debug session. 