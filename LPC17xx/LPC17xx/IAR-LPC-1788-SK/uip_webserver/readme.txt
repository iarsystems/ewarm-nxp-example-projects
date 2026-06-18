########################################################################
#
#                           TouchDemo.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for IAR-LPC-1788-SK board. This demo shows a web server
 application  running on the top of the uIP 1.0 TCP-IP stack.
 It implements WEB server.
 
 The default IP address is:
   192.168.0.100
 The physical MAC address is (defined in uipopt.h):
   00-ff-ff-ff-ff-ff
  
COMPATIBILITY
=============

   The example project is compatible with IAR-LPC-1788-SK board. 
  By default, the project is configured to use the J-Link SWD interface.

CONFIGURATION
=============
  Flash Debug - The Progam is loaded to internal Flash.
  SDRAM Debug - The Progam is loaded to external SDRAM.
   
 Jumpers:
   PWR_SEL    - depending of power source
   ISP_E      - unfilled
   RST_E      - unfilled
  
 Note:
    The LCD shares pins with Trace port. If ETM is enabled the LCD will not
    work.
  
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\
     \LPC17xx\IAR-LPC-1788-SK\uip_webserver\uip_webserver.eww

  3) Select configuration.

  4) Power the board and connect the J-Link.
 
  5) Press CTRL+D or use Download and Debug button to start a debug session.
