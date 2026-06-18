

#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */
#include "NXP/iolpc210x.h"  /* Defines Special function registers */

//#include "ma_uart0.h"    /* UART0 Module driver header file */
#include "ma_uart1.h"    /* UART1 Module driver header file */
#include "ma_gpio.h"     /* GPIO  Module driver header file */
#include "ma_spi.h"      /* SPI   Module driver header file */
#include "ma_i2c.h"      /* I2C   Module driver header file */
#include "ma_pcb.h"     /* PCB Module driver header file */

#include "string.h"
#include "stdio.h"
#include "easypgm.h"
#include "Pgm24c.h"
#include "Pgm25c.h"

/* Code Read Protection
CRP     0x87654321 - Read Memory is disabled.
									 - Write to RAM is disabled.
									 - Go command is disabled.
									 - Copy RAM to Flash is disabled.
									 - JTAG is disabled.
*/
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned int crp = 0xFFFFFFFF;

//Extern functions
extern S8 MA_WritePoll_I2C(void);
extern unsigned char MA_WaitforChar_UART1( U8 *pChar );
extern void Exec_EditRAM(void);
extern void Send_Some_Uart(U8 Data8, U8 Count8);
extern void SPI_Reset_SPIF(void);
//extern short Exec_Copy24C256_RAM(void);
//extern short Exec_CopyRAM_24C256(void);

char *Text1="TinyARM  SPI&I2C Programmer";
char *Text2=">Select command (Space,Backspace):";
char *Cmd1 =" Edit RAM          ";
char *Cmd2 =" Copy 24C256 -> RAM";
char *Cmd3 =" Copy RAM -> 24C256";
char *Cmd4 =" Copy 25C256 -> RAM";
char *Cmd5 =" Copy RAM -> 25C256";

//char *TablePtr[] = {Cmd1,Cmd2,Cmd3,Cmd4,Cmd5};
char *TablePtr[5];
const short CmdCount=4;

char *CmdPtr;
short CmdIndex = 0;

// RamBuffer[0] reserved for I2C stuff
unsigned char RamBuffer[32768+1];
unsigned short CurrentPageSize=0;

int main(void)
{
	unsigned char MyData;	

	MA_Reset_PCB();
	MA_Init_GPIO();
	MA_Init_UART1();
	MA_Reset_UART1();
	MA_Init_SPI();
	MA_Reset_SPI();
	MA_Init_I2C();
	MA_Reset_I2C();

	TablePtr[0]=Cmd1;
	TablePtr[1]=Cmd2;
	TablePtr[2]=Cmd3;
	TablePtr[3]=Cmd4;
	TablePtr[4]=Cmd5;
	CmdPtr=Cmd1;

	CurrentPageSize=64;

	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);
	MA_PutData_UART1((U8*)Text1,strlen(Text1));
	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);
	for (;;)
	{
		MA_PutData_UART1((U8*)Text2,strlen(Text2));
		MA_PutChar_UART1(0xD);
		MA_PutChar_UART1(0xA);

		do
		{
		   CmdPtr = TablePtr[CmdIndex];
		   MA_PutChar_UART1(0xD);
		   MA_PutData_UART1((U8*)CmdPtr,strlen(CmdPtr));
		   do
		   {
		      if (MA_WaitforChar_UART1(&MyData))
		      	 {MyData=0;continue;}  //simple recovery from RS232 error
			  if ((MyData==KEY_ENTER) | (MyData==KEY_ESC)) break;
	       }
		   while ((MyData != ' ') & (MyData != 8) & (MyData != '+') & (MyData != '-'));

		   if ((MyData==' ') | (MyData == '+'))
		   {
		      CmdIndex++;
			  if (CmdIndex>CmdCount) CmdIndex=0;
		   }

		   if ((MyData==8) | (MyData == '-'))
		   {
			  if (CmdIndex==0) CmdIndex=CmdCount;
			  else 		       CmdIndex--;
		   }
		}
		while ((MyData!=KEY_ENTER) & (MyData!=KEY_ESC));
		
		if (MyData==KEY_ENTER)
		{
			switch (CmdIndex)
			{
				case 0: Exec_EditRAM(); break;
				case 1: Exec_Copy24C256_RAM(); break;
				case 2: Exec_CopyRAM_24C256(); break;
				case 3: Exec_Copy25C256_RAM(); break;
				case 4: Exec_CopyRAM_25C256(); break;
			}
		}
							
		if (MyData==KEY_ESC)
		{
			MA_PutChar_UART1(0xD);
			MA_PutChar_UART1(0xA);
			CmdIndex=0;
		}
	} // end of for (;;)
}

