Overview
========
The bubble application demonstrates basic usage of the on-board accelerometer to implement a
bubble level. A bubble level utilizes two axes to visually show deviation from a level plane
(0 degrees) on a given access. This demo uses the TPM to modulate the duty cycle of two pwm 
to gradually change as the board deviates from a level state.

Hardware requirements
=====================
- Micro USB cable
- MCIMX7ULP-EVK board
- I-Jet Debug Probe
- 5V power supply
- Oscilloscope
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
5.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the board is programmed, use oscilloscope probe to touch  
the R104/R107 arounding U9 module position, then simply tilt the board to see  the duty cycle change of the pwm.
One pwm indicates X-axis angle while another indicates Y-axis angle.
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~
Welcome to BUBBLE example

You will see the duty cycle change of the two PWM when change angles of board

x=     23 y =     56
x=      9 y =     53
x=     14 y =     49
......
~~~~~~~~~~~~
