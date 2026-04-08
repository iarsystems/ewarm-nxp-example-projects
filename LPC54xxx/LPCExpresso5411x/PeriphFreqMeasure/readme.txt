########################################################################
#
#                          periph_freqmeasure.eww
#
# $Revision: 28 $
#
########################################################################

periph_freqmeasure

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for LPCXpresso5411x board. The example demonstrates how to
  measure a target frequency using a (faster) reference frequency. The
  example uses the internal main clock as the reference frequency to measure
  the frequencies of the RTC, watchdog oscillator, and internal RC oscillator.
  If CLKOUT_SEL is defined a clock source will be output to P0_21 (J2's Pin2).
  

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
     Optionaly connect an oscilloscope to J2's Pin2.
  
  3) Open a terminal program with the following settings: 115200 bauds,
  8 data bits, 1 stop bits, no parity.
   
  4) Run the program.
     Observe the ouput in the terminal program and on the oscilloscope if 
     it is connected.