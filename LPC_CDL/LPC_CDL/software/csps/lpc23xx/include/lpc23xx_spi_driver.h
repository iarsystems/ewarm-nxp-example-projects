/***********************************************************************
* $Id: lpc23xx_spi_driver.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx SPI driver
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
#ifndef LPC23xx_SPI_DRIVER_H_
#define LPC23xx_SPI_DRIVER_H_
	
#include "lpc23xx_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * SPI driver private data and types
 **********************************************************************/

/* Macro for SPI Control Register */
/* Use first clock edge */
#define SPI_FIRST_CLK_EDGE		0x00
/* Use second clock edge */
#define SPI_SECOND_CLK_EDGE		0x01
/* Use active high clock */
#define SPI_CLK_HI				0x00
/* Use active low clock */
#define SPI_CLK_LO				0x01

/* Master control */
#define SPI_MASTER				0x01
/* Slave control */
#define SPI_SLAVE				0x00

/* Order of data to transfer - MSB */
#define SPI_ORDER_MSB			0x00
/* Order of data to transfer - LSB */
#define SPI_ORDER_LSB			0x01

/* Databit number = 8 */
#define SPI_DATABIT_8			0x08
/* Databit number = 9 */
#define SPI_DATABIT_9			0x09
/* Databit number = 10 */
#define SPI_DATABIT_10			0x0A
/* Databit number = 11 */
#define SPI_DATABIT_11			0x0B
/* Databit number = 12 */
#define SPI_DATABIT_12			0x0C
/* Databit number = 13 */
#define SPI_DATABIT_13			0x0D
/* Databit number = 14 */
#define SPI_DATABIT_14			0x0E
/* Databit number = 15 */
#define SPI_DATABIT_15			0x0F
/* Databit number = 16 */
#define SPI_DATABIT_16			0x00

/* Macro for return status value in SPI operation 
 * when using SPI_GetStatus () */
// SPI in idle state
#define SPI_STAT_IDLE		0x00
// SPI busy state
#define SPI_STAT_BUSY		((U32) (1 << 1))


/***********************************************************************
 * Public SPI driver functions
 **********************************************************************/

/* Init SPI device */
S32 SPI_Init (S32 SPINum, U32 Mode, U32 ClkRate, U32 Databit, 
			U32 PhaseMode, U32 ClkMode, U32 DataOrder);

/* Close the SPI device */
S32 SPI_Close (S32 SPIdev);

/* Configure SPI controller */
S32 SPI_Config (S32 SPIdev, U32 Mode, U32 ClkRate, U32 Databit, 
		U32 PhaseMode, U32 ClkMode, U32 DataOrder);

/* Setup clock rate for SPI device */
S32 SPI_SetClock (S32 SPIdev, U32 target_clock);

/* SPI Read and write in polling mode function */
S32 SPI_ReadWritePoll (S32 SPIdev,
	                 void *rbuffer,
	                 void *wbuffer,
	                 U32 length);

/* SPI Read and write in interrupt mode function */
S32 SPI_ReadWriteAsync (S32 SPIdev,
				    void *rbuffer,
				    void *wbuffer,
				    U32 length);

/* Set call back function for SPI in ASYNC Read Write mode */
S32 SPI_SetCBS (S32 SPIdev, void *pCBS);

/* Get status of current SPI operation */
U32 SPI_GetStatus (S32 SPIdev);

/* Common SPI interrupt handler */
void SPI_ISR (void);


#ifdef __cplusplus
}
#endif

#endif /*LPC23xx_SPI_DRIVER_H_*/
