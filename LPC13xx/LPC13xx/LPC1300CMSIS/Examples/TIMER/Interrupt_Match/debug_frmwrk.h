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

#ifndef DEBUG_FRMWRK_H_
#define DEBUG_FRMWRK_H_

#include "lpc13xx_uart.h"
#include "lpc13xx.h"

#define USED_UART_DEBUG_PORT	0

#if (USED_UART_DEBUG_PORT==0)
#define DEBUG_UART_PORT	UART0
#elif (USED_UART_DEBUG_PORT==1)
#define DEBUG_UART_PORT	UART1
#endif

#define _DBG(x)	 	_db_msg(x)
#define _DBG_(x)	_db_msg_(x)
#define _DBC(x)	 	_db_char(x)
#define _DBD(x)	 	_db_dec(x)
#define _DBD16(x)	 _db_dec_16(x)
#define _DBD32(x)	 _db_dec_32(x)
#define _DBH(x)	 	_db_hex(x)
#define _DBH16(x)	 _db_hex_16(x)
#define _DBH32(x)	 _db_hex_32(x)
#define _DG			_db_get_char()

extern void (*_db_msg)(const void *s);
extern void (*_db_msg_)(const void *s);
extern void (*_db_char)(uint8_t ch);
extern void (*_db_dec)(uint8_t decn);
extern void (*_db_dec_16)(uint16_t decn);
extern void (*_db_dec_32)(uint32_t decn);
extern void (*_db_hex)(uint8_t hexn);
extern void (*_db_hex_16)(uint16_t hexn);
extern void (*_db_hex_32)(uint32_t hexn);
extern uint8_t (*_db_get_char)();

void UARTPutChar (uint8_t ch);
void UARTPuts(const void *str);
void UARTPuts_(const void *str);
void UARTPutDec(uint8_t decnum);
void UARTPutDec16(uint16_t decnum);
void UARTPutDec32(uint32_t decnum);
void UARTPutHex (uint8_t hexnum);
void UARTPutHex16 (uint16_t hexnum);
void UARTPutHex32 (uint32_t hexnum);
uint8_t UARTGetChar ();
void debug_frmwrk_init(void);

#endif /* DEBUG_FRMWRK_H_ */
