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
  to develop code for NXP MCIMX7SABRE board. It shows a basic use of the
  GIC, the General Purpose Timers and the UART modules in single-core
  (standalone) and multi-core debug sessions.

COMPATIBILITY
=============
  The example project is compatible with NXP MCIMX7SABRE board.
  By default, the project is configured to use the I-Jet JTAG interface.

CONFIGURATIONS
===============

Project: cortex_a7

  OCRAM_STANDALONE
      Intended for debug in the internal RAM in standalone debug session for
      the selected CA7 core.

  OCRAM_AMP_MULTICORE
      Intended for debug in the internal RAM in AMP multicore debug session.
      It loads automatically OCRAM_AMP_SLAVE configuration for
      the secondary (CM4) core. OCRAM_AMP_SLAVE configuration should be built
      before building this configuration.

Project: cortex_m4

  OCRAM_STANDALONE
      Intended for debug in the internal RAM in standalone debug session
      for CM4 core.

  OCRAM_AMP_SLAVE
      Not intended for standalone debug sessions.
      It is loaded automatically by the OCRAM_AMP_MULTICORE configuration.

GETTING STARTED
===============
  1) Open the GettingStarted workspace and select a configuration.

  2) Connect the debugger probe and power the board.

  3) Connect the board's UART-TO-USB port (J11) to a PC using micro USB cable.
     Two new COM ports will appear on the PC. You should use the COM port with
     Location "on USB Serial Converter A" for connection to "Cortex-A7" application
     and the COM port with Location "on USB Serial Converter B" for connection to
     "Cortex-M4" application. Check this by opening the
     Device Manager -> Ports(COM&LPT). Right click on the suspected COM port.
     Select Properties -> General and check the Location.

  4) On the PC open a terminal program for the each port with the following
     settings: 115200 bps, 8 data bits, no parity, 1 stop bit.

  5) Press CTRL+D or use Download and Debug button to start a debug session.

  6) Press F5 or use the GO button to run the example. A start message and the
     counted seconds will be printed on the terminal for each port.
