This project is a simple General Purpose Input/Output (GPIO) example. It will blink on and off LED's E3 and E4. Pressing the push buttons on the Tower boads will cause E1 and E2 to light on and off as well. 

By default the OS-JTAG is used for the terminal output. The terminal should be configured for 115200 8-N-1.

Supported platforms:
- TWR_K40X256
- TWR_K53N512
- TWR_K60N512

The gpio.eww file will open the project for all of the supported platforms. Pick the specific project that corresponds to your hardware.

NOTE: if switching between platforms it is a good idea to do a make clean to make sure the code is properly configured for the new platform.

