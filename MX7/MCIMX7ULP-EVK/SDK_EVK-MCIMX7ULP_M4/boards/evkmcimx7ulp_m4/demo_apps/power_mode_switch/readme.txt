Overview
========
The Power mode switch demo application demonstrates the use of power modes in the KSDK. The demo prints the power mode menu
through the debug console, where the user can set the MCU to a specific power mode. The user can also set the wakeup
source by following the debug console prompts. The purpose of this demo is to show how to switch between different power
 modes, and how to configure a wakeup source and wakeup the MCU from low power modes.

 Tips:
 This demo is to show how the various power mode can switch to each other. However, in actual low power use case, to save energy and reduce the consumption even more, many things can be done including:
 - Disable the clock for unnecessary module during low power mode. That means, programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
 - Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux, and so on.
 - Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, the pin cirrent would be activied unexpectedly waste some energy.
 - Other low power consideration based on the actual application hardware.

Hardware requirements
=====================
- Micro USB cable
- MCIMX7ULP-EVK board
- I-Jet Debug Probe
- 5V power supply
- Personal Computer

Board settings
==============
No special settings are required.

#### Please note that this application must be built with ram link file! ####

#### Please note this application can support running with Linux BSP! ####

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
4.  Generate image file with imgutil and write to QSPI flash with U-Boot. For details, please refer to Getting Started with SDK v.2.0 for i.MX 7ULP Derivatives (Doc No: SDK20IMX7ULPGSUG)
5.  Press the reset button on your board to begin running the demo.

Running the demo
================
When running the demo, the debug console shows the menu to command the MCU to the target power mode, or query and set other system power behavior. The power mode switch demo need to run with Linux accordingly, so please make sure the power mode crossings between Cortex-A7 and Cortex-M4 are valid.

NOTE: Only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow and receive the wrong input value.
~~~~~~~~~~~~~~~~~~~~~
Power Mode Switch Task Started!!!
PMIC Remote Service Started!!!
PMIC Local Service Started!!!

SIM_DGO_GP1 change from 0x0 to 0x1ffd2c49

MCU wakeup source 0x6...

####################  Power Mode Switch Task ####################

    Core Clock = 98181818Hz
    Power mode: RUN

Select the desired operation

Press  A for enter: RUN      - Normal RUN mode
Press  B for enter: WAIT     - Wait mode
Press  C for enter: STOP     - Stop mode
Press  D for enter: VLPR     - Very Low Power Run mode
Press  E for enter: VLPW     - Very Low Power Wait mode
Press  F for enter: VLPS     - Very Low Power Stop mode
Press  G for enter: HSRUN    - High Speed RUN mode
Press  H for enter: VLLS     - Very Low Leakage Stop mode
Press  Q for query CA7 core power status.
Press  W for wake up CA7 core in VLLS.
Press  R for read PF1550 Register.
Press  S for set PF1550 Register.

Waiting for power mode select..
~~~~~~~~~~~~~~~~~~~~~
