########################################################################
#
#                    CameraCapture.eww
#
#                                                   $Revision: 7859 $
#
########################################################################


DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for Freescale i.MX31 PDK evaluation board based on MCIMX31.
  It shows basic use of timer, CSI, PrP, the interrupt controller and touchscreen.

  It show menu picture on the LCD, scan the touchscreen and capture picture form 
  the camera.


COMPATIBILITY
=============
  The project is compatible with Freescale i.MX31 PDK evaluation board.

  The project is by default configured to use the J-Link JTAG interface.


CONFIGURATION
=============
  The application is downloaded to the NAND flash or the SDRAM.

  NAND boot switches setup:
      SW4-1 ON, SW4-8 - ON, others OFF
      SW5 - x, SW6 - ON, SW7 - OFF, SW8 - OFF, SW9 - OFF, SW10 - OFF


GETTING STARTED
===============
  1) Open the CameraCapture workspace.

  2) Power the board and connect the debugger probe.

  3) Select configuration and build it. Press CTRL+D or use Download and
     Debug button to start a debug session.

  4) Press F5 or use the GO button to run the example.
