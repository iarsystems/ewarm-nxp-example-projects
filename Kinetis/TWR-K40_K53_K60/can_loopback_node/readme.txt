This project is a simple can_loopback_node example. It requires at least two CAN nodes: 
Node A is running sci2can or any other CAN application with internal loopback disabled;
Node B is running can_loopback_node code. 
The example echoes all messages to the remote node (Node A).
The default CAN bit rate is 83.33K.

 By default the serial port on the tower serial card is used for
 the terminal output. The terminal should be configured for 115200 8-N-1.

Supported platforms:
- TWR_K40X256
- TWR_K60N512

The can_loopback_node.eww file will open the project for all of the supported platforms. 
Pick the specific project that corresponds to your hardware.

NOTE: if switching between platforms it is a good idea to do a make clean to make sure the code
 is properly configured for the new platform.

Instructions on how to play with sci2can node (Node A):
1) Open sci2can.eww workspace, and change both FLEXCAN_SELF_RECEPTION and FLEXCAN_LOOP_BACK macro values to 0 in can_config.h as below: 
#define FLEXCAN_SELF_RECEPTION    0
#define FLEXCAN_LOOP_BACK         0

2) Select "FLASH_256KB_PFLASH_256KB_DFLASH" as the active configuration for the project;
3) Rebuild the whole project;
4) Select "Project->download and debug";
5) Select "Debug->Go" to run the code;
6) Switch off the power to the tower system Node A and disconnect the JTAG connection from the emulator and PC to the Node A;
7) Connect one end of the CAN cable to J7 on TWR-SER module of the Node A tower system , 
the other end to the J7 on another set of TWR-SER module of K40 or K60 tower system (Node B);
8) Connect JTAG connector from the emulator to the tower system Node B and PC;
9) switch on the power to the tower system Node B;
10) open can_loopback_node workspace by double-clicking on can_loopback_node.eww
11) Select "FLASH_256KB_PFLASH_256KB_DFLASH" as the active configuration for the project;
12) Rebuild the whole project and then select "Project->download & debug";
13) Select "Debug->Go" to run the code on the tower system Node B;
14) switch on the power to the tower system Node A to run sci2can demo;
15) follow aforementioned instructions to play with sci2can demo;

