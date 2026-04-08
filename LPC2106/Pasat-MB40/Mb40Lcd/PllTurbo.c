// -------------------------------------------------------------------
// -----------------------LPC2106 PLL routines------------------------
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

#include "ma_uart0.h"    /* UART0 Module driver header file */
#include "Led.h"
#include "mainmb40.h"
#include "my_files.h"
#include "PLLTurbo.h"  

void PLL_Flash_BlinkLedTest(void);
__ramfunc void PLL_Ram_BlinkLedTest(void);
__ramfunc void PLL_Send_To_4Leds(unsigned char Var8);

void Exec_Speed_Test(void)
{
	U8 Val8;

	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString("-Speed selection-");
	My_PutChar(0xD);
	My_PutChar(0xA);
//               "12345678901234567"
	My_PutString("Running in Flash ");
	// RS232 only
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);

	My_PutString("1.F=10MHz        ");
	// RS232 only
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);

	My_PutString("2.F=60MHz        ");
	// RS232 only
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);

	My_PutString("3.F=60MHz, Turbo ");
	My_PutChar(0xD);
	My_PutChar(0xA);

	// RS232 only
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);

	My_PutString("Running in RAM   ");
	// RS232 only
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);

	My_PutString("4.F=10MHz        ");
	// RS232 only
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);

	My_PutString("5.F=60MHz        ");
	// RS232 only
	MA_PutChar_UART0(0xD);
	MA_PutChar_UART0(0xA);

	for (;;)
	{
		do
		{
		    if (My_WaitForChar(&Val8))
			{Val8=0;continue;}  //simple recovery from RS232 error
			if (Val8==KEY_ESC) return;
    	}
		while ((Val8<'1') | (Val8>'5'));

		DisconnectPLL();
		DisableMAM();
		switch (Val8)
		{
			case '1':			// Flash, 10MHz, no accelerator	
				DisconnectPLL();
				PLL_Flash_BlinkLedTest();
				break;
			case '2':			// Flash, 60MHz, no accelerator
				ConnectPLL();
				PLL_Flash_BlinkLedTest();
				break;			
			case '3': 			// Flash, 60MHz, accelerator enabled
				ConnectPLL(); 
				EnableMAM();
				PLL_Flash_BlinkLedTest(); 
				break;
			case '4': 			// RAM  , 10MHz
				DisconnectPLL();
				PLL_Ram_BlinkLedTest(); 
				break;
			case '5': 			// RAM  , 60MHz
				ConnectPLL(); 
				PLL_Ram_BlinkLedTest(); 
				break;
		}

		DisconnectPLL();
		DisableMAM();
	}
}

//#include <LPC210x.h>

void ConnectPLL(void)
{
	PLLCON=3;       	//enable and connect 
	PLLCFG=0x26;		//P=2 (Bits 6,5=0y01), M=6 (Bits 4:0=0y00110)
	PLLFEED=0xAA;		//PLL Feed Sequence
	PLLFEED=0x55;		//PLL Feed Sequence
	while (!(PLLSTAT&0x400)); //wait for the PLL to be locked

	return;
}

void DisconnectPLL(void)
{
	PLLCON=0;       	//disable and disconnect 
	//SCB_PLLCFG=0x26;		//P=2 (Bits 6,5=0y01), M=6 (Bits 4:0=0y00110)
	PLLFEED=0xAA;		//PLL Feed Sequence
	PLLFEED=0x55;		//PLL Feed Sequence
	while (PLLSTAT & 0x300); //wait for the PLL to be disabled and disconnected

	return;
}

void EnableMAM(void)
{
    // (0:disabled, 1: partially enabled,2: fully enabled)
	MAMCR  = 0;
	// memory access time  (1/core frequency)
	MAMTIM = 3;
    // MAM control register
    // (0:disabled, 1: partially enabled,2: fully enabled)
	MAMCR  = 2;
	return;
}

void DisableMAM(void)
{
    // (0:disabled, 1: partially enabled,2: fully enabled)
	MAMCR  = 0;
	// memory access time  (1/core frequency)
	MAMTIM = 7;
	return;
}

#define TIME_DELAY_VALUE 100000

void PLL_Flash_BlinkLedTest(void)
{
	int i,j;
	unsigned long Var32;

	Var32=TIME_DELAY_VALUE;
	i=0;
	Send_To_4Leds(i);
	i=1;

	for (j=0;j<20;j++)
	{
	    while (Var32) Var32--;
		Var32=TIME_DELAY_VALUE;
		Send_To_4Leds(i);
		i<<=1;
		if (i>8) i=1;
	}
}

__ramfunc void PLL_Ram_BlinkLedTest(void)
{
	int i,j;
	unsigned long Var32;

	Var32=TIME_DELAY_VALUE;
	i=0;
	PLL_Send_To_4Leds(i);
	i=1;

	for (j=0;j<20;j++)
	{
	    while (Var32) Var32--;
		Var32=TIME_DELAY_VALUE;
		PLL_Send_To_4Leds(i);
		i<<=1;
		if (i>8) i=1;
	}
}

__ramfunc void PLL_Send_To_4Leds(unsigned char Var8)
{
  // Selected bits to logic low
  IOCLR = 0xF0000000;
  // Selected bits to logic high
  IOSET = ((unsigned long)Var8) << 28;
}
