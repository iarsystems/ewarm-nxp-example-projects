############################################################################
#
#                           VirtualCom.eww
#
# $Revision: 1591 $
#
############################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for a Embedded Artsists LPC315x evaluation board.
  It implements USB CDC (Communication Device Class) device and install
 it like a Virtual COM port.
  The projec also shows how the system can boot from the SPI NOR Flash.
  With this example project, you can echo a character between the RS232 
 Physical interface and the Virtual Com Port USB interface. You will need to 
 open two separate (Tera Term or HyperTerm) windows. One for the virtual 
 interface and another for the UART. Set both sessions to 8-N-1 No flow control. 
 Make sure that the virtual com port occupies a port between 1-8. You may have 
 to change this in the Device Manager option under the advanced tab.
  
  Note:
	Before you can use the following example the proper .inf file needs to be 
 installed for your version of windows. Please look in this example project 
 folder a subfolder \VirCOM_Driver_"X"\ . In this you will see the proper .inf file 
 that must be installed when Windows tries to install the generic CDC class driver. 
 Please note that "X" denotes your particular operating system. If it does 
 not install properly, you may have to go into your device manager and look 
 under "other devices" and manually point the driver to the proper .inf file 
 for your operating system. When you have properly installed the driver you 
 will see an entry in your Device Manager|Ports that says IAR Virtual Com Port, 
 "your board"(where "your board is the particular NXP board you are using). 

COMPATIBILITY
=============

   The Virtual COM port example project is compatible with Embedded Artsists
  LPC315x evaluation board. By default the project is configured to use the
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
  BOOT0      - L
  BOOT1      - L
  BOOT2      - H

  USB/UART

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\LPC315x\EmbArtLPC315x\VirtualCom\VirtualCom.eww

  3) Select configuration.

  4) Power the board and connect the J-Link.

  5) Press CTRL+D or use Download and Debug button to start a debug session.
