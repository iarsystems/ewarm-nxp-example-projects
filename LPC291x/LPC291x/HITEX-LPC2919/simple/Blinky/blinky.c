/*****************************************************************************
 *   blinky.c: Blinky C file for Philips LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2006, Philips Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"    /* LPC29xx definitions */
#include "type.h"
#include "target.h"
#include "irq.h"
#include <intrinsics.h> /* for interrupts iar */
#include "timer.h"

extern volatile DWORD timer_counter;
#define MIN_LED_POSI		24
#define MAX_LED_POSI		30

#if HITEX_BOARD
/* The order is 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 */
DWORD SevenSegmentTable[11] = { 0x5F, 0x03, 0x3E, 0x2F, 0x63, 0x6D, 
				0x7D, 0x07, 0x7F, 0x67, 0x5F };
#endif

/******************************************************************************
* Main Program                                                                *
*******************************************************************************/
int main (void)  
{
  DWORD counter, led_posi;

  init_timer(TIMER_TICKS);
  enable_timer();

  __enable_interrupt(); // iar
  
#if HITEX_BOARD
  /* GPIO 1.24~29 are LED pins set to output */
  GPIO0_DR = (1<<29)|(1<<30);
  GPIO1_DR = (1<<9)|(1<<24)|(1<<25)|(1<<26)|(1<<27)|(1<<28)|(1<<29)|(1<<30);
#else
  /* GPIO 1.24~29 are LED pins set to output */
  GPIO1_DR = (1<<24)|(1<<25)|(1<<26)|(1<<27)|(1<<28)|(1<<29);
#endif

  /* Loop forever */
  counter = 0;

#if HITEX_BOARD
  GPIO0_OR = 0x60000000;
  GPIO1_OR = 0x00000000;
  GPIO1_OR |= (SevenSegmentTable[counter]<<24);
  while ( 1 )
  {
	if ( timer_counter >= counter )
	{
	  GPIO1_OR &= ~0xFF000000;
	  GPIO1_OR |= (SevenSegmentTable[counter]<<24);
	  counter++;
	  if ( counter > 10 )
	  {
		counter = 0;	
		timer_counter = 0;
	  }
	}
  }
#else
  GPIO1_OR = 0x00000000;
  led_posi = MIN_LED_POSI;
  while ( 1 )
  {
	if ( timer_counter >= counter )
	{
	  GPIO1_OR = 1 << led_posi;
	  counter++;
	  led_posi++;
	  if ( led_posi > MAX_LED_POSI )
	  {
		led_posi = MIN_LED_POSI;
		counter = 0;	
		timer_counter = 0;
		GPIO1_OR = 0x00000000;
	  }
	}
  }
#endif
  return ( 0 );
}

/******************************************************************************
**                            End Of File
******************************************************************************/
     
