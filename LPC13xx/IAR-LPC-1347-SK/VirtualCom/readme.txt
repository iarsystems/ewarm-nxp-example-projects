########################################################################
#
#                           VirtualCom.eww
#
# $Revision: 1591 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for IAR-LPC1347-SK board.
  It implements USB CDC (Communication Device Class) device and installs
 it like a Virtual COM port. The UART is used for physical implementation
 of the RS232 port. There is LED indication for data transfer from UART->
 Virtual COM and from Virtual COM->UART.
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

  The example project is compatible with IAR-LPC1347-SK evaluation board.
 By default, the project is configured to use the J-Link SWD interface.

CONFIGURATION
=============
  Jumpers:
  RST_E   - unfilled
  ISP_E   - unfilled
  TMS/SWDIO - SWDIO (2-3)
  TDO/SWV   - SWV (2-3)

GETTING STARTED
===============

  1) Build and download the program.

  2) Connect a serial cable between a Host/PC COM port and the IAR LPC1347-SK
     UART connector and start a terminal emulator program on the Host.

  3) Run the program.

  4) The first time the device is connected to the computer, Windows will
     load the driver for identified device. The Virtual COM port driver is
     in the $PROJ_DIR$\VirCOM_Driver_XP\.

  5) Open second instance of terminal emulator and open the Virtual COM Port.
     Configure the Virtual COM Port with the same settings as PC COM Port.

  6) Open these two ports with the two instances of terminal program.
     When you type at Virtual COM Port window, you will see typed characters
     at PC COM Port window and vice versa. You will see LED1 indication when
     you send data from Virtual COM Port to PC COM Port, and LED2 indication
     when you send data from PC COM Port to Virtual COM Port.
