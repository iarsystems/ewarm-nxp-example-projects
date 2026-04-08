/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2009 Freescale Semiconductor, Inc.
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
 * @file ieee11073_dimfuncproto.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief 
 *
 *****************************************************************************/
#ifndef _IEEE11073_DIMFUNCPROTO_H
#define _IEEE11073_DIMFUNCPROTO_H
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <stack.h>
#ifdef LITTLE_ENDIAN
#include <ieee11073_phd_types_kinetis.h>
#else
#include <ieee11073_phd_types.h>
#endif
#include <ieee11073_dimstruct.h>

/*****************************************************************************
 * Local Function Prototypes
 *****************************************************************************/
/* MDS Functions */

/* This function is parses MDS Object Requests */
ERR_CODE MDSObjectMethods(
	OID_Type ActionType, 
	PTR_BUFFSTACK pReqBuffStack, 
	PTR_BUFFSTACK pRspBuffStack);

/* This function is called from the lower layer to enable/Disable data 
   transmission from MDS. Input is a Pointer to Data Request. Output is a 
   Pointer to Data Response */
ERR_CODE MDSDataRequest(
	DataRequest *pDataRequest, 
	PTR_BUFFSTACK pRspBuffStack);

/* This function is called from the lower layer to Set Time at MDS Input is a 
   Pointer to SetTimeInvoke. No Response is associated with this request */
ERR_CODE MDSSetTime(
	SetTimeInvoke *pSetTimeInvoke);

/* This is a function called from the lower layer to retrieve MDS Object 
   Attribute Data */
ERR_CODE MDSGetObjectAttribute(
	PTR_BUFFSTACK pReqBuffStack, 
	PTR_BUFFSTACK pRspBuffStack);

/* Not Supported */
ERR_CODE MDSSetObjectAttribute();

/* Generic Functions */
/* This function pushes Short Data type to Buffer Stack */
ERR_CODE Pushintu16(
	PTR_BUFFSTACK pBuffStack, 
	intu16 val, 
	intu16 *pAttrLen);
	
/* This function pushes Basic Numeric Observed Value to Buffer Stack */
#define PushBasicNuObsValue		Pushintu16
/* This function pushes Object ID to Buffer Stack */
#define PushOID_Type			Pushintu16
/* This function pushes Handle to Buffer Stack */
#define PushHANDLE				Pushintu16
/* This function pushes Nomenclature Partition to Buffer Stack */
#define PushNomPartition		Pushintu16
/* This function pushes Measurement Status to Buffer Stack */
#define PushMeasurementStatus	Pushintu16
/* This function pushes Metric Spec Small to Buffer Stack */
#define PushMetricSpecSmall		Pushintu16
/* This function pushes Sample Flags to Buffer Stack */
#define PushSaFlags				Pushintu16
/* This function pushes Object Class to Buffer Stack */
#define PushObjectClass         Pushintu16
/* This function pushes Confirm Mode flag to Buffer Stack */
#define PushConfirmMode         Pushintu16
/* This function pushes Instance Number to Buffer Stack */
#define PushInstNumber          Pushintu16
/* This function pushes Modify Operator to Buffer Stack */
#define PushModifyOperator      Pushintu16
/* This function pushes Operational Stat to Buffer Stack */
#define PushOperationalState    Pushintu16
/* This function pushes Data Request ID to Buffer Stack */
#define PushDataReqId           Pushintu16
/* This function pushes Person ID to Buffer Stack */
#define PushPersonId            Pushintu16
/* This function pushes Store Sample Algorithm to Buffer Stack */
#define PushStoSampleAlg        Pushintu16
/* This function pushes PM Store Capability to Buffer Stack */
#define PushPMStoreCapab        Pushintu16 
/* This function pushes Segment Stat Type to Buffer Stack */
#define PushSegmStatType        Pushintu16 
/* This function pushes Segmeny Entry Header to Buffer Stack */
#define PushSegmEntryHeader     Pushintu16
/* This function pushes Segment Event Status to Buffer Stack */
#define PushSegmEvtStatus       Pushintu16   

/* This function pushes Long Data type to Buffer Stack */
ERR_CODE Pushintu32(
	PTR_BUFFSTACK pBuffStack, 
	intu32 val, 
	intu16 *pAttrLen);

/* This function pushes Attrbiute ID and Length to Buffer Stack */
ERR_CODE PushAttribute(
	PTR_BUFFSTACK pBuffStack, 
	intu16 AttrId, 
	intu16 AttrLength, 
	intu16 *pAttrLen);
	
/* This function pushes Scale Range Spec16 to Buffer Stack */
#define PushScaleRangeScale16	        PushAttribute
/* This function pushes Scan Report Header to Buffer Stack */
#define PushScanReportHeader            PushAttribute
/* This function pushes Observation Scan List Header to Buffer Stack */
#define PushObsScanListHeader           PushAttribute
/* This function pushes Scan Report Per Var List Header to Buffer Stack */
#define PushScanReportPerVarListHeader  PushAttribute
/* This function pushes Action Result Simple Header to Buffer Stack */
#define PushActionResultSimpleHeader    PushAttribute

/* This function pushes Relative time to Buffer Stack */
#define PushRelativeTime(a,b,c)	Pushintu32((a), (intu32)(b), (c))
/* This function pushes Float Data Type to Buffer Stack */
#define PushFloat(a,b,c)	Pushintu32((a), (intu32)(b), (c))
/* This function pushes Simple Numeric Observed Value to Buffer Stack */
#define PushSimpleNuObsValue	PushFloat

/* This function pushes Any Data type to Buffer Stack */
ERR_CODE PushAny(
	PTR_BUFFSTACK pBuffStack, 
	Any *pAny, 
	intu16 *pAttrLen);

/* This function pushes octet_string Data type to Buffer Stack */
#define Pushoctet_string(a,b,c)	PushAny((a), (Any*)(b), (c))

/* This function pushes BatMeasure Data Type to Buffer Stack */
ERR_CODE PushBatMeasure(
	PTR_BUFFSTACK pBuffStack, 
	BatMeasure *pBatMeasure, 
	intu16 *pAttrLen);

/* This function pushes MdsTimeInfo Data Type to Buffer Stack */
ERR_CODE PushMdsTimeInfo(
	PTR_BUFFSTACK pBuffStack, 
	MdsTimeInfo *pMdsTimeInfo, 
	intu16 *pAttrLen);

/* This function pushes ProdSpecEntry Data Type to Buffer Stack */
ERR_CODE PushProdSpecEntry(
	PTR_BUFFSTACK pBuffStack, 
	ProdSpecEntry *pProdSpecEntry, 
	intu16 *pAttrLen);

/* This function pushes ProdSpecEntryList Data Type to Buffer Stack */
ERR_CODE PushProdSpecEntryList(
	PTR_BUFFSTACK pBuffStack, 
	intu16 SpecCount, 
	ProdSpecEntry *pProdSpecEntry, 
	intu16 *pAttrLen);

/* This function pushes ProdSpec Data Type to Buffer Stack */
ERR_CODE PushProductionSpec(
	PTR_BUFFSTACK pBuffStack, 
	ProductionSpec *pProdSpec, 
	intu16 *pAttrLen);

/* This function pushes AttrValMap Data Type to Buffer Stack */
ERR_CODE PushAttrValMap(
	PTR_BUFFSTACK pBuffStack, 
	AttrValMap *pAttrValMap, 
	intu16 *pAttrLen);

/* This function pushes AttrValMapEntry Data Type to Buffer Stack */
ERR_CODE PushAttrValMapEntry(
	PTR_BUFFSTACK pBuffStack, 
	AttrValMapEntry *pAttrValMapEntry, 
	intu16 *pAttrLen);

/* This function pushes TYPE Data Type to Buffer Stack */
#define PushTypeVer(a, b, c)	\
    PushAttrValMapEntry((a), (AttrValMapEntry *)(b), (c))

/* This function recursively pushes AttrValMapEntry Data Type 
   to Buffer Stack */
ERR_CODE PushAttrValMapEntryList(
	PTR_BUFFSTACK pBuffStack, 
	intu16 AttrCount, 
	AttrValMapEntry *pAttrValMapEntry, 
	intu16 *pAttrLen);
    
/* This function pushes Type Per Var List to Buffer Stack */
#define PushTypeVerList(a, b, c)	\
	PushAttrValMap((a), (AttrValMap *)(b), (c))
/* This function pushes Supplemental Type List to Buffer Stack */
#define PushSupplementalTypeList	PushTypeVerList

/* This function pushes SystemModel Data Type to Buffer Stack */
ERR_CODE PushSystemModel(
	PTR_BUFFSTACK pBuffStack, 
	SystemModel *pSystemModel, 
	intu16 *pAttrLen);

/* This function pushes TYPE Data Type to Buffer Stack */
ERR_CODE PushTYPE(
	PTR_BUFFSTACK pBuffStack, 
	TYPE *pTYPE, 
	uint_16 *pAttrLen);

	/* This function pushes SimpleNuObsValCmp Data type to Buffer Stack */
	ERR_CODE PushSimpleNuObsValueCmp(
		PTR_BUFFSTACK pBuffStack, 
		SimpleNuObsValueCmp* pSmpNuObsValCmp, 
		uint_16_ptr pAttrLen);

	/* This function pushes BasicNuObsValueCmp Data type to Buffer Stack */
	ERR_CODE PushBasicNuObsValueCmp(
		PTR_BUFFSTACK pBuffStack, 
		BasicNuObsValueCmp* pBasNuObsValCmp, 
		uint_16_ptr pAttrLen);

	/* This function pushes NuObsValue Data type to Buffer Stack */
	ERR_CODE PushNuObsValue(
		PTR_BUFFSTACK pBuffStack, 
		NuObsValue *pNuObsValue, 
		uint_16_ptr pAttrLen);

	/* This function pushes NuObsValueCmp Data type to Buffer Stack */
	ERR_CODE PushNuObsValueCmp(
		PTR_BUFFSTACK pBuffStack, 
		NuObsValueCmp* pNuObsValCmp, 
		uint_16_ptr pAttrLen);

	/* This function pushes Numeric Value to Buffer Stack */
	ERR_CODE PushNumericVal(
		PTR_BUFFSTACK pBuffStack, 
		OID_Type AttrId, 
		void* pVal, 
		intu16 *pAttrLen);

	/* This function pushes Numeric Attributes to Buffer Stack */
	ERR_CODE PushNumericAttribute(
		PTR_BUFFSTACK pBuffStack, 
		PNUMERIC pNumeric, 
		uint_16_ptr pAttrLen);

/* This function pushes Absolute Scale Range to Buffer Stack */
ERR_CODE PushScaleRangeAbsolute(
	PTR_BUFFSTACK pBuffStack, 
	FLOAT_Type fLower, 
	FLOAT_Type fUpper, 
	uint_16_ptr pAttrLen);

/* This function pushes Scale Range Spec32 to Buffer Stack */
#define PushScaleRangeScale32		PushScaleRangeAbsolute

/* This function pushes Metric ID List to Buffer Stack */
ERR_CODE PushMetricIdList(
	PTR_BUFFSTACK pBuffStack, 
	MetricIdList *pMetricIdList, 
	uint_16_ptr pAttrLen);

/* This function pushes the Handle Attribute Val Map Entry onto the
   Buffer Stack */
ERR_CODE PushHandleAttrValMapEntry(
	PTR_BUFFSTACK pBuffStack, 
	HandleAttrValMapEntry *pHandleAttrValMapEntry, 
	intu16* pAttrLen);

/* This function pushes the Attribute id and its value onto the Buffer 
   Stack */
ERR_CODE PushAVA_Type(
	PTR_BUFFSTACK pBuffStack, 
	AVA_Type *pAVA_Type, 
	intu16 *pAttrLen);

/* This function pushes the PM-Segment entry element onto the Buffer 
   Stack */
ERR_CODE PushSegmentEntryElem(
	PTR_BUFFSTACK pBuffStack, 
    SegmEntryElem *pSegmEntryElem, 
    intu16 *pAttrLen);

/* This function pushes the Attribute Modify Entry onto the Buffer Stack */
ERR_CODE PushAttributeModEntry(
	PTR_BUFFSTACK pBuffStack, 
    AttributeModEntry *pAttributeModEntry, 
    intu16 *pAttrLen);

/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
/* This function returns size of AVA_Type Data Structure */
extern intu16 GetAVA_Type(
	void* pvPtr);

/* This function returns size of OID_Type Data Structure */
extern intu16 GetOID_Type(
	void* pvPtr);

/* This function returns size of ProdSpecEntry Data Structure */
extern intu16 GetProdSpecEntry(
	void* pvPtr);

/* This function returns size of NuObsValue Data Structure */
extern intu16 GetNuObsValue(
	void* pvPtr);

/* This function returns size of AttrValMapEntry Data Structure */
extern intu16 GetAttrValMapEntry(
	void* pvPtr);

/* This function returns size of HandleAttrValMapEntry Data Structure */
extern intu16 GetHandleAttrValMapEntry(
	void* pvPtr);

/* This function returns size of InstNumber Data Structure */
extern intu16 GetInstNumber(
	void* pvPtr);

/* This function returns size of SegmentInfo Data Structure */
extern intu16 GetSegmentInfo(
	void* pvPtr);

/* This function returns size of SegmEntryElem Data Structure */
extern intu16 GetSegmEntryElem(
	void* pvPtr);

/* This function returns size of SegmElemStaticAttrEntry Data Structure */
extern intu16 GetSegmElemStaticAttrEntry(
	void* pvPtr);

/* This function returns size of SegmentStatisticEntry Data Structure */
extern intu16 GetSegmentStatisticEntry(
	void* pvPtr);

/* This function returns size of DataProto Data Structure */
extern intu16 GetDataProto(
	void* pvPtr);

/* This function returns size of AttributeModEntry Data Structure */
extern intu16 GetAttributeModEntry(
	void* pvPtr);

/* This function returns size of TypeVer Data Structure */
extern intu16 GetTypeVer(
	void* pvPtr);

/* This function returns size of ObservationScan Data Structure */
extern intu16 GetObservationScan(
	void* pvPtr);

/* This function returns size of ScanReportPerVar Data Structure */
extern intu16 GetScanReportPerVar(
	void* pvPtr);

/* This function returns size of ObservationScanFixed Data Structure */
extern intu16 GetObservationScanFixed(
	void* pvPtr);

/* This function returns size of ScanReportPerFixed Data Structure */
extern intu16 GetScanReportPerFixed(
	void* pvPtr);

/* This function returns size of ObservationScanGrouped Data Structure */
extern intu16 GetObservationScanGrouped(
	void* pvPtr);

/* This function returns size of ScanReportPerGrouped Data Structure */
extern intu16 GetScanReportPerGrouped(
	void* pvPtr);

/* This function returns size of ConfigObject Data Structure */
extern intu16 GetConfigObject(
	void* pvPtr);

/* This function returns size of HANDLE Data Structure */
extern intu16 GetHANDLE(
	void* pvPtr);

/* This function returns size of SimpleNuObsValue Data Structure */
extern intu16 GetSimpleNuObsValue(
	void* pvPtr);

/* This function returns size of BasicNuObsValue Data Structure */
extern intu16 GetBasicNuObsValue(
	void* pvPtr);

#endif /* _IEEE11073_DIMFUNCPROTO_H */
