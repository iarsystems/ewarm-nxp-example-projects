########################################################################
#
#                           uip_webserver.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM to
  develop code for IAR LPC-2468 evaluation boards. This demo shows a
  web server  application  running on the top of the uIP 1.0 TCP-IP stack.
   This example can work standalone on IAR LPC-2468 board.


COMPATIBILITY
=============
   The example project is compatible with, on IAR LPC-2468
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

   The default IP address is:
    192.168.0.100 (set in main.c main subroutine)
   The physical MAC address is (defined in uipopt.h):
    00-ff-ff-ff-ff-ff

CONFIGURATION
=============
   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   The GettingStarted application is downloaded to the iFlash or iRAM memory
  depending of the project's configuration and executed.

   Make sure that the following jumpers are correctly configured on the
  IAR LPC-2468 evaluation board:

  Jumpers:
   J5      - depending of power source
   ISP     - unfilled
   nRESET  - unfilled
   EINT0   - filled

  The LEDs share pins with Trace port. If ETM is enabled the LEDs can not be used. 

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC24xx\IAR-LPC-2468\uip_webserver\uip_webserver.eww

  3) Run the program.

  4) Connect cable between PC LAN Card or Hub and IAR LPC-2468 board.
     Start web browser and type 192.168.0.100 in address field of the browser.


