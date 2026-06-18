########################################################################
#
#                          periph_blinky.eww
#
# $Revision: 28 $
#
########################################################################

periph_blinky

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for LPCXpresso5411x board. The example uses an ISR to blink
  the LED on  P0_29. The ISR changes the LED state 10 times in a second.

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
     Observe the blinking LED.