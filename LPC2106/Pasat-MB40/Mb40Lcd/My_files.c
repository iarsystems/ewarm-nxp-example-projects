// -------------------------------------------------------------------
// -----------------------LPC2106 various routines--------------------
// -------------------------------------------------------------------
// Written for TinyARM MB40 development board by PASAT
// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

//--------------------------------------------------------------
// Filename: My_Files.c
//--------------------------------------------------------------
// Switching between local and remote I/O operations
//--------------------------------------------------------------
#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */

#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "ma_uart0.h"    /* UART0 Module driver header file */
#include "lcd_api.h"
#include "key_api.h"
#include "my_files.h"
#include "mainmb40.h"

U8 LocalFlag=0;


U8 Check_Input_Source(U8 *FirstChar)
{
	S8 LocalKey,RemoteKey;

	do
	{
		LocalKey=Test_Char_Keyboard();

	    RemoteKey=MA_GetChar_UART0(FirstChar);
		if (RemoteKey!=MA_OK) *FirstChar=0;
	}
	while ((LocalKey==0) & (*FirstChar==0));

	if (LocalKey)		// Local PS/2 keyboard
	{
		LocalFlag=1;
		*FirstChar=Get_Char_Keyboard();
	}
	else				// Remote via UART
		LocalFlag=0;
	return LocalFlag;
}

int My_PutChar(int Value)
{
	if (!LocalFlag)		// Remote
        MA_PutChar_UART0(Value);
	else
	{
		if (isprint(Value)) putchar(Value);
		if (Value==0xD) putchar(Value);
		if (Value==0xA) putchar(Value);
		if (Value==KEY_LEFT) Lcd_Key_Left();
	}
	return 0;
}


void My_Put_Some(U8 Data8, U8 Count8)
{
	U8 i;
	for (i=0; i<Count8; i++)
		My_PutChar(Data8);
}

int My_PutString(char *StringPtr)
{
	int Result;

	if (!LocalFlag)		// Remote
	{
        MA_PutString_UART0(StringPtr);
		Result=strlen(StringPtr);
	}
	else
		Result=printf(StringPtr);

	return Result;
}

//S8 MA_GetChar_UART0( U8 *pChar ) 

int My_WaitForChar( U8 *pChar)
{
        S8 Result8;	

	if (!LocalFlag)		// Remote
	{
	    Result8=MA_GetChar_UART0(pChar);
		if (Result8!=MA_OK)
		{
			*pChar=0;
			return 1;
		}
	}
	else
	{
		if (Test_Char_Keyboard()==0)
		{
			*pChar=0;
			return 1;
		}
		else
			*pChar = Get_Char_Keyboard();
	}
	return 0;

}


