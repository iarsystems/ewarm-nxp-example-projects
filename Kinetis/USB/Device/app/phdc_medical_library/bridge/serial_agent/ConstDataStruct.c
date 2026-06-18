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

/*****************************************************************************
 * Constant and Macro's - None
 *****************************************************************************/
#ifdef MDS_ATTR_SYS_TYPE
const TYPE g_MdcAttrSysType = 
{
	  MDC_PART_INFRA,
	  MDC_DEV_SPEC_PROFILE_TEMP
};
#endif

#ifdef MDS_ATTR_SYS_TYPE_SPEC_LIST
/* Type Per Var List */
const intu8 g_MdcAttrSysTypeSpecList[] =
{
    0x00, 0x01,
    0x00, 0x04,
    0x10, 0x08,
    0x00, 0x01    
};
#endif
/* System Model */
intu8 g_MdcAttrSysModel[] = 
{
    0x00, 0x18,
    'F','r','e','e','s','c','a','l','e',' ','S','e','m','i','c','o','n','d','u','c','t','o','r',0x0,
    0x00, 0x0c,
    'M','e','d','i','c','a','l',' ','D','e','v', 0x0
};
/* System Id */
intu8 g_MdcAttrSysId[] = 
{
    SYSTEM_ID
};

/* MDS Attribute Val Map */
#ifdef MDC_FIXED_DATA_FORMAT
const intu8 g_MdcAttrValMap[] = 
{
    0x00, 0x01,  /* Count */
    0x00, 0x04,  /* Length */
    0x0a, 0x4c,  /* Attribute Id */
    0x00, 0x02   /* Attribute data length */
};
#endif

/* Production Spec */
#ifdef MDS_PROD_SPEC
intu8 g_MdcAttrIdProdSpecn[] = 
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
MdsTimeInfo g_MdcTimeInfo = 
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
       TIME_RESOLUTION_UNKNOWN,     /* Absolute Time Resolution */
       TIME_RESOLUTION_UNKNOWN,     /* Relative Time Resolution */
       TIME_RESOLUTION_UNKNOWN      /* High Res Time Resolution */
};
#endif /* MDS_SETTABLE_TIME */

#ifdef MDS_ADJUST_ABSOLUTE_TIME
AbsoluteTimeAdjust g_MdcAbsTimeadjust = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

#ifdef MDS_POWER_STATUS
PowerStatus g_MdcAttrPowerStatus = ON_MAINS;
#endif

#ifdef MDS_BATT_CHARGE	
/* Battery Charge: 100% charge */  
intu16 g_MdcAttrValBattCharge = 100;
#endif

/* Battery Measure */
#ifdef MDS_BATT_REMAIN
BatMeasure g_MdcAttrTimeBattRemain = 
	{
	    /* 1 Day of Battery Remaining */
	    0x1,        
	    MDC_DIM_DAY
	};
#endif /* MDS_BATT_REMAIN */

#ifdef MDS_CONFIRM_TIMEOUT
/* Confirm timeout = 3 sec */
RelativeTime g_MdcAttrConfirmTimeout = 0x00005dc0;
#endif /* MDS_CONFIRM_TIMEOUT */

#ifdef MDS_ABSOLUTE_TIME   
AbsoluteTime g_MdcAttrTimeAbs = 
{
  
       0x20,    /* Century */
       0x09,    /* Year */                   
       0x07,    /* Month */
       0x22,    /* Day */
       0x19,    /* Hour */
       0x30,    /* Minute */
       0x10,    /* Second */
       0x00     /* Second Fraction */
};
#endif /* MDS_ABSOLUTE_TIME */

/* Reg Ceritified Data List, as defined by Continua */
intu8 g_MdcAttrRegCertDataList[] = 
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
    0x00, 0x08
};

/* Temperature Attribute Val Map */
const intu8 g_AttrValMap[] = 
{
    0x00, 0x02,  /* Count */
    0x00, 0x08,  /* Length */
    0x0a, 0x56,  /* Attribute Id */
    0x00, 0x04,  /* Attribute data length */
    0x09, 0x90,  /* Attribute Id */
    0x00, 0x08   /* Attribute data length */
};

/* Enumeration Class Attribute Val Map */
#ifdef MDS_ENUMERATION
const intu8 g_EnumAttrValMap[] = 
{
    0x00, 0x02,  /* Count */
    0x00, 0x08,  /* Length */
    0x0A, 0x49,  /* Attribute Id */
    0x00, 0x02,  /* Attribute data length */
    0x09, 0x90,  /* Attribute Id */
    0x00, 0x08   /* Attribute data length */
};
#endif

/* Supplemental Type List */
intu8 g_Supplemental_type_list[] = 
{
    0x00, 0x01,
    0x00, 0x04,
    0x00, 0x02, /* MDC_PART_SCADA */
    0x4C, 0x34  /* MDC_MODALITY_FAST */
};

/* Unit label string */
const intu8 g_unit_label_string[] = 
{
    0x00, 0x08,
    'D', 'e', 'g', 'r', 'e', 'e', 'C', 0x0   
};

/* Label String */
const intu8 g_label_string[] = 
{
    0x00, 0x0c,
    'T', 'h', 'e', 'r', 'm', 'o', 'm', 'e', 't', 'e', 'r', 0x0
};

/* Metric Id List */
intu8 g_MetricId_list[] = 
{
    0x00, 0x03, 
    0x00, 0x06,
    0x4A, 0x05,
    0x4A, 0x06,
    0x4A, 0x07
};
/* Metric Id */
intu8 gMetricId[] = 
{
    0xE0, 0x08
};

/* Metric Classes */
#ifdef MDS_METRIC
METRIC g_Metric[] = 
{
    {
        /* Numeric(Blood Pressure) Class Handle */
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
        (intu16)(OPT_MET_UNIT_CODE | OPT_MET_ID_PHYS), 
        /* SupplementalTypeList */
        (SupplementalTypeList*)&g_Supplemental_type_list,
        /* Metric Struct Small */
        {
            MS_STRUCT_COMPOUND, 3
        },
        /* Measurement Status */
        MS_VALIDATED_DATA,  
        /* MetricIdPhysioList */
        {
            METRIC_ID_LIST,
            (OID_Type *)&g_MetricId_list[0],
        },
        /* Metric ID Part */
        NOM_PART_OBJ,
        /* Unit Code */
        MDC_DIM_MMHG,
        NULL,
        /* source handle reference*/
        0x0002,
        /* Label String */
        (octet_string*)&g_label_string,
        /* Unit Label String */
        (octet_string*)&g_unit_label_string        
    },
    {
        /* Numeric(Temperature) Class Handle */
        (intu16)0x0002,      
        /* TYPE */
        {
            MDC_PART_SCADA,
            MDC_TEMP_BODY
        },
        /* Metric Spec Small */
        (intu16)(
        MSS_AVAIL_INTERMITTENT | MSS_AVAIL_STORED_DATA |
        MSS_UPD_APERIODIC | MSS_MSMT_APERIODIC | MSS_ACC_AGENT_INITIATED
        ),
        /* Optional Attributes */
        (intu16)(OPT_MET_UNIT_CODE | OPT_MET_ATTRVALMAP), 
        /* SupplementalTypeList */
        (SupplementalTypeList*)&g_Supplemental_type_list,
        /* Metric Struct Small */
        {
            MS_STRUCT_SIMPLE, 0
        },
        /* Measurement Status */
        MS_VALIDATED_DATA,  
        /* MetricIdPhysioList */
        {
            0,
            (OID_Type *)&gMetricId
        },
        /* Metric ID Part */
        NOM_PART_OBJ,
        /* Unit Code */
        MDC_DIM_DEGC,
        /* Attribute Val Map */
        (AttrValMap*)&g_AttrValMap,
        /* source handle reference*/
        0x0001,
        /* Label String */
        (octet_string*)&g_label_string,
        /* Unit Label String */
        (octet_string*)&g_unit_label_string        
    },
    {
        /* Enumeration Class Handle */
        (intu16)0x0007,      
        /* TYPE */
        {
            MDC_PART_PHD_HF,
            200,
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
            0, 0
        },
        /* Measurement Status */
        MS_VALIDATED_DATA,  
        /* MetricIdPhysioList */
        {
            0,
            0 
        },
        /* Metric ID Part */
        NOM_PART_OBJ,
        /* Unit Code */
        0,
        /* Attribute Val Map */
        (AttrValMap*)&g_EnumAttrValMap,
    },
    {
        /* Rtsa Class Handle */
        (intu16)0x0008,      
        /* TYPE */
        {
            MDC_PART_SCADA,
            MDC_TEMP_BODY
        },
        /* Metric Spec Small */
        (intu16)(
        MSS_AVAIL_INTERMITTENT | MSS_AVAIL_STORED_DATA |
        MSS_UPD_APERIODIC | MSS_MSMT_APERIODIC | MSS_ACC_AGENT_INITIATED
        ),
        /* Optional Attributes */
        (intu16)(0), 
    }
};
#endif
/* Numeric classes */
#ifdef MDS_NUMERIC
const NUMERIC g_Numeric[] = 
{
    {
        /* Metric class Pointer */
        &g_Metric[0], 
        /* Optional Attribute */       
        1,
        /* Accuracy */  
        0x00000001
    },
    {
        /* Metric class Pointer */
        &g_Metric[1], 
        /* Optional Attribute */       
        0,
        /* Accuracy */  
        0x00000001
    }
};
#endif

/* RTSA Object Class */
#ifdef MDS_RTSA
/* Simple Sa Observed Value */ 
intu8 g_RtsaSimpSaObsVal[] = 
{
    0x00, 0x0a,
    0x01, 0x02,        
    0x03, 0x04,        
    0x05, 0x06,        
    0x07, 0x08,            
    0x09, 0x0a    
};
/* Scale Range Spec */
ScaleRangeSpec8 g_scale8 = 
{
    (intu32)-45,50,0,190
};
const RTSA g_Rtsa[] = 
{
    /* Metric class Pointer */
    &g_Metric[3],
    /* sample period */    
    0x00000002,
    /* Simple Sa Observed Value */ 
    (octet_string*)&g_RtsaSimpSaObsVal,
    /* Scale Range Spec */
    0x00,
    (ScaleRangeSpec8 *)&g_scale8,
    /* Sa Spec */
    0x00a,          /*array size */
    0x08, 31,        /* sample type */
    SMOOTH_CURVE    /* sa flags */
};
#endif

/* Handle Attribute Val Map */
const intu8 g_HandleAttributeValMap[] = 
{
    0x00, 0x01,  /* Handle Count */
    0x00, 0x0e,  /* length */    
    0x00, 0x02,  /* Handle */
    0x00, 0x02,  /* Attribute Count */
    0x00, 0x08,  /* Length */
    0x0a, 0x56,  /* Attribute Id */
    0x00, 0x04,  /* Attribute data length */
    0x09, 0x90,  /* Attribute Id */
    0x00, 0x08   /* Attribute data length */
};

/* Scanner base class */    
#ifdef MDS_SCANNER
SCANNER g_Scanner[] = 
{
    {
        /* Episoidic class handle */
        5,
        /* Operational Stat */ 
        0,
        /* Handle Attribute Val Map */
        (HandleAttrValMap*)&g_HandleAttributeValMap,
    },
    {
        /* Periodic class handle */
        3, 
        /* Operational Stat */ 
        0,
        /* Handle Attribute Val Map */
        (HandleAttrValMap*)&g_HandleAttributeValMap,
    },
    
};
#endif    

/* Configuration scanner class */    
#ifdef MDS_CFGSCANNER    
CFGSCANNER g_CfgScanner[] = 
{
    {
        /* Optional Attributes */
        3,
        /* pointer to scanner class */
        (SCANNER*)&g_Scanner[0],
        /* confirm mode */
        0x0001,     
        /* confirm timeout - 2 secs */
        16000,
        /* transmit window */      
        0x0001
    },
    {
        /* Optional Attributes */
        3,
        /* pointer to scanner class */
        (SCANNER*)&g_Scanner[1],
        /* confirm mode */
        0x0001,
        /* confirm timeout - 2 secs */
        16000,
        /* transmit window */      
        0x0001
    },
};
#endif

/* Episoidic scanner object class */
#ifdef MDS_EPISOIDIC_SCANNER    
EPICFGSCANNER g_EpiScanner[] = 
{
    /* Optional Attributes */
    OPT_EPISCAN_MIN_RPT_INT,
    /* pointer to configuration scanner class */
    (CFGSCANNER*)&g_CfgScanner[0],
    /* minumum reporting interval - 2 secs */
    16000    
};
#endif

/* Periodic scanner object class */
#ifdef MDS_PERIODIC_SCANNER
PERICFGSCANNER g_PeriScanner[] = 
{
    /* pointer to configuration scanner class */
    (CFGSCANNER*)&g_CfgScanner[1],
    /* Scan reporting period - 1 sec */
    8000        
};
#endif
/* PM Segment Entry Map */
intu8 g_PmSegEntryMap[] = 
{
    /* Segment entry header - SEG_ELEM_HDR_ABSOLUTE_TIME */
    0x80, 0x00,
    /* number of elements in an entry */ 
    0x00, 0x01,
    /* length */
    0x00, 0x14,
    /* Object Class id - Numeric */
    0x00, 0x06, 
    /* TYPE */
    0x00, 0x02, /* MDC_PART_SCADA */ 
    0x4B, 0x5C, /* MDC_TEMP_BODY */
    /* Object handle */
    0x00, 0x02, 
    /* Attr Val Map */
    0x00, 0x02,  /* Count */
    0x00, 0x08,  /* Length */
    0x0a, 0x56,  /* Attribute Id */
    0x00, 0x04,  /* Attribute data length */
    0x09, 0x90,  /* Attribute Id */
    0x00, 0x08   /* Attribute data length */     
};
/* segment 1 label string */
intu8 g_PmSeg_label_string1[] = 
{
    0x00, 0x04,
    's', 'e', 'g', '1'
};
/* segment 2 label string */
intu8 g_PmSeg_label_string2[] = 
{
    0x00, 0x04,
    's', 'e', 'g', '2'
};
/* Segment statistics */
intu8 g_SegStat[] = 
{
    0x00, 0x01,
    0x00, 0x12,
    0x00, 0x01,
    0x00, 0x0E, /* handle + msr*/
    0x00, 0x01,
    0xFB, 0x38, 0x75, 0x20,
    0x20, 0x09, 0x08, 0x09,
    0x03, 0x29, 0x00, 0x00
};
/* PM Segment */
#ifdef MDS_PMSTORE    
PMSEGMENT g_PmSegment[] = 
{
    {
        /* optional attribute flag */    
        0x01FF, 
        /* instance number */
        0x0001, 
        /* PM Segment entry map */
        (PmSegmentEntryMap*)&g_PmSegEntryMap,
        /* Person ID */
    #ifdef MULTI_PERSON_SUPPORT
        0x0001, 
    #endif    
        /* operational state */    
        0x0000,
        /* Sample period */ 
        0x05,  
        /* segment label string */
        (octet_string*)&g_PmSeg_label_string1,
        /* Segment Start Time */
        0x20, 0x09, 0x09, 0x13, 0x02, 0x00, 0x00, 0x00, 
        /* Segment End Time */
        0x20, 0x09, 0x09, 0x13, 0x04, 0x00, 0x00, 0x00, 
        /* Date and time adjustment */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        /* usage count */
        0x00, 
        /* Segment Statistics */
        (SegmentStatistics*)&g_SegStat,
        /* pointer to Segment data */
        NULL, 
        /* Confirm timeout = 4 secs */
        0x00007D00,
        /* Transfer timeout = 4 secs */    
        0x00007D00  
    },
    {
        
        /* optional attribute flag */    
        0x01FF, 
        /* instance number */
        0x0002, 
        /* PM Segment entry map */
        (PmSegmentEntryMap*)&g_PmSegEntryMap,
        /* Person ID */
    #ifdef MULTI_PERSON_SUPPORT
        0x0002,
    #endif    
        /* operational state */    
        0x0000, 
        /* Sample period */ 
        0x05, 
        /* segment label string */
        (octet_string*)&g_PmSeg_label_string2,
        /* Segment Start Time */
        0x20, 0x09, 0x09, 0x13, 0x02, 0x00, 0x00, 0x00, 
        /* Segment End Time */
        0x20, 0x09, 0x09, 0x13, 0x04, 0x00, 0x00, 0x00, 
        /* Date and time adjustment */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        /* usage count */
        0x00, 
        /* Segment Statistics */
        (SegmentStatistics*)&g_SegStat,
        /* pointer to Segment data */
        NULL, 
        /* Confirm timeout = 4 secs */
        0x00007D00,
        /* Transfer timeout = 4 secs */    
        0x00007D00
    }
};

/* PM Store Label String */    
const intu8 g_pm_label_string[] = 
{
    0x00, 0x0b,
    't', 'e', 'm', 'p', 'e', 'r', 'a', 't', 'u', 'r', 'e'
};

/* PM Store Object Class */
PMSTORE g_PMStore[] = 
{
    /* optinal attribute flag */
    0x0f,
    /* PM Store Object handle */     
    0x04,
    /* PM Store Capability */ 
    (
    #ifdef MULTI_PERSON_SUPPORT 
        PMSC_MULTI_PERSON | 
    #endif 
    #ifdef SAMPLE_PERIOD
    PMSC_PERI_SEG_ENTRIES | /* if sample period attr present in pm store 
                               or in all pm segments, then this shud be set */
    #endif
    PMSC_CLEAR_SEGM_REMOVE | PMSC_CLEAR_SEGM_BY_TIME_SUP | 
    PMSC_EPI_SEG_ENTRIES | PMSC_CLEAR_SEGM_BY_LIST_SUP), 
    /* sampling algorithm */
    ST_ALG_NO_DOWNSAMPLING, 
    /* max pm segments entries */
    0x04, 
    /* actual num of segment entries presently used */
    0x02, 
    /* operational state, if data is actively added it should be 0x01 */
    0x00, 
    /* PM Store Label String */    
    (octet_string*)&g_pm_label_string,
    /* Sample Period */
    0x000000ff,
    /* num of pm segments instantiated */
    0x02, 
    /* Clear segment timeout */
    0xFFFFFFFF,
    /* PM Segment Count */
    0x02,
    /* Pointer to array of PM segments */
    (PMSEGMENT (*)[])&g_PmSegment
};
#endif

/* Enumeration Object Class */
#ifdef MDS_ENUMERATION
ENUMERATION g_Enum[] = 
{
    /* Pointer to metric class */
    &g_Metric[2],
    /* operational attribute flag */
    1,
    /* Nom Partition */
    NOM_PART_SITES
};            
#endif

/* Device Configuration */
const CONFIGURATION g_Configuration[] = 
{
    /* Configuration 1 */
    {
        /* Device Configuration ID */
        0x4000, 
        
#ifdef MDS_NUMERIC
        /* Numeric object count */
        sizeof(g_Numeric)/sizeof(g_Numeric[0]),
        /* Pointer to an array of Numeric classes */
        (NUMERIC (*)[])&g_Numeric,
#else
        /* Numeric object count */
        0,
        /* Pointer to an array of Numeric classes */
        NULL,
#endif

#ifdef MDS_RTSA
        /* RTSA object count */
        sizeof(g_Rtsa)/sizeof(g_Rtsa[0]),
        /* Pointer to an array of RTSA classes */
        (RTSA (*)[])&g_Rtsa,
#else
        /* RTSA object count */
        0,
        /* Pointer to an array of RTSA classes */
        NULL,
#endif

#ifdef MDS_ENUMERATION
        /* Enumeration object count */
    	sizeof(g_Enum)/sizeof(g_Enum[0]),
        /* Pointer to an array of Enumeration classes */    	
    	(ENUMERATION (*)[])&g_Enum,
#else
        /* Enumeration object count */
    	0,
        /* Pointer to an array of Enumeration classes */    	
    	NULL,
#endif

#ifdef MDS_EPISOIDIC_SCANNER
        /* Episoidic Scanner object count */
    	sizeof(g_EpiScanner)/sizeof(g_EpiScanner[0]),
        /* Pointer to an array of Episoidic Scanner classes */    	
	    (EPICFGSCANNER (*)[])&g_EpiScanner,
#else
        /* Episoidic Scanner object count */
    	0,
        /* Pointer to an array of Episoidic Scanner classes */    	
	    NULL,
#endif

#ifdef MDS_PERIODIC_SCANNER 
        /* Periodic Scanner object count */
    	sizeof(g_PeriScanner)/sizeof(g_PeriScanner[0]),
        /* Pointer to an array of Periodic Scanner classes */    	
	    (PERICFGSCANNER (*)[])&g_PeriScanner,
#else
        /* Periodic Scanner object count */
    	0,
        /* Pointer to an array of Periodic Scanner classes */    	
	    NULL,

#endif
	    
#ifdef MDS_PMSTORE
        /* PM Store object count */
	    sizeof(g_PMStore)/sizeof(g_PMStore[0]),
        /* Pointer to an array of PM Store classes */	    
	    (PMSTORE (*)[])&g_PMStore
#else
        /* PM Store object count */
	    0,
        /* Pointer to an array of PM Store classes */	    
	    NULL	    
#endif	    
    },
    /* Configuration 2 */
    {
        /* Device Configuration ID */
        0x4001,
         
#ifdef MDS_NUMERIC
        /* Numeric object count */
        sizeof(g_Numeric)/sizeof(g_Numeric[0]),
        /* Pointer to an array of Numeric classes */
        (NUMERIC (*)[])&g_Numeric,
#else
        /* Numeric object count */
        0,
        /* Pointer to an array of Numeric classes */
        NULL,        
#endif

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

#ifdef MDS_PERIODIC_SCANNER 
        /* Periodic Scanner object count */
    	  sizeof(g_PeriScanner)/sizeof(g_PeriScanner[0]),
        /* Pointer to an array of Periodic Scanner classes */    	
	      (PERICFGSCANNER (*)[])&g_PeriScanner,
#else
        /* Periodic Scanner object count */
        0,
        /* Pointer to an array of Periodic Scanner classes */    	
	      NULL,
#endif

#ifdef MDS_PMSTORE
        /* PM Store object count */
	      sizeof(g_PMStore)/sizeof(g_PMStore[0]),
        /* Pointer to an array of PM Store classes */	    
	      (PMSTORE (*)[])&g_PMStore
#else
        /* PM Store object count */
	      0,
        /* Pointer to an array of PM Store classes */	    
	      NULL
#endif
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
    0x4000,    
#ifdef MDC_FIXED_DATA_FORMAT
    /* pointer to attribute val map */
	  (AttrValMap*)&g_MdcAttrValMap,
#else
    /* pointer to attribute val map */
    NULL,
#endif /* MDC_FIXED_DATA_FORMAT */

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
    2,
    /* pointer to an array of configurations */
    (CONFIGURATION (*)[])&g_Configuration
    
};
/* DIM variable */
DIM g_DIM = 
{
    /* Pointer to MDS */
    &g_Mds,                 
    /* Device Configuration ID */ 
    EXTENDED_CONFIG_START   
};

/* Data Proto List */
intu8 g_DataProtoList[] = {
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
    /* dev_config_id */
    0x40, 0x00,
    /* data_req_mode_capab */ 
    0x00, 0x01, 
    MAX_AGENT_DATA_COUNT, MAX_MANAGER_DATA_COUNT,    
    /* optionList */
    0x00, 0x00, 0x00, 0x00
};
