
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

extern unsigned char RamBuffer[];
extern unsigned short CurrentPageSize;
extern char Info;
extern char StatusI2C;

//Extern functions
extern void Send_Some_Uart(U8 Data8, U8 Count8);
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

	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);

	AdrPtr=&AdrBuffer[0];

	for (i=0; i<Size_24C256/CurrentPageSize; i++)
	{
		PageStart=i*CurrentPageSize;
		if ((PageStart & 0xFF)==0)
		{
			MA_PutString_UART1(" Adr=");
			sprintf(AdrPtr,"%04hX",PageStart);
			MA_PutString_UART1(AdrPtr);
	    	Send_Some_Uart(KEY_LEFT,9);
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
	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);

	if (Result==MA_OK) return 0;

	// Error
	MA_PutString_UART1(" Read Error");
/*
	if (!Info)   MA_PutChar_UART1('0');
	if (Info==1) MA_PutChar_UART1('1');
	if (Info==2) MA_PutChar_UART1('2');
	if (Info==3) MA_PutChar_UART1('3');
	if (Info==4) MA_PutChar_UART1('4');
	if (Info==5) MA_PutChar_UART1('5');
	if (Info==6) MA_PutChar_UART1('6');
	if (Info==7) MA_PutChar_UART1('7');
	MA_PutString_UART1(" Status=");
	sprintf(AdrPtr,"%04hX",StatusI2C);
	MA_PutString_UART1(AdrPtr);
*/
	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);

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

	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);

	AdrPtr=&AdrBuffer[0];
	Error=0;

	for (i=0; i<Size_24C256/CurrentPageSize; i++)
	{
		PageStart=i*CurrentPageSize;
		if ((PageStart & 0xFF)==0)
		{
			MA_PutString_UART1(" Adr=");
			sprintf(AdrPtr,"%04hX",PageStart);
			MA_PutString_UART1(AdrPtr);
	    	Send_Some_Uart(KEY_LEFT,9);
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

	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);
	if (!Error) return Result;

	if (Error==1)
	{
		MA_PutString_UART1(" Burn Error");
		if (!Info) MA_PutChar_UART1('0');
		if (Info==1) MA_PutChar_UART1('1');
		if (Info==2) MA_PutChar_UART1('2');
		if (Info==3) MA_PutChar_UART1('3');
		if (Info==4) MA_PutChar_UART1('4');

		MA_PutString_UART1(" Status=");
		sprintf(AdrPtr,"%04hX",StatusI2C);
		MA_PutString_UART1(AdrPtr);

		MA_PutString_UART1(" Adr=");
		sprintf(AdrPtr,"%04hX",PageStart);
		MA_PutString_UART1(AdrPtr);
		
	}
	else          MA_PutString_UART1(" Poll Error"); 
	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);
	return Result;
}
