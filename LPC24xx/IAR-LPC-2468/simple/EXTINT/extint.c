/*****************************************************************************
 *   extint.c:  External interrupt API C file for NXP LPC23xx/24xx
 *   Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
 *****************************************************************************/
#include <nxp/iolpc2468.h>
#include "type.h"
#include "irq.h"
#include "extint.h"
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
__irq __nested __arm void EINT0_Handler (void)
{
  EXTINT   = 1;		            /* Clear pending interrupt */

  __enable_interrupt();       /* handles nested interrupt */

  eint0_counter++;

  /* flashing USB Link LED */
  USB_LINK_LED1_FIO ^= USB_LINK_LED1_MASK;

  VICADDRESS = 0;   /* Acknowledge Interrupt */
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

  if ( install_irq( EINT0_INT, (void *)EINT0_Handler, HIGHEST_PRIORITY ) == FALSE )
  {
    return (FALSE);
  }

  return( TRUE );
}

/******************************************************************************
**                            End Of File
******************************************************************************/
