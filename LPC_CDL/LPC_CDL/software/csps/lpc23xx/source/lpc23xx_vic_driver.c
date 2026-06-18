/***********************************************************************
* $Id: lpc23xx_vic_driver.c 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx VIC  driver
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: VIC
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
#include "lpc23xx_vic_driver.h"

/**********************************************************************
 * Vector interrupt controller driver public functions
 *********************************************************************/

/*********************************************************************
 * Function: VIC_Init
 * Purpose: Initilize and refresh VIC module
 * Parameters:
 *      None
 * Returns: 
 * 		None
 *********************************************************************/
void VIC_Init(void)
{
    U32 i = 0;
   	
    /* initialize VIC */
    VIC->INTENCLR = 0xFFFFFFFF;
    VIC->INTSELECT = 0;
    VIC->VECTADDR = 0;
    
    /* set all the vector and vector control register to 0 */
    for ( i = 0; i < VIC_SIZE; i++ )
    {
		VIC->VECTADDRn[i] = 0;
		VIC->VECTPRIORITYn[i] = 0x0F;
    }
  
    return;
}

/*********************************************************************
 * Function: VIC_InstallIRQ
 * Purpose: Install IRQ interrupt for interrupt source
 * Parameters:
 *      int_source: interrupt source to install IRQ interrupt
 * 		handler_addr: pointer to handler for interrupt source
 * 		priority: priority level of interrupt
 * Returns: 
 * 		Status of processing (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 VIC_InstallIRQ (U32 int_source, 
						void *handler_addr,
						U32 priority)
{        
    if (int_source >= VIC_SIZE)
    {
    	return _ERROR;
    }
    else
    {
    	/* First disable interrupt in source */
    	VIC->INTENCLR = _BIT(int_source);
    	VIC->VECTADDRn[int_source] = (S32) handler_addr;
    	VIC->VECTPRIORITYn[int_source] = priority; 
    }
    /* Use privated interrupt enable function, not used here */
    
    return _NO_ERROR;
}

/*********************************************************************
 * Function: VIC_UninstallIRQ
 * Purpose: Uninstall IRQ interrupt for interrupt source
 * Parameters:
 *      int_source: interrupt source to uninstall IRQ interrupt
 * Returns: 
 * 		Status of processing (_ERROR or _NO_ERROR)
 *********************************************************************/
S32 VIC_UninstallIRQ (U32 int_source)
{    
    if (int_source >= VIC_SIZE)
    {
    	return _ERROR;
    }
    else
    {
    	/* First disable interrupt in source */
    	VIC->INTENCLR = _BIT(int_source);
		/* clear the VIC entry in the VIC table */
		VIC->VECTADDRn[int_source] = (S32) NULL;
		/* Set priority to be the lowest */
	    VIC->VECTPRIORITYn[int_source] = 0x0F;
    }
    
    return _NO_ERROR;
}

/*********************************************************************
 * Function: VIC_EnableInt
 * Purpose: Enable interrupt for interrupt source
 * Parameters:
 *     int_source: interrupt source to enable interrupt
 * Returns: 
 * 		None
 *********************************************************************/
void VIC_EnableInt (U32 int_source)
{
	VIC->INTENABLE = _BIT(int_source);
}

/*********************************************************************
 * Function: VIC_DisableInt
 * Purpose: Disable interrupt for interrupt source
 * Parameters:
 *     int_source: interrupt source to disable interrupt
 * Returns: 
 * 		None
 *********************************************************************/
void VIC_DisableInt (U32 int_source)
{
	VIC->INTENCLR = _BIT(int_source);
}

/*********************************************************************
 * Function: VIC_GetIRQPending
 * Purpose: Get current IRQ interrupt pending status 
 * Parameters:
 *    	None
 * Returns: 
 * 		Value of IRQ Status register 
 *********************************************************************/
U32 VIC_GetIRQPending (void)
{
	return VIC->IRQSTATUS;
}

/*********************************************************************
 * Function: VIC_GetFIQPending
 * Purpose: Get current FIQ interrupt pending status
 * Parameters:
 *    	None
 * Returns: 
 * 		Value of FIQ Status register
 *********************************************************************/
U32 VIC_GetFIQPending (void)
{
	return VIC->FIQSTATUS;
}

/*********************************************************************
 * Function: VIC_SetSWInt
 * Purpose: Set Software interrupt
 * Parameters:
 *    	int_source: Software interrupt source 
 * Returns: 
 * 		None
 *********************************************************************/
void VIC_SetSWInt (U32 int_source)
{
	VIC->SOFTINT = _BIT(int_source);
}

/*********************************************************************
 * Function: VIC_ClrSWInt
 * Purpose: Clear Software interrupt
 * Parameters:
 *    	int_source: Software interrupt source
 * Returns: 
 * 		None
 *********************************************************************/
void VIC_ClrSWInt (U32 int_source)
{
	VIC->SOFTINTCLR = _BIT(int_source);
}

/*********************************************************************
 * Function: VIC_Ack
 * Purpose: Acknowledge interrupt by updating value VIC address register 
 * Parameters:
 *    	None
 * Returns:
 *  	None
 *********************************************************************/
void VIC_Ack(void)
{
	VIC->VECTADDR = 0x00;
}
