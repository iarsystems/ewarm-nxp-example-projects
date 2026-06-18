// -------------------------------------------------------------------
// -----------------------LPC2106 LCD API ----------------------------
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
#include "em6125.h"
#include "lcd_api.h"

#include "string.h"
#include "stdio.h"
#include "ctype.h"

#define CHAR_Y_MAX  9
#define CHAR_X_MAX 16

unsigned char LcdBuffer[10][17];
unsigned short CharX,CharY,CharScroll;

void LCD_Redraw_CharScreen(unsigned char *Ptr,short Count);

void Lcd_Init_API(void)
{
	EM_Initialise_LCD();
	
	memset(&LcdBuffer[0][0],' ',17*10);
	LCD_Redraw_CharScreen(&LcdBuffer[0][0],17*10);

	CharY=0;		// Row=0
	CharX=0;		// Column=0
	CharScroll=0; 	// Scroll off
	// Set X,Y position for the cursor
	Lcd_Set_CharYX(CharY,CharX);
	// Show cursor
	EM_Display_InvChar(LcdBuffer[CharY][CharX]);
	// Restore X,Y position
	Lcd_Set_CharYX(CharY,CharX);
}

int Lcd_Putchar(int Value)
{
	Value &= 0x7F;
	// Check if ASCII displayable characters
	if (isprint(Value))
	{  	// ASCII character, display it
		LcdBuffer[CharY][CharX]=Value;
		EM_Display_Char(Value);
		// Check if cursor is at the bottom right corner
		if ((CharY==CHAR_Y_MAX) & (CharX==CHAR_X_MAX))
		{   // Check if SCROLL is ON or OFF
			if (!CharScroll)
			{	// SCROLL is off, goto to top left (Y=0,X=0) position
				// Cursor position - beginning of row 0
				CharY=0;
				CharX=0;
			}
			else
			{	// SCROLL is on, scroll-up display
			    // First, copy buffer {row1-row9} to {row0-row8}
				// note: memmove creates buffer
				memmove(&LcdBuffer[0][0],&LcdBuffer[1][0],17*9);
				// Second,clear buffer row 9
				memset(&LcdBuffer[9][0],' ',17);
				// Send buffer to LCD screen
				LCD_Redraw_CharScreen(&LcdBuffer[0][0],17*10);
				// Cursor position - beginning of row 9
				CharY=9;
				CharX=0;
			}
		}
		else
		{	// Cursor is somewhere on the screen
			if (CharX==CHAR_X_MAX)
			{	// last column
				CharY++;
				CharX=0;
			}
			else
				CharX++;	//increment column
		}
		// Set X,Y position for the cursor
		Lcd_Set_CharYX(CharY,CharX);
		// Show cursor
		EM_Display_InvChar(LcdBuffer[CharY][CharX]);
		// Restore X,Y position
		Lcd_Set_CharYX(CharY,CharX);
		return Value; //????
	}	//end of printable ASCII character

	if (Value==0xD)
	{	// Carriage Return - go to column 0
		// Remove cursor from current position
		EM_Display_Char(LcdBuffer[CharY][CharX]);
		CharX=0;
		// Set X,Y position for the cursor
		Lcd_Set_CharYX(CharY,CharX);
		// Show cursor
		EM_Display_InvChar(LcdBuffer[CharY][CharX]);
		// Restore X,Y position
		Lcd_Set_CharYX(CharY,CharX);
		return Value; //????
	}	// End of CR handling


	if (Value==0xA)
	{	// Line Feed - go to next line
		// Remove cursor from current position
		EM_Display_Char(LcdBuffer[CharY][CharX]);

		if (CharY==CHAR_Y_MAX)
		{   // Last row
			// Check if SCROLL is ON or OFF
			if (!CharScroll)
			{	// SCROLL is off, goto to row 0 (Y=0) position
				// Cursor position - row 0
				CharY=0;
			}
			else
			{	// SCROLL is on, scroll-up display
			    // First, copy buffer {row1-row9} to {row0-row8}
				// note: memmove creates buffer
				memmove(&LcdBuffer[0][0],&LcdBuffer[1][0],17*9);
				// Second,clear buffer row 9
				memset(&LcdBuffer[9][0],' ',17);
				// Send buffer to LCD screen
				LCD_Redraw_CharScreen(&LcdBuffer[0][0],17*10);
				// Cursor position - beginning of row 9
				CharY=9;
			}
		}
		else
		{	// Row < RowMax
			CharY++;
		}

		// Set X,Y position for the cursor
		Lcd_Set_CharYX(CharY,CharX);
		// Show cursor
		EM_Display_InvChar(LcdBuffer[CharY][CharX]);
		// Restore X,Y position
		Lcd_Set_CharYX(CharY,CharX);
		return Value; //????
	}	//end of LF handling

	return -1;
}

S8 Lcd_Set_CharYX(short CharPosY,short CharPosX)
{
	S8 Result;

	if (CharPosY>9)  return 1;
	if (CharPosX>16) return 1;
	Result=EM_Set_YPosition(CharPosY+1);
	if (Result != MA_OK) return 1;
	Result=EM_Set_XPosition(CharPosX*6);
	if (Result != MA_OK) return 1;
	return 0;
}

void LCD_Redraw_CharScreen(unsigned char *Ptr,short Count)
{
	short i,j;

	Lcd_Set_CharYX(0,0);
	for (i=0;i<CHAR_Y_MAX+1;i++)
	{
		for (j=0;j<CHAR_X_MAX+1;j++)
			EM_Display_Char(LcdBuffer[i][j]);
	}

	return;
}

int Lcd_Put_Ctrl(int Value)
{
	// Remove cursor from current position
	EM_Display_Char(LcdBuffer[CharY][CharX]);

	switch (Value)
	{
		//----------------- Left arrow -----------------
	    case 0x4B:		// left arrow
		{
			// Check if cursor is at the top left corner
			if ((CharY==0) & (CharX==0))
			{   // Check if SCROLL is ON or OFF
				if (!CharScroll)
				{	// SCROLL is off, goto to bottom right (Y=9,X=17) position
					// Cursor position - beginning of row 0
					CharY=CHAR_Y_MAX;
					CharX=CHAR_X_MAX;
				}
				else
				{	// SCROLL is on, scroll-up display
			    	// First, copy buffer {row0-row8} to {row1-row9}
					// note: memmove creates buffer
					memmove(&LcdBuffer[1][0],&LcdBuffer[0][0],17*9);
					// Second,clear buffer row 0
					memset(&LcdBuffer[0][0],' ',17);
					// Send buffer to LCD screen
					LCD_Redraw_CharScreen(&LcdBuffer[0][0],17*10);
					// Cursor position - beginning of row 0
					CharY=0;
					CharX=0;
				}
			}
			else
			{	// Cursor is somewhere on the screen
				if (CharX==0)
				{	// last column
					CharY--;
					CharX=CHAR_X_MAX;
				}
				else
					CharX--;	//increment column
			}
			// Set X,Y position for the cursor
			Lcd_Set_CharYX(CharY,CharX);
			// Show cursor
			EM_Display_InvChar(LcdBuffer[CharY][CharX]);
			// Restore X,Y position
			Lcd_Set_CharYX(CharY,CharX);
			break;
		}

		//----------------- Right arrow -----------------
	    case 0x4D:		// right arrow
		{
			// Check if cursor is at the bottom right corner
			if ((CharY==CHAR_Y_MAX) & (CharX==CHAR_X_MAX))
			{   // Check if SCROLL is ON or OFF
				if (!CharScroll)
				{	// SCROLL is off, goto to top left (Y=0,X=0) position
					// Cursor position - beginning of row 0
					CharY=0;
					CharX=0;
				}
				else
				{	// SCROLL is on, scroll-up display
			    	// First, copy buffer {row1-row9} to {row0-row8}
					// note: memmove creates buffer
					memmove(&LcdBuffer[0][0],&LcdBuffer[1][0],17*9);
					// Second,clear buffer row 9
					memset(&LcdBuffer[9][0],' ',17);
					// Send buffer to LCD screen
					LCD_Redraw_CharScreen(&LcdBuffer[0][0],17*10);
					// Cursor position - beginning of row 9
					CharY=9;
					CharX=0;
				}
			}
			else
			{	// Cursor is somewhere on the screen
				if (CharX==CHAR_X_MAX)
				{	// last column
					CharY++;
					CharX=0;
				}
				else
					CharX++;	//increment column
			}
			// Set X,Y position for the cursor
			Lcd_Set_CharYX(CharY,CharX);
			// Show cursor
			EM_Display_InvChar(LcdBuffer[CharY][CharX]);
			// Restore X,Y position
			Lcd_Set_CharYX(CharY,CharX);
			break;
		}

		//----------------- Up arrow -----------------
	    case 0x48:		// up arrow
		{
			// Check if cursor is at the top
			if (CharY==0)
			{   // Check if SCROLL is ON or OFF
				if (!CharScroll)
				{	// SCROLL is off, goto to bottom row (Y=max) position
					// Cursor position - row 0, column not changed
					CharY=CHAR_Y_MAX;
				}
				else
				{	// SCROLL is on, scroll-up display
			    	// First, copy buffer {row-row8} to {row1-row9}
					// note: memmove creates buffer
					memmove(&LcdBuffer[1][0],&LcdBuffer[0][0],17*9);
					// Second,clear buffer row 0
					memset(&LcdBuffer[0][0],' ',17);
					// Send buffer to LCD screen
					LCD_Redraw_CharScreen(&LcdBuffer[0][0],17*10);
					// Cursor position - row 0
					CharY=0;
				}
			}
			else
			{	// Cursor is somewhere on the screen
				CharY--;
			}
			// Set X,Y position for the cursor
			Lcd_Set_CharYX(CharY,CharX);
			// Show cursor
			EM_Display_InvChar(LcdBuffer[CharY][CharX]);
			// Restore X,Y position
			Lcd_Set_CharYX(CharY,CharX);
			break;
		}

		//----------------- Down arrow -----------------
	    case 0x50:		// down arrow
		{
			// Check if cursor is at the bottom
			if (CharY==CHAR_Y_MAX)
			{   // Check if SCROLL is ON or OFF
				if (!CharScroll)
				{	// SCROLL is off, goto to top row (Y=0) position
					// Cursor position - row 0, column not changed
					CharY=0;
				}
				else
				{	// SCROLL is on, scroll-up display
			    	// First, copy buffer {row1-row9} to {row0-row8}
					// note: memmove creates buffer
					memmove(&LcdBuffer[0][0],&LcdBuffer[1][0],17*9);
					// Second,clear buffer row 9
					memset(&LcdBuffer[9][0],' ',17);
					// Send buffer to LCD screen
					LCD_Redraw_CharScreen(&LcdBuffer[0][0],17*10);
					// Cursor position - row 9
					CharY=CHAR_Y_MAX;
				}
			}
			else
			{	// Cursor is somewhere on the screen
				CharY++;
			}
			// Set X,Y position for the cursor
			Lcd_Set_CharYX(CharY,CharX);
			// Show cursor
			EM_Display_InvChar(LcdBuffer[CharY][CharX]);
			// Restore X,Y position
			Lcd_Set_CharYX(CharY,CharX);
			break;
		}

		//----------------- Scroll -----------------
	    case 0x54:		
		{
			if (!CharScroll) 	CharScroll=1;
			else				CharScroll=0;
			// Set X,Y position for the cursor
			Lcd_Set_CharYX(CharY,CharX);
			// Show cursor
			EM_Display_InvChar(LcdBuffer[CharY][CharX]);
			// Restore X,Y position
			Lcd_Set_CharYX(CharY,CharX);
			break;
		}

		//----------------- Home -----------------
	    case 0x47:		
		{
			CharY=0;		// Row=0
			CharX=0;		// Column=0
			// Set X,Y position for the cursor
			Lcd_Set_CharYX(CharY,CharX);
			// Show cursor
			EM_Display_InvChar(LcdBuffer[CharY][CharX]);
			// Restore X,Y position
			Lcd_Set_CharYX(CharY,CharX);
			break;
		}
	}

	return 0;
}

int Lcd_Erase_CharScreen(void)
{
	memset(&LcdBuffer[0][0],' ',17*10);
	LCD_Redraw_CharScreen(&LcdBuffer[0][0],17*10);

	CharY=0;		// Row=0
	CharX=0;		// Column=0
	CharScroll=0; 	// Scroll off
	// Set X,Y position for the cursor
	Lcd_Set_CharYX(CharY,CharX);
	// Show cursor
	EM_Display_InvChar(LcdBuffer[CharY][CharX]);
	// Restore X,Y position
	Lcd_Set_CharYX(CharY,CharX);
	return 0;
}

int Lcd_Scroll_On(void)
{
 	CharScroll=0;
 	Lcd_Put_Ctrl(0x54);
	return 0;
}

int Lcd_Scroll_Off(void)
{
 	CharScroll=1;
 	Lcd_Put_Ctrl(0x54);
	return 0;
}

int Lcd_Key_Left(void)
{
 	Lcd_Put_Ctrl(0x4B);
	return 0;
}
