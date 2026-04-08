Overview
========
The MMCCARD Interrupt project lpc_mmccard_interrupt is a demonstration program that uses the SDK software. It reads/writes
/erases the MMC card continusly. The purpose of this example is to show how to use MMCCARD driver and
show how to use interrupt based transfer API in SDHC driver in SDK software to access MMC card.


Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54608 board
- Personal Computer

Board settings
==============
Insert the card into the card slot

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the LPC-Link USB port (J8) on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
When the example runs successfully, following information can be seen on the OpenSDA terminal:



Read/write the card continuously untile encounters error......

Write/read one data block......
Compare the read/write content......
The read/write content is consistent.

Write/read multiple data blocks......
Compare the read/write content......
The read/write content is consistent.

Input 'q' to quit read/write/erase process. 
Input other char to read/write/erase data blocks again.



Customization options
=====================
