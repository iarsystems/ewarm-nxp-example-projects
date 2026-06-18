########################################################################
#
#                           Wakeup.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the NGX-BLUEBOARD-LPC11U37 evaluation board. 
   The project allows the user to put the part into various modes by editing
  the pmu.h header file.

    It demonstrates an wakeup switch :
    - When you press external interrupt switch then the controller enters into
      the deep power down mode.
    - when you press wakeup switch then the controller wake up from the deep
      power down mode.

    The LPC11Uxx support a variety of power control features. In Active mode,
  when the chip is running, power and clocks to selected peripherals can be 
  optimized for power consumption. In addition, there are four special modes
  of processor power reduction with different peripherals running: Sleep mode,
  Deep-sleep mode, Power-down mode, and Deep power-down mode.

COMPATIBILITY
=============
   The Wakeup example project is compatible with NGX-BLUEBOARD-LPC11U37
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

GETTING STARTED
===============
  1) Build and download the program.
  2) Run the program.
  