/***********************************************************************
 * $Id: lpc23xx_rtc_driver.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx RTC driver
 *
 * Description:
 *     This file contains driver support for the LPC23xx RTC.
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

#include "lpc23xx_rtc_driver.h"
#include "lpc23xx_scb_driver.h"

/***********************************************************************
 * Private RTC driver data
***********************************************************************/
/* RTC device configuration structure type */
typedef struct
{
	RTC_Type *regptr;	/* pointer to RTC register */
	BOOL32 init;		/* Init flag for RTC device */
} RTC_CFG_T;

/* RTC driver data */
static RTC_CFG_T rtcdat;

/***********************************************************************
 * Public RTC driver data
***********************************************************************/


/***********************************************************************
 * Function: RTC_Init
 * Purpose: Initilize RTC controller
 * Parameters: None
 * Returns: Pointer to RTC device if success or 0 if error
 **********************************************************************/
S32 RTC_Init (void)
{
	S32 rtcptr = 0x00;

	/* Has the RTC been previously initialized? */
	if (rtcdat.init == FALSE) 
	{
		/* RTC is available */
		rtcdat.init = TRUE;
		rtcdat.regptr = RTC;
		
		// Enable power and clock for RTC module
		SCB_EnablePPWR (PCRTC);
		// Clock divider = 4 from CCLK as default
		SCB_SetPCLKDiv (PCLK_RTC, CCLK_DIV_4);
		
		// Clear all register to be default
		rtcdat.regptr->ILR = 0x07;
		rtcdat.regptr->CCR = 0x00;
		rtcdat.regptr->CIIR = 0x00;
		rtcdat.regptr->CISS = 0x00;
		rtcdat.regptr->AMR = 0xFFFFFFFF;
		
		/* Return pointer to specific RTC structure */
		rtcptr = (S32) (&rtcdat);
	}
	
	return rtcptr;
}


/***********************************************************************
 * Function: RTC_Close
 * Purpose: Close RTC device 
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_Close (S32 RTCdev)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	// Disable power and clock for RTC module
	SCB_DisablePPWR (PCRTC);
	
	pRTCdev->init = FALSE;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_SetClockSource
 * Purpose: Set Clock source for RTC device
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		clksrc: type of clock source
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_SetClockSource (S32 RTCdev, S32 clksrc)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	U32 sysclock = 0;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	if((RTC_CLKSRC_OPT) clksrc == RTC_CLKSRC_EXT32K)
	{
		rtcdat.regptr->CCR |= RTC_CCR_CLKSRC;
		pRTCdev->regptr->PREINT = 0x00;
		pRTCdev->regptr->PREFRAC = 0x00;
	}
	// If system clock is used for RTC module 
	else if ((RTC_CLKSRC_OPT) clksrc == RTC_CLKSRC_SYSCLK)
	{
		rtcdat.regptr->CCR &= ~RTC_CCR_CLKSRC;
		
		// Determine the value of prescale integer and fractional
		sysclock = (U32) SCB_GetPCLK (PCLK_RTC);
		pRTCdev->regptr->PREINT = ((S32)((sysclock / 32768) - 1)) & RTC_PREINT;
		pRTCdev->regptr->PREFRAC = ((U32) (sysclock - ((pRTCdev->regptr->PREINT + 1)
									* 32768))) & RTC_PREFRAC;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_Reset
 * Purpose: Reset clock tick counter in RTC device  
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_Reset (S32 RTCdev)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	U32 tmp = 0xFF;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	// reset counter value 
	pRTCdev->regptr->CCR |= RTC_CCR_CTCRST;
	while (tmp--);
	// release reset bit
	pRTCdev->regptr->CCR &= ~RTC_CCR_CTCRST;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_Start
 * Purpose: Start RTC device
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_Start (S32 RTCdev)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	pRTCdev->regptr->CCR |= RTC_CCR_CLKEN;
	pRTCdev->regptr->ILR = 0x07;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_Stop
 * Purpose: Stop RTC device
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_Stop (S32 RTCdev)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	pRTCdev->regptr->CCR &= ~RTC_CCR_CLKEN;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_GetTickCnt
 * Purpose: Get tick counter in RTC 
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * Returns: Value of current tick counter in RTC device
 **********************************************************************/
U32 RTC_GetTickCnt (S32 RTCdev)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	return (U32) RTC_CTCR_VALUE(pRTCdev->regptr->CTC);
}


/***********************************************************************
 * Function: RTC_EnableCII
 * Purpose:  Enable Counter increment interrupt flags bit
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		Timetype: Type of time in RTC
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_EnableCII (S32 RTCdev, S32 Timetype)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	switch ((RTC_TIMETYPE_OPT) Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		pRTCdev->regptr->CIIR |= RTC_CIIR_SECEN;
		break;
	case RTC_TIMETYPE_MINUTE:
		pRTCdev->regptr->CIIR |= RTC_CIIR_MINEN;
		break;
	case RTC_TIMETYPE_HOUR:
		pRTCdev->regptr->CIIR |= RTC_CIIR_HOUREN;
		break;
	case RTC_TIMETYPE_DAYOFWEEK:
		pRTCdev->regptr->CIIR |= RTC_CIIR_DOWEN;
		break;
	case RTC_TIMETYPE_DAYOFMONTH:
		pRTCdev->regptr->CIIR |= RTC_CIIR_DOMEN;
		break;
	case RTC_TIMETYPE_DAYOFYEAR:
		pRTCdev->regptr->CIIR |= RTC_CIIR_DOYEN;
		break;
	case RTC_TIMETYPE_MONTH:
		pRTCdev->regptr->CIIR |= RTC_CIIR_MONEN;
		break;
	case RTC_TIMETYPE_YEAR:
		pRTCdev->regptr->CIIR |= RTC_CIIR_YEAREN;
		break;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_DisableCII
 * Purpose: Disable Counter increment interrupt flags bit 
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		Timetype: Type of time in RTC
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_DisableCII (S32 RTCdev, S32 Timetype)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	switch ((RTC_TIMETYPE_OPT) Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		pRTCdev->regptr->CIIR &= ~RTC_CIIR_SECEN;
		break;
	case RTC_TIMETYPE_MINUTE:
		pRTCdev->regptr->CIIR &= ~RTC_CIIR_MINEN;
		break;
	case RTC_TIMETYPE_HOUR:
		pRTCdev->regptr->CIIR &= ~RTC_CIIR_HOUREN;
		break;
	case RTC_TIMETYPE_DAYOFWEEK:
		pRTCdev->regptr->CIIR &= ~RTC_CIIR_DOWEN;
		break;
	case RTC_TIMETYPE_DAYOFMONTH:
		pRTCdev->regptr->CIIR &= ~RTC_CIIR_DOMEN;
		break;
	case RTC_TIMETYPE_DAYOFYEAR:
		pRTCdev->regptr->CIIR &= ~RTC_CIIR_DOYEN;
		break;
	case RTC_TIMETYPE_MONTH:
		pRTCdev->regptr->CIIR &= ~RTC_CIIR_MONEN;
		break;
	case RTC_TIMETYPE_YEAR:
		pRTCdev->regptr->CIIR &= ~RTC_CIIR_YEAREN;
		break;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_EnableALI
 * Purpose: Enable Alarm interrupt flag
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		Timetype: Type of time in RTC
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_EnableALI (S32 RTCdev, S32 Timetype)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	switch ((RTC_TIMETYPE_OPT) Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		pRTCdev->regptr->AMR &= ~RTC_AMR_SECEN;
		break;
	case RTC_TIMETYPE_MINUTE:
		pRTCdev->regptr->AMR &= ~RTC_AMR_MINEN;
		break;
	case RTC_TIMETYPE_HOUR:
		pRTCdev->regptr->AMR &= ~RTC_AMR_HOUREN;
		break;
	case RTC_TIMETYPE_DAYOFWEEK:
		pRTCdev->regptr->AMR &= ~RTC_AMR_DOWEN;
		break;
	case RTC_TIMETYPE_DAYOFMONTH:
		pRTCdev->regptr->AMR &= ~RTC_AMR_DOMEN;
		break;
	case RTC_TIMETYPE_DAYOFYEAR:
		pRTCdev->regptr->AMR &= ~RTC_AMR_DOYEN;
		break;
	case RTC_TIMETYPE_MONTH:
		pRTCdev->regptr->AMR &= ~RTC_AMR_MONEN;
		break;
	case RTC_TIMETYPE_YEAR:
		pRTCdev->regptr->AMR &= ~RTC_AMR_YEAREN;
		break;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_DisableALI
 * Purpose: Disable Alarm interrupt flag 
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		Timetype: Type of time in RTC
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_DisableALI (S32 RTCdev, S32 Timetype)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	switch ((RTC_TIMETYPE_OPT) Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		pRTCdev->regptr->AMR |= RTC_AMR_SECEN;
		break;
	case RTC_TIMETYPE_MINUTE:
		pRTCdev->regptr->AMR |= RTC_AMR_MINEN;
		break;
	case RTC_TIMETYPE_HOUR:
		pRTCdev->regptr->AMR |= RTC_AMR_HOUREN;
		break;
	case RTC_TIMETYPE_DAYOFWEEK:
		pRTCdev->regptr->AMR |= RTC_AMR_DOWEN;
		break;
	case RTC_TIMETYPE_DAYOFMONTH:
		pRTCdev->regptr->AMR |= RTC_AMR_DOMEN;
		break;
	case RTC_TIMETYPE_DAYOFYEAR:
		pRTCdev->regptr->AMR |= RTC_AMR_DOYEN;
		break;
	case RTC_TIMETYPE_MONTH:
		pRTCdev->regptr->AMR |= RTC_AMR_MONEN;
		break;
	case RTC_TIMETYPE_YEAR:
		pRTCdev->regptr->AMR |= RTC_AMR_YEAREN;
		break;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_EnableSubSecSel
 * Purpose: Enable SubSecSelSub-Second Select for interrupt
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		SSStype: SubSecSel type
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_EnableSubSecSel (S32 RTCdev, U32 SSStype)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	// Clear bit field
	pRTCdev->regptr->CISS &= ~(_SBF(2, 0x07));
	// Set bit field according
	pRTCdev->regptr->CISS |= SSStype;
	
	pRTCdev->regptr->CISS |= _BIT(7);
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_DisableSubSecSel
 * Purpose: Disable SubSecSelSub-Second Select for interrupt
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_DisableSubSecSel (S32 RTCdev)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	pRTCdev->regptr->CISS |= ~(_BIT(7));
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_SetTime
 * Purpose: Set current time value for each type register
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		Timetype: Type of time in RTC
 * 		TimeValue: Time value to set
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_SetTime (S32 RTCdev, S32 Timetype, U32 TimeValue)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	switch ((RTC_TIMETYPE_OPT) Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		pRTCdev->regptr->SEC = TimeValue;
		break;
	case RTC_TIMETYPE_MINUTE:
		pRTCdev->regptr->MIN = TimeValue;
		break;
	case RTC_TIMETYPE_HOUR:
		pRTCdev->regptr->HOUR = TimeValue;
		break;
	case RTC_TIMETYPE_DAYOFWEEK:
		pRTCdev->regptr->DOW = TimeValue;
		break;
	case RTC_TIMETYPE_DAYOFMONTH:
		pRTCdev->regptr->DOM = TimeValue;
		break;
	case RTC_TIMETYPE_DAYOFYEAR:
		pRTCdev->regptr->DOY = TimeValue;
		break;
	case RTC_TIMETYPE_MONTH:
		pRTCdev->regptr->MONTH = TimeValue;
		break;
	case RTC_TIMETYPE_YEAR:
		pRTCdev->regptr->YEAR = TimeValue;
		break;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_GetTime
 * Purpose:  Get current time value for each type register 
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		Timetype: Type of time in RTC
 * Returns: Value of current time of each Timetype
 **********************************************************************/
U32 RTC_GetTime (S32 RTCdev, S32 Timetype)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return 0;
	}
	
	switch ((RTC_TIMETYPE_OPT) Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		return pRTCdev->regptr->SEC;
		break;
	case RTC_TIMETYPE_MINUTE:
		return pRTCdev->regptr->MIN;
		break;
	case RTC_TIMETYPE_HOUR:
		return pRTCdev->regptr->HOUR;
		break;
	case RTC_TIMETYPE_DAYOFWEEK:
		return pRTCdev->regptr->DOW;
		break;
	case RTC_TIMETYPE_DAYOFMONTH:
		return pRTCdev->regptr->DOM;
		break;
	case RTC_TIMETYPE_DAYOFYEAR:
		return pRTCdev->regptr->DOY;
		break;
	case RTC_TIMETYPE_MONTH:
		return pRTCdev->regptr->MONTH;
		break;
	case RTC_TIMETYPE_YEAR:
		return pRTCdev->regptr->YEAR;
		break;
	}
	
	return 0;
}


/***********************************************************************
 * Function: RTC_GetFullTime
 * Purpose: Get full of time
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		pFullTime: Pointer to variable (full-time type) to get time value
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_GetFullTime (S32 RTCdev, RTC_TIME_Type *pFullTime)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return 0;
	}
	
	pFullTime->DOM = pRTCdev->regptr->DOM;
	pFullTime->DOW = pRTCdev->regptr->DOW;
	pFullTime->DOY = pRTCdev->regptr->DOY;
	pFullTime->HOUR = pRTCdev->regptr->HOUR;
	pFullTime->MIN = pRTCdev->regptr->MIN;
	pFullTime->SEC = pRTCdev->regptr->SEC;
	pFullTime->MONTH = pRTCdev->regptr->MONTH;
	pFullTime->YEAR = pRTCdev->regptr->YEAR;
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_SetALTime
 * Purpose: Set alarm time value for each type register
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		Timetype: Type of time in RTC
 * 		ALValue: Alarm time value to set
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_SetALTime (S32 RTCdev, S32 Timetype, U32 ALValue)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	switch ((RTC_TIMETYPE_OPT) Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		pRTCdev->regptr->ALSEC = ALValue;
		break;
	case RTC_TIMETYPE_MINUTE:
		pRTCdev->regptr->ALMIN = ALValue;
		break;
	case RTC_TIMETYPE_HOUR:
		pRTCdev->regptr->ALHOUR = ALValue;
		break;
	case RTC_TIMETYPE_DAYOFWEEK:
		pRTCdev->regptr->ALDOW = ALValue;
		break;
	case RTC_TIMETYPE_DAYOFMONTH:
		pRTCdev->regptr->ALDOM = ALValue;
		break;
	case RTC_TIMETYPE_DAYOFYEAR:
		pRTCdev->regptr->ALDOY = ALValue;
		break;
	case RTC_TIMETYPE_MONTH:
		pRTCdev->regptr->ALMON = ALValue;
		break;
	case RTC_TIMETYPE_YEAR:
		pRTCdev->regptr->ALYEAR = ALValue;
		break;
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 * Function: RTC_GetALTime
 * Purpose: Set alarm time value for each type register 
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		Timetype: Type of time in RTC	
 * Returns: Value of current alarm time of each Timetype
 **********************************************************************/
U32 RTC_GetALTime (S32 RTCdev, S32 Timetype)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return 0;
	}
	
	switch ((RTC_TIMETYPE_OPT) Timetype)
	{
	case RTC_TIMETYPE_SECOND:
		return pRTCdev->regptr->ALSEC;
		break;
	case RTC_TIMETYPE_MINUTE:
		return pRTCdev->regptr->ALMIN;
		break;
	case RTC_TIMETYPE_HOUR:
		return pRTCdev->regptr->ALHOUR;
		break;
	case RTC_TIMETYPE_DAYOFWEEK:
		return pRTCdev->regptr->ALDOW;
		break;
	case RTC_TIMETYPE_DAYOFMONTH:
		return pRTCdev->regptr->ALDOM;
		break;
	case RTC_TIMETYPE_DAYOFYEAR:
		return pRTCdev->regptr->ALDOY;
		break;
	case RTC_TIMETYPE_MONTH:
		return pRTCdev->regptr->ALMON;
		break;
	case RTC_TIMETYPE_YEAR:
		return pRTCdev->regptr->ALYEAR;
		break;
	}
	
	return 0;
}


/***********************************************************************
 * Function: RTC_ReadIntPending
 * Purpose: Read the interrupt location register (ILR)
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * Returns: Value of pending interrupt flag in RTC 
 **********************************************************************/
U32 RTC_ReadIntPending (S32 RTCdev)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return 0x00;
	}
	
	return pRTCdev->regptr->ILR;
}


/***********************************************************************
 * Function: RTC_ClrIntPending
 * Purpose: Clear the interrupt location register (ILR)
 * Parameters: 
 * 		RTCdev: Pointer to RTC device
 * 		IntSrc: Value to clear interrupt source
 * Returns: Status of progress (_ERROR or _NO_ERROR)
 **********************************************************************/
S32 RTC_ClrIntPending (S32 RTCdev, U32 IntSrc)
{
	RTC_CFG_T *pRTCdev = (RTC_CFG_T *) RTCdev;
	
	if (pRTCdev->init == FALSE)
	{
		return _ERROR;
	}
	
	pRTCdev->regptr->ILR = IntSrc;
	
	return _NO_ERROR;
}
