/*****************************************************************************
 *   fmctest.c:  main C entry file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.07.19  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "target.h"
#include "irq.h"
#include "fmc.h"
#include <intrinsics.h> /* for interrupts iar */

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  DWORD i, *PageAddr;
  BYTE TestPage[FMC_PAGE_SIZE];

  for ( i = 0; i < FMC_PAGE_SIZE; i++ )
  {
	if ( i % 2 )
	  TestPage[i] = 0x55;
	else
	  TestPage[i] = 0xAA;
  }
      	    		
  /*** The main Function is an endless loop ****/
  FMC_Init();
  
  FMC_GetSignature();

  __enable_interrupt(); // iar
    
  /* The test routine will erase a small sector a large sector */
  FMC_EraseSector( SECTOR_ADDR_11 );
  FMC_EraseSector( SECTOR_ADDR_0 );

  for ( i = 0; i < FMC_LA_PAGE_NUM; i++ )
  {
	FMC_WritePage( SECTOR_ADDR_0 + i*FMC_PAGE_SIZE, 
						&TestPage[0], FMC_PAGE_SIZE );
  }
  for ( i = 0; i < FMC_SM_PAGE_NUM; i++ )
  {
	FMC_WritePage( SECTOR_ADDR_11 + i*FMC_PAGE_SIZE, 
						&TestPage[0], FMC_PAGE_SIZE );
  }

  /* Verify if the sector 0 and 11 have been programmed correctly
  or not. */
  PageAddr = (DWORD *)((DWORD)SECTOR_ADDR_0);
  for ( i = 0; i < FMC_LA_PAGE_NUM * FMC_PAGE_SIZE; i+=4 )
  {
	if ( *PageAddr != 0x55AA55AA )
	{
	  while ( 1 );
	}
	PageAddr++;
  }

  PageAddr = (DWORD *)((DWORD)SECTOR_ADDR_11);
  for ( i = 0; i < FMC_SM_PAGE_NUM * FMC_PAGE_SIZE; i+=4 )
  {
	if ( *PageAddr != 0x55AA55AA )
	{
	  while ( 1 );
	}
	PageAddr++;
  }
  
  while(1);
  
  return 0;
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
