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
 to develop code for TWR-K70F120M board.
 It shows basic use of GPIO and system timer.
  It toggles LED D7 (PTA4) of TWR-K70F120M 15 times per second.

  Note: The flashloader keep controller in unlocked state. For locking the controller
  only 00 (10 is unlock combination at address 0x40C bits 0,1) is valid combination


COMPATIBILITY
=============
  The example project is compatible with Freescale TWR-K70F120M board.
  By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============
  The Workspace contains Getting Started example with few configurations.

  K70 RAM Debug:   The example is loaded into internal RAM for TWR-K70 board
  K70 Flash Debug: The example is flashed to embedded flash memory for TWR-K70 board

 Jumpers:
  J19 - Filled
  J18 - 1-2

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Connect the J-Link

  3) Power on the board.

  4) Select GettingStarted RAM or Flash Configuration

  5) Press Ctrl+D or use Download and Debug button to start a debug session
