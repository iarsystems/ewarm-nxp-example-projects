########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for the Freescale TWR-VF600 board. It shows basic use
 of I/O, system initialization, UART and interrupts. The example blink LED
 D1 and print a message to UART1 on SW1 pressing.
  UART settings- 115200, 8 bits, noparity, 1 stop bit

 DIP Swiches (BOOT CONFIG):
  Boot from QSPI0 -
   1 - ON
   2,3,4,5,6 - OFF


COMPATIBILITY
=============
   The example project is compatible with WTR-VF600 and TWR-SER boards.
   By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATIONS
===============
A5 Debug OCRAM
	This configuration is intended for debugging in OCRAM for Coretx-A5 core
M4 Debug OCRAM
	This configuration is intended for debugging in OCRAM for Cortex-M4 core
A5 Debug DRR
	This configuration is intended for debugging in DRR for Coretx-A5 core
M4 Debug DDR
	This configuration is intended for debugging in DDR for Cortex-M4 core
A5 Debug QSPI XIP
	This configuration is intended for debugging in QuadSPI0 Memory for
  Coretx-A5 core

GETTING STARTED
===============
   1) Build and download the program.

   2) Connect Host PC to TWR-SER board and start terminal program

   3) Run the program.