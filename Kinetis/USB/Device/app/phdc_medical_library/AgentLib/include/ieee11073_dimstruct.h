/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************//*!
 *
 * @file ieee11073_dimstruct.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file Defines DIM Structure for Ieee11073 Agent Library
 *
 *****************************************************************************/
#ifndef _IEEE11073_DIMSTRUCT_H
#define _IEEE11073_DIMSTRUCT_H

/******************************************************************************
 * Includes
 *****************************************************************************/
#ifdef LITTLE_ENDIAN
#include <ieee11073_phd_types_kinetis.h>
#else
#include "ieee11073_phd_types.h"
#endif

/******************************************************************************
 * Types
 *****************************************************************************/
#pragma pack(1)
/* Scanner Data Structure */
typedef struct
{
	/* Handle */
	HANDLE MdcAttrIdHandle;
	/* Operational Stat */
	OperationalState MdcAttrOpStat;
	/* Pointer to handle attribute val map */
	HandleAttrValMap *pMdcAttrScanHandleAttrValMap;
}SCANNER, *PSCANNER;

/* Configuration Scanner Data Structure */
typedef struct
{
    /* optional/conditional attrubute flag */
    intu8 OptionalAttributeFlag;
    #define OPT_CFGSCAN_CONFIRM_TIMEOUT     0x01
    #define OPT_CFGSCAN_TRANS_WINDOW        0x02
    /* Pointer to scanner base class */
	PSCANNER pScanner;
	/* Confirm mode flag */
	ConfirmMode MdcAttrConfirmMode;
	/* Confirm Timeout */
	RelativeTime MdcAttrConfirmTimeout;			/* Conditional */
	/* transmit window */
	intu16 MdcAttrTxWind;						/* Optional */
}CFGSCANNER, *PCFGSCANNER;

/* Periodic Configuration Scanner Data Structure */
typedef struct
{
	/* pointer to configuration scanner base class */
	PCFGSCANNER pCfgScanner;
	/* Periodic scanner reporting interval */
	RelativeTime MdcAttrScanRepPd;
	/* Timer Index */
	intu8 TimerIndex;
}PERICFGSCANNER, *PPERICFGSCANNER;


/* Episoidic Configuration Scanner Data Structure */
typedef struct
{
    /* optional/conditional attrubute flag */
    intu8 OptionalAttributeFlag;
    #define OPT_EPISCAN_MIN_RPT_INT     0x01
	/* pointer to configuration scanner base class */
	PCFGSCANNER pCfgScanner;
	/* minimum reporting interval of the episodic scanner */
	/* Optional */
	RelativeTime MdcAttrScanRepPdMin;			
	/* Timer Index */
	intu8 TimerIndex;
}EPICFGSCANNER, *PEPICFGSCANNER;

/* Metric Data Structure */
typedef struct
{
	/* Handle */
	HANDLE MdcAttrIdHandle;
	/* Attribute Id type */
	TYPE MdcAttrIdType;
	/* Metric spec small */
	MetricSpecSmall MdcAttrMetricSpecSmall;
    /* optional/conditional attrubute flag */
    intu16 OptionalAttributeFlag;
    #define OPT_MET_SUPP_TYPES      0x0001
    #define OPT_MET_STRUCT_SMALL    0x0002
    #define OPT_MET_MSMT_STAT       0x0004
    #define OPT_MET_ID_PHYS         0x0008
    #define OPT_MET_ID_PART         0x0010
    #define OPT_MET_UNIT_CODE       0x0020
    #define OPT_MET_SRC_HNDL_REF    0x0040
    #define OPT_MET_ID_LABEL        0x0080
    #define OPT_MET_UNIT_LABEL      0x0100
    #define OPT_MET_MSMT_ACTIVE     0x0200
    #define OPT_MET_ATTRVALMAP      0x0400

	/* Pointer to Supplemental Type list */
	SupplementalTypeList *pMdcAttrSupplementalTypes;	/* Optional */
	/* Metric spec small */
	MetricStructureSmall MdcAttrMetricStructSmall;		/* Optional */
	/* Measurement status */
	MeasurementStatus MdcAttrMsmtStat;					/* Optional */
	struct {
		intu8 ID_type;
        #define METRIC_ID			(0)
        #define METRIC_ID_LIST		(1)
		union
		{
			/* Metric Physio ID */
			OID_Type *pMdcAttrIdPhysio;					/* Optional */
			/* Metric Physio ID list */
			MetricIdList *pMdcAttrIdPhysioList;
		}u;
	}MetricIdPhysioList;
	/* Metric ID partition */
	NomPartition MdcAttrMetricIdPart;					/* Optional */
	/* Unit code */
	OID_Type MdcAttrUnitCode;							/* Optional */
	/* Pointer to attribute val map */
	AttrValMap *pMdcAttrAttributeValMap;				/* Conditional */
	/* Source handle reference */
	HANDLE MdcAttrSourceHandleRef;						/* Optional */
	/* Label String */
	octet_string *pMdcAttrIdLabelString;				/* Optional */
	/* Unit Label string */
	octet_string *pMdcAttrUnitLabelString;				/* Optional */
	/* Measurement active period */
	FLOAT_Type MdcAttrTimePdMsmtActive;					/* Optional */
}METRIC, *PMETRIC;

/* Real Time Sample Array Data Structure */
typedef struct
{
	/* Pointer to metric base class */
	PMETRIC pMetric;
	/* Sample time period */
	RelativeTime MdcAttrTimePdSamp;
	/* Pointer to sample observed val */
	octet_string *pMdcAttrSimpSaObsVal;
	/* Scale range specification */
	struct {
		intu8 scale_range_spec_type;
		union
		{			
			ScaleRangeSpec8 *pMdcAttrScaleSpecnI8;
			ScaleRangeSpec16 *pMdcAttrScaleSpecnI16;
			ScaleRangeSpec32 *pMdcAttrScaleSpecnI32;
		}u;
	}ScaleRangeSpec;
	/* Sample specification */
	SaSpec MdcAttrSaSpecn;
}RTSA, *PRTSA;

/* Numeric Data Structure */
typedef  struct
{
	/* Pointer to metric base class */
	PMETRIC pMetric;
    /* optional/conditional attrubute flag */
    intu8 OptionalAttributeFlag;
    #define OPT_NUMERIC_ACCURACY        0x01
	/* Numeric measurement accuracy */
	FLOAT_Type MdcAttrNuAccurMsmt;						/* Optional */
}NUMERIC, *PNUMERIC;

/* Enumeration Data Structure */
typedef struct
{
	/* Pointer to metric base class */
	PMETRIC pMetric;
    /* optional/conditional attrubute flag */
    intu8 OptionalAttributeFlag;
    #define OPT_ENUM_OBS_VAL_PART       0x01
    /* Enum observed val partition */
	NomPartition MdcAttrEnumObsValPart;					/* Optional */
}ENUMERATION, *PENUMERATION;


#define INVALID_PMSEG_INST  0xFFFF
/* PM-Segment Data Structure */
typedef struct
{
    /* optional/conditional attrubute flag */
    intu16 OptionalAttributeFlag;

    #define OPT_PMSEG_PERSON_ID         0x0001
    #define OPT_PMSEG_SAMPLE_PERIOD     0x0002
    #define OPT_PMSEG_LABEL             0x0004
    #define OPT_PMSEG_ABS_TIME          0x0008
    #define OPT_PMSEG_DATE_TIME_ADJUST  0x0020
    #define OPT_PMSEG_USAGE_CNT         0x0040
    #define OPT_PMSEG_SEG_STATISTICS    0x0080
    #define OPT_PMSEG_CONFIRM_TIMEOUT   0x0100
	
	/* Segment instance number */
	InstNumber MdcAttrIdInstNo;
	/* PM Segment Entry map */
	PmSegmentEntryMap* pMdcAttrPmSegMap;
	/* Person ID */
	PersonId MdcAttrPmSegPersonId;
	/* Operational State */
	OperationalState MdcAttrOpStat;
	/* Sample time period */
	RelativeTime MdcAttrTimePdSamp;
	/* Pointer to PM Segment Label String */
	octet_string* pMdcAttrPmSegLabelString;				/* Optional */
	/* PM Segment Start Time */
	AbsoluteTime MdcAttrTimeStartSeg;					/* Optional */
	/* PM Segment End Time */
	AbsoluteTime MdcAttrTimeEndSeg;						/* Optional */
	/* Absolute Time Adjust */
	AbsoluteTimeAdjust MdcAttrTimeAbsAdjust;
	/* Segment Usage count */
	intu32 MdcAttrsegUsageCnt;							/* Optional */
	/* Pointer to segment statistics */
	SegmentStatistics* pMdcAttrSegStats;				/* Optional */
	/* Pointer to segment data */
	void* MdcAttrSegFixedData;				/* Data Specific to the Device */
	/* Confirm Timeout */
	RelativeTime MdcAttrConfirmTimeout;					/* Optional */
	/* Transfer Timeout */
	RelativeTime MdcAttrTransferTimeout;
}PMSEGMENT, *PPMSEGMENT;

/* PM-Store Data Structure */
typedef struct
{
    /* optional/conditional attrubute flag */
    intu8 OptionalAttributeFlag;
    #define OPT_PMSTORE_CAPACITY_COUNT  0x01
    #define OPT_PMSTORE_USAGE_CNT       0x02
    #define OPT_PMSTORE_LABEL_STRING    0x04
    #define OPT_PMSTORE_SAMPLE_PRD      0x08
    /* Handle */
	HANDLE MdcAttrIdHandle;
	/* PM Store Capability */
	PMStoreCapab MdcAttrPmStoreCapab;
	/* Store sample algorithm */
	StoSampleAlg MdcAttrMetricStoreSampleAlg;
	/* Store Capacity count */
	intu32 MdcAttrMetricStoreCapacCnt;			/* Optional */
	/* Store Usage Count */
	intu32 MdcAttrMetricStoreUsageCnt;			/* Optional */
	/* Operational Stat */
	OperationalState MdcAttrOpState;
	/* Pointer to PM Store Label String */
	octet_string *pMdcAttrPmStoreLabelString;	/* Optional */
	/* Sample Time period */
	RelativeTime MdcAttrTimePdSamp;             /* Conditional */
	/* Number of Segments */
	intu16 MdcAttrNumSeg;
	/* Clear Segment Timeout */
	RelativeTime MdcAttrClearTimeout;
	/* PM Segment count */
	intu8 PmSegmentCount;
	/* Pointer to an array of PM Segments */
	PMSEGMENT (*pPmSegment)[];					
}PMSTORE, *PPMSTORE;

typedef struct _CONFIGURATION
{
  /* Configuration ID */
  intu16 ConfigurationID;
	/* Numeric Object Count */
	intu8 NumericCount;
	/* Pointer to an array of numeric objects */
	NUMERIC (*pNumeric)[];		
	/* RT-SA object count */
	intu8 RtsaCount;
	/* Pointer to an array of RT-SA objects */
	RTSA (*pRtSa)[];				    	
	/* Enumeration Object count */
	intu8 EnumCount;
	/* Pointer to an array of enumeration objects */
	ENUMERATION (*pEnumeration)[];	
	/* Episodic Scanner object count */
	intu8 EpiScanCount;
	/* Pointer to an array of Episodic Scanner Objects */
	EPICFGSCANNER (*pEpiScanner)[];		
	/* Periodic Scanner Object count */
	intu8 PeriScanCount;
	/* Pointer to an array of Periodic Scanner Objects */
	PERICFGSCANNER (*pPeriScanner)[];	
	/* PM Store Object count */
	intu8 PmStoreCount;
	/* Pointer to an array of PM Store Objects */
	PMSTORE (*pPmStore)[];
}CONFIGURATION, *PCONFIGURATION;

/* Medical Device System (MDS) Data Structure */
typedef struct
{
	/* Handle */
	HANDLE MdcAttrIdHandle;
  #define MDS_HANDLE      (0x0000)
    /* System Type */
	TYPE *pMdcAttrSysType;						/* Conditional */
	/* Pointer to System Type Spec List */
	TypeVerList *pMdcAttrSysTypeSpecList;		/* Conditional */
	/* System Model */
	SystemModel *pMdcAttrSysModel;
	/* System ID */
	octet_string *pMdcAttrSysId;
	/* Device configuration id */
	ConfigId MdcAttrDevConfigId;
	/* Pointer to Attribute val map */
	AttrValMap *pMdcAttrValMap;
	/* Pointer to Production spec */
	ProductionSpec *pMdcAttrIdProdSpecn;	/* Optional */
	/* MDS Time info */
	MdsTimeInfo *pMdcAttrMdsTimeInfo;		  /* Conditional */
	/* Absolute Time */
	AbsoluteTime *pMdcAttrTimeAbs;				/* Conditional */
	/* Relative Time */
	RelativeTime MdcAttrTimeRel;				      /* Conditional */
	/* High Resolution relative time */
	HighResRelativeTime MdcAttrTimeRelHiRes;	/* Conditional */
	/* Absolute time adjust */
	AbsoluteTimeAdjust *pMdcAttrTimeAbsAdjust;/* Conditional */
	/* Power status */
	PowerStatus *pMdcAttrPowerStatus;				  /* Optional */
	/* Battery Charge */	
	intu16 *pMdcAttrValBattCharge;				    /* Optional */
	/* Battery remain */
	BatMeasure *pMdcAttrTimeBattRemain;			  /* Optional */
	/* Pointer to Reg certified data list */
	RegCertDataList *pMdcAttrRegCertDataList;	
	/* Confirm Timeout */
	RelativeTime *pMdcAttrConfirmTimeout;			/* Optional */
	/* Configuration selected */
	intu8 ConfigSelected;	
	/* Number of configuration device supports */
	intu8 ConfigCount;
	/* Pointer to an array of configurations */
	CONFIGURATION (*aConfiguration)[];	
}MDS, *PMDS;

/* MDS Object Action Methods */
typedef enum _MDSObjectMethodType
{
    MDS_OBJ_MTHD_MDC_ACT_SET_TIME = 3095,
    MDS_OBJ_MTHD_MDC_ACT_DATA_REQUEST = 3099
}MDSObjectMethodType;

/* MDS Object Events */
typedef enum _MDSObjectEventType
{
	MDS_OBJ_EVNT_MDC_NOTI_CONFIG = 3356,
	MDS_OBJ_EVNT_MDC_NOTI_SCAN_REPORT_FIXED = 3357,
	MDS_OBJ_EVNT_MDC_NOTI_SCAN_REPORT_VAR = 3358,
	MDS_OBJ_EVNT_MDC_NOTI_SCAN_REPORT_MP_FIXED = 3359,
	MDS_OBJ_EVNT_MDC_NOTI_SCAN_REPORT_MP_VAR = 3360
}MDSObjectEventType;

/* Episoidic Scanner Events */
typedef enum _EpiScannerObjectEvents
{
	EPI_SCN_OBJ_EVNT_MDC_NOTI_UNBUF_SCAN_REPORT_VAR = 3362,
	EPI_SCN_OBJ_EVNT_MDC_NOTI_UNBUF_SCAN_REPORT_FIXED = 3363,
	EPI_SCN_OBJ_EVNT_MDC_NOTI_UNBUF_SCAN_REPORT_GROUPED = 3364,
	EPI_SCN_OBJ_EVNT_MDC_NOTI_UNBUF_SCAN_REPORT_MP_VAR = 3365,
	EPI_SCN_OBJ_EVNT_MDC_NOTI_UNBUF_SCAN_REPORT_MP_FIXED = 3366,
	EPI_SCN_OBJ_EVNT_MDC_NOTI_UNBUF_SCAN_REPORT_MP_GROUPED = 3367
}EpiScannerObjectEvents;

/* Periodic Scanner Events */
typedef enum _PeriScannerObjectEvents
{
	PERI_SCN_OBJ_EVNT_MDC_NOTI_BUF_SCAN_REPORT_VAR = 3368,
	PERI_SCN_OBJ_EVNT_MDC_NOTI_BUF_SCAN_REPORT_FIXED = 3369,
	PERI_SCN_OBJ_EVNT_MDC_NOTI_BUF_SCAN_REPORT_GROUPED = 3370,
	PERI_SCN_OBJ_EVNT_MDC_NOTI_BUF_SCAN_REPORT_MP_VAR = 3371,
	PERI_SCN_OBJ_EVNT_MDC_NOTI_BUF_SCAN_REPORT_MP_FIXED = 3372,
	PERI_SCN_OBJ_EVNT_MDC_NOTI_BUF_SCAN_REPORT_MP_GROUPED = 3373
}PeriScannerObjectEvents;

/* PM-Store Object Events */
typedef enum _PMStoreObjectEventType
{
	PMSTR_OBJ_EVNT_MDC_ACT_SEG_CLR = 3084,
	PMSTR_OBJ_EVNT_MDC_ACT_SEG_GET_INFO = 3085,
	PMSTR_OBJ_EVNT_MDC_ACT_SEG_TRIG_XFER = 3100
}PMStoreObjectEventType;

/* Domain Information Model (DIM) Data Structure */
typedef struct
{
	PMDS pMds;
	intu16 dev_config_id;
}DIM, *PDIM;

#if !defined(__IAR_SYSTEMS_ICC__)
#pragma options align=reset
#endif

/******************************************************************************
 * MACROS and Constants
 *****************************************************************************/
#define APDU_HEADER_SIZE				    (4)
#define OBJECT_CLASS_SIZE                 	(2)
#define TYPEVERLIST_HEADER_SIZE				(4)
#define OCTET_STRING_HEADER_SIZE			(2)
#define PRODUCTION_SPEC_HEADER_SIZE			(4)
#define ANY_HEADER_SIZE						(2)
#define ATTR_HEADER_SIZE                    (4)
#define SUPPLEMENTAL_TYPE_LIST_HEADER_SIZE	(4)
#define METRIC_ID_LIST_HEADER_SIZE			(4)
#define ATTR_VAL_MAP_HEADER_SIZE			(4)
#define LABEL_STRING_HEADER_SIZE			(2)
#define UNIT_LABEL_STRING_HEADER_SIZE		(2)
#define DATAPROTOLIST_HEADER_SIZE	        (4)
#define DATAPROTO_HEADER_SIZE		        (4)

#define CMPD_SIM_NU_HEADER_SIZE				(4)
#define CMPD_BASIC_NU_HEADER_SIZE			(4)
#define CMPD_NU_HEADER_SIZE					(4)
#define SIMPLE_SA_OBS_VAL_HEADER_SIZE		(2)

#define RTSA_SCALE_RANGE_SPEC_8				(0)
#define RTSA_SCALE_RANGE_SPEC_16			(1)
#define RTSA_SCALE_RANGE_SPEC_32			(2)

#define ENUM_OBS_VAL_SIMPLE_OID				(0)
#define ENUM_OBS_VAL_SIMPLE_BIT_STR			(1)
#define ENUM_OBS_VAL_BASIC_BIT_STR			(2)
#define ENUM_OBS_VAL_SIMPLE_STR				(3)
#define ENUM_OBS_VAL						(4)

#define ENUM_OBS_VAL_SIMPLE_STR_HEADER_SIZE	(2)
#define ENUM_VAL_HEADER_SIZE				(4)

#define PMSTORE_LABEL_HEADER_SIZE			(2)
#define SEG_ENTRY_ELEM_HEADER_SIZE          (8)
#define SEG_ENTRY_LIST_HEADER_SIZE			(4)
#define SEGMENT_LABEL_HEADER_SIZE			(2)
#define SEGMENT_STATISTICS_HEADER_SIZE		(4)
#define SEGMENT_INFO_LIST_HEADER_SIZE       (4)

#define SCAN_HANDLE_LIST_HEADER_SIZE			(4)
#define SCAN_HANDLE_ATTR_VAL_MAP_HEADER_SIZE	(4)
#define SCAN_RPT_INFO_GPD_LIST_HEADER_SIZE      (4)

#define ACTIONRESULTSIMPLE_HEADER_SIZE          (sizeof(HANDLE) + \
										                             sizeof(OID_Type) + ANY_HEADER_SIZE)

#define REMOTE_OPERATION_HEADER_SIZE	          (sizeof(intu16) + sizeof(intu16))
#define DATA_APDU_HEADER_SIZE			              (sizeof(InvokeIDType) +     \
                                                 REMOTE_OPERATION_HEADER_SIZE)
#define PRST_APDU_HEADER_SIZE			              (OCTET_STRING_HEADER_SIZE)
#define CONFIG_REPORT_HEADER_SIZE               (sizeof(ConfigId) + \
                                                 ATTR_HEADER_SIZE)

#define EVNT_RPT_ARG_SMP_HEADER_SIZE            (sizeof(HANDLE) + \
                                                 sizeof(RelativeTime) +  \
                                                 sizeof(OID_Type) + ANY_HEADER_SIZE)

#define GetDataApduHeaderSize()                ((intu16)(DATA_APDU_HEADER_SIZE + \
                                                (intu16)(OCTET_STRING_HEADER_SIZE + \
                                                         APDU_HEADER_SIZE)))
#define GetEvntRptArgSmpHeaderSize()           ((intu16)(EVNT_RPT_ARG_SMP_HEADER_SIZE + \
                                                         GetDataApduHeaderSize()))

#define GetConfigReportHeaderSize()            ((intu16)(CONFIG_REPORT_HEADER_SIZE + \
                                                         GetEvntRptArgSmpHeaderSize()))

#define GetActionResultSimpleHeaderSize()      ((intu16)(ACTIONRESULTSIMPLE_HEADER_SIZE + \
                                                         GetDataApduHeaderSize()))

#define GetSetResultSimpleHeaderSize()         ((intu16)(sizeof(HANDLE) + ATTR_HEADER_SIZE + \
                                                GetDataApduHeaderSize()))

#define GetScanRptHeaderSize()                 ((intu16)SCAN_RPT_INFO_GPD_LIST_HEADER_SIZE + \
                                                        sizeof(DataReqId) + \
                                                        sizeof(intu16) + \
                                                        GetEvntRptArgSmpHeaderSize())
#define GetTYPESize()                          ((intu16)(sizeof(NomPartition) + sizeof(OID_Type)))

#define FLOAT(exp, mag)		(uint_32)((((exp) & 0xFF) << 24) | ((mag) & 0xFFFFFF))
#define SFLOAT(exp, mag)	(uint_16)((((exp) & 0xF) << 12) | ((mag) & 0xFFF))

#endif /* _IEEE11073_DIMSTRUCT_H */
