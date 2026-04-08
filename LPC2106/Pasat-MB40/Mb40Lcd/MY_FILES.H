#ifndef _MY_FILES_INCLUDED
#define _MY_FILES_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

//Function prototypes
MEMORY_ATTRIBUTE U8 Check_Input_Source(U8 *FirstChar);
MEMORY_ATTRIBUTE int My_PutChar(int Value);
MEMORY_ATTRIBUTE void My_Put_Some(U8 Data8, U8 Count8);
MEMORY_ATTRIBUTE int My_PutString(char *StringPtr);
MEMORY_ATTRIBUTE int My_WaitForChar( U8 *pChar);


#endif


