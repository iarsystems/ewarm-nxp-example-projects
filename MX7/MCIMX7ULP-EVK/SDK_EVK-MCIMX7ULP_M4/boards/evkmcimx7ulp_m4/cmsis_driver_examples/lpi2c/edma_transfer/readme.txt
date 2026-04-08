Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware
that implements for example communication stacks, file systems, or graphic user interfaces.

The cmsis_lpi2c_edma_transfer example shows how to use LPI2C CMSIS driver in EDMA way:

In this example , one lpi2c instance used as LPI2C master and another lpi2c instance used as LPI2C slave .
1. LPI2C master send data to LPI2C slave with EDMA . (LPI2C Slave using interrupt to receive the data)
2. LPI2C master read data from LPI2C slave with EDMA . (LPI2C Slave using interrupt to send the data)
3. LPI2C master abort transfer when send 7 data(LPI2C master send data to LPI2C slave with EDMA.I2C Slave using interrupt to receive the data)
4. LPI2C master read data from LPI2C slave with EDMA in nostop mode. (LPI2C Slave using interrupt to send the data) then LPI2C master read data again.
5. LPI2C master send data to LPI2C slave with EDMA in nostop mode. (LPI2C Slave using interrupt to read the data) then LPI2C master send data again.

Hardware requirements
=====================
- Micro USB cable
- MCIMX7ULP-EVK board
- I-Jet Debug Probe
- 5V power supply
- Personal Computer

Board settings
==============
The example requires doing connection between two LPI2C pins.
Please use two dupontline to touch the test point (TP) on MCIMX7ULP-EVK base board as following:
- TP24, TP25 connected
- TP26, TP27 connected

#### Please note this application can't support running with Linux BSP! ####

Prepare the Demo
================
1.  Connect 5V power supply and I-Jet Debug Probe to the board, switch SW1 to power on the board.
2.  Connect a micro USB cable between the host PC and the J6 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example

Running the demo
================
The following message shows in the terminal if the example runs successfully.

test edma transfer
Master will send data :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f


Slave received data :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f

slave will send data :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f


master received data :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f
