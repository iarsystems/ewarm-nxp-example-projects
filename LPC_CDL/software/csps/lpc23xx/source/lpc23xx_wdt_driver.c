/***********************************************************************
 * $Id: lpc23xx_wdt_driver.c 28 2014-01-27 14:45:13Z danielru $
 *
 * Project: LPC23xx WDT driver
 *
 * Description:
 *     This file contains driver support for the LPC23xx WDT.
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

#include "lpc23xx_wdt_driver.h"
#include "lpc23xx_scb_driver.h"

/***********************************************************************
 * WDT driver package data
***********************************************************************/

/* WDT device configuration structure type */
typedef struct
{
	WDT_Type *regptr;     	/* Pointer to WDT registers */
    BOOL32	 init;          /* Device initialized flag */
} WDT_CFG_T;

/* WDT driver data */
static WDT_CFG_T wdtdat;

/***********************************************************************
 * WDT private API function
***********************************************************************/
/* Feed consequense */
void WDT_Feed (void)
{
    wdtdat.regptr->FEED = 0xAA;
    wdtdat.regptr->FEED = 0x55;
}

S32 WDT_SetTimeOut (S32 ClkSrc, U32 TimeOut)
{
	U32 pclk_wdt = 0;
	U32 tempval = 0;
	
	switch ((WDT_CLK_OPT) ClkSrc)
    {
    case WDT_CLKSRC_IRC:
    	pclk_wdt = 4000000;
    	// Calculate TC in WDT
    	tempval  = (((pclk_wdt) / 1000000) * (TimeOut / 4));
    	// Check if it valid
    	if ((tempval >= 0xFF) && (tempval <= 0xFFFFFFFF))
    	{
    		wdtdat.regptr->TC = tempval;
    		return _NO_ERROR;
    	}
    	
    	break;
    	
    case WDT_CLKSRC_PCLK:
    	
    	// Get WDT clock with CCLK divider = 4
    	pclk_wdt = SCB_GetSYSCLK(SYSCLK_FCCLK) / 4;
    	// Calculate TC in WDT 
    	tempval  = (((pclk_wdt) / 1000000) * (TimeOut / 4));
    	
    	if ((tempval >= 0xFF) && (tempval <= 0xFFFFFFFF))
		{
			SCB_SetPCLKDiv (PCLK_WDT, CCLK_DIV_4);
			wdtdat.regptr->TC = (U32) tempval;
			return _NO_ERROR;
		}

    	// Get WDT clock with CCLK divider = 2
    	pclk_wdt = SCB_GetSYSCLK(SYSCLK_FCCLK) / 2;
    	// Calculate TC in WDT 
    	tempval  = (((pclk_wdt) / 1000000) * (TimeOut / 4));
    	
    	if ((tempval >= 0xFF) && (tempval <= 0xFFFFFFFF))
		{
			SCB_SetPCLKDiv (PCLK_WDT, CCLK_DIV_2);
			wdtdat.regptr->TC = (U32) tempval;
			return _NO_ERROR;
		}
    	
    	// Get WDT clock with CCLK divider = 1
    	pclk_wdt = SCB_GetSYSCLK(SYSCLK_FCCLK);
    	// Calculate TC in WDT 
    	tempval  = (((pclk_wdt) / 1000000) * (TimeOut / 4));
    	
    	if ((tempval >= 0xFF) && (tempval <= 0xFFFFFFFF))
		{
			SCB_SetPCLKDiv (PCLK_WDT, CCLK_DIV_1);
			wdtdat.regptr->TC = (U32) tempval;
			return _NO_ERROR;
		}
    	
    	break;
    	
    case WDT_CLKSRC_RTC:
    	pclk_wdt = 32768;
    	// Calculate TC in WDT 
    	tempval  = (((pclk_wdt) / 1000000) * (TimeOut / 4));
    	// Check if it valid
    	if ((tempval >= 0xFF) && (tempval <= 0xFFFFFFFF))
    	{
    		wdtdat.regptr->TC = (U32) tempval;
    		return _NO_ERROR;
    	}
    	
    	break;
    	
	// Error parameter
    default:
    	break;
    }
    
    return _ERROR;
}


/***********************************************************************
 * WDT public functions
***********************************************************************/

/**********************************************************************
 * Function: WDT_Init
 * Purpose: Init the WDT
 * Parameter:
 * 		ClkSrc: Clock source for WDT device
 * 		WDTMode: WDT mode
 * 		TimeOut: value of time-out for WDT (us)
 * Return: 
 * 		Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 WDT_Init (S32 ClkSrc, S32 WDTMode, U32 TimeOut)
{
    /* has the selected WDT been previously initialized? */
    if (wdtdat.init == FALSE)
    {
        /* Device not initialized and it is usable, so set it to used */
        wdtdat.init = TRUE;
        /* Save address of register block */
        wdtdat.regptr = WDT;
        
        /* Set default clock divider for WDT */
        SCB_SetPCLKDiv (PCLK_WDT, CCLK_DIV_4);
        
        // If timeout is ok
        if (WDT_SetTimeOut(ClkSrc, TimeOut) == _NO_ERROR)
        {
        	// Set clock source
            switch ((WDT_CLK_OPT) ClkSrc)
            {
            case WDT_CLKSRC_IRC:
            	wdtdat.regptr->CLKSEL = WDT_CLKSEL_IRC;
            	break;
            	
            case WDT_CLKSRC_PCLK:
            	wdtdat.regptr->CLKSEL = WDT_CLKSEL_PCLK;
            	break;
            	
            case WDT_CLKSRC_RTC:
            	wdtdat.regptr->CLKSEL = WDT_CLKSEL_RTC;
            	break;
            	
    		// unsupported
        	default:
        		return _ERROR;
            }
            
        	// Set Mode of WDT operation
        	switch ((WDT_MODE_OPT) WDTMode)
        	{
        	case WDT_MODE_INT_ONLY:
        		wdtdat.regptr->MOD = WDT_ENABLE;
        		break;

        	case WDT_MODE_RESET:
        		wdtdat.regptr->MOD = WDT_ENABLE | WDT_RESET;
        		break;
        		
    		// unsupported
        	default:
        		return _ERROR;
        	}
            
        	return _NO_ERROR;
        }
    }
    
    return _ERROR;
}


/**********************************************************************
 * Function: WDT_Start
 * Purpose: Start WDT by the first feeding 
 * Parameter: None
 * Return: Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 WDT_Start (void)
{
	if (wdtdat.init == FALSE)
	{
		return _ERROR;
	}
	
	WDT_Feed();
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: WDT_ReloadTimeOut
 * Purpose: Reload WDT with current timeout value by feeding
 * Parameter: None
 * Return: Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 WDT_ReloadTimeOut (void)
{
	if (wdtdat.init == FALSE)
	{
		return _ERROR;
	}
	
	WDT_Feed();
	
	return _NO_ERROR;
}


/**********************************************************************
 * Function: WDT_UpdateTimeOut
 * Purpose: Update timeout value for WDT
 * Parameter: 
 * 		TimeOut: TimeOut value (us)
 * Return: Status of progress (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 WDT_UpdateTimeOut (U32 TimeOut)
{
	S32 ClkSrc = -1;
	
	if (wdtdat.init == FALSE)
	{
		return _ERROR;
	}
	
	// Check clock source selection
	if ((wdtdat.regptr->CLKSEL & 0x03) == WDT_CLKSEL_IRC)
	{
		ClkSrc = WDT_CLKSRC_IRC;
	}
	else if ((wdtdat.regptr->CLKSEL & 0x03) == WDT_CLKSEL_PCLK)
	{
		ClkSrc = WDT_CLKSRC_PCLK;
	}
	else if ((wdtdat.regptr->CLKSEL & 0x03) == WDT_CLKSEL_RTC)
	{
		ClkSrc = WDT_CLKSRC_RTC;
	}
	else
	{
		return _ERROR;
	}
	
    if (WDT_SetTimeOut(ClkSrc, TimeOut) == _NO_ERROR)
    {
    	return _NO_ERROR;
    }
    
	return _ERROR;
}


/**********************************************************************
 * Function: WDT_ReadCnt
 * Purpose: Read Current Counter in WDT
 * Parameter: None
 * Return: Value of WDT counter if success or 0 if error
 *********************************************************************/
U32 WDT_ReadCnt (void)
{
	if (wdtdat.init == FALSE)
	{
		return 0;
	}
	
	return wdtdat.regptr->TV;
}


/**********************************************************************
 * Function: WDT_ReadFlagPending
 * Purpose: Read pending flag
 * Parameter: None
 * Return: Pending flag status
 *********************************************************************/
U32 WDT_ReadFlagPending (void)
{
	return (U32) (WDT->MOD & 0x0C);
}


/**********************************************************************
 * Function: WDT_ClrTimeOutFlag
 * Purpose: Clear TimeOut Flag
 * Parameter: None
 * Return: None
 *********************************************************************/
void WDT_ClrTimeOutFlag (void)
{
	WDT->MOD = WDT_TOF_FLAG;
}




