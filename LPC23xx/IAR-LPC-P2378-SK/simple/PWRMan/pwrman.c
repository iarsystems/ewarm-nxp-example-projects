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
#include <nxp/iolpc2378.h>
#include "type.h"
#include "irq.h"
#include "target.h"
#include "pwrman.h"
#include "board.h"

#include <intrinsics.h>

volatile DWORD eint0_counter;
/*****************************************************************************
** Function name:   EINT3_Handler
**
** Descriptions:    external INT handler
**
** parameters:      None
** Returned value:    None
**
*****************************************************************************/
__irq __nested __arm void EINT3_Handler (void)
{

  IO0INTCLR = B1_MASK;  /* clear interrupt */
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
  IO0INTENF = B1_MASK;        /* B1 interrupt by falling edge. */

  if ( install_irq( EINT3_INT, (void *)EINT3_Handler, HIGHEST_PRIORITY ) == FALSE )
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
  INTWAKE_bit.GPIO0WAKE = 1;     /* Set GPIOWAKE as wakeup source */
  return ( TRUE );
}

/******************************************************************************
**                            End Of File
******************************************************************************/
