############################################################################
#
#                           MCB2300.eww
#
# $Revision: 28 $
#
############################################################################

DESCRIPTION
===========
   This example projects shows how to use the IAR Embedded Workbench for ARM
  and the NXP Common Driver Library (CDL) to develop code for 
  Kiel MCB2300 board. 
   
COMPATIBILITY
=============

   The examples are compatible with Kiel MCB2300 evaluation board.
   By default, the project is configured to use the
   J-Link JTAG interface.

CONFIGURATION
=============

Each example has the following configurations

  FLASH: This configuration is intended for debugging in the
         microcontroller's internal Flash memory.

  RAM:   This configuration is intended for debugging in the
         microcontroller's internal RAM.
                                                    
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>arm\examples\NXP\LPC_CDL\software\
     csps\lpc23xx\bsps\mcb2300\examples\mcb2300.eww

  3) Select configuration.

  4) Power the board and connect the J-Link.
 
  5) Press CTRL+D or use Download and Debug button to start a debug session. 