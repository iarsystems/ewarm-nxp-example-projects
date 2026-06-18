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
  to develop code for Freescale iMX35 PDK board. It shows basic use of I/O,
  system initialization (PLL, MAX, AIPI, EMI, MMU, AITC) and timers.


COMPATIBILITY
=============
   The example project is compatible with Freescale iMX35 PDK board.
   By default, the project is configured to use the J-Link JTAG interface.


CONFIGURATIONS
===============
SDRAM_DEBUG:
    This configuration is intended for debug in SDRAM
    Note: Initialization of the Clock Module and SDRAM controller is
          made in mcimx35_sdram.mac file.

IRAM_DEBUG
    Intended for debug in the internal RAM.

NAND_RELEASE
    Intended for standalone execution. The program is linked in the SDRAM and
    flashed in the external NAND Flash.
    Note: The program is loaded in the SDRAM by the bootloader part.
          The bootloader initializes the Clocks and SDRAM controller.

Note: During SDRAM_DEBUG, IRAM_DEBUG and NAND Flash programming the boot mode
      switches should be configured for Internal boot: SW9 - 0, SW10 - 0.
      This enables the execution of the ROM code which stops the Watchdog Power
      Down Counter.

Note: In order to start the standalone example, select NAND_RELEASE configuration.
      From Project menu select Download/Download active application. Change the
      boot mode swithces to External boot -  SW9 - 1, SW10 - 0. Set the boot
      config switch SW2 for NAND flash, 5-address cycles, page 4KB with 208B spare,
      8-bit ECC and 8-bit bus. Turn on the board power.
        SW2   1 - ON
              2 - OFF
              3 - OFF
              4 - ON
              5 - ON
              6 - ON
              7 - ON
              8 - OFF

GETTING STARTED
===============
  1) Open the GettingStarted workspace.

  2) Power the board and connect the debugger probe.

  3) Select configuration and build it. Press CTRL+D or use Download and
     Debug button to start a debug session.

  4) Press F5 or use the GO button to run the example.
