// -------------------------------------------------------------------
// -----------------------LPC2106 LCD routines------------------------
// -------------------------------------------------------------------
// Written for TinyARM MB40 development board and EM Marine LCD
// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */
#include "NXP/iolpc210x.h"  /* Defines Special function registers */
#include "S3_Lcd.h"
#include "em6125.h"

//----------------------------------------------------------------
// Exec_PcKey_LCD
//----------------------------------------------------------------
// If PCkey is pressed then character is displayed on LCD
// Exited when ESC is pressed on remote terminal (PC)
//----------------------------------------------------------------
void Exec_PcKey_LCD(void)
{
	U8	i;
	unsigned long Var32;

	EM_Initialise_LCD();

	i=0x20;
	for (;;)
	{
		Var32=2000;

		EM_Display_Char(i);
		i++;
		if (i>127) i=0x20;

		while (Var32)	{Var32--;}
	}

}
