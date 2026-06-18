// -------------------------------------------------------------------
// -----------------------LPC2106 I2C 24C256 EEPROM routines----------
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

//#include "ma_uart0.h"    /* UART0 Module driver header file */
//#include "ma_uart0.h"    /* UART1 Module driver header file */
#include "ma_gpio.h"     /* GPIO  Module driver header file */
#include "ma_spi.h"      /* SPI   Module driver header file */
#include "ma_i2c.h"      /* I2C   Module driver header file */
#include "ma_pcb.h"     /* PCB Module driver header file */

#include "string.h"
#include "stdio.h"
#include "mainmb40.h"
#include "Pgm24c.h"
#include "my_files.h"

extern unsigned char RamBuffer[];
extern unsigned short CurrentPageSize;
extern char Info;
extern char StatusI2C;

//Extern functions
//extern void My_Put_Some(U8 Data8, U8 Count8);
extern S8 MA_WritePoll_I2C(void);
extern S8 I2C_StartRead_24C256( U8 *pTxData );
extern S8 I2C_PageRead_24C256( U8 *pRxData, U16 Length );

//----------------------------------------------
// Copy 24C256 EEPROM to RAM buffer (Load EEPROM)
//----------------------------------------------
short Exec_Copy24C256_RAM_Old(void)
{
	char Result;

	RamBuffer[0] = I2C_SLAVEADR_RCV;
	// Note: Is it necessary to set initial address ?
	Result = MA_MasterRead_I2C(RamBuffer,Size_24C256);

	if (Result==MA_OK) return 0;
	else  return 1;
}

short Exec_Copy24C256_RAM(void)
{
	char Result;
    unsigned short i,PageStart;
	unsigned char *Ptr=&RamBuffer[1];
	unsigned char LocalBuffer[4+64];
	char AdrBuffer[10];
	char *AdrPtr;

	My_PutChar(0xD);
	My_PutChar(0xA);

	My_PutString(" Loading 24C256");
	My_PutChar(0xD);
	My_PutChar(0xA);

	AdrPtr=&AdrBuffer[0];

	for (i=0; i<Size_24C256/CurrentPageSize; i++)
	{
		PageStart=i*CurrentPageSize;
		if ((PageStart & 0xFF)==0)
		{
			My_PutString(" Adr=");
			sprintf(AdrPtr,"%04hX",PageStart);
			My_PutString(AdrPtr);
	    	My_Put_Some(KEY_LEFT,9);
		}
		LocalBuffer[0] = I2C_SLAVEADR_SEND;
		LocalBuffer[1] = (unsigned char)(PageStart>>8);
		LocalBuffer[2] = (unsigned char)(PageStart&0xFF);
		LocalBuffer[3] = I2C_SLAVEADR_RCV;

		Result = I2C_PageRead_24C256(LocalBuffer,CurrentPageSize+4);
		if (Result!=MA_OK) break;
		memcpy(Ptr,&LocalBuffer[4],CurrentPageSize);
		Ptr+=CurrentPageSize;
    }
	My_PutChar(0xD);
	My_PutChar(0xA);

	if (Result==MA_OK) return 0;

	// Error
	My_PutString(" Read Error");
/*
	if (!Info)   My_PutChar('0');
	if (Info==1) My_PutChar('1');
	if (Info==2) My_PutChar('2');
	if (Info==3) My_PutChar('3');
	if (Info==4) My_PutChar('4');
	if (Info==5) My_PutChar('5');
	if (Info==6) My_PutChar('6');
	if (Info==7) My_PutChar('7');
	My_PutString(" Status=");
	sprintf(AdrPtr,"%04hX",StatusI2C);
	My_PutString(AdrPtr);
*/
	My_PutChar(0xD);
	My_PutChar(0xA);

	return 1;
}

//----------------------------------------------
// Copy RAM buffer to 24C256 EEPROM (Burn EEPROM)
//----------------------------------------------
// Use a buffer: Buffer[0]=SLA+W,Buffer[1]=MSB Adr,
// Buffer[2]=LSB Adr, Buffer[3..66]=EEPROM page data
//----------------------------------------------
short Exec_CopyRAM_24C256(void)
{
    unsigned short i,PageStart;
	char Result,Error;
	unsigned char *Ptr=&RamBuffer[1];
	unsigned char LocalBuffer[3+64];
	char AdrBuffer[10];
	char *AdrPtr;

	My_PutChar(0xD);
	My_PutChar(0xA);

	My_PutString(" Burning 24C256");
	My_PutChar(0xD);
	My_PutChar(0xA);

	AdrPtr=&AdrBuffer[0];
	Error=0;

	for (i=0; i<Size_24C256/CurrentPageSize; i++)
	{
		PageStart=i*CurrentPageSize;
		if ((PageStart & 0xFF)==0)
		{
			My_PutString(" Adr=");
			sprintf(AdrPtr,"%04hX",PageStart);
			My_PutString(AdrPtr);
	    	My_Put_Some(KEY_LEFT,9);
		}
		LocalBuffer[0] = I2C_SLAVEADR_SEND;
		LocalBuffer[1] = (unsigned char)((i*CurrentPageSize)>>8);
		LocalBuffer[2] = (unsigned char)((i*CurrentPageSize)&0xFF);
		memcpy(&LocalBuffer[3],Ptr,CurrentPageSize);
		Result = MA_MasterWrite_I2C(LocalBuffer,CurrentPageSize+3);
	    if (Result!=MA_OK) {Error=1;break;}
		Result = MA_WritePoll_I2C();
	    if (Result!=MA_OK) {Error=2;break;}
		Ptr+=CurrentPageSize;
	}

	My_PutChar(0xD);
	My_PutChar(0xA);
	if (!Error) return Result;

	if (Error==1)
	{
		My_PutString(" Burn Error");
		if (!Info) My_PutChar('0');
		if (Info==1) My_PutChar('1');
		if (Info==2) My_PutChar('2');
		if (Info==3) My_PutChar('3');
		if (Info==4) My_PutChar('4');

		My_PutString(" Status=");
		sprintf(AdrPtr,"%04hX",StatusI2C);
		My_PutString(AdrPtr);

		My_PutString(" Adr=");
		sprintf(AdrPtr,"%04hX",PageStart);
		My_PutString(AdrPtr);
		
	}
	else          My_PutString(" Poll Error"); 
	My_PutChar(0xD);
	My_PutChar(0xA);
	return Result;
}
