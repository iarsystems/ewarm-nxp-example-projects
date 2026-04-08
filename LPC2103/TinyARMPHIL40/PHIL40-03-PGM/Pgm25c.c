//---------------------------------------------------------------------------
// Filename: Pgm25c.c
//---------------------------------------------------------------------------
// SPI routines for 25C256 programming
//---------------------------------------------------------------------------
// V0.1 01-12-2006 Initial release
//---------------------------------------------------------------------------
// Created for TinyARM PHIL40-03
// Tools used: IAR EWARM v4.40A
// Copyright PASAT 2004-2006
//---------------------------------------------------------------------------


#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */
#include "NXP/iolpc210x.h"  /* Defines Special function registers */

#include "ma_uart1.h"    /* UART1 Module driver header file */
#include "ma_gpio.h"     /* GPIO  Module driver header file */
#include "ma_spi.h"      /* SPI   Module driver header file */
#include "ma_i2c.h"      /* I2C   Module driver header file */
#include "ma_pcb.h"     /* PCB Module driver header file */

#include "string.h"
#include "stdio.h"
#include "easypgm.h"
#include "Pgm25c.h"

// External references
extern unsigned char RamBuffer[];
extern unsigned short CurrentPageSize;
// External functions
extern void Send_Some_Uart(U8 Data8, U8 Count8);

//----------------------------------------------
// Copy 25C256 EEPROM to RAM buffer (Load EEPROM)
//----------------------------------------------
void Exec_Copy25C256_RAM(void)
{
	int i;
	unsigned char *Ptr=RamBuffer+1;
	char AdrBuffer[10],DataBuffer[10];
	char *AdrPtr,*DataPtr,CurrData;

	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);

	AdrPtr=&AdrBuffer[0];
	DataPtr=&DataBuffer[0];

	for (i=0;i<Size_25C256;i++)
	{
		if ((i & 0xFF)==0)
		{
			MA_PutString_UART1(" Adr=");
			sprintf(AdrPtr,"%04hX",i);
			MA_PutString_UART1(AdrPtr);
			//MA_PutString_UART1(" Data=");
	    	Send_Some_Uart(KEY_LEFT,5+4);
		}

		CurrData = SPI_Read_Cycle(i);

//		if ((i & 0xFF)==0)
//		{
//			sprintf(DataPtr,"%02hX",CurrData);
//			MA_PutString_UART1(DataPtr);
//	    	Send_Some_Uart(KEY_LEFT,11+4+2);
//		}
		*Ptr++ = CurrData;
	}
	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);

}

//----------------------------------------------
// Copy RAM buffer to 25C256 EEPROM (Burn EEPROM)
//----------------------------------------------
short Exec_CopyRAM_25C256(void)
{
	int i,j;
	//unsigned char *Ptr=&RamBuffer[1];
	short Error;
	char AdrBuffer[10];
	char *AdrPtr;

	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);

	AdrPtr=&AdrBuffer[0];

	j=0;
	for (i=0; i<Size_25C256/CurrentPageSize; i++)
	{
		if ((j & 0xFF)==0)
		{
			MA_PutString_UART1(" Adr=");
			sprintf(AdrPtr,"%04hX",j);
			MA_PutString_UART1(AdrPtr);
	    	Send_Some_Uart(KEY_LEFT,9);
		}

		Error=SPI_Burn_Block(j);
		if (Error) return Error;
		j+=CurrentPageSize;
	}
	SPI_SendInstr_WRDI();

	MA_PutChar_UART1(0xD);
	MA_PutChar_UART1(0xA);
	return 0;
}

//----------------------------------------------
//----------------------------------------------
// 25C256 related routines
//----------------------------------------------
//----------------------------------------------

//----------------------------------------------
// 25C256 read cycle
//----------------------------------------------
unsigned char SPI_Read_Cycle(unsigned short MemAdr)
{
	unsigned char Data8=0;

	SPI_CS_Low();

	SPI_Send_READ();
	SPI_Wait_Empty();
	// This clears SPIF flag in SPSR
	SPI_Reset_SPIF();

	SPI_Send_Byte((unsigned char)(MemAdr>>8));
	SPI_Wait_Empty();
	// This clears SPIF flag in SPSR
	SPI_Reset_SPIF();

	SPI_Send_Byte((unsigned char)(MemAdr&0xFF));
	SPI_Wait_Empty();
	// This clears SPIF flag in SPSR
	SPI_Reset_SPIF();

	// Empty write
	SPI_Send_Byte(0);
	SPI_Wait_Empty();
	// This clears SPIF flag in SPSR
	SPI_Reset_SPIF();
    Data8=SPI_Read_Byte();
	SPI_CS_High();

	return Data8;
}

//----------------------------------------------
// 25C256 write cycle
//----------------------------------------------
unsigned char SPI_Burn_Block(int Index)
{
    unsigned char Data8;
    unsigned short i;
	
	// Catalyst 25C256 enters WriteDisable after previous write op
	SPI_SendInstr_WREN();
	// Now the sequence
	SPI_CS_Low();

	SPI_Send_WRITE();
	SPI_Wait_Empty();
	// This clears SPIF flag in SPSR
	SPI_Reset_SPIF();

	SPI_Send_Byte((unsigned char)(Index>>8));
	SPI_Wait_Empty();
	// This clears SPIF flag in SPSR
	SPI_Reset_SPIF();

	SPI_Send_Byte((unsigned char)(Index&0xFF));
	SPI_Wait_Empty();
	// This clears SPIF flag in SPSR
	SPI_Reset_SPIF();

	Index+=1;
	for (i=0;i<CurrentPageSize;i++)
	{
		SPI_Send_Byte(RamBuffer[Index]);
		SPI_Wait_Empty();
		// This clears SPIF flag in SPSR
		SPI_Reset_SPIF();
		Index++;
	}

	SPI_CS_High();
	do
	{
		Data8=SPI_Read_RDSR();
	}
	while (Data8 & 1);
// TOTO ESTE UPRAV !!!!!!!!!!!!!!!!!!!!!!
	return 0;
}

//-------------------------------------------------------------
// This clears SPIF flag in SPSR
//-------------------------------------------------------------
// Required for SPI
//-------------------------------------------------------------
void SPI_Reset_SPIF(void)
{
	volatile U8 Data;
	Data = S0SPDR;
}


//----------------------------------------------
// Send 1 byte to 25C256
//----------------------------------------------
unsigned char SPI_Send_Byte(unsigned char MyByte)
{
	unsigned char Data8;

	Data8=MA_PutChar_SPI(MyByte);
	return Data8;
}

//----------------------------------------------
// Wait for cycle end
//----------------------------------------------
void SPI_Wait_Empty(void)
{
	S8 Data8;

	do
	{
		Data8=MA_TestChar_SPI();
	}
	while (Data8==MA_EMPTY);
	// no error indication
}

//----------------------------------------------
// Send READ command to 25C256
//----------------------------------------------
unsigned char SPI_Send_READ(void)
{
	return SPI_Send_Byte(SPI25C_READ_CMD);
}

//----------------------------------------------
// Send WRITE command to 25C256
//----------------------------------------------
unsigned char SPI_Send_WRITE(void)
{
	return SPI_Send_Byte(SPI25C_WRITE_CMD);
}

//----------------------------------------------
// Send 1 byte instruction command to 25C256
//----------------------------------------------
unsigned char SPI_Send_Instruction(unsigned char MyByte)
{
	unsigned char Data8;

	SPI_CS_Low();

	Data8=SPI_Send_Byte(MyByte);
	SPI_Wait_Empty();
	// This clears SPIF flag in SPSR
	SPI_Reset_SPIF();

	SPI_CS_High();
	return Data8;
}

//----------------------------------------------
// Send WREN instruction to 25C256
//----------------------------------------------
unsigned char SPI_SendInstr_WREN(void)
{
	return SPI_Send_Instruction(SPI25C_WREN_CMD);
}

//----------------------------------------------
// Send WRDI instruction to 25C256
//----------------------------------------------
unsigned char SPI_SendInstr_WRDI(void)
{
	return SPI_Send_Instruction(SPI25C_WRDI_CMD);
}

//----------------------------------------------
// Send RDSR command to 25C256
//----------------------------------------------
unsigned char SPI_Send_RDSR(void)
{
	return SPI_Send_Byte(SPI25C_RDSR_CMD);
}

//----------------------------------------------
// Read 1 byte from 25C256
//----------------------------------------------
unsigned char SPI_Read_Byte(void)
{
	unsigned char Data8;

	Data8=MA_GetChar_SPI();
	return Data8;
}

//----------------------------------------------
// Read status register from 25C256
//----------------------------------------------
unsigned char SPI_Read_RDSR(void)
{
	unsigned char Data8;

	SPI_CS_Low();

	SPI_Send_RDSR();
	SPI_Wait_Empty();
	// This clears SPIF flag in SPSR
	SPI_Reset_SPIF();

	// Empty write
	SPI_Send_Byte(0);
	SPI_Wait_Empty();
	// This clears SPIF flag in SPSR
	SPI_Reset_SPIF();
    Data8=SPI_Read_Byte();

	SPI_CS_High();
	return Data8;
}

//----------------------------------------------
// Clear PIOx pin controlling 25C256 CSn pin
//----------------------------------------------
void SPI_CS_Low(void)
{
	// PIO P0.10 is used as CSn
	MA_WritePort_GPIO(0x00,0x400);
}

//----------------------------------------------
// Set PIOx pin controlling 25C256 CSn pin
//----------------------------------------------
void SPI_CS_High(void)
{
	// PIO P0.10 is used as CSn
	MA_WritePort_GPIO(0x400,0x400);
}
