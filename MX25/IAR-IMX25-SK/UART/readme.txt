########################################################################
#
#                           uart.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
This example project shows how to use the IAR Embedded Workbench for ARM
to develop code for the IMX25-SK evaluation board. It shows the basic use of I/O, 
system initialization (PLL, MAX, AIPI, EMI, MMU, AITC), timers and UART1 (CN3). 
The example returns echo on every received characher, LED0 indicates framing error, 
LED1 indicates overrun erro and LED2 indicates brake.
  
   Terminal settings:
   Communication speed: 115200 bps
   Number of data bits:    8
   Number of stop bits:    1
   Parity:              none

 COMPATIBILITY
=============
The example project is compatible with the IMX25-SK evaluation board. By default, 
the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============
Use the default jumper and switch settings on the board. For more information, read the the 
Getting Started Guide for IAR KickStart Kit for Freescale i.MX25.

You need also connect UART1 (CN3) and PC with a RS232 serial cable (not included in the kit), 
then open a Hyper Terminal (Start->All Programs->Accessories->Communications) with the 
following settings:

   Communication speed: 115200 bps
   Number of data bits:    8
   Number of stop bits:    1
   Parity:	none
   Flow control:	none

GETTING STARTED
===============
  1) Follow the steps in the Getting Started Guide to open the example project UART Echo. 

  2) Build and download the program.

  3) Run the program—Typing in the Hyper Terminal, you'll see the example returns echo on 
      every received characher. To test the error, just lower the Hyper Terminal communication 
      speed, for example to 4800. Typing again, you won't see any echo, but only the LD8 (LED2) 
      and LD10 (LED0) lighting.
