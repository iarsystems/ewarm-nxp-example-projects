/*****************************************************************************
 *   fmc.c:  Flash Memory Controller C file for NXP LPC29xx Family 
 *   Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.07.20  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "LPC29xx.h"                        /* LPC29xx definitions */
#include "type.h"
#include "irq.h"
#include "target.h"
#include "fmc.h"

DWORD or_mask[4] = {0x00000000, 0xFFFFFF00, 0xFFFF0000, 0xFF000000};

volatile DWORD eoe_flag = 0, eob_flag = 0, eom_flag = 0;
volatile BYTE ReadBuf[FMC_BUFFER_SIZE];

/*****************************************************************************
** Function name:		FMC_Handler
**
** Descriptions:		FMC handler for Flash Memory Controller
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void FMC_Handler(void)
{
  DWORD regVal;

  regVal = FMC_INT_STATUS;
  if ( regVal & EOE )		/* End of Erasing */
  {
	eoe_flag++;
	FMC_INT_CLR_STATUS = EOE;
  }
  if ( regVal & EOB )		/* End of Burning */ 
  {
	eob_flag++;
	FMC_INT_CLR_STATUS = EOB;
  }
  if ( regVal & EOM )		/* Signature Generation */
  {
	eom_flag++;
	FMC_INT_CLR_STATUS = EOM;
  }
  return;
}

/*****************************************************************************
** Function name:		FMCInit
**
** Descriptions:		Initialize flash memory controller(FMC)
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void FMC_Init( void )
{
  DWORD regVal;

  /* make it active, clear bit 2 to enable write and erase, default is 0x05 */
  FCTR = (0x1<<0);

#if 0  
  /* In the TargetResetInit(), the CPU and PERIPHE clock have been boosted
  to 80Mhz already. If it's too fast for the FMC module burning and erasing. 
  choose OSC clock as the system clock and make CPU and PERIPH clock down to 
  10Mhz before making FMC calls. Either way will work. FPTR needs to be
  set accordingly. */
  regVal = CGU_RDET;
  if ( regVal & (0x1<<1) )	/* OSC is present */
  {
	/* SYS_CLK is set back to OSC clock at 10Mhz */
	SYS_CLK_CONF = CLK_SEL_XTAL | AUTOBLK;
  }
  /* Set flash memory clock devider */
  regVal = FMC_PERIPH_CLK/(FMC_CRA_CLOCK * 3) - 1;
  FCRA = regVal;
#else
  regVal = PERIPH_CLK/(FMC_CRA_CLOCK * 3) - 1;
  FCRA = regVal;
#endif

#if FMC_INTERRUPT_ENABLED
  /* FMC driver can be made interrupt driven, but, in these simple
  test program, Erase and Programming are handled in the polling mode */
  install_irq( FLASH_INT, (void *)FMC_Handler, HIGHEST_PRIORITY );
  EnableIntReq_IRQ( FLASH_INT, ACTIVE_HIGH, HIGHEST_PRIORITY );
#endif
  FMC_INT_CLR_STATUS = EOM|EOB|EOE;	/* Clear EOM, EOB, and EOE */
  FMC_INT_SET_ENABLE = EOM|EOB|EOE;	/* Set EOM, EOB, and EOE */
  return;
}

/*****************************************************************************
** Function name:		FMC_Unprotect
**
** Descriptions:		A sector will be unprotected.
**
** parameters:			the starting address of the sector
** Returned value:		None
** 
*****************************************************************************/
void FMC_UnprotectSector( DWORD addrSector )
{
  DWORD *Sector;

  Sector = (DWORD *)(addrSector);
  /* unprotect sector */
  *Sector = UNPROTECT;
  FCTR = (FS_LOADREQ | FS_WPB | FS_WEB | FS_WRE | FS_CS);
  return;
}

/*****************************************************************************
** Function name:		FMC_EraseSector
**
** Descriptions:		A sector will be unprotected before erase takes place.
**
** parameters:			the starting address of the sector
** Returned value:		None
** 
*****************************************************************************/
void FMC_EraseSector(DWORD addrSector)
{
  volatile DWORD *Sector;
  DWORD regVal;

  FMC_UnprotectSector( addrSector );

  /* Set FMC program time timer, typical time to erase one sector is 100ms, 
  program one page is 1ms. */
  regVal = (PERIPH_CLK/512000) * FMC_SEC_ERASE_TIME;
  FPTR = regVal | EN_T;	/* set timer value and enable timer */

  /* Erase sector */
  *Sector = 0;
  FCTR = (FS_PROGREQ | FS_WPB | FS_CS);

#if FMC_INTERRUPT_ENABLED
  while ( eoe_flag == 0 );
  eoe_flag = 0;
#else
  /* Poll for flash to be ready, wait for End of Erase occur */
  while ((FMC_INT_STATUS & EOE) != EOE);
  FMC_INT_CLR_STATUS = EOE;
#endif
  return;
}

/*****************************************************************************
** Function name:		FMC_EraseChip
**
** Descriptions:		Unprotect all the sectors except the last one,
**						then, erase the last sector will include the whole
**						chip.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void EraseChip (void) 
{
  DWORD i, *Sector;

  /* Set timer register with maximum value for chip erase */
  FPTR = 0x7FFF | EN_T;

  /* Select all sectors except last for erasure	*/
  for (i = SECTOR_ADDR_0; i <= SECTOR_ADDR_10; i += 0x10000) 
  {
    FMC_UnprotectSector(i);
  }

  for (i = SECTOR_ADDR_11; i < SECTOR_ADDR_18; i += 0x2000) 
  {
    FMC_UnprotectSector(i);
  }

  /* Start last sector erase which will erase all the sectors if they 
  are unprotected. */ 
  Sector = (DWORD *)((DWORD)SECTOR_ADDR_18);
  *Sector = 0;
  FCTR = (FS_PROGREQ | FS_WPB | FS_CS);

#if FMC_INTERRUPT_ENABLED
  while ( eoe_flag == 0 );
  eoe_flag = 0;
#else
  /* Poll for flash to be ready, wait for End of Erase occur */
  while ((FMC_INT_STATUS & EOE) != EOE);
  FMC_INT_CLR_STATUS |= EOE;
#endif
  return;
}

/*****************************************************************************
** Function name:		FMCGetSignature
**
** Descriptions:		Get FMC signature
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void FMC_GetSignature( void )
{
  DWORD signatureValue[4];

  // 128bit signature generated by the dignature generator block
  
  signatureValue[0] = FMSW0;
  signatureValue[1] = FMSW1;
  signatureValue[2] = FMSW2;
  signatureValue[3] = FMSW3;
  return;
}

/*****************************************************************************
** Function name:		FMCWritePage
**
** Descriptions:		Write flash, the size is fixed, FMC_PAGE_SIZE 512 
**
** parameters:			The first parameter is the destination addr,
**						The second parameter is the pointer to the ssrc addr
**						The third parameter is the size
** Returned value:		None
** 
*****************************************************************************/
void FMC_WritePage( DWORD addrWrite, BYTE* pBuf, DWORD pSize )
{
  DWORD regVal;
  DWORD i;
  
  /* Preset data latches */ 
  FCTR = (FS_WRE | FS_WEB | FS_CS | FS_PDL);
  FCTR = (FS_WRE | FS_WEB | FS_CS);
  
  /* Set FMC program time timer, typical time to erase one sector is 100ms, 
  program one page is 1ms. */
  regVal = (PERIPH_CLK/512000) * FMC_PAGE_PROG_TIME;
  FPTR = regVal | EN_T;	/* set timer value and enable timer */

  /* If size is not whole number of words, fill rest of last word with 0xFF */
  if ((pSize%4) != 0) {
    *((DWORD *)pBuf+(pSize/4)) |= or_mask[pSize%4];
  }

  /* Load data to data latches */
  for (i = 0; i < ((pSize+3)/4); i++)  
  {
	*((volatile DWORD *) (addrWrite)) = *((DWORD *)pBuf);
    pBuf += 4;
    addrWrite += 4;
  }
  /* Load remaining bytes to full page (512) with 0xFF */
  for (i = ((pSize+3)/4); i < (FMC_PAGE_SIZE/4); i++) 
  {
    *((volatile DWORD *) (addrWrite)) = 0xFFFFFFFF;
    addrWrite += 4;
  }

  /* Burn Page */
  FCTR = (FS_PROGREQ | FS_WPB | FS_WRE | FS_CS);

#if FMC_INTERRUPT_ENABLED
  while ( eob_flag == 0 );
  eob_flag = 0;
#else   
  /* Poll for flash to be ready, wait for End of Burn occur. */
  while ((FMC_INT_STATUS & EOB) != EOB);
  FMC_INT_CLR_STATUS = EOB;
#endif
  return;

}

/******************************************************************************
**                            End Of File
******************************************************************************/
