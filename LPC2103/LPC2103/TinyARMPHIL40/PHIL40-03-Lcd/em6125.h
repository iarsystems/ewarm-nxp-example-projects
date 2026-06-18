

#ifndef _EM6125_INCLUDED
#define _EM6125_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

//Function prototypes
//MEMORY_ATTRIBUTE void Exec_LCD_Test(void);
MEMORY_ATTRIBUTE S8 EM_Fill_WithByte(U8 SlaveAdr, U8 CtrlByte, U8 ByteVal, U16 Length );
MEMORY_ATTRIBUTE short EM_Initialise_LCD(void);
MEMORY_ATTRIBUTE unsigned char * EM_Get_CharacterAdr(U8 Val8);
MEMORY_ATTRIBUTE S8 EM_Display_Char(unsigned char Val8);
MEMORY_ATTRIBUTE S8  EM_Display_InvChar(unsigned char Val8);
MEMORY_ATTRIBUTE S8  EM_Send_Cmd(unsigned char Adr8,unsigned char Val8);
MEMORY_ATTRIBUTE S8  EM_Set_YPosition(unsigned char Y8);
MEMORY_ATTRIBUTE S8  EM_Set_XPosition(unsigned char X8);

#endif


