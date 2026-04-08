/****************************************************************************
 *   $Id: ginttest.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC11Uxx GINT example
 *
 *   Description:
 *     This file contains GINT test modules, main entry, to test I2C APIs.
 *
****************************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.

* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.

****************************************************************************/

#include "LPC11Uxx.h"     /* LPC11xx Peripheral Registers */
#include "gpio.h"
#define BUZZER_PORT 0     // Port for buzzer
#define BUZZER_BIT 17
int flag=0;


 /*****************************************************************************
** Function name:   FLEX_INT1_IRQHandler
**
** Descriptions:    Use one GPIO pin as interrupt source
**
** parameters:      None
**
** Returned value:  None
**
*****************************************************************************/
void FLEX_INT1_IRQHandler(void)
{
 if ( LPC_GPIO_PIN_INT->FALL & (0x1<<1) )
   {
     LPC_GPIO_PIN_INT->FALL = 0x1<<1;
     if(flag==1)
     {
       GPIOSetBitValue( 0, 23, 1 );
       flag=0;
     }
     else if(flag==0)
     {
       GPIOSetBitValue( 0, 23, 0 );
       flag=1;
     }
  }
  LPC_GPIO_PIN_INT->IST = 0x1<<1;
  return;
}


/*******************************************************************************
**   Main Function  main()
*******************************************************************************/
int main (void)
{
  GPIOInit();
  GPIOSetDir( 0, 23, 1 );    // TEST_LED1 D3
  GPIOSetDir(PORT0, 7, 0);   // External switch(PIO0_7)
  GPIOSetFlexInterrupt( CHANNEL1,PORT0, 7, 0, 0);
  GPIOFlexIntEnable( CHANNEL1, 0);
  while ( 1 );
}

/******************************************************************************
**                            End Of File
******************************************************************************/
