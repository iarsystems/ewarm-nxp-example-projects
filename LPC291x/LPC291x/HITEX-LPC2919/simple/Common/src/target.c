/*****************************************************************************
 *   target.c:  Target C file for Philips LPC29xx Family Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.09.01  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "LPC29xx.h"
#include "type.h"
#include "irq.h"
#include "target.h"

/******************************************************************************
** Function name:		TargetInit
**
** Descriptions:		Initialize the target board; it is called in a necessary 
**				place, change it as needed
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TargetInit(void)
{
  /* Add your codes here */
  return;
}

/******************************************************************************
** Function name:	GPIOResetInit
**
** Descriptions:	Initialize the target board before running the main() 
**					function; User may change it as needed, but may not 
**					deleted it.
**
** parameters:		None
** Returned value:	None
** 
******************************************************************************/
void GPIOResetInit( void )
{
  return;        
}

/******************************************************************************
** Function name:	TargetResetInit
**
** Descriptions:	Initialize the target board before running the main() 
**		        function; User may change it as needed, but may not 
**			deleted it.
**
** parameters:		None
** Returned value:	None
** 
******************************************************************************/
void TargetResetInit(void)
{
  /* Disable all interrupts */
  init_VIC();

  IRAM1SanityCheck();	/* Check the second IRAM area @0x80008000 */

  SYS_CLK_CONF = CLK_SEL_XTAL | AUTOBLK;

  /* PLL, 3-phase output control enable, and power down PLL. */
  CGU_PLL_CTRL = P23EN | PLL_PD;
  /* PLL, MSEL=16, 3-phase output control enable, power down PLL. */		
  CGU_PLL_CTRL = PLL_XTAL_SEL | (PLL_M_VALUE<<MSEL_SHIFT) | P23EN | PLL_PD;
  /* PLL, MSEL=16, 3-phase output control enable, PLL normal operation. */
  CGU_PLL_CTRL = PLL_XTAL_SEL | (PLL_M_VALUE<<MSEL_SHIFT) | P23EN;

  /* Check lock bit, if unlocked, PLL_LOCK is always 0 */
  while ( !(CGU_PLL_STAT & PLL_LOCK) );
  /* Check clock detection register to make sure PLL is present now. */
  while ( !(CGU_RDET & PLL_PRESENT) );

  /* PLL is 160Mhz, SYS_CLK and TMR_CLK is 80Mhz, Flash Memory clock
  is half of the SYS_CLK(40Mhz). */
  SYS_CLK_CONF = CLK_SEL_PLL | AUTOBLK | DIV2;
  IVNSS_CLK_CONF = CLK_SEL_PLL | AUTOBLK | DIV2;
  MSCSS_CLK_CONF = CLK_SEL_PLL | AUTOBLK | DIV2;
  FRSS_CLK_CONF = CLK_SEL_PLL | AUTOBLK | DIV4;
  UART_CLK_CONF = CLK_SEL_PLL | AUTOBLK | DIV2;
  TMR_CLK_CONF = CLK_SEL_PLL | AUTOBLK | DIV2;
  
  /* Dependingn on the boards, on HITEX board, XTAL is 16Mhz.
  on SJA2510 board, XTAL is 10Mhz. So be careful with the setting.
  10Mhz/4=2.5Mhz, clock should be < 4.5Mhz */
  /* XTAL is 16Mhz. 16Mhz/4=4Mhz */
  SPI_CLK_CONF = CLK_SEL_XTAL | AUTOBLK | DIV4;
  
  /* denominator is 1250, load value is 1, configure FDIV1 to 
  provide a 128kHz clock from 160Mhz PLL. */
  CGU_FDIV_CONF1 = (0x02<<24)|(1<<12)|( 1250<<0);
  /* Make sure FDIV1 is present. */
  while ( !(CGU_RDET & (0x01<<6)) );	
  /* Configure the ADC clock to 32kHz (FDIV1/4)*/
  ADC_CLK_CONF = (0x06<<24) | AUTOBLK | DIV4;		/* For ADC */
//  ADC_CLK_CONF = CLK_SEL_XTAL | AUTOBLK | DIV4;		

  FCTR = FS_DCR | FS_CS;
  FBWST = SPECALWAYS | 0x04;
//  FBWST = CACHE2EN | SPECALWAYS | 0x03;

  /* Add your codes here */
  GPIOResetInit();
  return;
}

/******************************************************************************
** Function name:	IRAM1SanityCheck
**
** Descriptions:	Verify IRAM Instance 1 access.
**					If fail, spin forever.
**
** parameters:		None
** Returned value:	None
** 
******************************************************************************/
void IRAM1SanityCheck( void )
{
  volatile BYTE *wrb_ptr, *rdb_ptr;
  volatile DWORD *wr_ptr, *rd_ptr;
  DWORD i;

  wrb_ptr = (BYTE *)IRAM1_BASE_ADDR;
  for ( i = 0; i < IRAM1_SIZE; i++ )
	*wrb_ptr++ = 0x5a;

  rdb_ptr = (BYTE *)IRAM1_BASE_ADDR;
  for ( i = 0; i < IRAM1_SIZE; i++ )
  {
	if ( *rdb_ptr++ != 0x5a )
	  while ( 1 );		/* Fatal error */
  }

  wr_ptr = (DWORD *)IRAM1_BASE_ADDR;
  for ( i = 0; i < IRAM1_SIZE/4; i++ )
	*wr_ptr++ = 0xa5a5a5a5;

  rd_ptr = (DWORD *)IRAM1_BASE_ADDR;
  for ( i = 0; i < IRAM1_SIZE/4; i++ )
  {
	if ( *rd_ptr++ != 0xa5a5a5a5 )
	  while ( 1 );		/* Fatal error */
  }
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
