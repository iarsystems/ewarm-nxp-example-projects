########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
This example project shows how to use IAR Embedded Workbench for ARM
to develop code for the MC13260-EVK evaluation board. It shows the basic use
of I/O, system initialization (MMU, ASIC), timer and PWM FIFO.

COMPATIBILITY
=============
The example project is compatible with the MC13260-EVK evaluation board. By default, 
the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============
Jumpers:
  J74: 1-2   (Enable high intensity LEDs)
  J75: 2-3
  J65: 3-5, 4-6
  
Switches:
  SW2: 01000 (Enable SPI flash)
  SW3: 00100 (Enable Red and Green LEDs)
  SW5: 11111 (Boot from SPI flash, use internal bootloader)

GETTING STARTED
===============
  1) Follow the steps in the Getting Started Guide to open the example project Getting Started. 

  2) Build and download the program.

  3) Run the program — The D2 and D3 will start to blink in different phase. 
     D4-D7 will increase and decrease their intensity.
