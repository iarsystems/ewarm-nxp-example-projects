/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 **************************************************************************//*!
 *
 * @file ConstDataStruct.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief This file contains all the association and configuration information
 *
 *****************************************************************************/
#include "type.h"
#include "ieee11073_nom_codes.h"
#include "ieee11073_dimstruct.h"
#include "mds_config.h"
#include "phdc_app.h"
/*****************************************************************************
 * Constant and Macro's - None
 *****************************************************************************/
#ifdef MDS_ATTR_SYS_TYPE
const TYPE g_MdcAttrSysType = 
{
	  MDC_PART_INFRA,
	  MDC_DEV_SPEC_PROFILE_BP
};
#endif

#ifdef MDS_ATTR_SYS_TYPE_SPEC_LIST
/* Type Per Var List */
const intu8 g_MdcAttrSysTypeSpecList[] =
{
    0x00, 0x01,
    0x00, 0x04,
    0x10, 0x07,           /* MDC_DEV_SPEC_PROFILE_BP */
    0x00, 0x01    
};
#endif
/* System Model */
#ifdef _HCS08_
const intu8 g_MdcAttrSysModel[] = 
#else
intu8 g_MdcAttrSysModel[] = 
#endif
{
    0x00, 0x18,
    'F','r','e','e','s','c','a','l','e',' ','S','e','m','i','c','o','n','d','u','c','t','o','r',0x0,
    0x00, 0x10,
    'M','e','d','i','c','a','l',' ','B','P','M',' ','D','e','v', 0x0
};
/* System Id */
#ifdef _HCS08_
const intu8 g_MdcAttrSysId[] = 
#else
intu8 g_MdcAttrSysId[] = 
#endif
{
  SYSTEM_ID
};

/* Production Spec */
#ifdef MDS_PROD_SPEC
#ifdef _HCS08_
const intu8 g_MdcAttrIdProdSpecn[] = 
#else
intu8 g_MdcAttrIdProdSpecn[] = 
#endif
{
    0x00, 0x01,
    0x00, 0x10,
    0x00, 0x01,
    0x11, 0x11,
    0x00, 0x0a,
    'F','S','L','T','H','0','0','0','0', 0x0
};
#endif

#ifdef MDS_SETTABLE_TIME
const MdsTimeInfo g_MdcTimeInfo = 
{
        (
#ifdef MDS_MGR_SET_TIME
        MDS_TIME_MGR_SET_TIME | 
#endif
#ifdef  MDS_HIRES_TIME
        MDS_TIME_CAPAB_HIGH_RES_RELATIVE_TIME |
#endif
#ifdef MDS_RELATIVE_TIME
        MDS_TIME_CAPAB_RELATIVE_TIME |
#endif
#ifdef MDS_SET_CLOCK
        MDS_TIME_CAPAB_SET_CLOCK |
#endif
#ifdef MDS_RTC
        MDS_TIME_CAPAB_REAL_TIME_CLOCK |
#endif
        0),
       MDC_TIME_SYNC_NONE,
       TIME_SYNC_ACCURACY_UNKNOWN,
       100,                         /* Absolute Time Resolution (1 second) */
       TIME_RESOLUTION_UNKNOWN,     /* Relative Time Resolution */
       TIME_RESOLUTION_UNKNOWN      /* High Res Time Resolution */
};
#endif /* MDS_SETTABLE_TIME */

#ifdef MDS_CONFIRM_TIMEOUT
/* Confirm timeout = 3 sec */
const RelativeTime g_MdcAttrConfirmTimeout = 0x00005dc0;
#endif /* MDS_CONFIRM_TIMEOUT */

/* Reg Ceritified Data List, as defined by Continua */
#ifdef _HCS08_
const intu8 g_MdcAttrRegCertDataList[] = 
#else
intu8 g_MdcAttrRegCertDataList[] = 
#endif
{    
    0x00, 0x02, /* count */
    0x00, 0x12,
    AUTH_BODY_CONTINUA, CONTINUA_REG_STRUCT,
    0x00, 0x02,
    0x00, 0x00,
    
    AUTH_BODY_CONTINUA, CONTINUA_VERSION_STRUCT,
    0x00, 0x08,
    0x01, 0x00,
    0x00, 0x01,
    0x00, 0x02,
    0x00, 0x07    /* PAN_BLOODPRESSURE_MONITOR */
};

/* Unit label string */
#ifdef _HCS08_
const intu8 g_Bp1_unit_label_string[] = 
#else
intu8 g_Bp1_unit_label_string[] = 
#endif
{
    0x00, 0x05,
    'm', 'm', 'H', 'g', 0x0   
};

/* Unit label string */
#ifdef _HCS08_
const intu8 g_Bp2_unit_label_string[] = 
#else
intu8 g_Bp2_unit_label_string[] = 
#endif
{
    0x00, 0x04,
    'b', 'p', 'm', 0x0   
};

/* Label String */
#ifdef _HCS08_
const intu8 g_label_string[] = 
#else
intu8 g_label_string[] = 
#endif
{
    0x00, 0x17,
    'B', 'l', 'o', 'o', 'd', ' ', 'P', 'r', 'e', 's', 's', 'u', 'r', 'e', \
    ' ', 'M', 'o', 'n', 'i', 't', 'o', 'r', 0x0
};

/* Metric Id List */
#ifdef _HCS08_
const intu8 g_Bp1_MetricId_list[] = 
#else
intu8 g_Bp1_MetricId_list[] = 
#endif
{
    0x00, 0x03, 
    0x00, 0x06,
    0x4A, 0x05,    /* MDC_PRESS_BLD_NONINV_SYS */
    0x4A, 0x06,    /* MDC_PRESS_BLD_NONINV_DIA */
    0x4A, 0x07     /* MDC_PRESS_BLD_NONINV_MEAN */
};
#ifdef _HCS08_
const intu8 g_Bp1_AttrValMap[] = 
#else
intu8 g_Bp1_AttrValMap[] = 
#endif
{  
    0x00, 0x02,    /* AttrValMap.count = 2 */
    0x00, 0x08,    /* AttrValMap.length = 8 */
    0x0A, 0x75, 0x00, 0x0A,   /* MDC_ATTR_NU_CMPD_VAL_OBS_BASIC | value length = 10 */
    0x09, 0x90, 0x00, 0x08    /* MDC_ATTR_TIME_STAMP_ABS | value length = 8 */
};
#ifdef _HCS08_
const intu8 g_Bp2_AttrValMap[] = 
#else
intu8 g_Bp2_AttrValMap[] = 
#endif
{  
    0x00, 0x02,    /* AttrValMap.count = 2 */
    0x00, 0x08,    /* AttrValMap.length = 8 */
    0x0A, 0x4C, 0x00, 0x02,   /* MDC_ATTR_NU_VAL_OBS_BASIC | value length = 2 */
    0x09, 0x90, 0x00, 0x08,   /* MDC_ATTR_TIME_STAMP_ABS | value length = 8 */
};

/* Metric Classes */
/* -> Handle = 1: Compound Metric (NUMERIC) for Systolic, Diastolic, MAP */
/* -> Handle = 2: Compound Metric (NUMERIC) for Pulse */
METRIC g_Metric[] = 
{
    {
        /* Numeric (Systolic, Diastolic, MAP) Class Handle */
        (intu16)0x0001,      
        /* TYPE */
        {
            MDC_PART_SCADA,
            MDC_PRESS_BLD_NONINV
        },
        /* Metric Spec Small */
        (intu16)(
        MSS_AVAIL_INTERMITTENT | MSS_AVAIL_STORED_DATA |
        MSS_UPD_APERIODIC | MSS_MSMT_APERIODIC | MSS_ACC_AGENT_INITIATED
        ),
        /* Optional Attributes */
        (intu16)(OPT_MET_STRUCT_SMALL | OPT_MET_UNIT_CODE | OPT_MET_ID_PHYS | OPT_MET_ATTRVALMAP), 
        /* SupplementalTypeList */
        NULL,
        /* Metric Struct Small */
        {
             MS_STRUCT_COMPOUND_FIX, 3
        },
        /* Measurement Status */
        0,  
        /* MetricIdPhysioList */
        {
            METRIC_ID_LIST,
            (OID_Type *)&g_Bp1_MetricId_list[0],
        },
        /* Metric ID Part */
        NOM_PART_METRIC,
        /* Unit Code */
        MDC_DIM_MMHG,
        /* Pointer to attribute val map */
        (AttrValMap *)&g_Bp1_AttrValMap[0],
        /* source handle reference*/
        0x0000,
        /* Label String */
        (octet_string*)&g_label_string,
        /* Unit Label String */
        (octet_string*)&g_Bp1_unit_label_string        
    },
    {
        /* Numeric (Pulse rate) Class Handle */
        (intu16)0x0002,      
        /* TYPE */
        {
            MDC_PART_SCADA,
            MDC_PULS_RATE_NON_INV
        },
        /* Metric Spec Small */
        (intu16)(
        MSS_AVAIL_INTERMITTENT | MSS_AVAIL_STORED_DATA |
        MSS_UPD_APERIODIC | MSS_MSMT_APERIODIC | MSS_ACC_AGENT_INITIATED
        ),
        /* Optional Attributes */
        (intu16)(OPT_MET_UNIT_CODE | OPT_MET_ATTRVALMAP), 
        /* SupplementalTypeList */
        NULL,
        /* Metric Struct Small */
        {
            MS_STRUCT_SIMPLE, 0
        },
        /* Measurement Status */
        0,  
        /* MetricIdPhysioList */
        {
            0,
            NULL,
        },
        /* Metric ID Part */
        NOM_PART_METRIC,
        /* Unit Code */
        MDC_DIM_BEAT_PER_MIN,
        /* Pointer to attribute val map */
        (AttrValMap *)&g_Bp2_AttrValMap[0],
        /* source handle reference*/
        0x0000,
        /* Label String */
        (octet_string*)&g_label_string,
        /* Unit Label String */
        (octet_string*)&g_Bp2_unit_label_string        
    },
};

/* Numeric classes */
const NUMERIC g_Numeric[] = 
{
    {
        /* Metric class Pointer */
        &g_Metric[0],   /* (Systolic, Diastolic, MAP) */
        /* Optional Attribute */       
        0,
        /* Accuracy - optional */  
        0x00000000
    },
    {
        /* Metric class Pointer */
        &g_Metric[1],   /* (Pulse rate) */ 
        /* Optional Attribute */       
        0,
        /* Accuracy - optional */  
        0x00000000
    }
};

/* Device Configuration */
const CONFIGURATION g_Configuration[] = 
{
    /* Configuration 1 */
    {
        /* Device Configuration ID - Standard Configuration */
        0x02BC, 
        
        /* Numeric object count */
        sizeof(g_Numeric)/sizeof(g_Numeric[0]),
        /* Pointer to an array of Numeric classes */
        (NUMERIC (*)[])&g_Numeric,

        /* RTSA object count */
        0,
        /* Pointer to an array of RTSA classes */
        NULL,

        /* Enumeration object count */
       	0,
        /* Pointer to an array of Enumeration classes */    	
      	NULL,

        /* Episoidic Scanner object count */
      	0,
        /* Pointer to an array of Episoidic Scanner classes */    	
	      NULL,

        /* Periodic Scanner object count */
       	0,
        /* Pointer to an array of Periodic Scanner classes */    	
	      NULL,
	    
        /* PM Store object count */
	      0,
        /* Pointer to an array of PM Store classes */	    
	      NULL	        
    }
};

/* MDS Object */
MDS g_Mds = 
{
    /* MDS Handle */   
    0x0000, 
#ifdef MDS_ATTR_SYS_TYPE
    /* Sys type */
   (TYPE *)&g_MdcAttrSysType,
#else
    /* Sys type */
   NULL,
#endif   
#ifdef MDS_ATTR_SYS_TYPE_SPEC_LIST	
    /* Type Per Var List */
    (TypeVerList*)&g_MdcAttrSysTypeSpecList,  
#else 
    /* Type Per Var List */
    NULL,   
#endif    
    /* System Model */
    (SystemModel*)&g_MdcAttrSysModel,
    /* System Id */
    (octet_string*)&g_MdcAttrSysId,
    /* device Configuration ID */
    0x02BC,    
    /* pointer to attribute val map */
    NULL,
#ifdef MDS_PROD_SPEC
    /* pointer to production spec */
    (ProductionSpec*)&g_MdcAttrIdProdSpecn,	
#else
    NULL,	
#endif /* MDS_PROD_SPEC */

#ifdef MDS_SETTABLE_TIME
    /* Mds Time Info */
    (MdsTimeInfo *)&g_MdcTimeInfo,
#else
    NULL,
#endif        

/* Absolute Time Stamp */
#ifdef MDS_ABSOLUTE_TIME   
    (AbsoluteTime*)&g_MdcAttrTimeAbs,
#else
    NULL,    
#endif /* MDS_ABSOLUTE_TIME */   

/* Relative Time */
	0x00000000,

/* Hi Resolution Relative Time */
 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

#ifdef MDS_ADJUST_ABSOLUTE_TIME
/* Absolute Date and Time Adjust */
 (AbsoluteTimeAdjust *)&g_MdcAbsTimeadjust,
#else
 NULL,
#endif 

/* Power Status */
#ifdef MDS_POWER_STATUS
	(PowerStatus *)&g_MdcAttrPowerStatus,
#else
  NULL.	
#endif /* MDS_POWER_STATUS */

#ifdef MDS_BATT_CHARGE	
  /* Battery Charge */  
	(intu16 *)&g_MdcAttrValBattCharge,
#else
  NULL,	
#endif /* MDS_BATT_CHARGE */

/* Battery Measure */
#ifdef MDS_BATT_REMAIN
   (BatMeasure *)&g_MdcAttrTimeBattRemain,
#else
   NULL,      
#endif /* MDS_BATT_REMAIN */

	/* pointer to Reg Certified Data List */
	(RegCertDataList *)&g_MdcAttrRegCertDataList,

/* Confirm Timeout  - 3 secs */
#ifdef MDS_CONFIRM_TIMEOUT
	(RelativeTime *)&g_MdcAttrConfirmTimeout,
#else
  NULL,	
#endif
    /* Configuration Selected */
    0,
    /* Configuration Count */  
    sizeof(g_Configuration)/sizeof(g_Configuration[0]),
    /* pointer to an array of configurations */
    (CONFIGURATION (*)[])&g_Configuration
    
};
/* DIM variable */
DIM g_DIM = 
{
    /* Pointer to MDS */
    &g_Mds,                 
    /* Device Configuration ID - Standard Configuration */ 
    0x02BC   
};

/* Data Proto List */
#ifdef _HCS08_
const intu8 g_DataProtoList[] = 
#else
intu8 g_DataProtoList[] = 
#endif
{
    0x00, 0x01,
    0x00, 0x2A,
    /* Data Proto ID = 20601 */
    0x50, 0x79,         
    /* Data Proto Length  */ 
    0x00, 0x26,         
	/* PROTOCOL_VERSION1 */
    0x80, 0x00, 0x00, 0x00,
    /* Encoding rules */
#if defined(ENCODE_XER)
	0xC0, 0x00,
#elif defined(ENCODE_PER)
	0xA0, 0x00,
#elif defined(ENCODE_MDER)
	0x80, 0x00,
#else
	0x80, 0x00,
#endif
    /* NOM_VERSION1 */
    0x80, 0x00, 0x00, 0x00,
    /* Functional units */
	0x00, 0x00, 0x00, 0x00,  /* Device Cannot Enter Test Association */
    /* SYS_TYPE_AGENT */
    0x00, 0x80, 0x00, 0x00,
    /* system_id */ 
    SYSTEM_ID,
    /* dev_config_id - Standard Configuration */
    0x02, 0xBC,
    /* data_req_mode_capab */ 
    0x00, 0x01, 
    MAX_AGENT_DATA_COUNT, MAX_MANAGER_DATA_COUNT,    
    /* optionList */
    0x00, 0x00, 0x00, 0x00
};
