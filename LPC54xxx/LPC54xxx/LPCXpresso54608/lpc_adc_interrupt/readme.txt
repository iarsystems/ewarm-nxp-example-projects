Overview
========

 The lpc_adc_interrupt example shows how to use interrupt with LPC ADC driver.
 
 In this example, the internal temperature sensor is used to created the input
 analog signal. When user type in any key from the keyboard, the software trigger
 API is called to start the conversion. Then it polls the flag variable which would
 be asserted when the conversion completed ISR is executed. Then just print the
 conversion result to terminal.


Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54608 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J8) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, following information can be seen on the serial terminal:

ADC interrupt example.
ADC_DoSelfCalibration() Done.
Configuration Done.
gAdcResultInfoStruct.result        = 675
gAdcResultInfoStruct.channelNumber = 0
gAdcResultInfoStruct.overrunFlag   = 0

gAdcResultInfoStruct.result        = 676
gAdcResultInfoStruct.channelNumber = 0
gAdcResultInfoStruct.overrunFlag   = 0

gAdcResultInfoStruct.result        = 676
gAdcResultInfoStruct.channelNumber = 0
gAdcResultInfoStruct.overrunFlag   = 0

...

Customization options
=====================

