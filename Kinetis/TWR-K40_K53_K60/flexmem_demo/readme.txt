This project is a FlexMemory example. If the device has not been partitioned, then it will be configured for a 32 byte eEEPROM backed up by 128KBytes of flash. The example has three counter values--one of each data size (longword, word, and byte). All three counters are stored in the enhanced EEPROM memory. They will be zeroed out during partitioning otherwise they are left at their previous values. Next the PIT is configured to generate an interrupt every 15 seconds. In the PIT interrupt handler all three of the counters are incremented and the new values for the counters are printed to the terminal. This mimics how a data logger application might work where at some set interval of time data values are saved off.

By default the OS-JTAG is used for the terminal output. The terminal should be configured for 115200 8-N-1.

Supported platforms:
- TWR_K40X256
- TWR_K53N256 (NOTE: the K53N512 device does not support FlexMemory. The project is still included though, since the processor on the board could be changed either in a socket or reworked to swap out with a device that does support FlexMemory.)
- TWR_K60N512 (NOTE: the K60N512 device does not support FlexMemory. The project is still included though, since the processor on the board could be changed either in a socket or reworked to swap out with a device that does support FlexMemory.)

The flexmem_demo.eww file will open the project for all of the supported platforms. Pick the specific project that corresponds to your hardware.

NOTE: if switching between platforms it is a good idea to do a make clean to make sure the code is properly configured for the new platform.

