// -------------------------------------------------------------------
// -----------------------LPC2106 main routines-----------------------
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
#include <intrinsics.h>

#include "ma_scb.h"     /* SCB Module driver header file */
#include "ma_uart0.h"    /* UART0 Module driver header file */
#include "ma_gpio.h"     /* GPIO  Module driver header file */
#include "ma_pcb.h"     /* PCB Module driver header file */

#include "string.h"
#include "stdio.h"

int	MB_Remote_Control(void)
{
	U8 Val8;
    S8 Result8;	

	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);
	MA_PutString_UART0("-Type on your keyboard");
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);
		
	for (;;)
	{
		do
		{
		    Result8=MA_GetChar_UART0(&Val8);
	    }
		while (Result8!=MA_OK);
		MA_PutChar_UART0(Val8);
	}
}

int main(void)
{
	MA_Reset_SCB();
	MA_Reset_PCB();
	MA_Init_GPIO();
	MA_Init_UART0();
	MA_Reset_UART0();

	MB_Remote_Control();
}


