// -------------------------------------------------------------------
// -----------------------LPC2103 LED routines------------------------
// -------------------------------------------------------------------
// Written for TinyARM MB40 development board
// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */
#include "NXP/iolpc210x.h"  /* Defines Special function registers */

#include "S1_Led.h"

#define TIME_DELAY_VALUE 100000

void Send_To_4Leds(unsigned char Var8)
{
  // Selected bits to logic low
  IOCLR = 0xF0000000;
  // Selected bits to logic high
  IOSET = ((unsigned long)Var8) << 28;
}

void Exec_BlinkLed(void)
{
	U8 i;
	unsigned long Var32;

	Var32=TIME_DELAY_VALUE;
	i=0;
	Send_To_4Leds(i);
	i=1;

	for (;;)
	{
	    Var32--;
		if (!Var32)
		{
			Send_To_4Leds(i);
			Var32=TIME_DELAY_VALUE;
			i<<=1;
			if (i>8) i=1;
		}

	}
}


