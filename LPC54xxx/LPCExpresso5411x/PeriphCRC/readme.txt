########################################################################
#
#                          periph_crc.eww
#
# $Revision: 28 $
#
########################################################################

periph_crc

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for LPCXpresso5411x board. The CRC example demonstrates
  using the CRC engine for 8-bit, 16-bit, and 32-bit CRC computation.
  The CRC engine will continuously run via CRC computations and verify the
  CRC checksum. The system tick is used to introduce CRC errors
  into the expected data every other cycle. The board LED will turn on for
  an error and turn off for no errors.
  

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