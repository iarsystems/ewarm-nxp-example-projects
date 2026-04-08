########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for the Hitex LPC1800 board (LPC1850 rev. A). It shows basic
 use of I2C, timer and interrupt controllers.
  It starts by blinking LED0 (PCA9502 I2S to IOs controller).

  Note: If your application will be loaded to NORFLASH and is bigger than
        16Kb, verification may fail if you don't use the
        FlashHitexLPC1850_NOR_debug.mac macro file when starting the
        debugger. The macro file configures all of the address lines of EMC,
        in order to address the whole NORFLASH memory.

COMPATIBILITY
=============

   The example project is compatible with Hitex-LPC1850/57 evaluation board.

CONFIGURATION
=============

  Jumpers:
 	See ewinfo.ENU.html file

  Note: Boot config pin P2_9 must be pulled down to GND via 10k resistor
         to boot from external flash. On some early revision boards this pin
         cannot be configured by jumper and is default high level, so booting
         is not possible.

  The GettingStarted application is downloaded to NOR, SPI eeprom, embedded
  Flash or RAM memory  depending of the project's configuration and executed.

GETTING STARTED
===============

  1) Build and download the program.

  2) Run the program.
