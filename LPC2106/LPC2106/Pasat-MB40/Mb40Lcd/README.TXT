//------------------------------------------------------------------------
//      	Example code for MB40 LPC2106 development board
//------------------------------------------------------------------------
//					
//------------------------------------------------------------------------
// -------------------------------------------------------------------
// THIS EXAMPLE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------
// Target: LPC2106  Directory: \MB40\MB40LCD
// Code is placed in Flash - Workspace: Mb40Lcd.eww

//------------------------------------------------------------------------

//1. This is an IAR EWARM 4.11A project. Start EWARM and open the workspace.
//   Then the source files can be compiled and Intel HEX file can be created.
//   For quick start, precompiled HEX file can be found in /Debug/Exe

//2. Use NXP ISP utility to download Intel HEX file to MB40.
//   Use jumpers default settings as per MB40 manual with the exception of JP3
//   which should be in 2-3 position so that bootloader starts after Reset.
//   After downloading, change JP3 to 1-2, press Reset button.

//3. The code assumes that LCD and PS/2 keyboard is fitted (for local control).
//   Alternatively use RS232 cable to connect with the PC for remote control.

//4. After the program starts, it displays the message to LCD:
//	"TinyARM  LPC2106 "
//	"--Demonstration--" 
//	"--To start demo--" 
//	"Press MB40 key   " 
//	"or               " 
//	"Press PC key     " 
//   and similar message to the PC Hyperterminal window:
//	"TinyARM  LPC2106"
//	"--Demonstration--"
//	"--To start demo--" 
//	"Press key either on MB40 or PC keyboard
//   If you press key on local PS/2 keybard connected to MB40 then the control
//   is passed to PS/2 & LCD.
//   If you press key on a keyboard connected to the PC then the control is
//   passed to the PC.
//   Now follow the text on the LCD or Hyperterminal window.
//   You can do one of the following things:
//   - edit 32 kbyte buffer
//   - read I2C 24C256 EEPROM
//   - program I2C 24C256 EEPROM
//   - read SPI 25C256 EEPROM
//   - program SPI 25C256 EEPROM
//   - show/set time
//   - blink LEDs
//   - edit on LCD screen
//   - perform speed tests

//------------------------------------------------------------------------
// Notes for your own software development:
//1. Start by compiling projects provided by Pasat first.
//2. Use Pasat provided link file Pgm_v01.xcl. If necessary, open it and modify.
//   Note: IAR EWARM linker default file is lnkarm.xcl. That file is not for
//         NXP LPC21xx family !!! Do not use it.
//   To select link file in EWARM:
//      Click Project->Options->Linker->Config
//      Check Linker Command File->Override Default
//      Type $PROJ_DIR$\Pgm_v01.xcl as linker file (code placed in LPC2106 Flash)
//   To select Intel HEX file as linker output in EWARM:
//      Click Project->Options->Linker->Output
//      Check Output File->Override Default, type output file name
//      Select Format->Other, Output Format: Intel Extended
