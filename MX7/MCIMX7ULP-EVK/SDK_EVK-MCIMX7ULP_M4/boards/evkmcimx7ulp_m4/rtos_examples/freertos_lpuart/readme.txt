Overview
========
The LPUART example for FreeRTOS demonstrates the possibility to use the LPUART driver in the RTOS.
The example uses single instance of LPUART IP and writes string into, then reads back chars.
After every 4B received, these are sent back on LPUART.

Hardware requirements
=====================
- Micro USB cable
- MCIMX7ULP-EVK board
- I-Jet Debug Probe
- 5V power supply
- Personal Computer

Board settings
==============
No special settings are required.

#### Please note this application can't support running with Linux BSP! ####

Prepare the Demo
================
1.  Connect 5V power supply and I-Jet Debug Probe to the board, switch SW1 to power on the board.
2.  Connect a micro USB cable between the host PC and the J6 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal. You can also send characters to the console back and they will be printed out onto console in a group of 4 characters.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FreeRTOS LPUART driver example!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
