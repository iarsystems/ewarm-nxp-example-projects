########################################################################
#
#                          periph_mrt.eww
#
# $Revision: 28 $
#
########################################################################

periph_mrt

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for LPCXpresso5411x board. The MRT example demonstrates
  using the Multi-Rate Timer API functions. This example configures timers 0
  and 1 as periodic interrupts that blink LED0. Timer 2 is configured as a
  one-shot interrupt that gets reset in the interrupt handler. Timer 3 is setup
  as a one-shot interrupt that gets reset in the background loop.
  

COMPATIBILITY
=============
  The example project is compatible with LPCXpresso5411x board.

CONFIGURATION
=============
    The LPCXpresso5411x board must be powered from the Link2 USB connector J7,
    and jumper JP2 must be fitted in position pin 1 - 2 (Local Target). Jumper
    JP1 must be open to enable the target LPC5411x.

GETTING STARTED
===============
  1) Batch Build (F8->Rebuild All) and download the program.

  2) Run the program.
     Observe the blinking leds.
   