########################################################################
#
#                           USB_DCD.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
   DCD demo code is an integration of the DCD and the FSUSB-device 
  application that basically shows in the terminal what type of HOST
  controller is connected to the USB module (working in device mode).
  Also USB will recognize the board as a serial interface (COM port). 
  The driver for this is inicluded in the folder where the source files
  are stored. The terminal demo is a basic echo that returns the same
  character that was sent.

COMPATIBILITY
=============
   The example project is compatible with Freescale - TWR K40X256, K53N512 
  and K60N512 board. By default, the project is configured to use the J-Link 
  SWD interface.

   The demo code runs on the Tower system TWR TWR K40X256, K53N512 or K60N512 
  and a TWR-SER module is needed because the USB port of Kinetis is mapped to 
  the USB port of the TWR-serial board. The elevator modules are needed as well.

CONFIGURATION
=============
   The application is downloaded to the internal Flash or internal RAM
  memory, depending on selected configuration and executed.

   If you use TWR-K70F120M without TWR-ELEV and TWR-SER boards, you can
  use USB Virtual serial port at OSJTAG/OSBDM interface (virtual Serial
  toolkit from www.pemicro.com must be installed).
   You can use the serial port at TWR-SER board, any standard terminal
  software can be used.
  Terminal settings must be 115200 8-N-1.

  TWR-SER setup
  ----------------
  J16  USB_MODE
  1-2  ON
  3-4  -
  5-6  ON

  J11  IRQ_SEL2
  1-2  -
  3-4  -
  5-6  ON
  7-8  -


GETTING STARTED
===============
   1) Select configuration.

   2) Build and download the program.
   
   3) Run the program and connect the PC to TWR-SER USB port
   
   4) Install the "Freescale_CDC_Driver_Kinetis.inf" driver
      when prompted.
