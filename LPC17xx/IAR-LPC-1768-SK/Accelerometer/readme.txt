########################################################################
#
#                           Accelerometer.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
    This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for IAR-LPC1768-SK board. It shows basic use of I/O, timer, 
  interrupt controllers and LDC module and the on board accelerometer.
  
   It starts by show the IAR logo on the LCD and a cursor. The cursor moves as 
  the board position is changed. 


COMPATIBILITY
=============

   The example project is compatible with IAR-LPC1768-SK evaluation board.
   By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============
   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect. In this case adaptive
  clocking should be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   Make sure that the following jumpers are correctly configured on the
  IAR-LPC1768-SK evaluation board:

  PWR_SEL     - depending of power source
  RST_E       - unfilled
  ISP_E       - unfilled

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC17xx\IAR-LPC1768-SK\Accelerometer\Accelerometer.eww

  3) Run the program.

     The Accelerometer application is downloaded to the iFlash.
