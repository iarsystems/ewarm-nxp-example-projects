//---------------------------------------------------------------------------
// Filename: Editor.c
//---------------------------------------------------------------------------
// Editor routines for watching/modifying buffer in RAM
// V0.0 02-06-2004 Initial release
//---------------------------------------------------------------------------
// Created by Ivan Paulik for TinyARM DIP40
// Tools used: IAR EWARM v4.10B and IAR MakeApp for LPC2106 v4.00C
// Copyright PASAT 2004
//---------------------------------------------------------------------------

//#include "ma_uart0.h"    /* UART0 Module driver header file */
#include "ma_uart1.h"    /* UART1 Module driver header file */
#include "ma_gpio.h"     /* GPIO  Module driver header file */
#include "ma_spi.h"      /* SPI   Module driver header file */
#include "ma_i2c.h"      /* I2C   Module driver header file */

#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "easypgm.h"

extern unsigned char RamBuffer[];

unsigned short CurrAdr=0;

void Exec_EditRAM(void);
void Send_Some_Uart(U8 Data8, U8 Count8);
extern unsigned char MA_WaitforChar_UART1( U8 *pChar );

void Exec_EditRAM(void)
{
	char *EndPtr;
	char AdrBuffer[10],DataBuffer[10];
	char *AdrPtr,*DataPtr;
	U8 Position,Result,Data8,CurrData;

	DataPtr=&DataBuffer[0];
	AdrPtr=&AdrBuffer[0];
	sprintf(AdrPtr,"%04hX",CurrAdr);

	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);
	MA_PutString_UART1(">Editor");
	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);
	MA_PutString_UART1(" Adr=");
	MA_PutString_UART1(AdrPtr);
    Send_Some_Uart(KEY_LEFT,4);
	Position=1;

	do
	{	//entering start address - 4 hex digits,Space,Backspace
		Result=MA_WaitforChar_UART1(&Data8);
		if (Result) Data8=0;

		if (Data8==KEY_LEFT)
		{
			if (Position>1)
			{
				Position--;
				Send_Some_Uart(KEY_LEFT,1);
			}
			else
			{
				sprintf(AdrPtr,"%04hX",CurrAdr);
				MA_PutString_UART1(AdrPtr);
				Send_Some_Uart(KEY_LEFT,1);
				Position=4;
			}
		}

		if (Data8==0x20)
		{
			Data8=AdrBuffer[Position-1];
			MA_PutChar_UART1(Data8);
			Data8=0x20;

			if (Position<4)
				Position++;
			else
			{
				Send_Some_Uart(KEY_LEFT,4);
				Position=1;
			}
		}

		if (isxdigit(Data8))
		{
			// Limit address to 0x7FFF !
			if (isdigit(Data8)==0) toupper(Data8);
			if ((Position!=1) | (Data8<'8'))
			{
				MA_PutChar_UART1(Data8);
				AdrBuffer[Position-1]=Data8;
				CurrAdr=(unsigned short)strtoul(AdrPtr,&EndPtr,16);
				if (Position<4) Position++;
				else
				{
					Send_Some_Uart(KEY_LEFT,4);
					Position=1;
				}
			}
		}
	}
	while ((Data8 != KEY_ENTER) & (Data8 != KEY_ESC));

	if (Data8 == KEY_ESC) return;

	do
	{
		MA_PutChar_UART1(0xD);
		MA_PutChar_UART1(0xA);
		MA_PutString_UART1(" Adr=");
		sprintf(AdrPtr,"%04hX",CurrAdr);
		MA_PutString_UART1(AdrPtr);

		// RamBuffer[0] reserved for I2C stuff
		CurrData = RamBuffer[CurrAdr+1];
		MA_PutString_UART1("   Data=");
		sprintf(DataPtr,"%02hX",CurrData);
		MA_PutString_UART1(DataPtr);
	    Send_Some_Uart(KEY_LEFT,2);
		
		Position=1;

		do
		{	//entering data byte - 2 hex digits,Space,Backspace
			Result=MA_WaitforChar_UART1(&Data8);
			if (Result) Data8=0;

			if (Data8==KEY_LEFT)
			{
				if (Position>1)
				{
					Position--;
					Send_Some_Uart(KEY_LEFT,1);
				}
				else
				{
					sprintf(DataPtr,"%02hX",CurrData);
					MA_PutString_UART1(DataPtr);
					Send_Some_Uart(KEY_LEFT,1);
					Position=2;
				}
			}

			if (Data8==0x20)
			{
				Data8=DataBuffer[Position-1];
				MA_PutChar_UART1(Data8);
				Data8=0x20;

				if (Position<2) Position++;
				else
				{
					Send_Some_Uart(KEY_LEFT,2);
					Position=1;
				}
			}

			if (isxdigit(Data8))
			{
				MA_PutChar_UART1(Data8);
				DataBuffer[Position-1]=Data8;
				CurrData=(unsigned short)strtoul(DataPtr,&EndPtr,16);
				if (Position<2) Position++;
				else
				{
					Send_Some_Uart(KEY_LEFT,2);
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
	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);

}


void Send_Some_Uart(U8 Data8, U8 Count8)
{
	U8 i;
	for (i=0; i<Count8; i++)
		MA_PutChar_UART1(Data8);
}
