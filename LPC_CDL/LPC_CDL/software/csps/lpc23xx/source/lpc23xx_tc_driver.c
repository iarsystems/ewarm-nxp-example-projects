/***********************************************************************
 * ID: lpc23xx_tc_driver.c 2008-11-03 HieuNguyen
 *
 * Project: LPC23xx timer driver
 *
 * Description:
 *     This file contains driver support for the LPC23xx timer.
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
#include "lpc23xx_tc_driver.h"
#include "lpc23xx_scb_driver.h"


/***********************************************************************
 * Timer driver package data
***********************************************************************/

/* Timer device configuration structure type */
typedef struct
{
	BOOL32 			init;		/* Init flag of timer/counter device */
	TC_Type 		*regptr; 	/* Pointer to timer register */
} TC_DAT_Type;

/* Timer /counter driver data */
static TC_DAT_Type tcdat[4] = {
		{FALSE, (TC_Type *) 0}, 
		{FALSE, (TC_Type *) 0}, 
		{FALSE, (TC_Type *) 0}, 
		{FALSE, (TC_Type *) 0},
};


/***********************************************************************
 * Timer driver private functions
 **********************************************************************/
U32 timer_get_pclock (S32 timernum);
U32 timer_usec_to_val (S32 timernum, U32 usec);
S32 timer_ptr_to_timer_num (TC_Type *pTimer);
S32 timer_cfg_to_timer_num (TC_DAT_Type *pTimerCfg);
void timer_delay_cmn(TC_Type *pTimer, U32 usec);


/***********************************************************************
 * Function: timer_get_pclock
 * Purpose: Get peripheral clock of each timer controller
 * Parameters:
 *     timernum: Timer number
 * Returns: Peripheral clock of timer
 **********************************************************************/
U32 timer_get_pclock (S32 timernum)
{
	U32 clkdlycnt;
	switch (timernum)
	{
	case 0:
		clkdlycnt = (U64)SCB_GetPCLK (PCLK_TIMER0);
		break;
		
	case 1:
		clkdlycnt = (U64)SCB_GetPCLK (PCLK_TIMER1);
		break;
		
	case 2:
		clkdlycnt = (U64)SCB_GetPCLK (PCLK_TIMER2);
		break;
		
	case 3:
		clkdlycnt = (U64)SCB_GetPCLK (PCLK_TIMER3);
		break;
	}
	return clkdlycnt;
}


/***********************************************************************
 * Function: timer_usec_to_val
 * Purpose: Convert a time to a timer count value
 * Parameters:
 *     timernum: Timer number
 *     usec: Time in microseconds
 * Returns: The number of required clock ticks to give the time delay
 **********************************************************************/
U32 timer_usec_to_val (S32 timernum, U32 usec)
{
	U64 clkdlycnt;
	
	// Get Pclock of timer
	clkdlycnt = (U64) timer_get_pclock (timernum);
	clkdlycnt = (clkdlycnt * (U64) usec) / 1000000;

	return (U32) clkdlycnt;
}


/***********************************************************************
 * Function: timer_ptr_to_timer_num
 * Purpose: Convert a timer register pointer to a timer number
 * Parameters:
 *     pTimer: Pointer to a timer register set
 * Returns: The timer number (0 to 3) or -1 if register pointer is bad
 **********************************************************************/
S32 timer_ptr_to_timer_num (TC_Type *pTimer) 
{
	S32 tnum = -1;

	if (pTimer == TC0) 
	{
		tnum = 0;
	}
	else if (pTimer == TC1) 
	{
		tnum = 1;
	}
	else if (pTimer == TC2)
	{
		tnum = 2;
	}
	else if (pTimer == TC3)
	{
		tnum = 3;
	}
	
	return tnum;
}


/***********************************************************************
 * Function: timer_cfg_to_timer_num
 * Purpose: Convert a timer driver config pointer to a timer number
 * Parameters:
 *     pTimer: Pointer to a timer config structure
 * Returns: The timer number (0 to 3) or -1 if pointer is bad
 **********************************************************************/
S32 timer_cfg_to_timer_num (TC_DAT_Type *pTimerCfg)
{
	S32 tnum = -1;

	if (pTimerCfg == (TC_DAT_Type *) &tcdat[0])
	{
		tnum = 0;
	}
	else if (pTimerCfg == (TC_DAT_Type *) &tcdat[1])
	{
		tnum = 1;
	}
	else if (pTimerCfg == (TC_DAT_Type *) &tcdat[2])
	{
		tnum = 2;
	}
	else if (pTimerCfg == (TC_DAT_Type *) &tcdat[3])
	{
		tnum = 3;
	}
	
	return tnum;
}


/***********************************************************************
 * Function: timer_delay_cmn
 * Purpose: Delay for a period of microseconds
 * Parameters:
 *     pTimer: Pointer to timer register set to use
 *     usec  :  the delay time in microseconds
 * Returns: None
 **********************************************************************/
void timer_delay_cmn(TC_Type *pTimer, U32 usec)
{
    S32 timernum = -1;

	/* Get clock number */
	timernum = timer_ptr_to_timer_num(pTimer);
	
	if (timernum < 0)
	{
		return;
	}
	
	/* Reset and stop timer */
	pTimer->TCR = TC_RESET;
	pTimer->CTCR = 0x00;

    /* Clear and enable match function */
    pTimer->IR = 0xFFFFFFFF;

    timernum = timer_ptr_to_timer_num (pTimer);
    
    pTimer->PR = (U32) timer_usec_to_val(timernum, 1);
    
    /* Set match for number of usecs */
	pTimer->MR[0] = usec;

    /* Interrupt on match 0 */
	pTimer->MCR = TC_INT_ON_MATCH(0);

	/* Enable the timer */
	pTimer->TCR = TC_ENABLE;

	/* Loop until match occurs */
	while ((pTimer->IR & TC_MATCH_INT(0)) == 0);

	/* Stop timer */
	pTimer->TCR = 0;
}


/***********************************************************************
 * Timer driver public functions
 **********************************************************************/

/***********************************************************************
 * Function: TC_Init
 * Purpose: Init Timer/Counter device
 * Parameters:
 *     TCNum: timer number (0..3)
 * Returns:
	Pointer to timer device if success or 0 if error
 **********************************************************************/
S32 TC_Init (S32 TCNum)
{
	TC_DAT_Type *pTimerDat;
	U8 temp;
	
	if (TCNum >= 0 && TCNum <= 3)
	{
		pTimerDat = (TC_DAT_Type *) &tcdat[TCNum];
	}
	else
	{
		return 0;
	}

	/* Has the timer been previously initialized? */
	if (pTimerDat->init == FALSE) 
	{
		// Set init flag
		pTimerDat->init = TRUE;
		
		/* Save timer register pointer 
		 * Setup clock and power for timer controller
		 * peripheral clock devider set to 4 from CCLK as default */
		switch (TCNum)
		{
		case 0:
			pTimerDat->regptr = TC0;
			SCB_EnablePPWR (PCTIM0);
			SCB_SetPCLKDiv (PCLK_TIMER0, CCLK_DIV_4);
			break;
			
		case 1:
			pTimerDat->regptr = TC1;
			SCB_EnablePPWR (PCTIM1);
			SCB_SetPCLKDiv (PCLK_TIMER1, CCLK_DIV_4);
			break;
			
		case 2:
			pTimerDat->regptr = TC2;
			SCB_EnablePPWR (PCTIM2);
			SCB_SetPCLKDiv (PCLK_TIMER2, CCLK_DIV_4);
			break;
			
		case 3:
			pTimerDat->regptr = TC3;
			SCB_EnablePPWR (PCTIM3);
			SCB_SetPCLKDiv (PCLK_TIMER3, CCLK_DIV_4);
			break;
		}

		/* Setup default timer state as standard timer mode, timer
		   disabled and all match and counters disabled */
		pTimerDat->regptr->TCR = 0;
		pTimerDat->regptr->CTCR = 0;
		pTimerDat->regptr->MCR = 0;
		pTimerDat->regptr->CCR = 0;
		pTimerDat->regptr->EMR = 0;

		/* Clear pending interrupts and reset counts */
		pTimerDat->regptr->TC = 0;
		pTimerDat->regptr->PR = 0;
		pTimerDat->regptr->PC = 0;
		
		// Clear match register
		for (temp = 0; temp < 4; temp++)
		{
			pTimerDat->regptr->MR [temp] = 0;
		}
		
		// Clear interrupt pending
		pTimerDat->regptr->IR = 0xFFFFFFFF;
	}

	return (S32) pTimerDat;
}


/***********************************************************************
 * Function: TC_Close
 * Purpose: Close Timer/Counter device
 * Parameters:
 *     TCdev: Pointer to timer device
 * Returns: status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_Close (S32 TCdev)
{
	S32 tnum;
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	// Reset init flag 
	pTimerDat->init = FALSE;
	
	// Turn off power and clock
	tnum = timer_cfg_to_timer_num (pTimerDat);
	
	// Disable timer/counter
	pTimerDat->regptr->TCR = 0x00;
	
	switch (tnum)
	{
	case 0:
		SCB_DisablePPWR (PCTIM0);
		break;
	case 1:
		SCB_DisablePPWR (PCTIM1);
		break;
	case 2:
		SCB_DisablePPWR (PCTIM2);
		break;
	case 3:
		SCB_DisablePPWR (PCTIM3);
		break;	
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_Start
 * Purpose: Start Timer/Counter device
 * Parameters:
 *     TCdev: Pointer to timer device
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_Start (S32 TCdev)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->TCR	|= TC_ENABLE;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_Stop
 * Purpose: Stop Timer/Counter device 
 * Parameters:
 *     TCdev: Pointer to timer device
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_Stop (S32 TCdev)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->TCR &= ~TC_ENABLE;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_Reset
 * Purpose: Reset count value in Timer/Counter device
 * Parameters:
 *     TCdev: Pointer to timer device
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_Reset (S32 TCdev)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->TCR |= TC_RESET;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_SetMode
 * Purpose: Set operation mode for Timer/Counter device
 * Parameters:
 *     	TCdev: Pointer to timer device
 * 		TCMode: Timer/counter mode
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_SetMode (S32 TCdev, S32 TCMode)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	switch ((TC_MODE_OPT) TCMode)
	{
	case TC_TIMER_MODE:
		pTimerDat->regptr->CTCR = 0x00;
		break;
		
	case TC_COUNTER_RISING_MODE:
		pTimerDat->regptr->CTCR = 0x01;
		break;
		
	case TC_COUNTER_FALLING_MODE:
		pTimerDat->regptr->CTCR = 0x02;
		break;
	case TC_COUNTER_ANY_MODE:
		pTimerDat->regptr->CTCR = 0x03;
		break;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_SetupTimer
 * Purpose: Setup Timer for Timer/Counter device
 * Parameters:
 *     	TCdev: Pointer to timer device
 * 		prescale_option: prescale option (tick value or microsecond value)
 * 		prescale: prescale value
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_SetupTimer (S32 TCdev, S32 prescale_option, U32 prescale)
{
	S32 tnum = -1;
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	switch ((TC_PRESCALE_OPT) prescale_option)
	{
	case TC_PRESCALE_TICKVAL:
		pTimerDat->regptr->PR = prescale - 1;
		break;
		
	case TC_PRESCALE_USVAL:
		tnum = timer_cfg_to_timer_num (pTimerDat);
		pTimerDat->regptr->PR = (timer_usec_to_val(tnum, prescale)) - 1;
		break;	
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_SetCounterInput
 * Purpose: Set Counter input source for Timer/Counter device
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		ICAPsource: capture input source for counter   
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_SetCounterInput (S32 TCdev, S32 ICAPsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->CTCR &= ~TC_CTCR_INPUT_MASK;
		
	switch ((TC_COUNTER_INPUT_OPT) ICAPsource)
	{
	case TC_COUNTER_INCAP0:
		// Do nothing
		break;
		
	case TC_COUNTER_INCAP1:
		pTimerDat->regptr->CTCR |= _BIT(2);
		break;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_ConfigExtMatch
 * Purpose: Configure External Match Output
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		matchsource: match source number 
 * 		ExtMatchtype: External output match type
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_ConfigExtMatch (S32 TCdev, U32 matchsource, S32 ExtMatchtype)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (matchsource > 3) 
	{
		return _ERROR;
	}
	
	// Clear external match bit field
	pTimerDat->regptr->EMR &= ~TC_EM_MASK(matchsource);
	
	switch ((TC_EXTMATCH_OPT) ExtMatchtype)
	{
	case TC_EXTMATCH_NOTHING:
		// Do nothing
		break;
		
	case TC_EXTMATCH_LOW:
		pTimerDat->regptr->EMR |= TC_EM_SET(matchsource, TC_EM_LOW);
		break;
		
	case TC_EXTMATCH_HIGH:
		pTimerDat->regptr->EMR |= TC_EM_SET(matchsource, TC_EM_HIGH);
		break;
		
	case TC_EXTMATCH_TOGGLE:
		pTimerDat->regptr->EMR |= TC_EM_SET(matchsource, TC_EM_TOGGLE);
		break;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_SetMatch
 * Purpose: Setup match value for each match channel 
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		matchsource: match source number      
 * 		matchvalue: match value to set
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_SetMatch (S32 TCdev, U32 matchsource, U32 matchvalue)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (matchsource > 3)
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->MR[matchsource] = matchvalue;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_EnableMatchInt
 * Purpose: Enable Timer/Counter match interrupt
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		matchsource: match source number      
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_EnableMatchInt (S32 TCdev, U32 matchsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (matchsource > 3) 
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->MCR |= TC_INT_ON_MATCH(matchsource);
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_DisableMatchInt
 * Purpose: Disable Timer/Counter match interrupt 
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		matchsource: match source number  
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_DisableMatchInt (S32 TCdev, U32 matchsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (matchsource > 3) 
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->MCR &= ~TC_INT_ON_MATCH(matchsource);
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_EnableMatchResetTC
 * Purpose: Enable Reset Timer/Counter when matching
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		matchsource: match source number  
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_EnableMatchReset (S32 TCdev, U32 matchsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (matchsource > 3) 
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->MCR |= TC_RESET_ON_MATCH(matchsource);
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_DisableMatchResetTC
 * Purpose: Disable Reset Timer/Counter when matching 
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		matchsource: match source number   
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_DisableMatchReset (S32 TCdev, U32 matchsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (matchsource > 3) 
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->MCR &= ~TC_RESET_ON_MATCH(matchsource);
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_EnableMatchStopTC
 * Purpose: Enable Stop Timer/Counter when matching
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		matchsource: match source number   
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_EnableMatchStop (S32 TCdev, U32 matchsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (matchsource > 3) 
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->MCR |= TC_STOP_ON_MATCH(matchsource);
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_DisableMatchStopTC
 * Purpose: Disable Stop Timer/Counter when matching
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		matchsource: match source number 
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_DisableMatchStop (S32 TCdev, U32 matchsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (matchsource > 3) 
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->MCR &= ~TC_STOP_ON_MATCH(matchsource);
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_ReadIntPending
 * Purpose: Read Pending Match Interrupt in Timer/Counter device
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * Returns: Value of timer/counter interrupt register
 **********************************************************************/
U32 TC_ReadIntPending (S32 TCdev)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return 0;
	}
	
	return pTimerDat->regptr->IR;
}


/***********************************************************************
 * Function: TC_ClrMatchIntPending
 * Purpose: Clear pending Timer/Counter match interrupt flag
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		matchsource: match source number 
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_ClrMatchIntPending (S32 TCdev, U32 matchsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (matchsource > 3) 
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->IR |= TC_MATCH_INT(matchsource);
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_EnableCapInt
 * Purpose: Enable Timer/Counter capture interrupt
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		capsource: capture source number 
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_EnableCapInt (S32 TCdev, U32 capsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (capsource > 3) 
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->CCR |= TC_INT_ON_CAP(capsource);
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_DisableCapInt
 * Purpose: Disable Timer/Counter capture interrupt
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		capsource: capture source number
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_DisableCapInt (S32 TCdev, U32 capsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (capsource > 3) 
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->CCR &= ~TC_INT_ON_CAP(capsource);
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_ConfigCapture
 * Purpose: Configure capture mode for each capture source 
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		capsource: capture source number
 * 		capmode: capture mode  
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_ConfigCapture (S32 TCdev, U32 capsource, S32 capmode)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (capsource > 3)
	{
		return _ERROR;
	}
	
	// Clear bitfield of capture mode for each capture source 
	pTimerDat->regptr->CCR &= ~TC_EDGE_MASK(capsource);
	
	switch ((TC_CAP_MODE_OPT) capmode)
	{
	case TC_CAPTURE_NONE:
		// Do nothing
		break;
		
	case TC_CAPTURE_RISING:
		pTimerDat->regptr->CCR |= TC_CAP_RISING(capsource);
		break;
		
	case TC_CAPTURE_FALLING:
		pTimerDat->regptr->CCR |= TC_CAP_FALLING(capsource);
		break;
		
	case TC_CAPTURE_ANY:
		pTimerDat->regptr->CCR |= ((TC_CAP_RISING(capsource)) | (TC_CAP_FALLING(capsource)));
		break;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_ClrCapIntPending
 * Purpose: Clear pending Timer/Counter capture interrupt flag
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		capsource: capture source number
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 TC_ClrCapIntPending (S32 TCdev, U32 capsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return _ERROR;
	}
	
	if (capsource > 3) 
	{
		return _ERROR;
	}
	
	pTimerDat->regptr->IR |= TC_CAP_INT(capsource);
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: TC_ReadCapture
 * Purpose: Read value of capture register in timer/counter device
 * Parameters:
 *     	TCdev: Pointer to timer/counter device
 * 		capsource: capture source number
 * Returns: Value of capture register
 **********************************************************************/
U32 TC_ReadCapture (S32 TCdev, U32 capsource)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return 0;
	}
	
	if (capsource > 3)
	{
		return 0;
	}
	
	return pTimerDat->regptr->CR[capsource];
}


/***********************************************************************
 * Function: TC_Waitms
 * Purpose: Delay for msec milliseconds (minimum)
 * Parameters:
 *     TCdev: Pointer to timer device
 *     msec  :  the delay time in milliseconds
 * Returns: Nothing
 **********************************************************************/
void TC_Waitms (S32 TCdev, U32 msec)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return;
	}
	
    timer_delay_cmn (pTimerDat->regptr, (msec * 1000));
}


/***********************************************************************
 * Function: TC_Waitus
 * Purpose: Delay for usec microseconds (minimum)
 * Parameters:
 *     TCdev: Pointer to timer device
 *     usec  :  the delay time in microseconds
 * Returns: Nothing
 **********************************************************************/
void TC_Waitus (S32 TCdev, U32 usec)
{
	TC_DAT_Type *pTimerDat = (TC_DAT_Type *) TCdev;
	
	if (pTimerDat->init == FALSE)
	{
		return;
	}
	
	timer_delay_cmn (pTimerDat->regptr, usec);
}

