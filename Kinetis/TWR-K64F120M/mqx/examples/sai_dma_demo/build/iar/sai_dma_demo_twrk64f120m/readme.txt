########################################################################
#
#                         sai_dma_demo_twrk64f120m.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  The example shows how to use the I2S/SAI module connected to an external
  TWR-AUDIO audio card.

  The demo suppport 16bit, 32bit quantization level.
  Supporting sample rate: 8K, 11.025k, 12k, 16k, 22.05k, 24k, 32k, 44.1k,
  48k, 96KHZ.

  Example platform: SAI + SGTL5000 + sdcard

  Examples of the command in shell:

  play sai: a:/<filename>.wav
  record sai: a:/<filename>.wav <time>:<sample rate>:<quantization>:<channel>
  (For example: record sai: a:/test.wav 5:44100:16:2)

  Notice: 
  If record high quality music (i.e. 96KHZ sample rate), it needs a high
  speed SD card, not all SD card can reach that speed.

  The demo now can only play the standard PCM file, and the header of the
  file should be standard.

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
    J38 - 2-3
    J39 - OFF

  The application uses the serial interface on the TWR-SER board.

  Terminal settings must be 115200 8-N-1.

GETTING STARTED
===============
  1) Build the included libraries (if needed).

  2) Select configuration.

  3) Build and download the program.
