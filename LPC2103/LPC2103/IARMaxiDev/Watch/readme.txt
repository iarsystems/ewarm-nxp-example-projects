########################################################################
#
#                           Watch.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========

  This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for IAR-LPC-2103 Kickstart evaluation board. It 
  implement watch with date. The Watch example project works standalone on 
  IAR-LPC-2103 Kickstart board.

COMPATIBILITY
=============

  The Watch example project is compatible with, and works as a
  standalone project, on IAR-LPC-2103 Kickstart board. By default,
  the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============

  1) Make sure that the following jumpers are correctly configured on the
     IAR-LPC-2103 Kickstart board:
        EXT/JLINK - the correct jumper position depends on the power source used
        DBG       - install the jumper
 	INT0      - install the jumper
 	INT1      - install the jumper
 	INT2      - install the jumper
        
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\LPC2103\IARMaxiDev\Watch\Watch.eww

  3) Run the program.

     The Watch application is downloaded to the flash memory on the
     evaluation board and executed. 
  Buttons:
   INT0 - Decrement/Backlight On
   INT1 - Correct time/Backlight On
   INT2 - Increment/Backlight On
	
