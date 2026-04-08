########################################################################
#
#                           msd_disk_twrk64f120m.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  The msd_disk example enumerates as a USB Mass Storage Device. The demo
  uses one form of physical memory to present to the computer a memory
  drive. The USB stack enumerates in the computer host as a mass storage
  device with two bulk endpoints, one IN endpoint (device to PC) and one
  OUT endpoint (PC to device). Both endpoints are configured to use
  64 byte buffers.

  When the USB device enumerates, the host can access the device as a
  standard mass storage device, in most graphical operating systems this
  will show up as an accessible disk drive. The standard memory medium
  the demo uses is a section of RAM. Support for other storage media can
  be implemented by the user.

  The main task only calls the TestApp_Init function. The TestApp_Init
  function initializes all the application parameters. It first checks if
  the USB memory (for USB stack operation) is available and returns an
  error otherwise.

  Afterward it initializes the USB callbacks. Most important here is the
  Bulk_Transaction_Callback which handles the memory operations. The
  Bulk_Transaction_Callback runs asynchronously together with the USB
  stack interrupts and will be explained below.

  Afterwards the function passes the MSD configuration to the USB lower
  layers, which basically means copying user configurable values stored
  in macros in disk.h to the msd_config structure defined in usb_msc.h.
  These macros may be customized by user, however, deeper knowledge of
  USB protocol and the MQX USB API is recommended for this. The API
  documentation (MQXUSBDEVAPI) can be obtained from the Freescale
  website:

  www.freescale.com/mqx

  The TestApp_Ini finally initializes the stack and USB device and enters
  an infinite loop. The infinite loop calls two functions:

  USB_MSC_Periodic_Task and Disk_App.

  USB_MSC_Periodic_Task is part of the USB stack and needs to be called
  periodically.

  DiskApp is a function provided by the demo for users to write their own
  code that is dependent on USB execution. The function is only called if
  the device is properly enumerated, this is ensured by a check if the
  g_disk.start_app is TRUE.

  The rest of the memory handling is done in the
  Bulk_Transaction_Callback function. This function is called by the USB
  stack after every mass storage class event and contains a switch-case
  decoder that executes functions according to the type of the event. The
  data received and data sent events are not handled by this demo, though
  user can write custom code on them. The most relevant events are
  USB_MSC_DEVICE_READ_REQUEST and USB_MSC_DEVICE_WRITE_REQUEST these call
  the memory handling functions that write and read the physical memory
  blocks. Notice that in these two handlers there is a conditional
  compiler statement that only compiles the RAM reading and writing code
  into the code if the RAM_DISK_APP macro is set, this allows for the
  user to disable the basic demo operation and include custom memory
  handling code instead. The other relevant event is
  USB_MSC_START_STOP_EJECT_MEDIA is command designed to start or stop
  physical media, which is in particular relevant for media with slow
  start-up like hard disks.

  The workspace also contains the projects of needed libraries, which should
  be build before running the example. Simply press F8 and then batch build
  all configurations.
  Note: If the needed libraries are already build in other example, this step
  may be skipped.

  To enable symbolic names in the debugger MQX RTOS awareness dialogs
  you must download and install the complete MQX package from Freescale.

COMPATIBILITY
=============
   The example project is compatible with Freescale TWR-K64F120M board.
  By default, the project is configured to use the I-Jet SWD interface.

CONFIGURATION
=============
  Int Flash Debug   - The application is downloaded to the internal Flash for
                      debugging purposes.
  Int Flash Release - The application is downloaded to the internal Flash.

  Set the jumper positions to their defaults as described in TWR-K64F120M
  Quick Start Guide except:
    J16 - 2-3 (connect the RESET button after level)
    J18 - 3-4
    J29 - 5-6 (board will be powered from TWR-ELEV module)
    J38 - 2-3
    J39 - OFF

  The application uses the serial interface on the TWR-SER board.

  Terminal settings must be 115200 8-N-1.

GETTING STARTED
===============
  1) Build the included libraries (if needed).

  2) Select configuration.

  3) Build and download the program.

  4) Connect the board's K64_USB connector to the PC via an USB cable.
