########################################################################
#
#                           lcdc_rev2.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
   This is an example of using the LCD controller (LCDC) to drive the
  TWR-LCD-RGB display. Included in this project is code to do the Background
  Look Up Table (BGLUT) and Graphic Window Look up Table (GW). These are
  not used in the 24bpp mode that the TWR-LCD-RGB uses, but are for example
  purposes only.

COMPATIBILITY
=============
   The example project is compatible with Freescale TWR-K70F120M board.
  By default, the project is configured to use the J-Link SWD interface.

  The project uses the TWR-LCD-RGB display module.

CONFIGURATION
=============
   The application is downloaded to the internal Flash or internal RAM
  memory, depending on selected configuration and executed.

   If you use TWR-K70F120M without TWR-ELEV and TWR-SER boards, you can
  use USB Virtual serial port at OSJTAG/OSBDM interface (virtual Serial
  toolkit from www.pemicro.com must be installed).
   If your choice is the serial port at TWR-SER board, any standard terminal
  software can be used.
   Terminal settings must be 115200 8-N-1.
   Different versions of TWR-LCD-RGB board use LCD modules with positive or
  negative edge shift clock, please select configuration with suitable polarity
  of the clock.

 RAM 128KB negative edge LSCLK
  Configure negative edge of LCD shift clock
 RAM 128KB positive edge LSCLK
  Configure positive edge of LCD shift clock
 FLASH 1MB negative edge LSCLK
  Configure negative edge of LCD shift clock
 FLASH 1MB positive edge LSCLK
  Configure positive edge of LCD shift clock

GETTING STARTED
===============
   1) Select configuration.

   2) Build and download the program.
