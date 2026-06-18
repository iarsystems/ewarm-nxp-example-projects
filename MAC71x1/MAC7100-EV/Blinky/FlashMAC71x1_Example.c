#include "MAC7100InterruptHandler.h"
#include <NXP/iomac7100.h>
#include <intrinsics.h>


/*
** usefull for bitmask operations
*/
#define BIT0  ( 0x00000001 )
#define BIT1  ( 0x00000002 )
#define BIT2  ( 0x00000004 )
#define BIT3  ( 0x00000008 )
#define BIT4  ( 0x00000010 )
#define BIT5  ( 0x00000020 )
#define BIT6  ( 0x00000040 )
#define BIT7  ( 0x00000080 )
#define BIT8  ( 0x00000100 )
#define BIT9  ( 0x00000200 )
#define BIT10 ( 0x00000400 )
#define BIT11 ( 0x00000800 )
#define BIT12 ( 0x00001000 )
#define BIT13 ( 0x00002000 )
#define BIT14 ( 0x00004000 )
#define BIT15 ( 0x00008000 )
#define BIT16 ( 0x00010000 )
#define BIT17 ( 0x00020000 )
#define BIT18 ( 0x00040000 )
#define BIT19 ( 0x00080000 )
#define BIT20 ( 0x00100000 )
#define BIT21 ( 0x00200000 )
#define BIT22 ( 0x00400000 )
#define BIT23 ( 0x00800000 )
#define BIT24 ( 0x01000000 )
#define BIT25 ( 0x02000000 )
#define BIT26 ( 0x04000000 )
#define BIT27 ( 0x08000000 )
#define BIT28 ( 0x10000000 )
#define BIT29 ( 0x20000000 )
#define BIT30 ( 0x40000000 )
#define BIT31 ( 0x80000000 )


#define LED_MASK ( 0xFFUL<<8 )


void INTC_PIT1_Handler( void ) ;


int main( void )
{
  /* start/prepare timer */
  PITCTRL &= ~( BIT24 ) ;  /* enable pit */
  PITINTSEL |= ( BIT1 ) ;  /* timers generate interrupts */
  PITINTEN |= ( BIT1 ) ;  /* enable timer interrupts */
  MAC7100InstallIRQ( INTC_PIT1, 1, INTC_PIT1_Handler ) ;
  /* prepare output */
  CONFIG8_F = CONFIG9_F = CONFIG10_F = CONFIG11_F = CONFIG12_F = CONFIG13_F = CONFIG14_F = CONFIG15_F = ( BIT6 ) ;  /* set PF.8-15 as output */
  /* enable global interrupts */
  ICONFIG |= ( BIT4 ) ;  /* no fiq */
  __enable_interrupt( ) ;

  /* set the timeout compare interrupt in ms */
  TLVAL1 = 0xFFFFF ;  /* set timeout */
  PITFLG |= ( BIT1 ) ;  /* clear flag */
  PITEN |= ( BIT1 ) ;  /* enable pit1 */

  while( 1 )
    {
    }
}


void INTC_PIT1_Handler( void )
{
  static unsigned char DispBuffer = 0xAA;

  PITFLG |= ( BIT1 ) ;  /* clear flag */

  DispBuffer ^= ( 0xFF ) ;

  PORTDATA_F |= ( LED_MASK ) ;  /* clear all */
  PORTDATA_F &= ~( ( DispBuffer << 8 ) ) ;
}
