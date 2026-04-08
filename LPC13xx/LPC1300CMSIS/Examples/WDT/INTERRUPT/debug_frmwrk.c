/**
 * @file	: debug_frmwrk.c
 * @brief	: Contains some utilities that used for debugging through UART
 * @version	: 1.0
 * @date	: 27. Nov. 2009
 * @author	: ThieuTrinh
 *----------------------------------------------------------------------------
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
 **********************************************************************/

#ifndef _DEBUG_FRMWRK_
#define _DEBUG_FRMWRK_

#include "debug_frmwrk.h"
//#include "lpc13xx_pinsel.h"
#include "lpc13xx.h"

/* Debug framework */

void (*_db_msg)(const void *s);
void (*_db_msg_)(const void *s);
void (*_db_char)(uint8_t ch);
void (*_db_dec)(uint8_t decn);
void (*_db_dec_16)(uint16_t decn);
void (*_db_dec_32)(uint32_t decn);
void (*_db_hex)(uint8_t hexn);
void (*_db_hex_16)(uint16_t hexn);
void (*_db_hex_32)(uint32_t hexn);
uint8_t (*_db_get_char)(void);


/*********************************************************************//**
 * @brief		Puts a character to UART port
 * @param[in]	ch		Character to put
 * @return		None
 **********************************************************************/
void UARTPutChar (uint8_t ch)
{
	UART_Send(&ch, 1, BLOCKING);
}


/*********************************************************************//**
 * @brief		Get a character to UART port
 * @param[in]	ch		Character to put
 * @return		None
 **********************************************************************/
uint8_t UARTGetChar (void)
{
	uint8_t tmp = 0;
	UART_Receive(&tmp, 1, BLOCKING);
	return(tmp);
}


/*********************************************************************//**
 * @brief		Puts a string to UART port
 * @param[in]	str 	string to put
 * @return		None
 **********************************************************************/
void UARTPuts(const void *str)
{
	uint8_t *s = (uint8_t *) str;

	while (*s)
	{
		UARTPutChar(*s++);
	}
}


/*********************************************************************//**
 * @brief		Puts a string to UART port and print new line
 * @param[in]	str		String to put
 * @return		None
 **********************************************************************/
void UARTPuts_(const void *str)
{
	UARTPuts (str);
	UARTPuts ("\n\r");
}


/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutDec(uint8_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	UARTPutChar('0'+c3);
	UARTPutChar('0'+c2);
	UARTPutChar('0'+c1);
}

/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutDec16(uint16_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	uint8_t c4=(decnum/1000)%10;
	uint8_t c5=(decnum/10000)%10;
	UARTPutChar('0'+c5);
	UARTPutChar('0'+c4);
	UARTPutChar('0'+c3);
	UARTPutChar('0'+c2);
	UARTPutChar('0'+c1);
}

/*********************************************************************//**
 * @brief		Puts a decimal number to UART port
 * @param[in]	decnum	Decimal number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutDec32(uint32_t decnum)
{
	uint8_t c1=decnum%10;
	uint8_t c2=(decnum/10)%10;
	uint8_t c3=(decnum/100)%10;
	uint8_t c4=(decnum/1000)%10;
	uint8_t c5=(decnum/10000)%10;
	uint8_t c6=(decnum/100000)%10;
	uint8_t c7=(decnum/1000000)%10;
	uint8_t c8=(decnum/10000000)%10;
	uint8_t c9=(decnum/100000000)%10;
	uint8_t c10=(decnum/100000000)%10;
	UARTPutChar('0'+c10);
	UARTPutChar('0'+c9);
	UARTPutChar('0'+c8);
	UARTPutChar('0'+c7);
	UARTPutChar('0'+c6);
	UARTPutChar('0'+c5);
	UARTPutChar('0'+c4);
	UARTPutChar('0'+c3);
	UARTPutChar('0'+c2);
	UARTPutChar('0'+c1);
}

/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	hexnum	Hex number (8-bit long)
 * @return		None
 **********************************************************************/
void UARTPutHex (uint8_t hexnum)
{
	uint8_t nibble, i;

	UARTPuts("0x");
	i = 1;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		UARTPutChar((nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}


/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	hexnum	Hex number (16-bit long)
 * @return		None
 **********************************************************************/
void UARTPutHex16 (uint16_t hexnum)
{
	uint8_t nibble, i;

	UARTPuts("0x");
	i = 3;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		UARTPutChar((nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}

/*********************************************************************//**
 * @brief		Puts a hex number to UART port
 * @param[in]	hexnum	Hex number (32-bit long)
 * @return		None
 **********************************************************************/
void UARTPutHex32 (uint32_t hexnum)
{
	uint8_t nibble, i;

	UARTPuts("0x");
	i = 7;
	do {
		nibble = (hexnum >> (4*i)) & 0x0F;
		UARTPutChar((nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble));
	} while (i--);
}


/*********************************************************************//**
 * @brief		Initialize Debug frame work through initializing UART port
 * @param[in]	None
 * @return		None
 **********************************************************************/
void debug_frmwrk_init(void)
{
	// Initialize DEBUG_UART_PORT peripheral with given to corresponding parameter
	UART_Init();

	// Enable UART Transmit
	UART_TXCmd(ENABLE);

	_db_msg	= UARTPuts;
	_db_msg_ = UARTPuts_;
	_db_char = UARTPutChar;
	_db_hex = UARTPutHex;
	_db_hex_16 = UARTPutHex16;
	_db_hex_32 = UARTPutHex32;
	_db_dec = UARTPutDec;
	_db_dec_16 = UARTPutDec16;
	_db_dec_32 = UARTPutDec32;
	_db_get_char = UARTGetChar;
}

#endif /* _DEBUG_FRMWRK_ */
