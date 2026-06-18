/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2016
 *
 *    File name   : timers_config.h
 *    Description : timers driver configuration file
 *
 *    History :
 *    1. Date        : January 2016
 *       Author      : Stoyan Choynev
 *       Description : Initial revision.
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#ifndef __TIMERS_CONFIG_H
#define __TIMERS_CONFIG_H

/** local definitions **/
#define PTMR_BASE_ADDR  0x00A00600 /*Private timer address*/
#define WDTMR_BASE_ADDR 0x00A00620 /*Watchdog timer address*/
#define GTMR_BASE_ADDR  0x00A00200 /*Global timer address*/

#endif /*__TIMERS_CONFIG_H*/
