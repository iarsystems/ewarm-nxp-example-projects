########################################################################
#
#                          periph_iap.eww
#
# $Revision: 28 $
#
########################################################################

periph_iap

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for LPCXpresso5411x board. The example demonstrates 
  programming a FLASH block during run-time. For this example, the code is
  running from FLASH and a FLASH block not used for the executing code will
  be erased and programmed with some data. The example also toggles the LED
  in the systick interrupt. The interrupts need to be disabled during the IAP
  calls that change FLASH data and re-enabled after the calls are complete.
  

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
  
  3) Open a terminal program with the following settings: 115200 bauds,
  8 data bits, 1 stop bits, no parity.
   
  4) Run the program.
     Observe the ouput in the terminal program.