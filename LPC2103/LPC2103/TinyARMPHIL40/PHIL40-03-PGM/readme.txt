------------------------------------------------------------------------
	Example code for TinyARM PHIL40-03 LPC2103 board
------------------------------------------------------------------------
// -------------------------------------------------------------------
// THIS EXAMPLE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

1. This is an IAR EWARM 4.40A project. Start EWARM and open the project.
   Then the source files can be compiled and Intel HEX file can be created.

2. Assignment of LPC2103 resources:
   UART0 is used for communication with Philips ISP program running on PC.
   UART1 is used for communication with Terminal Emulator running on PC.
   SPI is used to program SPI 25C256 memory. 
   P0.10 is used as a Chip Select for SPI 25C256 memory
   I2C is used to program I2C 24C256 memory
   Note: P0.07 must be pulled up to 3.3V because Philips LPC2103 SPI peripheral
         requires it.
	 Address range for serial memory is limited to 0-7FFH internally due to 
         limited RAM on LPC2103. 

3. 
A. Start Philips ISP
B. Start Terminal Emulation program, set it to 9600Bd, 1Stop bit, no parity
C. Burn Pgm2103.hex into TinyARM PHIL40-03 LPC2103
D. Start the program from 0x200
E. The following text should be seen on the Terminal:
   TinyARM  SPI&I2C Programmer
   >Select command (Space,Backspace):
    Edit RAM
F. Use SPACE or BACKSPACE to see other options:
    Edit RAM          
    Copy 24C256 -> RAM
    Copy RAM -> 24C256
    Copy 25C256 -> RAM
    Copy RAM -> 25C256
G. If Edit RAM is selected then first specify start address and press ENTER.
   Then it is possible to edit byte. Press ENTER to accept the value and to
   advance to next address.
   Press ESC to abort editing.
H. Copy 24C256 -> RAM will copy the whole content of 24C256 memory to LPC2106
   internal RAM buffer.
I. Copy RAM -> 24C256 will copy internal RAM buffer of LPC2103 to 24C256 memory.
J. Copy 25C256 -> RAM will copy the whole content of 25C256 memory to LPC2106
   internal RAM buffer.
K. Copy RAM -> 25C256 will copy internal RAM buffer of LPC2103 to 25C256 memory.

4. Circuit diagram - see Pgm2103.pdf
