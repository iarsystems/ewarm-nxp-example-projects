########################################################################
#
#                           LCD_Demo.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for IAR LPC-2378-SK board. It shows basic use of the I/O,
 the timer, the interrupt controllers and the LDC module for graphic and text
 drawing.
  It starts by show the IAR logo on the LCD and after 5 second draw a test screen.
  Controls:
 AN_TRIM – contrast / backlight
 BUT1    - adjust backlight
 BUT2    - adjust LCD contrast


COMPATIBILITY
=============

   The example project is compatible with, on IAR LPC-2378-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============
   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
   Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
   If Port 2 bits 0 - 9 are used by application PINSEL10 must be cleared and
  this will disable trace port too.

   Make sure that the following jumpers are correctly configured on the
  IAR LPC-2378-SK evaluation board:

  PWR_SEL     - depending of power source
  USBC/USBSC  - USBSC
  RST_E       - unfilled
  ISP_E       - unfilled

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC23xx\IAR-LPC-2378-SK\LCD_Demo\LCD_Demo.eww

  3) Run the program.

     The LCD_Demo application is downloaded to the iFlash.
