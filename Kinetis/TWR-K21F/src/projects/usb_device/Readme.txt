********************************************************************************
		Release notes for usb device demo for 50MHz kinetis family
		        Freescale Semiconductor February 2012   
********************************************************************************

Demo code runs on the TWR-K21D50M module.

usb_device example emulates a COM port in a Windows HOST using the Comunication Device Class (CDC). The USB is recognized as a standard COM port and creates an echo aplication. This means that everything that goes out from the terminal returns to the terminal 

===========
Basic Setup
===========

TWR-K21D50M setup
----------------
5-6	ON

================
Demo explanation
================
Download the code and run it.
After the micro USB and the PC are connected, the PC will request a driver, point to the Freescale_CDC_Driver_kinetis.inf file located in the usb_device project folder. 

In the Device Manager window a Freescale CDC device will be found after the enumeration process is completed.

After that just start a hyperterminal opening the COM created by the MCU and start typing. All characters comes back immediately.

================
Known problem
================
Re-enumeration for this demo is not supported, for TWR-K21D50M does not have circuit to detect VBUS connection.
