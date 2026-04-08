########################################################################
#
#                           dual_core.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for the Freescale Faraday EVB board. It shows how to initialize
 second Cortex M4 core from Cortex A5 and execute two different applications
 on both cores.
  The dual core sample code consists of two projects:
  One for the A5, and one for the M4.
  The M4 code gets compiled into a binary with an executable code start
 address at 0x3f040401. The A5 project automatically loads that binary as part
 of the linker setup, and then points the M4 core to that address. The linker
 files are built to split the SystemRAM in half between the two cores.
  Open the \build\iar\projects\dual_core workspace to see both the A5 and M4
 projects. The key part of the M4 project is the linker file, so that the start
 address is always the same 0x3f040400), and the code will be loaded at
 0x3f040000 every time.
  The key pieces of code for the A5 project are:
  //Set M4 start address. Must be odd address since M4 is thumb
  SRC->GPR[2] = (unsigned int)__section_begin(".noinit_m4") + 0x401;
  //Enable M4 core
  CCM->CCOWR = 0x15a5a;

   Then in the A5 linker file, there’s two key lines:
    define symbol __code_start_m4__ = 0x3f040000;
    place at address mem:__code_start_m4__ { readonly section .noinit_m4 };
  This tells the linker where to place the M4 binary code.
  Finally the linker input configuration in the A5 project tells the linker
 to include the M4 binary code as part of the ELF it generates.

   UART settings - 115200, 8 bits, noparity, 1 stop bit


COMPATIBILITY
=============
   The example project is compatible with Faraday EVB board.
   By default, the project is configured to use the J-Link JTAG interface.


CONFIGURATIONS
===============
Project Dual core tower
    SRAM
  This configuration is intended for debugging in OCRAM for Coretx-A5 core
  and contains the image for a Coretx M4 core
Project Dual core M4 tower
    SRAM
  This configuration is intended for debugging in OCRAM for Cortex-M4 core

GETTING STARTED
===============
  1) Batch build the program (press key F8 and select appropriate action).

  2) Download the program for Cortex A5 core (the program contains the image for a Cortex M4 core too)

  3) Connet Host PC RS-232 to Faraday EVB board and open a terminal programe

  4) Run the program.

