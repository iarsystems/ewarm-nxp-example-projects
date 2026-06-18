/***********************************************************************
* $Id: lpc23xx_dma.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx GPDMA definition
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
#ifndef LPC23XX_DMA_H_
#define LPC23XX_DMA_H_

#include "lpc23xx.h"
#include "arm7tdmis_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                			GPDMA General Structure              		  	  */
/******************************************************************************/
typedef struct {
	__IO U32 INTSTATUS;				/* Interrupt Status Register */
	__IO U32 INTTCSTATUS;			/* Interrupt Terminal Count Status Register */
	__IO U32 INTTCCLR;				/* Interrupt Terminal Count Clear Register */
	__IO U32 INTERRORSTATUS;		/* Interrupt Error Status Register */
	__IO U32 INTERRCLR;				/* Interrupt Error Clear Register */
	__IO U32 RAWINTTCSTATUS;		/* Raw Interrupt Terminal Count Status Register */
	__IO U32 RAWINTERRORSTATUS;		/* Raw Error Interrupt Status Register */
	__IO U32 ENBLDCHNS;				/* Enabled Channel Register */
	__IO U32 SOFTBREQ;				/* Software Burst Request Register */
	__IO U32 SOFTSREQ;				/* Software Single Request Register */
	__IO U32 SOFTLBREQ;				/* Software Last Burst Request Register */
	__IO U32 SOFTLSREQ;				/* Software Last Single Request Register */
	__IO U32 CONFIGURATION;			/* Configuration Register */
	__IO U32 SYNC;					/* Synchronization Register */
} GPDMA_GEN_Type;

/******************************************************************************/
/*                			GPDMA Channel Structure              		  	  */
/******************************************************************************/
typedef struct {
	__IO U32 SRCADDR;			/* Channel Source Address Register */
	__IO U32 DESTADDR;			/* Channel Destination Address Register */
	__IO U32 LLI;				/* Channel Linked List Item Register */
	__IO U32 CONTROL;			/* Channel Control Register */
	__IO U32 CONFIGURATION;		/* Channel Configuration Register */
} GPDMA_CH_Type;

/* Macro pointing to DMA structure at DMA base address */
#define GPDMA		((GPDMA_GEN_Type *) GPDMA_BASE_ADDR)
/* Macro pointing to DMA Channel structure at DMA Channel 0 base address */
#define GPDMAC0	((GPDMA_CH_Type *) GPDMA_CH0_BASE_ADDR)
/* Macro pointing to DMA Channel structure at DMA Channel 1 base address */
#define GPDMAC1	((GPDMA_CH_Type *) GPDMA_CH1_BASE_ADDR)


#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_DMA_H_*/
