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
 to develop code for the Freescale Faraday EVB board. It shows basic use
 of I/O, system initialization, UART and interrupts. The example blink LED 
(P17 pin1 - PTD16 GPIO2[14]) and print a message to UART1 on SW6 pressing.
  UART settings - 115200, 8 bits, noparity, 1 stop bit
 Note: For boot in quad mode, add --quad_mode extra parameter for flashloader
 

COMPATIBILITY
=============
   The example project is compatible with Faraday EVB board.
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
A5 Debug QSPI0 XIP
	This configuration is intended for debugging in QuadSPI0 Memory for
  Coretx-A5 core
A5 Debug QSPI1 XIP
	This configuration is intended for debugging in QuadSPI1 Memory for
  Coretx-A5 core
A5 Release QSPI0 DDR
	This configuration is intended for release only and loader program from 
  QSPI0 to DDR and execute
  Coretx-A5 core

DIP switches:
SW10 - 2 
OFF - boot from QSPI0
ON  - boot from QSPI1

GETTING STARTED
===============
   1) Build and download the program.

   2) Connet Host PC to the board(P27) and start terminal program

   3) Run the program.
