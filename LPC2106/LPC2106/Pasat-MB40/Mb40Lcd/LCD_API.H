#ifndef _LCD_API_INCLUDED
#define _LCD_API_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

//Function prototypes
MEMORY_ATTRIBUTE void Lcd_Init_API(void);
MEMORY_ATTRIBUTE int  Lcd_Putchar(int Value);
MEMORY_ATTRIBUTE S8 Lcd_Set_CharYX(short CharPosY,short CharPosX);
//MEMORY_ATTRIBUTE void Lcd_Redraw_CharScreen(unsigned char *Ptr,short Count);
MEMORY_ATTRIBUTE int Lcd_Put_Ctrl(int Value);
MEMORY_ATTRIBUTE int Lcd_Erase_CharScreen(void);
MEMORY_ATTRIBUTE int Lcd_Scroll_Off(void);
MEMORY_ATTRIBUTE int Lcd_Scroll_On(void);
MEMORY_ATTRIBUTE int Lcd_Key_Left(void);

#endif


