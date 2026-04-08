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

#ifndef __MII_H
#define __MII_H

int mii_write(int, int, int);
int mii_read(int, int, unsigned short *);
void mii_init(struct fec_info_s *, int);

/* MII Speed Settings */
typedef enum {
	MII_10BASE_T,	/* 10Base-T  operation */
	MII_100BASE_TX	/* 100Base-TX operation */
} MII_SPEED;

/* MII Duplex Settings */
typedef enum {
	MII_HDX,		/* half-duplex */
	MII_FDX			/* full-duplex */
} MII_DUPLEX;

#define MII_TIMEOUT				0x10000
#define MII_LINK_TIMEOUT		0x10000

#define AUTO					99
#define _1000BASET				1000
#define _100BASET				100
#define _10BASET				10
#define HALF					22
#define FULL					44

/* phy register offsets */
#define PHY_BMCR				0x00
#define PHY_BMSR				0x01
#define PHY_PHYIDR1				0x02
#define PHY_PHYIDR2				0x03
#define PHY_ANAR				0x04
#define PHY_ANLPAR				0x05
#define PHY_ANER				0x06
#define PHY_ANNPTR				0x07
#define PHY_ANLPNP				0x08
#define PHY_1000BTCR			0x09
#define PHY_1000BTSR			0x0A
#define PHY_EXSR				0x0F
#define PHY_PHYSTS				0x10
#define PHY_MIPSCR				0x11
#define PHY_MIPGSR				0x12
#define PHY_DCR					0x13
#define PHY_FCSCR				0x14
#define PHY_RECR				0x15
#define PHY_PCSR				0x16
#define PHY_LBR					0x17
#define PHY_10BTSCR				0x18
#define PHY_PHYCTRL				0x19

/* PHY BMCR */
#define PHY_BMCR_RESET			0x8000
#define PHY_BMCR_LOOP			0x4000
#define PHY_BMCR_100MB			0x2000
#define PHY_BMCR_AUTON			0x1000
#define PHY_BMCR_POWD			0x0800
#define PHY_BMCR_ISO			0x0400
#define PHY_BMCR_RST_NEG		0x0200
#define PHY_BMCR_DPLX			0x0100
#define PHY_BMCR_COL_TST		0x0080

#define PHY_BMCR_SPEED_MASK		0x2040
#define PHY_BMCR_1000_MBPS		0x0040
#define PHY_BMCR_100_MBPS		0x2000
#define PHY_BMCR_10_MBPS		0x0000

/* phy BMSR */
#define PHY_BMSR_100T4			0x8000
#define PHY_BMSR_100TXF			0x4000
#define PHY_BMSR_100TXH			0x2000
#define PHY_BMSR_10TF			0x1000
#define PHY_BMSR_10TH			0x0800
#define PHY_BMSR_EXT_STAT		0x0100
#define PHY_BMSR_PRE_SUP		0x0040
#define PHY_BMSR_AUTN_COMP		0x0020
#define PHY_BMSR_RF				0x0010
#define PHY_BMSR_AUTN_ABLE		0x0008
#define PHY_BMSR_LS				0x0004
#define PHY_BMSR_JD				0x0002
#define PHY_BMSR_EXT			0x0001

#define PHY_ANAR_NEXT_PAGE		(0x8001)
#define PHY_ANAR_REM_FAULT		(0x2001)
#define PHY_ANAR_PAUSE			(0x0401)
#define PHY_ANAR_100BT4			(0x0201)
#define PHY_ANAR_100BTX_FDX		(0x0101)
#define PHY_ANAR_100BTX			(0x0081)
#define PHY_ANAR_10BT_FDX		(0x0041)
#define PHY_ANAR_10BT			(0x0021)
#define PHY_ANAR_802_3			(0x0001)

/* phy ANLPAR */
#define PHY_ANLPAR_NP			0x8000
#define PHY_ANLPAR_ACK			0x4000
#define PHY_ANLPAR_RF			0x2000
#define PHY_ANLPAR_ASYMP		0x0800
#define PHY_ANLPAR_PAUSE		0x0400
#define PHY_ANLPAR_T4			0x0200
#define PHY_ANLPAR_TXFD			0x0100
#define PHY_ANLPAR_TX			0x0080
#define PHY_ANLPAR_10FD			0x0040
#define PHY_ANLPAR_10			0x0020
#define PHY_ANLPAR_100			0x0380	/* we can run at 100 */
/* phy ANLPAR 1000BASE-X */
#define PHY_X_ANLPAR_NP			0x8000
#define PHY_X_ANLPAR_ACK		0x4000
#define PHY_X_ANLPAR_RF_MASK	0x3000
#define PHY_X_ANLPAR_PAUSE_MASK	0x0180
#define PHY_X_ANLPAR_HD			0x0040
#define PHY_X_ANLPAR_FD			0x0020

#define PHY_ANLPAR_PSB_MASK		0x001f
#define PHY_ANLPAR_PSB_802_3	0x0001
#define PHY_ANLPAR_PSB_802_9	0x0002

/* phy 1000BTCR */
#define PHY_1000BTCR_1000FD		0x0200
#define PHY_1000BTCR_1000HD		0x0100

/* phy 1000BTSR */
#define PHY_1000BTSR_MSCF		0x8000
#define PHY_1000BTSR_MSCR		0x4000
#define PHY_1000BTSR_LRS		0x2000
#define PHY_1000BTSR_RRS		0x1000
#define PHY_1000BTSR_1000FD		0x0800
#define PHY_1000BTSR_1000HD		0x0400

/* phy EXSR */
#define PHY_EXSR_1000XF			0x8000
#define PHY_EXSR_1000XH			0x4000
#define PHY_EXSR_1000TF			0x2000
#define PHY_EXSR_1000TH			0x1000

#define CONFIG_AUTO_PHY_DISCOVERY

#endif		/* __MII_H */
