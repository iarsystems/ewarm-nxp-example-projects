/***********************************************************************
* $Id: lpc23xx_spi_driver.c 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx  SPI driver
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

#include "lpc23xx_spi_driver.h"
#include "lpc23xx_vic_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_pinsel_driver.h"
#include "lpc23xx_gpio_driver.h"

/* Structure for SPI configuration data */
typedef struct {
    BOOL32		init;          			/* Device initialized flag */
    SPI_Type 	*regptr;    			/* Pointer to SPI registers */
    void 		(*cbs)(void);			/* Callback function pointer */
    void* 		rdata_transfer;			/* Read data buffer used in async mode */
    void* 		wdata_transfer;			/* Write data buffer used in async mode */
    U32 		index;					/* Index of data used in async mode */
    U32 		length;					/* Lenght of data uaed in async mode */
    U32 		status;					/* Status of current SPI activities */
    BOOL32		dsize;					/* Current data size */
    void		(*handler_int)(S32 SPIdev);	/* Interrupt handler pointer */
 } SPI_DAT_Type;
 
/* Global SPI data */
STATIC SPI_DAT_Type volatile spidrvdat = {
		FALSE, 
		(SPI_Type *) 0, 
		NULL, 
		NULL, 
		NULL,
		0, 0, 0, 
		FALSE,
		NULL
};


/***********************************************************************
 * SPI driver private functions
 **********************************************************************/
/* SPI standard interrupt handler */
void SPI_ISR (void);
/* SPI Read write interrupt handler */
void SPI_IntReadWrite (S32 SPIdev);


/***********************************************************************
 * SPI driver public functions
 **********************************************************************/

/***********************************************************************
 * Function: SPI_SetClock
 * Purpose: Setup clock rate for SPI device 
 * Parameters:
 * 		SPIdev: Pointer to SPI device
 *     	target_clock : clock of SPI (Hz)
 * Return: Status of process (_ERROR or _NO_ERROR)
 ***********************************************************************/
S32 SPI_SetClock (S32 SPIdev, U32 target_clock)
{
	U32 spi_pclk;
	U32 prescale, temp = 0;
	
	SPI_DAT_Type *pspidat = (SPI_DAT_Type *) SPIdev;
	SPI_Type *pspireg = pspidat->regptr;
	
	if (pspidat->init == FALSE)
	{
		return _ERROR;
	}
	
	spi_pclk =  SCB_GetPCLK (PCLK_SPI);
	prescale = 8;
	
	while (temp < spi_pclk)
	{
		prescale++;
		if(prescale > 255)
		{
			break;
		}

		temp = target_clock * prescale;
	}
	
	if ((prescale < 8) && (target_clock > spi_pclk))
	{
		return _ERROR;
	}

	pspireg->SPCCR = prescale;

	return _NO_ERROR;
}


/***********************************************************************
 * Function: SPI_Config
 * Purpose: Configure SPI controller
 * Parameters:
 * 		SPIdev: pointer to SPI device
 * 		Mode: SPI mode (master or slave)
 * 		ClkRate: target clock to set to SPI controller
 * 		Databit: Data bit number
 * 		PhaseMode: first edge or second edge
 * 		ClkMode: high or low active clock
 * 		DataOrder: MSB or LSB
 * Return: Status of process (_ERROR or _NO_ERROR)
 ***********************************************************************/
S32 SPI_Config (S32 SPIdev, U32 Mode, U32 ClkRate, U32 Databit, 
		U32 PhaseMode, U32 ClkMode, U32 DataOrder)
{
	U32 tmp;
	
	SPI_DAT_Type *pspidat = (SPI_DAT_Type *) SPIdev;
	SPI_Type *pspireg = pspidat->regptr;
	 
	// Check init flag
	if (pspidat->init == FALSE)
	{
		return _ERROR;
	}

	tmp = 0;
	
	// Set SPI mode
	if (Mode == SPI_MASTER)
	{
		// Set master mode 
		tmp |= SPI_MASTER_BIT;
		// Set SS pin as GPIO function due to SPI master
		PINSEL_Set (PORT_0, SPI_PIN_FUNC_SSEL, FUNC_0);
		GPIO_SetDirOutput (GPIO_0, (1 << SPI_PIN_FUNC_SSEL));
		FIO_SetDirOutput32 (FIO_0, (1 << SPI_PIN_FUNC_SSEL));
		GPIO_Set (GPIO_0, (1 << SPI_PIN_FUNC_SSEL));
		FIO_Set32 (FIO_0, (1 << SPI_PIN_FUNC_SSEL));
	}
	else if (Mode == SPI_SLAVE)
	{
		// Set SS pin as SPI function pin due to SPI slave
		PINSEL_Set (PORT_0, SPI_PIN_FUNC_SSEL, FUNC_3);
		// Set slave mode - Do nothing 
	}
	else
	{
		return _ERROR;
	}
	
	// Set Databit mode
	if (Databit == SPI_DATABIT_8)
	{
		pspidat->dsize = FALSE;
	}
	else if (((Databit > SPI_DATABIT_8) && (Databit <= SPI_DATABIT_15)) 
			|| (Databit == SPI_DATABIT_16))
	{
		tmp |= SPI_ENABLE_BIT;
		tmp |= SPI_BITS(Databit);
		pspidat->dsize = TRUE;
	}
	else
	{
		return _ERROR;
	}
	
	// Set Phase mode
	if (PhaseMode == SPI_FIRST_CLK_EDGE)
	{
		// Do no thing
	}
	else if (PhaseMode == SPI_SECOND_CLK_EDGE)
	{
		tmp |= SPI_CPHA_BIT;
	}
	else
	{
		return _ERROR;
	}
	
	// Set active clock mode
	if (ClkMode == SPI_CLK_HI)
	{
		// Do nothing
	}
	else if (ClkMode == SPI_CLK_LO)
	{
		tmp |= SPI_CPOL_BIT;
	}
	else
	{
		return _ERROR;
	}
	
	// Set Data order for SPI
	if (DataOrder == SPI_ORDER_MSB)
	{
		// Do no thing
	}
	else if (DataOrder == SPI_ORDER_MSB)
	{
		tmp |= SPI_LSBF_BIT;
	}
	else
	{
		return _ERROR;
	}

	// Set the value to register 
	pspireg->SPCR = tmp;
	
	if (SPI_SetClock (SPIdev, ClkRate) == _ERROR)
	{
		return _ERROR;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: SPI_Init
 * Purpose: Init SPI controller with parameter given
 * Parameters:
 * 		SPINum: SPI control number (must be 0 for this type of chip)
 * 		Mode: SPI mode (master or slave)
 * 		ClkRate: target clock to set to SPI controller
 * 		Databit: Data bit number
 * 		PhaseMode: first edge or second edge
 * 		ClkMode: high or low active clock
 * 		DataOrder: MSB or LSB
 * Return: Status of process (_ERROR or _NO_ERROR)
 ***********************************************************************/
S32 SPI_Init (S32 SPINum, U32 Mode, U32 ClkRate, U32 Databit, 
			U32 PhaseMode, U32 ClkMode, U32 DataOrder)
{
	SPI_DAT_Type *spidrvdatptr;

	if (SPINum == 0)
	{
		spidrvdatptr = (SPI_DAT_Type *) &spidrvdat;
	}
	else
	{
		return 0;
	}

	/* Check if its init status is FALSE or TRUE */
	if(spidrvdatptr->init == FALSE)
	{
		spidrvdatptr->init = TRUE;
		
		spidrvdatptr->status = SPI_STAT_BUSY;
		
		/* Set these function pins for SPI peripheral except of SS pin
		 * that will be configured when set SPI mode (master or slave) */
		PINSEL_Set (PORT_0, SPI_PIN_FUNC_SCK, FUNC_3);
		PINSEL_Set (PORT_0, SPI_PIN_FUNC_MISO, FUNC_3);
		PINSEL_Set (PORT_0, SPI_PIN_FUNC_MOSI, FUNC_3);
		
		/* Enable power for SPI controller */
		 SCB_EnablePPWR (PCSPI);
		 
		 /* Store pointer to SPI register address */
		 if (SPINum == 0)
		 {
			 spidrvdatptr->regptr = SPI0;
		 }
		 /* Configure SPI controller with paremeter given */
		 if (SPI_Config ((S32) &spidrvdat, Mode, ClkRate, Databit, 
				 PhaseMode, ClkMode, DataOrder) == _ERROR)
		 {
			 return _ERROR;
		 }
		 
		 spidrvdatptr->status = SPI_STAT_IDLE;
	}
	else
	{
		// Return ERROR if it is already initilized
		return _ERROR;
	}

	// return current address of SPI device
	 return (S32)spidrvdatptr;
}


/***********************************************************************
 * Function: SPI_Close
 * Purpose: Close the SPI device
 * Parameters:
 * 		SPIdev: Pointer to SPI device
 * Return: Status of process (_ERROR or _NO_ERROR)
 ***********************************************************************/
S32 SPI_Close (S32 SPIdev)
{
    SPI_DAT_Type *pspidrvdat = (SPI_DAT_Type *) SPIdev;

    if (pspidrvdat->init == TRUE)
    {
    	// Turn off power for SPI controller
    	SCB_DisablePPWR (PCSPI);
    	
        /* 'Uninitialize' device */
    	pspidrvdat->init = FALSE;
    	/* Disable SPI activities */
    	pspidrvdat->regptr->SPCR = 0;
    }
    else 
    {
    	return _ERROR;
    }

    return _NO_ERROR;
}


/***********************************************************************
 * Function: SPI_ReadWritePoll
 * Purpose: SPI Read write in polling mode function 
 * Parameters:
 * 		SPIdev: Pointer to SPI device
 * 		rbuffer: pointer to read buffer
 * 		wbuffer: pointer to write buffer
 * 		length: length of data to read and write
 * Return: length of actual data transferred
 ***********************************************************************/
S32 SPI_ReadWritePoll (S32 SPIdev,
	                 void *rbuffer,
	                 void *wbuffer,
	                 U32 length)
{
	SPI_DAT_Type *spidrvdatptr = (SPI_DAT_Type *) SPIdev;
	U16 *rdata16 = (U16 *) rbuffer;
    U8 *rdata8 = (U8 *) rbuffer;
    U16 *wdata16 = (U16 *) wbuffer;
    U8 *wdata8 = (U8 *) wbuffer;
    U32 idx = 0;
    U32 temp;
    
    if(spidrvdatptr->init == TRUE)
    {
    	// Set current status for SPI operation
    	spidrvdatptr->status = SPI_STAT_BUSY;
    	
    	//read for empty buffer
	    temp = spidrvdatptr->regptr->SPDR;
	    //dummy to clear status
	    temp = spidrvdatptr->regptr->SPSR;
	    
	    /* in master mode, SS pin must be drive 
	     * to low before transferring */
	    GPIO_Clr (GPIO_0, (1 << SPI_PIN_FUNC_SSEL));
	    FIO_Clr32 (FIO_0, (1 << SPI_PIN_FUNC_SSEL));
	    
    	while(idx < length)
    	{
    		// Write data to buffer
			if(wbuffer == NULL)
			{
				spidrvdatptr->regptr->SPDR = 0xFF;
			}
			else
			{
				if (spidrvdatptr->dsize == TRUE)
			   	{
					spidrvdatptr->regptr->SPDR = (U32) (*(wdata16));
					wdata16++;
			   	}
			   	else
			   	{
			   		 spidrvdatptr->regptr->SPDR = (U32) (*(wdata8));
			   		 wdata8++;
			   	}

			 }

			// Wait for transfer complete
			while (!(spidrvdatptr->regptr->SPSR & SPI_SPIF_MASK));
			
			// Read data from SPI dat
		    temp = (U32) spidrvdatptr->regptr->SPDR;
			
		    // Store data to destination 
		    if (rbuffer != NULL)
			{
		    	if (spidrvdatptr->dsize == TRUE)
			   	{
				   	*(rdata16) = (U16)temp;
				   	rdata16++;
				   	idx += 2;
			   	}
			   	else
			   	{
			   		*(rdata8) = (U8) temp;
			   		rdata8++;
			   		idx++;
			   	}
			}
    	}
    	
	    /* in master mode, SS pin must be drive to high 
	     * after finish transferring */
	    GPIO_Set (GPIO_0, (1 << SPI_PIN_FUNC_SSEL));
	    FIO_Set32 (FIO_0, (1 << SPI_PIN_FUNC_SSEL));
	    
	    // Release current SPI operation status
	    spidrvdatptr->status = SPI_STAT_IDLE;
	    
    	// Return length of actual data transferred
    	return idx;
    }
    else
    {
    	return 0;
    }
}


/***********************************************************************
 * Function: SPI_ReadWriteAsync
 * Purpose: SPI Read write in asynchronous (interrupt) mode function 
 * Parameters:
 * 		SPIdev: Pointer to SPI device
 * 		rbuffer: pointer to read buffer
 * 		wbuffer: pointer to write buffer
 * 		length: length of data to read and write
 * Return: length of actual data transferred
 ***********************************************************************/
S32 SPI_ReadWriteAsync (S32 SPIdev,
				    void *rbuffer,
				    void *wbuffer,
				    U32 length)
{
	SPI_DAT_Type *spidrvdatptr = (SPI_DAT_Type *) SPIdev;
    U16 *wdata16 = (U16 *) wbuffer;
    U8 *wdata8 = (U8 *) wbuffer;
    U32 temp;
    
    if (spidrvdatptr->init == TRUE)
    {
    	// Set current status for SPI operation
    	spidrvdatptr->status = SPI_STAT_BUSY;
    	
    	// Clear SPI interrupt flag
    	spidrvdatptr->regptr->SPINT = 0x01;
    	
    	// read for empty buffer
	    temp = spidrvdatptr->regptr->SPDR;
	    // dummy to clear status
	    temp = spidrvdatptr->regptr->SPSR;
	    
	    // store rdatabuffer
		spidrvdatptr->rdata_transfer = rbuffer;
		spidrvdatptr->wdata_transfer = wbuffer;
		// Reset index
		spidrvdatptr->index = 0;
		// store length of buffer;
		spidrvdatptr->length = length;
		
	    // Point to interrupt handler function 
	    spidrvdatptr->handler_int = SPI_IntReadWrite;
	    
	    /* in master mode, SS pin must be drive 
	     * to low before transferring */
	    GPIO_Clr (GPIO_0, (1 << SPI_PIN_FUNC_SSEL));
	    FIO_Clr32 (FIO_0, (1 << SPI_PIN_FUNC_SSEL));
	    
		// Enable SPI interrupt bit 
		spidrvdatptr->regptr->SPCR |= SPI_SPIE_BIT;
		
		/* Enable SPI Interrupt source */
		VIC_EnableInt (SPI0_INT);
		
		//send the first frame data
		if (spidrvdatptr->wdata_transfer == NULL)
		{
			spidrvdatptr->regptr->SPDR = (U32) 0xFF;
		}
		else
		{
			if (spidrvdatptr->dsize == TRUE)
			{
				spidrvdatptr->regptr->SPDR =
					(U32) (*(wdata16 + spidrvdatptr->index));
			}
			else
			{
				spidrvdatptr->regptr->SPDR =
					(U32) (*(wdata8 + spidrvdatptr->index));
			}
		}
		
		return (S32) spidrvdatptr->index;
    }
    else
    {
    	return 0;
    }
}


/***********************************************************************
 * Function: SPI_IntReadWrite
 * Purpose: SPI Interrupt read and write handler
 * Parameters: None
 * Return: None
 ***********************************************************************/
void SPI_IntReadWrite (S32 SPIdev)
{
	SPI_DAT_Type *spidrvdatptr = (SPI_DAT_Type *) SPIdev;
	
	// point to read write buffer 
	U8 *rdata8 = (U8 *) spidrvdatptr->rdata_transfer;
	U8 *wdata8 = (U8 *) spidrvdatptr->wdata_transfer;
	U16 *rdata16 = (U16 *) spidrvdatptr->rdata_transfer;
	U16 *wdata16 = (U16 *) spidrvdatptr->wdata_transfer;
	U32 temp;
	
	// Disable SPI interrupt flag 
	spidrvdatptr->regptr->SPCR &= ~SPI_SPIE_BIT;
	
	// dummy to clear status register
	temp = spidrvdatptr->regptr->SPSR;
	
	// clear int_flag
	spidrvdatptr->regptr->SPINT = 0x01;

	// Check SPI status
	if ((temp & 0x78) != 0)
	{
		// Store current status of SPI operation
		spidrvdatptr->status = temp | 0x01;
	    
		// Terminate current transferring
	    GPIO_Set (GPIO_0, (1 << SPI_PIN_FUNC_SSEL));
	    FIO_Set32 (FIO_0, (1 << SPI_PIN_FUNC_SSEL));
	    
		 // Disable SPI interrupt source 
		 VIC_DisableInt (SPI0_INT);
		
		 spidrvdatptr->cbs();
		 
		 return;
	}
	
	// Read data and increase index
	temp = (U32) spidrvdatptr->regptr->SPDR;
	
	if (spidrvdatptr->rdata_transfer != NULL)
	{
		if (spidrvdatptr->dsize == FALSE)
		{
			*(rdata8 + spidrvdatptr->index) = (U8) temp;
			spidrvdatptr->index++;
		}
		else
		{
			*(rdata16 + spidrvdatptr->index) = (U16) temp;
			spidrvdatptr->index += 2;
		}
	}
	
	// Write next data
	if (spidrvdatptr->index < spidrvdatptr->length)
	{
		// Enable SPI interrupt bit 
		spidrvdatptr->regptr->SPCR |= SPI_SPIE_BIT;
		
		 if (spidrvdatptr->wdata_transfer == NULL)
		{
			spidrvdatptr->regptr->SPDR = (U32) 0xFF;
		}
		else
		{
			if (spidrvdatptr->dsize == FALSE)
			{
				spidrvdatptr->regptr->SPDR = (U32) (*(wdata8 + spidrvdatptr->index));
			}
			else
			{
				spidrvdatptr->regptr->SPDR = (U32) (*(wdata16 + spidrvdatptr->index));
			}
		}
	 }
	 else
	 {
	    /* in master mode, SS pin must be drive to high 
	     * after finish transferring */
	    GPIO_Set (GPIO_0, (1 << SPI_PIN_FUNC_SSEL));
	    FIO_Set32 (FIO_0, (1 << SPI_PIN_FUNC_SSEL));
	    
		 // Disable SPI interrupt source 
		 VIC_DisableInt (SPI0_INT);
		 
		 // Release current SPI operation status
		 spidrvdatptr->status = SPI_STAT_IDLE;
		    
		 // Activate callback function if it is already installed
		 if (spidrvdatptr->cbs != NULL)
		 {
			 spidrvdatptr->cbs(); 
		 }
	 }
}


/***********************************************************************
 * Function: SPI_SetCBS
 * Purpose: Set call back function for SPI in ASYNC Read Write mode
 * Parameters: 
 * 		SPIdev: Pointer to SPI device
 * 		pCBS: Pointer to call back function
 * Return: Status of progress (_ERROR or _NO_ERROR)
 ***********************************************************************/
S32 SPI_SetCBS (S32 SPIdev, void *pCBS)
{
	SPI_DAT_Type *spidrvdatptr = (SPI_DAT_Type *) SPIdev;
	
	if (spidrvdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	spidrvdatptr->cbs = (void *) pCBS;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: SPI_GetStatus
 * Purpose: Get status of current SPI operation
 * Parameters: 
 * 		SPIdev: Pointer to SPI device
 * Return: Value of SPI operation status
 ***********************************************************************/
U32 SPI_GetStatus (S32 SPIdev)
{
	SPI_DAT_Type *spidrvdatptr = (SPI_DAT_Type *) SPIdev;
	
	if (spidrvdatptr->init == FALSE)
	{
		return 0;
	}
	
	return spidrvdatptr->status;
}


/***********************************************************************
 * Function: SPI_ISR
 * Purpose: Standard SPI interrupt handler function
 * Parameters: None
 * Return: None
 ***********************************************************************/
void SPI_ISR (void)
{
	spidrvdat.handler_int((S32) (&spidrvdat));
	
	// Update VIC addr
	VIC_Ack();
}
