########################################################################
#
#                           gpio.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
   This project is a simple General Purpose Input/Output (GPIO) example.
  It will turn-on LED E4 and blink LED E3. Pressing the push buttons on 
  the TWR_K70F120M will cause E1 and E2 to light on and off as well.

COMPATIBILITY
=============
   The example project is compatible with Freescale TWR-K70F120M board.
  By default, the project is configured to use the J-Link SWD interface.

CONFIGURATION
=============
   The application is downloaded to the internal Flash or internal RAM,
  depending on selected configuration and executed.

   If you use TWR-K70F120M without TWR-ELEV and TWR-SER boards, you can
  use USB Virtual serial port at OSJTAG/OSBDM interface (virtual Serial
  toolkit from www.pemicro.com must be installed).
   If your choice is serial port at TWR-SER board, any standard terminal
  software can be used.
  Termnal settings must be 115200 8-N-1.

GETTING STARTED
===============
   1) Select configuration.

   2) Build and download the program.
