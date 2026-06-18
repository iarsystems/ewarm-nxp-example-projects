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
// Target: LPC2106  Directory: \MB40\LED
// Code is placed in Flash - Workspace: S1_test.eww

//------------------------------------------------------------------------

//1. This is an IAR EWARM 4.11A project. Start EWARM and open the workspace.
//   Then the source files can be compiled and Intel HEX file can be created.
//   For quick start, precompiled HEX file can be found in /Debug/Exe

//2. Use NXP ISP utility to download Intel HEX file to MB40.
//   Use jumpers default settings as per MB40 manual with the exception of JP3
//   which should be in 2-3 position so that bootloader starts after Reset.
//   After downloading, change JP3 to 1-2, press Reset button.

//3. The codes assumes that there are 4 LED diodes connected to LPC2106
//   I/O ports P0.28 to P0.31. They are switched on and off - one after another.

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
