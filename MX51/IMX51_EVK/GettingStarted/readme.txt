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
  to develop code for the Freescale iMX51 EVK. It shows basic use
  of I/O, system initialization, GP timer and TZIC interrupt controller.
  The Diagnostic LED D25 at GPIO2_6 starts to blink.

COMPATIBILITY
=============
  The example project is compatible with Freescale iMX51 EVK board.
  By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATIONS
===============
DDR_DEBUG:
  This configuration is intended for debugging in DDR
  Note: Initialization of the DDR controller is
        made in mcimx51evk_ddr2.mac file.

GETTING STARTED
===============
  1) Build and download the example.
  2) Run the program.
