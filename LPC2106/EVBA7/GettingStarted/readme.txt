
########################################################################
#
#                             evba7.eww
#
#                                                   $Revision: 28 $
#
########################################################################


DESCRIPTION
===========
This example project shows how to use the IAR Embedded Workbench for ARM
to develop code for the Ashling EVBA7 evaluation board.
The CPU on this board is the NXP LPC2106 ARM chip.
It shows basic use of parallel I/O, timer and the interrupt controller.

The example only sets up a subset of the I/O registers.
Low level setup like clock/PLL configuration must be done prior to the start
of this program. The low level setup done by the flash boot program is
good enough. Make sure that the JTAG interface does not perform a hard
reset at startup as this will destroy the initialization done by the flash
boot code.

The example program will count 0-9 on the 7-segment LED display, the time
between digits is set to 0.5 seconds. The four LED's D2-D5 will show the same
value in binary form.

COMPATIBILITY
=============
The project is compatible with the Ashling EVBA7 (NXP LPC2106 based) evaluation board.
The project is by default configured to use the Ashling Opella JTAG interface.

CONFIGURATION
=============
Connect the JTAG cable and power up the system.

Start the IAR Embedded Workbench for ARM.
Select File->Open->Workspace...
Open the workspace file
...\IAR Systems\Embedded Workbench 4.0\ARM\examples\NXP\LPC210x\EVBA7\evba7.eww

Load the Ashling RDI driver:
Select Project->Options...
In the category field select C-SPY.
In the setup tab set driver to RDI.
Select the RDI tab.
Browse to the Opella.DLL driver file.
Close the options dialog.

Select RDI->Configure...
Configure the Ashling Opella for ARM7TDMI-S, JTAG clock 750 kHz and select the
option "Do Not issue a Hard Reset (nSRST)".
Close the Ashling RDI dialog.

Start the C-SPY debugger.
Press the Go button.


The project uses a modified version of cstartup (evba7_cstartup.s79).
The only difference from the standard cstartup is the addition of the IRQ
exception vector irq_handler.

The linker configuration file is evba7.xcl.
Vectors are located at 0x40000000-0x4000003F in RAM, this is mapped to 0x00-0x3F.
Code is located at 0x40000200-0x40009FFF in RAM.
Data is located at 0x4000A000-0x4000FFFF in RAM.

