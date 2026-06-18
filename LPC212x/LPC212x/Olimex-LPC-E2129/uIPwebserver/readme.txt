########################################################################
#
#                    uip_webserver.eww
#
#                                                   $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
 This example project is developed to show how to use the IAR Embedded
 Workbench for ARM and how to develop an application for the Olimex LPC-E2129
 evaluation boards. It shows a webserver application  runing on the top of the
 uIP Emdedded TCP/IP Stack ver.1.0. It is possible to control the LEDs' state
 from a web client and also to see the status of the Button1, Button2 and the 
 current voltage on pin AIN0. This is done in "Control" menu.
 

COMPATIBILITY
=============
The project is compatible with the Olimex LPC-E2129 evaluation board.
The project is by default configured to use the J-Link JTAG interface.


GETTING STARTED
===============
Start the IAR Embedded Workbench for ARM.
Select File->Open->Workspace...
Open the workspace file
...IAR Systems\Embedded Workbench 4.0\arm\src\examples\NXP\LPC212x
 \Olimex-LPC-E2129\uipwebserver\uip_webserver.eww

   - check the jumpers:
        DBG  - placed
        JRST - removed
   - power the LPC-E2129 board
   - compile, download and run program

 The default IP address is:
  192.168.0.100
 The physical MAC address is (defined in cs8900a.h):
  00-ff-ff-ff-ff-ff

CONFIGURATION
=============
The application is downloaded to the flash.

