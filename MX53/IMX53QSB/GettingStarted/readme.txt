########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 7859 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the Freescale iMX535 quick start board. It shows basic use
  of I/O, system initialization and GP timer.


COMPATIBILITY
=============
  The example project is compatible with Freescale iMX535 QSB board.
  By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATIONS
===============
DDR_DEBUG:
  This configuration is intended for debugging in DDR memory

  Note: Initialization of the Clock Module and DDR controller is
         made in mcimx535qsb_ddr.mac file.

GETTING STARTED
===============
  1) Open the GettingStarted workspace.

  2) Connect the debugger probe and power the board.

  3) Build the configuration. Press CTRL+D or use Download and
     Debug button to start a debug session.

  4) Press F5 or use the GO button to run the example.
