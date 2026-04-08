Purpose:
A program to implement Analog to  Digital Converter (ADC) example on LPC11Axx

Running mode:
* Compile, Flash the program and reset.
* Setup UART for 115200, 8 data bits, no parity, 1 stop bit
* Vary the Pot on board  

Note:
This example has been tested on LPC11A14 IAR SK

Specifications:
ADC pot is connected on AD7. However results of all ADC channels are displayed on the terminal

Output:

Output displays the channel number, current value, minimum, maximum values.
The number of samples until next clear is also displayed.

Ch CUR MIN MAX
0 3F7 3F0 3FF
1 285 266 397
2 1AE 053 3F7
3 3F9 3F2 3FF
4 000 000 001
5 3FF 3FF 3FF
6 1F3 1D3 3E5
7 247 000 3FF
No of Samples: 016