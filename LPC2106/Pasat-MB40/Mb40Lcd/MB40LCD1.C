// -------------------------------------------------------------------
// -----------------------LPC2106 main routines-----------------------
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
#include <intrinsics.h>

#include "ma_scb.h"     /* SCB Module driver header file */
#include "ma_uart0.h"    /* UART0 Module driver header file */
#include "ma_gpio.h"     /* GPIO  Module driver header file */
#include "ma_spi.h"      /* SPI   Module driver header file */
#include "ma_i2c.h"      /* I2C   Module driver header file */
#include "ma_pcb.h"     /* PCB Module driver header file */
#include "ma_vic.h"
#include "ma_tmr.h"

#include "string.h"
#include "stdio.h"
#include "mainmb40.h"
#include "Pgm24c.h"
#include "Pgm25c.h"
#include "Clock.h"
#include "Led.h"
#include "interupt.h"
#include "key_api.h"
#include "lcd.h"
#include "lcd_api.h"
#include "key_api.h"
#include "my_files.h"
#include "editor.h"
#include "PLLTurbo.h"

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

// RamBuffer[0] reserved for I2C stuff
unsigned char RamBuffer[32768+1];
unsigned short CurrentPageSize=64;

char Remote_Cmd_Text[][16]=
{  //01234567890123
	"Edit URAM    ",
	"Load 24C256  ",
	"Burn 24C256  ",
	"Load 25C256  ",
	"Burn 25C256  ",
	"Show/Set Time",
	"Blink LEDs   ",
	"PS/2 to UART ",
	"PS/2 to LCD  ",
	//" LCD Test        ",
	"Speed test   ",
	"Remote to LCD"
};

//          "12345678901234567"
char *Text1="TinyARM  LPC2106";
char *Text2="-Select command-";

char Local_Cmd_Text[][16]=
{
//  "12345678901234567"
	"Edit User RAM",
	"Load 24C256  ",
	"Burn 24C256  ",
	"Load 25C256  ",
	"Burn 25C256  ",
	"Show/Set Time",
	"Blink LEDs   ",
	"LCD editor   ",
	"Speed test   "
};

// External references
extern U8 LocalFlag;

// local functions
int	MB_Local_Control(void);
int	MB_Remote_Control(void);

void MB_Call_Corresponding(void)
{
	U8 Value8;
	
	for (;;)
	{
		Lcd_Erase_CharScreen();

		printf("TinyARM  LPC2106 ");
		printf("--Demonstration--");
		putchar(0xD);
		putchar(0xA);
		printf("--To start demo--");
		printf("Press MB40 key   ");
		printf("or               ");
		printf("Press PC key     ");

		MA_PutString_UART0("TinyARM  LPC2106");
		MA_PutChar_UART0(0xD);
		MA_PutChar_UART0(0xA);
		MA_PutString_UART0("--Demonstration--");
		MA_PutChar_UART0(0xD);
		MA_PutChar_UART0(0xA);
		MA_PutChar_UART0(0xD);
		MA_PutChar_UART0(0xA);
		MA_PutString_UART0("--To start demo--");
		MA_PutChar_UART0(0xD);
		MA_PutChar_UART0(0xA);
		MA_PutString_UART0("Press key either on MB40 or PC keyboard ");
		MA_PutChar_UART0(0xD);
		MA_PutChar_UART0(0xA);

		if (Check_Input_Source(&Value8))
		{	
			// Local PS/2 keyboard
			MB_Local_Control();
		}
		else
		{
			// Remote terminal
			MB_Remote_Control();
		}

			//LocalFlag=1;
			//MB_Local_Control();
			//LocalFlag=0;
			//MB_Remote_Control();
	}
}

int	MB_Local_Control(void)
{
	U8 Val8;
	short i;

	for (;;)
	{
		Lcd_Erase_CharScreen();
		printf("-Select command-");
		putchar(0xD);
		putchar(0xA);
		
		for (i=0; i<9; i++)
		{
			putchar(i+0x31);
			putchar('.');
			printf(&Local_Cmd_Text[i][0]);
			putchar(0xD);
			putchar(0xA);
		}

		do
		{
			while (Test_Char_Keyboard()==0)
			{};
			Val8 = Get_Char_Keyboard();
			if (Val8==KEY_ESC) return 0;
		}
		while ((Val8<'1') | (Val8>'9'));

		Lcd_Erase_CharScreen();
		Lcd_Scroll_On();

		switch (Val8)
		{
			case '1': Exec_EditRAM(); break;
			case '2': Exec_Copy24C256_RAM(); break;
			case '3': Exec_CopyRAM_24C256(); break;
			case '4': Exec_Copy25C256_RAM(); break;
			case '5': Exec_CopyRAM_25C256(); break;
			case '6': Exec_ShowTime(); break;
			//case '7': Exec_SetTime(); break;
			case '7': Exec_BlinkLed(); break;
			case '8': Exec_Lcd_Editor(); break;
			case '9': Exec_Speed_Test();break;
		}
		Lcd_Scroll_Off();
	}
}

int	MB_Remote_Control(void)
{
	U8 Val8;
	short i;

	for (;;)
	{
		My_PutChar(0xD);
		My_PutChar(0xA);
		My_PutChar(0xD);
		My_PutChar(0xA);
		My_PutString("-Select command-\n");
		My_PutChar(0xD);
		My_PutChar(0xA);
		
		for (i=0; i<10; i++)
		{
			My_PutChar(i+0x30);
			My_PutChar('.');
			My_PutString(&Remote_Cmd_Text[i][0]);
			My_PutChar(0xD);
			My_PutChar(0xA);
		}

		do
		{
		    if (My_WaitForChar(&Val8))
				{Val8=0;continue;}  //simple recovery from RS232 error
			if (Val8==KEY_ESC) return 0;
	    }
		while ((Val8<'0') | (Val8>'9'));

		switch (Val8)
		{
			case '0': Exec_EditRAM(); break;
			case '1': Exec_Copy24C256_RAM(); break;
			case '2': Exec_CopyRAM_24C256(); break;
			case '3': Exec_Copy25C256_RAM(); break;
			case '4': Exec_CopyRAM_25C256(); break;
			case '5': Exec_ShowTime(); break;
			//case '6': Exec_SetTime(); break;
			case '6': Exec_BlinkLed(); break;
			case '7': Exec_Keyboard_PC(); break;
			case '8': Exec_Keyboard_LCD();break;
			//case '9': Exec_PcKey_LCD();break;
			case '9': Exec_Speed_Test();break;
		}
	}
}

int main(void)
{
	MA_Reset_SCB();
//	MA_Init_SCB();
	MA_Reset_PCB();
	MA_Init_GPIO();
	MA_Init_UART0();
	MA_Reset_UART0();
	MA_Init_SPI();
	MA_Reset_SPI();
	MA_Init_I2C();
	MA_Reset_I2C();
	Init_My_Clock();

	Init_Keyboard();
	__disable_interrupt();
	MA_Init_VIC();

	MA_Init_TIMER();
	MA_Reset_TIMER();

	Install_IntHandlers();

	MA_Start_TIMER(1,1);

	Lcd_Init_API();

	__enable_interrupt();

	MB_Call_Corresponding();

}


