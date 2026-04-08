########################################################################
#
#                           StepMotorDemo.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
This example project shows how to use the IAR Embedded Workbench for ARM
to develop code for the IMX25-SK evaluation board. It shows basic use of I/O,
timers, interrupts, touch screen ADC and step motor control.

The stepper motor speed and direction are controlled by the trimer.

COMPATIBILITY
=============
The example project is compatible with the IMX25-SK evaluation board. By default, 
the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============
A stepper motor module is plugged on the board. Please check and make sure it’s firmly 
plugged in the right direction (see the evaluation board overview in Getting Started Guide). 

Befoer setting the jumpers as the following:
   JP19 - 1-2
   JP20 - 1-2
please power off and remove the USB cable connection to PC first to avoid potential damage
to the board.

GETTING STARTED
===============
  1) Follow the steps in the Getting Started Guide to open the example project Stepper Motor Demo. 

  2) Build and download the program.

  3) Run the program—By turning the trimer, you can hear that the stepper motor is running. 
