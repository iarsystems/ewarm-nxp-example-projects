########################################################################
#
#                           msg_twrk64f120m.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  This client/server model shows communication and task synchronization
  using message passing.
  Server task blocks waiting for a request message from Client task. When
  Server receives the request, it executes the request and returns the
  message to Client. Two-way message exchange is used, in order to block
  Client while Server runs.

  Server opens an input message queue that it will use to receive
  requests from Client tasks and creates a message pool, from which it
  allocates request messages. Server then creates a number of Client
  tasks. In a real application, the Client tasks most likely would not be
  created by Server.

  When Server has opened its message queue and created its message pool,
  it enters a loop, receiving messages from the message queue, acting on
  them (in this case, printing the data), and returning the message to
  Client.

  Client also opens a message queue. It allocates a message from the
  message pool, fills in the message field, sends the message to Server,
  and waits for a response from Server.

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
