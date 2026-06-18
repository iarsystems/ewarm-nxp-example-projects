########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for Olimex LPC-H288x header board. It shows
  basic use of I/O, timer and the interrupt controllers.
   It starts by togging LED (P2.1).

COMPATIBILITY
=============

   The Leds example project is compatible with, on Olimex LPC-H288x
  header board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATIONS
=============
	
iRAM Debug:
	Application is loaded in the internal RAM.

iFlash Debug:
	Application is programmed in the internal Flash.

xFlash Debug:
	Application is programmed in the external Flash.
	Note: This configuration contains code in both, the external
	      and the internal Flash memories. 
	      The FlashNXPLPC2888.flash flash loader (default) is used 
		      to program the internal memory.
	      The FlashOlimexLPCH288x.flash flash loader is used 
		      to program the external memory.
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC288x\Olimex-LPC-H288x\GettingStarted\GettingStarted.eww

  3) Run the program.

     The GettingStarted application is downloaded to the iRAM, iFlash or
     xFlash memory on the evaluation board and executed.
