This project demonstartes how to configure the MCG in various modes. 

By default the P and E Micro virtual serial port is used for the terminal output. The terminal should be configured for 19200 8-N-1. This baud rate is different from most other demonstration projects due to the slow clock frequencies used. The baud rate should be set in the hardwate specific kxx_tower.h file.

Supported platforms:
- TWR_K40X256
- TWR_K53N512
- TWR_K60N512

The mcg_demo.eww file will open the project for all of the supported platforms. Pick the specific project that corresponds to your hardware.

NOTE: if switching between platforms it is a good idea to do a make clean to make sure the code is properly configured for the new platform.

