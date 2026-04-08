/*****************************************************************************
 *   dma.h:  Header file for NXP Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __SDMA_H 
#define __SDMA_H

#define SDMA_INTERRUPT			1
#define SDMA_DOUBLE_BUFFER		0
#define PINGPONG_MODE			0
#define HW_TRIGGER				0	/* Only in single channel test. */
#define HW_TRIGGER_BURST		0

/* The last 16K bytes of the IRAM used for channel descriptor and reload descriptor */
#define DMA_CHNL_ENTRY		0x28000000		/* 2K */
#define DMA_RELOAD_ENTRY	0x28000800		/* 14K */

#define DMA_SRC			0x24000000
#define DMA_DST			0x24002000
#define DMA_SIZE		0x0100
#define CHANNEL_NUM		64

#define M2M				0x00
#define M2P				0x01
#define P2M				0x02
#define P2P				0x03

typedef struct
{
  volatile unsigned int reserved;        // 0x000
  volatile unsigned int Src;             // 0x004
  volatile unsigned int Dst;             // 0x008
  volatile unsigned int Link;            // 0x00C
} DMA_ChannelDescriptor;

typedef struct
{
  volatile unsigned int XferCfg;         // 0x000
  volatile unsigned int Src;             // 0x004
  volatile unsigned int Dst;             // 0x008
  volatile unsigned int Link;            // 0x00C
} DMA_ReloadDescriptor;

typedef struct
{
  uint32_t channel_num;									// Total number of channels
	DMA_ChannelDescriptor DMAChCfg;
	DMA_ReloadDescriptor  DMAReloadCfgA;
	DMA_ReloadDescriptor  DMAReloadCfgB;
	uint32_t count;
	uint32_t width;
	uint32_t mode;												// M2M, M2P, P2M, and P2P
	uint32_t cfg;
	uint32_t xfercfg;
	uint32_t hwtrigger;
  uint32_t burst;  
} DMASetupCfg;


#define CHANNEL_DESCRIPTOR(i)  ((DMA_ChannelDescriptor *)(DMA_CHNL_ENTRY + (i) * 4 * 4))	
#define RELOAD_DESCRIPTOR(i)   ((DMA_ReloadDescriptor *)(DMA_RELOAD_ENTRY + (i) * 4 * 4))

#define DMA_ENABLE		(0x1<<0)

#define	ACTIVE_INT		(0x1<<1)
#define ACTIVE_ERR_INT	(0x1<<2)

#define CFG_PERIPHREQEN (1 << 0)
#define CFG_HWTRIGEN (1 << 1)
#define CFG_TRIGPOL (1 << 4)
#define CFG_TRIGLEVEL (1 << 5)
#define CFG_TRIGBURST (1 << 6)
#define CFG_BURSTPWR(b) ((b) << 8)
#define CFG_SRCBURSTWRAP (1 << 14)
#define CFG_DSTBURSTWRAP (1 << 15)
#define CFG_PRI(p) ((p) << 16)

#define CTLSTAT_VALIDPENDING (1 << 0)
#define CTLSTAT_TRIGGERRED (1 << 2)

#define XFERCFG_CFGVALID (1 << 0)
#define XFERCFG_RELOAD (1 << 1)
#define XFERCFG_SWTRIG (1 << 2)
#define XFERCFG_CLRTRIGLATER (1 << 3)
#define XFERCFG_SETINTA (1 << 4)
#define XFERCFG_SETINTB (1 << 5)
#define XFERCFG_WIDTH(w) ((w) << 12)

enum {
	WIDTH8 = 0,
	WIDTH16 = 1,
	WIDTH32 = 2
};

#define XFERCFG_SRCINC(i) ((i) << 14)
#define XFERCFG_DSTINC(i) ((i) << 15)
#define XFERCFG_CNT(c) ((unsigned int)(c) << 16)

extern void DMA_IRQHandler( void );
extern DMASetupCfg *addr_conversion( DMASetupCfg *DMACfgPtr );
extern void DMA_Init( void );
extern uint32_t DMA_Channel_Config( uint32_t chnl_num, DMASetupCfg *DMACfgPtr );
extern uint32_t DMA_Channel_Pingpong( uint32_t chnl_num, DMASetupCfg *DMACfgPtr );

#endif  /* __SDMA_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/

