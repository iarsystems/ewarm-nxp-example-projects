
********************************************************************************
    Release notes for low power App for K21 kinetis family
		  Freescale Semiconductor August 2012   
********************************************************************************

Demo code runs on the TWR-K21D50M module.

The objective of this demo is to get familiar with the MCU and useful modules Like ADC,
I2C, UART and LPTMR all of them working on low power mode. 

The operation mode selected for this demo is LLS this to avoid the reset wake up on deeper operations modes.

Basic function the LPTMR will wake up the MCU every 100ms depending on the selected application the Potentiometer 
or the Accelerometer will be read.
 
In the case of the potentiometer the MCU must go back to sleep if the read voltage is under 2v. For the accelerometer
the MCU goes back to sleep while the tower board is on vertical position (normal tower position). If those conditions 
are not fulfilled a message must be send thru UART every second and LED D8 on the tower must be toggling. 


==============
What you need:
==============
1)	IAR Embedded Workbench for ARM v6.40 or CodeWarrior MCU V10.2 (with MCU 10.2 Kinetis 50MHz Service Pack installed)
2)	Serial Terminal 

===========
Basic Setup
===========

TWR-K21D50M setup
----------------
Use J8 jumper on pins 1-2 to measure current consumption.
 

================
Demo explanation
================

1. Download the code to MCU. Make sure to disconnect the debugger by stopping debugging or with a power on reset.

2. Open the Serial Terminal at 115200 bps with no parity and 8 bit size.

3. Reset the board. A prompt will appear showing reset information and the menu to select the component (Potentiometer or Accelerometer).

4. Select the device you prefer. Remember to push reset button if you want to back to principal menu.

5. Play with potentiometer or the tower position to verify the current when the MCU is on run mode or in LLS waking every 100ms. 

6. Try with the other component. 