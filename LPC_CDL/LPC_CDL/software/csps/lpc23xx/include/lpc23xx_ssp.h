/***********************************************************************
 * $Id: lpc23xx_ssp.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx SSP definitions
 *
 * Description:
 *     This file contains the structure definitions and manifest
 *     constants for the LPC23xx component: SSP
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
#ifndef LPC23XX_SSP_H_
#define LPC23XX_SSP_H_


#include "lpc23xx.h"
#include "arm7tdmis_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * SSP Module Register Structure
 **********************************************************************/
/* SSP Module Register Structure */
typedef struct
{
    __IO U32 CR0;     	/* SSP control register 0 */
    __IO U32 CR1;     	/* SSP control register 1 */
    __IO U32 DR;    	/* SSP data register */
    __IO U32 SR;      	/* SSP status register */
    __IO U32 CPSR;    	/* SSP clock prescale register */
    __IO U32 IMSC;    	/* SSP interrupt mask register */
    __IO U32 RIS;    	/* SSP raw interrupt status register */
    __IO U32 MIS;     	/* SSP masked interrupt status register */
    __IO U32 ICR;    	/* SSP interrupt clear register */
    __IO U32 DMACR;	 	/* DMA control Register */
} SSP_Type;

/***********************************************************************
 * CR0 register definitions
 **********************************************************************/
/* SSP data size select, must be 4 bits to 16 bits */
#define SSP_CR0_DSS(n)   _SBF(0,((n-1)&0xF))
/* SSP control 0 Motorola SPI mode */
#define SSP_CR0_FRF_MOT  0x00000000
/* SSP control 0 TI synchronous serial mode */
#define SSP_CR0_FRF_TI   0x00000010
/* SSP control 0 National Microwire mode */
#define SSP_CR0_FRF_NS   0x00000020
/* SSP control 0 protocol mask */
#define SSP_CR0_PRT_MSK  0x00000030
/* SPI clock polarity bit (used in SPI mode only), (1) = maintains the
   bus clock high between frames, (0) = low */
#define SSP_CR0_CPOL     _BIT(6)
/* SPI clock out phase bit (used in SPI mode only), (1) = captures data
   on the second clock transition of the frame, (0) = first */
#define SSP_CR0_CPHA     _BIT(7)
/* SSP serial clock rate value load macro, divider rate is
   PERIPH_CLK / (cpsr * (SCR + 1)) */
#define SSP_CR0_SCR(n)   _SBF(8,(n&0xFF))

/***********************************************************************
 * CR1 register definitions
 **********************************************************************/
/* SSP control 1 loopback mode enable bit */
#define SSP_CR1_LBM		_BIT(0)
/* SSP control 1 enable bit */
#define SSP_CR1_SSE		_BIT(1)
/* SSP control 1 master/slave bit, (1) = master, (0) = slave */
#define SSP_CR1_MS		_BIT(2)
/* SSP control 1 slave out disable bit, disables transmit line in slave
   mode */
#define SSP_CR1_SOD      _BIT(3)

/***********************************************************************
 * DR register definitions
 **********************************************************************/
/* SSP data mask */
#define SSP_DATAMASK(n)   ((n) & 0xFFFF)

/***********************************************************************
 * SR register definitions
 **********************************************************************/
/* SSP status TX FIFO Empty bit */
#define SSP_SR_TFE      _BIT(0)
/* SSP status TX FIFO not full bit */
#define SSP_SR_TNF      _BIT(1)
/* SSP status RX FIFO not empty bit */
#define SSP_SR_RNE      _BIT(2)
/* SSP status RX FIFO full bit */
#define SSP_SR_RFF      _BIT(3)
/* SSP status SSP Busy bit */
#define SSP_SR_BSY      _BIT(4)

/***********************************************************************
 * CPSR register definitions
 **********************************************************************/
/* SSP clock prescaler */
#define   SSP_CPSR_CPDVSR(n) 	_SBF(0,(n&0xFE))

/***********************************************************************
 * IMSC, RIS, MIS and ICR registers definitions
 **********************************************************************/
/* SSP interrupt bit for RX FIFO overflow */
#define SSP_IIR_RORRIS   _BIT(0)
/* SSP interrupt bit for RX FIFO not empty and has a data timeout */
#define SSP_IIR_RTRIS    _BIT(1)
/* SSP interrupt bit for RX FIFO half full */
#define SSP_IIR_RXRIS    _BIT(2)
/* SSP interrupt bit for TX FIFO half empty */
#define SSP_IIR_TXRIS    _BIT(3)

/***********************************************************************
 * DMACR register definitions
 **********************************************************************/
/* SSP bit for enabling RX DMA */
#define SSP_DMA_RXDMA  _BIT(0)
/* SSP bit for enabling TX DMA */
#define SSP_DMA_TXDMA  _BIT(1)

/* Macros pointing to SSP 0 registers */
#define SSP0  ((SSP_Type *)(SSP0_BASE))
/* Macros pointing to SSP 1 registers */
#define SSP1  ((SSP_Type *)(SSP1_BASE))

#ifdef __cplusplus
}
#endif


#endif /*LPC23XX_SSP_H_*/
