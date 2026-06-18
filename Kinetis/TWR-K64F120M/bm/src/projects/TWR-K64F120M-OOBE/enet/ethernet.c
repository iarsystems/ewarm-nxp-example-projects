/*
 * All software, source code, included documentation, and any implied know-how
 * are property of Freescale Semiconductor and therefore considered
 * CONFIDENTIAL INFORMATION.
 * This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.
 *
 * All Confidential Information remains the property of Freescale Semiconductor
 * and will not be copied or reproduced without the express written permission
 * of the Discloser, except for copies that are absolutely necessary in order
 * to fulfill the Purpose.
 *
 * Services performed by FREESCALE in this matter are performed AS IS and
 * without any warranty.
 * CUSTOMER retains the final decision relative to the total design and
 * functionality of the end product.
 * FREESCALE neither guarantees nor will be held liable by CUSTOMER for the
 * success of this project.
 *
 * FREESCALE disclaims all warranties, express, implied or statutory
 * including, but not limited to, implied warranty of merchantability or
 * fitness for a particular purpose on any hardware, software ore advise
 * supplied to the project by FREESCALE, and or any product resulting from
 * FREESCALE services.
 * In no event shall FREESCALE be liable for incidental or consequential
 * damages arising out of this agreement. CUSTOMER agrees to hold FREESCALE
 * harmless against any and all claims demands or actions by anyone on account
 * of any damage,or injury, whether commercial, contractual, or tortuous,
 * rising directly or indirectly as a result of the advise or assistance
 * supplied CUSTOMER in connectionwith product, services or goods supplied
 * under this Agreement.
 */
/*
 * File:	ethernet.c
 * Purpose:	Ethernet loop back Demo Code for TWR-K64F120M
 */

#include "common.h"
#include "ethernet.h"
#include "mii.h"
#include "uart.h"

//#define FEC_MII_INTERNAL_LOOPBACK
#define CONFIG_SYS_FEC_RMII
#define USE_FEC_INTR
#define FEC_RCVR
//#define FEC_XMIT

#define PKT_MAXBLR_SIZE		1520
#define PKT_MAXBUF_SIZE		1518

#define NUM_RXBDS			4
#define NUM_TXBDS			4
#define LAST_RXBD			(NUM_RXBDS - 1)

uint8 unaligned_txbd[(sizeof(NBUF) * NUM_RXBDS) + 31];
uint8 unaligned_rxbd[(sizeof(NBUF) * NUM_TXBDS) + 31];
uint8 unaligned_rxbuffer[(PKT_MAXBLR_SIZE * NUM_RXBDS) + 32];
uint8 unaligned_txbuffer[(PKT_MAXBLR_SIZE * NUM_TXBDS) + 32];

cbd_t *TxNBUF;
cbd_t *RxNBUF;

uint8 *TxBuffer;
uint8 *RxBuffer;

FEC_EVENT_LOG fec_log[2];
struct fec_info_s fec_info;
struct fec_info_s *info = &fec_info;
char phyname[32] = {0};

extern uint32_t irq_sw;
extern uint32_t timer_print;

extern void pit1_init(unsigned long uSec);
extern void msec_delay(uint32_t ms);
extern void usec_delay(uint32_t ms);

#ifdef FEC_LOGMSG
void fec_log_dump(void)
{
	printf("\n   FEC0 LOG\n"
	       "--------------\n");
	printf("Errors: %4d\n", fec_log[0].errors);
	printf(" hberr: %4d\n", fec_log[0].hberr);
	printf("  babr: %4d\n", fec_log[0].babr);
	printf("  babt: %4d\n", fec_log[0].babt);
	printf("   gra: %4d\n", fec_log[0].gra);
	printf("   txf: %4d\n", fec_log[0].txf);
	printf("   mii: %4d\n", fec_log[0].mii);
	printf("    lc: %4d\n", fec_log[0].lc);
	printf("    rl: %4d\n", fec_log[0].rl);
	printf("  xfun: %4d\n", fec_log[0].xfun);
	printf(" xferr: %4d\n", fec_log[0].xferr);
	printf(" rferr: %4d\n", fec_log[0].rferr);
	printf("  dtxf: %4d\n", fec_log[0].dtxf);
	printf("  drxf: %4d\n", fec_log[0].drxf);
	printf("\nRFSW:\n");
	printf("- inv: %4d\n", fec_log[0].rfsw_inv);
	printf("-   m: %4d\n", fec_log[0].rfsw_m);
	printf("-  bc: %4d\n", fec_log[0].rfsw_bc);
	printf("-  mc: %4d\n", fec_log[0].rfsw_mc);
	printf("-  lg: %4d\n", fec_log[0].rfsw_lg);
	printf("-  no: %4d\n", fec_log[0].rfsw_no);
	printf("-  cr: %4d\n", fec_log[0].rfsw_cr);
	printf("-  ov: %4d\n", fec_log[0].rfsw_ov);
	printf("-  tr: %4d\n", fec_log[0].rfsw_tr);
	printf("-----------------\n");
}
#endif

void fec_port(void)
{
	PORTA_PCR5 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;	/* RXER */
	PORTA_PCR12 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;	/* RXD1 */
	PORTA_PCR13 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;	/* RXD0 */
	PORTA_PCR14 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;	/* CRS */
	PORTA_PCR15 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;	/* TXEN */
	PORTA_PCR16 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;	/* TXD0 */
	PORTA_PCR17 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;	/* TXD1 */

	PORTB_PCR0 = PORT_PCR_MUX(4) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_DSE_MASK;	/* MDIO */
	PORTB_PCR1 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;	/* MDC */
}

void setFecDuplexSpeed(ENET_MemMapPtr fecp, int dup_spd)
{
	if ((dup_spd >> 16) == FULL) {
		printf("-- Full duplex ");
		/* Set maximum frame length */
		fecp->RCR = ENET_RCR_MAX_FL(PKT_MAXBUF_SIZE) | ENET_RCR_MII_MODE_MASK |
		    ENET_RCR_PROM_MASK;
#ifdef CONFIG_SYS_FEC_RMII
		fecp->RCR |= ENET_RCR_RMII_MODE_MASK;
#endif
		fecp->TCR = ENET_TCR_FDEN_MASK;
	} else {
		printf("-- Half duplex ");
		/* Half duplex mode */
		fecp->RCR = ENET_RCR_MAX_FL(PKT_MAXBUF_SIZE) |
			    ENET_RCR_MII_MODE_MASK | ENET_RCR_DRT_MASK;
#ifdef CONFIG_SYS_FEC_RMII
		fecp->RCR |= ENET_RCR_RMII_MODE_MASK;
#endif
		fecp->TCR &= ~ENET_TCR_FDEN_MASK;
	}

	if ((dup_spd & 0xFFFF) == _100BASET) {
		fecp->RCR &= ~ENET_RCR_RMII_10T_MASK;	/* disabled 10T base */
		printf("100Mbps\n");
	} else {
		fecp->RCR |= ENET_RCR_RMII_10T_MASK;	/* enabled 10T base */
		printf("10Mbps\n");
	}
}

void fec_tx(void)
{
	printf("%s\n", __FUNCTION__);
}

void fec_rx(void)
{
#ifdef FEC_RCVR
	ENET_MemMapPtr fecp;
	cbd_t *pRbd = &RxNBUF[info->rxIdx];

	fecp = (ENET_MemMapPtr)(ENET_BASE_PTR);

	fecp->EIR |= ENET_EIR_RXF_MASK;

	fecp->EIMR &= ~ENET_EIMR_RXF_MASK;

	fec_log[0].drxf++;

	if (pRbd->cbd_sc & BD_ENET_RX_ERR) {
		printf("other err\n");
		//stop_printing = 0;
		if (pRbd->cbd_sc & BD_ENET_RX_NO)
			fec_log[0].rfsw_no++;
		if (pRbd->cbd_sc & BD_ENET_RX_CR)
			fec_log[0].rfsw_cr++;
		if (pRbd->cbd_sc & BD_ENET_RX_OV)
			fec_log[0].rfsw_ov++;
		if (pRbd->cbd_sc & BD_ENET_RX_TR)
			fec_log[0].rfsw_tr++;
	}

	/* Increment BD count */
	info->rxIdx = (info->rxIdx + 1) % NUM_RXBDS;

	pRbd->cbd_datlen = RX_BUF_SZ;
#define BD_ENET_RX_WI	(BD_ENET_RX_WRAP | BD_ENET_RX_INTR)
	pRbd->cbd_sc &= BD_ENET_RX_WI;
	pRbd->cbd_sc |= BD_ENET_RX_EMPTY;

	fecp->RDAR = 0x01000000;

	fecp->EIMR |= ENET_EIMR_RXF_MASK;
#endif
}

void fec_isr(void)
{
	uint32_t event;
	ENET_MemMapPtr fecp = (ENET_MemMapPtr)(ENET_BASE_PTR);

	event = fecp->EIR;

	fecp->EIR |= event & 0xFFDFFFFF;

	printf("%s: %x\n", __FUNCTION__, event);
}

#ifndef USE_FEC_INTR
int fec_rx_poll(void)
{
	ENET_MemMapPtr fecp;
	cbd_t *pRbd = &RxNBUF[0];
	int frame_length, len = 0, frame_offset;
	uint8_t *databuf = 0;
	ETH_HDR *eth_hdr;

	fecp = (ENET_MemMapPtr)(ENET_BASE_PTR);

	while (1)
	{
		if (pRbd->cbd_sc & BD_ENET_RX_EMPTY) {
			continue;
		}

		if (pRbd->cbd_sc & BD_ENET_RX_ERR) {
			printf("other err\n");
			//stop_printing = 0;
			if (pRbd->cbd_sc & BD_ENET_RX_LG)
				fec_log[0].rfsw_lg++;
			if (pRbd->cbd_sc & BD_ENET_RX_NO)
				fec_log[0].rfsw_no++;
			if (pRbd->cbd_sc & BD_ENET_RX_CR)
				fec_log[0].rfsw_cr++;
			if (pRbd->cbd_sc & BD_ENET_RX_OV)
				fec_log[0].rfsw_ov++;
			if (pRbd->cbd_sc & BD_ENET_RX_TR)
				fec_log[0].rfsw_tr++;
		} else {
			if (pRbd->cbd_sc & BD_ENET_RX_MISS)
				fec_log[0].rfsw_m++;
			if (pRbd->cbd_sc & BD_ENET_RX_BC)
				fec_log[0].rfsw_bc++;
			if (pRbd->cbd_sc & BD_ENET_RX_MC)
				fec_log[0].rfsw_mc++;

			/* Get buffer address and size */
			frame_length = pRbd->cbd_datlen - (ETH_HDR_LEN + ETH_CRC_LEN);
			frame_offset = ETH_HDR_LEN;
			eth_hdr = (ETH_HDR *)pRbd->cbd_bufaddr;
			databuf = (uint8_t *)pRbd->cbd_bufaddr;

			fec_log[0].drxf++;

			fecp->EIR |= ENET_EIR_RXF_MASK;
		}

		pRbd->cbd_datlen = RX_BUF_SZ;
		if (info->rxIdx == LAST_RXBD)
		{
			pRbd->cbd_sc |= (BD_ENET_RX_EMPTY | BD_ENET_RX_WRAP);
			info->rxIdx = 0;
		}
		else
		{
			pRbd->cbd_sc |= BD_ENET_RX_EMPTY;
			info->rxIdx++;
		}
		pRbd = &RxNBUF[info->rxIdx];
		fecp->RDAR = 0x01000000;
	}

	return 1;
}
#endif		/* Polling mode */


int fec_init(void)
{
	ENET_MemMapPtr fecp;
	int i;

#ifdef FEC_XMIT
	static uint32_t testdata[] = {
		0x0013727f, 0x1b1d0030, 0xa7003253, 0x08004500,
		0x02a89016, 0x40004006, 0x327a0acb, 0x30210acb,
		0x300904b0, 0x0c4c57fe, 0x2b34e7fe, 0xa5135018,
		0x09924e3d, 0x0000bbbb, 0x7a020000, 0x00000500,
		0x0000558e, 0xd54a0100, 0x00000000, 0x00000025,
		0x00000001, 0x00558ed5, 0x4a801000, 0x00010001,
		0x8a4a0100, 0x00010200, 0x0000full0117, 0x00000103,
		0x00010245, 0x00013130, 0x2e323033, 0x2e34372e,
		0x32380000, 0x0000eb13, 0x5544505f, 0x540001ea,
		0x13170000, 0x01010153, 0x454c2d33, 0x33373800,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x4e6f2065,	0x72726f72,
		0x20666f75, 0x6e640074, 0x696f6e20,	0x73756363,
		0x65737366, 0x756c2e00, 0x7265706f,	0x7274696e,
		0x67207374, 0x61747573, 0x2e000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00020001, 0x01000100,
		0x00000101, 0x00000001, 0x35820000, 0x0150f80c,
		0x0001baf0, 0xd54a0000, 0x00000000, 0x00000000
	};
	static uint32_t testdata1[] = {
		0x010ccd01, 0x0b010030, 0xa7575757, 0x88b80001,
		0x00bd0000, 0x00006181, 0xb2802853, 0x454c5f37,
		0x3531415f, 0x515f3330, 0x4346472f, 0x4c4c4e30,
		0x24474f24, 0x4e657747, 0x4f4f5345, 0x4d657373,
		0x61676581, 0x011e8220, 0x53454c5f, 0x37353141,
		0x5f315f33, 0x30434647, 0x2f4c4c4e, 0x30244e65,
		0x77446174, 0x61736574, 0x830a5345, 0x4c5f3735,
		0x31415f31, 0x840838a6, 0x5e1dd4fd, 0xc0bf8503,
		0x05590786, 0x01008701, 0x00880101, 0x8901008a,
		0x010dab37, 0x83010183, 0x01018301, 0x01870508,
		0x00000000, 0x870508c2, 0x96231f87, 0x05080000,
		0x00008705, 0x08000000, 0x00830100, 0x83010183,
		0x01008301, 0x00830100, 0x83010100, 0x00000000
	};
#endif		/* FEC_XMIT */

	fec_info.iobase = 0;
	fec_info.miibase = 0;
	fec_info.phy_addr = -1;
	fec_info.phy_addr1 = -1;
	fec_info.dup_spd = 0;
	fec_info.phy_name = &phyname[0];
	fec_info.phyname_init = 0;
	fec_info.rxIdx = 0;
	fec_info.txIdx = 0;
	fec_info.initialized = 0;

	SIM_SCGC2 |= SIM_SCGC2_ENET_MASK;

	/* Port Configuration */
	fecp = (ENET_MemMapPtr)(ENET_BASE_PTR);

    /* Reset the FEC */
	fecp->ECR |= ENET_ECR_RESET_MASK;

	/* Set the PHY */
	mii_init(&fec_info, 120000000);

#ifdef FEC_MII_INTERNAL_LOOPBACK

#	if (FECSPEED == _10BASET)
	fec_info.dup_spd = _10BASET;
#	elif (FECSPEED == _100BASET)
	fec_info.dup_spd = _100BASET;
#	endif

#	if (FECDUPLEX == FULL)
	fec_info.dup_spd = (FULL << 16);
#	elif (FECDUPLEX == HALF)
	fec_info.dup_spd = (HALF << 16);
#	endif

#endif		/* FEC_MII_INTERNAL_LOOPBACK */

	setFecDuplexSpeed(fecp, fec_info.dup_spd);

	fecp->EIMR = 0;			/* Disabled pending interrupt */
	fecp->EIR = 0xFFFFFFFF;	/* Clear pending interrupts */

	fecp->PALR = 0x00E00CBC;
	fecp->PAUR = 0xE5600000;
	fecp->IALR = 0;
	fecp->IAUR = 0;
	fecp->GALR = 0;
	fecp->GAUR = 0;

	fecp->MRBR = PKT_MAXBLR_SIZE;

	/* Buffer descriptors *//*  */
	TxNBUF = (cbd_t *)((uint32)(unaligned_txbd + 15) & 0xFFFFFFF0);
	RxNBUF = (cbd_t *)((uint32)(unaligned_rxbd + 15) & 0xFFFFFFF0);
	RxBuffer = (uint8 *)((uint32)(unaligned_rxbuffer + 15) & 0xFFFFFFF0);
	TxBuffer = (uint8 *)((uint32)(unaligned_txbuffer + 15) & 0xFFFFFFF0);

	printf("-- Buf desc tx: %x rx: %x Buf tx: %x rx: %x\n",
		   (int)TxNBUF, (int)RxNBUF, (int)TxBuffer, (int)RxBuffer);

	for (i = 0; i < NUM_RXBDS; i++) {
		RxNBUF[i].cbd_sc = BD_ENET_RX_EMPTY;
		RxNBUF[i].cbd_datlen = 0;
		RxNBUF[i].cbd_bufaddr = (uint32)&RxBuffer[i * PKT_MAXBLR_SIZE];
	}
	RxNBUF[i - 1].cbd_sc |= BD_ENET_RX_WRAP;

	for (i = 0; i < NUM_TXBDS; i++) {
		TxNBUF[i].cbd_sc = BD_ENET_TX_READY | BD_ENET_TX_LAST | BD_ENET_TX_TC;
		TxNBUF[i].cbd_datlen = 1500;
		TxNBUF[i].cbd_bufaddr = (uint32)&TxBuffer[i * PKT_MAXBLR_SIZE];

#ifdef FEC_XMIT
		TxNBUF[i].cbd_datlen = 0x0286;

		{
			int j;
			volatile uint32_t *src = (uint32_t *)&testdata[0];
			volatile uint32_t *dst = (uint32_t *)&TxBuffer[i* PKT_MAXBLR_SIZE];

			for (j = 0; j < (sizeof(testdata) / sizeof(long)); j++) {
				*dst++ = *src++;
			}
		}
#endif		/* FEC_XMIT */
	}
	TxNBUF[i - 1].cbd_sc |= BD_ENET_TX_WRAP;

	fecp->RDSR = (uint32)RxNBUF;
	fecp->TDSR = (uint32)TxNBUF;

#ifdef USE_FEC_INTR
	/* Interrupts */
	enable_irq(100 - 16);		/* RX */
	enable_irq(101 - 16);		/* ERR & MISC */

#ifdef FEC_XMIT
	enable_irq(99 - 16);		/* TX */
#endif		/* FEC_XMIT */
#endif		/* USE_FEC_INTR */

	fecp->ECR |= ( ENET_ECR_DBSWP_MASK | ENET_ECR_ETHEREN_MASK);
//	fecp->RDAR = 0x01000000;

	return 1;
}

void ethernet_demo(void)
{
	ENET_MemMapPtr fecp;

	printf("Kinetis %s Ethernet loopback Demo!\n"
		   "- Must have either TWR-SER1 or TWR-SER2 plug-in\n"
		   "- Press SW3 to exit...\n",
		   TWR_STRING);

	fecp = (ENET_MemMapPtr)(ENET_BASE_PTR);

	MPU_CESR &= 0xFFFFFFFE;

	timer_print = 0;

	fec_port();
	fec_init();
	fecp->MIBC = 0;		/* enabled MIB */

#ifdef FEC_LOGMSG
	dbgFecRegs();
	dbgMibRegs();
#endif

#ifdef FEC_RCVR
	printf("-- rxidx %x\n", fec_info.rxIdx);
#endif

#ifdef FEC_XMIT
	printf("-- txidx %x\n", fec_info.txIdx);

#	ifdef FEC_MII_INTERNAL_LOOPBACK
	fecp->RCR &= ~(ENET_RCR_DRT_MASK);
	fecp->RCR |= (ENET_RCR_LOOP_MASK);
#	endif
#	ifdef FEC_PHY_INTERNAL_LOOPBACK
#	endif
#	ifdef FEC_PHY_REMOTE_LOOPBACK
#	endif
#	ifdef FEC_PHY_ANALOG_LOOPBACK
#	endif
#endif		/* FEC_XMIT */

#ifdef USE_FEC_INTR
#ifdef FEC_SPEEDTEST
	fecp->EIMR = 0;
#else
	fecp->EIMR = ENET_EIMR_BABR_MASK | \
				 ENET_EIMR_BABT_MASK | \
				 ENET_EIMR_GRA_MASK | \
				 ENET_EIMR_EBERR_MASK | \
				 ENET_EIMR_LC_MASK | \
				 ENET_EIMR_RL_MASK | \
				 ENET_EIMR_UN_MASK;
#endif
//	fecp->EIMR |= ENET_EIMR_PLR_MASK;

#ifdef FEC_RCVR
	fecp->EIMR |= ENET_EIMR_RXF_MASK;
#endif
#ifdef FEC_XMIT
	fecp->EIMR |= ENET_EIMR_TXF_MASK;
#endif

	printf("- Press SW3 to continue...\n");
	while (1) {
		if (irq_sw == 0x01) {
			irq_sw &= ~0x01;
			break;
		}
	}

	pit1_init(5000000);		/* .5 sec */

#ifdef FEC_XMIT
	fecp->TDAR = 0x01000000;
#endif
#ifdef FEC_RCVR
	fecp->RDAR = 0x01000000;
#endif

	while(1) {
		if (timer_print) {
			//uart_putchar(TERM_PORT, 0xd);
#if defined(FEC_RCVR) && defined(FEC_XMIT)
			printf("fr rx: %016d tx: %016d", fec_log[0].drxf, fec_log[0].dtxf);
#else	/* defined(FEC_RCVR) && defined(FEC_XMIT) */
#	ifdef FEC_RCVR
			printf("fr rx: %08d\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b", fec_log[0].drxf);
#	elif defined(FEC_XMIT)
			printf("fr tx: %016d", fec_log[0].dtxf);
#	endif		/* FEC_RCVR */
#endif		/* FEC_RCVR && FEC_XMIT */
			timer_print = 0;
		}
	}

#endif		/* USE_FEC_INTR */
}
