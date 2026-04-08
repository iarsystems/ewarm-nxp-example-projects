/****************************************************************************
 *   $Id: lpc13Axx_comp.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP lpc13Axx CMP example
 *
 *   Description:
 *     This file contains Comparator code example which include Comparator 
 *     initialization, COMP interrupt handler, and APIs.
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
****************************************************************************/
#include "lpc13Axx.h"			/* lpc13Axx Peripheral Registers */
#include "lpc13Axx_nmi.h"
#include "lpc13Axx_comp.h"

/* statistics of all the interrupts */
volatile uint32_t CompStatCnt[COMP_CH_NUM] = {0,0,0,0};

/*****************************************************************************
** Function name:		COMP_Handler
**
** Descriptions:		Comparator interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void COMP_Handler(uint32_t channel_num ) 
{
  uint32_t regVal;

  regVal = LPC_CMP->CMP[channel_num];
  if (regVal & CMP_INTFLAG)
  {
		CompStatCnt[channel_num]++;
		LPC_CMP->CMP[channel_num] |= CMP_INTFLAG;
  }
  return;
}

/*****************************************************************************
** Function name:		COMP0_IRQHandler
**
** Descriptions:		Comparator interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void COMP0_IRQHandler(void) 
{
  COMP_Handler( 0 );
  return;
}

/*****************************************************************************
** Function name:		COMP1_IRQHandler
**
** Descriptions:		Comparator interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void COMP1_IRQHandler(void) 
{
  COMP_Handler( 1 );
  return;
}

/*****************************************************************************
** Function name:		COMP2_IRQHandler
**
** Descriptions:		Comparator interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void COMP2_IRQHandler(void) 
{
  COMP_Handler( 2 );
  return;
}

/*****************************************************************************
** Function name:		COMP3_IRQHandler
**
** Descriptions:		Comparator interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void COMP3_IRQHandler(void) 
{
  COMP_Handler( 3 );
  return;
}

/*****************************************************************************
** Function name:		COMP_Init
**
** Descriptions:		Comparator initialization routine
**				
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void COMP_Init( void )
{
  uint32_t regVal;
   
  /* Comparator should be powered up first. Use of comparator requires BOD */
  LPC_SYSCON->PDRUNCFG &= ~( PDEN_COMP0 | PDEN_COMP1 | PDEN_COMP2 | PDEN_COMP3 | PDEN_BOD);
  /* Enable AHB clock to the Comparator. */
  LPC_SYSCON->SYSAHBCLKCTRL0 |= EN0_COMP;
  /* Peripheral reset control to Comparator, a "1" bring it out of reset. */
  LPC_SYSCON->PRESETCTRL0 |= EN0_COMP;

	/*connect the Comparator sigals to port pins(P0.0, P0.1, P0.7)*/
	LPC_IOCON->PIO0_25 &= ~(0x3 << 3);
	LPC_IOCON->PIO0_26 &= ~(0x3 << 3);
  LPC_SWM->PINENABLE0 &= ~(0xF << 25); /* P0.25~26 are COMP Common a and b, P0.27 is COMP_0a input; P0.28 is COMP_0b input */
  regVal = LPC_SWM->PINASSIGN12 & ~(0xFF << 8);
  LPC_SWM->PINASSIGN12 = regVal | (0x7 << 8);	/* P0.7 is comparator output */
  return;
}

/*****************************************************************************
** Function name:		COMP_SelectInput
**
** Descriptions:		Select comparator input channel
**
** parameters:			comp_channel, input
** Returned value:		None
** 
*****************************************************************************/
void COMP_SelectInput( uint32_t channel_num, uint32_t comp_channel, uint32_t input )
{
  switch ( comp_channel )
  {
    case COMP_VP:
      LPC_CMP->CMP[channel_num] &= ~(0x7<<8);
     LPC_CMP->CMP[channel_num] |= ((0x7 & input) << 8);
    break;
    case COMP_VM:
      LPC_CMP->CMP[channel_num] &= ~(0x7<<4);
      LPC_CMP->CMP[channel_num] |= ((0x7 & input) << 4);
    break;
    default:
    break;
  }
  return;
}

/*****************************************************************************
** Function name:		COMP_LadderControl
**
** Descriptions:		Laddrer control on the COMP channel
**                  ladder_ctrl 0 is disabled, 1 is enabled.
**                  0 <= ladder value <= 0x1F
**
** parameters:			channel num, ladder control, ladder value
** Returned value:		None
** 
*****************************************************************************/
void COMP_LadderControl( uint32_t channel_num, uint32_t ladder_ctrl, uint32_t ref_sel, uint32_t ladder_val )
{
	if ( ladder_ctrl == 0 )
  {
		LPC_CMP->CMP[channel_num] &= ~CMP_VLADEN(3);
		return;
	}
	else 
	{
		LPC_CMP->CMP[channel_num] |= CMP_VLADEN(3);
		if ( ref_sel == 0 )
			LPC_CMP->CMP[channel_num] &= ~CMP_VLADREF;
		else
			LPC_CMP->CMP[channel_num] |= CMP_VLADREF;
		LPC_CMP->CMP[channel_num] &= ~(0x1F<<24);
		LPC_CMP->CMP[channel_num] |= (ladder_val<<24);
  }
	return;
}

/*****************************************************************************
** Function name:		COMP_SetOutput
**
** Descriptions:		Set output async or sync
**
** parameters:			sync
** Returned value:		None
** 
*****************************************************************************/
void COMP_SetOutput( uint32_t channel_num, uint32_t sync )
{
  if ( sync == 0 )
  {
		LPC_CMP->CMP[channel_num] &= ~CMP_SYNC;
  }
  else
  {
		LPC_CMP->CMP[channel_num] |= CMP_SYNC;
  }
  return;
}

/*****************************************************************************
** Function name:		COMP_SetInterrupt
**
** Descriptions:		Set interrupt edge or level, falling or rising.
**            polarity: 0 is as-is, 1 is inverted 
**						int_type: 0 is edge, 1 is level. 
**						int_edge: 0 is falling, 1 is rising, 2 is both edge.
**
** parameters:			single, event
** Returned value:		None
** 
*****************************************************************************/
void COMP_SetInterrupt( uint32_t channel_num, uint32_t polarity, uint32_t int_type, uint32_t int_edge )
{
  if ( polarity == 0 ) {
		LPC_CMP->CMP[channel_num] &= ~CMP_INTPOL;
  }
  else {
		LPC_CMP->CMP[channel_num] |= CMP_INTPOL;
  }
  if ( int_type == 0 ) {
		LPC_CMP->CMP[channel_num] &= ~CMP_INTTYPE;
  }
  else {
		LPC_CMP->CMP[channel_num] |= CMP_INTTYPE;
  }
	/* Default is falling edge */
	LPC_CMP->CMP[channel_num] &= ~CMP_INTEDGE(3);
  if ( int_edge != 0x03 )
  {

		LPC_CMP->CMP[channel_num] |= CMP_INTEDGE(int_edge);
  }

  /* Enable the COMP Interrupt */
  NVIC_EnableIRQ( (IRQn_Type)(COMP0_IRQn+channel_num) );
  return;
}

/*****************************************************************************
** Function name:		COMP_SetHysteresis
**
** Descriptions:		Set hysteresis
**
** parameters:			level
** Returned value:		None
** 
*****************************************************************************/
void COMP_SetHysteresis( uint32_t channel_num, uint32_t level )
{
  LPC_CMP->CMP[channel_num] &= ~CMP_HYS(3);
  LPC_CMP->CMP[channel_num] |= CMP_HYS(level);
  return;
}

/*****************************************************************************
** Function name:		COMP_SetFilterDivider
**
** Descriptions:		Set Filter Divier
**
** parameters:			level
** Returned value:		None
** 
*****************************************************************************/
void COMP_SetFilterDivider( uint32_t channel_num, uint32_t clkdiv )
{
	/* Bit 2~4 is filter divider */
  LPC_CMP->CMPFILTR[channel_num] &= ~CMP_FILTER_CLKDIV(7);
  LPC_CMP->CMPFILTR[channel_num] |= CMP_FILTER_CLKDIV(clkdiv);
  return;
}

/*****************************************************************************
** Function name:		COMP_GetOutputStatus
**
** Descriptions:		Get output status
**
** parameters:			None
** Returned value:		Output status
** 
*****************************************************************************/
uint32_t COMP_GetOutputStatus( uint32_t channel_num )
{  
  return ( LPC_CMP->CMP[channel_num] & CMP_STAT );
}

/******************************************************************************
**                            End Of File
******************************************************************************/

