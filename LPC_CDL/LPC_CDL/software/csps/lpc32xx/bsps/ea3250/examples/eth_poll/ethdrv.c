/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2009 Embedded Artists AB
 *
 * Description:
 *    Ethernet driver
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <lpc_types.h>
#include <string.h>

#include <lpc32xx_clkpwr.h>
#include <lpc32xx_mac.h>
#include <lpc32xx_timer_driver.h>
#include "uart.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

/* PHY_ADDR, by default, AD0 has pull-up, AD1~4 have pull-downs, 
so, the default address is 0x0001 */
#define PHY_ADDR        (0x0001 << 8)   /* in MAC_MADR, bit 8~12 */


#define PHY_BMCR        0x0000
#define PHY_BMSR        0x0001
#define PHY_PHYIDR1     0x0002
#define PHY_PHYIDR2     0x0003
#define PHY_ANAR        0x0004
#define PHY_ANLPAR      0x0005
#define PHY_ANLPARNP    0x0005
#define PHY_ANER        0x0006
#define PHY_ANNPTR      0x0007

#define PHY_PHYSTS      0x0010
#define PHY_MICR        0x0011
#define PHY_MISR        0x0012
#define PHY_RESERVED1   0x0013
#define PHY_FCSCR       0x0014
#define PHY_RECR        0x0015
#define PHY_PCSR        0x0016
#define PHY_RBR         0x0017
#define PHY_LEDCR       0x0018
#define PHY_PHYCR       0x0019
#define PHY_10BTSCR     0x001A
#define PHY_CDCTRL1     0x001B
#define PHY_RESERVED2   0x001C
#define PHY_EDCR        0x001D

/* BMCR setting */
#define BMCR_RESET          0x8000
#define BMCR_LOOPBACK       0x4000
#define BMCR_SPEED_100      0x2000
#define BMCR_AN             0x1000
#define BMCR_POWERDOWN      0x0800
#define BMCR_ISOLATE        0x0400
#define BMCR_RE_AN          0x0200
#define BMCR_DUPLEX         0x0100

/* BMSR setting */
#define BMSR_100BE_T4       0x8000
#define BMSR_100TX_FULL     0x4000
#define BMSR_100TX_HALF     0x2000
#define BMSR_10BE_FULL      0x1000
#define BMSR_10BE_HALF      0x0800
#define BMSR_AUTO_DONE      0x0020
#define BMSR_REMOTE_FAULT   0x0010
#define BMSR_NO_AUTO        0x0008
#define BMSR_LINK_ESTABLISHED   0x0004


#define IRAM_ETH_BUFF_BASE 0x08010000 /* Uncached IRAM */
#define IRAM_ETH_BUFF_SIZE 0x00010000


#define ENET_MAXF_SIZE      1536
#define ENET_MAX_TX_PACKETS 16
#define ENET_MAX_RX_PACKETS 16

#define USE_PHY_RMII

/******************************************************************************
 * Local variables
 *****************************************************************************/

static UNS_8 macAddr[6];

static UNS_32 g_dmabase;
static UNS_32 gdma_size;
static TXRX_DESC_T *pTXDesc;
static UNS_32 *pTXStatus;
static UNS_32 pTXVBuffs [ENET_MAX_TX_PACKETS];
static TXRX_DESC_T *pRXDesc;
static RX_STATUS_T *pRXStatus;
static UNS_32 pRXVBuffs [ENET_MAX_RX_PACKETS];




/******************************************************************************
 * Implementation of local functions
 *****************************************************************************/


//------------------------------------------------------------------------------
static int RMII_Write (unsigned long PhyReg, unsigned long Value)
{
	unsigned long mst = 250;
	int sts = 0;

	// Write value at PHY address and register
	ENETMAC->madr = PHY_ADDR | PhyReg;
	ENETMAC->mwtd = Value;

	// Wait for unbusy status
	while (mst > 0)
	{
		if ((ENETMAC->mind & MIND_BUSY) == 0)
		{
			mst = 0;
			sts = 1;
		}
		else
		{
			mst--;
			timer_wait_ms(TIMER_CNTR0,1);
		}
	}

	return sts;
}

//------------------------------------------------------------------------------
static int RMII_Read(UNS_32 PhyReg, UNS_32 *data) 
{
	UNS_32 mst = 250;
	int sts = 0;

	// Read value at PHY address and register
	ENETMAC->madr = PHY_ADDR | PhyReg;
	ENETMAC->mcmd = MCMD_READ;

	// Wait for unbusy status
	while (mst > 0)
	{
		if ((ENETMAC->mind & MIND_BUSY) == 0)
		{
			mst = 0;
			*data = ENETMAC->mrdd;
			sts = 1;
		}
		else
		{
			mst--;
			timer_wait_ms(TIMER_CNTR0,1);
		}
	}

	ENETMAC->mcmd = 0;

	return sts;
}

//------------------------------------------------------------------------------
static int HYPHYReset(void)
{
	int goodacc;
	UNS_32 tmp1, mst;

	// Reset the PHY and wait for reset to complete
	goodacc = RMII_Write(PHY_BMCR, BMCR_RESET);
	if (goodacc == 0)
	{
		return 0;
	}
	mst = 400;
	goodacc = 0;
	while (mst > 0)
	{
		RMII_Read(PHY_BMCR, &tmp1);
		if ((tmp1 & BMCR_RESET) == 0)
		{
			mst = 0;
			goodacc = 1;
		}
		else
		{
			mst--;
			timer_wait_ms(TIMER_CNTR0,1);
		}
	}

	return goodacc;
}


/* Send a data block via Ethernet. */
static int eth_send (void *packet, int length)
{
	unsigned long idx, cidx, fb;

	// Determine number of free buffers and wait for a buffer if needed
	fb = 0;
	while (fb == 0)
	{
		idx = ENETMAC->txproduceindex;
		cidx = ENETMAC->txconsumeindex;

		if (idx == cidx)
		{
			// Producer and consumer are the same, all buffers are free
			fb = ENET_MAX_TX_PACKETS;
		}
		else if (cidx > idx)
		{
			fb = (ENET_MAX_TX_PACKETS - 1) -
				((idx + ENET_MAX_TX_PACKETS) - cidx);
		}
		else
		{
			fb = (ENET_MAX_TX_PACKETS - 1) - (cidx - idx);
		}
	}

	// Update descriptor with new frame size
	pTXDesc[idx].control = (length | 0x40000000);

	// Move data to buffer
	memcpy((void *) pTXVBuffs [idx], packet, length);

	// Get next index for transmit data DMA buffer and descriptor
	idx++;
	if (idx >= ENET_MAX_TX_PACKETS)
	{
		idx = 0;
	}
	ENETMAC->txproduceindex = idx;

	return 0;
}

/* Get a data block via Ethernet */
int eth_rx (UNS_8* pBuf, UNS_16 bufLen)
{
	UNS_32 idx, length;

	// Determine if a frame has been received
	length = 0;
	idx = ENETMAC->rxconsumeindex;
	if (ENETMAC->rxproduceindex != idx)
	{
		// Clear interrupt
		ENETMAC->intclear = MACINT_RXDONEINTEN;

		// Frame received, get size of RX packet
		length = (pRXStatus[idx].statusinfo & 0x7FF);

		/* Pass the packet up to the protocol layer */
		if (length > 0 && length < bufLen)
		{
      memcpy(pBuf, (void*)pRXVBuffs [idx], length); 
		}

		// Return DMA buffer
		idx++;
		if (idx >= ENET_MAX_TX_PACKETS)
		{
			idx = 0;
		}
		ENETMAC->rxconsumeindex = (unsigned long) idx;
	}

	return (int) length;
}


static int txrx_setup(void)
{
	int idx;
	UNS_32 *pTXStatusL, pbase1, pbase2, pbase3;
	TXRX_DESC_T *pTXRXDesc;
	RX_STATUS_T *pRXStatusL;

	// Get physical address and size of DMA buffers
	g_dmabase = (unsigned long) IRAM_ETH_BUFF_BASE;
	gdma_size = IRAM_ETH_BUFF_SIZE;

	// Setup base pointers
	pbase1 = g_dmabase;     // Start of descriptors
	pbase2 = pbase1 + 256;  // Start of statuses
	pbase3 = pbase1 + 1024; // Start of buffers

	// Setup pointers to TX structures
	ENETMAC->txdescriptor =  pbase1;
	ENETMAC->txstatus = pbase2;
	ENETMAC->txdescriptornumber = (ENET_MAX_TX_PACKETS - 1);

	// Save base address of TX descriptor table and TX status
	pTXRXDesc = (TXRX_DESC_T *) pbase1;
	pTXStatusL = (UNS_32 *) pbase2;
	pTXDesc = pTXRXDesc;
	pTXStatus = pTXStatusL;

	// Build TX descriptors
	for (idx = 0; idx < ENET_MAX_TX_PACKETS; idx++)
	{
		pTXRXDesc->packet = pbase3;
		pTXRXDesc->control = 0;
		*pTXStatusL = 0;

		// Save virtual address of buffer

		pTXVBuffs [idx] = (unsigned long) pbase3;

		// Next descriptor and status
		pTXRXDesc++;
		pTXStatusL++;
		pbase1 += sizeof (TXRX_DESC_T);
		pbase2 += sizeof (unsigned long);
		pbase3 += ENET_MAXF_SIZE;
	}

	// Setup pointers to RX structures
	ENETMAC->rxdescriptor = pbase1;
	ENETMAC->rxstatus = pbase2;
	ENETMAC->rxdescriptornumber = (ENET_MAX_RX_PACKETS - 1);

	// Save base address of RX descriptor table and RX status
	pRXDesc = pTXRXDesc;
	pRXStatus = pRXStatusL = (RX_STATUS_T *) pTXStatusL;

	// Build RX descriptors
	for (idx = 0; idx < ENET_MAX_TX_PACKETS; idx++)
	{
		pTXRXDesc->packet = pbase3;
		pTXRXDesc->control = 0x80000000 | (ENET_MAXF_SIZE - 1);
		pRXStatusL->statusinfo = 0;
		pRXStatusL->statushashcrc = 0;

		// Save virtual address of buffer
		pRXVBuffs [idx] = (unsigned long) pbase3;

		// Next descriptor and status
		pTXRXDesc++;
		pRXStatusL++;
		pbase1 += sizeof (TXRX_DESC_T);
		pbase2 += sizeof (unsigned long);
		pbase3 += ENET_MAXF_SIZE;
	}

  return 1;
}


static BOOL_32 ethHWInit(UNS_8* pMac)
{
	int btemp, goodacc;
	UNS_32 tmp1, mst = 250;

	// Enable MAC interface
	CLKPWR->clkpwr_macclk_ctrl = (CLKPWR_MACCTRL_HRCCLK_EN |
		CLKPWR_MACCTRL_MMIOCLK_EN | CLKPWR_MACCTRL_DMACLK_EN |
#ifdef USE_PHY_RMII
		CLKPWR_MACCTRL_USE_RMII_PINS);
#else
		CLKPWR_MACCTRL_USE_MII_PINS);
#endif

	// Set RMII management clock rate. This clock should be slower
	// than 12.5MHz (for NXP PHYs only). For a divider of 28, the
	// clock rate when HCLK is 150MHz will be 5.4MHz
	ENETMAC->mcfg = MCFG_CLOCK_SELECT(MCFG_CLOCK_HOST_DIV_28);

	// Reset all MAC logic
	ENETMAC->mac1 = (MAC1_SOFT_RESET | MAC1_SIMULATION_RESET |
		MAC1_RESET_MCS_TX | MAC1_RESET_TX | MAC1_RESET_MCS_RX |
		MAC1_RESET_RX);
	ENETMAC->command = (COMMAND_REG_RESET | COMMAND_TXRESET |
		COMMAND_RXRESET);
	timer_wait_ms(TIMER_CNTR0, 10);

	// Initial MAC initialization
	ENETMAC->mac1 = MAC1_PASS_ALL_RX_FRAMES;
	ENETMAC->mac2 = (MAC2_PAD_CRC_ENABLE | MAC2_CRC_ENABLE);
	ENETMAC->maxf = ENET_MAXF_SIZE;

	// Maximum number of retries, 0x37 collision window, gap */
	ENETMAC->clrt = (CLRT_LOAD_RETRY_MAX(0xF) |
		CLRT_LOAD_COLLISION_WINDOW(0x37));
	ENETMAC->ipgr = IPGR_LOAD_PART2(0x12);

#ifdef USE_PHY_RMII
	// RMII setup
	ENETMAC->command = (COMMAND_RMII | COMMAND_PASSRUNTFRAME);
	ENETMAC->supp = SUPP_RESET_RMII;
	timer_wait_ms(TIMER_CNTR0,10);
#else
	// MII setup
	ENETMAC->command = COMMAND_PASSRUNTFRAME;
#endif

	// Reset PHY
	goodacc = HYPHYReset();
	if (goodacc == 0)
	{
    uart_output("ENET:Reset of PHY timed out\r\n");
		return FALSE;
	}

	// Enable rate auto-negotiation for the link
	if (RMII_Write(PHY_BMCR,
		(BMCR_SPEED_100 | BMCR_AN)) == 0)
	{
		return FALSE;
	}

	// Wait up to 5 seconds for auto-negotiation to finish
	mst = 5000;
	goodacc = 1;
	btemp = 0;
	while (mst > 0)
	{
		goodacc &= RMII_Read(PHY_BMSR, &tmp1);
		if ((tmp1 & BMSR_AUTO_DONE) != 0)
		{
			mst = 0;
			btemp = 1;
			uart_output("ENET:auto-negotiation complete\r\n");
		}
		else
		{
			mst--;
			timer_wait_ms(TIMER_CNTR0,1);
		}
	}
	if ((goodacc == 0) || (btemp == 0))
	{
		uart_output("ENET:auto-negotiation failed\r\n");
		return FALSE;
	}

	// Check link status
	mst = 1000;
	goodacc = 1;
	btemp = 0;
	while (mst > 0)
	{
		goodacc &= RMII_Read(PHY_BMSR, &tmp1);
		if ((tmp1 & BMSR_LINK_ESTABLISHED) != 0)
		{
			mst = 0;
			btemp = 1;
			uart_output("ENET:Link status up\r\n");
		}
		else
		{
			mst--;
			timer_wait_ms(TIMER_CNTR0,1);
		}
	}
	if ((goodacc == 0) || (btemp == 0))
	{
		uart_output("ENET:Link status failure\r\n");
		return FALSE;
	}

	// Try 100MBase/full duplex
	goodacc = btemp = 0;
	if ((tmp1 & BMSR_100TX_FULL) != 0)
	{
		// Setup for full duplex and 100MBase
		goodacc = btemp = 1;
	}
	else if ((tmp1 & BMSR_100TX_HALF) != 0)
	{
		// Setup for half duplex and 100MBase
		goodacc = 1;
	}
	else if ((tmp1 & BMSR_10BE_FULL) != 0)
	{
		// Setup for full duplex and 10MBase
		btemp = 1;
	}

	// Configure Full/Half Duplex mode
	if (btemp == 1)
	{
		// 10MBase full duplex is supported
		ENETMAC->mac2 |= MAC2_FULL_DUPLEX;
		ENETMAC->command |= COMMAND_FULLDUPLEX;
		ENETMAC->ipgt = IPGT_LOAD(0x15);
		uart_output("ENET:FULL DUPLEX\r\n");
	}
	else
	{
		ENETMAC->ipgt = IPGT_LOAD(0x12);
		uart_output("ENET:HALF DUPLEX\r\n");
	}

	// Configure 100MBit/10MBit mode
	if (goodacc == 1)
	{
		// 100MBase mode
		ENETMAC->supp = SUPP_SPEED;
		uart_output("ENET:100MBase\r\n");
	}
	else
	{
		// 10MBase mode
		ENETMAC->supp = 0;
		uart_output("ENET:10Base\r\n");
	}

	// Save station address
	ENETMAC->sa [2] = (unsigned long) (pMac[0] | (pMac[1] << 8));
	ENETMAC->sa [1] = (unsigned long) (pMac[2] | (pMac[3] << 8));
	ENETMAC->sa [0] = (unsigned long) (pMac[4] | (pMac[5] << 8));

	// Setup TX and RX descriptors
	txrx_setup();

	// Enable broadcast and matching address packets
	ENETMAC->rxfliterctrl = (RXFLTRW_ACCEPTUBROADCAST |
		RXFLTRW_ACCEPTPERFECT);

	// Clear and enable interrupts
	ENETMAC->intclear = 0xFFFF;
	ENETMAC->intenable = 0;

	// Enable receive and transmit mode of MAC ethernet core
	ENETMAC->command |= (COMMAND_RXENABLE | COMMAND_TXENABLE);
	ENETMAC->mac1 |= MAC1_RECV_ENABLE;

	// Perform a 'dummy' send of the first ethernet frame with a size of 0
	// to 'prime' the MAC. The first packet after a reset seems to wait
	// until at least 2 packets are ready to go.
	goodacc = 0;
	eth_send(&goodacc, 4);

	return TRUE;
}






/******************************************************************************
 * Implementation of public functions
 *****************************************************************************/

/*****************************************************************************
 *
 * Description:
 *    Resets the NIC and initializes all required hardware registers. 
 *
 * Params:
 *    [in] pEthAddr - the ethernet address (MAC) that should be assigned to 
 *                    the driver. 
 *
 ****************************************************************************/
BOOL_32
ethIf_init(UNS_8* pEthAddr)
{

  memcpy(macAddr, pEthAddr, 6);

  /* the reset failed */
  if (ethHWInit(pEthAddr) == FALSE)
    return FALSE;

  return TRUE;
}

/*****************************************************************************
 *
 * Description:
 *    Send an ethernet packet. 
 *
 * Params:
 *    [in] pData - the data to send
 *    [in] len   - length of the data to send
 *
 ****************************************************************************/
void
ethIf_send(UNS_8* pData,
           UNS_16 len)
{
  eth_send (pData, len);
}

/*****************************************************************************
 *
 * Description:
 *    Poll the driver
 *
 * Params:
 *    [in] pBuf - allocated buffer to which data will be copied
 *    [in] len  - length of buffer
 *
 * Returns:
 *    Number of copied bytes to the supplied buffer
 *
 ****************************************************************************/
UNS_16
ethIf_poll(UNS_8* pBuf,
           UNS_16 len)

{
  return eth_rx(pBuf, len); 
}


