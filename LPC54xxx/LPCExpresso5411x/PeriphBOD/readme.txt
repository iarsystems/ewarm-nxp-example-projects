########################################################################
#
#                          periph_bod.eww
#
# $Revision: 28 $
#
########################################################################

periph_bod

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for LPCXpresso5411x board. The example demonstrates how to 
  detect brown out and how to use interrupt on brown out.
  

COMPATIBILITY
=============
  The example project is compatible with LPCXpresso5411x board.

CONFIGURATION
=============
    The LPCXpresso5411x board must be powered from the Link2 USB connector J7,
    and jumper JP2 must be fitted in position pin 1 - 2 (Local Target). Jumper
    JP1 must be open to enable the target LPC5411x.

GETTING STARTED
===============
  1) Batch Build (F8->Rebuild All) and download the program.

  2) Run the program.
   
  3) Power off the board by removing the power (USB) cable. As the power is
    declining on the board, the LED will toggle on (quickly) and then turn
    off as power is lost.