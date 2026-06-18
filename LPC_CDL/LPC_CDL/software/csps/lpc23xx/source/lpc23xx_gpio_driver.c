/***********************************************************************
 * $Id: lpc23xx_gpio_driver.c 28 2014-01-27 14:45:13Z danielru $
 * 
 * Project: LPC23xx GPIO and FIO driver
 *
 * Description:
 *     This file contains driver support for the GPIO and FIO modules on the
 *     LPC23xx.
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
#include "lpc23xx_gpio_driver.h"

/**********************************************************************
 * GPIO driver public functions
 *********************************************************************/

/*********************************************************************
 * Function: GPIO_SetDirInput
 * Purpose: Set direction for GPIO port as input
 * Parameters:
 *     	GPIOportnum: port number to set direction (GPIO_0 or GPIO_1)
 * 		portval: value of port contains all pins to set direction
 * 			(0x00000000 --> 0xFFFFFFFF)
 * Returns: None
 *********************************************************************/
void GPIO_SetDirInput (U32 GPIOportnum, U32 portval)
{
	switch (GPIOportnum)
	{
	case GPIO_0:
		GPIO0->IODIR &= ~(portval);
		break;
	case GPIO_1:
		GPIO1->IODIR &= ~(portval);
		break;
	}
}


/*********************************************************************
 * Function: GPIO_SetDirOutput
 * Purpose: Set direction for GPIO port as output
 * Parameters:
 *     	GPIOportnum: port number to set direction (GPIO_0 or GPIO_1)
 * 		portval: value of port contains all pins to set direction
 * 			(0x00000000 --> 0xFFFFFFFF)
 * Returns: None
 *********************************************************************/
void GPIO_SetDirOutput (U32 GPIOportnum, U32 portval)
{
	switch (GPIOportnum)
	{
	case GPIO_0:
		GPIO0->IODIR |= portval;
		break;
	case GPIO_1:
		GPIO1->IODIR |= portval;
		break;
	}
}


/*********************************************************************
 * Function: GPIO_Set
 * Purpose: Set state for GPIO as high level
 * Parameters:
 *     	GPIOportnum: port number to set state (GPIO_0 or GPIO_1)
 * 		portval: value of port contains all pins to set state
 * 			(0x00000000 --> 0xFFFFFFFF)
 * Returns: None
 *********************************************************************/
void GPIO_Set (U32 GPIOportnum, U32 portval)
{
	switch (GPIOportnum)
	{
	case GPIO_0:
		GPIO0->IOSET = portval;
		break;
	case GPIO_1:
		GPIO1->IOSET = portval;
		break;
	}
}


/*********************************************************************
 * Function: GPIO_Clr
 * Purpose: Set state for GPIO as low level
 * Parameters:
 *     	GPIOportnum: port number to set state (GPIO_0 or GPIO_1)
 * 		portval: value of port contains all pins to set state
 * 			(0x00000000 --> 0xFFFFFFFF)
 * Returns: None
 *********************************************************************/
void GPIO_Clr (U32 GPIOportnum, U32 portval)
{
	switch (GPIOportnum)
	{
	case GPIO_0:
		GPIO0->IOCLR = portval;
		break;
	case GPIO_1:
		GPIO1->IOCLR = portval;
		break;
	}
}


/*********************************************************************
 * Function: GPIO_GetState
 * Purpose: Get state on GPIO
 * Parameters:
 *     	GPIOportnum: port number to get state (GPIO_0 or GPIO_1)
 * Returns: None
 *********************************************************************/
U32 GPIO_GetState (U32 GPIOportnum)
{
	switch (GPIOportnum)
	{
	case GPIO_0:
		return GPIO0->IOPIN;
		break;
	case GPIO_1:
		return GPIO1->IOPIN;
		break;
	}
	return 0;
}


/*********************************************************************
 * Function: FIO_SetDirInput32
 * Purpose: Set direction for FIO port as input
 * Parameters:
 *     	FIOportnum: port number to set direction
 * 		portval: value of port contains all pins to set direction
 * 			(0x00000000 --> 0xFFFFFFFF)
 * Returns: None
 *********************************************************************/
void FIO_SetDirInput32 (U32 FIOportnum, U32 portval)
{
	switch (FIOportnum)
	{
	case FIO_0:
		FIO0->FIODIR &= ~(portval);
		break;
	case FIO_1:
		FIO1->FIODIR &= ~(portval);
		break;
	case FIO_2:
		FIO2->FIODIR &= ~(portval);
		break;
	case FIO_3:
		FIO3->FIODIR &= ~(portval);
		break;
	case FIO_4:
		FIO4->FIODIR &= ~(portval);
		break;
	}
}


/*********************************************************************
 * Function: FIO_SetDirOutput32
 * Purpose: Set direction for FIO port as output
 * Parameters:
 *     	FIOportnum: port number to set direction
 * 		portval: value of port contains all pins to set direction
 * 			(0x00000000 --> 0xFFFFFFFF)
 * Returns: None
 *********************************************************************/
void FIO_SetDirOutput32 (U32 FIOportnum, U32 portval)
{
	switch (FIOportnum)
	{
	case FIO_0:
		FIO0->FIODIR |= portval;
		break;
	case FIO_1:
		FIO1->FIODIR |= portval;
		break;
	case FIO_2:
		FIO2->FIODIR |= portval;
		break;
	case FIO_3:
		FIO3->FIODIR |= portval;
		break;	
	case FIO_4:
		FIO4->FIODIR |= portval;
		break;	
	}
}


/*********************************************************************
 * Function: FIO_Set32
 * Purpose: Set state for FIO as high level
 * Parameters:
 *     	FIOportnum: port number to set state
 * 		portval: value of port contains all pins to set state
 * 			(0x00000000 --> 0xFFFFFFFF)
 * Returns: None
 *********************************************************************/
void FIO_Set32 (U32 FIOportnum, U32 portval)
{
	switch (FIOportnum)
	{
	case FIO_0:
		FIO0->FIOSET = portval;
		break;
	case FIO_1:
		FIO1->FIOSET = portval;
		break;
	case FIO_2:
		FIO2->FIOSET = portval;
		break;
	case FIO_3:
		FIO3->FIOSET = portval;
		break;
	case FIO_4:
		FIO4->FIOSET = portval;
		break;	
	}
}


/*********************************************************************
 * Function: FIO_Clr32
 * Purpose: Set state for FIO as low level
 * Parameters:
 *     	FIOportnum: port number to set state
 * 		portval: value of port contains all pins to set state
 * 			(0x00000000 --> 0xFFFFFFFF)
 * Returns: None
 *********************************************************************/
void FIO_Clr32 (U32 FIOportnum, U32 portval)
{
	switch (FIOportnum)
	{
	case FIO_0:
		FIO0->FIOCLR = portval;
		break;
	case FIO_1:
		FIO1->FIOCLR = portval;
		break;
	case FIO_2:
		FIO2->FIOCLR = portval;
		break;
	case FIO_3:
		FIO3->FIOCLR = portval;
		break;
	case FIO_4:
		FIO4->FIOCLR = portval;
		break;
	}
}


/*********************************************************************
 * Function: FIO_GetState32
 * Purpose: Get state on FIO
 * Parameters:
 *     	GPIOportnum: port number to get state
 * Returns: None
 *********************************************************************/
U32 FIO_GetState32 (U32 FIOportnum)
{
	switch (FIOportnum)
	{
	case FIO_0:
		return FIO0->FIOPIN;
		break;
	case FIO_1:
		return FIO1->FIOPIN;
		break;
	case FIO_2:
		return FIO2->FIOPIN;
		break;
	case FIO_3:
		return FIO3->FIOPIN;
		break;
	case FIO_4:
		return FIO4->FIOPIN;
		break;
	}
	return 0;
}


/*********************************************************************
 * Function: FIO_SetMask32
 * Purpose: Set mask for enhanced FIO port
 * Parameters:
 *     	FIOportnum: port number to set mask
 * 		portval: value of port contains all pins to set mask
 * 			(0x00000000 --> 0xFFFFFFFF)
 * Returns: None
 *********************************************************************/
void FIO_SetMask32 (U32 FIOportnum, U32 portval)
{
	switch (FIOportnum)
	{
	case FIO_0:
		FIO0->FIOMASK |= portval;
		break;
	case FIO_1:
		FIO1->FIOMASK |= portval;
		break;
	case FIO_2:
		FIO2->FIOMASK |= portval;
		break;
	case FIO_3:
		FIO3->FIOMASK |= portval;
		break;
	case FIO_4:
		FIO4->FIOMASK |= portval;
		break;
	}
}

/*********************************************************************
 * Function: FIO_ClrMask32
 * Purpose: Clear mask for enhanced FIO port
 * Parameters:
 *     	FIOportnum: port number to clear mask
 * 		portval: value of port contains all pins to clear mask
 * 			(0x00000000 --> 0xFFFFFFFF)
 * Returns: None
 *********************************************************************/
void FIO_ClrMask32 (U32 FIOportnum, U32 portval)
{
	switch (FIOportnum)
	{
	case FIO_0:
		FIO0->FIOMASK &= ~(portval);
		break;
	case FIO_1:
		FIO1->FIOMASK &= ~(portval);
		break;
	case FIO_2:
		FIO2->FIOMASK &= ~(portval);
		break;
	case FIO_3:
		FIO3->FIOMASK &= ~(portval);
		break;
	case FIO_4:
		FIO4->FIOMASK &= ~(portval);
		break;
	}
}



#if 0
/*********************************************************************
 *
 * Function: ioint_set_r
 *
 * Purpose: Set GPIO Interrupt Enable for Rising edge
 *
 * Processing:
 *     See the function below
 *
 * Parameters:
 *     	iointport: GPIO interrupt port (IO0INT or IO2INT)
 * 		pin: all pins used to set for rising interrupt (0 --> 0xFFFFFFFF)
 * 		activate: TRUE to enable or FALSE to disable
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 *********************************************************************/
void ioint_set_r (IOINT_PORT_T iointport, UNS_32 pin, BOOL_32 activate)
{
	switch (activate)
	{
	case TRUE:
		switch (iointport)
		{
		case IO0INT:
			IO0INT_CNTR->intenr |= pin;
			break;
		case IO2INT:
			IO2INT_CNTR->intenr |= pin;
			break;
		}
		break;
		
	case FALSE:
		switch (iointport)
		{
		case IO0INT:
			IO0INT_CNTR->intenr &= ~pin;
			break;
		case IO2INT:
			IO2INT_CNTR->intenr &= ~pin;
			break;
		}
		break;
	}
}

/*********************************************************************
 *
 * Function: ioint_set_f
 *
 * Purpose: Set GPIO Interrupt Enable for Falling edge
 *
 * Processing:
 *     See the function below
 *
 * Parameters:
 *     	iointport: GPIO interrupt port (IO0INT or IO2INT)
 * 		pin: all pins used to set for rising interrupt (0 --> 0xFFFFFFFF)
 * 		activate: TRUE to enable or FALSE to disable     	
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 *********************************************************************/
void ioint_set_f (IOINT_PORT_T iointport, UNS_32 pin, BOOL_32 activate)
{
	switch (activate)
	{
	case TRUE:
		switch (iointport)
		{
		case IO0INT:
			IO0INT_CNTR->intenf |= pin;
			break;
		case IO2INT:
			IO2INT_CNTR->intenf |= pin;
			break;
		}
		break;
		
	case FALSE:
		switch (iointport)
		{
		case IO0INT:
			IO0INT_CNTR->intenf &= ~pin;
			break;
		case IO2INT:
			IO2INT_CNTR->intenf &= ~pin;
			break;
		}
		break;
	}
}

/*********************************************************************
 *
 * Function: ioint_get_rsta
 *
 * Purpose: Get GPIO Interrupt Status for Rising edge
 *
 * Processing:
 *     See the function below
 *
 * Parameters:
 *     	iointport: GPIO interrupt port (IO0INT or IO2INT)
 * 
 * Outputs: None
 *
 * Returns: Interrupt Status for Rising edge
 *
 * Notes: None
 *
 *********************************************************************/
UNS_32 ioint_get_rsta (IOINT_PORT_T iointport)
{
	switch (iointport)
	{
	case IO0INT:
		return IO0INT_CNTR->intstatr;
		break;
	case IO2INT:
		return IO2INT_CNTR->intstatr;
		break;
	}
	return 0;
}

/*********************************************************************
 * Function: ioint_get_fsta
 *
 * Purpose: Get GPIO Interrupt Status for Falling edge
 *
 * Processing:
 *     See the function below
 *
 * Parameters:
 *     	iointport: GPIO interrupt port (IO0INT or IO2INT)
 * 
 * Outputs: None
 *
 * Returns: Interrupt Status for Falling edge
 *
 * Notes: None
 *
 *********************************************************************/
UNS_32 ioint_get_fsta (IOINT_PORT_T iointport)
{
	switch (iointport)
	{
	case IO0INT:
		return IO0INT_CNTR->intstatf;
		break;
	case IO2INT:
		return IO2INT_CNTR->intstatf;
		break;
	}
	return 0;
}

/*********************************************************************
 *
 * Function: ioint_clr
 *
 * Purpose: Clear GPIO interrupt
 *
 * Processing:
 *     See the function below
 *
 * Parameters:
 *     	iointport: GPIO interrupt port (IO0INT or IO2INT)
 * 		pin: all pins used to clear for any interrupt (0 --> 0xFFFFFFFF)
 * 
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 *********************************************************************/
void ioint_clr (IOINT_PORT_T iointport, UNS_32 pin)
{
	switch (iointport)
	{
	case IO0INT:
		IO0INT_CNTR->intclr |= pin;
		break;
	case IO2INT:
		IO2INT_CNTR->intclr |= pin;
		break;
	}
}
#endif
