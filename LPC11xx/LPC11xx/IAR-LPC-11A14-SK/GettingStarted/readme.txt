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
  to develop code for IAR-LPC-11A14-SK board. It shows basic use of I/O,
  timer, ADC and interrupt controller.
    It shows the Trimer position on the LCD and UART, and lights up the eight
  board leds in "running light" mode.
    Press the USER button to invert leds state.

COMPATIBILITY
=============

   The example project is compatible with, on IAR-LPC-11A14-SK evaluation
  board.

CONFIGURATION
=============

   Make sure that the following jumpers are correctly configured on the
  IAR-LPC11A14-SK evaluation board:

  Jumpers:

  USER_E         - closed
  RS_RST/USB_RST - open/open
  RS_ISP/USB_ISP - open/open
  TRIM_E         - closed
  BUZ_E          - open
  ACMPO_INT_E    - open
  ACC_INT_E      - open
  LEDx_E         - all closed
  3.3V_CORE_E    - closed
  3.3V_IO_E      - closed
  RS_RX/USB_RX   - RS_RX
  RS_TX/USB_TX   - RS_TX
  REG/BAT        - depends on power source

  The GettingStarted application is downloaded to the internal Flash.

GETTING STARTED
===============

  1) Build and download the example.

  2) Run the program.
