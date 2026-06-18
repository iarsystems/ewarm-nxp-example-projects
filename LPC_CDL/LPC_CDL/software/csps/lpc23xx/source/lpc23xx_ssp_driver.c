/***********************************************************************
* $Id: lpc23xx_ssp_driver.c 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx SSP driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: SSP
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
#include "lpc23xx_ssp_driver.h"
#include "lpc23xx_vic_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_pinsel_driver.h"
#include "lpc23xx_gpio_driver.h"

/* Structure for SSP configuration data  */
typedef struct {
    BOOL32		init;          			/* Device initialized flag */
    SSP_Type 	*regptr;    			/* Pointer to SSP registers */
    void 		(*cbs)(void);			/* Callback function pointer */
    void* 		rdata_transfer;			/* Read data buffer used in async mode */
    void* 		wdata_transfer;			/* Write data buffer used in async mode */
    U32 		rdidx;					/* Index of read data used in async mode */
    U32			wridx;					/* Index of write data used in async mode */
    U32 		length;					/* Lenght of data uaed in async mode */
    U32 		status;					/* Status of current SSP activities */
    BOOL32		dsize;					/* Current data size */
    void		(*handler_int)(S32 SSPdev);	/* Interrupt handler pointer */
 } SSP_CFG_T;

/* SSP device configuration structure data */
STATIC SSP_CFG_T volatile sspdat [2] = {
		{FALSE, (SSP_Type *) 0, NULL, NULL, NULL,
										0, 0, 0, 0, FALSE, NULL},
		{FALSE, (SSP_Type *) 0, NULL, NULL, NULL,
										0, 0, 0, FALSE, FALSE, NULL}
};

/***********************************************************************
 * SSP driver private functions
 **********************************************************************/
/* SSP standard interrupt handler */
void SSP_ISR (void);
/* SSP Read write interrupt handler */
void SSP_IntReadWrite (S32 SSPdev);


/***********************************************************************
 * SSP driver public functions
 **********************************************************************/

/***********************************************************************
 * Function: SSP_SetClock
 * Purpose: Setup clock rate for SSP device 
 * Parameters:
 * 		SSPdev: Pointer to SSP device
 *     	target_clock : clock of SSP (Hz)
 * Return: Status of process (_ERROR or _NO_ERROR)
 ***********************************************************************/
S32 SSP_SetClock (S32 SSPdev, U32 target_clock)
{
    U32 prescale, cr0_div, cmp_clk, ssp_clk;
    SSP_CFG_T *pSSPdev = (SSP_CFG_T *) SSPdev;
    
    /* The SSP clock is derived from the (main system oscillator / 2),
       so compute the best divider from that clock */
    if (SSPdev == (S32)(&sspdat[0]))
    {
    	ssp_clk = SCB_GetPCLK (PCLK_SSP0);
    }
    else if (SSPdev == (S32)(&sspdat[1]))
    {
    	ssp_clk = SCB_GetPCLK (PCLK_SSP1);
    }
    else
    {
    	return _ERROR;
    }

	/* Find closest divider to get at or under the target frequency.
	   Use smallest prescaler possible and rely on the divider to get
	   the closest target frequency */
	cr0_div = 0;
	cmp_clk = 0xFFFFFFFF;
	prescale = 2;
	while (cmp_clk > target_clock)
	{
		cmp_clk = ssp_clk / ((cr0_div + 1) * prescale);
		if (cmp_clk > target_clock)
		{
			cr0_div++;
			if (cr0_div > 0xFF)
			{
				cr0_div = 0;
				prescale += 2;
			}
		}
	}

    /* Write computed prescaler and divider back to register */
    pSSPdev->regptr->CR0 &= ~SSP_CR0_SCR(0xFF);
    pSSPdev->regptr->CR0 |= SSP_CR0_SCR(cr0_div);
    pSSPdev->regptr->CPSR = prescale;

    return _NO_ERROR;
}


/***********************************************************************
 * Function: SSP_Config
 * Purpose: Configure SSP controller
 * Parameters:
 * 		SSPdev: pointer to SSP device
 * 		SSPMode: SSP mode (master or slave)
 * 		FrmMode: Frame data mode (SPI, TI or microwire)
 * 		ClkRate: target clock to set to SSP controller
 * 		Databit: Data bit number
 * 		PhaseMode: first edge or second edge
 * 		ClkMode: high or low active clock level
 * Return: Status of process (_ERROR or _NO_ERROR)
 ***********************************************************************/
S32 SSP_Config (S32 SSPdev, S32 SSPMode, S32 FrmMode, U32 ClkRate, 
		U32 Databit, S32 PhaseMode, S32 ClkMode)
{
	U32 tmp = 0, tmp1 = 0;
	
	SSP_CFG_T *pSSPdev = (SSP_CFG_T *) SSPdev;
	SSP_Type *pSSPreg = pSSPdev->regptr;
	 
	// Check init flag
	if (pSSPdev->init == FALSE)
	{
		return _ERROR;
	}
	
	// Set SSP mode
	if ((SSP_MASTER_OPT)SSPMode == SSP_MASTER)
	{
		// Do nothing
	}
	else if ((SSP_MASTER_OPT)SSPMode == SSP_SLAVE)
	{
		// Set slave mode
		tmp1 |= SSP_CR1_MS;
	}
	else
	{
		return _ERROR;
	}
	
	// Set frame mode
	if ((SSP_FRM_MODE_OPT) FrmMode == SSP_FRM_MODE_SPI)
	{
		// Do nothing
	}
	else if ((SSP_FRM_MODE_OPT) FrmMode == SSP_FRM_MODE_TI)
	{
		tmp |= SSP_CR0_FRF_TI;
	}
	else if ((SSP_FRM_MODE_OPT) FrmMode == SSP_FRM_MODE_MICROWIRE)
	{
		tmp |= SSP_CR0_FRF_NS;
	}
	else
	{
		return _ERROR;
	}
	
	// Set Databit mode
	if ((Databit > 16) || (Databit < 4))
	{
		return _ERROR;
	}
	else
	{
		tmp |= SSP_CR0_DSS(Databit);
		if (Databit > 8)
		{
			pSSPdev->dsize = TRUE;
		}
		else
		{
			pSSPdev->dsize = FALSE;
		}
	}
	
	// Set Phase mode
	if ((SSP_PHASE_OPT) PhaseMode == SSP_FIRST_CLK_EDGE)
	{
		// Do no thing
	}
	else if ((SSP_PHASE_OPT) PhaseMode == SSP_SECOND_CLK_EDGE)
	{
		tmp |= SSP_CR0_CPHA;
	}
	else
	{
		return _ERROR;
	}
	
	// Set active clock mode
	if ((SSP_CLK_LEVEL_OPT)ClkMode == SSP_CLK_LO)
	{
		// Do nothing
	}
	else if (ClkMode == SSP_CLK_HI)
	{
		tmp |= SSP_CR0_CPOL;
	}
	else
	{
		return _ERROR;
	}

	// Set the value to register 
	pSSPreg->CR0 = tmp;
	pSSPreg->CR1 = tmp1;
	
	if (SSP_SetClock (SSPdev, ClkRate) == _ERROR)
	{
		return _ERROR;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: SSP_Init
 * Purpose: Init SSP controller with parameter given
 * Parameters:
 * 		SSPNum: SSP control number (must be 0 or 1)
 * 		SSPMode: SSP mode (master or slave)
 * 		FrmMode: Frame data mode (SPI, TI or microwire)
 * 		ClkRate: target clock to set to SSP controller
 * 		Databit: Data bit number
 * 		PhaseMode: first edge or second edge
 * 		ClkMode: high or low active clock level
 * Return: Pointer to SSP device of success or 0 if error
 ***********************************************************************/
S32 SSP_Init (S32 SSPNum, S32 SSPMode, S32 FrmMode, U32 ClkRate, 
		U32 Databit, S32 PhaseMode, S32 ClkMode)
{
	SSP_CFG_T *pSSPdev;
	U32 tmp;
	
	if (SSPNum == 0)
	{
		pSSPdev = (SSP_CFG_T *) &sspdat[0];
	}
	else if (SSPNum == 1)
	{
		pSSPdev = (SSP_CFG_T *) &sspdat[1];
	}
	else
	{
		return 0;
	}

	/* Check if its init status is FALSE or TRUE */
	if(pSSPdev->init == FALSE)
	{
		pSSPdev->init = TRUE;
		
		pSSPdev->status = SSP_STAT_BUSY;
		
		/* Enable power, set clock divider and 
		 * Store pointer to SSP register address */
		if (SSPNum == 0)
		{
			SCB_EnablePPWR (PCSSP0);
			SCB_SetPCLKDiv (PCLK_SSP0, CCLK_DIV_4);
			pSSPdev->regptr = SSP0;
		}
		else if (SSPNum == 1)
		{
			SCB_EnablePPWR (PCSSP1);
			SCB_SetPCLKDiv (PCLK_SSP1, CCLK_DIV_4);
			pSSPdev->regptr = SSP1;
		}
		
		/* No initial callbacks */
		pSSPdev->cbs = NULL;
		/* No handler for interrupt */
		pSSPdev->handler_int = NULL;
		/* No read/write data pointer */
		pSSPdev->rdata_transfer = NULL;
		pSSPdev->wdata_transfer = NULL;
		pSSPdev->dsize = FALSE;
		pSSPdev->rdidx = 0;
		pSSPdev->wridx = 0;
		pSSPdev->length = 0;
		
		/* Configure SSP controller with paremeter given */
		if ((SSP_Config ((S32)pSSPdev, SSPMode, FrmMode, ClkRate, 
				Databit, PhaseMode, ClkMode)) == _ERROR)
		{
			return 0;
		}
 
		/* Empty RX FIFO */
		while (pSSPdev->regptr->SR & SSP_SR_RNE)
		{
			tmp = pSSPdev->regptr->DR;
		}

		/* Clear latched interrupts */
		pSSPdev->regptr->ICR = (SSP_IIR_RORRIS | SSP_IIR_RTRIS);

		/* Disable all interrupts */
		pSSPdev->regptr->IMSC = 0;

		/* Enable device */
		pSSPdev->regptr->CR1 |= SSP_CR1_SSE;
		
		pSSPdev->status = SSP_STAT_IDLE;
	}
	else
	{
		// Return ERROR if it is already initilized
		return 0;
	}

	// return current address of SSP device
	 return (S32)pSSPdev;
}


/***********************************************************************
 * Function: SSP_Close
 * Purpose: Close the SSP device
 * Parameters:
 * 		SSPdev: Pointer to SSP device
 * Return: Status of process (_ERROR or _NO_ERROR)
 ***********************************************************************/
S32 SSP_Close (S32 SSPdev)
{
    SSP_CFG_T *pSSPdev = (SSP_CFG_T *) SSPdev;

    if (pSSPdev->init == TRUE)
    {
    	// Turn off power for SSP controller
        if (SSPdev == (S32)(&sspdat[0]))
        {
        	SCB_DisablePPWR (PCSSP0);
        }
        else if (SSPdev == (S32)(&sspdat[1]))
        {
        	SCB_DisablePPWR (PCSSP1);
        }
        else
        {
        	return _ERROR;
        }
    	
        /* 'Uninitialize' device */
    	pSSPdev->init = FALSE;
    	/* Disable SSP activities */
    	pSSPdev->regptr->CR0 = 0;
    	pSSPdev->regptr = 0;
    }
    else 
    {
    	return _ERROR;
    }

    return _NO_ERROR;
}


/***********************************************************************
 * Function: SSP_ReadWritePoll
 * Purpose: SSP Read write in polling mode function 
 * Parameters:
 * 		SSPdev: Pointer to SSP device
 * 		rbuffer: pointer to read buffer
 * 		wbuffer: pointer to write buffer
 * 		length: length of data to read and write
 * Return: length of actual data transferred
 ***********************************************************************/
S32 SSP_ReadWritePoll (S32 SSPdev,
	                 void *rbuffer,
	                 void *wbuffer,
	                 U32 length)
{
	SSP_CFG_T *pSSPdev = (SSP_CFG_T *) SSPdev;
	U16 *rddata16 = (U16 *) rbuffer;
    U8 *rddata8 = (U8 *) rbuffer;
    U16 *wrdata16 = (U16 *) wbuffer;
    U8 *wrdata8 = (U8 *) wbuffer;
    U32 rdcnt = 0;
    U32 wrcnt = 0;
    U32 tmp = 0;
    
    if (pSSPdev->init == FALSE)
    {
    	return 0;
    }
    
	/* Clear all remaining data in RX FIFO */
	while (pSSPdev->regptr->SR & SSP_SR_RNE)
	{
		tmp = pSSPdev->regptr->DR;
	}
	
	/* Loop until wrcnt expires */
	while (wrcnt < length)
	{
		/* Check if TX FIFO is not full */
		if (pSSPdev->regptr->SR & SSP_SR_TNF)
		{
			if (wbuffer != NULL)
			{
				if (pSSPdev->dsize == FALSE)
	        	{
	    			pSSPdev->regptr->DR = (U32) (*wrdata8);
	        		wrdata8++;
	        		wrcnt++;
	        	}
	        	else
	        	{
	        		pSSPdev->regptr->DR = (U32) (*wrdata16);
	        		wrdata16++;
	        		wrcnt += 2;
	        	}
			}
			else
			{
				pSSPdev->regptr->DR = 0x00;
				if (pSSPdev->dsize == FALSE)
				{
					wrcnt++;
				}
				else
				{
					wrcnt += 2;
				}
			}

		}

		/* check if RX FIFO contains data */
		while (pSSPdev->regptr->SR & SSP_SR_RNE)
		{
			tmp = pSSPdev->regptr->DR;
			if (rbuffer != NULL)
			{
	        	if (pSSPdev->dsize == FALSE)
	        	{
	        		*rddata8 = (U8) tmp;
	        		rddata8++;
	        		rdcnt++;
	        	}
	        	else
	        	{
	        		*rddata16 = (U16) tmp;
	        		rddata16++;
	        		rdcnt += 2;
	        	}
			}
			else
			{
				if (pSSPdev->dsize == FALSE)
				{
					rdcnt++;
				}
				else
				{
					rdcnt += 2;
				}
			}
		}
	}

	/* Loop until rdcnt expires */
	while (rdcnt < length)
	{
		if (pSSPdev->regptr->SR & SSP_SR_RNE)
		{
			tmp = pSSPdev->regptr->DR;
			if (rbuffer != NULL)
			{
				if (pSSPdev->dsize == FALSE)
	        	{
	        		*rddata8 = (U8) tmp;
	        		rddata8++;
	        		rdcnt++;
	        	}
	        	else
	        	{
	        		*rddata16 = (U16) tmp;
	        		rddata16++;
	        		rdcnt += 2;
	        	}
			}
			else
			{
				if (pSSPdev->dsize == FALSE)
				{
					rdcnt++;
				}
				else
				{
					rdcnt += 2;
				}
			}
		}
	}
	
	return (S32) wrcnt;
}


/***********************************************************************
 * Function: SSP_ReadWriteAsync
 * Purpose: SSP Read write in asynchronous (interrupt) mode function 
 * Parameters:
 * 		SSPdev: Pointer to SSP device
 * 		rbuffer: pointer to read buffer
 * 		wbuffer: pointer to write buffer
 * 		length: length of data to read and write
 * Return:
 * 		Number of data byte has been tranferring in present
 ***********************************************************************/
S32 SSP_ReadWriteAsync (S32 SSPdev,
				    void *rbuffer,
				    void *wbuffer,
				    U32 length)
{
	SSP_CFG_T *pSSPdev = (SSP_CFG_T *) SSPdev;
	U16 *rddata16 = (U16 *) rbuffer;
    U8 *rddata8 = (U8 *) rbuffer;
    U16 *wrdata16 = (U16 *) wbuffer;
    U8 *wrdata8 = (U8 *) wbuffer;
    U32 tmp;
    
    if (pSSPdev->init == FALSE)
    {
    	return _ERROR;
    }

	// Set current status for SSP operation
	pSSPdev->status = SSP_STAT_BUSY;

	/* Clear all remaining data in RX FIFO */
	while (pSSPdev->regptr->SR & SSP_SR_RNE)
	{
		tmp = pSSPdev->regptr->DR;
	}
	
	/* Save buffer data addr */
	pSSPdev->rdata_transfer = rbuffer;
	pSSPdev->wdata_transfer = wbuffer;
	pSSPdev->rdidx = 0;
	pSSPdev->wridx = 0;
	pSSPdev->length = length;
 
	/* Check if TX FIFO is not full */
	while ((pSSPdev->regptr->SR & SSP_SR_TNF)
			&& (pSSPdev->wridx < pSSPdev->length))
	{
		if (pSSPdev->wdata_transfer != NULL)
		{
			if (pSSPdev->dsize == FALSE)
        	{
        		pSSPdev->regptr->DR = (U32) *(wrdata8 + pSSPdev->wridx);
        		pSSPdev->wridx++;
        	}
        	else
        	{
        		pSSPdev->regptr->DR = (U32) *(wrdata16 + pSSPdev->wridx);
        		pSSPdev->wridx += 2;
        	}
		}
		else
		{
			if (pSSPdev->dsize == FALSE)
			{
				pSSPdev->wridx++;
			}
			else
			{
				pSSPdev->wridx += 2;
			}
		}

		/* check if RX FIFO contains data */
		while (pSSPdev->regptr->SR & SSP_SR_RNE)
		{
			tmp = pSSPdev->regptr->DR;
			if (pSSPdev->rdata_transfer != NULL)
			{
	        	if (pSSPdev->dsize == FALSE)
	        	{
	        		*(rddata8 + pSSPdev->rdidx) = (U8) tmp;
	        		pSSPdev->rdidx++;
	        	}
	        	else
	        	{
	        		*(rddata16 + pSSPdev->rdidx) = (U16) tmp;
	        		pSSPdev->rdidx += 2;
	        	}
			}
			else
			{
				if (pSSPdev->dsize == FALSE)
				{
					pSSPdev->rdidx++;
				}
				else
				{
					pSSPdev->rdidx += 2;
				}
			}
		}
	}

	/* If there are more data need to be sent */
	if (pSSPdev->wridx < pSSPdev->length)
	{
    	/* Clear all interrupt */
    	pSSPdev->regptr->ICR = (SSP_IIR_RORRIS | SSP_IIR_RTRIS);

		/* Check if slave mode is activated */
		if (pSSPdev->regptr->CR1 & SSP_CR1_MS)
		{
			/* enable interrupt bit for RX FIFO overflow
			 * and interrupt bit for RX FIFO not empty and has a data timeout
			 * and interrupt bit for RX FIFO half full */
			pSSPdev->regptr->IMSC = (SSP_IIR_RORRIS | SSP_IIR_RTRIS
											| SSP_IIR_RXRIS);
		}
		/* Check if master mode is activated */
		else
		{
			/* Enable SSP_IIR_TXRIS */
			pSSPdev->regptr->IMSC = SSP_IIR_TXRIS;
		}

		// Set interrupt handler for next SSP interrupt
		pSSPdev->handler_int = SSP_IntReadWrite;
		/* enable interrupt for next transmission */
		if (SSPdev == (S32) (&sspdat[0]))
		{
			VIC_EnableInt (SPI0_INT);
		}
		else if (SSPdev == (S32) (&sspdat[1]))
		{
			VIC_EnableInt (SPI1_INT);
		}
		
	}
	else
	{
    	/* Wait for current transmission completed */
		while (pSSPdev->rdidx < pSSPdev->length)
		{
			/* Read all remain data in RX FIFO */
			if (pSSPdev->regptr->SR & SSP_SR_RNE)
			{
				tmp = pSSPdev->regptr->DR;
				if (pSSPdev->rdata_transfer != NULL)
				{
		        	if (pSSPdev->dsize == FALSE)
		        	{
		        		*(rddata8 + pSSPdev->rdidx) = (U8) tmp;
		        		pSSPdev->rdidx++;
		        	}
		        	else
		        	{
		        		*(rddata16 + pSSPdev->rdidx) = (U16) tmp;
		        		pSSPdev->rdidx += 2;
		        	}
				}
				else
				{
					if (pSSPdev->dsize == FALSE)
					{
						pSSPdev->rdidx++;
					}
					else
					{
						pSSPdev->rdidx += 2;
					}
				}
			}
		}
    	/* Handle call back function */
		if (pSSPdev->cbs != NULL)
		{
			pSSPdev->cbs();
		}
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: SSP_IntReadWrite
 * Purpose: SSP Interrupt read and write handler
 * Parameters: None
 * Return: None
 ***********************************************************************/
void SSP_IntReadWrite (S32 SSPdev)
{
	SSP_CFG_T *pSSPdev = (SSP_CFG_T *) SSPdev;
	
	U16 *rddata16;
    U8 *rddata8;
    U16 *wrdata16;
    U8 *wrdata8;
    U32 tmp;

	rddata16 = (U16 *) pSSPdev->rdata_transfer;
    rddata8 = (U8 *) pSSPdev->rdata_transfer;
    wrdata16 = (U16 *) pSSPdev->wdata_transfer;
    wrdata8 = (U8 *) pSSPdev->wdata_transfer;

	/* Check if TX FIFO is not full */
	while ((pSSPdev->regptr->SR & SSP_SR_TNF)
			&& (pSSPdev->wridx < pSSPdev->length))
	{
		/* check if RX FIFO contains data */
		while (pSSPdev->regptr->SR & SSP_SR_RNE)
		{
			tmp = pSSPdev->regptr->DR;
			if (pSSPdev->rdata_transfer != NULL)
			{
	        	if (pSSPdev->dsize == FALSE)
	        	{
	        		*(rddata8 + pSSPdev->rdidx) = (U8) tmp;
	        		pSSPdev->rdidx++;
	        	}
	        	else
	        	{
	        		*(rddata16 + pSSPdev->rdidx) = (U16) tmp;
	        		pSSPdev->rdidx += 2;
	        	}
			}
			else
			{
				if (pSSPdev->dsize == FALSE)
				{
					pSSPdev->rdidx++;
				}
				else
				{
					pSSPdev->rdidx += 2;
				}
			}
		}

		if (pSSPdev->wdata_transfer != NULL)
		{
			if (pSSPdev->dsize == FALSE)
	    	{
	    		pSSPdev->regptr->DR = (U32) (*(wrdata8 + pSSPdev->wridx));
	    		pSSPdev->wridx++;
	    	}
	    	else
	    	{
	    		pSSPdev->regptr->DR = (U32) (*(wrdata16 + pSSPdev->wridx));
	    		pSSPdev->wridx += 2;
	    	}
		}
		else
		{
			pSSPdev->regptr->DR = 0x00;
			if (pSSPdev->dsize == FALSE)
			{
				pSSPdev->wridx++;
			}
			else
			{
				pSSPdev->wridx += 2;
			}
		}
	}

	/* If there are more data need to be sent */
	if (pSSPdev->wridx < pSSPdev->length)
	{
    	/* Clear all interrupt */
    	pSSPdev->regptr->ICR = (SSP_IIR_RORRIS | SSP_IIR_RTRIS);
	}
	else
	{
		/* Disable interrupt bit for RX FIFO overflow
		 * and interrupt bit for RX FIFO not empty and has a data timeout
		 * and interrupt bit for RX FIFO half full
		 * and interrupt bit for TX FIFO half empty */
		pSSPdev->regptr->IMSC = 0;

		/* Disable interrupt */
		if (SSPdev == (S32) (&sspdat[0]))
		{
			VIC_DisableInt (SPI0_INT);
		}
		else if (SSPdev == (S32) (&sspdat[1]))
		{
			VIC_DisableInt (SPI1_INT);
		}

		/* Wait for current transmission completed */
		while (pSSPdev->rdidx < pSSPdev->length)
		{
			/* Read all remain data in RX FIFO */
			if (pSSPdev->regptr->SR & SSP_SR_RNE)
			{
				tmp = pSSPdev->regptr->DR;
				if (pSSPdev->rdata_transfer != NULL)
				{
		        	if (pSSPdev->dsize == FALSE)
		        	{
		        		*(rddata8 + pSSPdev->rdidx) = (U8) tmp;
		        		pSSPdev->rdidx++;
		        	}
		        	else
		        	{
		        		*(rddata16 + pSSPdev->rdidx) = (U16) tmp;
		        		pSSPdev->rdidx += 2;
		        	}
				}
				else
				{
					if (pSSPdev->dsize == FALSE)
					{
						pSSPdev->rdidx++;
					}
					else
					{
						pSSPdev->rdidx += 2;
					}
				}
			}
		}
    	/* Handle call back function */
		if (pSSPdev->cbs != NULL)
		{
			pSSPdev->cbs();
		}
	}

	/* Update VIC addr */
	VIC_Ack();
}


/***********************************************************************
 * Function: SSP_SetCBS
 * Purpose: Set call back function for SSP in ASYNC Read Write mode
 * Parameters: 
 * 		SSPdev: Pointer to SSP device
 * 		pCBS: Pointer to call back function
 * Return: Status of progress (_ERROR or _NO_ERROR)
 ***********************************************************************/
S32 SSP_SetCBS (S32 SSPdev, void *pCBS)
{
	SSP_CFG_T *pSSPdev = (SSP_CFG_T *) SSPdev;
	
	if (pSSPdev->init == FALSE)
	{
		return _ERROR;
	}

	pSSPdev->cbs = (void *) pCBS;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: SSP_GetStatus
 * Purpose: Get status of current SSP operation
 * Parameters: 
 * 		SSPdev: Pointer to SSP device
 * Return: Value of SSP operation status
 ***********************************************************************/
U32 SSP_GetStatus (S32 SSPdev)
{
	SSP_CFG_T *pSSPdev = (SSP_CFG_T *) SSPdev;
	
	if (pSSPdev->init == FALSE)
	{
		return 0;
	}
	
	return pSSPdev->status;
}


/***********************************************************************
 * Function: SSP_EnableDMA
 * Purpose: Enable DMA control on SSP device
 * Parameters: 
 * 		SSPdev: Pointer to SSP device
 * 		DMAtype: type of DMA on SSP (Rx or Tx)
 * Return: Status of progress (_ERROR or _NO_ERROR)
 ***********************************************************************/
S32 SSP_EnableDMA (S32 SSPdev, U32 DMAtype)
{
	SSP_CFG_T *pSSPdev = (SSP_CFG_T *) SSPdev;
	
	if (pSSPdev->init == FALSE)
	{
		return 0;
	}
	
	pSSPdev->regptr->DMACR |= DMAtype;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: SSP_DisableDMA
 * Purpose: Disable DMA control on SSP device
 * Parameters: 
 * 		SSPdev: Pointer to SSP device
 * 		DMAtype: type of DMA on SSP (Rx or Tx)
 * Return: Status of progress (_ERROR or _NO_ERROR)
 ***********************************************************************/
S32 SSP_DisableDMA (S32 SSPdev, U32 DMAtype)
{
	SSP_CFG_T *pSSPdev = (SSP_CFG_T *) SSPdev;
	
	if (pSSPdev->init == FALSE)
	{
		return 0;
	}
	
	pSSPdev->regptr->DMACR &= ~DMAtype;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: SSP_ISR
 * Purpose: Standard SSP interrupt handler function
 * Parameters: None
 * Return: None
 ***********************************************************************/
void SSP0_ISR (void)
{
	sspdat[0].handler_int((S32) (&sspdat[0]));
	
	// Update VIC addr
	VIC_Ack();
}

/***********************************************************************
 * Function: SSP_ISR
 * Purpose: Standard SSP interrupt handler function
 * Parameters: None
 * Return: None
 ***********************************************************************/
void SSP1_ISR (void)
{
	sspdat[1].handler_int((S32) (&sspdat[1]));
	
	// Update VIC addr
	VIC_Ack();
}
