/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : dm9000_ea_lpc315x.c
 *    Description : DM9000A device driver x 16 bits
 *
 *    History :
 *    1. Date        : 27.8.2009
 *       Author      : Stanimir Bonev
 *       Description : initial revision.
 *    2. Date        : 5.10.2009
 *       Author      : Stanimir Bonev
 *       Description : Adapt for an Embedded Artsists LPC315x board
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "netif/dm9000_ea_lpc315x.h"

void DM9000A_init_16bit(void)
{
	/*
	 * Configure Chip-Select 1 on SMC for the DM9000.
	 * Note: These timings were calculated for MASTER_CLOCK = 90000000
	 *  according to the DM9000 timings.
	 */
	MPMCStaticConfig1   = 0x81;
	MPMCStaticWaitWen1  = 1;
	MPMCStaticWaitOen1  = 1;
	MPMCStaticWaitRd1   = 3;
	MPMCStaticWaitPage1 = 1;
	MPMCStaticWaitWr1   = 1;
	MPMCStaticWaitTurn1 = 2;
}


void DM9000A_reg_write(Int16U addr, Int16U data)
{
  // Set index reg
  iowr(DM9000A_CS_CMD_BASE_ADDR,addr);
  // wtite to data reg
  iowr(DM9000A_CS_DAT_BASE_ADDR,data);
}

Int16U DM9000A_reg_read(Int16U addr)
{
  // Set index reg
  iowr(DM9000A_CS_CMD_BASE_ADDR,addr);
  // read from data reg
  return(iord(DM9000A_CS_DAT_BASE_ADDR));
}

Int16U DM9000A_phy_reg_read(Int16U addr)
{
Int16U tmp;
  // Set Phy reg address
  DM9000A_reg_write(DM9000_EPAR,(addr & 0x1F) | 0x40);
  // Write command
  DM9000A_reg_write(DM9000_EPCR,EPCR_ERPRR | EPCR_EPOS);
  // wait until command done
  do
  {
    tmp = DM9000A_reg_read(DM9000_EPCR);
  }
  while(tmp & EPCR_ERRE);
  // clear read command
  DM9000A_reg_write(DM9000_EPCR,EPCR_EPOS);
  // read data
  tmp = DM9000A_reg_read(DM9000_EPDRH) << 8;
  tmp|= DM9000A_reg_read(DM9000_EPDRL);
  return(tmp);
}

void DM9000A_phy_reg_write(Int16U addr, Int16U data)
{
Int16U tmp;
  // Set Phy reg address
  DM9000A_reg_write(DM9000_EPAR,(addr & 0x1F) | 0x40);
  // load data
  DM9000A_reg_write(DM9000_EPDRH,data >> 8);
  DM9000A_reg_write(DM9000_EPDRL,data);
  // Write command
  DM9000A_reg_write(DM9000_EPCR,EPCR_ERPRW | EPCR_EPOS);
  // wait until command done
  do
  {
    tmp = DM9000A_reg_read(DM9000_EPCR);
  }
  while(tmp & EPCR_ERRE);
  // clear read command
  DM9000A_reg_write(DM9000_EPCR,EPCR_EPOS);
}
