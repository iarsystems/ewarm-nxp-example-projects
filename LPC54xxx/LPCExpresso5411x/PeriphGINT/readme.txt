########################################################################
#
#                          periph_gint.eww
#
# $Revision: 28 $
#
########################################################################

periph_gint

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for LPCXpresso5411x board. The example demonstrates the
  Grouped GPIO interrupt feature. This example configures Grouped GPIO
  interrupt 0 to be invoked when both the buttons SW1-WAKE and SW2-ISP are 
  pressed by the user. The interrupt toggles the state of LED 0.
  

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
  
  3) Press both SW1-WAKE and SW2-ISP and observe LED 0.