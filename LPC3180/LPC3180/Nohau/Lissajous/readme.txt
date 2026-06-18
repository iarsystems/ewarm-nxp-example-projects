########################################################################
#
#                           Lissajous.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for a NOHAU LPC3100 evaluation board. It shows
  basic use of LCD, the interrupt controllers and mathematics routine.
   It starts by show of Lissajous figures on the LCD. The button S3 and
  the button S2 change the phase between signals and the button S4 and
  the button S5 the frequency.

COMPATIBILITY
=============

   The Lissajous example project is compatible with on a NOHAU LPC3100 
  evaluation board. By default, the project is configured to use the 
  J-Link JTAG interface and have two configurations, one with enable a VFP9-S 
  and second without a VFP9-S.

CONFIGURATION
=============

  1) Make sure that the jumpers settings are regarding Nohau LPC3100 user manual.
        
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC3180\Nohau\Lissajous\Lissajous.eww

  3) Run the program.

     The Lissajous application is downloaded to the iRAM memory 
   on the evaluation board and executed. 
  Buttons:
   S3 - Decrease phase
   S2 - Increase phase
   S4 - Decrease frequency
   S5 - Increase frequency

