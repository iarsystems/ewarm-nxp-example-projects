/**************************************************************************
MODULE:       MAIN - LPC17xx Version
CONTAINS:     Demonstration of non-volatile storage in flash memory
              Reset the board and the count shown on the LCD will
              continue where it left off
              Erase all flash memory when programming

              Ported for IAR-LPC1768-SK
			  
DEVELOPED BY: Embedded Systems Academy, Inc. 2010
              www.esacademy.com
COPYRIGHT:    NXP Semiconductors, 2010. All rights reserved.
VERSION:      1.00
PORTED:       IAR Systems , 2012
***************************************************************************/ 

#include "LPC17xx.h"
#include "system_LPC17xx.h"
#include "flash_nvol.h"
#include <stdio.h>
#include <string.h>
#include "drv_glcd.h"
#include "glcd_ll.h"
#include "iar_logo.h"
// a unique identifier for the non-volatile variable
#define COUNTER 1

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

// an 8-bit counter
static volatile UNSIGNED8 Counter;

/**************************************************************************
DOES:    Timer interrupt handler
         Increases counter, displays count on LEDs and
		 stores count in non-volatile memory
**************************************************************************/
void TIMER0_IRQHandler (
  void
  ) 
{
  // clear interrupt flag
  LPC_TIM0->IR = 1;
 
  // increment counter and store in non-volatile memory
  Counter++;
  if (Counter > 59) Counter = 0;
  
  if (!NVOL_SetVariable(COUNTER, (UNSIGNED8 *)&Counter, sizeof(Counter)))
  {
    while(1);
  }

  NVIC_ClearPendingIRQ(TIMER0_IRQn);
}

/**************************************************************************
DOES:    Program extry point
         Demonstration of non-volatile memory
**************************************************************************/
int main
  (
  void
  )
{
UNSIGNED8 OldCounter = 0;
  // initialize microcontroller
  SystemInit();
 
  /* Turn on clock for timer 0 */
  LPC_SC->PCONP |= (1<<1);

  // initialize timer
  LPC_TIM0->TCR = 0;                                       // timer 0 disable
  LPC_TIM0->TCR = 2;                                       // timer 0 reset
  LPC_TIM0->TCR = 0;                                       // timer 0 release reset
  LPC_TIM0->MCR = 3;                                       // interrupt and Reset on MR0
  LPC_TIM0->PR  = SystemCoreClock/(4*1000);                // 1kHz
  LPC_TIM0->MR0 = 1000;                                    // 1 second tick
  LPC_TIM0->IR  = 1;                                       // clear pending interrupt
  LPC_TIM0->TCR = 1;                                       // timer 0 enable

  // initialize non-volatile memory
  if (!NVOL_Init())
  {
    // error
    while(1);
  }

  /*Init LCD*/
  // GLCD init
  GLCD_PowerUpInit((uint8_t *)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);

  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(10,112,131,131);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fCNTR: \r");

  GLCD_SetWindow(46,112,131,131);
  GLCD_TextSetPos(0,0);
  // get the last value of the counter or set to zero if not found
  if (!NVOL_GetVariable(COUNTER, (UNSIGNED8 *)&Counter, sizeof(Counter)))
  {
    Counter = 0;
  }

  // enable timer interrupt with priority of 10
  NVIC_EnableIRQ(TIMER0_IRQn);
  NVIC_SetPriority(TIMER0_IRQn, 10);

  // loop forever
  while (1)
  {
    /*Show conuter on LCD*/
    if(Counter != OldCounter)
    {
      OldCounter = Counter;
      GLCD_print("%d\r",Counter);
    }
  }
}

#define DLY_100US             550
/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: uint32_t Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
 *		
 *************************************************************************/
void DelayResolution100us(uint32_t Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile uint32_t j = DLY_100US; j; j--)
    {
    }
  }
}

