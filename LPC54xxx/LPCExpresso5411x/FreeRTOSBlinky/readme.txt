########################################################################
#
#                          freertos_blinky.eww
#
# $Revision: 28 $
#
########################################################################

FreeRTOSBlinky

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for LPCXpresso5411x board. The example starts up
  FreeRTOS and creates 3 tasks. Two of the tasks blink the LEDs on P0_29 and 
  P1_10. The third task prints a message via UART if DEBUG_ENABLE and 
  DEBUG_UART are defined (They are defined by default and the UART0 is used). 

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

  3) Run the program.
     Observe the blinking LEDs (they are in one body) and the output in
     the terminal program.