/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2009 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 **************************************************************************//*!
 *
 * @file phdc_app.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains Macro's and functions needed by the PAN Application
 *
 *****************************************************************************/

#ifndef _PHDC_APP_H
#define _PHDC_APP_H

#include <hidef.h>          /* for EnableInterrupts macro */
#include <derivative.h>     /* include peripheral declarations */
#include <type.h>          /* User Defined Data Types */
#include <RealTimerCounter.h>
/* skip the inclusion in dependency statge */
#ifndef __NO_SETJMP
   #include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>			
#include <type.h>
#include <error.h>
#include <stack.h>
#include <mempool.h>
#include <user_config.h>
#include <ieee11073_dimstruct.h>
#include <ieee11073_nom_codes.h>
#include <MedAgentLibInterface.h>
#include <UsbShimAgent.h>
/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define  KBI_STAT_MASK              (0x0F)

#define SEND_BPM_MSR                (0x02)
#define SEND_TEMPERATURE_MSR        (0x04)
#define SEND_EPISOIDIC_SCANNER_MSR  (0x08)

#define SELECT_TIMEOUT      3000    /* 3 Second Specialization Select Timeout*/

uint_8 g_SegmDataEvent[] = 
{
    0x00, 0x01,             /* segment instance number */
    0x00, 0x00, 0x00, 0x00, /* segment entry index number */
    0x00, 0x00, 0x00, 0x04, /* segment entry count */
    0xC0, 0x00,             /* segment event status */
    0x00, 0x50,             /* segment data length */
    
    /* segment data */    
    0x20, 0x09, 0x08, 0x09,
    0x03, 0x30, 0x00, 0x00,
    0xFB, 0x38, 0x75, 0x20,
    0x20, 0x09, 0x08, 0x09,
    0x03, 0x30, 0x00, 0x00,

    0x20, 0x09, 0x08, 0x09,
    0x04, 0x30, 0x00, 0x00,
    0xFB, 0x40, 0x75, 0x20,
    0x20, 0x09, 0x08, 0x09,
    0x04, 0x30, 0x00, 0x00,
 
    0x20, 0x09, 0x08, 0x09,
    0x05, 0x30, 0x00, 0x00,
    0xFB, 0x48, 0x75, 0x20,
    0x20, 0x09, 0x08, 0x09,
    0x05, 0x30, 0x00, 0x00,
 
    0x20, 0x09, 0x08, 0x09,
    0x06, 0x30, 0x00, 0x00,
    0xFB, 0x50, 0x75, 0x20,
    0x20, 0x09, 0x08, 0x09,
    0x06, 0x30, 0x00, 0x00
};

 /*****************************************************************************
 * Global variables
 *****************************************************************************/
/* To record the status of the button pressed */
extern uint_8 kbi_stat;
/*****************************************************************************
 * Global Functions
 *****************************************************************************/
extern void TestApp_Init(void);
extern void TestApp_Task(void);

#endif
