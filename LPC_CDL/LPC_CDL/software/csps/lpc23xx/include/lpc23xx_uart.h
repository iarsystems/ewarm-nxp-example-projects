/***********************************************************************
* $Id: lpc23xx_uart.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx ADC definition
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
#ifndef LPC23xx_UART_H
#define LPC23xx_UART_H

#include "arm7tdmis_types.h"
#include "lpc23xx.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/********************************************************************
 * UART Module Register Structure
 ********************************************************************/
/* UART Module Register Structure (for all UARTs 0, 2, 3) */ 
typedef struct 
{
	__IO U32 RBTHDLR; 		/* Receive-Transmit-Divisor LSB register */
	__IO U32 DMIER; 		/* Divisor MSB-interrupt enable register */
	__IO U32 IIFCR; 		/* Interrupt ID-FIFO control register */
	__IO U32 LCR; 			/* Line control register */
	__IO U32 RESERVED0;		/* reserved */
	__IO U32 LSR; 			/* Line Status register */
	__IO U32 RESERVED1;		/* Reserved */
	__IO U32 SCR; 			/* Sratch pad register */
	__IO U32 ACR; 			/* Auto baud control register */
	__IO U32 ICR;			/* IrDA Control registerister */
	__IO U32 FDR; 			/* Fractional devider register */
	__IO U32 RESERVED2;		/* Reserved */
	__IO U32 TER; 			/* Tx enable register */
} UART_Type;

/* UART Module Register Structure (for UARTs 1) */ 
typedef struct 
{
	__IO U32 RBTHDLR; 		/* Receive-Transmit-Divisor LSB register */
	__IO U32 DMIER; 		/* Divisor MSB-interrupt enable register */
	__IO U32 IIFCR; 		/* Interrupt ID-FIFO control register */
	__IO U32 LCR; 			/* Line Control register */
	__IO U32 MCR;			/* Modem Control status */
	__IO U32 LSR; 			/* Line Status register */
	__IO U32 MSR;			/* Modem Status registerister */
	__IO U32 SCR; 			/* Sratch Pad register */
	__IO U32 ACR; 			/* Auto baud control register */
	__IO U32 RESERVED0;		/* Reserved */
	__IO U32 FDR; 			/* Fractional devider register*/
	__IO U32 RESERVED1;		/* reserved */
	__IO U32 TER; 			/* Tx enable register */
} UART_Modem_Type;

/***********************************************************************
 * UART interrupt enable register defines
 **********************************************************************/
#define UART_IER_RBR		0x01 /*RBR Interrupt enable*/
#define UART_IER_THRE		0x02 /*THR Interrupt enable*/
#define UART_IER_RLS		0x04 /*RX line status interrupt enable*/

/***********************************************************************
 * UART interrupt indentification register defines
 **********************************************************************/
#define UART_IIR_NOPEND		0x01 /*Interrupt no pending*/
#define UART_IIR_RLS		0x06 /*Interrupt indentification: 
Receive line status*/ 
#define UART_IIR_RDA		0x04 /*Interrupt indentification: 
Receive data avalable*/
#define UART_IIR_CTI		0x0C /*Interrupt indentification: 
Character time-out indicator*/
#define UART_IIR_THRE		0x02 /*Interrupt indentification: 
THRE interrupt*/

/***********************************************************************
 * UART FIFO control register defines
 **********************************************************************/
#define UART_FIFO_EN		0x00000001 /* UART FIFO enable */
#define UART_FIFO_RX_RS		0x00000002 /* UART FIFO RX reset */
#define UART_FIFO_TX_RS		0x00000004 /* UART FIFO TX reset */
#define UART_FIFO_TRG_LEV0	0x00000000 /* UART FIFO trigger level 0 */
#define UART_FIFO_TRG_LEV1	0x00000040 /* UART FIFO trigger level 1 */
#define UART_FIFO_TRG_LEV2	0x00000080 /* UART FIFO trigger level 2 */
#define UART_FIFO_TRG_LEV3	0x000000C0 /* UART FIFO trigger level 3 */

/***********************************************************************
 * UART line control register defines
 **********************************************************************/
#define UART_LCR_WLEN5     0x00000000   /* UART 5 bit data mode */
#define UART_LCR_WLEN6     0x00000001   /* UART 6 bit data mode */
#define UART_LCR_WLEN7     0x00000002   /* UART 7 bit data mode */
#define UART_LCR_WLEN8     0x00000003   /* UART 8 bit data mode */
#define UART_LCR_STP2      0x00000004   /* UART Two Stop Bits Select */
#define UART_LCR_PEN       0x00000008   /* UART Parity Enable */
#define UART_LCR_PODD      0x00000000   /* UART Odd Parity Select */
#define UART_LCR_PEVEN     0x00000010   /* UART Even Parity Select */
#define UART_LCR_SP_1	   0x00000020	/* UART force 1 stick parity */
#define UART_LCR_SP_0	   0x00000030	/* UART force 0 stick parity */

#define UART_LCR_BRKEN     0x00000040   /* UART Transmission 
Break enable */
#define UART_LCR_DLABDIS   0x00000000	/* UART Divisor Latches 
Access bit disable */
#define UART_LCR_DLABEN    0x00000080	/* UART Divisor Latches 
Access bit enable */

/***********************************************************************
 * UART line status register defines
 **********************************************************************/
#define UART_LSR_RDR	0x01 /*Line status reg: Receive data ready*/
#define UART_LSR_OE		0x02 /*Line status reg: Overrun error*/
#define UART_LSR_PE		0x04 /*Line status reg: Parity error*/
#define UART_LSR_FE		0x08 /*Line status reg: Framing error*/
#define UART_LSR_BI		0x10 /*Line status reg: Break interrupt*/
#define UART_LSR_THRE	0x20 /*Line status reg: Transmit holding reg empty*/
#define UART_LSR_TEMT	0x40 /*Line status reg: Transmitter empty*/
#define UART_LSR_RXFE	0x80 /*Error in RX FIFO*/

/***********************************************************************
 * UART Tx Enable register defines
 **********************************************************************/
#define UART_TER_TXEN	0x80 /* Transmit enable bit */

/***********************************************************************
 * UART IrDA control register defines
 **********************************************************************/
/* IrDA mode enable */
#define IRDAEN			_BIT(0)
/* IrDA serial input inverted */
#define IRDAINV			_BIT(1)
/* IrDA fixed pulse width mode */
#define IRDAFIXPULSE_EN	_BIT(2)
/* PulseDiv mask - Configures the pulse when FixPulseEn = 1 */
#define IRDAPDIV_MASK	_SBF(0x07, 3)
/* PulseDiv configuration field - Configures the pulse when FixPulseEn = 1 */
#define IRDAPDIV_CFG(x)	_SBF(x, 3)

/* UART Data mask (8 bits) */
#define UART_DATA_MASK    0xFF         

/* Macro pointing to UART registers at UART 0 base address */
#define UART0  ((UART_Type *)(UART0_BASE))

/* Macro pointing to UART registers at UART 1 base address */
#define UART1  ((UART_Type *)(UART1_BASE))

#ifdef __cplusplus
}
#endif

#endif /* LPC23xx_UART_H */
