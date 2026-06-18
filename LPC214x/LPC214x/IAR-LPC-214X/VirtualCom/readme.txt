########################################################################
#
#                    VirtualCom.eww
#
#                                                   $Revision: 1591 $
#
########################################################################

DESCRIPTION
===========
 This example project shows how to use the IAR Embedded Workbench for ARM 
to develop code for the IAR-LPC-214X evaluation boards. It implements USB 
CDC (Communication Device Class) device and install it like a Virtual COM 
port. The UART1 is used for physical implementation of the RS232 port.
This example can work standalone on the IAR-LPC-214X board.
 With this example project, you can echo a character between the RS232 
Physical interface and the Virtual Com Port USB interface. You will need to 
open two separate (Tera Term or HyperTerm) windows. One for the virtual 
interface and another for the UART. Set both sessions to 8-N-1 No flow control. 
Make sure that the virtual com port occupies a port between 1-8. You may have 
to change this in the Device Manager option under the advanced tab.
  
 Note:
 Before you can use the following example the proper .inf file needs to be 
installed for your version of windows. Please look in this example project 
folder a subfolder \VirCOM_Driver_"X"\ . In this you will see the proper .inf file 
that must be installed when Windows tries to install the generic CDC class driver. 
Please note that "X" denotes your particular operating system. If it does 
not install properly, you may have to go into your device manager and look 
under "other devices" and manually point the driver to the proper .inf file 
for your operating system. When you have properly installed the driver you 
will see an entry in your Device Manager|Ports that says IAR Virtual Com Port, 
"your board"(where "your board is the particular NXP board you are using). 

COMPATIBILITY
=============
The project is compatible with the IAR-LPC-214X evaluation board.

The project is by default configured to use the J-Link JTAG interface.


GETTING STARTED
===============
Start the IAR Embedded Workbench for ARM.
Select File->Open->Workspace...
Open the workspace file
<installation-root>\arm\examples\NXP\LPC214x
	\IAR-LPC-214X\VirtualCom\VirtualCom.eww

   - put the jumpers:
   	PWR_J   - depend of power source
	DBG     - present
	
   - run program
   - use the USB cable to connect the PC to USB of the Board
The first time the device is connected to the computer, Windows will 
load the driver for identified device. The Virtual COM port driver is 
in the $PROJ_DIR$\VirCOM_Driver_XP\.

CONFIGURATION
=============
The application is downloaded to the flash.
