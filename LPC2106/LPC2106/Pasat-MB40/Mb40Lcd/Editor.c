// -------------------------------------------------------------------
// -----------------------LPC2106 memory editor-----------------------
// -------------------------------------------------------------------
// Written for TinyARM MB40 development board by PASAT
// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

#include "ma_uart0.h"    /* UART0 Module driver header file */
#include "ma_gpio.h"     /* GPIO  Module driver header file */
#include "ma_spi.h"      /* SPI   Module driver header file */
#include "ma_i2c.h"      /* I2C   Module driver header file */

#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "mainmb40.h"
#include "my_files.h"
#include "editor.h"
#include "key_api.h"
#include "lcd_api.h"

extern unsigned char RamBuffer[];

unsigned short CurrAdr=0;

void Exec_EditRAM(void)
{
	char *EndPtr;
	char AdrBuffer[10],DataBuffer[10];
	char *AdrPtr,*DataPtr;
	U8 Position,Result,Data8,CurrData;

	DataPtr=&DataBuffer[0];
	AdrPtr=&AdrBuffer[0];
	sprintf(AdrPtr,"%04hX",CurrAdr);

	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(">Editor");
	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" Address=");
	My_PutString(AdrPtr);
    My_Put_Some(KEY_LEFT,4);
	Position=1;

	do
	{	//entering start address - 4 hex digits,Space,Backspace
		Result=My_WaitForChar(&Data8);
		if (Result) Data8=0;

		if (Data8==KEY_LEFT)
		{
			if (Position>1)
			{
				Position--;
				My_Put_Some(KEY_LEFT,1);
			}
			else
			{
				sprintf(AdrPtr,"%04hX",CurrAdr);
				My_PutString(AdrPtr);
				My_Put_Some(KEY_LEFT,1);
				Position=4;
			}
		}

		if (Data8==0x20)
		{
			Data8=AdrBuffer[Position-1];
			My_PutChar(Data8);
			Data8=0x20;

			if (Position<4)
				Position++;
			else
			{
				My_Put_Some(KEY_LEFT,4);
				Position=1;
			}
		}

		if (isxdigit(Data8))
		{
			// Limit address to 0x7FFF !
			if (isdigit(Data8)==0) toupper(Data8);
			if ((Position!=1) | (Data8<'8'))
			{
				My_PutChar(Data8);
				AdrBuffer[Position-1]=Data8;
				CurrAdr=(unsigned short)strtoul(AdrPtr,&EndPtr,16);
				if (Position<4) Position++;
				else
				{
					My_Put_Some(KEY_LEFT,4);
					Position=1;
				}
			}
		}
	}
	while ((Data8 != KEY_ENTER) & (Data8 != KEY_ESC));

	if (Data8 == KEY_ESC) return;

	do
	{
		My_PutChar(0xD);
		My_PutChar(0xA);
		//My_PutString(" (xxxx) =");
		//My_PutString(" Adr=");
		My_PutString(" (");
		sprintf(AdrPtr,"%04hX",CurrAdr);
		My_PutString(AdrPtr);
		My_PutString(") ");

		// RamBuffer[0] reserved for I2C stuff
		CurrData = RamBuffer[CurrAdr+1];
		My_PutString("= ");
		sprintf(DataPtr,"%02hX",CurrData);
		My_PutString(DataPtr);
	    My_Put_Some(KEY_LEFT,2);
		
		Position=1;

		do
		{	//entering data byte - 2 hex digits,Space,Backspace
			Result=My_WaitForChar(&Data8);
			if (Result) Data8=0;

			if (Data8==KEY_LEFT)
			{
				if (Position>1)
				{
					Position--;
					My_Put_Some(KEY_LEFT,1);
				}
				else
				{
					sprintf(DataPtr,"%02hX",CurrData);
					My_PutString(DataPtr);
					My_Put_Some(KEY_LEFT,1);
					Position=2;
				}
			}

			if (Data8==0x20)
			{
				Data8=DataBuffer[Position-1];
				My_PutChar(Data8);
				Data8=0x20;

				if (Position<2) Position++;
				else
				{
					My_Put_Some(KEY_LEFT,2);
					Position=1;
				}
			}

			if (isxdigit(Data8))
			{
				My_PutChar(Data8);
				DataBuffer[Position-1]=Data8;
				CurrData=(unsigned short)strtoul(DataPtr,&EndPtr,16);
				if (Position<2) Position++;
				else
				{
					My_Put_Some(KEY_LEFT,2);
					Position=1;
				}
			}

		}
		while ((Data8 != KEY_ENTER) & (Data8 != KEY_ESC));

		if (Data8 == KEY_ENTER)
		{
			RamBuffer[CurrAdr+1]=CurrData;
			CurrAdr++;
			CurrAdr &= 0x7FFF;
		}
	}
	while (Data8 != KEY_ESC);
	My_PutChar(0xD);
	My_PutChar(0xA);

}


//----------------------------------------------------------------
// Exec_Lcd_Editor
//----------------------------------------------------------------
// Simple LCD editor
// Exited when ESC is pressed on PS/2 keyboard
//----------------------------------------------------------------
void Exec_Lcd_Editor(void)
{
	U8	Val8;
	//S8 Result8;

	Lcd_Erase_CharScreen();

	for (;;)
	{
		while (Test_Char_Keyboard()==0)
		{
			// blinking cursor here
		}

		Val8 = Get_Char_Keyboard();
		if (Val8== KEY_ESC) return;

		// Handling extended keys
		if ((Val8==0xE0) | (Val8==0x00))
		{
			Val8 = Get_Char_Keyboard();
			Lcd_Put_Ctrl(Val8);
		}
		else
			Lcd_Putchar(Val8);
		// if <ENTER> add LF after CR
		if (Val8==0xD) Lcd_Putchar(0xA);

	}

}
