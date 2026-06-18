/**************************************************************************
MODULE:       MAIN - LPC13xx Version
CONTAINS:     Demonstration of non-volatile storage in flash memory
              Reset the board and the count shown on the LCD will
              continue where it left off
              Erase all flash memory when programming

              Ported for IAR-LPC1343-SK
			  
DEVELOPED BY: Embedded Systems Academy, Inc. 2010
              www.esacademy.com
COPYRIGHT:    NXP Semiconductors, 2010. All rights reserved.
VERSION:      1.00
PORTED:       IAR Systems , 2012
***************************************************************************/ 

#include "LPC13xx.h"
#include "system_LPC13xx.h"
#include "flash_nvol.h"
#include <stdio.h>
#include <string.h>
#include "drv_hd44780.h"

// a unique identifier for the non-volatile variable
#define COUNTER 1

// an 8-bit counter
static volatile UNSIGNED8 Counter;

/**************************************************************************
DOES:    Timer interrupt handler
         Increases counter, displays count on LEDs and
		 stores count in non-volatile memory
**************************************************************************/
void TIMER32_0_IRQHandler (
  void
  ) 
{
  // clear interrupt flag
  LPC_TMR32B0->IR = 1;
 
  // increment counter and store in non-volatile memory
  Counter++;
  if (Counter > 59) Counter = 0;
  
  if (!NVOL_SetVariable(COUNTER, (UNSIGNED8 *)&Counter, sizeof(Counter)))
  {
    while(1);
  }

  NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
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
char buf[8];
  // initialize microcontroller
  SystemInit();
 
  // Turn on clock for 32-bit timer 0
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);

  // initialize timer
  LPC_TMR32B0->TCR = 0;                                       // timer 0 disable
  LPC_TMR32B0->TCR = 2;                                       // timer 0 reset
  LPC_TMR32B0->TCR = 0;                                       // timer 0 release reset
  LPC_TMR32B0->MCR = 3;                                       // interrupt and Reset on MR0
  LPC_TMR32B0->PR  = SystemCoreClock/1000;                    // 1kHz
  LPC_TMR32B0->MR0 = 1000;                                    // 1 second tick
  LPC_TMR32B0->IR  = 1;                                       // clear pending interrupt
  LPC_TMR32B0->TCR = 1;                                       // timer 0 enable

  // initialize non-volatile memory
  if (!NVOL_Init())
  {
    // error
    while(1);
  }

  /*Init LCD*/
  HD44780_IO_Init();
  if(HD44780_OK != HD44780_PowerUpInit())
  {/*If LCD Init fails*/
     while(1);
  }
  // get the last value of the counter or set to zero if not found
  if (!NVOL_GetVariable(COUNTER, (UNSIGNED8 *)&Counter, sizeof(Counter)))
  {
    Counter = 0;
  }

  HD44780_StrShow(1,1,(const HD44780_STRING_DEF *) "CNTR   0");
  // enable timer interrupt with priority of 10
  NVIC_EnableIRQ(TIMER_32_0_IRQn);
  NVIC_SetPriority(TIMER_32_0_IRQn, 10);

  // loop forever
  while (1)
  {
    /*Show conuter on LCD*/
    if(Counter != OldCounter)
    {
      OldCounter = Counter;
      sprintf(buf,"%d",Counter); 
      HD44780_StrShow(7,1,(const HD44780_STRING_DEF *) "  ");
      HD44780_StrShow(1+8-strlen(buf),1,(const HD44780_STRING_DEF *)buf);
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

