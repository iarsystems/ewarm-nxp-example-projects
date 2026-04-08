/*****************************************************************************
 *   irq.c: Interrupt handler C file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.07.13  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"
#include "type.h"
#include "irq.h"

typedef struct VICTableEntry
{
   HandlerAddr	Isr;		        /* Interrupt service routine address */
   DWORD	Priority;	        /* Priority of the vector            */
} VICTableEntry_t;

VICTableEntry_t IRQ_VectorTable[VIC_SIZE];
VICTableEntry_t FIQ_VectorTable[VIC_SIZE];

/* Initialize the interrupt controller */

/******************************************************************************
** Function name:		IRQ_Exception
**
** Descriptions:		Interrupt Handler
** parameters:			None 
** Returned value:		None
** 
******************************************************************************/
__irq __arm void  irq_handler(void) //IRQ_Exception( void )
{
  DWORD regVal, index;
  HandlerAddr irqHandler;

  regVal = INT_VECTOR_1;
  index = regVal >> 3;
  if ( index == 0 )
  {
	/* no-interrupt handler for IRQ */
	return;
  }
  irqHandler = IRQ_VectorTable[index].Isr;
  (*irqHandler)();
  return;
}

/******************************************************************************
** Function name:		FIQ_Exception
**
** Descriptions:		Interrupt Handler
** parameters:			None 
** Returned value:		None
** 
******************************************************************************/
__irq __arm void  FIQ_Exception( void )
{
  DWORD regVal, index;
  HandlerAddr fiqHandler;

  regVal = INT_VECTOR_0;
  index = regVal >> 3;
  if ( index == 0 )
  {
	/* no-interrupt handler for FIQ. */
	return;
  }
  fiqHandler = FIQ_VectorTable[index].Isr;
  (*fiqHandler)();
}

/* Initialize the interrupt controller */

/******************************************************************************
** Function name:		DisableIntReq
**
** Descriptions:		Disable Int request in the VIC interrupt controller.
** parameters:			Int request number 
** Returned value:		None
** 
******************************************************************************/
void DisableIntReq_IRQ( DWORD IntNum )
{
  DWORD regVal;
  DWORD *vect_addr;

  vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_INT_REQ_INDEX + IntNum*4);
  regVal = *vect_addr;
  regVal |= (1<<26)|(1<<27);		/* Enable WR and TARGET bit */
  regVal &= ~((1<<16)|(1<<8));		/* Disable INT request, target force to IRQ */
  *vect_addr = regVal;
  return;
}

/******************************************************************************
** Function name:		EnableIntReq
**
** Descriptions:		Enable Int request in the VIC interrupt controller.
**						This is for IRQ only!!!
** parameters:			Int request number, Polarity, and Priority
** Returned value:		None
** 
******************************************************************************/
void EnableIntReq_IRQ( DWORD IntNum, DWORD Polarity, DWORD Priority )
{
  DWORD regVal;
  DWORD *vect_addr;

  vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_INT_REQ_INDEX + IntNum*4);
  regVal = *vect_addr;
  /* Enable WR, TARGET, and PRIORITY bits, enable INT request and set priority */
  /* if Priority > 16, it'll be wrapped to (priority % 0x0F). */
  regVal |= ((1<<26)|(1<<27)|(1<<28)|(1<<16)|(Priority&0xF));
  if ( Polarity == ACTIVE_HIGH )
  {
	regVal |= (1<<25);
	regVal &= ~(1<<17);
  }
  else
  {
	regVal |= (1<<25)|(1<<17);
  }
  /* target is IRQ */	
  regVal |= (1<<8);
  *vect_addr = regVal;
  return;
}

/******************************************************************************
** Function name:		DisableIntReq
**
** Descriptions:		Disable Int request in the VIC interrupt controller.
** parameters:			Int request number
** Returned value:		None
** 
******************************************************************************/
void DisableIntReq_FIQ( DWORD IntNum )
{
  DWORD regVal;
  DWORD *vect_addr;

  vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_INT_REQ_INDEX + IntNum*4);
  regVal = *vect_addr;
  /* Enable WR and TARGET bit, target force to IRQ */
  regVal |= (1<<26)|(1<<27)|(1<<8);		
  regVal &= ~(1<<16);			/* Disable INT request */
  *vect_addr = regVal;
  return;
}

/******************************************************************************
** Function name:		EnableIntReq
**
** Descriptions:		Enable Int request in the VIC interrupt controller.
**						This is for IRQ only!!!
** parameters:			Int request number, Polarity, and Priority 
** Returned value:		None
** 
******************************************************************************/
void EnableIntReq_FIQ( DWORD IntNum, DWORD Polarity, DWORD Priority )
{
  DWORD regVal;
  DWORD *vect_addr;

  vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_INT_REQ_INDEX + IntNum*4);
  regVal = *vect_addr;
  /* Enable WR, TARGET, and PRIORITY bits, enable INT request and set priority */
  /* if Priority > 16, it'll be wrapped to (priority % 0x0F). */
  regVal |= ((1<<26)|(1<<27)|(1<<28)|(1<<16)|(Priority&0xF));
  if ( Polarity == ACTIVE_HIGH )
  {
	regVal |= (1<<25);
	regVal &= ~(1<<17);
  }
  else
  {
	regVal |= (1<<25)|(1<<17);
  }
  /* target is FIQ */	
  regVal &= ~(1<<8);
  *vect_addr = regVal;
  return;
}

/******************************************************************************
** Function name:		init_VIC
**
** Descriptions:		Initialize VIC interrupt controller.
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void init_VIC(void) 
{
  DWORD i = 0;
   	
  /* initialize VIC*/
  INT_VECTOR_0 = 0;
  INT_PRIOR_MASK_0 = 0;
  INT_VECTOR_1 = 0;
  INT_PRIOR_MASK_1 = 0;

  /* set all the vector and vector control register to 0 */
  /* Interrupt request 0 doesn't exist, 1 based */
  for ( i = 1; i < VIC_SIZE; i++ )
  {
	DisableIntReq_IRQ( i );
	IRQ_VectorTable[i].Isr = (HandlerAddr)NULL;
	IRQ_VectorTable[i].Priority = 0;
	FIQ_VectorTable[i].Isr = (HandlerAddr)NULL;
	FIQ_VectorTable[i].Priority = 0;
  }
  return;
}

/******************************************************************************
** Function name:		install_irq
**
** Descriptions:		Install interrupt handler
** parameters:			Interrupt number, interrupt handler address, 
**						and interrupt priority
** Returned value:		true or false, return false if IntNum is out of range
** 
******************************************************************************/
DWORD install_irq( DWORD IntNumber, void *IRQAddr, DWORD Priority )
{   
  if ( IntNumber >= VIC_SIZE )
  {
	return ( FALSE );
  }
  else
  {
	DisableIntReq_IRQ( IntNumber );	/* Disable Interrupt */
	/* find first un-assigned VIC address for the handler */
	IRQ_VectorTable[IntNumber].Isr = (HandlerAddr)IRQAddr;
	IRQ_VectorTable[IntNumber].Priority = Priority;
	return( TRUE );
  }
}

/******************************************************************************
** Function name:		install_fiq
**
** Descriptions:		Install interrupt handler
** parameters:			Interrupt number, interrupt handler address, 
**						interrupt priority
** Returned value:		true or false, return false if IntNum is out of range
** 
******************************************************************************/
DWORD install_fiq( DWORD IntNumber, void *FIQAddr, DWORD Priority )
{
  if ( IntNumber >= VIC_SIZE )
  {
	return ( FALSE );
  }
  else
  {
	DisableIntReq_FIQ( IntNumber );	/* Disable Interrupt */
	/* find first un-assigned VIC address for the handler */
	FIQ_VectorTable[IntNumber].Isr = (HandlerAddr)FIQAddr;
	FIQ_VectorTable[IntNumber].Priority = Priority;
	return( TRUE );
  }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
