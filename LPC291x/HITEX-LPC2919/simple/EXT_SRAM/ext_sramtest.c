/*****************************************************************************
 *   ext_sramtest.c:  main C entry file for NXP LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "irq.h"
#include "ext_sram.h"

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{	    
   volatile DWORD *wr_ptr; 
   volatile BYTE *char_wr_ptr;
   volatile WORD *short_wr_ptr;
   DWORD  i;
   Smc_Config_t smcConfig;
   Smc_Timing_t smcTiming;

   SCU_SetPort();

   smcConfig.memoryWidth  = Smc_8Bit;
   smcConfig.burstMode    = Smc_NonBurst;
   smcConfig.csPolarity   = Smc_CSActiveLow;
   smcConfig.readByteLane = Smc_BLSasBLS;
   smcConfig.writeProtect = Smc_ReadWrite;
   Smc_SetConfig(0, &smcConfig);

   smcTiming.idleCycles      = 1;
   smcTiming.waitStatesRead  = 1;
   smcTiming.waitStatesWrite = 1;
   smcTiming.waitStatesOEN   = 0;
   smcTiming.waitStatesWEN   = 1;
   Smc_SetTiming(0, &smcTiming);

   wr_ptr = (DWORD *)SRAM_BASE_ADDR;
   char_wr_ptr = (BYTE *)wr_ptr;
   /* Clear content before 8 bit access test */
   for ( i= 0; i < SRAM_SIZE/4; i++ )
   {
      *wr_ptr++ = 0;
   }

   /* 8 bit write */
   for (i=0; i<SRAM_SIZE; i++)
   {
      *char_wr_ptr++ = 0xAA;
      *char_wr_ptr++ = 0x55;
      *char_wr_ptr++ = 0x5A;
      *char_wr_ptr++ = 0xA5;
   }

   /* verifying */
   wr_ptr = (DWORD *)SRAM_BASE_ADDR;
   for ( i= 0; i < SRAM_SIZE/4; i++ )
   {
      if ( *wr_ptr != 0xA55A55AA )	/* be aware of endianess */
	  {
         while ( 1 );	/* fatal error */
      }
      wr_ptr++;
   }

   wr_ptr = (DWORD *)SRAM_BASE_ADDR;
   short_wr_ptr = (WORD *)wr_ptr;
   /* Clear content before 16 bit access test */
   for ( i= 0; i < SRAM_SIZE/4; i++ )
   {
      *wr_ptr++ = 0;
   }

   /* 16 bit write */
   for (i=0; i<(SRAM_SIZE/2); i++)
   {
      *short_wr_ptr++ = 0x5AA5;
      *short_wr_ptr++ = 0xAA55;
   }

   /* Verifying */
   wr_ptr = (DWORD *)SRAM_BASE_ADDR;
   for ( i= 0; i < SRAM_SIZE/4; i++ )
   {
      if ( *wr_ptr != 0xAA555AA5 )	/* be aware of endianess */
      {
         while ( 1 );	/* fatal error */
      }
      wr_ptr++;
   }
   return 0;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
