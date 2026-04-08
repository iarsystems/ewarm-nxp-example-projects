########################################################################
#
#                           freq_change_twrk64f120m.eww
#
# $Revision: 16 $
#
########################################################################

DESCRIPTION
===========
  The example shows switching between several predefined clock configurations.
  The core and serial driver are being notified and their settings are updated
  based on new clock configuration transparently for the application. The switch
  is initiated by the on-board button press. New clock settings are displayed
  on terminal and the change is also visible on the rate the LED toggles.

  The Frequency Change application belongs to the set of examples of MQX low power
  support. The MQX_ENABLE_LOW_POWER macro must be set to non-zero in the user_config.h
  file prior to compilation of MQX kernel libraries and the example itself.

  The application example creates three tasks:

    main:
      task sets up the BUTTON and waits for the BUTTON press event in an
      endless loop. Any time the button is pressed this task changes the
      clock configuration and prints out new clock settings.

    led_task:
      sets up the LED1 and toggles its state once per second regardless
      of the current clock configuration. This is a demonstration of
      clock-independent task.

    for_loop_led_task:
      sets up the LED2 and toggles its state with a frequency given by
      current clock configuration. You will see a change of the clock
      configuration as a change of LED2 blink rate.

  The LWGPIO driver is used for the LED and BUTTON pins handling. The button_init()
  function initializes the corresponding pins for input and output and it also
  installs the falling-edge interrupt for the button (with internal pull-up resistor
  enabled). The ISR handler sets the event to trigger an action in the main task. The
  clock settings are changed using the Low-power Manager API function
  _lpm_set_clock_configuration(). The parameter of this function is one of the BSP
  constants that correspond to predefined clock configurations.

  The actual frequencies are obtained using the Clock Manager API function
  _cm_get_clock(). The parameters are (the current) clock configuration and an
  identifier of the clock source whose clock frequency is to be obtained.

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
