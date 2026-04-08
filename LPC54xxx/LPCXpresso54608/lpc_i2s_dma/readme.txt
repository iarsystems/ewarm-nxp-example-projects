Overview
========

The I2S example project lpc_i2s_dma uses one I2S interface to continuously record
input sound to a buffer and another I2S interface to playback the buffer to output - digital loopback.
The example uses the DMA controller to handle tx and rx events.

It requires NXP Mic/Audio/Oled Shield with WM8904 codec.
It needs to have 1-2 connected on JP3.

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54608 board
- Personal Computer
- headphones with 3.5 mm stereo jack
- source of sound (line output to 3.5 mm stereo jack)


Board settings
==============
No special settings are required on LPCXpresso54608 board.

Prepare the Demo
================
Connect a serial cable from the debug UART port of the board to the PC. Start a
terminal program and make a connection to the virtual serial port.

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
5. Confirm selected parameters by OK button.
6. Download the program to the target board.
7. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Customization options
=====================

