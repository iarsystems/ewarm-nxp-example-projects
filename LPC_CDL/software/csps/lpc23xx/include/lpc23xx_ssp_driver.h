/***********************************************************************
 * $Id: lpc23xx_ssp_driver.h 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx SSP driver
 *
 * Description:
 *     This file contains driver support for the SSP module on the
 *     LPC23xx
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
#ifndef LPC23XX_SSP_DRIVER_H_
#define LPC23XX_SSP_DRIVER_H_

#include "lpc23xx_ssp.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * SSP driver private data and types
 **********************************************************************/
/* Clock edge option */
typedef enum
{
	SSP_FIRST_CLK_EDGE = 0,		/* Use first clock edge */
	SSP_SECOND_CLK_EDGE			/* Use second clock edge */
} SSP_PHASE_OPT;

/* Clock level option */
typedef enum
{
	SSP_CLK_HI = 0,		/* Use active high clock */
	SSP_CLK_LO			/* Use active low clock */
} SSP_CLK_LEVEL_OPT;

/* Master control option */
typedef enum
{
	SSP_MASTER = 0,		/* Use SSP in master mode */
	SSP_SLAVE			/* Use SSP in slave mode */
} SSP_MASTER_OPT;

/* Frame mode option */
typedef enum
{
	SSP_FRM_MODE_SPI = 0,	/* SPI mode of Motorolla */
	SSP_FRM_MODE_TI, 		/* TI mode */
	SSP_FRM_MODE_MICROWIRE  /* Microwire mode */
} SSP_FRM_MODE_OPT;

/* Macro for return status value in SSP operation 
 * when using SSP_GetStatus () */
/* SSP in idle state */
#define SSP_STAT_IDLE		0x00
/* SSP busy */
#define SSP_STAT_BUSY		((U32) (1 << 1))

/***********************************************************************
 * Public SSP driver functions
 **********************************************************************/

/* Init SSP device */
S32 SSP_Init (S32 SSPNum, S32 SSPMode, S32 FrmMode, U32 ClkRate, 
		U32 Databit, S32 PhaseMode, S32 ClkMode);

/* Close the SSP device */
S32 SSP_Close (S32 SSPdev);

/* Configure SSP controller */
S32 SSP_Config (S32 SSPdev, S32 SSPMode, S32 FrmMode, U32 ClkRate, 
		U32 Databit, S32 PhaseMode, S32 ClkMode);

/* Setup clock rate for SSP device */
S32 SSP_SetClock (S32 SSPdev, U32 target_clock);

/* SSP Read and write in polling mode function */
S32 SSP_ReadWritePoll (S32 SSPdev,
	                 void *rbuffer,
	                 void *wbuffer,
	                 U32 length);

/* SSP Read and write in interrupt mode function */
S32 SSP_ReadWriteAsync (S32 SSPdev,
				    void *rbuffer,
				    void *wbuffer,
				    U32 length);

/* Set call back function for SSP in ASYNC Read Write mode */
S32 SSP_SetCBS (S32 SSPdev, void *pCBS);

/* Get status of current SSP operation */
U32 SSP_GetStatus (S32 SSPdev);

/* Enable DMA control on SSP device */
S32 SSP_EnableDMA (S32 SSPdev, U32 DMAtype);

/* Disable DMA control on SSP device */
S32 SSP_DisableDMA (S32 SSPdev, U32 DMAtype);

/* Common SSP0 interrupt handler */
void SSP0_ISR (void);

/* Common SSP1 interrupt handler */
void SSP1_ISR (void);

#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_SSP_DRIVER_H_*/
