########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
This example project shows how to use AR Embedded Workbench for ARM
to develop code for Freescale IMX23 EVK board. It shows the basic use
of I/O, system initialization (PLL, EMI, MMU, ICOLL) and timers.

COMPATIBILITY
=============
The example project is compatible with the MX23 EVK board. By default, 
the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============
  SRAM Debug: This configuration is intended for debugging in the
              microcontroller's internal RAM.

  DDR Debug:  This configuration is intended for debugging in the external
              DDR.
Switches:
   S36(BOOT MODE)  - 0110 (DEBUG)
   S14(USB5V) - ON
   S22(DEBUG) - ON
   S1 (HOLD)  - OFF
   S12(BATT SUPPLY)  - REG (Left)

Connect 5V power to J6.
Connect the board USB port (J4) to USB Host (PC)

GETTING STARTED
===============
  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\Freescale\MX23\IMX23_EVK\
     GettingStarted\GettingStarted.eww

  3) Select configuration.

  4) Power the board and connect the J-Link.
 
  5) Press CTRL+D or use Download and Debug button to start a debug session. 