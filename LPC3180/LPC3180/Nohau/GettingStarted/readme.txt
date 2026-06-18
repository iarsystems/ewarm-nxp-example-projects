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
  to develop code for a NOHAU LPC3100 evaluation board. It shows
  basic use of I/O, timer and the interrupt controllers. It starts by 
  blinking D3 and D4. The button S3 change blinking speed.
   The jumpers settings are regarding Nohau LPC3100 user manual

COMPATIBILITY
=============

   The LEDs example project is compatible with, on a NOHAU LPC3100 
  evaluation board. By default, the project is configured to use the 
  J-Link JTAG interface.

CONFIGURATION
=============

  1) Make sure that the jumpers settings are regarding Nohau LPC3100 
    user manual.
        
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC3180\Nohau\GettingStarted\GettingStarted.eww

  3) Run the program.

     The GettingStarted application is downloaded to the iRAM memory 
   on the evaluation board and executed. 
  Buttons:
   S3 - Change LEDs blinking speed 
