#ifndef _LCD_INCLUDED
#define _LCD_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

//Function prototypes
MEMORY_ATTRIBUTE void Exec_LCD_Test(void);
//MEMORY_ATTRIBUTE S8 LCD_Display_Char(unsigned char Val8);
MEMORY_ATTRIBUTE void Exec_PcKey_LCD(void);

#endif
