/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : EMC_drv.h
 *    Description : SDRAM driver HY57V561620FTP-H 16MBx16bit
 *                  and NAND Flash k9F1208U0B  driver
 *    History :
 *    1. Date        : 6, March 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : June 2008
 *       Author      : Stoyan Choynev
 *       Description : SDRAM Driver ported HY57V561620FTP-H device
 *                     added NAND Flash Driver
 *
 *    $Revision: 28 $
 **************************************************************************/


#ifndef __EMC_DRV_H
#define __EMC_DRV_H
#define SYS_FREQ  48MHZ

void EMC_Init (void);
Boolean SDRAM_Test (void);
Boolean NAND_Test(void);

#endif // __EMC_DRV_H
