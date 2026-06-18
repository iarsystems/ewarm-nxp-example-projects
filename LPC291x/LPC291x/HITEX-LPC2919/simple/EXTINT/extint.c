/*****************************************************************************
 *   extint.c:  Event router for external interrupt C file for NXP LPC29xx 
 *   Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.07.20  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "irq.h"
#include "target.h"
#include "extint.h"

volatile DWORD eint_counter = 0;

/*****************************************************************************
** Function name:		EventRouter_Handler
**
** Descriptions:		Event Router handler for External Interrupts
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void EventRouter_Handler(void)
{
  DWORD regVal;

  /* either EXTINT4 or EXTINT5 buttons are pressed,
  an interrupt will be triggered to the event router. */
  regVal = ER_PEND;
  if ( regVal & (0x01<<4) )
  {
	eint_counter++;
	ER_INT_CLR = (0x01<<4);
  }

  if ( regVal & (0x01 <<5) )
  {
	eint_counter++;
	ER_INT_CLR = (0x01<<5);
  }
  return;
}

/*****************************************************************************
** Function name:		EventRouter_Init
**
** Descriptions:		Initialize event router for external interrupt, 
**						install the event router interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void EventRouter_Init( void )
{

#if HITEX_BOARD
  
  /* EINT5 is edge trigger, rising edge. */
  ER_APR = (0x01<<5);
  ER_ATR = (0x01<<5);

  /* P1.31 configured as EXTINT5. */
  	
  SFSP1_31 = (0x01<<2)|(0x03<<0); /* Digital no PU and PD, func. 3. */

  ER_INT_CLR = 0x7FFFFFF;
  ER_MASK_CLR = 0x7FFFFFF;

  install_irq( EVT_INT, (void *)EventRouter_Handler, HIGHEST_PRIORITY );
  EnableIntReq_IRQ( EVT_INT, ACTIVE_HIGH, HIGHEST_PRIORITY );
  ER_MASK_SET |= (0x01<<5);	/* Enable interrupt on EINT5 */ 


#else
   
  /* EINT4 and EINT5 are edge trigger, rising edge. */
  ER_APR = (0x01<<4)|(0x01<<5);
  ER_ATR = (0x01<<4)|(0x01<<5);

  /* P1.31 and P1.30 are configured as EXTINT4 and EXTINT5. */
  SFSP1_30 = (0x01<<2)|(0x03<<0);	/* Digital no PU and PD, func. 3. */
  SFSP1_31 = (0x01<<2)|(0x03<<0);

  ER_INT_CLR = 0x3FFFFFF;
  ER_MASK_CLR = 0x3FFFFFF;

  install_irq( EVT_INT, (void *)EventRouter_Handler, HIGHEST_PRIORITY );
  EnableIntReq_IRQ( EVT_INT, ACTIVE_HIGH, HIGHEST_PRIORITY );
  ER_MASK_SET |= ((0x01<<4)|(0x01<<5));	/* Enable interrupt on EINT4 and EINT5 */ 
  
#endif 
  
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
