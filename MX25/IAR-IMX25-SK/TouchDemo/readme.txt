########################################################################
#
#                           TouchDemo.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
This example project shows how to use the IAR Embedded Workbench for ARM
to develop code for the IMX25-SK evaluation board. It shows the basic use of I/O,
timers, interrupts, LCD controllers, LCD touch screen, touch screen ADCand beep 
generator of CS43L22 codec.

COMPATIBILITY
=============
The example project is compatible with the IMX25-SK evaluation board. By default, 
the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============
Use the default jumper and switch settings on the board. For more information, read the the 
Getting Started Guide for IAR KickStart Kit for Freescale i.MX25.

GETTING STARTED
===============
  1) Follow the steps in the Getting Started Guide to open the example project Accelerometer Demo. 

  2) Build and download the program.

  3) Run the program—A square cursor is shown and moving when the screen is touched. 
     You will hear the beeping sound when touching the cursor. The RV1 controls the transparency 
     level of background and graphic windows. 