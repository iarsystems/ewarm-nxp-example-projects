########################################################################
#
#                         uart.eww
#
#                                                   $Revision: 34 $
#
########################################################################


DESCRIPTION
===========
 This example project shows how to use the IAR Embedded Workbench
for ARM to develop code for the Logic PD LH79524 evaluation boards.
It shows basic use of the UART0 and the interrupt controller.

 This application is intended to show how to set up the UART to transmit
and receive characters. The UART0 configuration is 115,200 baud, 8-n-1 mode

COMPATIBILITY
=============
The project is compatible with a LogicPD LH79524 board.

GETTING STARTED
===============
Start the IAR Embedded Workbench for ARM.
Select File->Open->Workspace...
Open the workspace file

JTAG
====
Build the project.
Select Project->Options...
In the category field select C-SPY.
In the setup tab set driver to Macraigor, J-Link or RDI depending on your JTAG hardware.
Start the debugger.
The board may require special setup in a C-SPY macro before the code is downloaded
by the debugger.
