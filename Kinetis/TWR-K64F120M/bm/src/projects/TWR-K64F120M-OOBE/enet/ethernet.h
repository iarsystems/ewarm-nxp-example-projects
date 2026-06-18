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

#ifndef __ETHERNET_H
#define __ETHERNET_H

#define ETH_ADDR_LEN	(6)
#define ETH_TYPE_LEN	(2)
#define ETH_CRC_LEN		(4)
#define ETH_HDR_LEN		(ETH_ADDR_LEN * 2 + ETH_TYPE_LEN)

typedef uint8 ETH_ADDR[ETH_ADDR_LEN];
typedef uint16 ETH_FRM_TYPE;

typedef struct
{
	ETH_ADDR dest;
	ETH_ADDR src;
	ETH_FRM_TYPE type;
} ETH_HDR;

/* Buffer sizes in bytes */
#ifndef RX_BUF_SZ
#define RX_BUF_SZ  1520		/* 2048 */
#endif
#ifndef TX_BUF_SZ
#define TX_BUF_SZ  1520
#endif

typedef struct NODE {
    struct NODE *next;
} QNODE;

typedef struct {
    QNODE node;
    uint8 *data;
    uint16 offset;
    uint16 length;
} NBUF;

typedef struct cpm_buf_desc {
	uint16 cbd_datlen;	/* Data length in buffer */
	uint16 cbd_sc;		/* Status and Control */
	uint32 cbd_bufaddr;	/* Buffer address in host memory */
} cbd_t;

struct fec_info_s {
	uint32_t iobase;
	uint32_t miibase;
	int phy_addr;
	int phy_addr1;
	int dup_spd;
	int dup_spd1;
	char *phy_name;
	int phyname_init;
	uint32_t rxIdx;
	uint32_t txIdx;
	int initialized;
};

typedef struct {
	int errors;
	int hberr;
	int babr;
	int babt;
	int gra;
	int txf;
	int mii;
	int lc;
	int rl;
	int xfun;
	int xferr;
	int rferr;
	int dtxfe;
	int dtxf;
	int drxfe;
	int drxf;
	int rfsw_inv;
	int rfsw_l;
	int rfsw_m;
	int rfsw_bc;
	int rfsw_mc;
	int rfsw_lg;
	int rfsw_no;
	int rfsw_cr;
	int rfsw_ov;
	int rfsw_tr;
} FEC_EVENT_LOG;

/* Buffer descriptor control/status used by Ethernet receive. */
#define BD_ENET_RX_EMPTY	((uint16_t)0x8000)
#define BD_ENET_RX_RO1		((uint16_t)0x4000)
#define BD_ENET_RX_WRAP		((uint16_t)0x2000)
#define BD_ENET_RX_INTR		((uint16_t)0x1000)
#define BD_ENET_RX_RO2		BD_ENET_RX_INTR
#define BD_ENET_RX_LAST		((uint16_t)0x0800)
#define BD_ENET_RX_FIRST	((uint16_t)0x0400)
#define BD_ENET_RX_MISS		((uint16_t)0x0100)
#define BD_ENET_RX_BC		((uint16_t)0x0080)
#define BD_ENET_RX_MC		((uint16_t)0x0040)
#define BD_ENET_RX_LG		((uint16_t)0x0020)
#define BD_ENET_RX_NO		((uint16_t)0x0010)
#define BD_ENET_RX_SH		((uint16_t)0x0008)
#define BD_ENET_RX_CR		((uint16_t)0x0004)
#define BD_ENET_RX_OV		((uint16_t)0x0002)
#define BD_ENET_RX_CL		((uint16_t)0x0001)
#define BD_ENET_RX_TR		BD_ENET_RX_CL
#define BD_ENET_RX_STATS	((uint16_t)0x013f)	/* All status bits */

/* Buffer descriptor control/status used by Ethernet transmit. */
#define BD_ENET_TX_READY	((uint16_t)0x8000)
#define BD_ENET_TX_PAD		((uint16_t)0x4000)
#define BD_ENET_TX_TO1		BD_ENET_TX_PAD
#define BD_ENET_TX_WRAP		((uint16_t)0x2000)
#define BD_ENET_TX_INTR		((uint16_t)0x1000)
#define BD_ENET_TX_TO2		BD_ENET_TX_INTR_
#define BD_ENET_TX_LAST		((uint16_t)0x0800)
#define BD_ENET_TX_TC		((uint16_t)0x0400)
#define BD_ENET_TX_DEF		((uint16_t)0x0200)
#define BD_ENET_TX_ABC		BD_ENET_TX_DEF
#define BD_ENET_TX_HB		((uint16_t)0x0100)
#define BD_ENET_TX_LC		((uint16_t)0x0080)
#define BD_ENET_TX_RL		((uint16_t)0x0040)	u16 cbd_sc1;

#define BD_ENET_TX_RCMASK	((uint16_t)0x003c)
#define BD_ENET_TX_UN		((uint16_t)0x0002)
#define BD_ENET_TX_CSL		((uint16_t)0x0001)
#define BD_ENET_TX_STATS	((uint16_t)0x03ff)	/* All status bits */

#define BD_ENET_RX_ERR	(BD_ENET_RX_LG | BD_ENET_RX_NO | BD_ENET_RX_CR | \
			 BD_ENET_RX_OV | BD_ENET_RX_TR)

#endif		/* __ETHERNET_H */
