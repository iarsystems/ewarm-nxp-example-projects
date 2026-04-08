#ifndef _CLOCK_INCLUDED
#define _CLOCK_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

//Function prototypes
MEMORY_ATTRIBUTE void Exec_ShowTime(void);
MEMORY_ATTRIBUTE void Exec_SetTime(void);
MEMORY_ATTRIBUTE void Init_My_Clock(void);
MEMORY_ATTRIBUTE U8 Arrow_Up_Down(U8 Value,U8 ValueMin,U8 ValueMax,U8 *MyKey);

#endif


