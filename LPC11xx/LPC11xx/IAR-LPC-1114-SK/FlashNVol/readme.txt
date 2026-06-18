########################################################################
#
#                           FlashNVol.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM to
 develop code for IAR-LPC-1114-SK board. It describes the implementation and
 use of a library that allows on-chip flash memory to be used for non-volatile
 storage, in a similar manner to EEPROM. The example is based on NXP AN11008 
 application note and is ported for EWARM and IAR-LPC-1114-SK board
 
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

  This example shows how to use the flash memory for variable storage, which takes
 into account erasing flash memory in a way that avoids data loss and minimizes
 the effect of flash memory lifespan.
 
 1. Open the  LPC1114 FlashNVol workspace.
 2. Power the board and connect the debugger probe.
 3. Use Project > Download > Erase memory menu to erase the whole device memory
 4. Build the FlashNVol project. Press CTRL+D or use Download and Debug button 
    to start a debug session.
 5. A counter showing the seconds is displayed on the LCD. The counter is stored in
    the flash memory so even after power on reset the count will continue where
    it left off.
    Since there is only one variable stored in the flash it will take 
    ( (SECTOR_SIZE - 48) / (MAX_VARIABLE_SIZE + 4) ) records to fill the
    whole sector. The lifespan of the LPC1100 series microcontrollers flash is
    minimum of 10,000 programming and erase cycles. Because two sectors are
    used and the counter is saved every second the lifespan is 
    2*10,000*( (4096 - 48) / (12 + 4) ) = 5,060,000 seconds.
