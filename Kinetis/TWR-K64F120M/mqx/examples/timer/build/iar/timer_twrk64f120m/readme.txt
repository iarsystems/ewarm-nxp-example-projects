########################################################################
#
#                           timer_twrk64f120m.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  The example timer is a simple demonstration of MQX timer component.
  It creates two timers, each timer has a 2 period seconds
  with an offset of 1 second between them. Task runs for 6 seconds.

  The main task starts to create timer component with more stack size in
  order to handle printf() requirements.

  Two timers (on_timer and off_timer) are created with periodic timer
  type, which expire repeatedly at a specified interval. The code to do
  that is:

    on_timer = _timer_start_periodic_at_ticks(LED_on, 0,
      TIMER_ELAPSED_TIME_MODE, &ticks, &dticks);
    _time_add_sec_to_ticks(&ticks, 1);
    off_timer = _timer_start_periodic_at_ticks(LED_off, 0,
      TIMER_ELAPSED_TIME_MODE, &ticks, &dticks);

  Off_timer is 1 second offset with on_timer.

  After 6 seconds delay time, the following code is used to cancel two
  created periodic timer (on_timer & off_timer).

    _timer_cancel(on_timer);
    _timer_cancel(off_timer);

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
