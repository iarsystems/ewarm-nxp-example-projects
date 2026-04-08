########################################################################
#
#                         led_blink.eww
#
#                                                   $Revision: 34 $
#
########################################################################


DESCRIPTION
===========
 This example project shows how to use the IAR Embedded Workbench
for ARM to develop code for the Logic PD LH7A400 evaluation boards.
It shows basic use of MMU, parallel I/O, timer and the interrupt
controller.

 It starts by showing different patterns on the LED's separated by
half second.

COMPATIBILITY
=============
The project is compatible with the LogicPD LH7A400 board.

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
