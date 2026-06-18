/***********************************************************************
 * ID: mcb2300_init.c 2008-08-13 HieuNguyen
 *
 * Project: MCB2300 board definitions (BSP)
 *
 * Description:
 *     This file contains driver support for the MCB2300 board.
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
#include "mcb2300_init.h"
#include "lpc23xx_pinsel_driver.h"
#include "lpc23xx_vic_driver.h"
#include "lpc23xx_scb_driver.h"
#include "lpc23xx_gpio_driver.h"

void mcb2300_init (void)
{	
	U8 i;
	
	SCB_SetupSYSCLK (12000000, 12, 1, 0x01, 6, 6);
	
	/* MAM fetch cycles are 3 CCLKs in duration */
	SCB_SetMAM (0x03);
	
	VIC_Init ();
	
	/* Configure pin connect block and GPIO, FIO */
	for (i = 0; i <= 10; i++)
	{
		PINSEL->PINSELn[i] = 0x00;
		PINSEL->PINMODEn[i] = 0x00;
	}

	GPIO0->IODIR = 0x00;
	GPIO1->IODIR = 0x00;
	GPIO0->IOSET = 0x00;
	GPIO1->IOSET = 0x00;
	
	FIO0->FIODIR = 0x00;
	FIO1->FIODIR = 0x00;
	FIO2->FIODIR = 0x00;
	FIO3->FIODIR = 0x00;
	FIO4->FIODIR = 0x00;
	
	FIO0->FIOSET = 0x00;
	FIO1->FIOSET = 0x00;
	FIO2->FIOSET = 0x00;
	FIO3->FIOSET = 0x00;
	FIO4->FIOSET = 0x00;
	
	return;
}
