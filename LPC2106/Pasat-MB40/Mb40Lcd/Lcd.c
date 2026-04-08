// -------------------------------------------------------------------
// -----------------------LPC2106 LCD routines------------------------
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
#include "Lcd.h"
#include "em6125.h"

#include "mainmb40.h"

void Exec_LCD_Test(void)
{
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);
	return;
}

//S8 EM_Display_Char(unsigned char Val8)
//{
//	return 0;
//}

//----------------------------------------------------------------
// Exec_PcKey_LCD
//----------------------------------------------------------------
// If PCkey is pressed then character is displayed on LCD
// Exited when ESC is pressed on remote terminal (PC)
//----------------------------------------------------------------
void Exec_PcKey_LCD(void)
{
	U8	pChar;
	S8 Result8;

	EM_Initialise_LCD();

	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);
	MA_PutString_UART0(" Press key(s) on PC keyboard ");
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);
	MA_PutString_UART0(" Watch your graphic LCD ");
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);
	MA_PutString_UART0(" ------- Press ESC on PC's keyboard to exit -------");
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);

	for (;;)
	{
		pChar=0;
		while (pChar==0)
		{
		    Result8=MA_GetChar_UART0(&pChar);
			if (Result8!=MA_OK) pChar=0;
			if (pChar == KEY_ESC)
			{
				MA_PutChar_UART0(0xD);
				MA_PutChar_UART0(0xA);
			 	return;
			}

		}

		//Val8 = Get_Char_Keyboard();
		//MA_PutChar_UART0(pChar);
		EM_Display_Char(pChar);
	}

}
