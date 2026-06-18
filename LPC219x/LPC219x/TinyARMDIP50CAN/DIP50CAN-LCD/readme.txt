//------------------------------------------------------------------------
//     	Example code for DIP50CAN LPC2194 and DEVB4050 development board
//------------------------------------------------------------------------
//					
//------------------------------------------------------------------------
// -------------------------------------------------------------------
// THIS EXAMPLE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------
// Target: LPC2194  Directory: \DIL50CAN LCD\
// Code is placed in Flash - Workspace: S3_Flash.eww
// Code is placed in Ram   - Workspace: S3_Ram.eww (Start = 0x40000200)
//------------------------------------------------------------------------

//1. This is an IAR EWARM 4.40A project. Start EWARM and open the workspace.
//   Then the source files can be compiled and Intel HEX file can be created.
//   For quick start, precompiled HEX file can be found in /Debug/Exe

//2. Use Philips ISP utility to download Intel HEX file to DEVB4050.
//   Use jumpers default settings as per DEVB4050 manual with the exception of JP3
//   which should be in 2-3 position so that bootloader starts after Reset.
//   After downloading, change JP3 to 1-2, press Reset button.

//3. The code assumes that EM Marine LCD is connected on the DEVB4050 motherboard.
//   The program keeps displaying all ASCII characters (codes 0x20 to 0x7F)
//   again and again.

//------------------------------------------------------------------------
// Notes for your own software development:
//1. Start by compiling projects provided by Pasat first.
//2. Use Pasat provided link file DIP50CAN Flash.xcl or DIP50CAN Ram.xcl. 
//   To select link file in EWARM:
//      Click Project->Options->Linker->Config
//      Check Linker Command File->Override Default
//      Type $PROJ_DIR$\DIP50CAN Flash.xcl as linker file (code placed in LPC2194 Flash)
//      or $PROJ_DIR$\DIP50CAN Ram.xcl if you wish to plac code in LPC2194 Ram.
//   To select Intel HEX file as linker output in EWARM:
//      Click Project->Options->Linker->Output
//      Check Output File->Override Default, type output file name
//      Select Format->Other, Output Format: Intel Extended
