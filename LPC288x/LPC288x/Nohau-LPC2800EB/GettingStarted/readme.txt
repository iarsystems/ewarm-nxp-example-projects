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
  to develop code for NOHAU LPC2800 evaluation board. It shows
  basic use of I/O, timer and the interrupt controllers. It starts by 
  blinking D3 and D4. The button S3 change blinking speed.
   The jumpers settings are regarding Nohau LPC2800 user manual

COMPATIBILITY
=============

   The Leds example project is compatible with, on NOHAU LPC2800 
  evaluation board. By default, the project is configured to use the 
  J-Link JTAG interface.

CONFIGURATION
=============

  1) Make sure that the jumpers settings are regarding Nohau LPC2800 
    user manual.
        
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC288x\Nohau-LPC2800EB\GettingStarted\GettingStarted.eww

  3) Run the program.

     The GettingStarted application is downloaded to the iRAM, iFlash
	 or xFlash memory depending of configuraton on the evaluation board 
	 and executed. 
