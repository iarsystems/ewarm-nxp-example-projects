/***********************************************************************
* $Id: lpc23xx_spi.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx SPI definition
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: SPI
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
#ifndef LPC23xx_SPI_H_
#define LPC23xx_SPI_H_

#include "lpc23xx.h"
#include "arm7tdmis_types.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/***********************************************************************
 * SPI Module Register Structure
 **********************************************************************/
typedef struct 
{
    __IO U32 SPCR;     		/* SPI Control Register */
    __IO U32 SPSR;     		/* SPI Status Register */
    __IO U32 SPDR;    		/* SPI Data Register */
    __IO U32 SPCCR;  		/* SPI Clock Counter Register */
    __IO U32 RESERVED[3];	/* Reserved */
    __IO U32 SPINT;    		/* SPI Interrupt Flag */
} SPI_Type;

/* Pin function definition */
// SCK pin
#define SPI_PIN_FUNC_SCK	15
// SSEL pin
#define SPI_PIN_FUNC_SSEL	16
// MISO pin
#define SPI_PIN_FUNC_MISO	17
// MOSI pin
#define SPI_PIN_FUNC_MOSI	18

/***********************************************************************
 * SPI Control Register defines
 **********************************************************************/
/* Bit enable, the SPI controller sends and receives the number of bits selected by bits 11:8 */
#define SPI_ENABLE_BIT		0x04 
/* Clock phase control bit */
#define SPI_CPHA_BIT		0x08  
/* Clock polarity control bit */
#define SPI_CPOL_BIT 		0x10 
/* SPI master mode enable */
#define SPI_MASTER_BIT 		0x20
/* LSB enable bit */
#define SPI_LSBF_BIT 		0x40
/* SPI interrupt enable bit */
#define SPI_SPIE_BIT 		0x80
/*  When bit 2 of this register is 1, this field controls the
number of bits per transfer */
#define SPI_BITS(n)			_SBF(8,n)

/***********************************************************************
 * SPI Status, Test Control Register defines
 **********************************************************************/
/* Slave abort */
#define SPI_ABRT_MASK 		0x08
/* Mode fault */
#define SPI_MODF_MASK		0x10
/* Read overrun */
#define SPI_ROVR_MASK		0x20
/* Write collision */
#define SPI_WCOL_MASK		0x40
/* SPI transfer complete flag */
#define SPI_SPIF_MASK 		0x80

/***********************************************************************
 * SPI Clock Counter Register defines
 **********************************************************************/
/* SSPI clock counter setting */
#define SPI_CLK_SCALE(n) 	n	

/***********************************************************************
 * SPI Interrupt Register defines
 **********************************************************************/
/* SPI interrupt flag */
#define SPI_INT_FLAG_CLR 	1

/* Macro pointing to SPI0 controller register base address */
#define SPI0 ((SPI_Type *) SPI0_BASE)

#ifdef __cplusplus
}
#endif

#endif /*LPC23xx_SPI_H_*/
