########################################################################
#
#                          periph_i2csint.eww
#
# $Revision: 28 $
#
########################################################################

periph_i2csint

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for LPCXpresso5411x board. The example demonstrates how to
  configure I2C as a bus slave in interrupt mode using the I2CS driver. 
     This example provides 2 simple (emulated) EEPROMs at different I2C slave 
  addresses. Both are on the same I2C bus, but the slave controller will be
  configured to support 2 slave addresses on the single bus. The emulated
  EEPROMs have their memory locations set and read via I2C write and read
  operations. Operations can be as little as a byte or continuous until the
  master terminates the transfer.
    The example also provides the master interface on the same I2C bus as the slave
  to communicate with the emulated EEPROMs without requiring an external I2C master.
  

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

  2) Connect USB to serial cable to a PC and to the board J5's TX, RX and GND. 
  
  3) Open a terminal program with the following settings: 115200 bauds,
  8 data bits, 1 stop bits, no parity.
   
  4) Run the program.
     Observe the ouput in the terminal program.