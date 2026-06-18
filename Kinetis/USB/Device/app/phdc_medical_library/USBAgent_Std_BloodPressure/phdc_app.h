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

#include "hidef.h"          /* for EnableInterrupts macro */
#include "derivative.h"     /* include peripheral declarations */
#include "type.h"          /* User Defined Data Types */
#include "RealTimerCounter.h"
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
#include "user_config.h"
#include "ieee11073_dimstruct.h"
#include "ieee11073_nom_codes.h"
#include "MedAgentLibInterface.h"
#include "UsbShimAgent.h"
/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define KBI_STAT_MASK               (0x0F)

#define KBI_PTG1_MASK               (0x02)

#if (defined(_MCF51MM256_H) || defined(_MC9S08MM128_H) || (defined _MCF51JE256_H) || defined(_MC9S08JE128_H))
#define KBI_PTG2_MASK               (0x08)
#define KBI_PTG3_MASK               (0x04)
#else
#define KBI_PTG2_MASK               (0x04)
#define KBI_PTG3_MASK               (0x08)
#endif

#define SELECT_TIMEOUT      3000    /* 3 Second Specialization Select Timeout*/

 /*****************************************************************************
 * Global variables
 *****************************************************************************/
/* To record the status of the button pressed */
extern uint_8 kbi_stat;

#ifdef MDS_ABSOLUTE_TIME   
extern AbsoluteTime g_MdcAttrTimeAbs;
#endif

#ifdef MDS_ADJUST_ABSOLUTE_TIME
extern AbsoluteTimeAdjust g_MdcAbsTimeadjust;
#endif

#ifdef MDS_POWER_STATUS
extern PowerStatus g_MdcAttrPowerStatus;
#endif

#ifdef MDS_BATT_CHARGE	
/* Battery Charge: 100% charge */  
extern intu16 g_MdcAttrValBattCharge;
#endif

/* Battery Measure */
#ifdef MDS_BATT_REMAIN
extern BatMeasure g_MdcAttrTimeBattRemain;
#endif /* MDS_BATT_REMAIN */

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
extern void TestApp_Init(void);
extern void TestApp_Task(void);

#endif
