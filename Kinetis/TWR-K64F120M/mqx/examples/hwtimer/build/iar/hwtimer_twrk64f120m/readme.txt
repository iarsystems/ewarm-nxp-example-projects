########################################################################
#
#                           hwtimer_twrk64f120m.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  The example hwtimer handles two different hwtimers instances with the
  callback functionality. It shows how to work with the component and how
  to use API functions.

  There is just one task (hwtimer_task) and callback function for each
  hwtimer (hwtimer1_callback, hwtimer2_callback).

  hwtimer_task:
  - Create job_done lw semaphore.
  - Initialize hwtimer1.
  - Set the frequency of 250Hz for hwtimer1.
  - Register callback hwtimer1_callback() for hwtimer1.
  - Initialize hwtimer2.
  - Set the period of 1000000ms for hwtimer2.
  - Register callback hwtimer2_callback() for hwtimer2.
  - Start hwtimer2.
  - Wait on job_done lw semaphore.
  - Deinitialize hwtimer1 and hwtimer2.
  - Destroy job_done lw semaphore.

  hwtimer1_callback:
  - Print “.”

  hwtimer2_callback:
  - Store ticks of hwtimer2.
  - After 1s (ticks == 1) start hwtimer1.
  - After 1s (ticks == 2) block callback of hwtimer1.
  - After 2s (ticks == 4) unblock callback of hwtimer1.
  - After 2s (ticks == 6) stop both hwtimers and post job_done lw semaphore.

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
