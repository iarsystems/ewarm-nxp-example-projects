/* TMS470InterruptHandler.c */


#include "MAC7100InterruptHandler.h"
#include <NXP/iomac7100.h>


static void ( *IRQ_VectorTable[64] )( void ) ;


/* standard IRQ interrupt handler */
__irq __arm void IRQ_Handler( void )
{
  unsigned char IRQIACK_Buffer ;

  IRQIACK_Buffer = IRQIACK - 64 ;
  if( IRQ_VectorTable[IRQIACK_Buffer] )
    {
    ( IRQ_VectorTable[IRQIACK_Buffer] )( ) ;
    }
  CLMASK = SLMASK ;
}


void MAC7100InstallIRQ( unsigned char INTC_Num, unsigned char INTC_Level, void ( *HandlerFunc )( ) )
{
  ( ( &ICR0 )[INTC_Num] ) = INTC_Level ;
  IRQ_VectorTable[INTC_Num] = HandlerFunc ;  /* store handler func in table */
  CIMR = ( INTC_Num ) ;  /* enable this channel */
}


void MAC7100RemoveIRQ( unsigned char INTC_Num )
{
  SIMR = ( INTC_Num ) ;  /* disable this channel */
  IRQ_VectorTable[INTC_Num] = 0 ;
}
