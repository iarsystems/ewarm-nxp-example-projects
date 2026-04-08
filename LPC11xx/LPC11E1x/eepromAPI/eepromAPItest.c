/****************************************************************************
 *   $Id: eepromAPItest.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC11Exx EEPROM example
 *
 *   Description:
 *     This file contains EEPROM test modules, main entry, to test EEPROM APIs.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 * Permission to use, copy, modify, and distribute this software and its 
 * documentation is hereby granted, under NXP Semiconductors' 
 * relevant copyright in the software, without fee, provided that it 
 * is used in conjunction with NXP Semiconductors microcontrollers.  This 
 * copyright, permission, and disclaimer notice must appear in all copies of 
 * this code.
****************************************************************************/
#include "LPC11Exx.h"
#include "uart.h"
#include "eepromAPI.h"
#include <stdio.h>
#include <string.h>

#define EE_SIZE 4032
const uint8_t* menu_array[6] = {
"\r\nLPC11Exx eepromAPI test\r\n",
"'d' - Display Contents of EEPROM (Little Endian)\r\n",
"'f' - Fill EEPROM with 0xFF\r\n",
"'0' - Fill EEPROM with 0x00\r\n",
"'c' - Fill EEPROM with incrementing bytes\r\n",
"'t' - Fill EEPROM with timestamps\r\n\r\n>"};

void menu(void)
{
	uint32_t i;
	for (i=0; i<6; i++)
	{
		print_string( (uint8_t*) menu_array[i] );
	}
}

void eefill(uint8_t fillPattern)
{
	uint32_t i,j,z;
	z 	= (	fillPattern<<24 | 
			fillPattern<<16 |
			fillPattern<<8	|
			fillPattern<<0 );

	for (i=0; i<EE_SIZE; i+=16)
	{
		for (j=0; j<16; j+=4)
		{
			writeEEPROM( (uint8_t*) i+j, 
						 (uint8_t*) &z, sizeof(z));
		}
	}		
}

void eeDump(void)
{
	uint32_t i,j,z;

	char valBuff[12];
	char lineBuff[80];

	for (i=0; i<EE_SIZE; i+=16)
	{
		sprintf(lineBuff,"0x%03X ", i);
		for (j=0; j<16; j+=4)
		{
			readEEPROM( (uint8_t*) i+j, 
						(uint8_t*) &z, sizeof(z));									
			sprintf(valBuff,"%08X ", z);
			strcat(lineBuff,valBuff);
		}
		strcat(lineBuff,"\r\n");
		print_string((uint8_t*) lineBuff);
	}		
}

int main (void)
{	    		
	uint8_t c;
	uint8_t data[16];
	uint32_t i, hold;
	char cmd;
	char lineBuff[80];
    
	UARTInit(9600);
	NVIC_DisableIRQ(UART_IRQn);

	/* Setup Timer to performance metrics */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);

	menu();
	/* Loop forever */
	while (1) 
	{				
		cmd = get_key();		
		print_string("\r\n");
		LPC_CT32B0->TCR = 2;
		switch(cmd)
		{
			case 'f':
				eefill(0xFF);
				eeDump();
				break;
	
			case '0':
				eefill(0x00);
				eeDump();				
				break;
	
			case '1':
			    data[0] = 0xA0;
			    data[1] = 0xA1;
			    data[2] = 0xA2;
			    data[3] = 0xA3;
			    data[4] = 0xA4;
			    data[5] = 0xA5;
			    data[6] = 0xA6;
			    data[7] = 0xA7;
			    data[8] = 0xA8;
			    data[9] = 0xA9;
			    data[10] = 0xAA;
			    data[11] = 0xAB;
			    data[12] = 0xAC;
			    data[13] = 0xAD;
			    data[14] = 0xAE;
			    data[15] = 0xAF;
				writeEEPROM( (uint8_t*) 0xf01, (uint8_t*) &data[0], 16 );
				eeDump();
				break;

			case '2':
			    data[0] = 0x50;
			    data[1] = 0x51;
			    data[2] = 0x52;
			    data[3] = 0x53;
			    data[4] = 0x54;
			    data[5] = 0x55;
			    data[6] = 0x56;
			    data[7] = 0x57;
			    data[8] = 0x58;
			    data[9] = 0x59;
			    data[10] = 0x5A;
			    data[11] = 0x5B;
			    data[12] = 0x5C;
			    data[13] = 0x5D;
			    data[14] = 0x5E;
			    data[15] = 0x5F;
				writeEEPROM( (uint8_t*) 0xf02, (uint8_t*) &data[0], 16 );
				eeDump();
				break;

			case '3':
			    data[0] = 0xA0;
			    data[1] = 0xA1;
			    data[2] = 0xA2;
			    data[3] = 0xA3;
			    data[4] = 0xA4;
			    data[5] = 0xA5;
			    data[6] = 0xA6;
			    data[7] = 0xA7;
			    data[8] = 0xA8;
			    data[9] = 0xA9;
			    data[10] = 0xAA;
			    data[11] = 0xAB;
			    data[12] = 0xAC;
			    data[13] = 0xAD;
			    data[14] = 0xAE;
			    data[15] = 0xAF;
				writeEEPROM( (uint8_t*) 0xf03, (uint8_t*) &data[0], 16 );
				eeDump();
				break;

			case '5':
			    data[0] = 0xA0;
			    data[1] = 0xA1;
			    data[2] = 0xA2;
			    data[3] = 0xA3;
				writeEEPROM( (uint8_t*) 0xf05, (uint8_t*) &data[0], 4 );
				eeDump();
				break;

			case '6':
			    data[0] = 0x50;
			    data[1] = 0x51;
			    data[2] = 0x52;
			    data[3] = 0x53;
				writeEEPROM( (uint8_t*) 0xf06, (uint8_t*) &data[0], 4 );
				eeDump();
				break;

			case '7':
			    data[0] = 0xA0;
			    data[1] = 0xA1;
			    data[2] = 0xA2;
			    data[3] = 0xA3;
				writeEEPROM( (uint8_t*) 0xf07, (uint8_t*) &data[0], 4 );
				eeDump();
				break;

			case '9':
			    data[0] = 0xA0;
			    data[1] = 0xA1;
				writeEEPROM( (uint8_t*) 0xf09, (uint8_t*) &data[0], 2 );
				eeDump();
				break;

			case 'a':
			    data[0] = 0x50;
			    data[1] = 0x51;
				writeEEPROM( (uint8_t*) 0xf0a, (uint8_t*) &data[0], 2 );
				eeDump();
				break;

			case 'b':
			    data[0] = 0xA0;
			    data[1] = 0xA1;
				writeEEPROM( (uint8_t*) 0xf0b, (uint8_t*) &data[0], 2 );
				eeDump();
				break;

			case 'c':
				for (i=0,c=0x0; i<EE_SIZE; i+=4)
				{
					hold = c++;
					hold |= c++<<8;
					hold |= c++<<16;
					hold |= c++<<24;
					writeEEPROM( (uint8_t*) i, (uint8_t*) &hold, 4 );
				}
				eeDump();
				break;

			case 't':
				LPC_CT32B0->TCR = 1;
				for (i=0; i<EE_SIZE; i+=4)
				{
					LPC_CT32B0->TCR = 0;
					hold = LPC_CT32B0->TC;
					writeEEPROM( (uint8_t*) i, (uint8_t*) &hold, 4 );
				}
				eeDump();
				break;
						 	
			case 'd':	
				eeDump();		
				break;
	
			/* Default case whenever an incorrect cmd has been entered */
			default:
				print_string("Invalid command!\r\n\r\n");
				break;
	
			}
		if (LPC_CT32B0->TC > 0)
		{
			sprintf(lineBuff,"Cycles:%03d\r\n", LPC_CT32B0->TC);
			print_string((uint8_t*) lineBuff);					
		}
		menu();
	}
}
