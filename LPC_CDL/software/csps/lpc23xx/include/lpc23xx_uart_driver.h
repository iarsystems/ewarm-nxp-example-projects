/***********************************************************************
* $Id: lpc23xx_uart_driver.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx UART driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: UART
*
***********************************************************************
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
#ifndef LPC23xx_UART_DRIVER_H
#define LPC23xx_UART_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lpc23xx_uart.h"

/***********************************************************************
 * Macro for setting UART baudrate argument 
 **********************************************************************/
#define BPS_2400	2400	/* Baudrate = 2.4 Kbps */
#define BPS_4800	4800	/* Baudrate = 4.8 Kbps */
#define BPS_9600	9600	/* Baudrate = 9.6 Kbps */
#define BPS_19200   19200   /* Baudrate = 19.2 Kbps */
#define BPS_28800	28800	/* Baudrate = 28.8 Kbps */
#define BPS_38400   38400	/* Baudrate = 38.4 Kbps */
#define BPS_57600   57600   /* Baudrate = 57.6 Kbps */
#define BPS_115200  115200  /* Baudrate = 115.2 Kbps */
#define BPS_153600  153600  /* Baudrate = 153.6 Kbps */
#define BPS_230400  230400  /* Baudrate = 230.4 Kbps */
#define BPS_460800  460800  /* Baudrate = 460.8 Kbps */

/********************************************************************
 * Macro for setting UART parity argument
 ********************************************************************/
#define PARITY_NONE		0x00	/* No parity */
#define PARITY_ODD		0x01	/* Odd parity */
#define PARITY_EVEN		0x02	/* Even parity */
#define PARITY_SP_1		0x03	/* Forced "1" stick parity */
#define PARITY_SP_0		0x04	/* Forced "0" stick parity */

/********************************************************************
 * Macro for setting UART parity argument
 ********************************************************************/
#define FIFO_DISABLE	0x04	/* Disable FIFO */
#define FIFO_LEVEL_1	0x00	/* FIFO 1 character trigger level */
#define FIFO_LEVEL_4	0x01	/* FIFO 4 character trigger level */
#define FIFO_LEVEL_8	0x02	/* FIFO 8 character trigger level */
#define FIFO_LEVEL_14	0x03	/* FIFO 14 character trigger level */
	
/********************************************************************
 * Macro for UART Port number definition
 ********************************************************************/
#define UART_PORT_0 0x00	/* UART port number 0 */
#define UART_PORT_1 0x01	/* UART port number 1 */

#if 0
#define UARTPort2 0x02
#define UARTPort3 0x03
#endif

/********************************************************************
 * UART driver compiler options
 ********************************************************************/

#ifndef UART_RING_BUFSIZE
/* Default UART ring buffer size in bytes. For each enabled UART, a
   transmit and receive ring buffer of this size will be defined */
#define UART_RING_BUFSIZE 256
#endif

#ifndef UART_0_ENABLE
/* The UART_0_ENABLE define enables support for UART 0 when set to 1 */
#define UART_0_ENABLE 1
#endif


#ifndef UART_1_ENABLE
/* The UART_1_ENABLE define enables support for UART 1 when set to 1 */
#define UART_1_ENABLE 1
#endif

/********************************************************************
 * UART driver API functions
 ********************************************************************/

/* Init UART */
S32 UART_Init (S32 UARTPortNum, U32 Baudrate, U32 Databit, 
				U32 Parity, U32 Stopbit);

/* Set baudrate for UART */
void UART_SetBaudrate (S32 UARTdev, U32 Baudrate);

/* Set databit for UART */
void UART_SetDatabit (S32 UARTdev, U32 Databit);

/* Set Parity for UART */
void UART_SetParity (S32 UARTdev, U32 Parity);

/* Set stop bit for UART */
void UART_SetStopbit (S32 UARTdev, U32 Stopbit);

/* Set FIFO level for UART */
void UART_SetFIFO (S32 UARTdev, U32 FIFOlevel);

/* Enable selected UART interrupt */
void UART_EnableInt (S32 UARTdev, U32 UARTIntType);

/* Disable selected UART interrupt */
void UART_DisableInt (S32 UARTdev, U32 UARTIntType);

/* Get current status of UART Line control register */
U32 UART_GetStatus (S32 UARTdev);

/* Close a UART */
S32 UART_Close (S32 UARTdev);

/* UART read function for polled mode */
S32 UART_ReadPoll (S32 UARTdev,
                        void *buffer,
                        S32 max_bytes);

/* UART write function for polled mode */
S32 UART_WritePoll (S32 UARTdev,
                         void *buffer,
                         S32 n_bytes);

/* UART read function for interrupt mode (using ring buffers) */
S32 UART_ReadRing (S32 UARTdev,
                      void *buffer,
                      S32 max_bytes);

/* UART write function for interrupt mode (using ring buffers) */
S32 UART_WriteRing (S32 UARTdev,
                       void *buffer,
                       S32 n_bytes);

/***********************************************************************
 * UART driver utilities
 **********************************************************************/

/* Put a character to UART port */
void UART_PutChar (S32 devid, CHAR ch);

/* Put a string to UART port */
void UART_Puts (S32 devid, const void *str);

/* Put a string to UART port and print new line */
void UART_Puts_ (S32 devid, const void *str);

/* Put a decimal number (UNS_8) to the UART */
void UART_PutDec (S32 devid, U64 decnum);

/* Covert a hex number (UNS_8) to string and send to UART */
void UART_PutHex (S32 devid, U32 hexnum);

/********************************************************************
 * UART driver support functions
 ********************************************************************/

#if UART_0_ENABLE == 1
/* UART 0 interrupt function */
void uart0_isr(void);
#endif
#if UART_1_ENABLE == 1
/* UART 1 interrupt function */
void uart1_isr(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* LPC23xx_UART_DRIVER_H */
