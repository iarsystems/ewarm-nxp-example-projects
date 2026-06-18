//---------------------------------------------------------------------------
// Filename: Pgm25c.h
//---------------------------------------------------------------------------
// Headers for SPI routines for 25C256 programming
// V0.0 02-06-2004 Initial release
//---------------------------------------------------------------------------
// Created by Ivan Paulik for TinyARM DIP40
// Tools used: IAR EWARM v4.10B and IAR MakeApp for LPC2106 v4.00C
// Copyright PASAT 2004
//---------------------------------------------------------------------------

#ifndef _PGM25C_INCLUDED
#define _PGM25C_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

//Function prototypes
MEMORY_ATTRIBUTE void Exec_Copy25C256_RAM(void);
MEMORY_ATTRIBUTE short Exec_CopyRAM_25C256(void);

MEMORY_ATTRIBUTE unsigned char SPI_Read_Cycle(unsigned short MemAdr);
MEMORY_ATTRIBUTE unsigned char SPI_Burn_Block(int Index);
MEMORY_ATTRIBUTE unsigned char SPI_Send_Byte(unsigned char MyByte);
MEMORY_ATTRIBUTE void SPI_Wait_Empty(void);
MEMORY_ATTRIBUTE unsigned char SPI_Send_READ(void);
MEMORY_ATTRIBUTE unsigned char SPI_Send_WRITE(void);
MEMORY_ATTRIBUTE unsigned char SPI_Send_Instruction(unsigned char MyByte);
MEMORY_ATTRIBUTE unsigned char SPI_SendInstr_WREN(void);
MEMORY_ATTRIBUTE unsigned char SPI_SendInstr_WRDI(void);
MEMORY_ATTRIBUTE unsigned char SPI_Send_RDSR(void);
MEMORY_ATTRIBUTE unsigned char SPI_Read_Byte(void);
MEMORY_ATTRIBUTE unsigned char SPI_Read_RDSR(void);
MEMORY_ATTRIBUTE void SPI_CS_Low(void);
MEMORY_ATTRIBUTE void SPI_CS_High(void);
MEMORY_ATTRIBUTE void SPI_Reset_SPIF(void);

#endif




