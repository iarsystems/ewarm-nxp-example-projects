########################################################################
#
#                           watchdog_twrk64f120m.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  The watchdog example code is used to demonstrate how to use a software
  watchdog for a task. It creates a soft watchdog for the task which
  restarts it in a loop. The loop time is increased each time until the
  watchdog timer expires.

  The main task first initializes a MQX_TICK_STRUCT using the following
  line:

    _time_init_ticks(&ticks, 10);

  The value 10 is used to initialize struct ticks, and the struct ticks
  will be used to restart the watchdog later in the main loop.
  Then the main task creates a watchdog component using the following
  line:

    result = _watchdog_create_component(BSP_TIMER_INTERRUPT_VECTOR,
    handle_watchdog_expiry);

  By this a software watchdog is created for the task and the handler for
  watchdog expiry is assigned. When the watchdog is not restarted in
  time, the watchdog will expire and handle_watchdog_expiry() will be
  called automatically.

  Then main task enters a loop, in the loop it first restarts the
  watchdog with the parameter ticks which was initialized in previous
  steps. And then it wastes some time - the time will be increased 10
  times every loop. Then it stops the watchdog and prints the elapsed
  time and continues with the next loop.

  When the value exceeds the watchdog expiration period,
  handle_watchdog_expiry will be called, and watchdog expiry message
  along with the task number will be printed to STDOUT.

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
