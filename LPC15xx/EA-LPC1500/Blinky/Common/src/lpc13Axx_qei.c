/*****************************************************************************
 *   qei.c:  QEI module file for NXP LPC13Axx Family Microprocessors
 *
 *   Copyright(C) 2010, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2010.04.22  ver 1.00    Preliminary version, first Release
 *
 ****************************************************************************
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
 *****************************************************************************/
#include "LPC13Axx.h"                    		/* LPC13Axx definitions                */
#include "type.h"
#include "lpc13Axx_qei.h"
#include "lpc13Axx_gpio.h"

/*****************************************************************************
** Function name:		QEI_VelocityUpdated
**
** Descriptions:		returns 1 if velocity interrupt occured
**						
*****************************************************************************/
uint32_t QEI_VelocityUpdated(void)
{
	if(LPC_QEI->INTSTAT&(1<<1))	// bit1 : TIM_int, velocity timer overflow
	{
		LPC_QEI->CLR = (1<<1);	// Clear bit 1 : TIM_int, velocity timer overflow
		return 1;
	}
	return 0;
}

/*****************************************************************************
** Function name:		vQEI_Init
**
** Description:			Initialize QEI
**
*****************************************************************************/
void QEI_Init(void)
{
  /* Enable AHB clock to the QEI domain. */
  LPC_SYSCON->SYSAHBCLKCTRL1 |= EN1_QEI;
	
	/* Peripheral reset control to QEI. */
	LPC_SYSCON->PRESETCTRL1 |= EN1_QEI;
 	
	LPC_QEI->LOAD = QEI_LOAD; 						// Velocity timer match value
	LPC_QEI->CON  = 0x0F;             				// Reset index counter, velocity, reset on index
	LPC_QEI->CONF = QEI_CAPMODE | 					// BOTH PhA and PhB edges are counted (4X) 
					/* QEI_DIRINV | */ 				// Direction invert.
					QEI_CRESPI |					// Continuously reset position counter on index
					/*QEI_INVINX |*/ 				// Invert Index
					QEI_INXGATE(((1<<0)|(1<<1)|(1<<2)|(1<<3)));		// Pass all index 
  LPC_QEI->MAXPOS = QEI_MAX_POS;         			// Set QEI max pos
	LPC_QEI->FILTERPHA = 0;						// Set number of samples for valid value
	LPC_QEI->FILTERPHB = 0;						// Set number of samples for valid value
	LPC_QEI->FILTERINX = 0;						// Set number of samples for valid value
  LPC_QEI->IEC  = 0x000FFFF;        				// Disable all QEI interrupts
  LPC_QEI->CLR  = 0x000FFFF;        				// Clear all QEI pending interrupts   
    
	LPC_QEI->INXCMP0 = 4;
  LPC_QEI->INXCMP1 = 6;
	LPC_QEI->INXCMP2 = 8;
	LPC_QEI->CMPOS0 = 500;
	LPC_QEI->CMPOS1 = 1000;
	LPC_QEI->CMPOS2 = 1500;
//	LPC_QEI->VELCOMP = 50;

	/* Interrupt enable */
	//LPC_QEI->IES = MAXPOS_Int;
	//LPC_QEI->IES = REV2_Int;
	//LPC_QEI->IES = REV1_Int;
	LPC_QEI->IES = POS2REV_Int;
	LPC_QEI->IES = POS1REV_Int;
	LPC_QEI->IES = POS0REV_Int;
	//LPC_QEI->IES = REV0_Int;
	//LPC_QEI->IES = POS2_Int;
	//LPC_QEI->IES = POS1_Int;
	//LPC_QEI->IES = POS0_Int;
	//LPC_QEI->IES = ENCLK_Int;
	//LPC_QEI->IES = ERR_Int;
	//LPC_QEI->IES = DIR_Int;
	//LPC_QEI->IES = VELC_Int;
	//LPC_QEI->IES = TIM_Int;
	//LPC_QEI->IES = INX_Int;
	return;     
}

/*****************************************************************************
** Function name:		QEI_IRQHandler
**
** Descriptions:		Handles QEI interrupt
**
*****************************************************************************/
void QEI_IRQHandler(void)
{
	uint32_t regVal;
	
  regVal = LPC_QEI->INTSTAT;
	LPC_QEI->CLR |= regVal;          		// clear QEI interrupt sources

//	if(regVal&MAXPOS_Int)					// Indicates that the current position count goes through the MAXPOS value to zero in
//	{										// forward direction, or through zero to MAXPOS in backward direction.
//		GPIOSetBitValue( 1, 4, 1 );	
//	}
//	if(regVal&REV2_Int)						//Indicates that the index 2 compare value is equal to the current index count.
//	{
//		GPIOSetBitValue( 1, 7, 1 );		
//	}
//	if(regVal&REV1_Int)						//Indicates that the index 1 compare value is equal to the current index count
//	{
//		GPIOSetBitValue( 1, 6, 1 );	
//	}
 	if(regVal&POS2REV_Int)					// Combined position 2 and revolution count interrupt. Set when
	{										// both the POS2_Int bit is set and the REV_Int is set.
		GPIOSetBitValue( 1, 7, 1 );			
	}										
	if(regVal&POS1REV_Int)					// Combined position 1 and revolution count interrupt. Set when
	{										// both the POS1_Int bit is set and the REV_Int is set.
		GPIOSetBitValue( 1, 6, 1 );			
	}
	if(regVal&POS0REV_Int)					// Combined position 0 and revolution count interrupt. Set when
	{										// both the POS0_Int bit is set and the REV_Int is set.
		GPIOSetBitValue( 1, 5, 1 );		
	}
//	if(regVal&REV0_Int)						// Indicates that the index compare value is equal to the current index count
//	{
//		GPIOSetBitValue( 1, 5, 1 );	
//	}
//	if(regVal&POS2_Int)						// Indicates that the position 2 compare value is equal to the current position
//	{
//		GPIOSetBitValue( 1, 5, 1 );	
//	}
//	if(regVal&POS1_Int)						// Indicates that the position 1 compare value is equal to the current position
//	{
//		GPIOSetBitValue( 1, 6, 1 );
//	}	
//	if(regVal&POS0_Int)						// Indicates that the position 0 compare value is equal to the current position
//	{
//		GPIOSetBitValue( 1, 7, 1 );
//	}
// 	if(regVal&ENCLK_Int)					// Indicates that and encoder clock pulse was detected
//	{
//		GPIOSetBitValue( 1, 7, 0 );	
//	}
//	if(regVal&ERR_Int)						// Indicates that an encoder phase error was detected
//	{
//		GPIOSetBitValue( 1, 7, 1 );	
//	}
//	if(regVal&DIR_Int)						// Indicates that a change of direction was detected
//	{
//		GPIOSetBitValue( 1, 7, 0 );	
//	}
// 	if(regVal&VELC_Int)						// Indicates that captured velocity is less than compare velocity
//	{
//		GPIOSetBitValue( 1, 5, 0 );		
//	}
//	if(regVal&TIM_Int)						// Indicates that a velocity timer overflow occured
//	{
//		GPIOSetBitValue( 1, 7, 0 );	
//	}
// 	if(regVal&INX_Int)					 	// Indicates that an index pulse was detected.
//	{
//		GPIOSetBitValue( 1, 5, 0 );
//		GPIOSetBitValue( 1, 6, 0 );
//		GPIOSetBitValue( 1, 7, 0 );
//	}
    return;
}
/*****************************************************************************
** Function name:		QEI_SetDirection
**
** Descriptions:		Set QEI direction
**						      0 = not inverted
**						      1 = inverted
**
*****************************************************************************/
void QEI_SetDirection(uint8_t dir)
{
  if ( dir )
    LPC_QEI->CONF |= QEI_DIRINV;
  else 
    LPC_QEI->CONF &= ~QEI_DIRINV;
  return;	
}

