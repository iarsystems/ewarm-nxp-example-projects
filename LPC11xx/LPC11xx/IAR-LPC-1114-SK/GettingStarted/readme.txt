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
 to develop code for IAR-LPC-1114-SK board. It shows basic use of I/O,
 timer, ADC and interrupt controllers.
 It shows the Temperature and the Trimer position no LCD and UART
 Press B1 to see the temperature on the LCD.
 Press B2 to see the trimer position on the LCD

COMPATIBILITY
=============

   The example project is compatible with, on IAR-LPC-1114-SK evaluation 
  board. By default, the project is configured to use the J-Link SWD 
  interface.

CONFIGURATION
=============

  Jumpers:
   RST_E   - unfilled
   ISP_E   - unfilled
   TMS/SWD - SWD (2-3) 
   TDO/SWO - SWO (2-3) 
   
   J-Lind_D - filled - to use with an external J-Link
            - unfilled - to use with the on board J-Link

  The GettingStarted application is downloaded to the iFlash.

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

      <installation-root>\arm\examples\NXP\
     LPC11xx\IAR-LPC-1114-SK\GettingStarted\GettingStarted.eww

  3) Run the program.
