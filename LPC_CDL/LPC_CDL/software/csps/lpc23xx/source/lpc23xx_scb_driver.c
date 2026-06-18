/***********************************************************************
* $Id: lpc23xx_scb_driver.c 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx SCB driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: SCB
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

#include "lpc23xx_scb_driver.h"


/***********************************************************************
 *  System control block  driver private data and functions
 **********************************************************************/

/* Data for Current FIN (FOSC) value */
STATIC U32 current_fosc = 0;

/* Structure used for PLL setting argument */
typedef struct {
	U32 fosc;  			/* Oscillators value */
	U32 msel; 			/* PLL multiplier value, the value stored 
							here is (msel - 1) = MSEL.
	At higher oscillator frequencies, in the MHz range, values of msel from 6 
	through 512 are allowed. This supports the entire useful range of both 
	the main oscillator and the IRC. For lower frequencies, specifically 
	when the RTC is used to clock the PLL, a set of 65 additional msel values 
	have been selected for supporting baud rate generation, CAN/USB operation,
	and attaining even MHz frequencies. These values are shown in table 4-29
	of user_manual_lpc23xx */
	U32 nsel; 			/* PLL divider value , the value stored 
							here is (nsel - 1) = NSEL, 
							must be an integer from 1 - 32 */
	U32 clk_src_sel;		/* Clock source selection */
	U32 cclk_div;		/* CPU clock divider value, the value stored here is 
							(cclk_div - 1) = CCLKSEL,
	Only 0 and odd values (1, 3, 5, ..., 255) are supported and can be
	used when programming the CCLKSEL bits */
	U32 usbclk_div;		/* USB clock divider, the value stored here is 
	 						(usbclk_div - 1) = USBSEL */
} PLL_CFG_T;


/***********************************************************************
 *
 * Function: SYSCLK_CheckParam
 *
 * Purpose: Check if PLL setup parameter valid
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     pllptr : Pointer to PLL setup structure
 *
 * Outputs: None
 *
 * Returns: Status of checking (_ERROR or _NO_ERROR)
 *
 * Notes: 
 * 		PLL parameter determination can be simplified by using
 * a spreadsheet available from NXP
 *
 **********************************************************************/
S32 SYSCLK_CheckParam (PLL_CFG_T *pllptr)
{
	U64 t_cco;
	U32 t_m, t_n, t_osc, t_cclk, t_usb_mul, t_cclk_div, t_usb_div;
	t_m = pllptr->msel;
	t_n = pllptr->nsel;
	t_osc = pllptr->fosc;
	t_cclk_div = pllptr->cclk_div;
	t_usb_div = pllptr->usbclk_div;
	
	/* Check clock source with its value are valid */
	switch (pllptr->clk_src_sel)
	{
	/* Check internal RC */
	case CLKSRCSEL_IRC:
		if ((pllptr->usbclk_div != 0) || (t_osc != 4000000))
		{
			return _ERROR;
		}
		break;
		
	/* Check main OSC */
	case CLKSRCSEL_MAINOSC:
		if ((t_osc < 1000000) || (t_osc > 24000000))
		{
			return _ERROR;
		}
		break;
		
	/* Check RTC OSC */
	case CLKSRCSEL_RTC:
		if (t_osc != 32768)
		{
			return _ERROR;
		}
		break;
	default:
		break;
	}
	
	/* Check Fcco */
	t_cco = (U64) ((t_osc * t_m * 2) / t_n);
	if ((t_cco < 275000000) || (t_cco > 550000000))
	{
		return _ERROR;
	}

	
	/* Check Fcclk */
	t_cclk = t_cco / t_cclk_div;
	if (t_cclk <= 72000000)
	{
		if ((pllptr->usbclk_div != 0) && (t_cclk < 18000000))
		{
			return _ERROR;
		}
	}
	else
	{
		return _ERROR;
	}

	
	/* Check USB clock divider if using */
	if (pllptr->usbclk_div != 0)
	{
		t_usb_mul = 48000000 * t_usb_div;
		if (t_usb_mul != t_cco)
		{
			return _ERROR;
		}
	}
	
	return _NO_ERROR;
}


/***********************************************************************
 *
 * Function: SCB_GetPCLKSEL
 *
 * Purpose: Get current value of each Peripheral Clock Selection
 *
 * Processing:
 *     	See function.
 *
 * Parameters:
 *     	clktype: Peripheral Clock Selection value
 *
 * Outputs: None
 *
 * Returns: Value of Selected Peripheral Clock Selection 
 *
 * Notes: 
 *
 **********************************************************************/
U32 SCB_GetPCLKSEL (U32 clktype)
{
	U32 bitpos, retval;
	
	if (clktype < 32)
	{
		bitpos = clktype;
		retval = SCB->PCLKSEL0;
		retval = PCLK_PCLKSEL_GET(bitpos, retval);
	}
	else
	{
		bitpos = clktype - 32;
		retval = SCB->PCLKSEL1;
		retval = PCLK_PCLKSEL_GET(bitpos, retval);
	}
	return retval;
}




/***********************************************************************
 *  System control block  driver public functions
 **********************************************************************/

/***********************************************************************
 *
 * Function: SCB_ReadEXTINTPending
 *
 * Purpose: Read status of external interrupt pending flag source
 *
 * Processing:
 * 		See the function below.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Status of external interrupt flag register.
 *
 * Notes: None
 *
 **********************************************************************/
U32 SCB_ReadEXTINTPending(void)
{
	return SCB->EXTINT;	
}

/***********************************************************************
 *
 * Function: SCB_ClrEXTINTPending
 *
 * Purpose: Clear external interrupt flag source
 *
 * Processing:
 * 		See the function below.
 *
 * Parameters:
 *     source: a number contains external interrupt sources (bit 0..3) 
 * 		to clear interrupt flag.
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void SCB_ClrEXTINTPending(U32 source)
{
	SCB->EXTINT |= source;
}

/***********************************************************************
 *
 * Function: SCB_SetEXTINTMode
 *
 * Purpose: Set up external interrupt mode
 *
 * Processing:
 * 		See the function below.
 *
 * Parameters:
 *     	source: a number contains external interrupt sources (bit 0..3) 
 * 		to set interrupt mode
 *		mode: external interrupt mode to select.
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void SCB_SetEXTINTMode(U32 source, U32 mode)
{	
	switch (mode)
	{
	case EXTINT_MODE_LEVEL:
		SCB->EXTMODE &= ~source;
		break;
	case EXTINT_MODE_EDGE:
		SCB->EXTMODE |= source;
		break;
	}
}

/***********************************************************************
 *
 * Function: SCB_SetEXTINTPolar
 *
 * Purpose: Setup external interrupt polar
 *
 * Processing:
 * 		See the function below.
 *
 * Parameters:
 *     	source: a number contains external interrupt sources (bit 0..3) 
 * 		to set interrupt polar
 *		polar: external interrupt polar to select.
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void SCB_SetEXTINTPolar(U32 source, U32 polar)
{
	switch (polar)
	{
	case EXTINT_POLAR_LOW_FALL:
		SCB->EXTPOLAR &= ~source;
		break;
	case EXTINT_POLAR_HIGH_RISE:
		SCB->EXTPOLAR |= source;
		break;
	}
}

/***********************************************************************
 *
 * Function: SCB_SetGPIOMode
 *
 * Purpose: Setup GPIO mode (GPIO or FIO mode)
 *
 * Processing:
 * 		See the function below.
 *
 * Parameters:
 *     	gpiomode: 0 if GPIO mode used - 1 if FIO mode used
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: 
 * 		Mode = 0: GPIO ports 0 and 1 are accessed via APB addresses in a 
 * fashion compatible with previous LPC2000 devices.
 * 		Mode = 1 High speed GPIO is enabled on ports 0 and 1, accessed 
 * via addresses in the on-chip memory range. This mode includes the port 
 * masking feature described in the GPIO chapter.
 **********************************************************************/
void SCB_SetGPIOMode (U32 gpiomode)
{
	if (gpiomode == 0)
	{
		SCB->SCS &= ~GPIOM;
	}
	else
	{
		SCB->SCS |= GPIOM;
	}
}

/***********************************************************************
 *
 * Function: SCB_SetMCIPWR
 *
 * Purpose: Set MCIPWR active level 
 *
 * Processing:
 * 		See the function below.
 *
 * Parameters:
 *     	activelevel: 0 The MCIPWR pin is low - 1 The MCIPWR pin is high.
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void SCB_SetMCIPWR (U32 activelevel)
{
	if (activelevel == 0)
	{
		SCB->SCS &= ~MCIPWRAL;
	}
	else
	{
		SCB->SCS |= MCIPWRAL;
	}
}


/***********************************************************************
 *
 * Function: SCB_SetMAM
 *
 * Purpose: Memmory acceleration module setup
 *
 * Processing:
 * 		See the function below.
 *
 * Parameters:
 *     cycle_val: Value of cycle to set to Memmory acceleration module
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void SCB_SetMAM(U32 cycle_val)
{
	SCB->MAMCR = MAM_MODE_DIS;
	SCB->MAMTIM = cycle_val;
	SCB->MAMCR = MAM_MODE_FULL;
}


/***********************************************************************
 *
 * Function: SCB_SetupSYSCLK
 *
 * Purpose: Setup system clock
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *    	Fosc : value of external OSC
 * 		MSel: value of MSEL
 * 		NSel: value of NSEL
 * 		ClkSrcSel: Clock source select value 
 * 		CCLKDiv: CPU clock divider from FCCO
 * 		USBCLKDiv: USB clock divider from FCCO
 *
 * Outputs: None
 *
 * Returns: 
 * 		_ERROR or _NO_ERROR
 *
 * Notes: 
 *
 **********************************************************************/
S32 SCB_SetupSYSCLK (U32 Fosc, U32 MSel, U32 NSel, 
					U32 ClkSrcSel, U32 CCLKDiv, U32 USBCLKDiv)
{
	U32 m, n;
	
	PLL_CFG_T checkval;
	checkval.fosc = Fosc;
	checkval.msel = MSel;
	checkval.nsel = NSel;
	checkval.clk_src_sel = ClkSrcSel;
	checkval.cclk_div = CCLKDiv;
	checkval.usbclk_div = USBCLKDiv;
	
	/* Check if PLL value is valid */
	if (SYSCLK_CheckParam(&checkval) == _NO_ERROR)
	{
		m = checkval.msel - 1;
		n = checkval.nsel - 1;
		
		if (SCB->PLLSTAT & PLL_STAT_PLLC)
		{
			/* Disconnect the PLL with one feed sequence 
			 * if PLL is already connected */
			SCB->PLLCON = PLL_CON_ENABLE;
			SCB->PLLFEED = 0xAA;
			SCB->PLLFEED = 0x55;
		}
		/* Disable the PLL with one feed sequence */
		SCB->PLLCON = 0;
		SCB->PLLFEED = 0xAA;
		SCB->PLLFEED = 0x55;
		
		/* Enable main OSC */
		SCB->SCS |= OSCEN;
		/* Wait for The main oscillator is ready to be 
		 * used as a clock source */
		while (!(SCB->SCS & OSCSTAT));
		/* Select the clock source */
		SCB->CLKSRCSEL = checkval.clk_src_sel;
		
		/* Configure PLL value (multiplier and divider) */
		SCB->PLLCFG = (U32) (m | (n << 16));
		SCB->PLLFEED = 0xAA;
		SCB->PLLFEED = 0x55;
		
		/* Enable PLL, disconnect PLL */
		SCB->PLLCON = PLL_CON_ENABLE;
		SCB->PLLFEED = 0xAA;
		SCB->PLLFEED = 0x55;
		
		/* Configure CCLK */
		SCB->CCLKCFG = (checkval.cclk_div - 1);
		
		/* Configure USB clock if used */
		if (checkval.usbclk_div != 0)
		{
			SCB->USBCLKCFG = (checkval.usbclk_div - 1);
		}
		
		/* Wait for lock bit status */
		while (!(SCB->PLLSTAT & PLL_STAT_PLOCK));
		
		/* Enable and connect PLL */
		SCB->PLLCON = (PLL_CON_ENABLE | PLL_CON_CONNECT);
		SCB->PLLFEED = 0xAA;
		SCB->PLLFEED = 0x55;
		
		/* Wait for connect status bit */
		while (!(SCB->PLLSTAT & PLL_STAT_PLLC));
		
		current_fosc = checkval.fosc;
		
		return _NO_ERROR;
	}
	else
	{
		return _ERROR;
	}
}


/***********************************************************************
 *
 * Function: SCB_GetSYSCLK
 *
 * Purpose: Get current value of each system clock type
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     sysclktype : Type of system clock.
 *
 * Outputs: None
 *
 * Returns: Value of current selected system clock
 *
 * Notes: 
 *
 **********************************************************************/
S32 SCB_GetSYSCLK (U32 sysclktype)
{
	U64 retval = (U64) current_fosc;
	U32 regval, m_t, n_t;
	
	regval = SCB->PLLSTAT;
	m_t = PLL_MSEL_GET(regval) + 1;
	n_t = PLL_NSEL_GET(regval) + 1;
	
	switch (sysclktype)
	{
	case SYSCLK_FOSC:
		break;
	case SYSCLK_FCCO:
		retval = (U64)((retval * 2 * m_t) / n_t); 
		break;
		
	case SYSCLK_FCCLK:
		regval = SCB->CCLKCFG + 1;
		retval = (U64)((retval * 2 * m_t) / n_t);
		retval = (U64) (retval / regval);
		break;
	}
	
	return (U32) retval;
}


/***********************************************************************
 *
 * Function: SCB_SetPCLKDiv
 *
 * Purpose: Set value of each Peripheral Clock Selection 
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     	clktype: Peripheral Clock Selection of each type
 * 		divval: Value of divider
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: 
 *
 **********************************************************************/
void SCB_SetPCLKDiv (U32 clktype, U32 divval)
{
	U32 bitpos;

	if (clktype < 32)
	{
		bitpos = clktype;
	}
	else
	{
		bitpos = clktype - 32;
	}
	
	/* PCLKSEL0 selected */
	if (clktype < 32)
	{
		/* Clear two bit at bit position */
		SCB->PCLKSEL0 &= ~(PCLK_PCLKSEL_MASK(bitpos));
		
		/* Set two selected bit */
		SCB->PCLKSEL0 |= PCLK_PCLKSEL_SET(bitpos, divval);
	}
	/* PCLKSEL1 selected */
	else
	{
		/* Clear two bit at bit position */
		SCB->PCLKSEL1 &= ~(PCLK_PCLKSEL_MASK(bitpos));
		
		/* Set two selected bit */
		SCB->PCLKSEL1 |= PCLK_PCLKSEL_SET(bitpos, divval);
	}
}


/***********************************************************************
 *
 * Function: SCB_GetPCLK
 *
 * Purpose: Get current value of each Peripheral Clock  
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     	clktype: Peripheral Clock Selection of each type
 *
 * Outputs: None
 *
 * Returns: Value of Selected Peripheral Clock
 *
 * Notes: 
 *
 **********************************************************************/
U32 SCB_GetPCLK (U32 clktype)
{
	U32 retval, div;
	
	retval = SCB_GetSYSCLK(SYSCLK_FCCLK);
	div = SCB_GetPCLKSEL(clktype);
	
	switch (div)
	{
	case 0:
		div = 4;
		break;
		
	case 1:
		div = 1;
		break;
		
	case 2:
		div = 2;
		break;
		
	case 3:
		div = 8;
		break;
	}
	retval /= div;
	
	return retval;
}

/***********************************************************************
 *
 * Function: SCB_SetWakeUpSource
 *
 * Purpose: Set up wake-up source
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     	source: value that contains source to set wake-up source
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: 
 *
 **********************************************************************/
void SCB_SetWakeUpSource (U32 source)
{
	SCB->INTWAKE |= source;
}

/***********************************************************************
 *
 * Function: SCB_ClrWakeUpSource
 *
 * Purpose: Clear wake-up source
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     	source: value that contains source to clear wake-up source
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: 
 *
 **********************************************************************/
void SCB_ClrWakeUpSource (U32 source)
{
	SCB->INTWAKE &= ~source;
}

/***********************************************************************
 *
 * Function: SCB_InitPDOWN
 *
 * Purpose: Enter power down mode
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     	None
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: 
 *
 **********************************************************************/
void SCB_InitPDOWN(void)
{
	SCB->PCON |= PCON_PM1_PD;
}

/***********************************************************************
 *
 * Function: SCB_InitSLEEP
 *
 * Purpose: Enter sleep mode
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     	None
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: 
 *
 **********************************************************************/
void SCB_InitSLEEP (void)
{
	SCB->PCON |= (PCON_PM0_IDL | PCON_PM2);
}

/***********************************************************************
 *
 * Function: SCB_InitIDLE
 *
 * Purpose: Enter idle mode
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     	None
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: 
 *
 **********************************************************************/
void SCB_InitIDLE (void)
{
	SCB->PCON |= PCON_PM0_IDL;
}

/***********************************************************************
 *
 * Function: SCB_EnablePPWR
 *
 * Purpose: Enable power supply for each peripheral 
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     	ppwrsrc: type of peripheral used to enable power
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: 
 *
 **********************************************************************/
void SCB_EnablePPWR (U32 ppwrsrc)
{
	SCB->PCONP |= ppwrsrc;
}

/***********************************************************************
 *
 * Function: SCB_DisablePPWR
 *
 * Purpose: Disable power supply for each peripheral 
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     	ppwrsrc: type of peripheral used to disable power
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: 
 *
 **********************************************************************/
void SCB_DisablePPWR (U32 ppwrsrc)
{
	SCB->PCONP &= ~ppwrsrc;
}
