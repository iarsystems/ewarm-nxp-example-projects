Overview
========
The CRC Example project (lpc_crc) is a demonstration program that uses the KSDK software to generate checksums
for an ASCII string. Several CRC protocols are implemented using the CRC driver API.

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54608 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
Connect a serial cable from the debug UART port of the board to the PC. Start a terminal program
 and make a connection to the virtual serial port.

1. Start a terminal program
2. New connection -> Serial
3. Set apropriate COMx port (x is port number) in Port context menu. Number is provided by operation
   system and could be different from computer to computer. Select COM number related to virtual
   serial port. Confirm selected port by OK button.
4. Set following connection parameters in menu Setup->Serial port...
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: one
5.  Confirm selected parameters by OK button.

Running the demo
================
Example output on terminal:


CRC Peripheral Driver Example

Test string: 123456789
CRC-16 CCITT FALSE: 0x29b1
CRC-16 ARC: 0xbb3d
CRC-32: 0xcbf43926


Customization options
=====================

