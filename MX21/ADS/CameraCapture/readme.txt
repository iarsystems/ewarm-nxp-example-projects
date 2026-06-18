
########################################################################
#
#                    CameraCapture.eww
#
#                                                   $Revision: 16 $
#
########################################################################


DESCRIPTION
===========
 This example project shows how to use the IAR Embedded Workbench for ARM
to develop code for Freescale MX21 ADS evaluation board based on MC9328MX21.
It shows basic use of timer, CSI, PrP, the interrupt controller and touchscreen.

 It show menu picture on the LCD, scan the touchscreen and capture picture form 
the camera.

COMPATIBILITY
=============
The project is compatible with Freescale MX21 ADS evaluation board.

The project is by default configured to use the J-Link JTAG interface.


GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM.

  2) Select File->Open->Workspace...
     Open the following workspace:

     <installation-root>\arm\examples\Freescale\MX21\ADS\CameraCapture\CameraCapture.eww

CONFIGURATION
=============
The application is downloaded to the flash or the SDRAM.

To start the downloaded application standalone from flash the S2 DIP switches must be set as:
BOOT0 - on
BOOT1 - off
BOOT2 - off 
BOOT3 - on 

