########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
   This is a port of the NXP Common Driver Library (CDL) Package version 1.03 and EALPS3250 examples for use with
   IAR's EWARM. 
   This port does not include the S1L and the Kickstart bootloaders. Instead it has a simple bootloader which allows stand alone execution
   of the examples.

COMPATIBILITY
=============
   The examples are compatible with Embedded Artists LPC3250 evaluation board.
   By default, the project is configured to use the
   J-Link JTAG interface.

   See lpc32xx_readme.txt and CDL docs. You can download them from NXP website.

CONFIGURATIONS
=============

 Each example has the following configurations
 
   Debug_iRAM: This configuration is intended for debugging in the microcontroller's internal RAM.

   Debud_xRAM: This configuration is intended for debugging in the external SDRAM.

   Release_xRAM_boot_from_NAND: This confuguration will download the example and the bootloader in
                                the NAND flash. It is intended for example stand alone execution.
                                Note: This will overwrite the S1L and Kickstart bootloaders that are
                                shipped with the board

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\LPC3250\nxpmcu\software\csps\lpc32xx\bsps\ea3250\examples\LPC3250.eww

  3) Select an example and configuration.

  4) Power the board and connect the J-Link.
 
  5) Press CTRL+D or use Download and Debug button to start a debug session.

