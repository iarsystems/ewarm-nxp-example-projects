########################################################################
#
#                           lwevent_twrk64f120m.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  The lightweight event example code shows how a task waits for an event.
  This event can be set by any other process or interrupt in the system
  that can set the variable lwevent. The example simulates an ISR event
  using another simple task.

  The example application creates two tasks. The service_task task
  creates a lightweight event group and enters a loop in which it waits
  for an event bit. When the appropriate lwevent bit is set, it clears it
  and prints "Tick.". The simulated_ISR_task task periodically sets the
  corresponding lwevent bit with a delay in between (no connection is
  opened to a lwevent group).

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
