########################################################################
#
#                           iar_spifi_memmap
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for LPCXpresso54608 board. 
 
  The example is intended to demonstrate how to use a .mac file (SPIFIMAP_LPC546xx.mac)
  to memory map the SPI Flash memory attached. This permits the verification of
  the programmed memory. The project also programs the SPIFIIMG.BIN to spifi memory. 
  
  The project uses  startup_LPC54608.s and others startup files which are part
  of NXP's SDK.
 
 The SPIFIIMG.BIN file was created using the command prompt "copy" command:
 "$PROJ_DIR$\bin>copy /b iar_logo_480x272.bmp+arena_480x272.bmp
  +squirrel_480x272.bmp+model_480x272.bmp+strawberry_480x272.bmp+Next_76x27.bmp
  +Prev_76x27.bmp+Loud_52x52.bmp+wide_stereo_52x52.bmp SPIFIIMG.BIN"

COMPATIBILITY
=============

   The example project is compatible with NXP's LPCXpresso54608 board. 
   By default, the project is configured to use the on board CMSIS DAP debugger.

CONFIGURATION
=============
  Download  - To program the SPI flash memory use the "Download and Debug" IDE
              GUI button. This will use SPIFIMAP_LPC546xx.mac file for verification
              of the SPIFI contents.
    
GETTING STARTED
===============

  1) Open the iar_spifi_memmap.eww workspace.
  
  2) Power the board and connect the debugger probe.

  3) Use the "Download and Debug" button to start the example.
     
