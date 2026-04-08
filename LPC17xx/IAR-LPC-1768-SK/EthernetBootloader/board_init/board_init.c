//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#include <NXP\iolpc1768.h>                        /* LPC17xx definitions */
#include "board_init.h"
#include "sbl_config.h"
#include "..\timer\timer.h"

void init_PLL(void)
{
    unsigned MValue, NValue;
  
    if ( PLL0STAT & (1 << 25) )
    {
  	PLL0CON = 1;			/* Enable PLL, disconnected */
  	PLL0FEED = 0xaa;
  	PLL0FEED = 0x55;
    }
  
    PLL0CON = 0;				/* Disable PLL, disconnected */
    PLL0FEED = 0xaa;
    PLL0FEED = 0x55;
      
    SCS |= 0x20;			/* Enable main OSC */
    while( !(SCS & 0x40) );	/* Wait until main OSC is usable */
  
    CLKSRCSEL = 0x1;		/* select main OSC, 12MHz, as the PLL clock source */
  
    PLL0CFG = PLL_MVAL | (PLL_NVAL << 16);
    PLL0FEED = 0xaa;
    PLL0FEED = 0x55;
        
    PLL0CON = 1;				/* Enable PLL, disconnected */
    PLL0FEED = 0xaa;
    PLL0FEED = 0x55;
  
    CCLKCFG = CCLKDIV;	/* Set clock divider */
  
    while ( ((PLL0STAT & (1 << 26)) == 0) );	/* Check lock bit status */
      
    MValue = PLL0STAT & 0x00007FFF;
    NValue = (PLL0STAT & 0x00FF0000) >> 16;
    while ((MValue != PLL_MVAL) && ( NValue != PLL_NVAL) );
  
    PLL0CON = 3;				/* enable and connect */
    PLL0FEED = 0xaa;
    PLL0FEED = 0x55;
    while ( ((PLL0STAT & (1 << 25)) == 0) );	/* Check connect bit status */
    return;
}

void init_MAM(void)
{
  FLASHCFG = (0x2UL<<12) | 0x3AUL;
}

void init_PCB(void)
{
  /* Turn Off all LEDs  */
  USB_LINK_LED_DIR |= USB_LINK_LED_MASK;
  USB_LINK_LED_SET |= USB_LINK_LED_MASK;

  USB_CONNECT_LED_DIR |= USB_CONNECT_LED_MASK;
  USB_CONNECT_LED_SET |= USB_CONNECT_LED_MASK;

  LED1_DIR |= LED1_MASK;
  LED1_CLR |= LED1_MASK;

  LED2_DIR |= LED2_MASK;
  LED2_SET |= LED2_MASK;
}

void VectorsRemap(void)
{
}

void board_init(void)
{
  Timer_init();
  init_PLL();
	// wait for PLL to complete initialization after POR
	Timer_Delay(100);
	init_MAM();
	init_PCB();
	VectorsRemap();
}
