/****************************************************************************
 *   $Id: dactest.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC11Axx DAC example
 *
 *   Description:
 *     This file contains DAC test modules, main entry, to test DAC APIs.
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
* documentation is hereby granted, under NXP Semiconductors’ 
* relevant copyright in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.

****************************************************************************/

#include "LPC11Axx.h"                        /* LPC11Axx definitions */
#include "dac.h"

/* 64 point Sine Wave centered about 512 DAC counts */
static const uint16_t table[64] = {
0x8000  ,
0x8C80  ,
0x9900  ,
0xA500  ,
0xB100  ,
0xBC40  ,
0xC700  ,
0xD100  ,
0xDA40  ,
0xE2C0  ,
0xEA40  ,
0xF0C0  ,
0xF600  ,
0xFA40  ,
0xFD40  ,
0xFF40  ,
0xFFC0  ,
0xFF40  ,
0xFD40  ,
0xFA40  ,
0xF600  ,
0xF0C0  ,
0xEA40  ,
0xE2C0  ,
0xDA40  ,
0xD100  ,
0xC700  ,
0xBC40  ,
0xB100  ,
0xA500  ,
0x9900  ,
0x8C80  ,
0x8000  ,
0x7380  ,
0x6700  ,
0x5B00  ,
0x4F00  ,
0x43C0  ,
0x3900  ,
0x2F00  ,
0x25C0  ,
0x1D40  ,
0x15C0  ,
0x0F40  ,
0x0A00  ,
0x05C0  ,
0x02C0  ,
0x00C0  ,
0x0040  ,
0x00C0  ,
0x02C0  ,
0x05C0  ,
0x0A00  ,
0x0F40  ,
0x15C0  ,
0x1D40  ,
0x25C0  ,
0x2F00  ,
0x3900  ,
0x43C0  ,
0x4F00  ,
0x5B00  ,
0x6700  ,
0x7380  };

/* SysTick interrupt happens ~63 times per second */
void SysTick_Handler(void)
{
  static uint32_t i=0;
  LPC_DAC->CR = table[i++&63];
}

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  SystemInit();
  /* Initialize DAC  */
  DACInit();
  SysTick_Config(SystemCoreClock/(1000/16));

  while ( 1 )
  {
    __WFI();
  }
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/

