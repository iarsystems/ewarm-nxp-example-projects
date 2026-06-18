########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 7859 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for Freescale iMX31 PDK board. It shows basic use of I/O,
  system initialization (PLL, MAX, AIPI, EMI, MMU, AITC) and timers.


COMPATIBILITY
=============
   The example project is compatible with Freescale iMX31 PDK board.
   By default, the project is configured to use the J-Link JTAG interface.


CONFIGURATIONS
===============
SDRAM_DEBUG:
    This configuration is intended for debugging in SDRAM
    Note: Initialization of the Clock Module and SDRAM controller is
          made in mcimx31.mac file.

IRAM_DEBUG
    Intended for debug in the internal RAM.

NAND_DEBUG
    Programs the example in external NAND Flash.
    Note: The program is loaded in the SDRAM by the bootloader part.
          The bootloader initializes  the Clocks and SDRAM controller.
          The mximx31_nand.mac is used to reset the processor. This starts
          the iMX31 boot-up sequence. Please refer to iMX31 PDK Users Guide
          for proper setting of Boot mode switches. They should be in "8-bit
          2K NAND flash ext boot" possition.
    SW4-1 ON, SW4-8 - ON, others OFF
    SW5 - x, SW6 - ON, SW7 - OFF, SW8 - OFF, SW9 - OFF, SW10 - OFF

GETTING STARTED
===============
  1) Open the GettingStarted workspace.

  2) Power the board and connect the debugger probe.

  3) Select configuration and build it. Press CTRL+D or use Download and
     Debug button to start a debug session.

  4) Press F5 or use the GO button to run the example.
