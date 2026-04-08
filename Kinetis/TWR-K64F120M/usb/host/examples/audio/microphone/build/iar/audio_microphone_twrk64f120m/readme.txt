########################################################################
#
#                      audio_microphone_twrk64f120m.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  The Demo application is used to control and communicate with USB Audio
  Microphone devices. It has the following main features:

  - Identify the USB Audio Microphone device attached to it: The device
    belong to Class, sub class, number of interfaces it has, the audio
    characteristic the device supports

  - Control the USB Audio Microphone device: mute control.

  - Record audio data received from USB audio microphone devices and store
    the data in a SD card in WAV format. 

  - The user controls the USB Audio Microphone host by commands. The commands
    are received from a terminal window console. The terminal window is also
    used to display message from the application.

  The workspace also contains the projects of needed libraries, which should
  be build before running the example. Simply press F8 and then batch build
  all configurations.
  Note: If the needed libraries are already build in other example, this step
  may be skipped.

  To enable symbolic names in the debugger MQX RTOS awareness dialogs
  you must download and install the complete MQX package from Freescale.

COMPATIBILITY
=============
   The example project is compatible with Freescale TWR-K64F120M board.
  By default, the project is configured to use the I-Jet SWD interface.

CONFIGURATION
=============
  Int Flash Debug   - The application is downloaded to the internal Flash for
                      debugging purposes.
  Int Flash Release - The application is downloaded to the internal Flash.

  Set the jumper positions to their defaults as described in TWR-K64F120M
  Quick Start Guide except:
    J16 - 2-3 (connect the RESET button after level)
    J18 - 3-4
    J29 - 5-6 (board will be powered from TWR-ELEV module)
    J38 - 2-3 (USB will be powered from TWR-ELEV module)
    J39 - OFF

  The application uses the serial interface on the TWR-SER board.

  Terminal settings must be 115200 8-N-1.

GETTING STARTED
===============
  1) Build the included libraries (if needed).

  2) Select configuration.

  3) Build and download the program.

  4) Connect an USB microphone to the board's K64_USB connector.
