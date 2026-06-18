/***********************************************************************
* $Id: lpc23xx_vic.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx VIC definition
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: VIC
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
#ifndef LPC23XX_VIC_H_
#define LPC23XX_VIC_H_

#include "lpc23xx.h"
#include "arm7tdmis_types.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/******************************************************************************/
/*                Vectored Interrupt Controller (VIC) structure               */
/******************************************************************************/
typedef struct 
{
	__IO U32 IRQSTATUS;   		/* IRQ interrupt status*/
    __IO U32 FIQSTATUS;   		/* FIQ interrupt status*/
    __IO U32 RAWINT;   			/* RAW interrupt status*/
    __IO U32 INTSELECT; 		/* Interrupt select*/
    __IO U32 INTENABLE; 		/* Interrupt enable*/
    __IO U32 INTENCLR; 			/* Interrupt enable clear*/
    __IO U32 SOFTINT; 			/* Software interrupt enable */
    __IO U32 SOFTINTCLR; 		/* Software interrupt disable */
    __IO U32 PROTECTION; 		/* Protection */
    __IO U32 SWPRIORITYMASK; 	/* Software Priority Mask Register */
    __IO U32 RESERVED0[54]; 	/* reserved */
    __IO U32 VECTADDRn[32]; 	/* Vector addr n 0..31 */
    __IO U32 RESERVED1[32]; 	/* reserved */
    __IO U32 VECTPRIORITYn[32]; /* vector control n 0..31 */
    __IO U32 RESERVED2[800]; 	/* reserved */
    __IO U32 VECTADDR;			/* Vector addr regs */
} VIC_Type;

/* Macro pointing to VIC registers address */
#define VIC	((VIC_Type *)(VIC_BASE))


/******************************************************************************/
/* 					Macro used in VIC driver definition 					  */
/******************************************************************************/
/* Interrupt source bit-position definition */
#define	WDT_INT  		0 		/* Watchdog interrupt */
#define	SWI_INT	 		1 		/* Software interrupt */
#define	ARM_CORE0_INT  	2 		/* ARM Core 0 interrupt */
#define	ARM_CORE1_INT  	3 		/* ARM Core 1 interrupt  */
#define	TIMER0_INT  	4 		/* Timer 0 interrupt */
#define	TIMER1_INT  	5 		/* Timer 1 interrupt */
#define	UART0_INT  		6 		/* UART 0 interrupt */
#define	UART1_INT  		7 		/* UART 1 interrupt */
#define	PWM1_INT  		8 		/* PWM1 interrupt */
#define	I2C0_INT  		9 		/* I2C 0 interrupt */
#define	SPI0_INT  		10 		/* SPI SSP0 interrupt */
#define	SPI1_INT  		11 		/* SSP1 interrupt */
#define	PLL_INT	 		12 		/* PLL interrupt */
#define	RTC_INT	 		13 		/* RTC interrupt */
#define	EINT0_INT  		14 		/* External 0 interrupt */
#define	EINT1_INT  		15 		/* External 1 interrupt */
#define	EINT2_INT  		16 		/* External 2 interrupt */
#define	EINT3_INT  		17 		/* External 3 interrupt - shared with GPIO interrupts */
#define	ADC0_INT  		18 		/* ADC 0 interrupt */
#define	I2C1_INT  		19 		/* I2C 1 interrupt */
#define	BOD_INT	 		20 		/* BOD interrupt */
#define	Ethernet_INT  	21 		/* WakeupInt, SoftInt, TxDoneInt, TxFinishedInt, 
									TxErrorInt, TxUnderrunInt, RxDoneInt,
									RxFinishedInt, RxErrorInt, RxOverrunInt */
#define	USB_INT	 		22 		/* USB interrupt */
#define	CAN_INT	 		23 		/* CAN Common, CAN 0 Tx, CAN 0 Rx, CAN 1 Tx,CAN 1 Rx */
#define	SD_MMC_INT  	24 		/* RxDataAvlbl, TxDataAvlbl, RxFifoEmpty, TxFifoEmpty,
									RxFifoFull, TxFifoFull, RxFifoHalfFull, TxFifoHalfEmpty,
									RxActive, TxActive, CmdActive, DataBlockEnd, StartBitErr,
									DataEnd, CmdSent, CmdRespEnd, RxOverrun,
									TxUnderrun, DataTimeOut, CmdTimeOut, DataCrcFail,
									CmdCrcFail */
#define	GP_DMA_INT  	25 		/* IntStatus of DMA channel 0, IntStatus of DMA channel 1 */
#define	TIMER2_INT  	26 		/* Match 0-3 Capture 0-1 */
#define	TIMER3_INT  	27 		/* Match 0-3 Capture 0-1 */
#define	UART2_INT  		28 		/* Rx Line Status (RLS)
									Transmit Holding Register Empty (THRE)
									Rx Data Available (RDA)
									Character Time-out Indicator (CTI)
									End of Auto-Baud (ABEO)
									Auto-Baud Time-Out (ABTO) */
#define	UART3_INT  		29 		/* Rx Line Status (RLS)
									Transmit Holding Register Empty (THRE)
									Rx Data Available (RDA)
									Character Time-out Indicator (CTI)
									End of Auto-Baud (ABEO)
									Auto-Baud Time-Out (ABTO) */
#define	I2C2_INT  		30 		/* SI (state change) */
#define	I2S_INT 		31 		/* IRQ_rx, IRQ_tx */

/* VIC size */
#define VIC_SIZE		32

#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_VIC_H_*/
