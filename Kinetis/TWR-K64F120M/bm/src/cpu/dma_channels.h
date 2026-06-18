/*
 * All software, source code, included documentation, and any implied know-how
 * are property of Freescale Semiconductor and therefore considered
 * CONFIDENTIAL INFORMATION.
 * This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.
 *
 * All Confidential Information remains the property of Freescale Semiconductor
 * and will not be copied or reproduced without the express written permission
 * of the Discloser, except for copies that are absolutely necessary in order
 * to fulfill the Purpose.
 *
 * Services performed by FREESCALE in this matter are performed AS IS and
 * without any warranty.
 * CUSTOMER retains the final decision relative to the total design and
 * functionality of the end product.
 * FREESCALE neither guarantees nor will be held liable by CUSTOMER for the
 * success of this project.
 *
 * FREESCALE disclaims all warranties, express, implied or statutory
 * including, but not limited to, implied warranty of merchantability or
 * fitness for a particular purpose on any hardware, software ore advise
 * supplied to the project by FREESCALE, and or any product resulting from
 * FREESCALE services.
 * In no event shall FREESCALE be liable for incidental or consequential
 * damages arising out of this agreement. CUSTOMER agrees to hold FREESCALE
 * harmless against any and all claims demands or actions by anyone on account
 * of any damage,or injury, whether commercial, contractual, or tortuous,
 * rising directly or indirectly as a result of the advise or assistance
 * supplied CUSTOMER in connectionwith product, services or goods supplied
 * under this Agreement.
 */

/*
 * File:	dma_channels.h
 * Purpose:	DMA request macros for use on Pioneer processors.
 * 			This file gives default DMA channel assignments
 *			for all of the possible Kinetis module DMA requests.
 *
 * Notes:	There are more DMA requests than DMA channels, so
 *			care should be taken to make sure that DMA channel
 *			assignments are unique for the modules that are
 *			being used at any time.
 */

#ifndef _DMA_CHANNELS_H
#define _DMA_CHANNELS_H

/* NOTE: There are more DMA requests than DMA channels, so
 *		 care should be taken to make sure that DMA channel
 *	 assignments are unique for the modules that are
 *	 being used at any time.
 *
 *	 It is recommended that you read the appropriate DMAMUX_CHCFGn
 *	 register before updating it to verify it is 0x0. If the
 *	 DMAMUX_CHCFGn register is not zero, then that indicates the
 *	 selected DMA channel might already be in use by another peripheral
 *	 (a more specific test would be to look for DMAMUX_CHCFGn[ENBL] set).
 *	 The module's DMA configuration routine can return an error
 *	 when this situation is detected.
 */

/* Default DMA channel assignments and module request macros */

/* UARTs */
#define DMA_UART0RX_CH			0
#define DMA_UART0TX_CH			1

#define DMA_UART1RX_CH			2
#define DMA_UART1TX_CH			3

#define DMA_UART2RX_CH			10
#define DMA_UART2TX_CH			11

#define DMA_UART3RX_CH			12
#define DMA_UART3TX_CH			13

#define DMA_UART4RX_CH			6
#define DMA_UART4TX_CH			7

#define DMA_UART5RX_CH			8
#define DMA_UART5TX_CH			9

/* SSI/SAI */
#define DMA_SSI0RX_CH			4
#define DMA_SSI0TX_CH			5

/* DSPIs */
#define DMA_DSPI0RX_CH			6
#define DMA_DSPI0TX_CH			7

#define DMA_DSPI1RX_CH			8
#define DMA_DSPI1TX_CH			9

#define DMA_DSPI2RX_CH			14
#define DMA_DSPI2TX_CH			15

/* I2Cs */
#define DMA_I2C0_CH				7
#define DMA_I2C1_CH				2

/* FTMs */
#define DMA_FTM0CH0_CH			5
#define DMA_FTM0CH1_CH			6
#define DMA_FTM0CH2_CH			3
#define DMA_FTM0CH3_CH			4
#define DMA_FTM0CH4_CH			12
#define DMA_FTM0CH5_CH			13
#define DMA_FTM0CH6_CH			14
#define DMA_FTM0CH7_CH			15

#define DMA_FTM1CH0_CH			10
#define DMA_FTM1CH1_CH			11

#define DMA_FTM2CH0_CH			0
#define DMA_FTM2CH1_CH			1

/* Ethernet timers */
#define DMA_ENETTMR0_CH			4
#define DMA_ENETTMR1_CH			8
#define DMA_ENETTMR2_CH			0
#define DMA_ENETTMR3_CH			15

/* ADCs */
#define DMA_ADC0_CH				12
#define DMA_ADC1_CH				3

/* HSCMPs */
#define DMA_HSCMP0_CH			13
#define DMA_HSCMP1_CH			2
#define DMA_HSCMP2_CH			9

/* 12-bit DAC */
#define DMA_12bDAC0_CH			14

/* CMT */
#define DMA_CMT_CH				5

/* PDB */
#define DMA_PDB_CH				10

/* GPIO Ports */
#define DMA_GPIOPORTA_CH		15
#define DMA_GPIOPORTB_CH		0
#define DMA_GPIOPORTC_CH		1
#define DMA_GPIOPORTD_CH		11
#define DMA_GPIOPORTE_CH		8

#endif /* _DMA_CHANNELS_H */