/****************************************************************************
 *   $Id: lpc13Axx_gpio.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP lpc13Axx GPIO example
 *
 *   Description:
 *     This file contains GPIO code example which include GPIO 
 *     initialization, GPIO interrupt handler, and related APIs for 
 *     GPIO access.
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
#include "LPC13Axx.h"			/* lpc13Axx Peripheral Registers */
#include "lpc13Axx_nmi.h"
#include "lpc13Axx_gpio.h"

volatile uint32_t pin_int_counter[INT_CHANNEL_NUM] = {0,0,0,0,0,0,0,0};
volatile uint32_t pin_int_level_counter[INT_CHANNEL_NUM] = {0,0,0,0,0,0,0,0};
volatile uint32_t pin_int_rising_edge_counter[INT_CHANNEL_NUM] = {0,0,0,0,0,0,0,0};
volatile uint32_t pin_int_falling_edge_counter[INT_CHANNEL_NUM] = {0,0,0,0,0,0,0,0};

volatile uint32_t gint0_counter = 0;
volatile uint32_t gint1_counter = 0;
volatile uint32_t gint0_level_counter = 0;
volatile uint32_t gint0_edge_counter = 0;
volatile uint32_t gint1_level_counter = 0;
volatile uint32_t gint1_edge_counter = 0;

/*****************************************************************************
** Function name:		PIN_INT_Handler
**
** Descriptions:		General call made by the handlers.
**
** parameters:			IRQ number(0~7)
** 						
** Returned value:	None
** 
*****************************************************************************/
void PIN_INT_Handler ( uint32_t irq_num )
{
	uint32_t condition, regVal;
	
	pin_int_counter[irq_num]++;
	if ( LPC_GPIO_PIN_INT->PMCTRL & SEL_PMATCH )
	{
		condition = ( LPC_GPIO_PIN_INT->PMCFG >> (8 + irq_num *3) ) & 0x7;
		switch ( condition )
		{
			case PATTERN_LEVEL_HI: 
			case PATTERN_LEVEL_LO:
				pin_int_level_counter[irq_num]++;
				break;
			case PATTERN_RISING:
			case PATTERN_FALLING:
			case PATTERN_R_OR_F:
				if ( LPC_GPIO_PIN_INT->RISE & (0x1<<irq_num) ) {
					LPC_GPIO_PIN_INT->RISE = 0x1<<irq_num;
					pin_int_rising_edge_counter[irq_num]++;
				}
				else {
					LPC_GPIO_PIN_INT->FALL = 0x1<<irq_num;
					pin_int_falling_edge_counter[irq_num]++;
				}
				regVal = LPC_GPIO_PIN_INT->PMSRC;
				LPC_GPIO_PIN_INT->PMSRC = regVal;
				break;
			case PATTERN_EVENT:
				/* Non-sticky, won't know if it's rising or falling, increment both. */
				pin_int_rising_edge_counter[irq_num]++;
				pin_int_falling_edge_counter[irq_num]++;
				break;
			default:
				break;
		}
		return;	
	}
	/* regular pin interrupt */
  if ( LPC_GPIO_PIN_INT->IST & (0x1<<irq_num) ) {
		if ( LPC_GPIO_PIN_INT->ISEL & (0x1<<irq_num) ) {
			pin_int_level_counter[irq_num]++;
		}
		else {
			if ( ( LPC_GPIO_PIN_INT->RISE & (0x1<<irq_num) ) && ( LPC_GPIO_PIN_INT->IENR & (0x1<<irq_num) ) ) {
				pin_int_rising_edge_counter[irq_num]++;
				LPC_GPIO_PIN_INT->RISE = 0x1<<irq_num;
			}
			if ( ( LPC_GPIO_PIN_INT->FALL & (0x1<<irq_num) ) && ( LPC_GPIO_PIN_INT->IENF & (0x1<<irq_num) ) ) {
				pin_int_falling_edge_counter[irq_num]++;
				LPC_GPIO_PIN_INT->FALL = 0x1<<irq_num;
			}
			LPC_GPIO_PIN_INT->IST = 0x1<<irq_num;
		}
  }		
}

/*****************************************************************************
** Function name:		PIN_INT0_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
** 						
** Returned value:		None
** 
*****************************************************************************/
void PIN_INT0_IRQHandler(void)
{
	PIN_INT_Handler ( 0 );
  return;
}

/*****************************************************************************
** Function name:		PIN_INT1_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
** 						
** Returned value:		None
** 
*****************************************************************************/
void PIN_INT1_IRQHandler(void)
{
	PIN_INT_Handler ( 1 );
  return;
}

/*****************************************************************************
** Function name:		PIN_INT2_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
** 						
** Returned value:		None
** 
*****************************************************************************/
void PIN_INT2_IRQHandler(void)
{
	PIN_INT_Handler ( 2 );
  return;
}

/*****************************************************************************
** Function name:		PIN_INT3_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
** 						
** Returned value:		None
** 
*****************************************************************************/
void PIN_INT3_IRQHandler(void)
{
	PIN_INT_Handler ( 3 );
  return;
}

/*****************************************************************************
** Function name:		PIN_INT4_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
** 						
** Returned value:		None
** 
*****************************************************************************/
void PIN_INT4_IRQHandler(void)
{
	PIN_INT_Handler ( 4 );
  return;
}

/*****************************************************************************
** Function name:		PIN_INT5_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
** 						
** Returned value:		None
** 
*****************************************************************************/
void PIN_INT5_IRQHandler(void)
{
	PIN_INT_Handler ( 5 );
  return;
}

/*****************************************************************************
** Function name:		PIN_INT6_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
** 						
** Returned value:		None
** 
*****************************************************************************/
void PIN_INT6_IRQHandler(void)
{
	PIN_INT_Handler ( 6 );
  return;
}

/*****************************************************************************
** Function name:		PIN_INT7_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
** 						
** Returned value:		None
** 
*****************************************************************************/
void PIN_INT7_IRQHandler(void)
{
	PIN_INT_Handler ( 7 );
  return;
}

/*****************************************************************************
** Function name:		GINT0_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
** 						
** Returned value:		None
** 
*****************************************************************************/
void GINT0_IRQHandler(void)
{
  gint0_counter++;
  if ( LPC_GPIO_GROUP_INT0->CTRL & (0x1<<0) ) {
    if ( LPC_GPIO_GROUP_INT0->CTRL & (0x1<<2) ) {
      gint0_level_counter++;
    }
    else {
      gint0_edge_counter++;
    }
    LPC_GPIO_GROUP_INT0->CTRL |= (0x1<<0);
  }		
  return;
}

/*****************************************************************************
** Function name:		GINT1_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
** 						
** Returned value:		None
** 
*****************************************************************************/
void GINT1_IRQHandler(void)
{
  gint1_counter++;
  if ( LPC_GPIO_GROUP_INT1->CTRL & (0x1<<0) ) {
    if ( LPC_GPIO_GROUP_INT1->CTRL & (0x1<<2) ) {
      gint1_level_counter++;
    }
    else {
      gint1_edge_counter++;
    }
    LPC_GPIO_GROUP_INT1->CTRL |= (0x1<<0);
  }		
  return;
}

/*****************************************************************************
** Function name:		GPIOInit
**
** Descriptions:		Initialize GPIO and GPIO INT block
**
** parameters:			None
** Returned value:	None
** 
*****************************************************************************/
void GPIOInit( void )
{
  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL0 |= (EN0_P_MUX | EN0_GPIO0 | EN0_GPIO1 | EN0_GPIO2 | EN0_GPIO_INT | EN0_GPIO_GINT);
	
	/* Peripheral reset control to GPIO and GPIO INT, a "1" bring it out of reset. */
	LPC_SYSCON->PRESETCTRL0 |= (EN0_P_MUX | EN0_GPIO0 | EN0_GPIO1 | EN0_GPIO2 | EN0_GPIO_INT | EN0_GPIO_GINT);
	LPC_SYSCON->PRESETCTRL0 &= ~((EN0_P_MUX | EN0_GPIO0 | EN0_GPIO1 | EN0_GPIO2 | EN0_GPIO_INT | EN0_GPIO_GINT));
  return;
}

/*****************************************************************************
** Function name:		GPIOSetPinInterrupt
**
** Descriptions:		Set interrupt sense, event, etc.
**						sense: edge or level, 0 is edge, 1 is level 
**						event/polarity: 0 is active low/falling, 1 is high/rising.
**
** parameters:			channel #, port #, bit position, sense, event(polarity)
** 						
** Returned value:		None
** 
*****************************************************************************/
void GPIOSetPinInterrupt( uint32_t channelNum, uint32_t portNum, uint32_t bitPosi,
		uint32_t sense, uint32_t event )
{
#if NMI_ENABLED
	NVIC_DisableIRQ( (IRQn_Type)(PIN_INT0_IRQn+channelNum) );
	NMI_Init( (IRQn_Type)(PIN_INT0_IRQn+channelNum) );
#else
	NVIC_EnableIRQ((IRQn_Type)(PIN_INT0_IRQn+channelNum));
#endif
  if ( portNum ) {
    LPC_PMUX->GPIO_INT_P_MUX[channelNum] = bitPosi + 24; 
  }
  else {
    LPC_PMUX->GPIO_INT_P_MUX[channelNum] = bitPosi; 
  }

  if ( sense == 0 ) {
		LPC_GPIO_PIN_INT->ISEL &= ~(0x1<<channelNum);	/* Edge trigger */
		if ( event == 0 ) {
			LPC_GPIO_PIN_INT->IENF |= (0x1<<channelNum);	/* faling edge */
		}
		else {
			LPC_GPIO_PIN_INT->IENR |= (0x1<<channelNum);	/* Rising edge */
		}
  }
  else
  {
		LPC_GPIO_PIN_INT->ISEL |= (0x1<<channelNum);	/* Level trigger. */
		LPC_GPIO_PIN_INT->IENR |= (0x1<<channelNum);	/* Level enable */
		if ( event == 0 ) {
			LPC_GPIO_PIN_INT->IENF &= ~(0x1<<channelNum);	/* active-low */
		}
		else {
			LPC_GPIO_PIN_INT->IENF |= (0x1<<channelNum);	/* active-high */
		}	
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOPinIntEnable
**
** Descriptions:		Enable Interrupt
**
** parameters:			channel num, event(0 is falling edge, 1 is rising edge)
** Returned value:		None
** 
*****************************************************************************/
void GPIOPinIntEnable( uint32_t channelNum, uint32_t event )
{
  if ( !( LPC_GPIO_PIN_INT->ISEL & (0x1<<channelNum) ) ) {
		if ( event == 0 ) {
			LPC_GPIO_PIN_INT->SIENF |= (0x1<<channelNum);	/* faling edge */
		}
		else {
			LPC_GPIO_PIN_INT->SIENR |= (0x1<<channelNum);	/* Rising edge */
		}
  }
  else {
		LPC_GPIO_PIN_INT->SIENR |= (0x1<<channelNum);	/* Level */
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOPinIntDisable
**
** Descriptions:		Disable Interrupt
**
** parameters:			channel num, event(0 is falling edge, 1 is rising edge)
** 						
** Returned value:		None
** 
*****************************************************************************/
void GPIOPinIntDisable( uint32_t channelNum, uint32_t event )
{
  if ( !( LPC_GPIO_PIN_INT->ISEL & (0x1<<channelNum) ) ) {
		if ( event == 0 ) {
			LPC_GPIO_PIN_INT->CIENF |= (0x1<<channelNum);	/* faling edge */
		}
		else {
			LPC_GPIO_PIN_INT->CIENR |= (0x1<<channelNum);	/* Rising edge */
		}
  }
  else {
		LPC_GPIO_PIN_INT->CIENR |= (0x1<<channelNum);	/* Level */
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOPinIntStatus
**
** Descriptions:		Get Interrupt status
**
** parameters:			channel num
** 						
** Returned value:		None
** 
*****************************************************************************/
uint32_t GPIOPinIntStatus( uint32_t channelNum )
{
  if ( LPC_GPIO_PIN_INT->IST & (0x1<<channelNum) ) {
		return( 1 );
  }
  else {
		return( 0 );
  }
}

/*****************************************************************************
** Function name:		GPIOPinIntClear
**
** Descriptions:		Clear Interrupt
**
** parameters:			channel num
** 						
** Returned value:		None
** 
*****************************************************************************/
void GPIOPinIntClear( uint32_t channelNum )
{
  if ( !( LPC_GPIO_PIN_INT->ISEL & (0x1<<channelNum) ) ) {
		LPC_GPIO_PIN_INT->IST = (1<<channelNum);
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOSetGroupedInterrupt
**
** Descriptions:		Set interrupt logic, sense, eventPattern, etc.
**						logic: AND or OR, 0 is OR, 1 is AND 
**						sensePattern: edge or level, 0 is edge, 1 is level 
**						event/polarity: 0 is active low/falling, 1 is high/rising.
**
** parameters:			group #, bit pattern, logic, sense, event(polarity) pattern
** 						
** Returned value:		None
** 
*****************************************************************************/
void GPIOSetGroupedInterrupt( uint32_t groupNum, uint32_t *bitPattern, uint32_t logic,
		uint32_t sense, uint32_t *eventPattern )
{
  switch ( groupNum )
  {
    case GROUP0:
      if ( sense == 0 ) {
        LPC_GPIO_GROUP_INT0->CTRL &= ~(0x1<<2);	/* Edge trigger */
      }
      else {
        LPC_GPIO_GROUP_INT0->CTRL |= (0x1<<2);	/* Level trigger. */
      }
      if ( logic == 0 ) {
        LPC_GPIO_GROUP_INT0->CTRL &= ~(0x1<<1);	/* OR */
      }
      else {
        LPC_GPIO_GROUP_INT0->CTRL |= (0x1<<1);	/* AND */
      }
      LPC_GPIO_GROUP_INT0->PORT_POL[0] = *((uint32_t *)(eventPattern + 0));
      LPC_GPIO_GROUP_INT0->PORT_POL[1] = *((uint32_t *)(eventPattern + 1));
      LPC_GPIO_GROUP_INT0->PORT_ENA[0] = *((uint32_t *)(bitPattern + 0));
      LPC_GPIO_GROUP_INT0->PORT_ENA[1] = *((uint32_t *)(bitPattern + 1));
      /* as soon as enabled, an edge may be generated       */
      /* clear interrupt flag and NVIC pending interrupt to */
      /* workaround the potential edge generated as enabled */
      LPC_GPIO_GROUP_INT0->CTRL |= (1<<0);
      NVIC_ClearPendingIRQ(GINT0_IRQn);
      NVIC_EnableIRQ(GINT0_IRQn);
      break;
    case GROUP1:
      if ( sense == 0 ) {
        LPC_GPIO_GROUP_INT1->CTRL &= ~(0x1<<2);	/* Edge trigger */
      }
      else {
        LPC_GPIO_GROUP_INT1->CTRL |= (0x1<<2);	/* Level trigger. */
      }
      if ( logic == 0 ) {
        LPC_GPIO_GROUP_INT1->CTRL &= ~(0x1<<1);	/* OR */
      }
      else {
        LPC_GPIO_GROUP_INT1->CTRL |= (0x1<<1);	/* AND */
      }
      LPC_GPIO_GROUP_INT1->PORT_POL[0] = *((uint32_t *)(eventPattern + 0));
      LPC_GPIO_GROUP_INT1->PORT_POL[1] = *((uint32_t *)(eventPattern + 1));
      LPC_GPIO_GROUP_INT1->PORT_ENA[0] = *((uint32_t *)(bitPattern + 0));
      LPC_GPIO_GROUP_INT1->PORT_ENA[1] = *((uint32_t *)(bitPattern + 1));
      /* as soon as enabled, an edge may be generated       */
      /* clear interrupt flag and NVIC pending interrupt to */
      /* workaround the potential edge generated as enabled */
      LPC_GPIO_GROUP_INT1->CTRL |= (1<<0);
      NVIC_ClearPendingIRQ(GINT1_IRQn);
      NVIC_EnableIRQ(GINT1_IRQn);
      break;
    default:
      break;
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOGetPinValue
**
** Descriptions:		Read Current state of port pin, PIN register value
**
** parameters:			port num, bit position
** Returned value:		None
**
*****************************************************************************/
uint32_t GPIOGetPinValue( uint32_t portNum, uint32_t bitPosi )
{
  uint32_t regVal = 0;	

  if( bitPosi < 0x20 ) {	
		if ( LPC_GPIO->PIN[portNum] & (0x1<<bitPosi) ) {
			regVal = 1;
		}
  }
	else if( bitPosi == 0xFF ) {
		regVal = LPC_GPIO->PIN[portNum];
  }
  return ( regVal );		
}

/*****************************************************************************
** Function name:		GPIOSetBitValue
**
** Descriptions:		Set/clear a bit in a specific position
**
** parameters:			port num, bit position, bit value
** 						
** Returned value:		None
**
*****************************************************************************/
void GPIOSetBitValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
  if ( bitVal ) {
		LPC_GPIO->SET[portNum] = 1<<bitPosi;
  }
  else {
		LPC_GPIO->CLR[portNum] = 1<<bitPosi;
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOSetDir
**
** Descriptions:		Set the direction in GPIO port
**
** parameters:			portNum, bit position, direction (1 out, 0 input)
** 						
** Returned value:		None
**
*****************************************************************************/
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
  if( dir ) {
		LPC_GPIO->DIR[portNum] |= (1<<bitPosi);
  }
  else {
		LPC_GPIO->DIR[portNum] &= ~(1<<bitPosi);
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOPatternMatchInit
**
** Descriptions:		Initialize pattern match to constant 0, disable any unused slices.
**
** parameters:			None
** 						
** Returned value:	None
**
*****************************************************************************/
void GPIOPatternMatchInit( void )
{
  LPC_GPIO_PIN_INT->PMCFG = (PATTERN_CONST_0<<BIT_SLICE0)|
														(PATTERN_CONST_0<<BIT_SLICE1)|
														(PATTERN_CONST_0<<BIT_SLICE2)|
														(PATTERN_CONST_0<<BIT_SLICE3)|
														(PATTERN_CONST_0<<BIT_SLICE4)|
														(PATTERN_CONST_0<<BIT_SLICE5)|
														(PATTERN_CONST_0<<BIT_SLICE6)|
														(PATTERN_CONST_0<<BIT_SLICE7);
	return;
}

/*****************************************************************************
** Function name:		GPIOSetPatternMatchInput
**
** Descriptions:		Set pattern match input, map to PINTSEL register.
**
** parameters:			channel #, port #, and port pin
** 						
** Returned value:	None
**
*****************************************************************************/
void GPIOSetPatternMatchInput( uint32_t channelNum, uint32_t portNum, uint32_t bitPosi )
{
  if ( portNum ) {
    LPC_PMUX->GPIO_INT_P_MUX[channelNum] = bitPosi + 24; 
  }
  else {
    LPC_PMUX->GPIO_INT_P_MUX[channelNum] = bitPosi; 
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOSetPatternMatchSlice
**
** Descriptions:		Set the pattern match slice based on the input
**
** parameters:			channel #, slice #, condition, and product EP 
** 						
** Returned value:	None
**
*****************************************************************************/
void GPIOSetPatternMatchSlice( uint32_t channelNum, uint32_t sliceNum, uint32_t condition, uint32_t isProdEP  )
{
	LPC_GPIO_PIN_INT->PMSRC &= ~(0x7 << (8 + sliceNum * 3));
	LPC_GPIO_PIN_INT->PMSRC |= (channelNum << (8 + sliceNum * 3));
	LPC_GPIO_PIN_INT->PMCFG &= ~(0x7 << (8 + sliceNum * 3));
  LPC_GPIO_PIN_INT->PMCFG |= (condition << (8 + sliceNum * 3));
	if ( isProdEP  ) {
		if ( sliceNum != 7 ) {
			LPC_GPIO_PIN_INT->PMCFG |= (0x1 << sliceNum);
		}
		NVIC_EnableIRQ((IRQn_Type)(PIN_INT0_IRQn+sliceNum));
	}
	return;
}

/*****************************************************************************
** Function name:		GPIOPatternMatchEnable
**
** Descriptions:		Enable the pattern match engine, called only after inputs 
**									and slices are configured. 
**
** parameters:			RxEv enable flag (TRUE or FALSE )
** 						
** Returned value:	None
**
*****************************************************************************/
void GPIOPatternMatchEnable( uint32_t PMatchSel, uint32_t RxEvEna )
{
	LPC_GPIO_PIN_INT->PMCTRL &= ~(SEL_PMATCH | ENA_PXEV);

	if ( PMatchSel ) {
		LPC_GPIO_PIN_INT->PMCTRL |= SEL_PMATCH;
	}	
	if ( RxEvEna ) {
		LPC_GPIO_PIN_INT->PMCTRL |= ENA_PXEV;
	}
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
