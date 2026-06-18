/***********************************************************************
 * $Id: lpc23xx_pwm_driver.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx PWM driver 
 *
 * Description:
 *     This file contains driver support for the LPC23xx PWM.
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
 *********************************************************************/

#include "lpc23xx_pwm_driver.h"
#include "lpc23xx_scb_driver.h"

/***********************************************************************
 * PWM driver package data
***********************************************************************/

/* PWM device configuration structure type */
typedef struct
{
    PWM_Type *regptr;     /* Pointer to PWM registers */
    BOOL32 init;           /* Device initialized flag */
} PWM_CFG_T;

/* PWM driver data */
static PWM_CFG_T pwmdat;

/***********************************************************************
 * PWM driver private functions
 **********************************************************************/

/***********************************************************************
 * Function: PWM_WriteMatch
 * Purpose: write into the specified PWM match register
 * Parameters:
 *     devid: Pointer to PWM config structure
 *     reg:   match register id
 *     arg:   argument
 * Returns: None
 **********************************************************************/
void PWM_WriteMatch (PWM_Type * devid, S32 reg, S32 arg)
{
    if (reg < 4)
    {
        *(((U32 *) &(devid->MR0)) + reg) = arg;
    }
    else
    {
        *(((U32 *) &(devid->MR4)) + reg - 4) = arg;
    }
}

/***********************************************************************
 * Public PWM driver functions
 **********************************************************************/

/**********************************************************************
 * Function: PWM_Init
 * Purpose: Init PWM controller
 * Parameter: 
 * 		PWMNum: PWM controler number (must be 0)
 * Return: 
 * 		Pointer to PWM device if success or 0 if error
 *********************************************************************/
S32 PWM_Init (S32 PWMNum)
{
	PWM_CFG_T *pwmdatptr;
	
    if (PWMNum == 0)
    {
    	pwmdatptr = &pwmdat;
    }
    else
    {
    	return 0;
    }
    
    
    /* has the selected PWM been previously initialized? */
    if (pwmdatptr->init == FALSE)
    {
        /* Device not initialized and it is usable, so set it to
           used */
        pwmdatptr->init = TRUE;

        /* Save address of register block */
        pwmdatptr->regptr = PWM;
        /* Disable selected PWM timer and outputs */
        pwmdatptr->regptr->TCR = 0x00;
        pwmdatptr->regptr->PCR &= ~0x00007E00;

        // Enable power and clock for PWM module
        SCB_EnablePPWR (PCPWM1);
        // PCLK = CCLK/4 as default
        SCB_SetPCLKDiv (PCLK_PWM1, CCLK_DIV_4);
    }
    // Has been already initialized
    else
    {
    	return 0;
    }
    
    return (S32) (pwmdatptr);
}


/**********************************************************************
 * Function: PWM_Close
 * Purpose: Close PWM device
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_Close (S32 PWMdev)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}

    /* Disable PWM */
	pwmdatptr->regptr->TCR = 0x00;

    /* Set timer as uninitialized */
	pwmdatptr->init = FALSE;
    
	// Turn off power for PWM module
	SCB_DisablePPWR (PCPWM1);
	
    return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_Enable
 * Purpose: Enable PWM activity 
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_Enable (S32 PWMdev)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->TCR |= PWM_COUNTER_ENABLE;
	pwmdatptr->regptr->TCR |= PWM_ENABLE;
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_Disable
 * Purpose: Disable PWM activity
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_Disable (S32 PWMdev)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->TCR &= ~PWM_ENABLE;
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_StartTC
 * Purpose: Start timer/counter in PWM module
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_StartTC (S32 PWMdev)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->TCR |= PWM_COUNTER_ENABLE;
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_StopTC
 * Purpose: Stop timer/counter in PWM module 
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_StopTC (S32 PWMdev)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->TCR &= ~PWM_COUNTER_ENABLE;
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_ResetTC
 * Purpose: Reset counter/timer in PWM module 
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_ResetTC (S32 PWMdev)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->TCR |= PWM_COUNTER_RESET;
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_Setup
 * Purpose: Setup PWM module system
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		prescale: PWM frequency prescaler value from CCLK
 * 		period: value of period (0 --> 0xFFFFFFFF)
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_Setup (S32 PWMdev, U32 prescale, U32 period)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->PR = prescale - 1;
	pwmdatptr->regptr->MR0 = period;
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_SetTCMode
 * Purpose: Set TC mode for PWM
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		TCMode: Timer/Counter mode
 * 		ICAPsource:	Input capture source (available if TCMode is Counter mode)
 * 		ICAPMode: Input capture mode (available if TCMode is Counter mode)
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_SetTCMode (S32 PWMdev, S32 TCMode, S32 ICAPsource, S32 ICAPMode)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	switch ((PWM_TC_MODE_OPT) TCMode)
	{
	case PWM_TC_MODE_TIMER:
		pwmdatptr->regptr->CTCR = 0x00;
		break;
		
	case PWM_TC_MODE_COUNTER:		
		switch ((PWM_ICAP_MODE_OPT) ICAPMode)
		{
		case PWM_CAPTURE_RISING:
			pwmdatptr->regptr->CTCR = 0x01;
			break;
			
		case PWM_CAPTURE_FALLING:
			pwmdatptr->regptr->CTCR = 0x02;
			break;
			
		case PWM_CAPTURE_ANY:
			pwmdatptr->regptr->CTCR = 0x03;
			break;
		}
		
		if (((PWM_ICAP_OPT) ICAPsource) == PWM_ICAP_PCAP10)
		{
			// Do nothing
		}
		else if (((PWM_ICAP_OPT) ICAPsource) == PWM_ICAP_CAP11)
		{
			pwmdatptr->regptr->CTCR |= (1 << 2);
		}
		
		break;
	}
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_Update
 * Purpose: Update PWM system after re-configure 
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_Update (S32 PWMdev)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->LER |= PWM_EN_MATCHL(0);
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_UpdateNow
 * Purpose: Update PWM system immediately after re-configure
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_UpdateNow (S32 PWMdev)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->LER |= PWM_EN_MATCHL(0);
	pwmdatptr->regptr->TCR |= PWM_COUNTER_ENABLE;
	pwmdatptr->regptr->TCR &= ~PWM_ENABLE;
	pwmdatptr->regptr->TCR |= PWM_ENABLE;
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_ConfigChannel
 * Purpose: Configure PWM channel
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		channel: PWM channel
 * 		mode: PWM mode (single edge or dual edge)
 * 		duty_option: duty option (ABS or percent)
 * 		duty: value of duty
 * 		offset_option: offset value for the first edge from the beginning
 * of each cycle (available if dual edge applied) 
 * 		offset: offset value
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_ConfigChannel (S32 PWMdev, U32 channel, S32 mode, 
						S32 duty_option, U32 duty, S32 offset_option, U32 offset)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	U32 pointf, pointr, pulse;
	BOOL32 solved;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
    if ((channel == 0) || (channel > 6))
    {
        return _ERROR;
    }

    switch ((PWM_MODE_OPT) mode)
	{
	    case PWM_SINGLE_EDGE:
	    {
	        if ((PWM_IOCTL_DUTY_OPT) duty_option == PWM_WAV_DUTY_ABS)
	        {
	            pointf = duty;
	        }
	        else if (duty == 0)
	        {
	            pointf = 0;
	        }
	        else if (duty > 99)
	        {
	            pointf = 0xFFFFFFFF;
	        }
	        else
	        {
	            pointf = (duty * (pwmdatptr->regptr->MR0)) / 100;
	        }
	        
	        PWM_WriteMatch(pwmdatptr->regptr,channel,pointf);
	        if (channel > 1)
	        {
	        	pwmdatptr->regptr->PCR &= ~PWM_DOUBLE_EDGE(channel);
	        }
	    }
	    break;
	
	    case PWM_DUAL_EDGE:
	    {
	        solved = FALSE;
	        if ((PWM_IOCTL_DUTY_OPT) duty_option == PWM_WAV_DUTY_ABS)
	        {
	            pulse = duty;
	        }
	        else if (duty > 100)
	        {
	            return _ERROR;                                        
	            break;
	        }
	        else if (duty == 0)
	        {
	            pointf = 0;
	            pointr = 0xFFFFFFFF;
	            solved = TRUE;
	        }
	        else if (duty == 100)
	        {
	            pointr = 0;
	            pointf = 0xFFFFFFFF;
	            solved = TRUE;
	        }
	        else
	        {
	            pulse = (duty * (pwmdatptr->regptr->MR0)) / 100;
	        }
	        if (solved == FALSE)
	        {
	            if ((PWM_IOCTL_OFFSET_OPT) offset_option == PWM_WAV_OFFSET_ABS)
	            {
	                pointr = offset;
	            }
	            else if (offset > 100)
	            {
	                return _ERROR;                                        
	                break;
	            }
	            else if (offset == 0)
	            {
	                pointr = 0;
	            }
	            else if (offset == 100)
	            {
	                pointr = pwmdatptr->regptr->MR0;
	            }
	            else
	            {
	                pointr = (offset * (pwmdatptr->regptr->MR0)) / 100;
	            }
	            pointf = pointr + pulse;
	        }
	
	        PWM_WriteMatch(pwmdatptr->regptr, channel-1, pointr);
	        PWM_WriteMatch(pwmdatptr->regptr, channel, pointf);
	        pwmdatptr->regptr->PCR |= PWM_DOUBLE_EDGE(channel);
	    }
	    break;
	
	    default:
	    {
	        return _ERROR;
	    }
	    break;
	}   
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_EnableChannel
 * Purpose: Enable PWM channel
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		channel: PWM channel
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_EnableChannel (S32 PWMdev, U32 channel)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->PCR |= PWM_ENABLE_OUT(channel);
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_DisableChannel
 * Purpose: Disable PWM channel
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		channel: PWM channel
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_DisableChannel (S32 PWMdev, U32 channel)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->PCR &= ~PWM_ENABLE_OUT(channel);
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_UpdateChannel
 * Purpose: Update PWM channel after re-configure
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		channel: PWM channel
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_UpdateChannel (S32 PWMdev, U32 channel)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	if ((channel <= 0) || (channel > 6))
	{
		return _ERROR;
	}
	else
	{
		pwmdatptr->regptr->LER |= 1 << channel;
	}
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_EnableMatchInt
 * Purpose: Enable PWM match interrupt
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		matchsource: match source that generates interrupt
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_EnableMatchInt (S32 PWMdev, U32 matchsource)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
    if (matchsource > 6)
    {
        return _ERROR;
    }
    else
    {
    	pwmdatptr->regptr->MCR |= PWM_INT_ON_MATCH(matchsource);
    }
    
    return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_DisableMatchInt
 * Purpose: Disable PWM match interrupt 
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		matchsource: match source that generates interrupt
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_DisableMatchInt (S32 PWMdev, U32 matchsource)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
    pwmdatptr->regptr->MCR &= ~PWM_INT_ON_MATCH(matchsource);
    
    return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_EnableMatchResetTC
 * Purpose: Enable Reset TC when matching
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		matchsource: match source that can be enabled reset TC
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_EnableMatchResetTC (S32 PWMdev, U32 matchsource)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->MCR |= PWM_RESET_ON_MATCH(matchsource);
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_DisableMatchResetTC
 * Purpose: Disable Reset TC when matching
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		matchsource: match source that can be disabled reset TC
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_DisableMatchResetTC (S32 PWMdev, U32 matchsource)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->MCR &= ~PWM_RESET_ON_MATCH(matchsource);
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_EnableMatchStopTC
 * Purpose: Enable Stop TC when matching
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		matchsource: match source that can be enabled stop TC
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_EnableMatchStopTC (S32 PWMdev, U32 matchsource)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->MCR |= PWM_STOP_ON_MATCH(matchsource);
	
	return _NO_ERROR;	
}


/**********************************************************************
 * Function: PWM_DisableMatchStopTC
 * Purpose: Disable Stop TC when matching
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		matchsource: match source that can be disabled stop TC
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_DisableMatchStopTC (S32 PWMdev, U32 matchsource)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->MCR &= ~PWM_STOP_ON_MATCH(matchsource);
	
	return _NO_ERROR;	
}


/**********************************************************************
 * Function: PWM_ReadMatchIntPending
 * Purpose: Read Pending Match Interrupt 
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * Return: 
 * 		Value of pending match interrupt 
 *********************************************************************/
U32 PWM_ReadMatchIntPending (S32 PWMdev)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return 0;
	}
    
    return pwmdatptr->regptr->IR;
}


/**********************************************************************
 * Function: PWM_ClrMatchIntPending
 * Purpose: Clear pending PWM match interrupt flag
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		matchsource: match source to clear its pending interrupt
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_ClrMatchIntPending (S32 PWMdev, U32 matchsource)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	pwmdatptr->regptr->IR |= PWM_INT_MATCH(matchsource);
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_EnableCapInt
 * Purpose: Enable PWM capture interrupt
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		capsource: capture source that generates interrupt
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_EnableCapInt (S32 PWMdev, U32 capsource)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->CCR |= PWM_ICAP_EN(capsource);
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_DisableCapInt
 * Purpose: Disable PWM capture interrupt
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		capsource: capture source that generates interrupt
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_DisableCapInt (S32 PWMdev, U32 capsource)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->CCR &= ~PWM_ICAP_EN(capsource);
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: PWM_ClrCapIntPending
 * Purpose: Clear pending PWM capture interrupt flag
 * Parameter: 
 * 		PWMdev: Pointer to PWM device
 * 		capsource: capture source to clear its pending interrupt
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 PWM_ClrCapIntPending (S32 PWMdev, U32 capsource)
{
	PWM_CFG_T *pwmdatptr = (PWM_CFG_T *) PWMdev;
	
	if (pwmdatptr->init == FALSE)
	{
		return _ERROR;
	}
	
	pwmdatptr->regptr->IR |= PWMCAP_INT(capsource);
	
	return _NO_ERROR;
}
