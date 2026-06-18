########################################################################
#
#                          GettingStarted.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for Hitex-LPC4350 evaluation board. It shows basic use of
  I/O, RI timer, I2C and interrupt controller.
  It toggles USB0_IND LED every 0.25s. Also lights up corresponding LED -
  LED0, LED1, LED2, LED3, when one or more sensor buttons are touched.

  Note: If your application will be loaded to NORFLASH and is bigger than
        16Kb, verification may fail if you don't use the
        FlashHitexLPC4350_NOR_debug.mac macro file when starting the
        debugger. The macro file configures all of the address lines of EMC,
        in order to address the whole NORFLASH memory.

COMPATIBILITY
=============

  The example project is compatible with Hitex-LPC4350-A2 evaluation board.

CONFIGURATION
=============

   Once you have successfully programmed the flash on the Hitex board, there is
  one step left before the LPC4350 can boot from external flash. That final step
  is to configure the boot mode using the boot mode jumpers.

  See GettingStarted/Boot_Jumpers.png

   Here is a diagram that shows how to configure the boot jumpers on the LPC4350
  Hitex board. The boards are shipped with the center two positions shorted with
  jumpers.

  See GettingStarted/Capture7.png

  Jumper JP5 should be open.

  Note: Boot config pin P2_9 must be pulled down to GND via 10k resistor
         to boot from external flash. On some early revision boards this pin
         cannot be configured by jumper and is default high level, so booting
         is not possible.

   The GettingStarted application is downloaded to NOR, SPI Flash (SPIFI),
  internal flash or RAM memory depending of the project's configuration and
  executed.

  IRAM - runs project from internal RAM.

  NORFLASH - loads project to external NOR flash.

  SPIFI - loads project to external SPI flash.

  IFLASH - loads project to internal flash (only if LPC43xx device with internal
           flash is present).

GETTING STARTED
===============

  1) Build and download the program.

  2) Run the program.
