This project is a simple example project that uses the FlexBus external bus interface to write/read to a Magneto-resistive Random Access Memory (MRAM) device.  Communicating to an MRAM is identical to an SRAM and is very simple.

The example runs on the Tower system with the TWR-MEM module which has the MRAM device.

Supported platforms:
- TWR_K40X256
- TWR_K60N512

Required tower modules
- TWR-K40X256 or TWR_K60N512 or TWR_K53N512
- TWR-SER
- TWR-MEM

Instructions: 
On the TWR-MEM module ensure that J10 is shorted and J11 is off.
On the TWR-K40 module ensure the J16 is set to position 2-3.

Plug in the Kinetis tower module, serial module (TWR-SER), and memory module (TWR-MEM) into the tower system.


The flexbus.eww file will open the project for all of the supported platforms. Pick the specific project that corresponds to your hardware.

NOTE: if switching between platforms it is a good idea to do a make clean to make sure the code is properly configured for the new platform.

