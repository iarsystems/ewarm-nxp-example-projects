########################################################################
#
#                           adc_demo.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
   This project is a simple Analog to Digital Converter (ADC)  example
  that shows how to trigger the ADC conversions with the Programable
  Delay Block (PDB). The results are displayed on terminal window.

   If you hit any key, the program will end.  Output will be the conversions
  from the ADC, and one filtered output, all on one line. That line will
  update about once a second, as slowly scheduled by the PDB.
   If you do not type anything it will continue forever. This gives you
  a chance to play with the Potentiometer (POT). To vary the conversion
  results seen on the screen, operate the black coin edge of the POT on
  the bottom of the board. See if you can hit every code from zero to 64k-1.
  The codes will be in decimal.

   For further information, please refer to the Kinetis QRUG chapter "Using
  the Peripheral Delay Block (PDB) to schedule conversions of the Analog to
  Digital Converter (ADC) for Kinetis Microcontrollers"

COMPATIBILITY
=============
   The example project is compatible with Freescale TWR-K70F120M board.
  By default, the project is configured to use the J-Link SWD interface.

CONFIGURATION
=============
   The application is downloaded to the internal Flash, internal RAM or
  external DDR2 memory, depending on selected configuration and executed.

   If you use TWR-K70F120M without TWR-ELEV and TWR-SER boards, you can
  use USB Virtual serial port at OSJTAG/OSBDM interface (virtual Serial
  toolkit from www.pemicro.com must be installed).
   If your choice is the serial port at TWR-SER board, any standard terminal
  software can be used.
  Terminal settings must be 115200 8-N-1.

GETTING STARTED
===============
   1) Select configuration.

   2) Build and download the program.
