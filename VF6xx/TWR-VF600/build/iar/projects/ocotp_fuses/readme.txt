########################################################################
#
#                           ocotp_fuses.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for the Freescale TWR-VF600 board. The tprojeyc will blow
 OTP fuses. By default will set the fuses to boot from SD Card.
  Fuse location and descriptions are in Chapter 36 in the OCOTP_CFGx 
 registers. Fuses are 0 by default. Once blown to '1', the change is permanent.
 Be careful to ensure you are blowing the correct fuses. Ensure other fuses 
 have not already been blown that would interfere with fuses you want to blow. 
 The shadow registers where fuse state can be viewed starts at 0x400a5400
  UART settings - 115200, 8 bits, noparity, 1 stop bit.

 Note:
  FUSE CHANGES ARE PERMANENT. ENSURE YOU ARE BLOWING THE CORRECT FUSES!


COMPATIBILITY
=============
   The example project is compatible with WTR-VF600 and TWR-SER boards.
   By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATIONS
===============
Project OCOTP Fuses
    SRAM
	This configuration is intended for debugging in OCRAM for Coretx-A5 core

GETTING STARTED
===============
  1) Build and download the program.

  2) Connet Host PC to TWR-SER board and start terminal programme

  3) Run the program.
