Example description
    
DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for NXP S32K144EVB board. 
  It shows basic use of S32K144: 
    - GPIO: External interrupt and GPIO as output
    - SysTick 
	- ADC
	- Watchdog
	- CRC: Calculate code flash memory checksum
	- NVIC
	
  It shows basic use of IAR Embedded Workbench for ARM: 
    - printf via SWO
	- Code flash memory checksum generation during link
	- Static usage analysis during link
	- FreeRTOS plugin during debug
	
   It shows basic use of FreeRTOS:
    - Event: Synchronization between task and task/interrupt
	- Notification: Synchronization between task and interrupt
	- Mutex: Shared resource protection between tasks
  
  
  The main functionality is as following:
    - Toggle LED green when switch 2 is pressed 
	- Toggle LED blue when switch 3 is pressed 
	- Turn on LED red when ADC value is greater than 1280
	- Turn off LED red when ADC value is less than 768
	- Calculate code flash memory checksum and compare with stored checksum
	- Refresh watchdog periodically
  
  Note: 
  The flash configuration field 0x00000400 ~ 0x0000040f need to be 
  configured properly. Otherwise, the calculated code flash checksum will 
  be different by linker and CRC peripheral.
  
  The prinf vis SWO is only available in debug session.
  It's not avialable in standalone mode.


COMPATIBILITY
=============
  The example project is compatible with NXP S32K144EVB.
  By default, the project is configured to use the I-Jet SWD interface.
  It can also use NXP onboard OpenSDA (PE Micro), but the printf vis SWO is not available.
  
GETTING STARTED
===============

  1) Start the IAR Embedded Workbench for ARM

  2) Connect the I-Jet

  3) Power on the board

  4) Select FreeRTOS Debug Configuration

  5) Press Ctrl+D or use the Download and Debug button to start a debug session
