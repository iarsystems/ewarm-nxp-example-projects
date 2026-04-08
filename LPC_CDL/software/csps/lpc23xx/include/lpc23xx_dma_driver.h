/***********************************************************************
* $Id: lpc23xx_dma_driver.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx GPDMA driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: GPDMA
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
#ifndef LPC23XX_DMA_DRIVER_H_
#define LPC23XX_DMA_DRIVER_H_

#include "lpc23xx_dma.h"

#ifdef __cplusplus
extern "C"
{
#endif

	
/******************************************************************************/
/*                		Macro used for DMA function               		  	  */
/******************************************************************************/

/* Flow control and transfer type in Channel Configuration registers */
/* Memory to memory - DMA control */
#define GPDMA_M2M				0x00
/* Memory to peripheral - DMA control */
#define GPDMA_M2P				0x01
/* Peripheral to memory - DMA control */
#define GPDMA_P2M				0x02
/* Source peripheral to destination peripheral - DMA control */
#define GPDMA_P2P				0x03
/* Source peripheral to destination peripheral - Destination peripheral control */
#define GPDMA_SP2DP_DP			0x04
/* Memory to peripheral - Peripheral control */
#define GPDMA_M2P_P				0x05
/* Peripheral to memory - Peripheral control */
#define GPDMA_P2M_P				0x06	
/* Source peripheral to destination peripheral - Source peripheral control */
#define GPDMA_SP2DP_SP			0x07


/* Macro used for Channel Control Registers */

/* Burst size in Source and Destination definition */
/* Burst size = 1 */
#define GPDMA_BSIZE_1 	0x00
/* Burst size = 4 */
#define GPDMA_BSIZE_4 	0x01
/* Burst size = 8 */
#define GPDMA_BSIZE_8 	0x02
/* Burst size = 16 */
#define GPDMA_BSIZE_16 	0x03
/* Burst size = 32 */
#define GPDMA_BSIZE_32 	0x04
/* Burst size = 64 */
#define GPDMA_BSIZE_64 	0x05
/* Burst size = 128 */
#define GPDMA_BSIZE_128 0x06
/* Burst size = 256 */
#define GPDMA_BSIZE_256 0x07

/* Width in Source transfer width and Destination transfer width */
/* Width = 1 byte */
#define GPDMA_WIDTH_BYTE		0x00
/* Width = 2 byte */
#define GPDMA_WIDTH_HALFWORD	0x01
/* Width = 4 byte */
#define GPDMA_WIDTH_WORD		0x02

/* Source peripheral and Destination peripheral */
/* Source peripheral = SSP0TX */
#define GPDMA_PSRC_SSP0TX		0x00
/* Source peripheral = SSP0RX */
#define GPDMA_PSRC_SSP0RX		0x01
/* Source peripheral = SSP1TX */
#define GPDMA_PSRC_SSP1TX		0x02
/* Source peripheral = SSP1RX */
#define GPDMA_PSRC_SSP1RX		0x03
/* Source peripheral = SDMMC */
#define GPDMA_PSRC_SDMMC		0x04
/* Source peripheral = I2S0 */
#define GPDMA_PSRC_I2S0			0x05
/* Source peripheral = I2S1 */
#define GPDMA_PSRC_I2S1			0x06

	
/******************************************************************************/
/*                		Public DMA driver function               		  	  */
/******************************************************************************/

/* Common interrupt service rountine for DMA */
void GPDMA_ISR( void ); 

/* Get current terminal counter if GPDMA */
U32 GPDMA_GetTC (void);

/* Get current error counter if GPDMA */
U32 GPDMA_GetEC (void);

/* Clear current terminal counter of GPDMA */
void GPDMA_ClrTC (void);

/* Clear current error counter of GPDMA */
void GPDMA_ClrEC (void);

/* Init DMA channel controller */
void GPDMA_Init (void);

/* Configure DMA channel controller */
S32 GPDMA_Config (U8 ChannelNum, U8 FlowCtrl, U32 TransferSize, 
		U8 SrcBurstSize, U8 DstBurstSize, U8 SrcWidth, U8 DstWidth, 
		U32 DMASrcAddr, U32 DMADstAddr, U8 SrcPeripheral, U8 DestPeripheral);

#ifdef __cplusplus
}
#endif


#endif /*LPC23XX_DMA_DRIVER_H_*/
