########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 9663 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for MCIMX6SX-SDB board. It shows basic use of the GIC,
  the Private Timer and the UART module.

COMPATIBILITY
=============
   The example project is compatible with MCIMX6SX-SDB PDK board.
  By default, the project is configured to use the I-Jet JTAG interface.

CONFIGURATIONS
===============
OCRAM_DEBUG
    Intended for debug in the on-chip RAM.

QSPI_DEBUG
    The Program is linked at and will be executed from the QSPI memory.
    Note: Select Boot mode Internal Boot and boot device QSPI.

          S1   1 - OFF
               2 - ON

          SW10 All pins OFF

          SW11 All pins OFF

    SW12 pins 4 and 5 ON, all other OFF

GETTING STARTED
===============
  1) Open the GettingStarted workspace and select a configuration.

  2) Power the board and connect the debugger probe.

  3) Connect the board's UART-TO-USB port (J16) to a PC using micro USB cable.
     Two new COM ports will appear on the PC. You should use the COM port with
     Location "on USB Serial Converter A". Check this by opening the Device
     Manager -> Ports(COM&LPT). Right click on the suspected COM port. Select
     Properties -> General and check the Location.

  4) On the PC open a terminal program with the following settings: 115200 bps,
     8 data bits, no parity, 1 stop bit.

  5) Press CTRL+D or use Download and Debug button to start a debug session.

  6) Press F5 or use the GO button to run the example. A start message and the
     counted seconds will be printed on the terminal.
