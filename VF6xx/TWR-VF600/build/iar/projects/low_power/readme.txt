########################################################################
#
#                           low_power.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for the Freescale TWR-VF600 board. It show how to use low 
 power modes.


COMPATIBILITY
=============
   The example project is compatible with WTR-VF600 and TWR-SER boards.
   By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATIONS
===============
Project Interrupts
    SRAM
	This configuration is intended for debugging in OCRAM for Coretx-A5 core

GETTING STARTED
===============
  1) Batch build the program (press key F8 and select appropriate action).

  2) Download the program for Cortex A5 core (the program contains the image for a Cortex M4 core too)

  3) Connet Host PC RS232 to TWR-SER board and open a terminal programe

  4) Run the program.
