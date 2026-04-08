/***********************************************************************
* $Id: lpc23xx_adc_driver.c 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx ADC driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: ADC
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
#include "lpc23xx_adc_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_vic_driver.h"

/***********************************************************************
 * ADC driver private data and types
 **********************************************************************/
/* ADC configuration type */
typedef struct
{
  BOOL32 		init;       			    	/* Device initialized flag */
  ADC_Type 		*regptr; 						/* Pointer to ADC registers */
  __IO U32 		ADCBuff[8];  					/* ADC data buffer */
  void			(*cbs)(void);					/* Call back function */
} ADC_CFG_T;

/* ADC configuration data */
STATIC ADC_CFG_T adccfg = {
		FALSE, 
		(ADC_Type *) 0,
		{0, 0, 0, 0, 0, 0, 0, 0}
};


/***********************************************************************
 * Public ADC API driver 
 **********************************************************************/

/**********************************************************************
 * Function: ADC_Init
 * Purpose: Init ADC device 
 * Parameter: None
 * Return: 
 * 		Pointer to ADC device if success or 0 if error
 *********************************************************************/
S32 ADC_Init(void)
{	
	U8 tmp;
	S32 status = 0;
	U32 dummy;
	
	if (adccfg.init == FALSE)
	{
		/* Device is valid and not previously initialized */
		adccfg.init = TRUE;
		
		/* Save and return address of peripheral block */
		adccfg.regptr = ADC0;
		
		// Clear all ADC buff data
		for (tmp = 0; tmp < 8; tmp++)
		{
			adccfg.ADCBuff[tmp] = 0;
		}
		
		// Turn on power and clock
		SCB_EnablePPWR (PCAD);
		
		// Set clock divider for ADC to 4 from CCLK as default
		SCB_SetPCLKDiv (PCLK_ADC, CCLK_DIV_4);
		
		// Set ADC control to default
		adccfg.regptr->CR = ADC_CR_PDN;
		adccfg.regptr->INTEN = 0x00;
		
		// Read current status value to clear
		dummy = adccfg.regptr->STAT;
		dummy = adccfg.regptr->GDR;
		
		for (tmp = 0; tmp < 8; tmp++)
		{
			dummy = adccfg.regptr->DR[tmp];
		}
		
		// set callback function to NULL
		adccfg.cbs = NULL;
		
		/* Return pointer to ADC configuration structure */
		status = (S32) (&adccfg);
	}
	
	return status;
}


/**********************************************************************
 * Function: ADC_Close
 * Purpose: Close ADC device
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 ADC_Close (S32 ADCdev)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	
	if (pADCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	// Clear PDN bit 
	pADCdev->regptr->CR &= ~ADC_CR_PDN;
	
	// Turn off ADC power
	SCB_DisablePPWR (PCAD);
	
	// Remove pointer to register
	pADCdev->regptr = 0;
	
	// Set init flag to FALSE
	pADCdev->init = FALSE;
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: ADC_Config
 * Purpose: Configure ADC device
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * 		freq: target frequency of ADC conversion
 * 		resolution: resolution of ADC conversion (must be from 3..10 bit)
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 ADC_Config (S32 ADCdev, U32 freq, U32 resolution)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	U32 tmp;
	
	if (pADCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	if ((resolution > 10) || (resolution < 3))
	{
		return _ERROR;
	}
	
	if (freq > 4500000)
	{
		return _ERROR;
	}
	
	tmp = SCB_GetPCLK (PCLK_ADC);
	
	tmp = (tmp / freq) - 1;
	pADCdev->regptr->CR &= ~(ADC_CR_CLKDIV(0xFF));
	pADCdev->regptr->CR |= ADC_CR_CLKDIV(tmp);
	
	pADCdev->regptr->CR &= ~(ADC_CR_BITS(3));
	pADCdev->regptr->CR |= ADC_CR_BITS(resolution);
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: ADC_EnableCH
 * Purpose: Enable channel of ADC to be convert
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * 		ADC_channel: channel on ADC device to enable (must be 0..7)
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 ADC_EnableCH (S32 ADCdev, U32 ADC_channel)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	
	if (pADCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	if (ADC_channel > 7)
	{
		return _ERROR;
	}
	
	pADCdev->regptr->CR |= ADC_CR_CH_SEL(ADC_channel);
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: ADC_DisableCH
 * Purpose: Disable channel of ADC to be convert
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * 		ADC_channel: channel on ADC device to disable (must be 0..7)
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 ADC_DisableCH (S32 ADCdev, U32 ADC_channel)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	
	if (pADCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	if (ADC_channel > 7)
	{
		return _ERROR;
	}
	
	pADCdev->regptr->CR &= ~(ADC_CR_CH_SEL(ADC_channel));
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: ADC_Start
 * Purpose: Start AD conversion in each type of start 
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * 		starttype: type of start (selected from ADC_START_OPT enum)
 * 		arg: additional parameter used if starttype selected from
 * CAP/MATCH type (selected from ADC_START_ON_EDGE_OPT enum)
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 ADC_Start (S32 ADCdev, S32 starttype, S32 arg)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	U8 tmp;
	U32 dummy;
	
	if (pADCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	// Read all ADC data register to clear DONE bit
	for (tmp = 0; tmp < 8; tmp++)
	{
		dummy = pADCdev->regptr->DR[tmp];
	}
	
	// clear current start type
	pADCdev->regptr->CR &= ~(ADC_CR_START_MAT11);
	pADCdev->regptr->CR &= ~ADC_CR_BURST;
	
	switch ((ADC_START_OPT) starttype)
	{
	case ADC_START_CONTINUOUS:
		pADCdev->regptr->CR |= ADC_CR_BURST;
		break;
		
	case ADC_START_NOW:
		pADCdev->regptr->CR |= ADC_CR_START_NOW;
		break;
		
	case ADC_START_ON_EINT0:
		pADCdev->regptr->CR |= ADC_CR_START_EINT0;
		break;
		
	case ADC_START_ON_CAP01:
		pADCdev->regptr->CR |= ADC_CR_START_CAP01;
		break;
		
	case ADC_START_ON_MAT01:
		pADCdev->regptr->CR |= ADC_CR_START_MAT01;
		break;
		
	case ADC_START_ON_MAT03:
		pADCdev->regptr->CR |= ADC_CR_START_MAT03;
		break;
		
	case ADC_START_ON_MAT10:
		pADCdev->regptr->CR |= ADC_CR_START_MAT10;
		break;
		
	case ADC_START_ON_MAT11:
		pADCdev->regptr->CR |= ADC_CR_START_MAT11;
		break;
		
	// Bad parameter
	default:
		return _ERROR;
		break;
	}
	
	if (((ADC_START_OPT) starttype != ADC_START_CONTINUOUS)
			&& ((ADC_START_OPT) starttype != ADC_START_NOW))
	{
		if ((ADC_START_ON_EDGE_OPT) arg == ADC_START_ON_RISING)
		{
			pADCdev->regptr->CR &= ~(ADC_CR_EDGE);
		}
		else if ((ADC_START_ON_EDGE_OPT) arg == ADC_START_ON_FALLING)
		{
			pADCdev->regptr->CR |= ADC_CR_EDGE;
		}
		else
		{
			return _ERROR;
		}
	}
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: ADC_Stop
 * Purpose: Stop AD conversion
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 ADC_Stop (S32 ADCdev)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	
	if (pADCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	// clear current start type
	pADCdev->regptr->CR &= ~(ADC_CR_START_MAT11);
	pADCdev->regptr->CR &= ~ADC_CR_BURST;
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: ADC_ReadPoll
 * Purpose: Read AD value of each channel according in polling mode
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * 		ADC_channel: ADC channel to read value
 * Return: ADC value read from ADC channel
 *********************************************************************/
U32 ADC_ReadPoll (S32 ADCdev, U32 ADC_channel)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	U32 regval;
	
	if (pADCdev->init == FALSE)
	{
		return 0xFFFFFFFF;
	}
	
	if (ADC_channel > 7)
	{
		return 0xFFFFFFFF;
	}
	
	// Wait for DONE bit is set
	while (!((pADCdev->regptr->DR[ADC_channel]) & ADC_DR_DONE_FLAG));
	regval = pADCdev->regptr->DR[ADC_channel];
	
	return ADC_DR_RESULT(regval);
}


/**********************************************************************
 * Function: ADC_ReadAsync
 * Purpose: Read AD value of each channel in interrupt mode,
 * after interrupt handler, current value of each channel will be
 * copied to AD buffer [0..7]
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * 		ADC_channel: ADC channel to read value from ADC data buffer
 * Return: ADC value read from ADC channel
 *********************************************************************/
U32 ADC_ReadAsync (S32 ADCdev, U32 ADC_channel)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	U32 regval;
	
	if (pADCdev->init == FALSE)
	{
		return 0xFFFFFFFF;
	}
	
	if (ADC_channel > 7)
	{
		return 0xFFFFFFFF;
	}
	
	// Check if DONE bit is set
	if ((pADCdev->ADCBuff[ADC_channel]) & ADC_DR_DONE_FLAG)
	{
		regval = ADC_DR_RESULT(pADCdev->ADCBuff[ADC_channel]);
		// Clear value in ADC buff after reading
		pADCdev->ADCBuff[ADC_channel] = 0;
	}
	// If DONE bit is not set
	else
	{
		return 0xFFFFFFFF;
	}
	
	return regval;
}


/**********************************************************************
 * Function: ADC_EnableInt
 * Purpose: Enable ADC interrupt
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * 		ADC_channel: ADC channel to enable interrupt on it.
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 ADC_EnableInt (S32 ADCdev, U32 ADC_channel)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	
	if (pADCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	if (ADC_channel > 7)
	{
		return _ERROR;
	}
	
	pADCdev->regptr->INTEN |= ADC_INTEN_CH(ADC_channel);
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: ADC_DisableInt
 * Purpose: Enable ADC interrupt
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * 		ADC_channel: ADC channel to disable interrupt on it.
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 ADC_DisableInt (S32 ADCdev, U32 ADC_channel)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	
	if (pADCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	if (ADC_channel > 7)
	{
		return _ERROR;
	}
	
	pADCdev->regptr->INTEN &= ~(ADC_INTEN_CH(ADC_channel));
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: ADC_EnableGlobalInt
 * Purpose: Enable global interrupt on ADC device
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 ADC_EnableGlobalInt (S32 ADCdev)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	
	if (pADCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	pADCdev->regptr->INTEN |= ADC_INTEN_GLOBAL;
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: ADC_DisableGlobalInt
 * Purpose: Disable global interrupt on ADC device
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 ADC_DisableGlobalInt (S32 ADCdev)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	
	if (pADCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	pADCdev->regptr->INTEN &= ~ADC_INTEN_GLOBAL;
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: ADC_ISR
 * Purpose: Interrupt handler for ADC interrupt mode 
 * Parameter: None
 * Return: None
 *********************************************************************/
void ADC_ISR (void)
{
	U32 regval = adccfg.regptr->STAT;
	U32 dummy;
	U8 i;
	
	if (regval & 0x0000FF00)	/* check OVERRUN error first */
	{
		regval = ADC_STAT_CH_OVERRUN_FLAG(regval);
	    
		for (i = 0; i < 8; i++)
		{
			if (regval & 0x01)
			{
				/* if overrun, just read ADDR to clear */
				dummy = adccfg.regptr->DR[i];
			}
			regval = regval >> 1;
		}
	}
	
	// Check DONE bit
	else if (regval & ADC_STAT_INT_FLAG)
	{
		regval = ADC_STAT_CH_DONE_FLAG(regval);
	    
		for (i = 0; i < 8; i++)
		{
			if (regval & 0x01)
			{
				/* if overrun, just read ADDR to clear */
				adccfg.ADCBuff[i] = adccfg.regptr->DR[i];
			}
			regval = regval >> 1;
		}
	}
	
	// call callback function if it is already installed
	if (adccfg.cbs != NULL)
	{
		adccfg.cbs();
	}
	
	// Acknowledge interrupt
	VIC_Ack();
}


/**********************************************************************
 * Function: ADC_SetCBS
 * Purpose: Set callback functon for ADC device
 * Parameter:
 * 		ADCdev: Pointer to ADC device
 * 		pCBS: pointer to call back function
 * Return: 
 * 		Status of progress (_NO_ERROR or _ERROR)
 *********************************************************************/
S32 ADC_SetCBS (S32 ADCdev, void *pCBS)
{
	ADC_CFG_T *pADCdev = (ADC_CFG_T *) ADCdev;
	
	if (pADCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	pADCdev->cbs = pCBS;
	
	return _NO_ERROR;
}
