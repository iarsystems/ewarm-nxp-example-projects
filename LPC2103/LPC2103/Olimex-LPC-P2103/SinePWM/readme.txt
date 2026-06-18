########################################################################
#
#                           SinePWM.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========

   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for NXP LPC family microcontrollers. It is developed for 
  Olimex LPC-P2103 board.
  
   This project implements sine modulated PWM using the Timer 2 module of the 
  LPC2103 microcontroller. The PWM period is 20kHz. Three sine wave singals with 
  same frequancy but different phase(120) can be seen if simple Low Pass filters 
  are connected to each of the outputs.
   The frequency and the level of the sine waves can be changed with commands from 
  UART0
   The UART configuration is: 115200bps, 8-data bits, no parity(), 1 - stop bit
  (115200-8-N-1)
   Use "Help" Command for details.
    
   The project also shows how the FIQ can be used. The PWM Interrup Service Routine
  is installed as FIQ and resides in the RAM for faster execution.
 
COMPATIBILITY
=============

  The Leds example project is compatible with, and works as a
  standalone project, on the Olimex LPC-P2103 board. By default,
  the project is configured to use the J-Link JTAG interface.

        
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\NXP\LPC2103\Olimex-LPC-P2103\
    \SinePWM\SinePWM.eww

  3) Run the program.
