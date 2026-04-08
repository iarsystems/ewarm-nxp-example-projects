This project is a simple Touch Sense Interface (TSI) example. It toggles on and off the LEDs when the touch pads E1-E4 are touched. It uses the TSI interrupt to debounce the touches and toggle the LEDs on and off. 

Note that the Touch Sensing Software (TSS) library should be used for more robust touch software development and for enhanced features. See more details at the TSS website: http://www.freescale.com/tss. Kinetis requires TSS 2.5 or later. 

Supported platforms:
- TWR_K40X256
- TWR_K53N512
- TWR_K60N512

The tsi.eww file will open the project for all of the supported platforms. Pick the specific project that corresponds to your hardware.

NOTE: if switching between platforms it is a good idea to do a make clean to make sure the code is properly configured for the new platform.
