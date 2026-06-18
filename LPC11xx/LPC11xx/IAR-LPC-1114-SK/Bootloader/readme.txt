########################################################################
#
#                           Bootloader.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for IAR-LPC-1114-SK board. It shows how to create a secondary
 bootloader for the LPC1100. UART0 and the XMODEM-1K protocol is used to
 download new application firmware. The example is based on NXP AN10995 
 LPC1100 secondary bootloader and is ported for EWARM and IAR-LPC-1114-SK board
 
COMPATIBILITY
=============

   The example project is compatible with IAR-LPC-1114-SK evaluation board.
   By default the project is configured to use J-Link SWD interface.

CONFIGURATION
=============

  Jumpers:
   RST_E   - unfilled
   ISP_E   - unfilled
   TMS/SWD - SWD (2-3) 
   TDO/SWO - SWO (2-3) 
   
   J-Lind_D - filled - to use with an external J-Link
            - unfilled - to use with the on board J-Link

GETTING STARTED
===============

  The software consists of a secondary bootloader and an example application.
 The following set of steps describes how to use this software to demonstrate 
 the operation of the secondary bootloader
 
 1. Open the  LPC1114 Bootloader workspace.
 2. Select the Bootloader project, build it and program it on IAR-LPC1114-SK
    board
 3. Select the Application project and build it. This will result in creation
    of the \Application\Flash\Exe\Application.bin file
 4. Connect the Board to a PC via the RS232 connector. Start a terminal
    emulator application on the PC that is capable of communication using the
    XMODEM-1K protocol (HyperTerminal is used in this example). The terminal 
    emulator should be configured to communicate at 9600-8-N-1.
 5. Reset the target hardware and it should then start periodically
    transmitting the ‘C’ character to the PC – indicating it is ready to begin
    a transfer using the XMODEM-1K protocol. Note that when using HyperTerminal
    the board should be reset before it is connected to the PC – otherwise it
    may enter ISP mode.
 6. Now the application binary file can be transmitted to the target hardware,
    where the secondary bootloader will program it into flash. To start this
    process use HyperTerminal to send the file Application.bin using the 
    XMODEM-1K protocol.
 7. Once the transfer is complete the target hardware should be reset and LED1
    will begin to flash (if using HyperTerminal remember to disconnect before 
    pressing the base board reset button – otherwise the LPC1100 can enter ISP
    mode).
    The application code can initiate a new download simply by invalidating the
    CRC that is stored in the last location of flash memory. This can be done 
    by writing all zero’s to this location using the IAP calls and then
    re-invoking the secondary bootloader by resetting the device. The example
    application contains a function that implements this when pin P0.7 is taken
    low (this is connected to BUTTON 1 on the IAR-LPC1114-SK board).
    Pressing this button when the application is running (LED1 is flashing)
    will re-invoke the secondary bootloader and allow a new application to be
    downloaded.
