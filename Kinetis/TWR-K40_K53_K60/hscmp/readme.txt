This project is a simple high speed comparator example. It uses the IR Reciever to trigger HSCMP comparisons. This in turn triggers an interrupt that determines if the signal was higher or lower than the reference signal.

 *   P input is DAC output (1/2 VREF for this example, or 1.65V)
 *   M input is IR receiver signal on PTC6
 *
 *   This example uses IR transmitter on PTD7 to drive the
 *     IR receiver. They are angled so that the IR transmitter
 *     can drive the IR receiver on the tower board. This
 *     requires shunting J24 or J5.
 *
 *   Alternatively you can point a remote to the IR
 *     receiver and toggle the HSCMP CMPO input that way.

By default the OS-JTAG is used for the terminal output. The terminal should be configured for 115200 8-N-1.

Supported platforms:
- TWR_K40X256
- TWR_K53N512
- TWR_K60N512

The hscmp.eww file will open the project for all of the supported platforms. Pick the specific project that corresponds to your hardware.

NOTE: if switching between platforms it is a good idea to do a make clean to make sure the code is properly configured for the new platform.

