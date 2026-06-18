########################################################################
#
#                    AudioDevice.eww
#
#                                                   $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
 This example project shows how to use the IAR Embedded Workbench for ARM to develop 
code for the IAR-LPC-214X evaluation boards. It implements usb audio device with one
output and one input.
This example can work standalone on the IAR-LPC-214X board.

COMPATIBILITY
=============
The project is compatible with the IAR-LPC-214X evaluation board.

The project is by default configured to use the J-Link JTAG interface.


GETTING STARTED
===============
Start the IAR Embedded Workbench for ARM.
Select File->Open->Workspace...
Open the workspace file
<installation-root>\arm\examples\NXP\LPC214x\IAR-LPC-214X\AudioDevice\AudioDevice.eww

   - put the jumpers:
   	PWR_J   - depend of power source
	PWM/DAC - 2-3 (PWM)
	MIC/TR  - 1-2 (MIC)
	DBG     - present
	
   - run program
   - use the USB cable to connect the PC to USB of the Board
The first time the device is connected to the computer, Windows will load the 
driver for identified device. The USB Audio Device driver will be automatically loaded.
The audio output is on the AOUT pin and the audio input is on the AD03 pin.

CONFIGURATION
=============
The application is downloaded to the flash.

