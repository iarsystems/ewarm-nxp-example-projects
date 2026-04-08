// -------------------------------------------------------------------
// -----------------------LPC2106 LED routines------------------------
// -------------------------------------------------------------------
// Written for TinyARM MB40 development board by PASAT
// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */
#include "NXP/iolpc210x.h"  /* Defines Special function registers */

#include "ma_uart0.h"    /* UART1 Module driver header file */
#include "ma_gpio.h"     /* GPIO  Module driver header file */
#include "mainmb40.h"

#include "string.h"
#include "stdio.h"
#include "Led.h"
#include "my_files.h"

#define TIME_DELAY_VALUE 10000

void Send_To_4Leds(unsigned char Var8)
{
  // Selected bits to logic low
  IOCLR = 0xF0000000;
  // Selected bits to logic high
  IOSET = ((unsigned long)Var8) << 28;
}

void Exec_BlinkLed(void)
{
	U8 pChar,i;
	S8 Result8;
	unsigned long Var32;

	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" LED blinking");
	My_PutChar(0xD);
	My_PutChar(0xA);

	Var32=TIME_DELAY_VALUE;
	i=0;
	Send_To_4Leds(i);
	i=1;

	do
	{
	    Var32--;
		if (!Var32)
		{
			Send_To_4Leds(i);
			Var32=TIME_DELAY_VALUE;
			i<<=1;
			if (i>8) i=1;
		}

	    Result8=My_WaitForChar(&pChar);
		if (Result8) pChar=0;
	}
	while (pChar != KEY_ESC);
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);

}


