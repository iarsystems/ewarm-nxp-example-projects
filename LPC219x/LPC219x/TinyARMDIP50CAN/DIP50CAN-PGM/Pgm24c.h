//---------------------------------------------------------------------------
// Filename: Pgm24C.H
//---------------------------------------------------------------------------
// Headers for I2C routines for 24C256 programming
// V0.0 02-06-2004 Initial release
//---------------------------------------------------------------------------
// Created by Ivan Paulik for TinyARM DIP40
// Tools used: IAR EWARM v4.10B and IAR MakeApp for LPC2106 v4.00C
// Copyright PASAT 2004
//---------------------------------------------------------------------------

#ifndef _PGM24C_INCLUDED
#define _PGM24C_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

//Function prototypes
MEMORY_ATTRIBUTE short Exec_Copy24C256_RAM(void);
MEMORY_ATTRIBUTE short Exec_CopyRAM_24C256(void);

#endif

