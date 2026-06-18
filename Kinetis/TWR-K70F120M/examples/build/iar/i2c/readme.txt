########################################################################
#
#                           i2c.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
   This project is a simple I2C example that communicates with the MMA8451Q
  accelerometer on the tower board. It prints out the acceleromer X, Y, and
  Z axis data to the terminal.

COMPATIBILITY
=============
   The example project is compatible with Freescale TWR-K70F120M board.
  By default, the project is configured to use the J-Link SWD interface.

CONFIGURATION
=============
   The application is downloaded to the internal Flash or internal RAM
  memory, depending on selected configuration and executed.

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
