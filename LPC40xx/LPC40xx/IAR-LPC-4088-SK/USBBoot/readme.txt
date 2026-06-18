########################################################################
#
#                           USBBoot.eww
#
# $Revision: 28 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for IAR-LPC-4088-SK board.  The USB Bootloader example
 implements USB Mass Storage Device and enables both, the microcontroller's flash
 memory and the onboard SPIFI NOR, to be accessed by PC via USB.
 The first 24KB of the microcontroller's flash are used by the USB bootloader.
 The rest of the flash can be used by a user program. On start up the USB bootloader
 will check for valid user program at address 0x00006000. If there is no user code
 or BUT1 button is pressed (if CRP3 is selected the bootloader will not check for
 pressed BUT1 button) the USB Mass Storage is starded. Else the User program is started.
  
 User Program Build:
  In order to be able to load user program with the USB bootloader you should do:
  1. Link the user program at start address 0x6000. You can do this by simply
     using the <installation-root>\arm\examples\NXP\LPC40xx\IAR-LPC-4088-SK
     \common\config\LPC4088_UserFlash.icf file.
  2. Build a binary image of the user program: Select Project\Options\Output Converter
     Check the "Generate additional output" box and select "binary"
 
 To download a user program and SPIFI image:
  1. Program the USB Bootloader example on the target board (IAR-LP4088-SK). 
  2. Reset the board with BUT1 button pressed.
  3. Connect the board to PC with USB cable. Two disk drives will appear on your PC.
     If you connect it for a first time Windows will automatically load the proper
     device driver. The microcontroller's flash device name depends of the Code Read
     Protection level (CPR) you have selected for USB Bootloader example. The SPIFI 
     NOR flash device is named simlpy SPIFI FLASH
  4. The CPR disk drive contains two files USBOOT.BIN and FIRMWARE.BIN. Delete the 
     FIRMWARE.BIN file (this will erase the user flash array).
  5. Copy the user program bin file to the "CRP " disk.
  6. The SPIFI FLASH disk drive contains only one file SPIFIIMG.BIN. Delete it to erase
     the SPIFI NOR flash memory. Copy a spifi image file to SPIFI FLASH disk.

  7. Reset the board. The user program should start.
  
  To load new user program repeat from step 2.
  
 You can find TouchDemo, AudioDevice, LCD demo and SPIFIIMG bin files at
    $PROJ_DIR$\arm\examples\NXP\LPC40xx\IAR-LPC-4088-SK\USBBoot\bin
  
 Note: The USBBOOT.BIN can be also delited from the disk. This will erase the first
 24KB flash (if CRP is disabled) or the whole flash memory (if CRP1, CPR2 or CRP3 
 is enalbed). Since the USB Bootloader resides in the first 24KB flash the demo will
 hang. This however can be used to disable CRP if it was previously enabled. The other
 way is to use ISP.
 
 Note: If CRP3 is selected, no future factory testing can be performed on the device

COMPATIBILITY
=============

   The example project is compatible with IAR-LPC-4088-SK board. 
  By default, the project is configured to use the J-Link SWD interface.

CONFIGURATION
=============
  Flash Debug - The Progam is loaded to internal Flash.
   
 Jumpers:
   PWR_SEL    - depending of power source
   ISP_E      - unfilled
   RST_E      - unfilled
  
 Note:
    The LCD shares pins with Trace port. If ETM is enabled the LCD will not
    work.
  
GETTING STARTED
===============

  1) Open the  USBBoot work space.
  
  2) Power the board and connect the debugger probe.

  3) Build the  USBBoot example. Press CTRL+D or use Download and
     Debug button to start a debug session.
 
  4) While holding BUT1 press F5 or use the GO button to run the example.
  
  5) Use USB-mini cable to connect the IAR-LPC4088-SK board to a PC.
  
  6) The CRP drive and SPI FLASH drive will apear appear in your PC configuration 
