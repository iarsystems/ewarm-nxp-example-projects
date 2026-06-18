#ifndef _KEY_API_INCLUDED
#define _KEY_API_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

//Function prototypes
MEMORY_ATTRIBUTE void Init_Keyboard(void);
MEMORY_ATTRIBUTE int Get_Char_Keyboard(void);
MEMORY_ATTRIBUTE void Key_Decode(unsigned char sc);
MEMORY_ATTRIBUTE int Test_Char_Keyboard(void);

MEMORY_ATTRIBUTE void Exec_Keyboard_PC(void); 
MEMORY_ATTRIBUTE void Exec_Keyboard_LCD(void);

#endif
