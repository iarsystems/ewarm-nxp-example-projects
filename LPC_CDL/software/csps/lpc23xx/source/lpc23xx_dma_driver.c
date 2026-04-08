/***********************************************************************
* $Id: lpc23xx_dma_driver.c 28 2014-01-27 14:45:13Z danielru $
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

#include "lpc23xx_dma_driver.h"
#include "arm7tdmis_types.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_vic_driver.h"

/******************************************************************************/
/*                		Private DMA driver data								  */
/******************************************************************************/
/* DMA terminal counter */
volatile U32 DMATCCount = 0;
/* DMA error counter */
volatile U32 DMAErrCount = 0;


/******************************************************************************/
/*                		Public DMA driver function               		  	  */
/******************************************************************************/

/**********************************************************************
 * Function: DMA_ISR
 * Purpose: Common interrupt service rountine for DMA
 * Parameter: None
 * Return: None
 *********************************************************************/
void GPDMA_ISR( void )
{
	U32 regVal;

	regVal = GPDMA->INTTCSTATUS;
	if ( regVal )
	{
		DMATCCount++;
		GPDMA->INTTCCLR |= regVal;
	}

	regVal = GPDMA->INTERRORSTATUS;
	if ( regVal )
	{
		DMAErrCount++;
		GPDMA->INTERRCLR |= regVal;
	}
	// update VIC addr
	VIC_Ack ();
}


/**********************************************************************
 * Function: GPDMA_GetTC
 * Purpose: Get current terminal counter of GPDMA
 * Parameter: None
 * Return: Value of terminal counter
 *********************************************************************/
U32 GPDMA_GetTC (void)
{
	return (U32) DMATCCount;
}


/**********************************************************************
 * Function: GPDMA_GetTC
 * Purpose: Get current error counter of GPDMA
 * Parameter: None
 * Return: Value of error counter
 *********************************************************************/
U32 GPDMA_GetEC (void)
{
	return (U32) DMAErrCount;
}


/**********************************************************************
 * Function: GPDMA_ClrTC
 * Purpose: Clear current terminal counter of GPDMA
 * Parameter: None
 * Return: None
 *********************************************************************/
void GPDMA_ClrTC (void)
{
	DMATCCount = 0;
}


/**********************************************************************
 * Function: GPDMA_ClrEC
 * Purpose: Clear current error counter of GPDMA
 * Parameter: None
 * Return: None
 *********************************************************************/
void GPDMA_ClrEC (void)
{
	DMAErrCount = 0;
}


/**********************************************************************
 * Function: GPDMA_Init
 * Purpose: Init DMA channel controller
 * Parameter: None
 * Return: None
 *********************************************************************/
void GPDMA_Init (void)
{
	/* Enable GPDMA clock */
	SCB_EnablePPWR (PCGPDMA);

	/* Clear all DMA interrupt and error flag */
	GPDMA->INTTCCLR = 0x03;
	GPDMA->INTERRCLR = 0x03;

	/* Reset Terminal counter and Error counter */
	DMATCCount = 0;
	DMAErrCount = 0;
}


/**********************************************************************
 * Function: GPDMA_Config
 * Purpose: Configure DMA channel controller
 * Parameter:
 * 		ChannelNum: DMA channel number (must be 0 or 1)
 * 		FlowCtrl:  Flow control number (0..3)
 * 		TransferSize: Size of transfer
 * 		SrcBurstSize: Size of Source Burst
 * 		DstBurstSize: Size of Destination Burst
 * 		SrcWidth: Width of Source transfer
 * 		DstWidth: Width of Destination transfer
 * 		DMASrcAddr: Value of Source Address
 * 		DMADstAddr: Value of Destination Address
 * 		SrcPeripheral: Source of Peripheral
 * 		DestPeripheral: Destination of Peripheral
 * Return:
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 GPDMA_Config (U8 ChannelNum, U8 FlowCtrl, U32 TransferSize,
		U8 SrcBurstSize, U8 DstBurstSize, U8 SrcWidth, U8 DstWidth,
		U32 DMASrcAddr, U32 DMADstAddr, U8 SrcPeripheral, U8 DestPeripheral)
{
	GPDMA_CH_Type *pDMAch;

	/* Point to GPDMA channel */
	if (ChannelNum == 0)
	{
		pDMAch = GPDMAC0;
		GPDMA->INTTCCLR = 0x01;
		GPDMA->INTERRCLR = 0x01;
	}
	else if (ChannelNum == 1)
	{
		pDMAch = GPDMAC1;
		GPDMA->INTTCCLR = 0x02;
		GPDMA->INTERRCLR = 0x02;
	}
	else
	{
		// Bad parameter
		return _ERROR;
	}

	/* Assign Source and Destination memory address */
	pDMAch->SRCADDR = DMASrcAddr;
	pDMAch->DESTADDR = DMADstAddr;
	pDMAch->LLI = 0x00;

	pDMAch->CONTROL = 0x00;
	pDMAch->CONFIGURATION = 0x00;

	/* Set value to Channel Control Registers */
	switch (FlowCtrl)
	{
	case GPDMA_M2M:
		pDMAch->CONTROL |= (TransferSize & 0x0FFF) | ((SrcBurstSize & 0x07) << 12)
							| ((DstBurstSize & 0x07) << 15) | ((SrcWidth & 0x07) << 18)
							| ((DstWidth & 0x07) << 21)| (1 << 26) | (1 << 27) | 0x80000000;
		break;

	case GPDMA_M2P:
		pDMAch->CONTROL |= (TransferSize & 0x0FFF) | ((SrcBurstSize & 0x07) << 12)
							| ((DstBurstSize & 0x07) << 15) | ((SrcWidth & 0x07) << 18)
							| ((DstWidth & 0x07) << 21)| (1 << 26) | 0x80000000;
		break;

	case GPDMA_P2M:
		pDMAch->CONTROL |= (TransferSize & 0x0FFF) | ((SrcBurstSize & 0x07) << 12)
							| ((DstBurstSize & 0x07) << 15) | ((SrcWidth & 0x07) << 18)
							| ((DstWidth & 0x07) << 21) | (1 << 27) | 0x80000000;
		break;

	case GPDMA_P2P:
		pDMAch->CONTROL |= (TransferSize & 0x0FFF) | ((SrcBurstSize & 0x07) << 12)
							| ((DstBurstSize & 0x07) << 15) | ((SrcWidth & 0x07) << 18)
							| ((DstWidth & 0x07) << 21) | 0x80000000;
		break;

	case GPDMA_SP2DP_DP:
		pDMAch->CONTROL |= (TransferSize & 0x0FFF) | ((SrcBurstSize & 0x07) << 12)
							| ((DstBurstSize & 0x07) << 15) | ((SrcWidth & 0x07) << 18)
							| ((DstWidth & 0x07) << 21)| 0x80000000;
		break;

	case GPDMA_M2P_P:
		pDMAch->CONTROL |= (TransferSize & 0x0FFF) | ((SrcBurstSize & 0x07) << 12)
							| ((DstBurstSize & 0x07) << 15) | ((SrcWidth & 0x07) << 18)
							| ((DstWidth & 0x07) << 21)| (1 << 26) | 0x80000000;
		break;

	case GPDMA_P2M_P:
		pDMAch->CONTROL |= (TransferSize & 0x0FFF) | ((SrcBurstSize & 0x07) << 12)
							| ((DstBurstSize & 0x07) << 15) | ((SrcWidth & 0x07) << 18)
							| ((DstWidth & 0x07) << 21) | (1 << 27) | 0x80000000;
		break;

	case GPDMA_SP2DP_SP:
		pDMAch->CONTROL |= (TransferSize & 0x0FFF) | ((SrcBurstSize & 0x07) << 12)
							| ((DstBurstSize & 0x07) << 15) | ((SrcWidth & 0x07) << 18)
							| ((DstWidth & 0x07) << 21) | 0x80000000;
		break;
	/* Bad parameter */
	default:
		return _ERROR;
		break;
	}

	/* Enable DMA channels, little endian */
	GPDMA->CONFIGURATION = 0x01;
	while ( !(GPDMA->CONFIGURATION & 0x01) );

	
#if 1
	pDMAch->CONFIGURATION |= 0xC001 | ((FlowCtrl & 0x07) << 11)
							| ((SrcPeripheral & 0x0F) << 1) | ((DestPeripheral & 0x0F) << 6);
#endif
#if 0	
	if (ChannelNum == 0)
	{
		pDMAch->CONFIGURATION |= 0x1C001 | ((FlowCtrl & 0x07) << 11)
								| ((SrcPeripheral & 0x0F) << 1) | ((DestPeripheral & 0x0F) << 6);
	}
	else
	{
		pDMAch->CONFIGURATION |= 0xC001 | ((FlowCtrl & 0x07) << 11)
										| ((SrcPeripheral & 0x0F) << 1) | ((DestPeripheral & 0x0F) << 6);
	}
#endif
	return _NO_ERROR;
}
