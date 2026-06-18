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
 to develop code for IAR-LPC-122X-SK board. It shows basic use of I/O,
 timer, ADC and interrupt controllers.
 It shows Trimer position no LCD and UART
 
COMPATIBILITY
=============

   The example project is compatible with, on IAR-LPC-122X-SK evaluation 
  board. By default, the project is configured to use the J-Link SWD 
  interface.

CONFIGURATION
=============

  Jumpers:
   RST_E   - unfilled
   ISP_E   - unfilled

  The GettingStarted application is downloaded to the iFlash.

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

      <installation-root>\arm\examples\NXP\
     LPC12xx\IAR-LPC-122X-SK\GettingStarted\GettingStarted.eww

  3) Run the program.
