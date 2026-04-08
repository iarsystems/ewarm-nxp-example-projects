This project is a simple Analog to Digital Converter (ADC)  example that shows how to trigger the ADC conversions with the Programable Delay Block (PDB).


By default the OS-JTAG is used for the terminal output. The terminal should be configured for 115200 8-N-1.

If you hit any key, the program will end.  Output will be the conversions from the ADC, and one filtered output, all on one line.
That line will update about once a second, as slowly scheduled by the PDB.  

If you do not type anything it will continue forever.  This gives you a chance to play with the Potentiometer (POT).

To vary the conversion results seen on the screen, operate the black coin edge of the POT on the bottom of the board.  See if you can hit every code from zero to 64k-1.  The codes will be in decimal.

For further information, please refer to the Kinetis QRUG chapter "Using the Peripheral Delay Block (PDB) to schedule conversions of the Analog to Digital Converter (ADC) for Kinetis Microcontrollers" 

Supported platforms:
- TWR_K40X256
- TWR_K60N512

The adc_demo.eww file will open the project for all of the supported platforms. Pick the specific project that corresponds to your hardware.

NOTE: if switching between platforms it is a good idea to do a make clean to make sure the code is properly configured for the new platform.

