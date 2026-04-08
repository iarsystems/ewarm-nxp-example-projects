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
// Target: LPC2194  Directory: \DIL50CAN LED\
// a. Code placed in RAM   - Workspace: Ex_Ram.eww (Start = 0x40000200)
// b. Code placed in Flash - Workspace: Ex_Flash.eww

//------------------------------------------------------------------------

//1. This is an IAR EWARM 4.40A project. Start EWARM and open the project.
//   Then the source files can be compiled and Intel HEX file can be created.
//   For quick start, precompiled HEX file can be found in /Debug/Exe

//2. There are two projects in the directory. The only difference is that
//   one compiles the code for LPC2194 Flash and the other for LPC2194 RAM.

//3. The codes assumes that there are 47 LED diodes connected to LPC2194
//   I/O ports. They are switched on and off - one after another.
