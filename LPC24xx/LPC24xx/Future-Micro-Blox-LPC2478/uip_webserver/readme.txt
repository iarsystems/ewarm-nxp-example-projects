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
  develop code for Micro-Blox LPC2478 boards. This demo shows a web server  
  application  running on the top of the uIP 1.0 TCP-IP stack.
   This example can work standalone on Micro-Blox LPC2478 board.
 
COMPATIBILITY
=============

   The Touch demo project is compatible with Micro-Blox LPC2478 board. 
   By default, the project is configured to use the
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

   The uIp web server application is downloaded to the iFlash.
  
  Jumpers:
   On Main board
    J1  - filled
    J13 - 2-3
    J14 - 2-3
    J48 - filled
    J49 - 3-4
    J50 - 5-6
    J58 - 1-2
    J59 - 1-2

   On CPU board
    J1  - 1-2
    J3  - 1-2
    J7  - filled
    J24 - 2-3
    J26 - 2-3
    J31 - 2-3
    J37 - filled

  
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

      <installation-root>\arm\examples\NXP\
     LPC24xx\Micro-Blox-LPC2478\uip_webserver\uip_webserver.eww

  3) Run the program.

  4) Connect a network cable between PC LAN Card, a hub, or a network card and
     IAR LPC-2478-SK board. Start a web browser and type 192.168.0.100 in
     address field of the browser.
     
     If you can't connect to the web server, then try to set your
     default gateway to 192.168.0.100.
