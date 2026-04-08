########################################################################
#
#                           lwsem_twrk64f120m.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  The lwsem example code shows how lightweight semaphores works. The code
  is written in a way that two different semaphores are synchronized to
  ensure mutual exclusion of a common memory space.

  The light weight semaphores example uses four tasks:
  - 1 read_task
  - 3 write_task

  The read_task starts by creating two lightweight semaphores (WRITE_SEM and
  READ_SEM) that govern the access to a data memory location (FIFO.Data). The

    WRITE_SEM starts enabled, while the READ_SEM is disabled.
      Result = _lwsem_create(&fifo.READ_SEM, 0);
      Result = _lwsem_create(&fifo.WRITE_SEM, 1);
        Parameters:
        &fifo.READ_SEM = pointer to the lightweight semaphore to create
        0 = Initial semaphore counter

  Then 3 write task are created with initial_data equal to ‘A’, ‘B’, ‘C’ correspondingly.
  If the READ_SEM is available (_lwsem_wait), the read_task prints on the terminal
  whatever the shared memory space contains.

    Result = _lwsem_wait(&fifo.READ_SEM);
    putchar(‘\n’);
    putchar(fifo.DATA);

  Finally the WRITE_SEM is posted
  _lwsem_post(&fifo.WRITE_SEM);

  The write task verifies if the WRITE_SEM is available (_lwsem_wait), then writes at the
  shared memory space, task initial_value.

    Result = _lwsem_wait(&fifo.WRITE_SEM)
    fifo.DATA = (uchar)initial_data;

  Finally the READ_SEM is posted
  _lwsem_post(&fifo.READ_SEM);

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
