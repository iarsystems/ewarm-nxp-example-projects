/****************************************************************************
 *   $Id: lpc13Axx_dma.c 28 2014-01-27 14:45:13Z danielru $
 *   Project: NXP LPC122x DMA example
 *
 *   Description:
 *     This file contains DMA code example which include DMA initialization, 
 *     DMA interrupt handler, and APIs for DMA access.
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
****************************************************************************/
#include <math.h>
#include "lpc13Axx.h"
#include "lpc13Axx_dma.h"

volatile uint32_t DMADoneACount[CHANNEL_NUM];
volatile uint32_t DMADoneBCount[CHANNEL_NUM];
volatile uint32_t DMAErrCount[CHANNEL_NUM];
volatile uint32_t DMADoneA0 = 0, DMADoneB0 = 0;

volatile DMASetupCfg DMACfg;
DMASetupCfg *DMACfgPtr = (DMASetupCfg *)&DMACfg;

/******************************************************************************
** Function name:		DMA_IRQHandler
**
** Descriptions:		DMA interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void DMA_IRQHandler (void) 
{
  uint32_t i;
  uint32_t err0, err1;
  uint32_t activeA0, activeA1;
  uint32_t activeB0, activeB1;
    
  if ( LPC_DMA->INTSTAT & ACTIVE_ERR_INT )
  {
    err0 = LPC_DMA->ERRINT0;
    if ( err0 != 0 )
    {
      for ( i = 0; i < 32; i++ ) {
        if ( err0 & (0x1<<i) )
        DMAErrCount[i]++;	
      }	
      LPC_DMA->ERRINT0 = err0;
      LPC_DMA->INTENCLR0 = err0;
    }
    err1 = LPC_DMA->ERRINT1;
    if ( err1 != 0 )
    {
      for ( i = 0; i < 32; i++ ) {
      if ( err1 & (0x1<<i) )
        DMAErrCount[i+32]++;	
      }	
      LPC_DMA->ERRINT1 = err1;
      LPC_DMA->INTENCLR1 = err1;
    }
    return;
  } 
  
  if ( LPC_DMA->INTSTAT & ACTIVE_INT )
  {
    activeA0 = LPC_DMA->INTA0;
    if ( activeA0 != 0 )
    {
      for ( i = 0; i < 32; i++ ) {
        if ( activeA0 & (0x1<<i) )
          DMADoneACount[i]++;	
        }
        DMADoneA0 = activeA0;
        LPC_DMA->INTA0 = activeA0;	
    }

    activeA1 = LPC_DMA->INTA1;
    if ( activeA1 != 0 ) {
      for ( i = 0; i < 32; i++ ) {
        if ( activeA1 & (0x1<<i) )
          DMADoneACount[i+32]++;	
      }	
      LPC_DMA->INTA1 = activeA1;
    }
	
    activeB0 = LPC_DMA->INTB0;
    if ( activeB0 != 0 )
    {
      for ( i = 0; i < 32; i++ ) {
        if ( activeB0 & (0x1<<i) )
          DMADoneBCount[i]++;	
      }
      DMADoneB0 = activeB0;
      LPC_DMA->INTB0 = activeB0;	
    }

    activeB1 = LPC_DMA->INTB1;
    if ( activeB1 != 0 )
    {
      for ( i = 0; i < 32; i++ ) {
        if ( activeB1 & (0x1<<i) )
          DMADoneBCount[i+32]++;	
        }
      LPC_DMA->INTB1 = activeB1;	
    }
  } 
  return;
}

DMASetupCfg *addr_conversion( DMASetupCfg *DMACfgPtr ) 
{
	switch ( DMACfgPtr->mode )
	{
		case M2M:
			DMACfgPtr->DMAChCfg.Src += ((DMACfgPtr->count - 1) * pow(2, DMACfgPtr->width));
			DMACfgPtr->DMAChCfg.Dst += ((DMACfgPtr->count - 1) * pow(2, DMACfgPtr->width));
		break;
		case M2P:
			DMACfgPtr->DMAChCfg.Src += ((DMACfgPtr->count - 1) * pow(2, DMACfgPtr->width));
		break;
		case P2M:
			DMACfgPtr->DMAChCfg.Dst += ((DMACfgPtr->count - 1) * pow(2, DMACfgPtr->width));
		break;
		case P2P:
		default:
		break;
	}
	return ( DMACfgPtr );	
}

/******************************************************************************
** Function name:		DMA_Init
**
** Descriptions:		Set clock, reset DMA block, clear all interrupts. 
**                      
**
** parameters:          None			
** Returned value:		
** 
******************************************************************************/
void DMA_Init( void )
{ 
  LPC_SYSCON->SYSAHBCLKCTRL0 |= EN0_DMA;
  LPC_SYSCON->PRESETCTRL0 |= EN0_DMA;

  /* Enable DMA controller, master enable, user access, 
  non-bufferable, non-cacheable. */
  LPC_DMA->CTRL = 0;
  /* clear all interrupts on all channels */
  LPC_DMA->ERRINT0 = 0xFFFFFFFF;
  LPC_DMA->ERRINT1 = 0xFFFFFFFF;
#if SDMA_INTERRUPT
  LPC_DMA->INTENCLR0 = 0xFFFFFFFF;
  LPC_DMA->INTENCLR1 = 0xFFFFFFFF;
  NVIC_DisableIRQ(DMA_IRQn);
  NVIC_ClearPendingIRQ(DMA_IRQn);
  NVIC_EnableIRQ(DMA_IRQn);
#endif
  LPC_DMA->CTRL = DMA_ENABLE;
  
  LPC_DMA->SRAMBASE = (uint32_t)DMA_CHNL_ENTRY; 
  return;
}

/******************************************************************************
** Function name:		DMA_Channel_Config
**
** Descriptions:		Based on the mode, M2M, M2P, P2M, P2P,
**                  DMA set the proper channel for DMA single transfer.
**
** parameters:      Channel number and DMA Mode			
** Returned value:		
** 
******************************************************************************/
uint32_t DMA_Channel_Config( uint32_t chnl_num, DMASetupCfg *DMACfgPtr )
{
	DMASetupCfg *dmaptr;

	dmaptr = addr_conversion( DMACfgPtr );
  CHANNEL_DESCRIPTOR(chnl_num)->Src = (uint32_t)dmaptr->DMAChCfg.Src;
  CHANNEL_DESCRIPTOR(chnl_num)->Dst = (uint32_t)dmaptr->DMAChCfg.Dst;
  LPC_DMA_CH(chnl_num)->CFG = DMACfgPtr->cfg;
  LPC_DMA_CH(chnl_num)->XFERCFG = DMACfgPtr->xfercfg;
#if 0		
    CHANNEL_DESCRIPTOR(chnl_num)->Src = (uint32_t)(DMA_SRC + DMA_SIZE-4);
    CHANNEL_DESCRIPTOR(chnl_num)->Dst = (uint32_t)(DMA_DST + DMA_SIZE-4);
#if HW_TRIGGER
    LPC_DMA_CH(chnl_num)->CFG = CFG_TRIGPOL | CFG_HWTRIGEN;
#if HW_TRIGGER_BURST
    /* Test Burst wrap by transfer half of the total data */ 
    /* Case 1 */
//	CHANNEL_DESCRIPTOR(chnl_num)->Src = (uint32_t)(DMA_SRC + DMA_SIZE/2-4);
//	CHANNEL_DESCRIPTOR(chnl_num)->Dst = (uint32_t)(DMA_DST + DMA_SIZE/2-4);
//	LPC_DMA_CH(chnl_num)->DMACFG |= (CFG_TRIGBURST | CFG_BURSTPWR(5) | CFG_SRCBURSTWRAP | CFG_DSTBURSTWRAP);
	/* Case 2 */
//	CHANNEL_DESCRIPTOR(chnl_num)->Src = (uint32_t)(DMA_SRC + DMA_SIZE/2-4);
//	CHANNEL_DESCRIPTOR(chnl_num)->Dst = (uint32_t)(DMA_DST + DMA_SIZE-4);
//	LPC_DMA_CH(chnl_num)->DMACFG |= (CFG_TRIGBURST | CFG_BURSTPWR(5) | CFG_SRCBURSTWRAP);
	/* Case 3 */
//	CHANNEL_DESCRIPTOR(chnl_num)->Src = (uint32_t)(DMA_SRC + DMA_SIZE-4);
//	CHANNEL_DESCRIPTOR(chnl_num)->Dst = (uint32_t)(DMA_DST + DMA_SIZE/2-4);
//	LPC_DMA_CH(chnl_num)->DMACFG |= (CFG_TRIGBURST | CFG_BURSTPWR(5) | CFG_DSTBURSTWRAP);
	/* Case 4 */
//	CHANNEL_DESCRIPTOR(chnl_num)->Src = (uint32_t)(DMA_SRC + DMA_SIZE-4);
//	CHANNEL_DESCRIPTOR(chnl_num)->Dst = (uint32_t)(DMA_DST + DMA_SIZE-4);
//	LPC_DMA_CH(chnl_num)->DMACFG |= (CFG_TRIGBURST | CFG_BURSTPWR(5));
    LPC_DMA_CH(chnl_num)->CFG |= (CFG_TRIGBURST | CFG_BURSTPWR(6));
#endif	
    if ( LPC_DMA_CH(chnl_num)->CFG & CFG_TRIGPOL ) {
      /* if active high or raising edige, set I/O to low initially. */ 
      GPIOSetBitValue( 0, 0, 0 );
    }
    else {
      /* if active low or falling edige, set I/O to high initially. */ 
      GPIOSetBitValue( 0, 0, 1 );
    }
    LPC_DMA_CH(chnl_num)->XFERCFG = XFERCFG_CNT(DMA_SIZE/4 - 1) | XFERCFG_SRCINC(1) 
			| XFERCFG_DSTINC(1) | XFERCFG_WIDTH(WIDTH32) | XFERCFG_SETINTA;
#else
    LPC_DMA_CH(chnl_num)->CFG = 0;
    LPC_DMA_CH(chnl_num)->XFERCFG = XFERCFG_CNT(DMA_SIZE/4 - 1) | XFERCFG_SRCINC(1) 
			| XFERCFG_DSTINC(1) | XFERCFG_WIDTH(WIDTH32) | XFERCFG_SETINTA | XFERCFG_SWTRIG;
#endif
#endif

  /* Enable DMA channels */
  if ( chnl_num < 32 ) {
    LPC_DMA->ENABLESET0 = 0x01<<chnl_num;
#if SDMA_INTERRUPT 
    LPC_DMA->INTENSET0 = 0x01<<chnl_num;
#endif
  }
  else {
    LPC_DMA->ENABLESET1 = 0x01<<(chnl_num-32);
#if SDMA_INTERRUPT 
    LPC_DMA->INTENSET1 = 0x01<<(chnl_num-32);
#endif
  }
  return( 1 );
}

/******************************************************************************
** Function name:		DMA_Channel_Pingpong
**
** Descriptions:		Based on the mode, M2M, M2P, P2M, P2P,
**                      DMA set the proper channel for DMA multiple transfer.
**
** parameters:          Channel number and DMA Mode			
** Returned value:		
** 
******************************************************************************/
uint32_t DMA_Channel_Pingpong( uint32_t chnl_num, DMASetupCfg *DMACfgPtr )
{
  uint32_t desc_num = 0;
	DMASetupCfg *dmaptr;

  dmaptr = addr_conversion( DMACfgPtr );
  CHANNEL_DESCRIPTOR(chnl_num)->Src = (uint32_t)dmaptr->DMAChCfg.Src;
  CHANNEL_DESCRIPTOR(chnl_num)->Dst = (uint32_t)dmaptr->DMAChCfg.Dst;
  CHANNEL_DESCRIPTOR(chnl_num)->Link = (uint32_t)RELOAD_DESCRIPTOR(desc_num);
  RELOAD_DESCRIPTOR(desc_num)->XferCfg =  DMACfgPtr->DMAReloadCfgA.XferCfg;	
  RELOAD_DESCRIPTOR(desc_num)->Src = (uint32_t)DMACfgPtr->DMAReloadCfgA.Src;	/* Next block. PING-PONG */
  RELOAD_DESCRIPTOR(desc_num)->Dst = (uint32_t)DMACfgPtr->DMAReloadCfgA.Dst;
  RELOAD_DESCRIPTOR(desc_num)->Link = (uint32_t)RELOAD_DESCRIPTOR(desc_num+1);
  desc_num++;
  RELOAD_DESCRIPTOR(desc_num)->XferCfg = DMACfgPtr->DMAReloadCfgB.XferCfg;	
  RELOAD_DESCRIPTOR(desc_num)->Src = (uint32_t)DMACfgPtr->DMAReloadCfgB.Src;		/* Next block. PING-PONG */
  RELOAD_DESCRIPTOR(desc_num)->Dst = (uint32_t)DMACfgPtr->DMAReloadCfgB.Dst;
  RELOAD_DESCRIPTOR(desc_num)->Link = (uint32_t)RELOAD_DESCRIPTOR(desc_num-1);
  LPC_DMA_CH(chnl_num)->CFG = DMACfgPtr->cfg;
  LPC_DMA_CH(chnl_num)->XFERCFG = DMACfgPtr->xfercfg;
#if 0
    CHANNEL_DESCRIPTOR(chnl_num)->Src = (uint32_t)(DMA_SRC + DMA_SIZE-4);
    CHANNEL_DESCRIPTOR(chnl_num)->Dst = (uint32_t)(DMA_DST + DMA_SIZE-4);
    CHANNEL_DESCRIPTOR(chnl_num)->Link = (uint32_t)RELOAD_DESCRIPTOR(desc_num);
	
    RELOAD_DESCRIPTOR(desc_num)->XferCfg =  XFERCFG_CNT(DMA_SIZE/4 - 1) | XFERCFG_SRCINC(1) 
			| XFERCFG_DSTINC(1) | XFERCFG_WIDTH(WIDTH32) | XFERCFG_SETINTB | XFERCFG_SWTRIG | XFERCFG_RELOAD;	
    RELOAD_DESCRIPTOR(desc_num)->Src = (uint32_t)(DMA_SRC + DMA_SIZE*2-4);	/* Next block. PING-PONG */
    RELOAD_DESCRIPTOR(desc_num)->Dst = (uint32_t)(DMA_DST + DMA_SIZE*2-4);
    RELOAD_DESCRIPTOR(desc_num)->Link = (uint32_t)RELOAD_DESCRIPTOR(desc_num+1);
    desc_num++;
    RELOAD_DESCRIPTOR(desc_num)->XferCfg =  XFERCFG_CNT(DMA_SIZE/4 - 1) | XFERCFG_SRCINC(1) 
			| XFERCFG_DSTINC(1) | XFERCFG_WIDTH(WIDTH32) | XFERCFG_SETINTA | XFERCFG_SWTRIG | XFERCFG_RELOAD;		
    RELOAD_DESCRIPTOR(desc_num)->Src = (uint32_t)(DMA_SRC + DMA_SIZE-4);		/* Next block. PING-PONG */
    RELOAD_DESCRIPTOR(desc_num)->Dst = (uint32_t)(DMA_DST + DMA_SIZE-4);
    RELOAD_DESCRIPTOR(desc_num)->Link = (uint32_t)RELOAD_DESCRIPTOR(desc_num-1);
	
    LPC_DMA_CH(chnl_num)->CFG = 0;
    LPC_DMA_CH(chnl_num)->XFERCFG = XFERCFG_CNT(DMA_SIZE/4 - 1) | XFERCFG_SRCINC(1) 
			| XFERCFG_DSTINC(1) | XFERCFG_WIDTH(WIDTH32) | XFERCFG_SETINTA
			| XFERCFG_SWTRIG | XFERCFG_RELOAD;
#endif

  /* Enable DMA channels */
  if ( chnl_num < 32 ) {
    LPC_DMA->ENABLESET0 = 0x01<<chnl_num;
#if SDMA_INTERRUPT 
    LPC_DMA->INTENSET0 = 0x01<<chnl_num;
#endif
  }
  else {
    LPC_DMA->ENABLESET1 = 0x01<<(chnl_num-32);
#if SDMA_INTERRUPT 
    LPC_DMA->INTENSET1 = 0x01<<(chnl_num-32);
#endif
  }
  return( 1 );
}

/******************************************************************************
**                            End Of File
******************************************************************************/

