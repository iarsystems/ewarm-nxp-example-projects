/*
 * Copyright (C) 2004-2008 Freescale Semiconductor, Inc.
 * TsiChung Liew (Tsi-Chung.Liew@freescale.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include "common.h"
#include "ethernet.h"
#include "mii.h"

typedef struct phy_info_struct {
	uint32_t phyid;
	char *strid;
} phy_info_t;

extern void msec_delay(uint32_t ms);
extern void usec_delay(uint32_t ms);

int _strcpy(char *dst, char *src)
{
	int i = 0;
	while (*src != NULL) {
		*dst++ = *src++;
		i++;
	}
	return i;
}

int miiphy_speed (unsigned char addr)
{
	uint16_t bmcr, anlpar;

	/* Check Basic Management Control Register first. */
	if (mii_read (addr, PHY_BMCR, &bmcr)) {
		printf ("PHY speed");
		goto miiphy_read_failed;
	}
	/* Check if auto-negotiation is on. */
	if (bmcr & PHY_BMCR_AUTON) {
		/* Get auto-negotiation results. */
		if (mii_read (addr, PHY_ANLPAR, &anlpar)) {
			printf ("PHY AN speed");
			goto miiphy_read_failed;
		}
		return (anlpar & PHY_ANLPAR_100) ? _100BASET : _10BASET;
	}
	/* Get speed from basic control settings. */
	return (bmcr & PHY_BMCR_100MB) ? _100BASET : _10BASET;

miiphy_read_failed:
	printf (" read failed, assuming 10BASE-T\n");
	return _10BASET;
}

/* Determine full/half duplex.  Return half on error. */
int miiphy_duplex (unsigned char addr)
{
	uint16_t bmcr, anlpar;

	/* Check Basic Management Control Register first. */
	if (mii_read (addr, PHY_BMCR, &bmcr)) {
		printf ("PHY duplex");
		goto miiphy_read_failed;
	}
	/* Check if auto-negotiation is on. */
	if (bmcr & PHY_BMCR_AUTON) {
		/* Get auto-negotiation results. */
		if (mii_read (addr, PHY_ANLPAR, &anlpar)) {
			printf ("PHY AN duplex");
			goto miiphy_read_failed;
		}
		return (anlpar & (PHY_ANLPAR_10FD | PHY_ANLPAR_TXFD)) ?
		    FULL : HALF;
	}
	/* Get speed from basic control settings. */
	return (bmcr & PHY_BMCR_DPLX) ? FULL : HALF;

miiphy_read_failed:
	printf (" read failed, assuming half duplex\n");
	return HALF;
}

/*
 * \brief Write a value to a PHY's MII register.
 *
 * \param   phy_addr    Address of the PHY
 * \param   reg_addr    Address of the register in the PHY
 * \param   data        Data to be written to the PHY register
 * \return  0 if write is successful; 1 if write times out
 *
 * mii_write() polls for the FEC's MII interrupt event (which should
 * be masked from the interrupt handler) and clears it. If after a
 * suitable amount of time the event isn't triggered, a non-zero value
 * is returned.
 */
int mii_write(int phy_addr, int reg_addr, int data)
{
	int timeout;
	ENET_MemMapPtr fecp;

	fecp = (ENET_MemMapPtr)(ENET_BASE_PTR);

	/* Clear the MII interrupt bit */
	fecp->EIR = ENET_EIR_MII_MASK;

	/* Initiatate the MII Management write */
	fecp->MMFR = (ENET_MMFR_ST(1) | ENET_MMFR_OP(1) | ENET_MMFR_TA(2) | \
				  ENET_MMFR_PA(phy_addr) | ENET_MMFR_RA(reg_addr) | \
					ENET_MMFR_DATA(data));

	/* Poll for the MII interrupt (interrupt should be masked) */
	for (timeout = 0; timeout < MII_TIMEOUT; timeout++) {
		if (fecp->EIR & ENET_EIR_MII_MASK)
			break;
	}

	if(timeout == MII_TIMEOUT)
		return 1;

	/* Clear the MII interrupt bit */
	fecp->EIR = ENET_EIR_MII_MASK;

	return 0;
}

/*
 * \brief   Read a value from a PHY's MII register.
 * \param   phy_addr    Address of the PHY
 * \param   reg_addr    Address of the register in the PHY
 * \param   data        Pointer to location were read data will be stored
 * \return  0 if write is successful; 1 if write times out
 *
 * mii_read() polls for the FEC's MII interrupt event (which should
 * be masked from the interrupt handler) and clears it. If after a
 * suitable amount of time the event isn't triggered, a non-zero value
 * is returned.
 */
int mii_read(int phy_addr, int reg_addr, uint16_t *data)
{
	int timeout;
	ENET_MemMapPtr fecp;

	fecp = (ENET_MemMapPtr)(ENET_BASE_PTR);

	/* Clear the MII interrupt bit */
	fecp->EIR = ENET_EIR_MII_MASK;

	/* Initiatate the MII Management write */
	fecp->MMFR = (ENET_MMFR_ST(1) | ENET_MMFR_OP(2) | ENET_MMFR_TA(2) | \
				  ENET_MMFR_PA(phy_addr) | ENET_MMFR_RA(reg_addr));

	/* Poll for the MII interrupt (interrupt should be masked) */
	for (timeout = 0; timeout < MII_TIMEOUT; timeout++)
	{
		if (fecp->EIR & ENET_EIR_MII_MASK)
			break;
	}

	if(timeout == MII_TIMEOUT)
		return 1;

	/* Clear the MII interrupt bit */
	fecp->EIR = ENET_EIR_MII_MASK;

	*data = (uint16_t)(fecp->MMFR & 0x0000FFFF);

	return 0;
}

int mii_discover_phy(struct fec_info_s *info)
{
#define MAX_PHY_PASSES 11
	int phyaddr, pass;
	uint32_t phyno, phytype;
	uint16_t tmp;
	int i, found = 0;
	phy_info_t phyinfo[] = {
		{0x00221619, "KS8721BL"},	/* Micrel KS8721BL/SL */
		{0x00221537, "KSZ8041RNL"},	/* Micrel KSZ804RNL */
		{0x00221512, "KSZ8041NL"},	/* Micrel KSZ8041NL */
		{0x00221560, "KSZ8081RNA"},	/* Micrel KSZ8041NL */
		{0x20005CE1, "N83640"},		/* National 83640 */
		{0x20005C90, "N83848"},		/* National 83848 */
		{0x20005CA2, "N83849"},		/* National 83849 */
		{0, 0}
	};

	if (info->phyname_init)
		return info->phy_addr;

	phyno = 0;
	phyaddr = -1;		/* didn't find a PHY yet */
	for (pass = 1; pass <= MAX_PHY_PASSES && phyaddr < 0; ++pass) {
		if (pass > 1) {
			for (i = 0; i < 0x20000; i++)
			{
				asm("nop");
			}
		}

		for (; phyno < 32 && phyaddr < 0; ++phyno) {

			mii_read(phyno, PHY_PHYIDR1, &tmp);
			//printf("PHY type 0x%x pass %d type\n", phytype, pass);

			if (phytype != 0xffff) {
				phyaddr = phyno;
				phytype = tmp << 16;
				mii_read(phyno, PHY_PHYIDR2, &tmp);
				phytype |= tmp;
				//printf("PHY @ 0x%x pass %d\n", phyno, pass);

				for (i = 0; i < (sizeof(phyinfo) / sizeof(phy_info_t)); i++) {
					if (phyinfo[i].phyid == phytype) {
						printf("-- phyid %x - %s\n",
						       phyinfo[i].phyid,
						       phyinfo[i].strid);
						_strcpy(info->phy_name, phyinfo[i].strid);

						info->phyname_init = 1;
						found = 1;
						break;
					}
				}

				if (!found) {
					printf("-- 0x%08x\n", phytype);
					strcpy(info->phy_name, "unknown");
					info->phyname_init = 1;
					break;
				}
			}
		}
	}

	if (phyaddr < 0)
		printf("-- No PHY device found.\n");

	info->phy_addr = phyaddr;

	return phyaddr;
}

/*
 * \brief   Initialize the MII interface controller
 * \param   System Clock Frequency (in MHz)
 * \warning The system clock in this case is the clock that drives
 *          the FEC logic.  This may be different from the speed at which
 *          the CPU is operating.
 *
 * Initialize the MII clock (EMDC) frequency. The desired MII clock is 2.5MHz:
 *
 * MII Speed Setting = System_Clock / (2.5MHz * 2)
 * (plus 1 to round up)
 */
void mii_init(struct fec_info_s *info, int sys_clk_mhz)
{
	int i = 0;
	static uint16_t status = 0;
	uint16_t linkgood = 0;
	ENET_MemMapPtr fecp;

	fecp = (ENET_MemMapPtr)(ENET_BASE_PTR);

	fecp->MSCR = ((sys_clk_mhz / 1000000) / 5) << 1;

	mii_discover_phy(info);

#define MCFFEC_TOUT_LOOP	5000
#ifdef CONFIG_AUTO_PHY_DISCOVERY
	while (i < MCFFEC_TOUT_LOOP) {
		status = 0;

		i++;
		/* Read PHY control register */
		mii_read(info->phy_addr, PHY_BMCR, &status);

		/* If phy set to autonegotiate, wait for autonegotiation done,
		 * if phy is not autonegotiating, just wait for link up.
		 */
		if ((status & PHY_BMCR_AUTON) == PHY_BMCR_AUTON) {
			linkgood = (PHY_BMSR_AUTN_COMP | PHY_BMSR_LS);
		} else {
			linkgood = PHY_BMSR_LS;
		}
		/* Read PHY status register */
		mii_read(info->phy_addr, PHY_BMSR, &status);
		if ((status & linkgood) == linkgood)
			break;
	}
	if (i >= MCFFEC_TOUT_LOOP) {
		printf("Link UP timeout\n");
	}
#else
	while (i < MCFFEC_TOUT_LOOP) {
		status = 0;
		i++;
		/* Read PHY status register */
		mii_read(info->phy_addr, PHY_BMSR, &status);
		if ((status & PHY_BMSR_LS) == PHY_BMSR_LS)
			break;

		usec_delay(1);
	}

	if (i >= MCFFEC_TOUT_LOOP) {
		printf("Link UP timeout\n");
	}

	mii_read(info->phy_addr, PHY_BMCR, &status);
	if (FECSPEED == _100BASET) {
		status |= PHY_BMCR_100MB;
	} else if (FECSPEED == _10BASET) {
		status &= ~PHY_BMCR_100MB;
		status |= PHY_BMCR_COL_TST;
	}
	if (FECDUPLEX == FULL) {
		status |= PHY_BMCR_DPLX;
	} else if (FECDUPLEX == HALF) {
		status &= ~PHY_BMCR_DPLX;
	}
	mii_write(info->phy_addr, PHY_BMCR, status);

#endif		/* CONFIG_AUTO_PHY_DISCOVERY */

	/* adapt to the duplex and speed settings of the phy */
	info->dup_spd = miiphy_duplex(info->phy_addr) << 16;
	info->dup_spd |= miiphy_speed(info->phy_addr);
}
