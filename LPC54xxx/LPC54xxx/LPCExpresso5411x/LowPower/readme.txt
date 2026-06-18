########################################################################
#
#                          LowPower.eww
#
# $Revision: 28 $
#
########################################################################

LowPower

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for LPCXpresso5411x board. It demonstrates different power 
  saving modes - Sleep mode, Power down mode, Deep-Power down mode and 
  Deep Sleep mode.

COMPATIBILITY
=============
  The example project is compatible with LPCXpresso5411x board.

CONFIGURATION
=============
    a. Remove JS11 and connect multimeter across JP4 for making current measurments.
	b. Power the LPCXpresso board via J5 when making power measurements.
	c. Remove jumper on JP10 before making measurements.
    
    I. Sleep Mode, Power down Mode and Deep-Power down mode
        1. Frequency of CPU can be changed in platform.c file - line 16,17 and 18.
            #define SYS_CLK_RATE_HZ 12000000
            #define SYS_CLK_RATE_HZ 48000000
            #define SYS_CLK_RATE_HZ 96000000
        
        2. Make sure #define DEEP_SLEEP is commented in "core_main.c" and in 
           "platform.c".	   
        
    II. Deep Sleep Mode
    
        1. For Deep Sleep mode  uncomment "#define DEEP_SLEEP" in "core_main.c" 
           and in "platform.c".

        2. Wake up source can be Pin interrupt on PIO0_24(Switch SW1 on LPCXpresso board)
           or UTICK timer.    This can be selected in "core_main.c".
           On wake up LED D2 will be ON. 
           
        Note: In deep sleep mode power supply to Flash is off!

GETTING STARTED
===============
  1) Build and download the program.

  2) Run the program.

  3) Execute measurments

    LPCXpresso Board measurements : 
    12 mHz @3.3V ( Typical values - May change from part to part )
    Sleep mode - 850 uA
    Deep Sleep mode - 20 uA
    Deep Sleep2 mode* - 8.7 uA
    Deep power down mode - 750 nA
    
    * Power down mode is now Deep Sleep2 mode 

    Note : Since GPIO states of all pins are altered to reduce power on board, upon 
           wake up SWD/JTAG may not be available!
           
    Note : The VDDHV bit used in the software is used to power up or power down the
           supply voltage to Flash. The missing information will be documented in the
           next version of UM.