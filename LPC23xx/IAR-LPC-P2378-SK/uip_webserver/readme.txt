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
  develop code for IAR LPC-P2378-SK evaluation boards. This demo shows a
  web server  application  running on the top of the uIP 1.0 TCP-IP stack.
   This example can work standalone on IAR LPC-P2378-SK board.
   The UART 0 is used to send debbug messages. It settings are - 115200, 8bits,
  1 stop, no parity

   The default IP address is:
    192.168.0.100 (set in main.c main subroutine)
   The physical MAC address is (defined in uipopt.h):
    00-ff-ff-ff-ff-ff

COMPATIBILITY
=============

   The example project is compatible with, on IAR LPC-P2378-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG interface.

CONFIGURATION
=============

   After power-up the controller get clock from internal RC oscillator that
  is unstable and may fail with J-Link auto detect, therefore adaptive clocking
  should always be used. The adaptive clock can be select from menu:
  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.

   Make sure that the following jumpers are correctly configured on the
  IAR LPC-P2378-SK evaluation board:
  PWR_SEL     - depending of power source
  RST_E       - unfilled
  ISP_E       - unfilled

GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     LPC23xx\IAR-LPC-P2378-SK\uip_webserver\uip_webserver.eww

  3) Connect a serial cable between a Host COM port and the IAR LPC-P2378-SK
     RS232_0 connector and start a terminal emulator program on the Host.
     Configure the PC COM port: 115200 baud, 8-bit, 1 stop, no parity,
     no hardware handshacke.

  4) Run the program.

  5) Connect cable between PC LAN Card or Hub and IAR LPC-P2378-SK board.
     Start web browser and type 192.168.0.100 in address field of the browser.


