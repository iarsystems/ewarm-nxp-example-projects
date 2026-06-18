//------------------------------------------------------------------------
//		Example code for TinyARM DIP40 LPC2106 module
//------------------------------------------------------------------------
//					
//------------------------------------------------------------------------
// -------------------------------------------------------------------
// THIS EXAMPLE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------
// Target: LPC2106  Directory: \LPC210x
// a. Code placed in RAM   - Workspace: Ex_Ram210x.eww
// b. Code placed in Flash - Workspace: Ex_Flash2106.eww

//------------------------------------------------------------------------

//1. This is an IAR EWARM 4.11A project. Start EWARM and open the project.
//   Then the source files can be compiled and Intel HEX file can be created.
//   For quick start, precompiled HEX file can be found in /Debug/Exe

//2. There are two projects in the directory. The only difference is that
//   one compiles the code for LPC2106 Flash and the other for LPC2106 RAM.

//3. The codes assumes that there are 32 LED diodes connected to LPC2106
//   I/O ports. They are switched on and off - one after another.
