
########################################################################
#
#                    FlashMAC71x1_Example.eww
#
#                                                   $Revision: 16 $
#
########################################################################


DESCRIPTION
===========
This example project shows how to use the IAR Embedded Workbench for ARM
to develop code for Freescale MAC7100 evaluation board based on MAC7111.
It shows basic use of parallel I/O, timer and the interrupt controller.

It blinks the LED's DS2-DS9.

COMPATIBILITY
=============
The project is compatible with Freescale MAC7100 evaluation board.

The project is by default configured to use the J-Link JTAG interface.


GETTING STARTED
===============
Start the IAR Embedded Workbench for ARM.
Select File->Open->Workspace...
Open the workspace file
<installation-root>\ARM\examples\Freescale\MAC71x1\MAC7100-EV\Blinky\FlashMAC71x1_Example.eww

CONFIGURATION
=============
The application is downloaded to flash.

Make sure that the SW1 DIP switch MODA and MODB are set to ON.

When running in 3.3 Volt mode, jumper J43 should be in 2-3 and J11 in 2-3.
When running in 5.0 Volt mode, jumper J43 should be in 1-2 and J11 in 1-2.

