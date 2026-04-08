########################################################################
#
#                           Ext_Interrupt.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the NGX-BLUEBOARD-LPC11U37 evaluation board. 
   This project test GPIO Group Interrupt modules. When you press external
  interrupt switch the Test1_LED(D3) will ON.

    For each port/pin connected to one of the two the GPIO Grouped Interrupt
  blocks (GROUP0 and GROUP1), the GPIO grouped interrupt registers determine
  which pins are enabled to generate interrupts and what the active polarities
  of each of those inputs are. The GPIO grouped interrupt registers also select
  whether the interrupt output will be level or edge triggered and whether
  it will be based on the OR or the AND of all of the enabled inputs.
    When the designated pattern is detected on the selected input pins, the 
  GPIO grouped interrupt block will generate an interrupt. If the part is in
  a power-savings mode it will first asynchronously wake the part up prior to
  asserting the interrupt request. The interrupt request line can be cleared
  by writing a one to the interrupt status bit in the control register.

COMPATIBILITY
=============
   The Ext_Interrupt example project is compatible with NGX-BLUEBOARD-LPC11U37
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

GETTING STARTED
===============
  1) Build and download the program.
  2) Run the program.
