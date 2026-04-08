/*****************************************************************************
 *   pwrman.c:  Power Management module file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include <nxp/iolpc2468.h>
#include "type.h"
#include "irq.h"
#include "target.h"
#include "pwrman.h"
#include "board.h"

#include <intrinsics.h>

volatile DWORD eint0_counter;
/*****************************************************************************
** Function name:   EINT0_Handler
**
** Descriptions:    external INT handler
**
** parameters:      None
** Returned value:    None
**
*****************************************************************************/
__irq __nested __arm void EINT0_Handler (void)
{

  EXTINT   = 1;		            /* Clear pending interrupt */
  __enable_interrupt(); /* handles nested interrupt */
  eint0_counter++;
  VICADDRESS = 0;       /* Acknowledge Interrupt */
}

/*****************************************************************************
** Function name:   EINTInit
**
** Descriptions:    Initialize external interrupt pin and
**        install interrupt handler
**
** parameters:      None
** Returned value:    true or false, return false if the interrupt
**        handler can't be installed to the VIC table.
**
*****************************************************************************/
DWORD EINTInit( void )
{
  PINSEL4 = 0x00100000;	      /* set P2.10 as EINT0 and P2.0~7 GPIO output */
  EXTMODE = EINT0_EDGE;	      /* INT0 edge trigger */
  EXTPOLAR = 0;		            /* INT0 is falling edge by default */
  EXTINT   = 1;		            /* Clear pending interrupt */

  if ( install_irq( EINT3_INT, (void *)EINT0_Handler, HIGHEST_PRIORITY ) == FALSE )
  {
    return (FALSE);
  }

  return( TRUE );
}

/******************************************************************************
** Function name:   PM_Init
**
** Descriptions:    Set up wakeup source before powering down
**            Use EXTINT0 as wakeup source
**
** parameters:
** Returned value:    true or fase, if VIC table is full, return false
**
******************************************************************************/
DWORD PM_Init( void )
{
  if ( EINTInit() != TRUE )     /* Enable external interrupt 3 */
  {
    return ( FALSE );
  }
  INTWAKE_bit.EXTWAKE0 = 1;     /* Set EXTWAKE0 as wakeup source */
  return ( TRUE );
}

/******************************************************************************
**                            End Of File
******************************************************************************/
