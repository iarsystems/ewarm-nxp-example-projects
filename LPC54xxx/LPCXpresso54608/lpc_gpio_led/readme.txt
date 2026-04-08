Overview
========
The lpc_gpio_led is a demonstration program that uses the KSDK software
to manipulate the general-purpose outputs. The example use LEDs and buttons to
demonstrates GPIO API for port and pin manipulation (init, set, clear, and toggle).


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
Connect a serial cable from the debug UART port of the board to the PC. Start a terminal
program and make a connection to the virtual serial port.

1. Start a terminal program
2. New connection -> Serial
3. Set appropriate COMx port (x is port number) in Port context menu. Number is provided by operation
   system and could be different from computer to computer. Select COM number related to virtual
   serial port. Confirm selected port by OK button.
4. Set following connection parameters in menu Setup->Serial port...
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: none
5.  Confirm selected parameters by OK button.


Running the demo
================
After the board is flashed the following output could be observed on the terminal:

Example output:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~





 GPIO Driver example

 The LED is taking turns to shine.

 Standard port read: ffffffff

 Masked port read: ffff0000

 Port state: fffffffd

 Port state: ffffffff

 Port state: ffffffff

 Port state: ffffffff

 Port state: ffffffff

 Port state: ffffffff
 ...
 
 
 
 
 
 
 
 
 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================

