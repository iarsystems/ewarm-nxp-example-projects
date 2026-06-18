########################################################################
#
#                           mutex_twrk64f120m.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  The mutex example code is used to demonstrate how to use a mutex to
  synchronize two tasks. It creates a mutex and two tasks. Both tasks use
  STDOUT to print out messages. Each task will lock the mutex before
  printing and unlock it after printing to ensure that the outputs from
  tasks are not mixed together.

  The main task first initializes the mutex with the following line:
  _mutex_init(&print_mutex, &mutexattr)

  Then the main task creates a print task with parameter sting1, and
  creates a second print task with parameter strings with the following
  line:
  _task_create(0, PRINT_TASK, (uint_32)string1);
  _task_create(0, PRINT_TASK, (uint_32)string2);
  Then the main task blocks itself.

  The two print tasks both use the STDOUT. If they used it in the same
  time, there would be conflicts, so a mutex is used to synchronize the
  two tasks.

  Each print task will try to lock the mutex before printing the message;
  it will wait for the mutex as long as needed. Once the mutex is locked
  it prints the message and then unlocks the mutex so that the other task
  can lock it. This process is repeated indefinitely.

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
