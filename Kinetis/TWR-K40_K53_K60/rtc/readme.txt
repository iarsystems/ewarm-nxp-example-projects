This project demonstrates how to use the real time clock (rtc). It has an example of generating an alarm every second with no time compensation. 

The terminal should be configured for 115200 8-N-1.

Supported platforms:
- TWR_K40X256
- TWR_K53N512
- TWR_K60N512

The rtc.eww file will open the project for all of the supported platforms. Pick the specific project that corresponds to your hardware.

NOTE: if switching between platforms it is a good idea to do a make clean to make sure the code is properly configured for the new platform.

