########################################################################
#
#                          periph_clkout.eww
#
# $Revision: 28 $
#
########################################################################

periph_clkout

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for LPCXpresso5411x board. The example demonstrates how to 
  select a clock output and divider to route to the CLKOUT signal. When a 
  keyboard key is pressed while opened properly configured terminal program,
  the clock out is changed to the next available source.
  

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

  2) Connect USB to serial cable to a PC and to the board J5's TX, RX and GND. 
     Connect an oscilloscope to J2's pin 2.
  
  3) Open a terminal program with the following settings: 115200 bauds,
  8 data bits, 1 stop bits, no parity.
   
  4) Run the program.
     Observe the ouput of the oscilloscope and the terminal program.