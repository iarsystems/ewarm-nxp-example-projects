/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2016
 *
 *    File name   : gic_config.h
 *    Description : GICv1/v2 driver configuration file
 *
 *    History :
 *    1. Date        : January 2016
 *       Author      : Stoyan Choynev
 *       Description : Initial revision.
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#ifndef __GIC_CONFIG_H
#define __GIC_CONFIG_H

/** local definitions **/
#define GICD_BASE_ADDR 0x31001000 /*GIC Distributor address*/
#define GICC_BASE_ADDR 0x31002000 /*GIC CPU interface address*/

#define CPU_NUM 2

#endif /*__GIC_CONFIG_H*/
