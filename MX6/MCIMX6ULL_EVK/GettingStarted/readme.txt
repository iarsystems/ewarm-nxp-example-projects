########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 7084 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for NXP MCIMX6ULL-EVK board. It shows basic use of the
  GIC, the General Purpose Timer and the UART module.

COMPATIBILITY
=============
  The example project is compatible with NXP MCIMX6ULL-EVK board.
  By default, the project is configured to use the I-Jet JTAG interface.

CONFIGURATIONS
===============
OCRAM_DEBUG
    Intended for debug in the on-chip OCRAM.

QSPI_DEBUG
    The Program is linked at and will be executed from the QSPI memory.
    Note: Select Boot mode Internal Boot and boot device QSPI.

        SW602       1 - ON
                    2 - OFF

        SW601       1 - OFF
                    2 - OFF
                    3 - OFF
                    4 - OFF

DDR_DEBUG
    Intended for debug in the external DDR memory.

GETTING STARTED
===============
  1) Open the GettingStarted workspace and select a configuration.

  2) Power the board and connect the debugger probe.

  3) Connect the board's UART-TO-USB port (J1901) to a PC using micro USB cable.
     A new COM port "Silicon Labs CP210x USB to UART Bridge" will appear on the PC

  4) On the PC open a terminal program with the following settings: 115200 bps,
     8 data bits, no parity, 1 stop bit. Open the Silicon Labs CP210x USB to UART Bridge
     COM port

  5) Press CTRL+D or use Download and Debug button to start a debug session.

  6) Press F5 or use the GO button to run the example. A start message and the
     counted seconds will be printed on the terminal.
