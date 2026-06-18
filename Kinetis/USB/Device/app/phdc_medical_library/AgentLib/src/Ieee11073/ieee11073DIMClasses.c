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
 * @file Ieee11073DIMClasses.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains IEEE11073 Agent Library DIM Layer Implementation
 *
 *****************************************************************************/
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <stack.h>
#include <string.h>
#include <ieee11073_nom_codes.h>
#include <ieee11073_dimstruct.h>
#include <ieee11073_dimfuncproto.h>
#include <ieee11073.h>
#include <ieee11073_presentationlayer.h>
#include <ieee11073_ClassCallback.h>
#include <ieee11073_DimClasses.h>
#include <ieee11073_service.h>
#include <ieee11073_association.h>
/*****************************************************************************
 * Macro's
 *****************************************************************************/
#define TO_CER      3000
/*****************************************************************************
 * Global Variables - None
 *****************************************************************************/

/*****************************************************************************
 * Local Variables
 *****************************************************************************/
/* Variable to track Scan Report Info */
static volatile intu16 g_scan_report_num = 0;
static PDIM pDIM;
/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
/* Application implemented functions and variables */
extern ERR_CODE RTCSetTime(AbsoluteTime date_time, FLOAT_Type accuracy);
extern ERR_CODE RTCGetTime(AbsoluteTime *pdate_time);
extern void GetAbsoluteTime(AbsoluteTime *pAbsTime);
extern void GetRelativeTime(RelativeTime *pRelTime);
extern void GetHiResRelativeTime(HighResRelativeTime *pHiResRelTime);
extern ERR_CODE WritePMSegment(HANDLE Handle, InstNumber InstNum, 
    void *pData, intu32 size);

/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static int      CompareAbsTime(AbsoluteTime *pAbsA, AbsoluteTime *pAbsB);

static intu16   GetPmStoreAllAttributeSize(PPMSTORE pPmStore);
static ERR_CODE ClearPmSegm(PPMSTORE pPmStore, PPMSEGMENT pPmSegm, intu8 bDelete);

static void     EpiCfgScannerTimerCallback(void* arg);
static void     PeriCfgScannerTimerCallback(void* arg);

static ERR_CODE UpdateMdsAttribute(PMDS pObject, AVA_Type* pAvaType);
static intu16   GetMdsAttributeSize(PMDS pMds, OID_Type AttributeId);
static ERR_CODE SendMeasurements(void* pMsr, intu8 bPersonFlag, HANDLE handle, intu8 ReportType, intu16 ScanCount, intu8 bConfirm);
static ERR_CODE PushMdsAttribute(PTR_BUFFSTACK pBuffStack, PMDS pMds, OID_Type OidType, intu16 *pAttrLen);

static PMDS     GetMdsHandle(HANDLE obj_handle);

static intu16   GetCfgScannerAttributeSize(PCFGSCANNER pCfgScanner);
static ERR_CODE CfgScannerSetArgument(PTR_BUFFSTACK pReqBuffStack,
    PCFGSCANNER pCfgScanner, PTR_BUFFSTACK pRspBuffStack, intu16 AttrCount, 
    intu16 AttrLength, intu16 *pAttrLen);
static intu16   GetCfgScannerSetArgumentSize(PTR_BUFFSTACK pReqBuffStack);

static intu16   GetScannerAttributeSize(PSCANNER pScanner);
static ERR_CODE ScannerSetArgument(PTR_BUFFSTACK pReqBuffStack,
    PSCANNER pScanner, PTR_BUFFSTACK pRspBuffStack, intu16 AttrCount, 
    intu16 AttrLength, intu16 *pAttrLen);

static PENUMERATION   GetEnumHandle(HANDLE objHandle);
static intu16         GetEnumAttributeSize(PENUMERATION pEnumeration);

static PRTSA          GetRtsaHandle(HANDLE objHandle);
static intu16         GetRtsaAttributeSize(PRTSA pRtsa);

static PNUMERIC       GetNumericHandle(HANDLE objHandle);
static intu16         GetNumericAttributeSize(PNUMERIC pNumeric);

static intu16         GetMetricAttributeSize(PMETRIC pMetric);
static PEPICFGSCANNER GetEpiCfgScanHandle(HANDLE objHandle);
static PPERICFGSCANNER GetPeriCfgScanHandle(HANDLE objHandle);

static PPMSTORE GetPmStoreHandle(HANDLE objHandle);
static ERR_CODE PushPMSegmentAttribute(PPMSEGMENT pPMSegment, PTR_BUFFSTACK pBuffStack, intu16 *pAttrLength);
static ERR_CODE PushPmStoreAttribute(PTR_BUFFSTACK pBuffStack, PPMSTORE pPMStore, OID_Type AttributeId, intu16 *pAttrLen);
static ERR_CODE PushPmStoreAttributeList(PTR_BUFFSTACK pBuffStack, PPMSTORE pPMStore, AttributeIdList *pAttributeIdList);
static ERR_CODE PushAllPMStoreAttribute(PTR_BUFFSTACK pBuffStack, PPMSTORE pPMStore, intu16 *pAttrLen);
static ERR_CODE ClearPmSegment(SegmSelection* pSegmentSel, PTR_BUFFSTACK pBuffStack, PPMSTORE pPmStore);
static PPMSEGMENT GetPmSegmentById(PPMSTORE pPmStore, InstNumber InstNum);
static ERR_CODE PushAttrObs(HANDLE Handle, OID_Type AttrId, intu16 AttrLen,     
    void* pAttrVal, void* pAvaType);
static ERR_CODE PushRegCertData(PTR_BUFFSTACK pBuffStack, RegCertData* pRegCertData,intu16* pAttrLen);
static ERR_CODE PushRegCertDataList(PTR_BUFFSTACK pBuffStack,RegCertDataList *pRegCertDataList,intu16 *pAttrLen);
static ERR_CODE PushAllMdsAttribute(PMDS pMds,PTR_BUFFSTACK pBuffStack);
static intu16 GetPmSegInfoSize(PPMSEGMENT pPMSegment);
static intu16 GetPmSegSelInfoSize(PPMSTORE pPmStore,SegmSelection *pSegmSel);
static ERR_CODE PushPmSegInfoHeader(PTR_BUFFSTACK pBuffStack,intu16 Count,intu16 Length,intu16 *pAttrLen);
static ERR_CODE PushAllPmSegInfo(PPMSTORE pPmStore,PTR_BUFFSTACK pBuffStack);
static ERR_CODE PushPmSegmentInfoID(SegmIdList *pSegmIdList,PTR_BUFFSTACK pBuffStack,PPMSTORE pPmStore);
static ERR_CODE PushPmSegmentInfoTimeRange(AbsTimeRange *pAbsTimeRange,PTR_BUFFSTACK pBuffStack,PPMSTORE pPmStore);
static ERR_CODE GetPmSegmentInfo(SegmSelection* pSegmentSel,PTR_BUFFSTACK pRspBuffStack,PPMSTORE pPmStore);
static intu32 GetPMSegmentEntryElemDataSize(SegmEntryElem *pSegmEntry);
static intu32 GetPMSegmentSize(PmSegmentEntryMap *pPmSegmEntMap);
static AttributeList* GetAttrListByHandle(HANDLE Handle,ObservationScanList *pObsScanList);
static Any* GetAttrValueByAttrId(OID_Type OidType,AttributeList *pAttrList);
static ERR_CODE PushPmSegmentEntryElemData(void* pData,SegmEntryElem *pSegmEntry,ObservationScanList *pObsScanList);
static ERR_CODE UpdatePmSegmEntryData(PmSegmentEntryMap *pPmSegmEntMap,void* pData,ObservationScanList *pObsScanList);
static ERR_CODE UpdatePmSegmentEntry(HANDLE Handle, InstNumber InstNum,ObservationScanList *pObsScanList);
static ERR_CODE PushSegmDataEventDescr(PTR_BUFFSTACK pBuffStack,SegmDataEventDescr* pSegmDataEventDescr, intu16* pAttrLen);
static ERR_CODE PushSegmentData(PTR_BUFFSTACK pBuffStack,SegmentDataEvent* pSegmDataEvent,HANDLE PmStoreHandle,intu16* pAttrLen);
static ERR_CODE SetPmSegmentOpStat(HANDLE Handle,InstNumber InstNum,OperationalState OpStat);
static ERR_CODE TrigPmSegmentDataTrf(TrigSegmDataXferReq *pXferReq,PTR_BUFFSTACK pBuffStack,PPMSTORE pPmStore);
static intu16 GetMdsAllAttributeSize(PMDS pMds);
static ERR_CODE PushMdsAttributeList(PTR_BUFFSTACK pBuffStack,PMDS pMds,AttributeIdList *pAttributeIdList);
static ERR_CODE PushMetricAttribute(PTR_BUFFSTACK pBuffStack,PMETRIC pMetric,intu16 AttrCount,intu16 AttrLength,intu16 *pAttrLen);
static ERR_CODE PushScaleRangeSpec32(PTR_BUFFSTACK pBuffStack,ScaleRangeSpec32 *pScaleRangeSpec32,intu16 *pAttrLen);
static ERR_CODE PushScaleRangeSpec16(PTR_BUFFSTACK pBuffStack,ScaleRangeSpec16 *pScaleRangeSpec16,intu16 *pAttrLen);
static ERR_CODE PushScaleRangeSpec8(PTR_BUFFSTACK pBuffStack,ScaleRangeSpec8 *pScaleRangeSpec8,intu16 *pAttrLen);
static ERR_CODE PushSaSpec(PTR_BUFFSTACK pBuffStack,SaSpec *pSaSpec,intu16 *pAttrLen);
static ERR_CODE PushHandleAttrValMapEntryList(PTR_BUFFSTACK pBuffStack,intu16 AttrCount,HandleAttrValMapEntry *pHandleAttrValMapEntry,intu16* pAttrLen);
static ERR_CODE PushHandleAttrValMap(PTR_BUFFSTACK pBuffStack,HandleAttrValMap *pHandleAttrValMap,intu16* pAttrLen);
static ERR_CODE PushScannerAttribute(PTR_BUFFSTACK pBuffStack,PSCANNER pScanner,intu16 AttrCount,intu16 AttrLength,intu16 *pAttrLen);
static ERR_CODE PushCFGScannerAttribute(PTR_BUFFSTACK pBuffStack,PCFGSCANNER pCFGScanner,intu16 AttrCount,intu16 AttrLength,intu16 *pAttrLen);
static ERR_CODE PushPERICFGScannerAttribute(PTR_BUFFSTACK pBuffStack,PPERICFGSCANNER pPERICFGScanner,intu16 *pAttrLen);
static ERR_CODE PushSegmEntryElem(PTR_BUFFSTACK pBuffStack,SegmEntryElem *pSegmEntryElem,intu16* pAttrLen);extern ERR_CODE PushSegmEntryElemList(PTR_BUFFSTACK pBuffStack,intu16 AttrCount,SegmEntryElem *pSegmEntryElem,intu16* pAttrLen);
static ERR_CODE PushSegmElemEntry(PTR_BUFFSTACK pBuffStack,SegmEntryElemList *pSegmEntryElemList,intu16* pAttrLen);
static intu16 GetPeriCfgScannerAttributeSize(PPERICFGSCANNER pPeriScanner);
static intu16 GetPmStoreAttributeSize(	PPMSTORE pPmStore,OID_Type AttributeId);
static intu16 GetConfigrationEventReportSize(PCONFIGURATION pConfiguration);
static ERR_CODE PushConfigurationEventReport(intu8 bFlag,PTR_BUFFSTACK pBuffStack,PMDS pMds);
static ERR_CODE PushScanReportInfoHeader(PTR_BUFFSTACK pBuffStack,DataReqId DataRequestId,intu16 ScanRptNo,intu16 Count,intu16 *pAttrLen);
static AttrValMap* GetAttrValMapPtr(HANDLE handle);
static HandleAttrValMap* GetHandleAttrValMapPtr(HANDLE handle	);
static ERR_CODE MatchAttrListWithAttrValMap(ObservationScan* pObsScan	);
static ERR_CODE MatchAttrListWithHandleAttrValMap(ObservationScan* pObsScan,	HANDLE handle);
static ERR_CODE ScanAttrListWithAttrValMap(ObservationScan* pObsScan);
static intu16 GetAttrValMapDataSize(AttrValMap* pAttrValMap);
static intu16 GetObjectFixedDataSize(HANDLE handle		);
static intu16 GetFixedFormatEventRptSize(ObservationScanList* pObsScanList);
static intu16 GetMPFixedFormatEventRptSize(ScanReportPerVarList* pScanRptPerVarList );
static intu16 GetVarFormatEventRptSize(ObservationScanList* pObsScanList	);
static intu16 GetMPVarFormatEventRptSize(ScanReportPerVarList* pScanPerVarList	);
static intu16 GetGroupedFormatEventRptSize(HANDLE handle	);
static ERR_CODE PushObsScanList(PTR_BUFFSTACK pBuffStack,ObservationScanList* pObsScanList,intu16* pAttrLen,HANDLE handle, intu8 GrpdFormat);
static ERR_CODE PushFixedFormatEvntRpt(HANDLE handle,PTR_BUFFSTACK pBuffStack,ObservationScanList* (*pObsScanList)[],intu8 bConfirm,intu16 ScanCount);
static ERR_CODE PushVarFormatEvntRpt(HANDLE handle,PTR_BUFFSTACK pBuffStack,ObservationScanList* (*pObsScanList)[],intu8 bConfirm,intu16 ScanCount);
static ERR_CODE PushMPVarFormatEvntRpt(HANDLE handle,PTR_BUFFSTACK pBuffStack,ScanReportPerVarList* (*pScanRptPerVarList)[],intu8 bConfirm,intu16 ScanCount);
static ERR_CODE PushScanPerVarList(PTR_BUFFSTACK pBuffStack,ScanReportPerVarList* pScanRptPerVarList,HANDLE handle,intu8 GrpdFormat);
static ERR_CODE PushMPFixedFormatEvntRpt(HANDLE handle,PTR_BUFFSTACK pBuffStack,ScanReportPerVarList* (*pScanRptPerVarList)[],intu8 bConfirm,intu16 ScanCount);
static ERR_CODE AgentSendFixedFormatEventRpt(HANDLE handle,ObservationScanList* (*pObsScanList)[],intu8 bConfirm,intu16 ScanCount);
static ERR_CODE AgentSendMPFixedFormatEventRpt(HANDLE handle,ScanReportPerVarList* (*pScanRptPerVarList)[],intu8 bConfirm,intu16 ScanCount);
static ERR_CODE AgentSendVarFormatEventRpt(HANDLE handle,ObservationScanList* (*pObsScanList)[],intu8 bConfirm,intu16 ScanCount);
static ERR_CODE AgentSendMPVarFormatEventRpt(
	HANDLE handle,								/* [IN] Handle of the object 
													which has to send 
													measurements */ScanReportPerVarList* (*pScanRptPerVarList)[],intu8 bConfirm,intu16 ScanCount);
static ERR_CODE PushGroupedFormatEvntRpt(HANDLE handle,PTR_BUFFSTACK pBuffStack,ObservationScanList* (*pObsScanList)[],	intu16 ScanCount);
static ERR_CODE PushMPGroupedFormatEvntRpt(HANDLE handle,PTR_BUFFSTACK pBuffStack,ScanReportPerVarList* (*pScanRptPerVarList)[],intu16 ScanCount);
static ERR_CODE AgentSendGroupedFormatEventRpt(HANDLE handle,ObservationScanList* (*pObsScanList)[],intu16 ScanCount);
static ERR_CODE AgentSendMPGroupedFormatEventRpt(HANDLE handle,ScanReportPerVarList* (*pScanRptPerVarList)[],intu16 ScanCount);
static ERR_CODE UpdateMetricAttribute(PMETRIC pObject,AVA_Type* pAvaType);
static ERR_CODE UpdateNumericAttribute(PNUMERIC pObject,AVA_Type* pAvaType);
static ERR_CODE UpdatePmStoreAttribute(PPMSTORE pObject,AVA_Type* pAvaType);
static ERR_CODE UpdateScannerAttribute(PSCANNER pObject,AVA_Type* pAvaType);
static ERR_CODE UpdateCfgScannerAttribute(PCFGSCANNER pObject,AVA_Type* pAvaType);
static ERR_CODE UpdatePeriScannerAttribute(PPERICFGSCANNER pObject,AVA_Type* pAvaType);
static ERR_CODE UpdateMdsAttribute(PMDS pObject,AVA_Type* pAvaType);
static ERR_CODE UpdateAttribute(intu16 ObjType,	void* pObject,AVA_Type* pAvaType);
static ERR_CODE UpdateDIM(ObservationScanList* pObsScanList);
static ERR_CODE PushRTSAAttribute(PTR_BUFFSTACK pBuffStack,PRTSA pRTSA,intu16 *pAttrLen);
static ERR_CODE PushEnumerationAttribute(PTR_BUFFSTACK pBuffStack,PENUMERATION pEnumeration,intu16 *pAttrLen);
static ERR_CODE PushEPICFGScannerAttribute(PTR_BUFFSTACK pBuffStack, PEPICFGSCANNER pEPICFGScanner,intu16 *pAttrLen);
static PhdAssociationInformation* GetPHDAssocInfo(void);
static void* GetPhdAssocFieldPtr(intu8 ID);
static intu16 GetEpiCfgScannerAttributeSize(PEPICFGSCANNER pEpiScanner);
static AttrValMap* GetAttrValMapFromHandleAttrValMap(HANDLE handle,HandleAttrValMap* pHandleAttrValMap);
static ERR_CODE UpdateRtsaAttribute(PRTSA pObject, AVA_Type* pAvaType);
static ERR_CODE UpdateEnumAttribute(PENUMERATION pObject,AVA_Type* pAvaType);
static ERR_CODE SetMDSEventRptTimeout(PTR_BUFFSTACK pBuffStack);
static ERR_CODE UpdateEpiScannerAttribute(PEPICFGSCANNER pObject,AVA_Type* pAvaType);
static ERR_CODE ScanAllActiveScanners(ObservationScan* pObsScan,HANDLE handle);
static ERR_CODE SetMDSEventRptTimeout(PTR_BUFFSTACK pBuffStack);
/*****************************************************************************
 * Global Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  Pushintu16
 *
 * @brief This function pushes Short Data type to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 * @param val			:	Short Value
 * @param pAttrLen		:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Short Data type to Buffer Stack
 *****************************************************************************/
ERR_CODE Pushintu16(
    PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */
    intu16 val,					/* [IN] Short Value */
    intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	val = ieee_htons(val);
	
	*pAttrLen = 0;
	if(ERROR_SUCCESS == (err = PushStack(pBuffStack, &val, sizeof(intu16))))
	{
		*pAttrLen = sizeof(intu16);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  Pushintu32
 *
 * @brief This function pushes Long Data type to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 * @param val			:	Long Value
 * @param pAttrLen		:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Long Data type to Buffer Stack
 *****************************************************************************/
ERR_CODE Pushintu32(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */        
	intu32 val, 				/* [IN] Long Value */                    
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	val = ieee_htonl(val);
	
	*pAttrLen = 0;
	if(ERROR_SUCCESS == (err = PushStack(pBuffStack, &val, sizeof(intu32))))
	{
		*pAttrLen = sizeof(intu32);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushAny
 *
 * @brief This function pushes Any Data type to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 * @param pAny			:	Pointer to Any Data Type
 * @param pAttrLen		:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Any Data type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushAny(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */        
	Any *pAny, 					/* [IN] Pointer to Any Data Type */                     
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen;
	intu16 val;
		
	*pAttrLen = 0; 
	if(ERROR_SUCCESS != (err = PushStack(pBuffStack, &pAny->value, 
			pAny->length)))
	{
		return err;
	}

	AttrLen = pAny->length;
	val = ieee_htons(pAny->length);

	if(ERROR_SUCCESS != (err = PushStack(pBuffStack, &val, sizeof(intu16))))
	{
		(void)PopStack(pBuffStack, AttrLen, NULL, NULL);
		return err;
	}
	AttrLen += ANY_HEADER_SIZE;
	*pAttrLen = AttrLen;
	return  ERROR_SUCCESS;
}

/**************************************************************************//*!
 *
 * @name  PushAttribute
 *
 * @brief This function pushes Attrbiute ID and Length to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 * @param AttrId		:	Attribute ID
 * @param AttrLength	:	Attribute Length
 * @param pAttrLen		:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Attrbiute ID and Length to Buffer Stack
 *****************************************************************************/
ERR_CODE PushAttribute(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */
	intu16 AttrId, 					/* [IN] Attribute ID */    
	intu16 AttrLength, 				/* [IN] Attribute Length */              
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	intu16 val;
	
	*pAttrLen = 0;
	do
	{
		val = ieee_htons(AttrLength);
		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, &val, sizeof(val))))
		{
			break;
		}
		*pAttrLen = sizeof(val);
		val = ieee_htons(AttrId);
		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, &val, sizeof(val))))
		{
			break;
		}
		*pAttrLen += sizeof(val);
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;

	}
	return err;
}
/**************************************************************************//*!
 *
 * @name  PushRegCertData
 *
 * @brief This function pushes RegCertData Data Type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pRegCertData  	:	Pointer to RegCertData Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes RegCertData Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushRegCertData(
    PTR_BUFFSTACK pBuffStack,   /* [IN] Pointer to Buffer Stack */    
    RegCertData* pRegCertData,  /* [IN] Pointer to RegCertData 
									Data Type */
    intu16* pAttrLen            /* [OUT] Pointer to size of data 
											pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen;	
	
	*pAttrLen = 0;
	
	pRegCertData->authbodydata.length = ieee_htons(pRegCertData->authbodydata.length);

	if(ERROR_SUCCESS != (err = PushAny(pBuffStack, 
		&pRegCertData->authbodydata, &AttrLen)))
	{
		return err;
	}
	*pAttrLen += AttrLen;
	if(ERROR_SUCCESS != (err = PushStack(pBuffStack, 
		&pRegCertData->authbody, sizeof(AuthBodyStructType))))
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
		return err;
	}
	*pAttrLen += sizeof(AuthBodyStructType);
    
    return err;
}
/**************************************************************************//*!
 *
 * @name  PushRegCertDataList
 *
 * @brief This function pushes RegCertDataList Data Type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pRegCertDataList	:	Pointer to RegCertDataList Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes RegCertDataList Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushRegCertDataList(
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to Buffer Stack */        
	RegCertDataList *pRegCertDataList,	/* [IN] Pointer to RegCertDataList 
											Data Type */
	intu16 *pAttrLen					/* [OUT] Pointer to size of data 
											pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen;
	intu16 i;
	
	RegCertData* pRegCertData = &pRegCertDataList->value[0];
	
	*pAttrLen = 0;
	
	for(i = 0; i < ieee_htons(pRegCertDataList->count); i++)
	{
	    if(ERROR_SUCCESS != 
	        (err = PushRegCertData(pBuffStack, pRegCertData, &AttrLen)))
	    {
	        return err;
	    }
	    *pAttrLen += AttrLen;
	    (intu8*)pRegCertData += pRegCertData->authbodydata.length + 
	        ANY_HEADER_SIZE + sizeof(AuthBodyStructType);
	}
	if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
			ieee_htons(pRegCertDataList->count), ieee_htons(pRegCertDataList->length), &AttrLen)))
	{
		return err;
	}
	*pAttrLen += AttrLen;
	
	return ERROR_SUCCESS;
}

/**************************************************************************//*!
 *
 * @name  PushProdSpecEntry
 *
 * @brief This function pushes ProdSpecEntry Data Type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pProdSpecEntry	:	Pointer to ProdSpecEntry Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes ProdSpecEntry Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushProdSpecEntry(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */         
	ProdSpecEntry *pProdSpecEntry, 	/* [IN] Pointer to ProdSpecEntry Data Type*/        
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */ 
)
{
	ERR_CODE err;
	intu16 AttrLen;
	
	*pAttrLen = 0;
	do
	{
		pProdSpecEntry->prod_spec.length = ieee_htons(pProdSpecEntry->prod_spec.length);
		if(ERROR_SUCCESS != (err = Pushoctet_string(pBuffStack, 
			&pProdSpecEntry->prod_spec, &AttrLen)))
		{
			break;
		}
		*pAttrLen = AttrLen;
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				ieee_htons(pProdSpecEntry->spec_type), 
			pProdSpecEntry->component_id, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushProdSpecEntryList
 *
 * @brief This function pushes ProdSpecEntryList Data Type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param SpecCount			:	Spec Count
 * @param pProdSpecEntry	:	Pointer to ProdSpecEntry Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes ProdSpecEntryList Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushProdSpecEntryList(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */          
	intu16 SpecCount, 				/* [IN] Spec Count */
	ProdSpecEntry *pProdSpecEntry, 	/* [IN] Pointer to ProdSpecEntry Data Type*/
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;	
	
	*pAttrLen = 0;
	if(1 != SpecCount)
	{
		intu16 AttrLen = 0;
		ProdSpecEntry *temp = pProdSpecEntry;
		(uint_8_ptr)temp += sizeof(intu16) + sizeof(PrivateOid) + 
			OCTET_STRING_HEADER_SIZE + pProdSpecEntry->prod_spec.length;
		if(ERROR_SUCCESS == (err = PushProdSpecEntryList(pBuffStack, 
			(intu16)(SpecCount - 1), temp, &AttrLen)))
		{
			*pAttrLen += AttrLen;

			if(ERROR_SUCCESS == (err = PushProdSpecEntry(pBuffStack, 
				pProdSpecEntry, &AttrLen)))
			{
				*pAttrLen += AttrLen;
			}
			else
			{
				(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
				*pAttrLen = 0;
			}
		} 
		else
		{
			(void)PopStack(pBuffStack, AttrLen, NULL, NULL);
		}
		return err;
	}
	return PushProdSpecEntry(pBuffStack, pProdSpecEntry, pAttrLen);
}

/**************************************************************************//*!
 *
 * @name  PushProdSpec
 *
 * @brief This function pushes ProdSpec Data Type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pProdSpec			:	Pointer to ProdSpec Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes ProdSpec Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushProductionSpec(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */          
	ProductionSpec *pProdSpec, 	/* [IN] Pointer to ProdSpec Data Type */
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */  
)								
{
	ERR_CODE err;
	intu16 AttrLen;
	intu16 val;	
	
	*pAttrLen = 0;
	do 
	{
		if(ERROR_SUCCESS != (err = PushProdSpecEntryList(pBuffStack, 
				ieee_htons(pProdSpec->count), (ProdSpecEntry *)&pProdSpec->value, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		val = pProdSpec->length;
		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, &val, sizeof(val))))
		{
			break;
		}
		*pAttrLen += sizeof(val);
		val = pProdSpec->count;
		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, &val, sizeof(val))))
		{
			break;
		}
		*pAttrLen += sizeof(val);
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushMdsTimeInfo
 *
 * @brief This function pushes MdsTimeInfo Data Type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pMdsTimeInfo		:	Pointer to MdsTimeInfo Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes MdsTimeInfo Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushMdsTimeInfo(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */        
	MdsTimeInfo *pMdsTimeInfo, 	/* [IN] Pointer to MdsTimeInfo Data Type */  
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen = 0;
	intu32 val;
	
	*pAttrLen = 0;
	do
	{
		val = ieee_htonl(pMdsTimeInfo->time_resolution_high_res_time);
		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, &val, sizeof(val))))
		{
			break;
		}
		*pAttrLen = sizeof(val);
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
			pMdsTimeInfo->time_resolution_abs_time, 
			pMdsTimeInfo->time_resolution_rel_time, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushRelativeTime(pBuffStack, 
			pMdsTimeInfo->time_sync_accuracy, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;

		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
			pMdsTimeInfo->mds_time_cap_state, 
			pMdsTimeInfo->time_sync_protocol, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushBatMeasure
 *
 * @brief This function pushes BatMeasure Data Type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pBatMeasure		:	Pointer to BatMeasure Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes BatMeasure Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushBatMeasure(
	PTR_BUFFSTACK pBuffStack, /* [IN] Pointer to Buffer Stack */         
	BatMeasure *pBatMeasure,  /* [IN] Pointer to BatMeasure Data Type */
	intu16 *pAttrLen		  /* [OUT] Pointer to size of data pushed */ 
)
{
	ERR_CODE err;
	intu16 AttrLen = 0;
	intu16 val1;
	
	*pAttrLen = 0;
	
	val1 = ieee_htons(pBatMeasure->unit);
	err = PushStack(pBuffStack, &val1, sizeof(intu16));
	
	if(err == ERROR_SUCCESS)
	{
	  err = PushFloat(pBuffStack, pBatMeasure->value, &AttrLen);
	}
	
	if(err != ERROR_SUCCESS)
	{
	 (void)PopStack(pBuffStack, AttrLen, NULL, NULL);	
	}
	else
	{
	 *pAttrLen = (intu16)(AttrLen + sizeof(intu16));
	}
	
  return err;	
}

/**************************************************************************//*!
 *
 * @name  PushAttrValMapEntry
 *
 * @brief This function pushes AttrValMapEntry Data Type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pAttrValMapEntry	:	Pointer to AttrValMapEntry Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes AttrValMapEntry Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushAttrValMapEntry(
	PTR_BUFFSTACK pBuffStack,           /* [IN] Pointer to Buffer Stack */   
	AttrValMapEntry *pAttrValMapEntry,  /* [IN] Pointer to AttrValMapEntry Data 
	                                            Type */
	intu16 *pAttrLen                    /* [OUT] Pointer to size of data 
	                                            pushed */ 
)
{
	ERR_CODE err;
	intu16 val;
	
	*pAttrLen = 0;
	do
	{
	    
		val = pAttrValMapEntry->attribute_len;
		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, &val, sizeof(val))))
		{
			break;
		}
		*pAttrLen += sizeof(val);
		val = pAttrValMapEntry->attribute_id;
		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, &val, sizeof(val))))
		{
			break;
		}
		*pAttrLen += sizeof(val);

	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushAttrValMapEntryList
 *
 * @brief This function recursively pushes AttrValMapEntry Data Type 
 *        to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param AttrCount			:	Attribute Count
 * @param pAttrValMapEntry	:	Pointer to AttrValMapEntry Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function recursively pushes AttrValMapEntry Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushAttrValMapEntryList(
	PTR_BUFFSTACK pBuffStack,          /* [IN] Pointer to Buffer Stack */
	intu16 AttrCount,                  /* [IN] Attribute Count */
	AttrValMapEntry *pAttrValMapEntry, /* [IN] Pointer to AttrValMapEntry Data 
	                                            Type */
	intu16 *pAttrLen                   /* [OUT] Pointer to size of data 
	                                            pushed */
)
{
	ERR_CODE err;
	
	*pAttrLen = 0;
	if(1 != AttrCount)
	{
		intu16 AttrLen = 0;
		AttrValMapEntry *temp = pAttrValMapEntry;
		(uint_8_ptr)temp += sizeof(intu16) + sizeof(OID_Type);
		if(ERROR_SUCCESS == (err = PushAttrValMapEntryList(pBuffStack, 
			(intu16)(AttrCount - 1), temp, &AttrLen)))
		{
			*pAttrLen += AttrLen;

			if(ERROR_SUCCESS == (err = PushAttrValMapEntry(pBuffStack, 
				pAttrValMapEntry, &AttrLen)))
			{
				*pAttrLen += AttrLen;
			}
			else
			{
				(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
				*pAttrLen = 0;
			}
		} 
		else
		{
			(void)PopStack(pBuffStack, AttrLen, NULL, NULL);
		}
		return err;
	}
	return PushAttrValMapEntry(pBuffStack, pAttrValMapEntry, pAttrLen);
}

/**************************************************************************//*!
 *
 * @name  PushAttrValMap
 *
 * @brief This function pushes AttrValMap Data Type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pAttrValMap		:	Pointer to AttrValMap Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes AttrValMap Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushAttrValMap(
	PTR_BUFFSTACK pBuffStack,   /* [IN] Pointer to Buffer Stack */ 
	AttrValMap *pAttrValMap,    /* [IN] Pointer to AttrValMap Data Type */
	intu16 *pAttrLen            /* [IN] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen;
	
  *pAttrLen = 0;
  
	do
	{
		if(ERROR_SUCCESS != (err = PushAttrValMapEntryList(pBuffStack,
				ieee_htons(pAttrValMap->count), (AttrValMapEntry *)&pAttrValMap->value, 
				&AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				ieee_htons(pAttrValMap->count), ieee_htons(pAttrValMap->length), &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushSystemModel
 *
 * @brief This function pushes SystemModel Data Type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pSystemModel		:	Pointer to SystemModel Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes SystemModel Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushSystemModel(
	PTR_BUFFSTACK pBuffStack,   /* [IN] Pointer to Buffer Stack */ 
	SystemModel *pSystemModel,  /* [IN] Pointer to SystemModel Data Type */  
	intu16 *pAttrLen            /* [IN] Pointer to size of data pushed */ 
)
{
	ERR_CODE err;
	intu16 AttrLen;

	octet_string *model =  &pSystemModel->manufacturer;

	*pAttrLen = 0;
	(intu8 *)model += OCTET_STRING_HEADER_SIZE + ieee_htons(model->length);
	model->length = ieee_htons(model->length);
	if(ERROR_SUCCESS != (err = Pushoctet_string(pBuffStack, model, &AttrLen)))
	{
		return err;
	}
	*pAttrLen += AttrLen;
	pSystemModel->manufacturer.length = ieee_htons(pSystemModel->manufacturer.length);
	if(ERROR_SUCCESS != (err = Pushoctet_string(pBuffStack, 
		&pSystemModel->manufacturer, &AttrLen)))
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	*pAttrLen += AttrLen;
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushTYPE
 *
 * @brief This function pushes TYPE Data Type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pTYPE				:	Pointer to TYPE Data Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes TYPE Data Type to Buffer Stack
 *****************************************************************************/
ERR_CODE PushTYPE(
	PTR_BUFFSTACK pBuffStack,   /* [IN] Pointer to Buffer Stack */ 
	TYPE *pTYPE,                /* [IN] Pointer to TYPE Data Type */ 
	uint_16 *pAttrLen            /* [IN] Pointer to size of data pushed */ 
)
{
	return PushAttribute(pBuffStack, ieee_htons(pTYPE->partition), ieee_htons(pTYPE->code), pAttrLen);
}

/**************************************************************************//*!
 *
 * @name  PushAllMdsAttribute
 *
 * @brief This function pushes All MDS Attributes to Buffer Stack
 *
 * @param pMds				:	Pointer to MDS
 * @param pBuffStack		:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes All MDS Attributes to Buffer Stack
 *****************************************************************************/
ERR_CODE PushAllMdsAttribute(
	PMDS pMds,                  /* [IN] Pointer to MDS */
	PTR_BUFFSTACK pBuffStack    /* [IN] Pointer to Buffer Stack */
)
{
	ERR_CODE err;
	intu16 AttrCount = 0;
	intu16 AttrLength = 0;
	intu16 AttrLen;

	do
	{
        if(pMds->pMdcAttrConfirmTimeout != NULL) 
        {          
          if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_CONFIRM_TIMEOUT, &AttrLen)))
          {
            break;
          }
          AttrLength += AttrLen;
          AttrCount ++;
        }
        
        if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_REG_CERT_DATA_LIST, &AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;
        AttrCount ++;

        if(pMds->pMdcAttrTimeBattRemain != NULL) 
        {          
          if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_TIME_BATT_REMAIN, &AttrLen)))
          {
            break;
          }
          AttrLength += AttrLen;
          AttrCount ++;
        }

        if(pMds->pMdcAttrValBattCharge != NULL) 
        {          
          if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_VAL_BATT_CHARGE, &AttrLen)))
          {
            break;
          }
          AttrLength += AttrLen;
          AttrCount ++;
        }
        
        if(pMds->pMdcAttrPowerStatus != NULL) 
        {          
          if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_POWER_STAT, &AttrLen)))
          {
            break;
          }
          AttrLength += AttrLen;
          AttrCount ++;
        }

        if(pMds->pMdcAttrTimeAbsAdjust != NULL) 
        {          
          if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_TIME_ABS_ADJUST, &AttrLen)))
          {
            break;
          }
          AttrLength += AttrLen;
          AttrCount ++;
        }

        if(pMds->pMdcAttrTimeAbs != NULL) 
        {          
          if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_TIME_ABS, &AttrLen)))
          {
            break;
          }
          AttrLength += AttrLen;
          AttrCount ++;
        }
        
        if(pMds->pMdcAttrMdsTimeInfo != NULL) 
        {          
          if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_MDS_TIME_INFO, &AttrLen)))
          {
            break;
          }
          AttrLength += AttrLen;
          AttrCount ++;
          
          if(pMds->pMdcAttrMdsTimeInfo->mds_time_cap_state & MDS_TIME_CAPAB_RELATIVE_TIME) 
          {            
            if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_TIME_REL, &AttrLen)))
            {
              break;
            }
            AttrLength += AttrLen;
            AttrCount ++;
          }

          if(pMds->pMdcAttrMdsTimeInfo->mds_time_cap_state & MDS_TIME_CAPAB_HIGH_RES_RELATIVE_TIME) 
          {          
            if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_TIME_REL_HI_RES, &AttrLen)))
            {
              break;
            }
            AttrLength += AttrLen;
            AttrCount ++;
          }
        }
        
        if(pMds->pMdcAttrIdProdSpecn != NULL)
        { 
          err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_ID_PROD_SPECN, &AttrLen);
          
          if(err != ERROR_SUCCESS)
          {
            break;
          }
          
          AttrLength += AttrLen;
          AttrCount ++;
        }
        
        if(pMds->pMdcAttrValMap != NULL) 
        {          
          err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_ATTRIBUTE_VAL_MAP, &AttrLen);
          if(err != ERROR_SUCCESS)
          {
            break;
          }
          AttrLength += AttrLen;
          AttrCount ++;
	      }
	      
        if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, 
            MDC_ATTR_DEV_CONFIG_ID, &AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;
        AttrCount ++;
        
        if(pMds->pMdcAttrSysId != NULL)
        {
         err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_SYS_ID, &AttrLen);
         if(err != ERROR_SUCCESS)
         {
            break;
         }
         AttrLength += AttrLen;
         AttrCount ++;
        }
        
        if(pMds->pMdcAttrSysModel != NULL)
        {
         err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_ID_MODEL, &AttrLen);
         if(err != ERROR_SUCCESS)
         {
            break;
         }
         AttrLength += AttrLen;
         AttrCount ++;
        }

        if(pMds->pMdcAttrSysType != NULL) 
        {  
          err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_SYS_TYPE, &AttrLen);
          
          if(err != ERROR_SUCCESS)
          {
            break;
          }
          AttrLength += AttrLen;
          AttrCount ++;
        } 
        
        if(pMds->pMdcAttrSysTypeSpecList != NULL) 
        {  
          err = PushMdsAttribute(pBuffStack, pMds, MDC_ATTR_SYS_TYPE_SPEC_LIST, &AttrLen);
          if(err != ERROR_SUCCESS)
          {
            break;
          }
          AttrLength += AttrLen;
          AttrCount ++;
        }
        
        if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, AttrCount, 
			AttrLength, &AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;
        if(ERROR_SUCCESS != (err = PushHANDLE(pBuffStack, pMds->MdcAttrIdHandle, 
            &AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;

	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, AttrLength, NULL, NULL);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  CompareAbsTime
 *
 * @brief This function comapres 2 Absolute Time Structures
 *
 * @param pAbsA			:	Pointer to Absolute Time A
 * @param pAbsB			:	Pointer to Absolute Time B
 * @param pAttrLen		:	Pointer to size of data pushed
 *
 * @return int
 *			 0			: if equal
 *		     1			: if A > B
 *			-1			> if B > A
 ******************************************************************************
 * This function comapres 2 Absolute Time Structures
 *****************************************************************************/
static int CompareAbsTime(
	AbsoluteTime *pAbsA,    /* [IN] Pointer to Absolute Time A */
	AbsoluteTime *pAbsB     /* [IN] Pointer to Absolute Time B */
)
{
    int Val = strncmp((const char*)pAbsA, (char*)pAbsB, sizeof(AbsoluteTime));
    return ((Val < 0) ? -1 : (Val > 0) ? 1:0);
}

/**************************************************************************//*!
 *
 * @name  GetPmSegmentById
 *
 * @brief This function returns PmSegment pointer based on Instance number
 *
 * @param pPmStore			:	Pointer to PmStore
 * @param InstNum			:	PM Segment Instance Number
 *
 * @return Pointer to PM Segment if Successful otherwise NULL
 ******************************************************************************
 * This function returns PmSegment pointer based on Instance number
 *****************************************************************************/
static PPMSEGMENT GetPmSegmentById(
	PPMSTORE pPmStore,			/* [IN] Pointer to PmStore */
	InstNumber InstNum			/* [IN] PM Segment Instance Number */
)
{
    intu16 i;

    PPMSEGMENT pPmSegm = &(*pPmStore->pPmSegment)[0];
    for (i = 0; i < pPmStore->PmSegmentCount; i++)
    {    
        if(InstNum == pPmSegm->MdcAttrIdInstNo)
            return pPmSegm;
        pPmSegm++;
    }
    
    return NULL;    
}

/**************************************************************************//*!
 *
 * @name  ClearPmSegment
 *
 * @brief This function Clears PM Segment
 *
 * @param pSegmentSel		:	Pointer to Segment Selection
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pPmStore			:	Pointer to PmStore
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS		: If Successful
 *			ERROR_OBJECT_BUSY	: If PM Segment is being modified
 *			ERROR_UNSUPPORTED	    : If operation not supported
 ******************************************************************************
 * This function Clears PM Segment
 *****************************************************************************/
static ERR_CODE ClearPmSegment(
	SegmSelection* pSegmentSel,	/* [IN] Pointer to Segment Selection */ 
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */
    PPMSTORE pPmStore			/* [IN] Pointer to PmStore */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 i;
    CLRPMSEGMINFO ClrSegmInfo;
    
    ClrSegmInfo.Handle = pPmStore->MdcAttrIdHandle;
    switch(ieee_ntohs(pSegmentSel->choice))
    {
        case ALL_SEGMENTS_CHOSEN:
        {
            PPMSEGMENT pPmSegm = &(*pPmStore->pPmSegment)[0];
            for(i = 0; i < pPmStore->PmSegmentCount; i++)
            {
                if(pPmSegm->MdcAttrIdInstNo != INVALID_PMSEG_INST)
                {                                
                  if(pPmSegm->MdcAttrOpStat == TRUE)
                  {
                      return ERROR_OBJECT_BUSY;                  
                  }
                }
                
                pPmSegm++;
            }
            
            pPmSegm = &(*pPmStore->pPmSegment)[0];
            for(i = 0; i < pPmStore->PmSegmentCount; i++)
            {
                if(pPmSegm->MdcAttrIdInstNo != INVALID_PMSEG_INST)
                {

                  ClrSegmInfo.SegmInstNum = pPmSegm->MdcAttrIdInstNo;
                
                  if(pPmStore->MdcAttrPmStoreCapab & PMSC_CLEAR_SEGM_REMOVE)
                  {
                      /* Remove PM Segment Completely */
                      (void)ClearPmSegm(pPmStore, pPmSegm, TRUE);
                  }
                  else
                  {
                      /* Erase PM Segment Data */
                      (void)ClearPmSegm(pPmStore, pPmSegm, FALSE);
                  }
                  Ieee11073GetAppCallback()(IEEE11073_CLEAR_PMSEGMENT, 
                      (void*)&ClrSegmInfo);
                }
                
                pPmSegm++;
            }
            break;
        }
        case SEGM_ID_LIST_CHOSEN:
        {
            InstNumber *pInst;
            if(!(pPmStore->MdcAttrPmStoreCapab & PMSC_CLEAR_SEGM_BY_LIST_SUP))
            {
                err = ERROR_UNSUPPORTED;
                break;
            }
                
            pInst = &pSegmentSel->u.segm_id_list.value[0]; 
            for(i = 0; i < ieee_ntohs(pSegmentSel->u.segm_id_list.count); i++)
            {
                PPMSEGMENT pPmSegm;
                pPmSegm = GetPmSegmentById(pPmStore, ieee_ntohs(pInst[i])); 
                if((pPmSegm != NULL) && (pPmSegm->MdcAttrOpStat == TRUE))
                {
                    return ERROR_OBJECT_BUSY;
                }
            }
            for(i = 0; i < ieee_ntohs(pSegmentSel->u.segm_id_list.count); i++)
            {
                PPMSEGMENT pPmSegm = GetPmSegmentById(pPmStore,ieee_ntohs(pInst[i]));
			          
			          if(pPmSegm == NULL)
			          {
			            return ERROR_UNSUPPORTED;
			          }
                if(pPmSegm->MdcAttrIdInstNo == INVALID_PMSEG_INST)
                {
                    continue;
                }
                ClrSegmInfo.SegmInstNum = ieee_ntohs(pInst[i]);

                if(pPmStore->MdcAttrPmStoreCapab & PMSC_CLEAR_SEGM_REMOVE)
                {
                    /* Remove PM Segment Completely */
                    (void)ClearPmSegm(pPmStore, pPmSegm, TRUE);
                }
                else
                {
                    /* Erase PM Segment Entry */
                    (void)ClearPmSegm(pPmStore, pPmSegm, FALSE);
                }
                Ieee11073GetAppCallback()(IEEE11073_CLEAR_PMSEGMENT, 
                    (void*)&ClrSegmInfo);
            } 
            break;
        }
        case ABS_TIME_RANGE_CHOSEN:
        {
            AbsTimeRange *pAbsTimeRange;
            intu16 SegMatchFlag = FALSE;
            PPMSEGMENT pPmSegm = &(*pPmStore->pPmSegment)[0];
            if(!(pPmStore->MdcAttrPmStoreCapab & PMSC_CLEAR_SEGM_BY_TIME_SUP))
            {
                err = ERROR_UNSUPPORTED;
                break;
            }
            
            pAbsTimeRange = &pSegmentSel->u.abs_time_range;
            
            for(i = 0; i < pPmStore->PmSegmentCount; i++)
            {
                intu16 Flag;
                if(pPmSegm->MdcAttrIdInstNo != INVALID_PMSEG_INST)
                {
                  Flag = (intu16)((pPmSegm->OptionalAttributeFlag & OPT_PMSEG_ABS_TIME) >> 3);
                  if(Flag == TRUE)
                  {
                    
                    int FromComp = CompareAbsTime(&pAbsTimeRange->from_time, &pPmSegm->MdcAttrTimeStartSeg);
                    int ToComp = CompareAbsTime(&pAbsTimeRange->to_time, &pPmSegm->MdcAttrTimeEndSeg);
                    
                    if((pPmSegm[i].MdcAttrOpStat == TRUE) &&
                       (FromComp <= 0) && (ToComp >= 0)) 
                    {
                        return ERROR_OBJECT_BUSY;
                    }
                  }
                }
               pPmSegm++; 
            }
            
            pPmSegm = &(*pPmStore->pPmSegment)[0];
            for(i = 0; i < pPmStore->PmSegmentCount; i++)
            {
                intu16 Flag;

                if(pPmSegm->MdcAttrIdInstNo != INVALID_PMSEG_INST)
                {                                
                  Flag = (intu16)((pPmSegm->OptionalAttributeFlag & OPT_PMSEG_ABS_TIME) >> 3);
                  if(Flag == TRUE)
                  {
                    
                    int FromComp = CompareAbsTime(&pAbsTimeRange->from_time, &pPmSegm->MdcAttrTimeStartSeg);
                    int ToComp = CompareAbsTime(&pAbsTimeRange->to_time, &pPmSegm->MdcAttrTimeEndSeg);
                    if((FromComp <= 0) && (ToComp >= 0)) 
                    {
                        SegMatchFlag = TRUE;
                        ClrSegmInfo.SegmInstNum = pPmSegm->MdcAttrIdInstNo;

                        if(pPmStore->MdcAttrPmStoreCapab & PMSC_CLEAR_SEGM_REMOVE)
                        {
                            /* Remove PM Segment Completely */
                            (void)ClearPmSegm(pPmStore, pPmSegm, TRUE);
                        }
                        else
                        {
                            /* Erase PM Segment Entry */
                            (void)ClearPmSegm(pPmStore, pPmSegm, FALSE);
                        }
                        Ieee11073GetAppCallback()(IEEE11073_CLEAR_PMSEGMENT, 
                            (void*)&ClrSegmInfo);
                    }
                  }
                }
               pPmSegm++; 
            }
            if(SegMatchFlag == FALSE)
            {
                return ERROR_UNSUPPORTED;
            }
            break;
        }
        default :
            err = ERROR_UNSUPPORTED;
            break;
    }
    if(ERROR_SUCCESS == err)
    {
        intu16 AttrLen;
        /* push any header */
        err = Pushintu16(pBuffStack, 0, &AttrLen);
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  ClearPmSegm
 *
 * @brief This function Clears PM Segment
 *
 * @param pPmStore		:	Pointer to PmStore
 * @param pPmSegm		:	Pointer to PM Segment
 * @param bDelete		:	Delete Flag
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS		: If Successful
 *			ERROR_INVALID_PARAM	: If error
 ******************************************************************************
 * This function Clears PM Segment
 *****************************************************************************/
static ERR_CODE ClearPmSegm(
	PPMSTORE pPmStore,		/* [IN] Pointer to PmStore */
	PPMSEGMENT pPmSegm,		/* [IN] Pointer to PM Segment */
	intu8 bDelete			/* [IN] Delete Flag */
)
{
    if((pPmSegm == NULL) || (pPmStore == NULL))
    {
        return ERROR_INVALID_PARAM;
    }
    
    if((pPmStore->OptionalAttributeFlag & OPT_PMSTORE_USAGE_CNT) &&
    (pPmSegm->OptionalAttributeFlag & OPT_PMSEG_USAGE_CNT))
    {
        pPmStore->MdcAttrMetricStoreUsageCnt -= pPmSegm->MdcAttrsegUsageCnt;
    }

    if(bDelete == TRUE)
    {
        /* Remove PM Store Completely */
        pPmStore->MdcAttrNumSeg--;
        pPmSegm->MdcAttrIdInstNo = INVALID_PMSEG_INST;
        return ERROR_SUCCESS;
    }
    /* Reset PM Segment Entry in PMStore */
    if(pPmSegm->OptionalAttributeFlag & OPT_PMSEG_USAGE_CNT)
    {
        pPmSegm->MdcAttrsegUsageCnt = 0;
    }   
    if(pPmSegm->OptionalAttributeFlag & OPT_PMSEG_ABS_TIME)
    {
        (void)memset(&pPmSegm->MdcAttrTimeStartSeg, 0xFF, sizeof(AbsoluteTime));
        (void)memset(&pPmSegm->MdcAttrTimeEndSeg, 0xFF, sizeof(AbsoluteTime));
    }
    if(pPmSegm->OptionalAttributeFlag & OPT_PMSEG_DATE_TIME_ADJUST)
    {
        (void)memset(&pPmSegm->MdcAttrTimeAbsAdjust, 0x00, sizeof(AbsoluteTimeAdjust));
    }
    return ERROR_SUCCESS;   
}

/**************************************************************************//*!
 *
 * @name  GetPmSegInfoSize
 *
 * @brief This function returns PM Segment Info Size
 *
 * @param pPMSegment		:	Pointer to PM Segment 
 *
 * @return PM Segment Info Size
 ******************************************************************************
 * This function returns PM Segment Info Size
 *****************************************************************************/
intu16 GetPmSegInfoSize(
	PPMSEGMENT pPMSegment		/* [IN] Pointer to PM Segment */
)
{
    intu16 size = sizeof(RelativeTime) + ATTR_HEADER_SIZE;

    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_CONFIRM_TIMEOUT)
    {
        size += sizeof(RelativeTime) + ATTR_HEADER_SIZE;
    }
    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_SEG_STATISTICS)
	{
	    size += ieee_htons(pPMSegment->pMdcAttrSegStats->length) + 
			SEGMENT_STATISTICS_HEADER_SIZE + ATTR_HEADER_SIZE;
	}
    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_USAGE_CNT)
    {
        size += sizeof(intu32) + ATTR_HEADER_SIZE;
    }
    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_DATE_TIME_ADJUST)
    {
    	size += sizeof(AbsoluteTimeAdjust) + ATTR_HEADER_SIZE;
    }
    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_ABS_TIME)
	{
    	size += sizeof(AbsoluteTime) + ATTR_HEADER_SIZE;
    	size += sizeof(AbsoluteTime) + ATTR_HEADER_SIZE;
	}
	if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_LABEL)
	{
    	size += ieee_htons(pPMSegment->pMdcAttrPmSegLabelString->length) + 
			OCTET_STRING_HEADER_SIZE + ATTR_HEADER_SIZE;
	}
	if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_SAMPLE_PERIOD)
	{
        size += sizeof(RelativeTime) + ATTR_HEADER_SIZE;
	}  
	
	size += sizeof(OperationalState) + ATTR_HEADER_SIZE;
	
    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_PERSON_ID)
    {
        size += sizeof(PersonId) + ATTR_HEADER_SIZE;
    }
		
    size += sizeof(SegmEntryHeader) + 
    ieee_htons(pPMSegment->pMdcAttrPmSegMap->segm_entry_elem_list.length) + 
        SEG_ENTRY_LIST_HEADER_SIZE + ATTR_HEADER_SIZE;        

    size += sizeof(InstNumber) + ATTR_HEADER_SIZE;
    
    size += sizeof(InstNumber) + ATTR_HEADER_SIZE;
    
    return size;
}

/**************************************************************************//*!
 *
 * @name  GetPmSegSelInfoSize
 *
 * @brief This function returns PM Segment Selection Info Size
 *
 * @param pPmStore		:	Pointer to PM Store
 * @param pSegmSel		:	Pointer to Segment Selection
 *
 * @return PM Segment Selection Info Size
 ******************************************************************************
 * This function returns PM Segment Selection Info Size
 *****************************************************************************/
intu16 GetPmSegSelInfoSize(
	PPMSTORE pPmStore,			/* [IN] Pointer to PM Store */
	SegmSelection *pSegmSel		/* [IN] Pointer to Segment Selection */
)
{
    intu16 size = SEGMENT_INFO_LIST_HEADER_SIZE;
    intu16 i;
    
    switch(ieee_ntohs(pSegmSel->choice))
    {
        case ALL_SEGMENTS_CHOSEN:
           {            
            PPMSEGMENT pPmSegm = &(*pPmStore->pPmSegment)[0];
            for(i = 0; i < pPmStore->PmSegmentCount; i++)
            {
                size += GetPmSegInfoSize(pPmSegm++);
            }
           }
            break;
        case SEGM_ID_LIST_CHOSEN:
        {
            SegmIdList *pSegmIdList = &pSegmSel->u.segm_id_list;
            InstNumber *pInst = &pSegmIdList->value[0];
            for(i = 0; i < ieee_ntohs(pSegmIdList->count); i++)
            {
                intu16 j;
                PPMSEGMENT pPmSegm = &(*pPmStore->pPmSegment)[0];
                for(j = 0; j < pPmStore->PmSegmentCount; j++)
                {
                    if(pInst[i] == pPmSegm->MdcAttrIdInstNo)
                    {
                        size += GetPmSegInfoSize(pPmSegm);
                        break;
                    }
                    pPmSegm++;
                }
            }
            break;
        }
        case ABS_TIME_RANGE_CHOSEN:
        {
            AbsTimeRange *pAbsTimeRange = &pSegmSel->u.abs_time_range;
            
            PPMSEGMENT pPmSegm = &(*pPmStore->pPmSegment)[0];
            for(i = 0; i < pPmStore->PmSegmentCount; i++)
            {
                intu16 Flag = (intu16)(pPmSegm->OptionalAttributeFlag & OPT_PMSEG_ABS_TIME);
                if(Flag == TRUE)
                {
                    
                    int FromComp = CompareAbsTime(&pAbsTimeRange->from_time, &pPmSegm->MdcAttrTimeStartSeg);
                    int ToComp = CompareAbsTime(&pAbsTimeRange->to_time, &pPmSegm->MdcAttrTimeEndSeg);
                    if((FromComp <= 0) && (ToComp >= 0)) 
                    {
                        size += GetPmSegInfoSize(pPmSegm);
                    }
                }
                pPmSegm++;
            }
            break;
        }
        default :
            size = 0;
    }
    return size;    
}

/**************************************************************************//*!
 *
 * @name  PushPmSegInfoHeader
 *
 * @brief This function pushes PM Segment Info Header to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param Count				:	Number of PM Segments
 * @param Length			:	Length of Data
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes PM Segment Info Header to Buffer Stack
 *****************************************************************************/
ERR_CODE PushPmSegInfoHeader(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */     
	intu16 Count, 				/* [IN] Number of PM Segments */
	intu16 Length, 				/* [IN] Length of Data */                
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */
)
{
    ERR_CODE err;       
    intu16 AttrLen;   
    
    *pAttrLen = 0;
    do
    {        
        /* push segment info list header */
        if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, Count, Length, 
			&AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;

        /* push any header */
        if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, 
			(intu16)(Length + AttrLen), &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  PushAllPmSegInfo
 *
 * @brief This function pushes All PM Segment Info to Buffer Stack
 *
 * @param pPmStore			:	Pointer to PM Store
 * @param pBuffStack		:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes All PM Segment Info to Buffer Stack
 *****************************************************************************/
ERR_CODE PushAllPmSegInfo(
	PPMSTORE pPmStore,			/* [IN] Pointer to PM Store */
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
    ERR_CODE err = ERROR_SUCCESS;   
    intu16 i; 
    intu16 AttrLength = 0;
    intu16 AttrLen;    
    intu16 PmSegmCnt = 0;
    do
    {        
        PPMSEGMENT pPmSegment = &(*pPmStore->pPmSegment)[0];
        for(i = 0; i < pPmStore->PmSegmentCount; i++)
        {
            if(pPmSegment->MdcAttrIdInstNo != INVALID_PMSEG_INST)
            {
                if(ERROR_SUCCESS != (err = PushPMSegmentAttribute(pPmSegment, pBuffStack, &AttrLen)))
                {
                    break;
                }
                AttrLength += AttrLen;
                PmSegmCnt++;
            }
            pPmSegment++;
        }
        if(ERROR_SUCCESS != err)
        {
            break;
        }
        if(ERROR_SUCCESS != (err = PushPmSegInfoHeader(pBuffStack, 
            PmSegmCnt, AttrLength, &AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, AttrLength, NULL, NULL);
    }
    return err;    
}

/**************************************************************************//*!
 *
 * @name  PushPmSegmentInfoID
 *
 * @brief This function pushes PM Segment Info based on Segment ID List 
 *        to Buffer Stack
 *
 * @param pSegmIdList		:	Pointer to Segment ID List
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pPmStore			:	Pointer to PM Store
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes PM Segment Info based on Segment ID List to Buffer Stack
 ******************************************************************************/
ERR_CODE PushPmSegmentInfoID(
	SegmIdList *pSegmIdList,	/* [IN] Pointer to Segment ID List */
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */  
	PPMSTORE pPmStore			/* [IN] Pointer to PM Store */       
)
{
    ERR_CODE err;   
    intu16 i; 
    intu16 AttrCount = 0;
    intu16 AttrLength = 0;
    intu16 AttrLen;
    
    if(pSegmIdList == NULL)
    {
     return ERROR_INVALID_PARAM;
    }
    
    do
    {        
        InstNumber *pInst = &pSegmIdList->value[0];
        for(i = 0; i < ieee_ntohs(pSegmIdList->count); i++)
        {
            intu16 j;
            PPMSEGMENT pPmSegm = &(*pPmStore->pPmSegment)[0];
            for(j = 0; j < pPmStore->PmSegmentCount; j++)
            {
                if(pInst[i] == pPmSegm->MdcAttrIdInstNo)
                {
                    if(ERROR_SUCCESS != (err = PushPMSegmentAttribute(pPmSegm, pBuffStack, &AttrLen)))
                    {
                        break;
                    }
                    AttrLength += AttrLen;
                    AttrCount++;
                }
              pPmSegm++;
            }
        }
        if(ERROR_SUCCESS != (err = PushPmSegInfoHeader(pBuffStack, 
            AttrCount, AttrLength, &AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, AttrLength, NULL, NULL);
    }
    return err;    
}

/**************************************************************************//*!
 *
 * @name  PushPmSegmentInfoTimeRange
 *
 * @brief This function pushes PM Segment Info based on Absolute Time Range 
 *        to Buffer Stack
 *
 * @param pAbsTimeRange		:	Pointer to Absolute Time Range
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pPmStore			:	Pointer to PM Store
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes PM Segment Info based on Absolute Time Range to Buffer 
 * Stack
 ******************************************************************************/
ERR_CODE PushPmSegmentInfoTimeRange(
	AbsTimeRange *pAbsTimeRange,	/* [IN] Pointer to Absolute Time Range */
    PTR_BUFFSTACK pBuffStack, 		/* [IN] Pointer to Buffer Stack */   
	PPMSTORE pPmStore				/* [IN] Pointer to PM Store */       
)
{
    ERR_CODE err;   
    intu16 i; 
    intu16 AttrCount = 0;
    intu16 AttrLength = 0;
    intu16 AttrLen;
    do
    {        
        PPMSEGMENT pPmSegm = &(*pPmStore->pPmSegment)[0];
        for(i = 0; i < pPmStore->PmSegmentCount; i++)
        {
            intu16 Flag = (intu16)(pPmSegm->OptionalAttributeFlag & OPT_PMSEG_ABS_TIME);
            if(Flag == TRUE)
            {
                
                int FromComp = CompareAbsTime(&pAbsTimeRange->from_time, &pPmSegm->MdcAttrTimeStartSeg);
                int ToComp = CompareAbsTime(&pAbsTimeRange->to_time, &pPmSegm->MdcAttrTimeEndSeg);
                if((FromComp <= 0) && (ToComp >= 0)) 
                {
                    if(ERROR_SUCCESS != (err = PushPMSegmentAttribute(pPmSegm, pBuffStack, &AttrLen)))
                    {
                        break;
                    }
                    AttrLength += AttrLen;
                    AttrCount++;
                }
            }
            pPmSegm++;
        }
        if(ERROR_SUCCESS != (err = PushPmSegInfoHeader(pBuffStack, 
            AttrCount, AttrLength, &AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, AttrLength, NULL, NULL);
    }
    return err;    
}

/**************************************************************************//*!
 *
 * @name  GetPmSegmentInfo
 *
 * @brief This function pushes PM Segment Info based on Segment Selection 
 *        to Buffer Stack
 *
 * @param pSegmentSel		:	Pointer to Segment Selection
 * @param pRspBuffStack		:	Pointer to Buffer Stack
 * @param pPmStore			:	Pointer to PM Store
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Request is not supported 
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes PM Segment Info based on Segment Selection to Buffer 
 * Stack
 ******************************************************************************/
ERR_CODE GetPmSegmentInfo(
	SegmSelection* pSegmentSel,		/* [IN] Pointer to Segment Selection */
	PTR_BUFFSTACK pRspBuffStack,	/* [IN] Pointer to Buffer Stack */     
	PPMSTORE pPmStore				/* [IN] Pointer to PM Store */         
)
{
    ERR_CODE err;
    switch(ieee_ntohs(pSegmentSel->choice))
    {
        case ALL_SEGMENTS_CHOSEN:
            /* 
				Check for operational state of the segments, 
				if enabled send roer 
				Get segment info of all pm segments 
				(instance num and attribute list)
			*/
            err = PushAllPmSegInfo(pPmStore, pRspBuffStack);            
            break;
        case SEGM_ID_LIST_CHOSEN:
            /* 
				Check for operational state of the segments, 
				if enabled send roer
				Get segment info of the pm segments 
				(instance num and attribute list) provided in the request 
			*/
            err = PushPmSegmentInfoID(&pSegmentSel->u.segm_id_list, 
				pRspBuffStack, pPmStore);
            break;
        case ABS_TIME_RANGE_CHOSEN:
            /* Check for operational state of the segments, if enabled send roer */
            /* Get segment info of all the pm segments in the given time range */        
            err = PushPmSegmentInfoTimeRange(&pSegmentSel->u.abs_time_range, 
				pRspBuffStack, pPmStore);
            break;
        default :
            err = ERROR_INVALID_REQUEST;
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  GetPMSegmentEntryElemDataSize
 *
 * @brief This function returns PM Segment Entry Element Data Size
 *
 * @param pSegmEntry		:	Pointer to Segment Entry Element
 *
 * @return PM Segment Entry Element Data Size
 *******************************************************************************
 * This function returns PM Segment Entry Element Data Size
 ******************************************************************************/
intu32 GetPMSegmentEntryElemDataSize(
	SegmEntryElem *pSegmEntry	/* [IN] Pointer to Segment Entry Element */
)
{
    AttrValMap *pAttrValMap = &pSegmEntry->attr_val_map;
    intu32 size = 0;
    intu16 i;
    AttrValMapEntry* pAttrValMapEntry = &pAttrValMap->value[0];
    for(i = 0; i < pAttrValMap->count; i++)
    {
        size += ieee_ntohs(pAttrValMapEntry->attribute_len);
        (intu8*)pAttrValMapEntry += ATTR_HEADER_SIZE;
    }
    return size;
}

/**************************************************************************//*!
 *
 * @name  GetPMSegmentSize
 *
 * @brief This function returns PM Segment Attribute Size
 *
 * @param pPmSegmEntMap		:	Pointer to PM Segment Entry Map
 *
 * @return PM Segment Entry Element Data Size
 *******************************************************************************
 * This function returns PM Segment Attribute Size
 ******************************************************************************/
intu32 GetPMSegmentSize(
	PmSegmentEntryMap *pPmSegmEntMap  /* [IN] Pointer to PM Segment Entry Map */
)
{   
    intu32 size;
    SegmEntryElemList *pSegmEntryList = &pPmSegmEntMap->segm_entry_elem_list;
    SegmEntryElem *pSegmEntry = &pSegmEntryList->value[0];
    intu16 i;

    switch(pPmSegmEntMap->segm_entry_header)
    {
        case SEG_ELEM_HDR_ABSOLUTE_TIME:
            size = sizeof(AbsoluteTime);
            break;
        case SEG_ELEM_HDR_RELATIVE_TIME:
            size = sizeof(RelativeTime);
            break;
        case SEG_ELEM_HDR_HIRES_RELATIVE_TIME:
            size = sizeof(HighResRelativeTime);
            break;
        default:
            size = 0;
            break;
    }
    for(i = 0; i < pSegmEntryList->count; i++)
    {
        size += GetPMSegmentEntryElemDataSize(pSegmEntry);
        (intu8*)pSegmEntry += pSegmEntry->attr_val_map.length + 
            SEG_ENTRY_ELEM_HEADER_SIZE + ATTR_HEADER_SIZE;
    }
    
    return size;
    
}

/**************************************************************************//*!
 *
 * @name  GetAttrListByHandle
 *
 * @brief This function returns Pointer to Attribute List from Observation Scan
 *		  List based on Handle
 *
 * @param Handle			:	Handle
 * @param pObsScanList		:	Pointer to Observation Scan List
 *
 * @return Pointer to Attribute List if Successful otherwise NULL
 *******************************************************************************
 * This function returns Pointer to Attribute List from Observation Scan List 
 * based on Handle
 ******************************************************************************/
AttributeList* GetAttrListByHandle(
	HANDLE Handle,					  /* [IN] Handle */
	ObservationScanList *pObsScanList /* [IN] Pointer to Observation Scan List */
)
{
    intu16 i;
    ObservationScan* pObsScan = &pObsScanList->value[0];
    for(i = 0; i < ieee_ntohs(pObsScanList->count); i++)
    {
        if(ieee_ntohs(pObsScan->obj_handle) == Handle)
        {
        	return &pObsScan->attributes;
        }
        (intu8*)pObsScan += ieee_ntohs(pObsScan->attributes.length) + sizeof(HANDLE) + ATTR_HEADER_SIZE; 
    }                                                            
    return (AttributeList*)NULL;    
}

/**************************************************************************//*!
 *
 * @name  GetAttrValueByAttrId
 *
 * @brief This function returns Pointer to Any Data Type from Attribute List 
 *        based on OID Type
 *
 * @param OidType		:	OID Type
 * @param pAttrList		:	Pointer to Attribute List
 *
 * @return Pointer to Pointer to Any Data Type if Successful otherwise NULL
 *******************************************************************************
 * This function returns Pointer to Any Data Type from Attribute List based on 
 * OID Type
 ******************************************************************************/
Any* GetAttrValueByAttrId(
	OID_Type OidType,			/* [IN] OID Type */
	AttributeList *pAttrList	/* [IN] Pointer to Attribute List */
)
{
    intu16 i;
    AVA_Type* pAvaType = &pAttrList->value[0];
    
    for(i = 0; i < ieee_ntohs(pAttrList->count); i++)
    {
        if(pAvaType->attribute_id == OidType)
        {
        	return &pAvaType->attribute_value;
        }
        (intu8*)pAvaType += sizeof(OID_Type) + ieee_ntohs(pAvaType->attribute_value.length) + ANY_HEADER_SIZE;
    }
    return (Any*)NULL;
}
/**************************************************************************//*!
 *
 * @name  PushPmSegmentEntryElemData
 *
 * @brief This function pushes PM Segment Entry Element Data based on 
 *		  Observation Scan List to pData
 *
 * @param pData			:	Pointer to Store PMSegment Entry Element Data
 * @param pSegmEntry	:	Pointer to PM Segment Entry
 * @param pObsScanList	:	Pointer to Observation Scan List
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *			ERROR_INVALID_DATA		: if Observation scan list does not contain
 *									  Attribute ID
 *******************************************************************************
 * This function pushes PM Segment Entry Element Data based on Observation Scan 
 * List to pData
 ******************************************************************************/
ERR_CODE PushPmSegmentEntryElemData(
	void* pData,						/* [IN] Pointer to Store PMSegment Entry 
										        Element Data */
	SegmEntryElem *pSegmEntry,			/* [IN] Pointer to PM Segment Entry */
    ObservationScanList *pObsScanList	/* [IN] Pointer to Observation Scan 
												List */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 i;
    AttributeList* pAttrList;
    AttrValMapEntry* pAttrValMapEntry = &pSegmEntry->attr_val_map.value[0];
    Any* pAny;
    
    pAttrList = GetAttrListByHandle(pSegmEntry->handle, pObsScanList);
    if(pAttrList == NULL)
        return ERROR_INVALID_DATA;
    
    for(i = 0; i < pSegmEntry->attr_val_map.count ; i++)
    {        
        pAny = GetAttrValueByAttrId(pAttrValMapEntry->attribute_id, pAttrList);
        
        /* If attribute Id is not found then incomplete data */
        if(pAny == NULL)
            return ERROR_INVALID_DATA;
        
        /* Data length should be equal to attribute length specified */
        if(pAny->length != pAttrValMapEntry->attribute_len)
            return ERROR_INVALID_DATA;
        /* Copy Data to Buffer */
        (void)memcpy(pData, &pAny->value[0], pAny->length);
        (intu8*)pData += pAny->length;
        
        (intu8*)pAttrValMapEntry += ATTR_HEADER_SIZE;
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  UpdatePmSegmEntryData
 *
 * @brief This function Updates PM Segment Entry Element Data conatined in 
 *        based on PM Segment Entry Map based on Observation Scan List to pData
 *
 * @param pPmSegmEntMap	:	Pointer to PM Segment Entry Map
 * @param pData			:	Pointer to Store PMSegment Entry Element Data
 * @param pObsScanList	:	Pointer to Observation Scan List
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *			ERROR_INVALID_DATA		: if Observation scan list does not contain
 *									  Attribute ID
 *******************************************************************************
 * This function Updates PM Segment Entry Element Data conatined in 
 * based on PM Segment Entry Map based on Observation Scan List to pData
 ******************************************************************************/
ERR_CODE UpdatePmSegmEntryData(
	PmSegmentEntryMap *pPmSegmEntMap, /* [IN] Pointer to PM Segment Entry Map */
	void* pData,					  /* [IN] Pointer to Store PMSegment Entry 
											  Element Data */
    ObservationScanList *pObsScanList /* [IN] Pointer to Observation Scan List */
)
{
    intu16 i;
    void* TimePtr;
    intu8 length;
    ERR_CODE err = ERROR_SUCCESS;
    AbsoluteTime AbsTime;
    RelativeTime RelTime;
    HighResRelativeTime HiResRelTime;
        
    SegmEntryElemList *pSegmEntryList = &pPmSegmEntMap->segm_entry_elem_list;
    SegmEntryElem *pSegmEntry = &pSegmEntryList->value[0];
    switch(pPmSegmEntMap->segm_entry_header)
    {
        case SEG_ELEM_HDR_ABSOLUTE_TIME:
        {            
            GetAbsoluteTime(&AbsTime);
            TimePtr = (void*)&AbsTime;
            length = sizeof(AbsoluteTime);
            break;
        }
        case SEG_ELEM_HDR_RELATIVE_TIME:
        {            
            GetRelativeTime(&RelTime);
            TimePtr = (void*)&RelTime;
            length = sizeof(RelativeTime);
            break;
        }
        case SEG_ELEM_HDR_HIRES_RELATIVE_TIME:
        {            
            GetHiResRelativeTime(&HiResRelTime);
            TimePtr = (void*)&HiResRelTime;
            length = sizeof(HighResRelativeTime);
            break;
        }
        default:
            length = 0;
            TimePtr = NULL;
            break;
    }
    (void)memcpy(pData, TimePtr, length);
    (intu8*)pData += length;
    for(i = 0; i < pSegmEntryList->count; i++)
    {
        err = PushPmSegmentEntryElemData(pData, pSegmEntry, pObsScanList);
        if(err != ERROR_SUCCESS)
            return err;
        (intu8*)pSegmEntry += pSegmEntry->attr_val_map.length + 
            SEG_ENTRY_ELEM_HEADER_SIZE + ATTR_HEADER_SIZE;
    }
    return err;
    
}

/**************************************************************************//*!
 *
 * @name  UpdatePmSegmEntry
 *
 * @brief This function Updates PM Segment Entry Data based on PM Store Handle
 *        PM Segment Instance Number and Observation Scan List
 *
 * @param Handle		:	Handle to PM Store
 * @param InstNum		:	PM Segment Instance Number
 * @param pObsScanList	:	Pointer to Observation Scan List
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *			ERROR_INVALID_DATA		: if Observation scan list does not contain
 *									  Attribute ID
 *          ERROR_INVALID_PARAM		: If Handle is not PM Store
 *******************************************************************************
 * This function Updates PM Segment Entry Data based on PM Store Handle 
 * PM Segment Instance Number and Observation Scan List
 ******************************************************************************/
ERR_CODE UpdatePmSegmentEntry(
	HANDLE Handle,					  /* [IN] Handle to PM Store */
	InstNumber InstNum,				  /* [IN] PM Segment Instance Number */
    ObservationScanList *pObsScanList /* [IN] Pointer to Observation Scan List*/
)
{
    PPMSTORE pPmStore;
    PPMSEGMENT pPmSegm;
    intu16 objType;
    intu32 size;
    void* pData;
    ERR_CODE err;
    
    pPmStore = (PPMSTORE)GetObjectByHandle(Handle, &objType);    
    if(objType != MDC_MOC_VMO_PMSTORE)
        return ERROR_INVALID_PARAM;
    pPmSegm = GetPmSegmentById(pPmStore, InstNum);
    if(pPmSegm == NULL)
        return ERROR_INVALID_PARAM;
    /* Check for OS Enabled State */
    if(pPmSegm->MdcAttrOpStat != OS_ENABLED)
        return ERROR_INVALID_REQUEST;
    
    size = GetPMSegmentSize(pPmSegm->pMdcAttrPmSegMap);
    
    pData = mempool_malloc(size);
    if(pData == NULL)
    {
        return ERROR_INSUFFICIENT_MEMORY;
    }
        
    err = UpdatePmSegmEntryData(pPmSegm->pMdcAttrPmSegMap, pData, pObsScanList);
    if(ERROR_SUCCESS != err)
    {
        mempool_free(pData);
        return err;
    }
    /* Call external interface to update data on persistant storage */
    err = WritePMSegment(Handle, InstNum, pData, size);
    mempool_free(pData);
    if(err != ERROR_SUCCESS)
    {
        return err;
    }
    if(pPmSegm->OptionalAttributeFlag & OPT_PMSEG_USAGE_CNT)
    {
        if(pPmSegm->OptionalAttributeFlag & OPT_PMSEG_ABS_TIME)
        {
            if(0 == pPmSegm->MdcAttrsegUsageCnt)
            {
                /* Update Absolute Start Time */
                GetAbsoluteTime(&pPmSegm->MdcAttrTimeStartSeg);
            }
            else
            {
                /* Update Absolute End Time */
                GetAbsoluteTime(&pPmSegm->MdcAttrTimeEndSeg);
            }
        }
        pPmSegm->MdcAttrsegUsageCnt++;
    }
    if(pPmStore->OptionalAttributeFlag & OPT_PMSTORE_USAGE_CNT)
    {
        pPmStore->MdcAttrMetricStoreUsageCnt++; 
    }
    return ERROR_SUCCESS;
}

/**************************************************************************//*!
 *
 * @name  PushSegmDataEventDescr
 *
 * @brief This function pushes PM Segment Data Event Descriptor to Buffer Stack
 *
 * @param pBuffStack			:	Pointer to Buffer Stack
 * @param pSegmDataEventDescr	:	PM Segment Data Event Descriptor
 * @param pAttrLen				:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes PM Segment Data Event Descriptor to Buffer Stack
 ******************************************************************************/
ERR_CODE PushSegmDataEventDescr(
	PTR_BUFFSTACK pBuffStack,				 /* [IN] Pointer to Buffer Stack */
    SegmDataEventDescr* pSegmDataEventDescr, /* [IN] PM Segment Data Event 
													 Descriptor*/
	intu16* pAttrLen						 /* [OUT] Pointer to size of data 
													 pushed */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 AttrLen;
    *pAttrLen = 0;
    
    do
    {
        if(ERROR_SUCCESS != (err = PushSegmEvtStatus(pBuffStack, 
            pSegmDataEventDescr->segm_evt_status, 
            &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = Pushintu32(pBuffStack, 
            pSegmDataEventDescr->segm_evt_entry_count, 
            &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = Pushintu32(pBuffStack, 
            pSegmDataEventDescr->segm_evt_entry_index, 
            &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = PushInstNumber(pBuffStack, 
            pSegmDataEventDescr->segm_instance, 
            &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
    }while(0);
    
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  PushSegmentData
 *
 * @brief This function pushes PM Segment Data Event to Buffer Stack
 *
 * @param pBuffStack			:	Pointer to Buffer Stack
 * @param pSegmDataEvent		:	Pointer to PM Segment Data Event
 * @param PmStoreHandle			:	PM Store Handle
 * @param pAttrLen				:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes PM Segment Data Event to Buffer Stack
 ******************************************************************************/
ERR_CODE PushSegmentData(
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to Buffer Stack */
    SegmentDataEvent* pSegmDataEvent,	/* [IN] Pointer to PM Segment Data 
												Event */ 
	HANDLE PmStoreHandle,				/* [IN] PM Store Handle */
	intu16* pAttrLen					/* [OUT] Pointer to size of data pushed*/
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 AttrLen;
    *pAttrLen = 0;
    do
    {
        if(ERROR_SUCCESS != (err = Pushoctet_string(pBuffStack, 
            &pSegmDataEvent->segm_data_event_entries, 
            &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = PushSegmDataEventDescr(pBuffStack, 
            &pSegmDataEvent->segm_data_event_descr, 
            &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = PushConfEventRpt(pBuffStack, 
            PmStoreHandle, 0xFFFFFFFF, MDC_NOTI_SEGMENT_DATA,
            &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
    }while(0);
    
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
    }
    return err;
}
#define TO_CER      3000
/**************************************************************************//*!
 *
 * @name  SendSegmentData
 *
 * @brief This function sends PM Segment Data Event to Manager
 *
 * @param pSegmDataEvent		:	Pointer to PM Segment Data Event
 * @param PmStoreHandle			:	PM Store Handle
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS	: if Successful
 *			Others			: if Unsuccessful
 *******************************************************************************
 * This function sends PM Segment Data Event to Manager
 ******************************************************************************/
ERR_CODE SendSegmentData(
	SegmentDataEvent* pSegmDataEvent, /* [IN] Pointer to PM Segment Data 
											  Event */           
	HANDLE PmStoreHandle			  /* [IN] PM Store Handle */		         
)									           
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 size;
    PTR_BUFFSTACK pBuffStack;
    intu16 AttrLen;
    intu16 AttrLength = 0;
    PPMSTORE pPmStore;
    PPMSEGMENT pPmSegment;
    size = (intu16)(pSegmDataEvent->segm_data_event_entries.length + 
        OCTET_STRING_HEADER_SIZE + sizeof(SegmDataEventDescr) + 
        GetEvntRptArgSmpHeaderSize());
    pBuffStack = StackInit(size);
    if(pBuffStack == NULL)
    {
        return ERROR_INSUFFICIENT_MEMORY;
    }
    pPmStore = GetPmStoreHandle(PmStoreHandle);
    pPmSegment = GetPmSegmentById(pPmStore, 
        ieee_ntohs(pSegmDataEvent->segm_data_event_descr.segm_instance));

    if(pPmSegment->OptionalAttributeFlag & OPT_PMSEG_CONFIRM_TIMEOUT)
    {
        pBuffStack->Timeout = (intu32)(pPmSegment->MdcAttrConfirmTimeout >> 3);
    }
    else
    {
        pBuffStack->Timeout = (intu32)(TO_CER);
    }
    do
    {
        if(ERROR_SUCCESS != (err = PushSegmentData(pBuffStack, pSegmDataEvent, 
            PmStoreHandle, &AttrLen)))
        {
            break;
        }
        err = ObjectSendEventReport(TRUE, pBuffStack);    
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)StackDeInit(pBuffStack);
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  SetPmSegmentOpStat
 *
 * @brief This function sets PM Segment Operational Stat
 *
 * @param Handle		:	PM Store Handle
 * @param InstNum		:	PM Segment Instance Number
 * @param OpStat		:	Operational Stat Value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS		: if Successful
 *			ERROR_INVALID_PARAM	: if Unsuccessful
 *******************************************************************************
 * This function sets PM Segment Operational Stat
 ******************************************************************************/
ERR_CODE SetPmSegmentOpStat(
	HANDLE Handle,				/* [IN] PM Store Handle */
	InstNumber InstNum, 		/* [IN] PM Segment Instance Number */
	OperationalState OpStat		/* [IN] Operational Stat Value */   
)
{
    PPMSTORE pPmStore;
    PPMSEGMENT pPmSegm;
    intu16 objType;
    pPmStore = (PPMSTORE)GetObjectByHandle(Handle, &objType);    
    if(objType != MDC_MOC_VMO_PMSTORE)
        return ERROR_INVALID_PARAM;
    pPmSegm = GetPmSegmentById(pPmStore, InstNum);
    if(pPmSegm == NULL)
        return ERROR_INVALID_PARAM;
    
    if(OpStat > OS_NOT_AVAILABLE)
        return ERROR_INVALID_PARAM;
    pPmSegm->MdcAttrOpStat = OpStat;
    return ERROR_SUCCESS;
}

/**************************************************************************//*!
 *
 * @name  TrigPmSegmentDataTrf
 *
 * @brief This function Triggers Data Tranfer for PM Segment requested by 
 *        Manager
 *
 * @param pXferReq		:	Pointer to TrigSegmDataXferReq
 * @param pBuffStack	:	Pointer to Buffer Stack
 * @param pPmStore		:	Ponter to PM Store Object
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS		: if Successful
 *			ERROR_INVALID_PARAM	: if Unsuccessful
 *******************************************************************************
 * This function Triggers Data Tranfer for PM Segment requested by Manager
 ******************************************************************************/
ERR_CODE TrigPmSegmentDataTrf(
	TrigSegmDataXferReq *pXferReq,	/* [IN] Pointer to TrigSegmDataXferReq */
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */
	PPMSTORE pPmStore				/* [IN] Ponter to PM Store Object */
)
{
    ERR_CODE err;
    PPMSEGMENT pPmSegm;
    TRIGSEGMDATAXFRRSP Rsp;
    intu16 AttrLen, AttrLength;
    pPmSegm = GetPmSegmentById(pPmStore, ieee_ntohs(pXferReq->seg_inst_no));
    
    if((pPmSegm != NULL) && (pPmSegm->MdcAttrOpStat == OS_ENABLED))
        return ERROR_OBJECT_BUSY;
    
    Rsp.Handle = pPmStore->MdcAttrIdHandle;
    Rsp.TrigSegmDataRsp.seg_inst_no = ieee_ntohs(pXferReq->seg_inst_no);

    if(pPmSegm != NULL)
    {        
        Ieee11073GetAppCallback()(IEEE11073_TRIG_PMSEGMENT, (void*) &Rsp);
    }
    else
    {
        Rsp.TrigSegmDataRsp.trig_segm_xfer_rsp = TSXR_FAIL_NO_SUCH_SEGMENT;
    }
        
    do
    {
        if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
            Rsp.TrigSegmDataRsp.seg_inst_no, 
			Rsp.TrigSegmDataRsp.trig_segm_xfer_rsp, &AttrLen)))
        {
            break;
        }
        AttrLength = AttrLen;
        /* push any header */
        if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, AttrLength, 
			&AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;
        
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, AttrLength, NULL, NULL);
    }
    return err;    
}

/**************************************************************************//*!
 *
 * @name  GetPmStoreObjectMethodSize
 *
 * @brief This function returns size of PM Store Object Method Response Size 
 *
 * @param ActionType	:	Action Type
 * @param pReqBuffStack	:	Pointer to Buffer Stack
 * @param pPmStore		:	Ponter to PM Store Object
 *
 * @return Size of PM Store Object Method Response Size
 *******************************************************************************
 * This function returns size of PM Store Object Method Response Size 
 ******************************************************************************/
intu16 GetPmStoreObjectMethodSize(
	OID_Type ActionType,			/* [IN] Action Type */
	PTR_BUFFSTACK pReqBuffStack, 	/* [IN] Pointer to Buffer Stack */       
	PPMSTORE pPmStore				/* [IN] Ponter to PM Store Object */     
)
{
    intu16 size;
    size = GetActionResultSimpleHeaderSize();
	switch(ActionType)
	{
	    case PMSTR_OBJ_EVNT_MDC_ACT_SEG_CLR:
	        break;
	    case PMSTR_OBJ_EVNT_MDC_ACT_SEG_GET_INFO:
	    {
	        Any *pAny = (Any*)GetStackTop(pReqBuffStack);
	        SegmSelection *pSegmSel = (SegmSelection *)&pAny->value[0];
            size += GetPmSegSelInfoSize(pPmStore, pSegmSel);
	        break;
	    }
	    case PMSTR_OBJ_EVNT_MDC_ACT_SEG_TRIG_XFER:
	        size += sizeof(TrigSegmDataXferRsp);
	        break;
	    default:
	        size = 0;
	        break;
	}
	return size;
}

/**************************************************************************//*!
 *
 * @name  PMStoreObjectMethods
 *
 * @brief This function handles PM Store Object Methods
 *
 * @param ActionType	:	Action Type
 * @param pReqBuffStack	:	Pointer to Request Buffer Stack
 * @param pRspBuffStack	:	Pointer to Response Buffer Stack
 * @param pPmStore		:	Ponter to PM Store Object
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS		: If Successful
 *			ERROR_INVALID_REQUEST	: If Invalid Request
 *			Others				: If Unsuccessful
 *******************************************************************************
 * This function handles PM Store Object Methods
 ******************************************************************************/
ERR_CODE PMStoreObjectMethods(
	OID_Type ActionType,			/* [IN] Action Type */               
	PTR_BUFFSTACK pReqBuffStack, 	/* [IN] Pointer to Request Buffer Stack */   
	PTR_BUFFSTACK pRspBuffStack, 	/* [IN] Pointer to Response Buffer Stack */  
	PMSTORE* pPmStore				/* [IN] Ponter to PM Store Object */
)
{
    ERR_CODE err = ERROR_SUCCESS;
	Any *pAny;
	(void)PopStack(pReqBuffStack, (intu16)GetDataSize(pReqBuffStack), &pAny, NULL);
	pAny->length = ieee_ntohs(pAny->length);
	switch(ActionType)
	{
	    case PMSTR_OBJ_EVNT_MDC_ACT_SEG_CLR:
	        err = ClearPmSegment((SegmSelection*)&pAny->value[0], pRspBuffStack, 
				pPmStore);
	        break;
	    case PMSTR_OBJ_EVNT_MDC_ACT_SEG_GET_INFO:
	        err = GetPmSegmentInfo((SegmSelection*)&pAny->value[0], 
				pRspBuffStack, pPmStore);
	        break;
	    case PMSTR_OBJ_EVNT_MDC_ACT_SEG_TRIG_XFER:
	        err = TrigPmSegmentDataTrf((TrigSegmDataXferReq*)&pAny->value[0], 
				pRspBuffStack, pPmStore);
	        break;
	    default:
	        err = ERROR_INVALID_REQUEST;
	        break;
	}
    return err;
}

/**************************************************************************//*!
 *
 * @name  GetMdsObjectMethodSize
 *
 * @brief This function returns size of MDS Object Method Response Size 
 *
 * @param ActionType	:	Type of the MDS Action
 *
 * @return Size of MDS Object Method Response Size 
 *******************************************************************************
 * This function returns size of MDS Object Method Response Size 
 ******************************************************************************/
intu16 GetMdsObjectMethodSize(
	OID_Type ActionType     /* [IN] Type of the MDS Action */ 
)
{
    intu16 size = 0;
    switch(ActionType)
    {
        case MDC_ACT_DATA_REQUEST:
            break;
        case MDC_ACT_SET_TIME:
            size = sizeof(HANDLE) + sizeof(OID_Type) + ANY_HEADER_SIZE;
            break;
        default:
            break;
    }
    if(0 != size)
    {
        size += GetDataApduHeaderSize();
    }
    return size;
}

/**************************************************************************//*!
 *
 * @name  MDSObjectMethods
 *
 * @brief This function handles MDS Object Methods
 *
 * @param ActionType	:	Action Type
 * @param pReqBuffStack	:	Pointer to Request Buffer Stack
 * @param pRspBuffStack	:	Pointer to Response Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS		: If Successful
 *			ERROR_INVALID_REQUEST	: If Invalid Request
 *			Others				: If Unsuccessful
 *******************************************************************************
 * This function handles MDS Object Methods
 ******************************************************************************/
ERR_CODE MDSObjectMethods(
	OID_Type ActionType,			/* [IN] Action Type */                     
	PTR_BUFFSTACK pReqBuffStack, 	/* [IN] Pointer to Request Buffer Stack */ 
	PTR_BUFFSTACK pRspBuffStack		/* [IN] Pointer to Response Buffer Stack */
)
{
    ERR_CODE err = ERROR_SUCCESS;
	Any *pAny;
	(void)PopStack(pReqBuffStack, (intu16)GetDataSize(pReqBuffStack), &pAny, NULL);
	pAny->length = ieee_ntohs(pAny->length);
    switch(ActionType)
    {
        case MDC_ACT_DATA_REQUEST:
            err = MDSDataRequest((DataRequest *)&pAny->value, pRspBuffStack);
            break;
        case MDC_ACT_SET_TIME:
        {
            intu16 AttrLen;
            Any Rsp = 
            {
                0
            };
            err = MDSSetTime((SetTimeInvoke *) &pAny->value);
            
            if((ERROR_SUCCESS == err) && (NULL != pRspBuffStack))
            {
                err = PushAny(pRspBuffStack, &Rsp, &AttrLen);
            }
            
            break;
        }
        default:
            err = ERROR_INVALID_REQUEST;
            break;
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  MDSDataRequest
 *
 * @brief This function handles MDS Data Request Method
 *
 * @param pDataRequest	:	Pointer to Data Request
 * @param pRspBuffStack	:	Pointer to Response Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_UNSUPPORTED		: Always
 *******************************************************************************
 * This function handles MDS Data Request Method. This method is not supported
 * by Continua Design Guidlines for Agent Implementation
 ******************************************************************************/
ERR_CODE MDSDataRequest(
	DataRequest *pDataRequest,	/* [IN] Pointer to Data Request */
	PTR_BUFFSTACK pRspBuffStack /* [IN] Pointer to Response Buffer Stack */
)
{
    UNUSED(pDataRequest)
    UNUSED(pRspBuffStack)
	return ERROR_UNSUPPORTED;
}

/**************************************************************************//*!
 *
 * @name  MDSSetTime
 *
 * @brief This function handles MDS Set Time Method
 *
 * @param pSetTimeInvoke	:	Pointer to SetTimeInvoke
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS		: Always
 *          ERROR_UNSUPPORTED     : if Unsuccessful
 *******************************************************************************
 * This function handles MDS Set Time Method
 ******************************************************************************/
ERR_CODE MDSSetTime(
	SetTimeInvoke *pSetTimeInvoke	/* [IN] Pointer to SetTimeInvoke */
)
{
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    ERR_CODE err = ERROR_UNSUPPORTED;
    
    if((pMds != NULL) && (pMds->pMdcAttrTimeAbs != NULL) && (pMds->pMdcAttrMdsTimeInfo != NULL))
    {      
      intu16 Mask = (MDS_TIME_CAPAB_SET_CLOCK|MDS_TIME_CAPAB_REAL_TIME_CLOCK);
      if((pMds->pMdcAttrMdsTimeInfo->mds_time_cap_state & Mask) == Mask)
      {
          /* Invoke RTC to Set Time with the defined Accuracy */
          (void)RTCSetTime(pSetTimeInvoke->date_time, pSetTimeInvoke->accuracy);
          (void)memcpy(pMds->pMdcAttrTimeAbs, &pSetTimeInvoke->date_time, sizeof(AbsoluteTime));
          (void)RTCGetTime(pMds->pMdcAttrTimeAbs);
          err = ERROR_SUCCESS;
      }
    }
    
    return err;
}

/**************************************************************************//*!
 *
 * @name  GetMdsAllAttributeSize
 *
 * @brief This function returns size of All MDS Attrbiutes
 *
 * @param pMds	:	Pointer to MDS Object
 *
 * @return Size of All MDS Attrbiutes
 *******************************************************************************
 * This function returns size of All MDS Attrbiutes
 ******************************************************************************/
intu16 GetMdsAllAttributeSize(
	PMDS pMds		/* [IN] Pointer to MDS Object */
)
{
    intu16 size = 0;
  
    size += GetMdsAttributeSize(pMds, MDC_ATTR_CONFIRM_TIMEOUT);
    size += GetMdsAttributeSize(pMds, MDC_ATTR_REG_CERT_DATA_LIST);
    size += GetMdsAttributeSize(pMds, MDC_ATTR_TIME_BATT_REMAIN);
    size += GetMdsAttributeSize(pMds, MDC_ATTR_VAL_BATT_CHARGE);
    size += GetMdsAttributeSize(pMds, MDC_ATTR_POWER_STAT);
    size += GetMdsAttributeSize(pMds, MDC_ATTR_MDS_TIME_INFO);
    
	  size += GetMdsAttributeSize(pMds, MDC_ATTR_TIME_REL);        
	  size += GetMdsAttributeSize(pMds, MDC_ATTR_TIME_REL_HI_RES);
   
    size += GetMdsAttributeSize(pMds, MDC_ATTR_TIME_ABS);


    size += GetMdsAttributeSize(pMds, MDC_ATTR_ID_PROD_SPECN);
  
    size += GetMdsAttributeSize(pMds, MDC_ATTR_ATTRIBUTE_VAL_MAP);

    size += GetMdsAttributeSize(pMds, MDC_ATTR_TIME_ABS_ADJUST);
    size += GetMdsAttributeSize(pMds, MDC_ATTR_DEV_CONFIG_ID);
    
    size += GetMdsAttributeSize(pMds, MDC_ATTR_SYS_ID);
    size += GetMdsAttributeSize(pMds, MDC_ATTR_ID_MODEL);
    
    size += GetMdsAttributeSize(pMds, MDC_ATTR_SYS_TYPE);

    size += GetMdsAttributeSize(pMds, MDC_ATTR_SYS_TYPE_SPEC_LIST);
    
    size += GetMdsAttributeSize(pMds, MDC_ATTR_ID_HANDLE);
    
    return size;    
}

/**************************************************************************//*!
 *
 * @name  GetMdsAllAttributeSize
 *
 * @brief This function returns size of MDS Attrbiute
 *
 * @param pMds			:	Pointer to MDS Object
 * @param AttributeId	:	Attribute ID 
 *
 * @return Size of All MDS Attrbiutes
 *******************************************************************************
 * This function returns size of All MDS Attrbiutes
 ******************************************************************************/
static intu16 GetMdsAttributeSize(
	PMDS pMds,				/* [IN] Pointer to MDS Object */
	OID_Type AttributeId	/* [IN] Attribute ID */
)
{
    intu16 size = 0;
    switch(AttributeId)
    {
        case MDC_ATTR_CONFIRM_TIMEOUT:
            if(pMds->pMdcAttrConfirmTimeout != NULL)
            {              
             size = sizeof(RelativeTime);
            }
            break;
        case MDC_ATTR_REG_CERT_DATA_LIST:
            if(pMds->pMdcAttrRegCertDataList != NULL)
            {              
             size = (intu16)(ieee_htons(pMds->pMdcAttrRegCertDataList->length) + ATTR_HEADER_SIZE);
            }
            break;
        case MDC_ATTR_TIME_BATT_REMAIN:
            if(pMds->pMdcAttrTimeBattRemain)
            {              
             size = sizeof(BatMeasure);    
            }
            break;
        case MDC_ATTR_POWER_STAT:                
            if(pMds->pMdcAttrPowerStatus != NULL)
            {
             size = sizeof(intu16);            
            }
            break;
        case MDC_ATTR_VAL_BATT_CHARGE: 
            if(pMds->pMdcAttrValBattCharge != NULL)
            {
             size = sizeof(intu16);
            }
        case MDC_ATTR_ID_HANDLE:
        case MDC_ATTR_DEV_CONFIG_ID:
            size = sizeof(intu16);
            break;
        case MDC_ATTR_MDS_TIME_INFO:
            if(pMds->pMdcAttrMdsTimeInfo != NULL)
            {            
             size = sizeof(MdsTimeInfo);
            }
            break; 
        case MDC_ATTR_TIME_ABS_ADJUST:
            if(pMds->pMdcAttrTimeAbsAdjust != NULL)
            {              
             size = sizeof(AbsoluteTimeAdjust);
            }
            break;
        case MDC_ATTR_TIME_REL_HI_RES:
        	  if(pMds->pMdcAttrMdsTimeInfo->mds_time_cap_state & MDS_TIME_CAPAB_HIGH_RES_RELATIVE_TIME)
        	  {        	    
        	   size = sizeof(HighResRelativeTime);
        	  }
            break;
        case MDC_ATTR_TIME_REL:
            if(pMds->pMdcAttrMdsTimeInfo->mds_time_cap_state & MDS_TIME_CAPAB_RELATIVE_TIME) 
            {              
	           size = sizeof(RelativeTime);
            }
            break;
        case MDC_ATTR_TIME_ABS:
	          if(pMds->pMdcAttrTimeAbs != NULL)
	          {	            
	           size = sizeof(AbsoluteTime);
	          }
            break;
        case MDC_ATTR_ID_PROD_SPECN:
            if(pMds->pMdcAttrIdProdSpecn != NULL)
            {              
             size = (intu16)(ieee_htons(pMds->pMdcAttrIdProdSpecn->length) + PRODUCTION_SPEC_HEADER_SIZE);
            }
            break;
        case MDC_ATTR_ATTRIBUTE_VAL_MAP:
            if(pMds->pMdcAttrValMap != NULL)
            {
             size = (intu16)(ATTR_VAL_MAP_HEADER_SIZE + ieee_htons(pMds->pMdcAttrValMap->length));
            }
            break;
        case MDC_ATTR_SYS_ID:            
            if(pMds->pMdcAttrSysId != NULL)
            {              
             size = (intu16)(OCTET_STRING_HEADER_SIZE + ieee_htons(pMds->pMdcAttrSysId->length));
            }
            break;
        case MDC_ATTR_ID_MODEL:
            if(pMds->pMdcAttrSysModel != NULL)
            {
                octet_string* temp = &pMds->pMdcAttrSysModel->manufacturer;

                (intu8*)temp += (OCTET_STRING_HEADER_SIZE + 
                		ieee_htons(pMds->pMdcAttrSysModel->manufacturer.length));
                size = (intu16)((OCTET_STRING_HEADER_SIZE << 1) + 
                		ieee_htons(pMds->pMdcAttrSysModel->manufacturer.length) + 
                		ieee_htons(temp->length));
            }
            break;
        case MDC_ATTR_SYS_TYPE:
            if(pMds->pMdcAttrSysType != NULL)
            {              
             size = sizeof(TYPE);
            }
            break;
        case MDC_ATTR_SYS_TYPE_SPEC_LIST:
            if(pMds->pMdcAttrSysTypeSpecList != NULL)
            {              
             size = (intu16)(ieee_htons(pMds->pMdcAttrSysTypeSpecList->length) + TYPEVERLIST_HEADER_SIZE);
            }
            break;
        default:;
            break;
    }
    if(size != 0)
    {        
        size += ATTR_HEADER_SIZE;
    }
    return size;
}

/**************************************************************************//*!
 *
 * @name  PushMdsAttributeList
 *
 * @brief This function pushes MDS Attributes Specified in Attribute ID List
 *		  to Buffer Stack
 * @param pBuffStack			:	Pointer to Buffer Stack
 * @param pMds					:	Pointer to MDS Object
 * @param pAttributeIdList		:	Pointer to Attribute ID List
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes MDS Attributes Specified in Attribute ID List 
 * to Buffer Stack
 ******************************************************************************/
ERR_CODE PushMdsAttributeList(
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to Buffer Stack */
	PMDS pMds, 							/* [IN] Pointer to MDS Object */      
	AttributeIdList *pAttributeIdList	/* [IN] Pointer to Attribute ID List */
)
{
    ERR_CODE err = ERROR_SUCCESS;   
    intu16 AttrLen;
    intu16 AttrLength = 0;
    intu16 AttrCount = 0;
    intu16 i;
    OID_Type *pOidType;
    pOidType = &pAttributeIdList->value[0];
    do
    {
        
        for(i = 0; i< ieee_ntohs(pAttributeIdList->count); i++)
        {
            if(ERROR_SUCCESS != (err = PushMdsAttribute(pBuffStack, pMds, 
				ieee_ntohs(pOidType[i]), &AttrLen)))
            {
                break;
            }
            if(AttrLen != 0)   /* No error is returned if the attribute is not supported */
            {              
             AttrLength += AttrLen;
             AttrCount++;
            }                        
        }
        if(ERROR_SUCCESS != err)
        {
            break;
        }
        if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, AttrCount, 
			AttrLength, &AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;
        if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, 
			pMds->MdcAttrIdHandle, &AttrLen)))
        {
        	break;;
        }
        AttrLength += AttrLen;
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, AttrLength, NULL, NULL);
    }
    return err;
}
    
/**************************************************************************//*!
 *
 * @name  PushMdsAttribute
 *
 * @brief This function pushes MDS Attribute Specified by OID Type 
 *		  to Buffer Stack
 * @param pBuffStack	:	Pointer to Buffer Stack
 * @param pMds			:	Pointer to MDS Object
 * @param OidType		:	OID Type
 * @param pAttrLen		:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes MDS Attributes Specified by OID Type to Buffer Stack
 ******************************************************************************/
static ERR_CODE PushMdsAttribute(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */      
	PMDS pMds, 					/* [IN] Pointer to MDS Object */        
	OID_Type OidType, 			/* [IN] OID Type */ 
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;    
    intu16 AttrLen = 0;
    
    *pAttrLen = 0;

    switch(OidType)
    {
        case MDC_ATTR_CONFIRM_TIMEOUT:
        	/* Push MDS Relative Confirm Timeout */
        	{
        	  if(pMds->pMdcAttrConfirmTimeout != NULL)
        		{        	  
        		  if(ERROR_SUCCESS != (err = PushRelativeTime(pBuffStack, 
        		  *(pMds->pMdcAttrConfirmTimeout), &AttrLen)))
        		  {
        		    break;
        		  }
        		  *pAttrLen += AttrLen;
    			    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
    			    MDC_ATTR_CONFIRM_TIMEOUT, AttrLen, &AttrLen)))
    			    {
    				    break;
    			    }  
        		  *pAttrLen += AttrLen;
        	  }
        	}
          break;
        case MDC_ATTR_REG_CERT_DATA_LIST:
        	/* Push Certification Authority Structure and Data*/
        	if(pMds->pMdcAttrRegCertDataList != NULL)
        	{
        		if(ERROR_SUCCESS != (err = PushRegCertDataList(pBuffStack, 
					  pMds->pMdcAttrRegCertDataList, &AttrLen)))
        		{
        		    break;                 
        		}
        		*pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_REG_CERT_DATA_LIST, AttrLen, &AttrLen)))
    			  {
    				 break;
      			}
        		*pAttrLen += AttrLen;
        	}
        	break;       
        case MDC_ATTR_TIME_BATT_REMAIN:
        	/* Push Battery Reamining */
        	if(pMds->pMdcAttrTimeBattRemain != NULL)
        	{
        		if(ERROR_SUCCESS != (err = PushBatMeasure(pBuffStack, pMds->pMdcAttrTimeBattRemain, &AttrLen)))
        		{
        			break;
        		}
            *pAttrLen += AttrLen;
        		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, MDC_ATTR_TIME_BATT_REMAIN, AttrLen, &AttrLen)))
        		{
        			break;
        		}
            *pAttrLen += AttrLen;
        	}
          break;
        case MDC_ATTR_DEV_CONFIG_ID:
        	/* Push Configuration ID */
        	{
        		if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, 
					pMds->MdcAttrDevConfigId, &AttrLen)))
        		{
        			break;
        		}
                *pAttrLen += AttrLen;
        		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
					MDC_ATTR_DEV_CONFIG_ID, AttrLen, &AttrLen)))
        		{
        			break;
        		}
                *pAttrLen += AttrLen;
        	}
            break;
            
        case MDC_ATTR_VAL_BATT_CHARGE:
        	/* Push Battery Charge Status */        	
        	if(pMds->pMdcAttrValBattCharge != NULL)
        	{
        		if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, *(pMds->pMdcAttrValBattCharge), &AttrLen)))
        		{
        			break;
        		}	
            *pAttrLen += AttrLen;
        		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, MDC_ATTR_VAL_BATT_CHARGE, AttrLen, &AttrLen)))
        		{
        			break;
        		}
            *pAttrLen += AttrLen;
        	}
          break;

        case MDC_ATTR_POWER_STAT:
        	/* Push Power Status */
        	if(pMds->pMdcAttrPowerStatus != NULL)
        	{
        		if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, *(pMds->pMdcAttrPowerStatus), &AttrLen)))
        		{
        			break;
        		}	
            *pAttrLen += AttrLen;
    			  
    			  if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, MDC_ATTR_POWER_STAT, AttrLen, &AttrLen)))
    			  {
    				  break;
    			  }
            *pAttrLen += AttrLen;
        	}
          break;
        case MDC_ATTR_MDS_TIME_INFO:
        	if(pMds->pMdcAttrMdsTimeInfo != NULL)
        	{
        		if(ERROR_SUCCESS != (err = PushMdsTimeInfo(pBuffStack, pMds->pMdcAttrMdsTimeInfo, &AttrLen)))
        		{
        			break;
        		}	
                *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_MDS_TIME_INFO, AttrLen, &AttrLen)))
    			  {
    				  break;
    			  }
            *pAttrLen += AttrLen;
        	}
          break; 
        case MDC_ATTR_TIME_ABS_ADJUST:
        	/* Push Date Time Adjust */
        	if(pMds->pMdcAttrTimeAbsAdjust != NULL)
        	{       	
        		AttrLen = sizeof(AbsoluteTimeAdjust);
        		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, pMds->pMdcAttrTimeAbsAdjust, AttrLen)))
        		{
        			break;
        		}	
            *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_TIME_ABS_ADJUST, AttrLen, &AttrLen)))
    			  {
    				  break;
    			  }
            *pAttrLen += AttrLen;
        	}
          break;
        case MDC_ATTR_TIME_REL_HI_RES:
        	/* Push Hi-Resolution Time */
        	if((pMds->pMdcAttrMdsTimeInfo != NULL) && (pMds->pMdcAttrMdsTimeInfo->mds_time_cap_state & MDS_TIME_CAPAB_HIGH_RES_RELATIVE_TIME))
        	{        	                  	          	
        		AttrLen = sizeof(HighResRelativeTime);
        		
        		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, 
					  &pMds->MdcAttrTimeRelHiRes, AttrLen)))
        		{
        			break;
        		}	
            
            *pAttrLen += AttrLen;
    			  
    			  if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_TIME_REL_HI_RES, AttrLen, &AttrLen)))
    			  {
    				  break;
    			  }
            *pAttrLen += AttrLen;
        	}
          break;
        case MDC_ATTR_TIME_REL:
        	/* Push MDS Relative Time */
        	if((pMds->pMdcAttrMdsTimeInfo != NULL) && (pMds->pMdcAttrMdsTimeInfo->mds_time_cap_state & MDS_TIME_CAPAB_RELATIVE_TIME))
        	{        	          	        	          	
        		if(ERROR_SUCCESS != (err = PushRelativeTime(pBuffStack, 
					  pMds->MdcAttrTimeRel, &AttrLen)))
        		{
        			break;
        		}
            
            *pAttrLen += AttrLen;
    			  
    			  if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_TIME_REL, AttrLen, &AttrLen)))
    			  {
    				  break;
    			  }
            *pAttrLen += AttrLen;
        	}
          break;
          
        case MDC_ATTR_TIME_ABS:
        	/* Push Absolute Time */
        	if(pMds->pMdcAttrTimeAbs != NULL)
        	{        	        	          	
        		AttrLen = sizeof(AbsoluteTime);
        		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, pMds->pMdcAttrTimeAbs, AttrLen)))
        		{
        			break;
        		}	
            *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, MDC_ATTR_TIME_ABS, AttrLen, &AttrLen)))
    			  {
    				 break;
    			  }
            *pAttrLen += AttrLen;
        	 }
           break;
        case MDC_ATTR_ID_PROD_SPECN:
        	/* Push Production Spec*/
        	if(pMds->pMdcAttrIdProdSpecn != NULL)
        	{
        		if(ERROR_SUCCESS != (err = PushProductionSpec(pBuffStack, pMds->pMdcAttrIdProdSpecn, &AttrLen)))
        		{
        			break;
        		}	
            
            *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_ID_PROD_SPECN, AttrLen, &AttrLen)))
    			  {
    				  break;
    			  }
            *pAttrLen += AttrLen;
        	}
          break;
          
        case MDC_ATTR_ATTRIBUTE_VAL_MAP:
        	/* Push AttrValMap */
        	if(pMds->pMdcAttrValMap != NULL)
        	{
        		if(ERROR_SUCCESS != (err = PushAttrValMap(pBuffStack, 
					  pMds->pMdcAttrValMap, &AttrLen)))
        		{
        			break;
        		}	
            
            *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack,
					  MDC_ATTR_ATTRIBUTE_VAL_MAP, AttrLen, &AttrLen)))
        		{
        			break;
        		}	
            *pAttrLen += AttrLen;
        	}
          break;

        case MDC_ATTR_SYS_ID:
        	/* Push System ID */
        	if(pMds->pMdcAttrSysId != NULL)
        	{
        		pMds->pMdcAttrSysId->length = ieee_htons(pMds->pMdcAttrSysId->length); 
        		if(ERROR_SUCCESS != (err = Pushoctet_string(pBuffStack, pMds->pMdcAttrSysId, &AttrLen)))
        		{
        			break;
        		}	
            *pAttrLen += AttrLen;
        		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, MDC_ATTR_SYS_ID, AttrLen, &AttrLen)))
        		{
        			break;
        		}	
            *pAttrLen += AttrLen;
        	}
          break;
        case MDC_ATTR_ID_MODEL:
        	/* Push System Model */
        	if(pMds->pMdcAttrSysModel != NULL)
        	{
        		if(ERROR_SUCCESS != (err = PushSystemModel(pBuffStack, 
					  pMds->pMdcAttrSysModel, &AttrLen)))
        		{
        			break;
        		}	
            
            *pAttrLen += AttrLen;
       			if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_ID_MODEL, AttrLen, &AttrLen)))
        		{
        			break;
        		}	
            *pAttrLen += AttrLen;
        	}
          break;
        case MDC_ATTR_SYS_TYPE:
        	/* Push System Type */
        	if(pMds->pMdcAttrSysType != NULL)
        	{
        		if(ERROR_SUCCESS != (err = PushTYPE(pBuffStack, 
					  pMds->pMdcAttrSysType, &AttrLen)))
        		{
        			break;
        		}	
            
            *pAttrLen += AttrLen;
        		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
  					MDC_ATTR_SYS_TYPE, AttrLen, &AttrLen)))
        		{
        			break;
        		}	
            *pAttrLen += AttrLen;            
        	}
          break;
          
        case MDC_ATTR_SYS_TYPE_SPEC_LIST:
        	if(pMds->pMdcAttrSysTypeSpecList != NULL)
        	{
        		if(ERROR_SUCCESS != (err = PushTypeVerList(pBuffStack, pMds->pMdcAttrSysTypeSpecList, &AttrLen)))
        		{
        			break;
        		}	            
            *pAttrLen += AttrLen;
    			  
    			  if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, MDC_ATTR_SYS_TYPE_SPEC_LIST, AttrLen, &AttrLen)))
        		{
        			break;
        		}	
            *pAttrLen += AttrLen;
        	}
            break;
        default:
            break;
    }    
    return err;   
}
    
/**************************************************************************//*!
 *
 * @name  PMStoreGetObjectAttribute
 *
 * @brief This function handles PM Store Get Object Attribute request 
 *
 * @param pPmStore		:	Pointer to PM Store Object
 * @param pReqBuffStack	:	Pointer to Request Buffer Stack
 * @param pRspBuffStack	:	Pointer to Response Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function handles PM Store Get Object Attribute request 
 ******************************************************************************/
ERR_CODE PMStoreGetObjectAttribute(
	PPMSTORE pPmStore,				/* [IN] Pointer to PM Store Object */
	PTR_BUFFSTACK pReqBuffStack, 	/* [IN] Pointer to Request Buffer Stack */
	PTR_BUFFSTACK pRspBuffStack		/* [IN] Pointer to Response Buffer Stack */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 AttrLen;
    AttributeIdList *pAttributeIdList = 
        (AttributeIdList *)GetStackTop(pReqBuffStack);
    do
    {
        /* Push Mds Attributes */
        if(ieee_ntohs(pAttributeIdList->count) == 0)
        {
            err = PushAllPMStoreAttribute(pRspBuffStack, pPmStore, &AttrLen);
        } 
        else
        {            
            err = PushPmStoreAttributeList(pRspBuffStack, pPmStore, 
				pAttributeIdList);
        }
    
        if(ERROR_SUCCESS != err)
        {
            break;
        }
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pRspBuffStack, (intu16)GetDataSize(pRspBuffStack), NULL, NULL);
    }
    return err;
}
    
/**************************************************************************//*!
 *
 * @name  MdsGetObjectAttributeSize
 *
 * @brief This function returns size of Response Buffer for MDS Get Object 
 *        Attrbiute Request
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function returns size of Response Buffer for MDS Get Object Attrbiute 
 * Request
 ******************************************************************************/
intu16 MdsGetObjectAttributeSize(
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
    AttributeIdList *pAttributeIdList = GetStackTop(pBuffStack);
    intu16 i;
    intu16 OutBuffSize = 0;
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    
    /* Compute Size */
    if(ieee_ntohs(pAttributeIdList->count) == 0)
    {
        OutBuffSize = GetMdsAllAttributeSize(pMds);
    } 
    else
    {
        OID_Type *pOidType;
        pOidType = &pAttributeIdList->value[0];
        for(i = 0; i< pAttributeIdList->count; i++)
        {
            OutBuffSize += GetMdsAttributeSize(pMds, ieee_ntohs(pOidType[i]));
        }
        OutBuffSize += GetMdsAttributeSize(pMds, MDC_ATTR_ID_HANDLE);
    }
    OutBuffSize += GetDataApduHeaderSize();
    return OutBuffSize;     
}
    
/**************************************************************************//*!
 *
 * @name  MDSGetObjectAttribute
 *
 * @brief This function handles MDS Get Object Attribute Request
 *
 * @param pReqBuffStack	:	Pointer to Request Buffer Stack
 * @param pRspBuffStack	:	Pointer to Response Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function handles MDS Get Object Attribute Request
 ******************************************************************************/
ERR_CODE MDSGetObjectAttribute(
	PTR_BUFFSTACK pReqBuffStack,	/* [IN] Pointer to Request Buffer Stack */ 
	PTR_BUFFSTACK pRspBuffStack		/* [IN] Pointer to Response Buffer Stack */
)
{
	/*
		called from the service layer whenever host 
		sends a request to get MDS attributes
		returns a pointer to the attribute list
	*/
    /* Send all MDS attributes */
    ERR_CODE err;
    AttributeIdList *pAttributeIdList = 
        (AttributeIdList *)GetStackTop(pReqBuffStack);
    PMDS pMds = GetMdsHandle(MDS_HANDLE);

    do
    {
        /* Push Mds Attributes */
        if(ieee_ntohs(pAttributeIdList->count) == 0)
        {
            err = PushAllMdsAttribute(pMds, pRspBuffStack);
        } 
        else
        {            
            err = PushMdsAttributeList(pRspBuffStack, pMds, pAttributeIdList);
        }
    
        if(ERROR_SUCCESS != err)
        {
            break;
        }
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pRspBuffStack, (intu16)GetDataSize(pRspBuffStack), NULL, NULL);
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  MDSSetObjectAttribute
 *
 * @brief This function handles MDS Set Object Attribute Request
 *
 * @return ERR_CODE
 *			ERROR_NOTSUPPORTED		: Always
 *******************************************************************************
 * This function handles MDS Get Object Attribute Request
 ******************************************************************************/
ERR_CODE MDSSetObjectAttribute(void)
{
    return ERROR_NOTSUPPORTED;
}
    
/**************************************************************************//*!
 *
 * @name  GetEpiCfgScannerSetArgumentSize
 *
 * @brief This function returns size of Response Buffer for Episoidic 
 *        Configuration Scanner Set Argument Request
 *
 * @param pReqBuffStack	:	Pointer to Buffer Stack
 *
 * @return Size of Response Buffer for Episoidic Configuration Scanner Set  
 *         Argument Request 
 *******************************************************************************
 * This function returns size of Response Buffer for Episoidic Configuration 
 * Scanner Set Argument Request
 ******************************************************************************/
intu16 GetEpiCfgScannerSetArgumentSize(
	PTR_BUFFSTACK pReqBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
    return GetCfgScannerSetArgumentSize(pReqBuffStack);
}

/**************************************************************************//*!
 *
 * @name  EpiCfgScannerSetArgument
 *
 * @brief This function handles Episoidic Configuration Scanner Set Argument 
 *        Request
 *
 * @param pReqBuffStack	:	Pointer to Request Buffer Stack
 * @param pEpiScanner	:	Pointer to Episoidic Scanner Object 
 * @param pReqBuffStack	:	Pointer to Response Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function handles Episoidic Configuration Scanner Set Argument Request
 ******************************************************************************/
ERR_CODE EpiCfgScannerSetArgument(
	PTR_BUFFSTACK pReqBuffStack, /* [IN] Pointer to Request Buffer Stack */ 
    PEPICFGSCANNER pEpiScanner,  /* [IN] Pointer to Episoidic Scanner Object */
	PTR_BUFFSTACK pRspBuffStack	 /* [IN] Pointer to Response Buffer Stack */
)
{
    /* There are no Settable Attribute in Episoidic Scanner Class*/
    ERR_CODE err; 
    intu16 AttrLength;
    err = CfgScannerSetArgument(pReqBuffStack, pEpiScanner->pCfgScanner, 
        pRspBuffStack, 0, 0, &AttrLength);

	if(ERROR_SUCCESS != err)
	{
	    (void)PopStack(pRspBuffStack, AttrLength, NULL, NULL);
	}
	else
	{
	    /* Enable Timer for Minimum Reporting Interval if enabled */
	    if((pEpiScanner->pCfgScanner->pScanner->MdcAttrOpStat == OS_ENABLED) &&
	    (pEpiScanner->OptionalAttributeFlag & OPT_EPISCAN_MIN_RPT_INT))
	    {
	        /* Enable Timer */
	        TIMER_OBJECT TimerObject;
	        TimerObject.msCount = (pEpiScanner->MdcAttrScanRepPdMin >> 3);
	        TimerObject.pfnTimerCallback = EpiCfgScannerTimerCallback;
	        TimerObject.arg = pEpiScanner;
	        pEpiScanner->TimerIndex = PHD_Add_Timer(&TimerObject);
	    }
        else
        {
            /* Remove Timer Object */
            (void)PHD_Remove_Timer(&pEpiScanner->TimerIndex);
        }
	}
	return err;
}
/******************************************************************************
 *
 *    @name        EpiCfgScannerTimerCallback
 *
 *    @brief       This function is called when Episoidic Minmum Reporting 
 *                 Interval Timer Expires
 *
 *    @param       arg : Argument passed by Timer ISR
 *
 *    @return      None
 *
 *****************************************************************************
 * This function is called when Episoidic Minmum Reporting Interval Timer 
 * Expires
 *****************************************************************************/
static void EpiCfgScannerTimerCallback(
	void* arg	/* [IN] Argument passed by Timer ISR */
)
{
    PEPICFGSCANNER pEpiScanner = (PEPICFGSCANNER) arg;
    pEpiScanner->TimerIndex = (intu8)INVALID_TIME_COUNT;
}

/**************************************************************************//*!
 *
 * @name  PeriCfgScannerSetArgument
 *
 * @brief This function handles Periodic Configuration Scanner Set Argument 
 *        Request
 *
 * @param pReqBuffStack	:	Pointer to Request Buffer Stack
 * @param pPeriScanner	:	Pointer to Periodic Scanner Object 
 * @param pReqBuffStack	:	Pointer to Response Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function handles Periodic Configuration Scanner Set Argument Request
 ******************************************************************************/
ERR_CODE PeriCfgScannerSetArgument(
	PTR_BUFFSTACK pReqBuffStack,  /* [IN] Pointer to Request Buffer Stack */    
    PPERICFGSCANNER pPeriScanner, /* [IN] Pointer to Periodic Scanner Object */
	PTR_BUFFSTACK pRspBuffStack	  /* [IN] Pointer to Response Buffer Stack */   
)
{         
    /* There are no Settable Attribute in Periodic Scanner Class*/
    ERR_CODE err; 
    intu16 AttrLength;
    err = CfgScannerSetArgument(pReqBuffStack, pPeriScanner->pCfgScanner, 
        pRspBuffStack, 0, 0, &AttrLength);

	if(ERROR_SUCCESS != err)
	{
	    (void)PopStack(pRspBuffStack, AttrLength, NULL, NULL);
	}
    /* Enable Timer for Reporting Interval */
    if(ieee_htons(pPeriScanner->pCfgScanner->pScanner->MdcAttrOpStat) == OS_ENABLED)
    {
        /* Enable Timer */
        TIMER_OBJECT TimerObject;
        TimerObject.msCount = (pPeriScanner->MdcAttrScanRepPd >> 3);
        TimerObject.pfnTimerCallback = PeriCfgScannerTimerCallback;
        TimerObject.arg = pPeriScanner;
        pPeriScanner->TimerIndex = PHD_Add_Timer(&TimerObject);
    }
    else
    {
        /* Remove Timer Object */
        (void)PHD_Remove_Timer(&pPeriScanner->TimerIndex);
    }
	return err;
}

/******************************************************************************
 *
 *    @name        PeriCfgScannerTimerCallback
 *
 *    @brief       This function is called when Periodic Reporting 
 *                 Interval Timer Expires
 *
 *    @param       arg : Argument passed by Timer ISR
 *
 *    @return      None
 *
 *****************************************************************************
 * This function is called when Periodic Reporting Interval Timer 
 * Expires
 *****************************************************************************/
static void PeriCfgScannerTimerCallback(
	void* arg	/* [IN] Argument passed by Timer ISR */
)
{
    PPERICFGSCANNER pPeriScanner = (PPERICFGSCANNER) arg;
    HANDLE handle = pPeriScanner->pCfgScanner->pScanner->MdcAttrIdHandle;
    
    pPeriScanner->TimerIndex = (intu8)INVALID_TIME_COUNT;
    Ieee11073GetAppCallback()(IEEE11073_PERIODIC_SCANNER_EVENT, 
        (void*)&handle);
    if(ieee_htons(pPeriScanner->pCfgScanner->pScanner->MdcAttrOpStat) == OS_ENABLED)
    {
        /* Enable Timer */
        TIMER_OBJECT TimerObject;
        TimerObject.msCount = (pPeriScanner->MdcAttrScanRepPd >> 3);
        TimerObject.pfnTimerCallback = PeriCfgScannerTimerCallback;
        TimerObject.arg = pPeriScanner;
        pPeriScanner->TimerIndex = PHD_Add_Timer(&TimerObject);
    }
}
    
/**************************************************************************//*!
 *
 * @name  GetPeriCfgScannerSetArgumentSize
 *
 * @brief This function returns size of Response Buffer for Periodic 
 *        Configuration Scanner Set Argument Request
 *
 * @param pReqBuffStack	:	Pointer to Buffer Stack
 *
 * @return Size of Response Buffer for Periodic Configuration Scanner Set  
 *         Argument Request 
 *******************************************************************************
 * This function returns size of Response Buffer for Periodic Configuration 
 * Scanner Set Argument Request
 ******************************************************************************/
intu16 GetPeriCfgScannerSetArgumentSize(
	PTR_BUFFSTACK pReqBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
    return GetCfgScannerSetArgumentSize(pReqBuffStack);
}

/**************************************************************************//*!
 *
 * @name  CfgScannerSetArgument
 *
 * @brief This function handles Configuration Scanner Set Argument Request
 *
 * @param pReqBuffStack	:	Pointer to Request Buffer Stack
 * @param pCfgScanner	:	Pointer to Configuration Scanner Object 
 * @param pReqBuffStack	:	Pointer to Response Buffer Stack
 * @param AttrCount		:	Attribute Count
 * @param AttrLength	:	Attribute Length
 * @param pAttrLen		:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function handles Configuration Scanner Set Argument Request
 ******************************************************************************/
static ERR_CODE CfgScannerSetArgument(
	PTR_BUFFSTACK pReqBuffStack,	/* [IN] Pointer to Request Buffer Stack */   
    PCFGSCANNER pCfgScanner, 		/* [IN] Pointer to Configuration Scanner 
										    Object */
	PTR_BUFFSTACK pRspBuffStack, 	/* [IN] Pointer to Response Buffer Stack */  
	intu16 AttrCount,				/* [IN] Attribute Count */
    intu16 AttrLength,				/* [IN] Attribute Length */
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */
)
{
    /* There are no Settable Attribute in Configuration Scanner Class*/
    return ScannerSetArgument(pReqBuffStack, pCfgScanner->pScanner, 
    pRspBuffStack, AttrCount, AttrLength, pAttrLen);
}
    
/**************************************************************************//*!
 *
 * @name  GetCfgScannerSetArgumentSize
 *
 * @brief This function returns size of Response Buffer for Configuration 
 *        Scanner Set Argument Request
 *
 * @param pReqBuffStack	:	Pointer to Buffer Stack
 *
 * @return Size of Response Buffer for Configuration Scanner Set Argument 
 *         Request 
 *******************************************************************************
 * This function returns size of Response Buffer for Configuration Scanner 
 * Set Argument Request
 ******************************************************************************/
static intu16 GetCfgScannerSetArgumentSize(
	PTR_BUFFSTACK pReqBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
    return (intu16)GetScannerSetArgumentSize(pReqBuffStack);
}
    
/**************************************************************************//*!
 *
 * @name  GetScannerSetArgumentSize
 *
 * @brief This function returns size of Response Buffer for Scanner Set  
 *        Argument Request
 *
 * @param pReqBuffStack	:	Pointer to Buffer Stack
 *
 * @return Size of Response Buffer for Scanner Set Argument Request 
 *******************************************************************************
 * This function returns size of Response Buffer for Scanner Set Argument 
 * Request
 ******************************************************************************/
intu16 GetScannerSetArgumentSize(
	PTR_BUFFSTACK pReqBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
    intu16 size = 0;
    ModificationList *pModList = (ModificationList *)GetStackTop(pReqBuffStack);
    AttributeModEntry *pAttrModEntry = &pModList->value[0];
    intu16 i;
    for(i = 0; i < ieee_ntohs(pModList->count); i++)
    {
        /* Defined only for Operational State Attribute */                
        if(ieee_ntohs(pAttrModEntry->attribute.attribute_id) == 
            MDC_ATTR_OP_STAT)
        {
            size += (intu16)(sizeof(OID_Type) + 
                    ANY_HEADER_SIZE + 
                    sizeof(OperationalState));
        }
        
        (intu8*)pAttrModEntry += GetAttributeModEntry(pAttrModEntry);
    }
    
    return size;
}

/**************************************************************************//*!
 *
 * @name  ScannerSetArgument
 *
 * @brief This function handles Scanner Set Argument Request
 *
 * @param pReqBuffStack	:	Pointer to Request Buffer Stack
 * @param pScanner		:	Pointer to Scanner Object 
 * @param pReqBuffStack	:	Pointer to Response Buffer Stack
 * @param AttrCount		:	Attribute Count
 * @param AttrLength	:	Attribute Length
 * @param pAttrLen		:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function handles Scanner Set Argument Request
 ******************************************************************************/
static ERR_CODE ScannerSetArgument(
	PTR_BUFFSTACK pReqBuffStack,	/* [IN] Pointer to Request Buffer Stack */
    PSCANNER pScanner, 				/* [IN] Pointer to Scanner Object */    
	PTR_BUFFSTACK pRspBuffStack, 	/* [IN] Pointer to Response Buffer Stack */	                           
	intu16 AttrCount, 				/* [IN] Attribute Count */                 
    intu16 AttrLength, 				/* [IN] Attribute Length */                
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */ 
)									
{
    ERR_CODE err = ERROR_UNSUPPORTED;
    intu16 i;
    ModificationList *pModList = 
        (ModificationList *)GetStackTop(pReqBuffStack);
    AttributeModEntry *pAttrModEntry = &pModList->value[0];

    *pAttrLen = 0;
    for(i = 0; i < ieee_ntohs(pModList->count); i++)
    {
        /* Defined only for Operational State Attribute */                
        if(ieee_ntohs(pAttrModEntry->attribute.attribute_id) == 
            MDC_ATTR_OP_STAT)
        {
            switch(ieee_ntohs(pAttrModEntry->modify_operator))
            {
                case REPLACE:
                {
                    Any *pAny = &pAttrModEntry->attribute.
                        attribute_value; 
                    pScanner->MdcAttrOpStat = ieee_ntohs(pAny->value[1]);
                    break;
                }
                case SET_TO_DEFAULT:
                    pScanner->MdcAttrOpStat = FALSE;
                    break;
                default:
                    break;
            }
            /* For Confirmed Set Event Report */
            if(pRspBuffStack != NULL)
            {
                intu16 AttrLen;
                do
                {
                    
                    if(ERROR_SUCCESS != (err = 
                        PushOperationalState(pRspBuffStack, 
                        		ieee_htons(pScanner->MdcAttrOpStat), &AttrLen)))
                    {
                        break;
                    }
                    *pAttrLen += AttrLen;
                    if(ERROR_SUCCESS != (err = 
                        Pushintu16(pRspBuffStack, AttrLen, &AttrLen)
                        ))
                    {
                        break;
                    }
                    *pAttrLen += AttrLen;
                    if(ERROR_SUCCESS != (err = 
                        PushOID_Type(pRspBuffStack, MDC_ATTR_OP_STAT, 
                        &AttrLen)))
                    {
                        break;
                    }
                    *pAttrLen += AttrLen;
                    AttrCount++;
                    if(ERROR_SUCCESS != (err = 
                        PushAttribute(pRspBuffStack, AttrCount, 
                        (intu16)(*pAttrLen + AttrLength), &AttrLen)))
                    {
                        break;
                    }
                    *pAttrLen += AttrLen;
                }while(0);
                if(ERROR_SUCCESS != err)
                {
                    (void)PopStack(pRspBuffStack, *pAttrLen, NULL, NULL);
                    return err;
                }     
            }
        }
        (intu8*)pAttrModEntry += GetAttributeModEntry(pAttrModEntry);
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  PushMetricIdList
 *
 * @brief This function pushes Metric ID List to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pMetricIdList		:	Pointer to Metric ID List
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes Metric ID List to Buffer Stack
 ******************************************************************************/
ERR_CODE PushMetricIdList(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */
	MetricIdList *pMetricIdList,	/* [IN] Pointer to Metric ID List */
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen;
	intu16 i;
	OID_Type *pOIDType = (OID_Type*)(&pMetricIdList->value[0]);
	
	*pAttrLen = 0;
	do
	{
		for(i = (pMetricIdList->count); i > 0; i--)
		{
			if(ERROR_SUCCESS == (err = PushOID_Type(pBuffStack, pOIDType[i - 1], 
				&AttrLen)))
			{
				*pAttrLen += AttrLen;
			}
			else
			{
				break;
			}
		}
		if(ERROR_SUCCESS != err)
		{
			break;
		}
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
			pMetricIdList->count, pMetricIdList->length, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushMetricAttribute
 *
 * @brief This function pushes Metric Attribute to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pMetric			:	Pointer to Metric Object
 * @param AttrCount			:	Attribute Count
 * @param AttrLength		:	Attribute Length
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes Metric Attribute to Buffer Stack
 ******************************************************************************/
ERR_CODE PushMetricAttribute(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */
	PMETRIC pMetric,				/* [IN] Pointer to Metric Object */
	intu16 AttrCount, 				/* [IN] Attribute Count */                 
    intu16 AttrLength, 				/* [IN] Attribute Length */                
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err = ERROR_SUCCESS;
	intu16 AttrLen;
	
	*pAttrLen = 0;
	
	do
	{
        if(pMetric->OptionalAttributeFlag & OPT_MET_MSMT_ACTIVE)
        {     
    		  if(ERROR_SUCCESS == (err = PushFloat(pBuffStack, 
				  pMetric->MdcAttrTimePdMsmtActive, &AttrLen)))
    		  {
    			  *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_TIME_PD_MSMT_ACTIVE, AttrLen, &AttrLen)))
    			  {
    				  AttrCount++;
    				  *pAttrLen += AttrLen;
    			  }
    			  else
    			  {
    				  break;
    			  }
    		  }
    		  else
    		  {
    			  break;
    		  }
        }
        if((pMetric->OptionalAttributeFlag & OPT_MET_UNIT_LABEL) &&
           (pMetric->pMdcAttrUnitLabelString != NULL)
          ) 
        {     
    	  	/* Push Unit Label String */
    		  if(ERROR_SUCCESS == (err = Pushoctet_string(pBuffStack, 
				  pMetric->pMdcAttrUnitLabelString, &AttrLen)))
    		  {
    			  *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_UNIT_LABEL_STRING, AttrLen, &AttrLen)))
    			  {
    				  AttrCount++;
    				  *pAttrLen += AttrLen;
    			  }
    			  else
    			  {
       				break;
      			}
    		  }
    		  else
    		  {
    			  break;
    		  }
        }
        
        if((pMetric->OptionalAttributeFlag & OPT_MET_ID_LABEL) &&
           (pMetric->pMdcAttrIdLabelString != NULL)
          ) 
        {     
    		  /* Push ID Label String */
    		  if(ERROR_SUCCESS == (err = Pushoctet_string(pBuffStack, 
				  pMetric->pMdcAttrIdLabelString, &AttrLen)))
    		  {
    			  *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_ID_LABEL_STRING, AttrLen, &AttrLen)))
    			  {
    				  AttrCount++;
    				  *pAttrLen += AttrLen;
    			  }
    			  else
    			  {
      				break;
       			}
    	  	}
      		else
      		{
      			break;
    	  	}
        }
        
        if(pMetric->OptionalAttributeFlag & OPT_MET_SRC_HNDL_REF)
        {     
    		  /* Push Metric Source Handle Reference */
    		  if(ERROR_SUCCESS == (err = PushHANDLE(pBuffStack, 
				  pMetric->MdcAttrSourceHandleRef, &AttrLen)))
    		  {
    			  *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_SOURCE_HANDLE_REF, AttrLen, &AttrLen)))
    			  {
    				  AttrCount++;
    				  *pAttrLen += AttrLen;
    			  }
    			  else
    			  {
    				  break;
    			  }
    		  }
    		  else
    		  {
    			  break;
    		  }
        }
        
        if((pMetric->OptionalAttributeFlag & OPT_MET_ATTRVALMAP) &&
           (pMetric->pMdcAttrAttributeValMap != NULL)
          ) 
        {     
    		  /* Push Attribute Val Map */
    		  if(ERROR_SUCCESS == (err = PushAttrValMap(pBuffStack, 
				  pMetric->pMdcAttrAttributeValMap, &AttrLen)))
    		  {
    			  *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_ATTRIBUTE_VAL_MAP, AttrLen, &AttrLen)))
    			  {
    				  AttrCount++;
    				  *pAttrLen += AttrLen;
    			  }
    			  else
    			  {
    				  break;
    			  }
    		  }
    		  else
    		  {
    			  break;
    		  }
        }
        
        if(pMetric->OptionalAttributeFlag & OPT_MET_UNIT_CODE)
        {     
    		  /* Push OID */
    		  if(ERROR_SUCCESS == (err = PushOID_Type(pBuffStack, 
				  pMetric->MdcAttrUnitCode, &AttrLen)))
    		  {
    			  *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_UNIT_CODE, AttrLen, &AttrLen)))
    			  {
    				  AttrCount++;
    				  *pAttrLen += AttrLen;
    			  }
    			  else
    			  {
    				  break;
    			  }
    		  }
    		  else
    		  {
    			  break;
    		  }
        }
        
        if(pMetric->OptionalAttributeFlag & OPT_MET_ID_PART)
        {     
    		  /* Push NOMPartition */
    		  if(ERROR_SUCCESS == (err = PushNomPartition(pBuffStack, pMetric->MdcAttrMetricIdPart, &AttrLen)))
    		  {
    			  *pAttrLen += AttrLen;
    			  if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, MDC_ATTR_METRIC_ID_PART, AttrLen, &AttrLen)))
    			  {
    				  AttrCount++;
    				  *pAttrLen += AttrLen;
    			  }
    			  else
    			  {
    				  break;
      			}
      		}
    	  	else
    		  {
    			  break;
    		  }
        }
    
        if((pMetric->OptionalAttributeFlag & OPT_MET_ID_PHYS) &&
           (pMetric->MetricIdPhysioList.u.pMdcAttrIdPhysio != NULL) /* pMdcAttrIdPhysio and pMdcAttrIdPhysioList points to the same location */
          ) 
        {      
		      /* Push Metric Physio ID or Physio ID List*/
		      switch(pMetric->MetricIdPhysioList.ID_type)
		      {
		      	case METRIC_ID:   
    			    	/* Push Metric Physio ID */
    				    if(ERROR_SUCCESS == (err = PushOID_Type(pBuffStack, 
						    *pMetric->MetricIdPhysioList.u.pMdcAttrIdPhysio, 
    					  &AttrLen)))
    				    {
    					    *pAttrLen += AttrLen;
    					    if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, MDC_ATTR_ID_PHYSIO, AttrLen, &AttrLen)))
    					    {
    						    AttrCount++;
    						    *pAttrLen += AttrLen;
    					    }
    					    else
    					    {
    						    break;
      					  }
    				    }
    			    	else
    				    {
    					    break;
    				    }
				    break;
				    
			      case METRIC_ID_LIST:
				      /* Push metric ID list */
				      if(ERROR_SUCCESS == (err = PushMetricIdList(pBuffStack, 
					    pMetric->MetricIdPhysioList.u.pMdcAttrIdPhysioList, 
					    &AttrLen)))
				      {
					      *pAttrLen += AttrLen;
					      if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
						    MDC_ATTR_ID_PHYSIO_LIST, AttrLen, &AttrLen)))
					      {
						      AttrCount++;
						      *pAttrLen += AttrLen;
					      }
					      else
					      {
						      break;
					      }
				      }
				      else
				      {
					      break;
				      }
				      break;
				      
			        default:
				        err = ERROR_UNSUPPORTED;
				      break;
		      } /* END: switch(pMetric->MetricIdPhysioList.ID_type) */
	        if(ERROR_SUCCESS != err)
	        {
			      break;
	        }
        } /* END: if(pMetric->OptionalAttributeFlag & OPT_MET_ID_PHYS) */

        if(pMetric->OptionalAttributeFlag & OPT_MET_MSMT_STAT)
        {     
    		  /* Push MeasurementStatus */
    		  if(ERROR_SUCCESS == (err = PushMeasurementStatus(pBuffStack, 
				  pMetric->MdcAttrMsmtStat, &AttrLen)))
    		  {
      			*pAttrLen += AttrLen;
      			if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
		  			MDC_ATTR_MSMT_STAT, AttrLen, &AttrLen)))
    	  		{
    		  		AttrCount++;
    			  	*pAttrLen += AttrLen;
    			  }
    			  else
    			  {
    				  break;
    			  }
    		  }
    		  else
    		  {
    			  break;
    		  }
        }
        
        if(pMetric->OptionalAttributeFlag & OPT_MET_STRUCT_SMALL)
        {     
    		  /* Push Metric Struct Small */
    		  if(ERROR_SUCCESS == (err = PushStack(pBuffStack, 
				  &pMetric->MdcAttrMetricStructSmall, 
    			sizeof(MetricStructureSmall))))
    		  {
    			  *pAttrLen += sizeof(MetricStructureSmall);
    			  if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
					  MDC_ATTR_METRIC_STRUCT_SMALL, sizeof(MetricStructureSmall), 
					  &AttrLen)))
    			  {
    				  AttrCount++;
    				  *pAttrLen += AttrLen;
    			  }
    			  else
    			  {
    				  break;
    			  }
    		  }
    		  else
    		  {
    			  break;
    		  }
        }

		  /* Push MetricSpecSmall */
		  if(ERROR_SUCCESS == (err = PushMetricSpecSmall(pBuffStack, 
			  pMetric->MdcAttrMetricSpecSmall, &AttrLen)))
		  {
			  *pAttrLen += AttrLen;
			  if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
				  MDC_ATTR_METRIC_SPEC_SMALL, AttrLen, &AttrLen)))
			  {
				  AttrCount++;
				  *pAttrLen += AttrLen;
			  }
			  else
			  {
				  break;
			  }
		  }
		  else
		  {
			  break;
		  }

      if((pMetric->OptionalAttributeFlag & OPT_MET_SUPP_TYPES) &&
         (pMetric->pMdcAttrSupplementalTypes != NULL)
        ) 
      {     
    		/* Push Supplemental Type List */
    		if(ERROR_SUCCESS == (err = PushTypeVerList(pBuffStack, 
    			pMetric->pMdcAttrSupplementalTypes, &AttrLen)))
    		{
    			*pAttrLen += AttrLen;
    			if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
					MDC_ATTR_SUPPLEMENTAL_TYPES, AttrLen, &AttrLen)))
    			{
    				AttrCount++;
    				*pAttrLen += AttrLen;
    			}
    			else
    			{
    				break;
    			}
    		}
    		else
    		{
    			break;
    		}
      }
		  
		  /* Push Metric ID */
		  if(ERROR_SUCCESS == (err = PushTYPE(pBuffStack, 
			&pMetric->MdcAttrIdType, &AttrLen)))
  		{
	  		*pAttrLen += AttrLen;
		  	if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
				MDC_ATTR_ID_TYPE, AttrLen, &AttrLen)))
			  {
				  AttrCount++;
				  *pAttrLen += AttrLen;
			  }
			  else
			  {
				  break;
			  }
		  }
		  else
		  {
			  break;
		  }

		  /* Push Attribute Count and Length */
		  if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, AttrCount, 
			  (intu16)(*pAttrLen + AttrLength), &AttrLen)))
		  {
			  break;
		  }
		  *pAttrLen += AttrLen;
		  /* Push Metric Handle */
		  if(ERROR_SUCCESS != (err = PushHANDLE(pBuffStack, 
			  pMetric->MdcAttrIdHandle, &AttrLen)))
  		{
	  		break;
		  }
		  *pAttrLen += AttrLen;
	}while(0);
	
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, AttrLength, NULL, NULL);
	}
	
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushSimpleNuObsValueCmp
 *
 * @brief This function pushes SimpleNuObsValCmp Data type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pSmpNuObsValCmp	:	Pointer to SimpleNuObsValCmp Data type
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes SimpleNuObsValCmp Data type to Buffer Stack
 ******************************************************************************/
ERR_CODE PushSimpleNuObsValueCmp(
	PTR_BUFFSTACK pBuffStack,				/* [IN] Pointer to Buffer Stack */
	SimpleNuObsValueCmp* pSmpNuObsValCmp,	/* [IN] Pointer to SimpleNuObsValCmp 
													Data type */
	intu16 *pAttrLen						/* [OUT] Pointer to size of data 
													pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen = 0;
	SimpleNuObsValue *pSmpNuObsVal = 
		(SimpleNuObsValue *)&pSmpNuObsValCmp->value[0];
	intu16 i;
	*pAttrLen = 0;

	do
	{
		/* Push Values to Stack */
		for(i = pSmpNuObsValCmp->count; i > 0; i--)
		{
			if(ERROR_SUCCESS != (err = PushSimpleNuObsValue(pBuffStack, 
				pSmpNuObsVal[i], &AttrLen)))
			{
				break;
			}
			*pAttrLen += AttrLen;
		}
		if(ERROR_SUCCESS != err)
			break;

		/* Push Count and Length */
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
			pSmpNuObsValCmp->count, *pAttrLen, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushBasicNuObsValueCmp
 *
 * @brief This function pushes BasicNuObsValueCmp Data type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pBasNuObsValCmp	:	Pointer to BasicNuObsValueCmp Data type
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes BasicNuObsValueCmp Data type to Buffer Stack
 ******************************************************************************/
ERR_CODE PushBasicNuObsValueCmp(
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to Buffer Stack */  
	BasicNuObsValueCmp* pBasNuObsValCmp,/* [IN] Pointer to BasicNuObsValueCmp
												Data type */                
	intu16 *pAttrLen					/* [OUT] Pointer to size of data 
												pushed */
)												                   
{
	ERR_CODE err;
	intu16 AttrLen = 0;
	BasicNuObsValue *pBasNuObsVal = (BasicNuObsValue *)&pBasNuObsValCmp->value[0];
	intu16 i;
	*pAttrLen = 0;

	do
	{
		/* Push Values to Stack */
		for(i = ieee_htons(pBasNuObsValCmp->count); i > 0; i--)
		{
			if(ERROR_SUCCESS != (err = PushBasicNuObsValue(pBuffStack, 
				pBasNuObsVal[i-1], &AttrLen)))
			{
				break;
			}
			*pAttrLen += AttrLen;
		}
		if(ERROR_SUCCESS != err)
			break;

		/* Push Count and Length */
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				ieee_htons(pBasNuObsValCmp->count), *pAttrLen, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushNuObsValue
 *
 * @brief This function pushes NuObsValue Data type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pNuObsValue	:	Pointer to NuObsValue Data type
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes NuObsValue Data type to Buffer Stack
 ******************************************************************************/
ERR_CODE PushNuObsValue(
	PTR_BUFFSTACK pBuffStack, /* [IN] Pointer to Buffer Stack */   
	NuObsValue *pNuObsValue,  /* [IN] Pointer to NuObsValue Data type */    
	intu16 *pAttrLen		  /* [OUT] Pointer to size of data pushed */  		             
)							          
{							  		              
	ERR_CODE err;
	intu16 AttrLen = 0;
	*pAttrLen = 0;

	do
	{
		if(ERROR_SUCCESS != (err = PushFloat(pBuffStack, pNuObsValue->value, 
			&AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushOID_Type(pBuffStack, 
			pNuObsValue->unit_code, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
			pNuObsValue->metric_id, 
			pNuObsValue->state, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushNuObsValueCmp
 *
 * @brief This function pushes NuObsValueCmp Data type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pNuObsValCmp		:	Pointer to NuObsValueCmp Data type
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes NuObsValueCmp Data type to Buffer Stack
 ******************************************************************************/
ERR_CODE PushNuObsValueCmp(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */         
	NuObsValueCmp* pNuObsValCmp, 	/* [IN] Pointer to NuObsValueCmp Data type*/ 
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */  
)
{
	ERR_CODE err;
	intu16 AttrLen = 0;
	NuObsValue *pNuObsVal = (NuObsValue *)&pNuObsValCmp->value[0];
	intu16 i;
	*pAttrLen = 0;
	do
	{
		/* Push Values to Stack */
		for(i = pNuObsValCmp->count; i > 0; i--)
		{
			if(ERROR_SUCCESS != (err = PushNuObsValue(pBuffStack, &pNuObsVal[i],
				&AttrLen)))
			{
				break;
			}
			*pAttrLen += AttrLen;
		}
		if(ERROR_SUCCESS != err)
			break;

		/* Push Count and Length */
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, pNuObsValCmp->count,
			*pAttrLen, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushNumericVal
 *
 * @brief This function pushes Numeric Value to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param AttrId			:	Attribute ID
 * @param pNumeric			:	Pointer to Numeric Value
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Numeric Value to Buffer Stack
 *****************************************************************************/
ERR_CODE PushNumericVal(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */
	OID_Type AttrId,            /* [IN] Attribute ID */           
	void* pVal, 			    /* [IN] Pointer to Numeric Value */ 
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */    
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		switch(AttrId)
		{
			case MDC_ATTR_NU_VAL_OBS_SIMP:
				err = PushSimpleNuObsValue(pBuffStack, 
				    *((SimpleNuObsValue*)(pVal)), &AttrLen);
				break;
			case MDC_ATTR_NU_CMPD_VAL_OBS_SIMP:
				err = PushSimpleNuObsValueCmp(pBuffStack, 
				    (SimpleNuObsValueCmp*)(pVal), &AttrLen);
				break;
			case MDC_ATTR_NU_VAL_OBS_BASIC:
				err = PushBasicNuObsValue(pBuffStack, *((BasicNuObsValue*)(pVal)),
				     &AttrLen);
				break;
			case MDC_ATTR_NU_CMPD_VAL_OBS_BASIC:
				err = PushBasicNuObsValueCmp(pBuffStack, 
				    (BasicNuObsValueCmp*)(pVal), &AttrLen);
				break;
			case MDC_ATTR_NU_VAL_OBS:
				err = PushNuObsValue(pBuffStack, (NuObsValue*)(pVal), &AttrLen);
				break;
			case MDC_ATTR_NU_CMPD_VAL_OBS:
				err = PushNuObsValueCmp(pBuffStack, 
					(NuObsValueCmp*)(pVal), &AttrLen);
				break;
			default:
				err = ERROR_UNSUPPORTED;
				break;
		}
		if(err != ERROR_SUCCESS)
		{
			break;
		}
		*pAttrLen += AttrLen;
		
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		*pAttrLen = 0;
	}
	return err;
	
}

/**************************************************************************//*!
 *
 * @name  PushNumericAttribute
 *
 * @brief This function pushes Numeric Attributes to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pNumeric			:	Pointer to Numeric Object
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes Numeric Attributes to Buffer Stack
 ******************************************************************************/
ERR_CODE PushNumericAttribute(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */       
	PNUMERIC pNumeric, 			/* [IN] Pointer to Numeric Object */     
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	intu16 AttrCount = 0;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
        if(pNumeric->OptionalAttributeFlag & OPT_NUMERIC_ACCURACY)
        {     
    		/* Push Accuracy */
    		if(ERROR_SUCCESS == (err = PushFloat(pBuffStack, 
				pNumeric->MdcAttrNuAccurMsmt, &AttrLen)))
    		{
    			*pAttrLen += AttrLen;
    			if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
					MDC_ATTR_NU_ACCUR_MSMT, AttrLen, &AttrLen)))
    			{
    				AttrCount++;
    				*pAttrLen += AttrLen;
    			}
    			else
    			{
    				break;
    			}
    		}
    		else
    		{
    			break;
    		}
        }
		if(ERROR_SUCCESS != (err = PushMetricAttribute(pBuffStack, 
			pNumeric->pMetric, AttrCount, *pAttrLen, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushObjectClass(pBuffStack, 
			MDC_MOC_VMO_METRIC_NU, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushScaleRangeAbsolute
 *
 * @brief This function pushes Absolute Scale Range to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param fLower			:	Lower Scale Range Value
 * @param fUpper			:	Upper Scale Range Value
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes Absolute Scale Range to Buffer Stack
 ******************************************************************************/
ERR_CODE PushScaleRangeAbsolute(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */      
	FLOAT_Type fLower, 			/* [IN] Lower Scale Range Value */      
	FLOAT_Type fUpper, 			/* [IN] Upper Scale Range Value */      
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushFloat(pBuffStack, fUpper, &AttrLen)))
		{
			break;
		}
		*pAttrLen = AttrLen;
		if(ERROR_SUCCESS != (err = PushFloat(pBuffStack, fLower, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;

	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushScaleRangeSpec32
 *
 * @brief This function pushes ScaleRangeSpec32 Data type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pScaleRangeSpec32	:	Pointer to ScaleRangeSpec32 Data type
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes ScaleRangeSpec32 Data type to Buffer Stack
 ******************************************************************************/
ERR_CODE PushScaleRangeSpec32(
	PTR_BUFFSTACK pBuffStack,				/* [IN] Pointer to Buffer Stack */             
	ScaleRangeSpec32 *pScaleRangeSpec32, 	/* [IN] Pointer to ScaleRangeSpec32 
													Data type */
	intu16 *pAttrLen						/* [OUT] Pointer to size of data 
													pushed */      
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushScaleRangeScale32(pBuffStack, 
			pScaleRangeSpec32->lower_scaled_value, 
			pScaleRangeSpec32->upper_scaled_value, &AttrLen)))
		{
			break;
		}
		*pAttrLen = AttrLen;
		if(ERROR_SUCCESS != (err = PushScaleRangeAbsolute(pBuffStack, 
			pScaleRangeSpec32->lower_absolute_value, 
			pScaleRangeSpec32->upper_absolute_value, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;

	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushScaleRangeSpec16
 *
 * @brief This function pushes ScaleRangeSpec16 Data type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pScaleRangeSpec16	:	Pointer to ScaleRangeSpec16 Data type
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes ScaleRangeSpec16 Data type to Buffer Stack
 ******************************************************************************/
ERR_CODE PushScaleRangeSpec16(
	PTR_BUFFSTACK pBuffStack,				/* [IN] Pointer to Buffer Stack */ 
	ScaleRangeSpec16 *pScaleRangeSpec16, 	/* [IN] Pointer to ScaleRangeSpec16
													Data type */               
	intu16 *pAttrLen						/* [OUT] Pointer to size of data    
													pushed */                  
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushScaleRangeScale16(pBuffStack, 
			pScaleRangeSpec16->lower_scaled_value, 
			pScaleRangeSpec16->upper_scaled_value, &AttrLen)))
		{
			break;
		}
		*pAttrLen = AttrLen;
		if(ERROR_SUCCESS != (err = PushScaleRangeAbsolute(pBuffStack, 
			pScaleRangeSpec16->lower_absolute_value, 
			pScaleRangeSpec16->upper_absolute_value, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;

	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushScaleRangeSpec8
 *
 * @brief This function pushes ScaleRangeSpec8 Data type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pScaleRangeSpec8	:	Pointer to ScaleRangeSpec8 Data type
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes ScaleRangeSpec8 Data type to Buffer Stack
 ******************************************************************************/
ERR_CODE PushScaleRangeSpec8(
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to Buffer Stack */ 
	ScaleRangeSpec8 *pScaleRangeSpec8, 	/* [IN] Pointer to ScaleRangeSpec8
												Data type */               
	intu16 *pAttrLen					/* [OUT] Pointer to size of data    
												pushed */                  
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, 
			&pScaleRangeSpec8->lower_scaled_value, (2*sizeof(intu8)))))
		{
			break;
		}
		*pAttrLen = (2*sizeof(intu8));
		if(ERROR_SUCCESS != (err = PushScaleRangeAbsolute(pBuffStack, 
			pScaleRangeSpec8->lower_absolute_value, 
			pScaleRangeSpec8->upper_absolute_value, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;

	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushSaSpec
 *
 * @brief This function pushes SaSpec Data type to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pSaSpec			:	Pointer to SaSpec Data type
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes SaSpec Data type to Buffer Stack
 ******************************************************************************/
ERR_CODE PushSaSpec(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */
	SaSpec *pSaSpec, 			/* [IN] Pointer to ScaleRangeSpec8 Data type */
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */  		              
)								               
{										
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushSaFlags(pBuffStack, pSaSpec->flags, 
			&AttrLen)))
		{
			break;
		}
		*pAttrLen = AttrLen;
		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, &pSaSpec->sample_type,
			sizeof(SampleType))))
		{
			break;
		}
		*pAttrLen += sizeof(SampleType);
		if(ERROR_SUCCESS != (err = PushSaFlags(pBuffStack, pSaSpec->array_size,
			&AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;

	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushRTSAAttribute
 *
 * @brief This function pushes RTSA Attributes to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pRTSA				:	Pointer to RTSA Object
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes RTSA Attributes to Buffer Stack
 ******************************************************************************/
ERR_CODE PushRTSAAttribute(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */              
	PRTSA pRTSA, 				/* [IN] Pointer to RTSA Object */ 
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */  		
)
{
	ERR_CODE err;
	intu16 AttrCount = 0;
	intu16 AttrLength = 0;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		/* Push SaSpec */
		if(ERROR_SUCCESS == (err = PushSaSpec(pBuffStack, 
			&pRTSA->MdcAttrSaSpecn, &AttrLen)))
		{
			*pAttrLen += AttrLen;
			if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
				MDC_ATTR_SA_SPECN, AttrLen, &AttrLen)))
			{
				AttrCount++;
				*pAttrLen += AttrLen;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}

		switch(pRTSA->ScaleRangeSpec.scale_range_spec_type)
		{
			case RTSA_SCALE_RANGE_SPEC_8:
				if(ERROR_SUCCESS == (err = PushScaleRangeSpec8(pBuffStack, 
				pRTSA->ScaleRangeSpec.u.pMdcAttrScaleSpecnI8, &AttrLen)))
				{
					*pAttrLen += AttrLen;
					if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
						MDC_ATTR_SCALE_SPECN_I8, AttrLen, &AttrLen)))
					{
						AttrCount++;
						*pAttrLen += AttrLen;
					}
					else
					{
						break;
					}
				}
				break;
			case RTSA_SCALE_RANGE_SPEC_16:
				if(ERROR_SUCCESS == (err = PushScaleRangeSpec16(pBuffStack, 
					pRTSA->ScaleRangeSpec.u.pMdcAttrScaleSpecnI16, &AttrLen)))
				{
					*pAttrLen += AttrLen;
					if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
						MDC_ATTR_SCALE_SPECN_I16, AttrLen, &AttrLen)))
					{
						AttrCount++;
						*pAttrLen += AttrLen;
					}
					else
					{
						break;
					}
				}
				break;
			case RTSA_SCALE_RANGE_SPEC_32:
				if(ERROR_SUCCESS == (err = PushScaleRangeSpec32(pBuffStack, 
					pRTSA->ScaleRangeSpec.u.pMdcAttrScaleSpecnI32, &AttrLen)))
				{
					*pAttrLen += AttrLen;
					if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack,
						MDC_ATTR_SCALE_SPECN_I32, AttrLen, &AttrLen)))
					{
						AttrCount++;
						*pAttrLen += AttrLen;
					}
					else
					{
						break;
					}
				}
				break;
			default:
				err = ERROR_UNSUPPORTED;
				break;
		}
		if(ERROR_SUCCESS != err)
		{
			break;
		}
		/* Push Sample Time Period */
		if(ERROR_SUCCESS == (err = PushRelativeTime(pBuffStack, 
			pRTSA->MdcAttrTimePdSamp, &AttrLen)))
		{
			*pAttrLen += AttrLen;
			if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
				MDC_ATTR_TIME_PD_SAMP , AttrLen, &AttrLen)))
			{
				AttrCount++;
				*pAttrLen += AttrLen;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
		
		/* Push Metric Class Attributes */
		if(ERROR_SUCCESS != (err = PushMetricAttribute(pBuffStack, 
			pRTSA->pMetric, AttrCount, *pAttrLen, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushObjectClass(pBuffStack,
			MDC_MOC_VMO_METRIC_SA_RT, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushEnumerationAttribute
 *
 * @brief This function pushes Enumeration Attributes to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pEnumeration		:	Pointer to Enumeration Object
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes Enumeration Attributes to Buffer Stack
 ******************************************************************************/
ERR_CODE PushEnumerationAttribute(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */       
	PENUMERATION pEnumeration, 	/* [IN] Pointer to Enumeration Object */        
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	intu16 AttrCount = 0;
	intu16 AttrLength = 0;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
	    if(pEnumeration->OptionalAttributeFlag & OPT_ENUM_OBS_VAL_PART)
	    {
    		if(ERROR_SUCCESS == (err = Pushintu16(pBuffStack, 
				pEnumeration->MdcAttrEnumObsValPart, &AttrLen)))
    		{
    			*pAttrLen += AttrLen;
    			if(ERROR_SUCCESS == (err = PushAttribute(pBuffStack, 
					MDC_ATTR_ENUM_OBS_VAL_PART, AttrLen, &AttrLen)))
    			{
    				AttrCount++;
    				*pAttrLen += AttrLen;
    			}
    			else
    			{
    				break;
    			}
    		}
    		else
    		{
    			break;
    		}
	    }
		/* Push Metric Class Attributes */
		if(ERROR_SUCCESS != (err = PushMetricAttribute(pBuffStack, 
			pEnumeration->pMetric, AttrCount, *pAttrLen, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushObjectClass(pBuffStack, 
			MDC_MOC_VMO_METRIC_ENUM, &AttrLen)))
		{               
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushHandleAttrValMapEntryList
 *
 * @brief This function recursively pushes Handle Attribute Val Map to Buffer
 *        Stack
 *
 * @param pBuffStack				:	Pointer to Buffer Stack
 * @param AttrCount					:	Attribute Count
 * @param pHandleAttrValMapEntry	:	Pointer to HandleAttrValMapEntry
 * @param pAttrLen					:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function recursively pushes Handle Attribute Val Map to Buffer Stack
 ******************************************************************************/
ERR_CODE PushHandleAttrValMapEntryList(
	PTR_BUFFSTACK pBuffStack,					  /* [IN] Pointer to Buffer 
												  		  Stack */ 
	intu16 AttrCount,							  /* [IN] Attribute Count */ 
	HandleAttrValMapEntry *pHandleAttrValMapEntry,/* [IN] Pointer to 
												  		HandleAttrValMapEntry */
	intu16* pAttrLen							 /* [OUT] Pointer to size of 
														  data pushed */
)
{
	ERR_CODE err;
	*pAttrLen = 0;
	if(1 != AttrCount)
	{
		intu16 AttrLen = 0;
		HandleAttrValMapEntry *temp = pHandleAttrValMapEntry;
		(uint_8_ptr)temp += sizeof(HANDLE) + ATTR_VAL_MAP_HEADER_SIZE + 
			temp->attr_val_map.length;
		if(ERROR_SUCCESS == (err = PushHandleAttrValMapEntryList(pBuffStack, 
			(intu16)(AttrCount - 1), temp, &AttrLen)))
		{
			*pAttrLen += AttrLen;

			if(ERROR_SUCCESS == (err = PushHandleAttrValMapEntry(pBuffStack, 
				pHandleAttrValMapEntry, &AttrLen)))
			{
				*pAttrLen += AttrLen;
			}
			else
			{
				(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
				*pAttrLen = 0;
			}
		} 
		else
		{
			(void)PopStack(pBuffStack, AttrLen, NULL, NULL);
		}
		return err;
	}
	return PushHandleAttrValMapEntry(pBuffStack, pHandleAttrValMapEntry, 
		pAttrLen);
}

/**************************************************************************//*!
 *
 * @name  PushHandleAttrValMap
 *
 * @brief This function pushes Handle Attribute Val Map to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pHandleAttrValMap	:	Pointer to HandleAttrValMap
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes Handle Attribute Val Map to Buffer Stack
 ******************************************************************************/
ERR_CODE PushHandleAttrValMap(
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to Buffer Stack */
	HandleAttrValMap *pHandleAttrValMap,/* [IN] Pointer to HandleAttrValMap */ 
	intu16* pAttrLen					/* [OUT] Pointer to size of data 
												 pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushHandleAttrValMapEntryList(pBuffStack, 
			pHandleAttrValMap->count, 
			(HandleAttrValMapEntry *)&pHandleAttrValMap->value, 
			&AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
			pHandleAttrValMap->count, 
			pHandleAttrValMap->length, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushScannerAttribute
 *
 * @brief This function pushes Scanner Attributes to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pScanner			:	Pointer to Scanner Object
 * @param AttrCount			:	Attribute Count
 * @param AttrLength		:	Attribute Length
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes Scanner Attributes to Buffer Stack
 ******************************************************************************/
ERR_CODE PushScannerAttribute(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */        
	PSCANNER pScanner, 			/* [IN] Pointer to Scanner Object */       
	intu16 AttrCount, 			/* [IN] Attribute Count */                
	intu16 AttrLength, 			/* [IN] Attribute Length */               
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */
)
{    
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		/* Push Handle Attribute Val Map */
		if(ERROR_SUCCESS != (err = PushHandleAttrValMap(pBuffStack, 
			pScanner->pMdcAttrScanHandleAttrValMap, &AttrLen)))
		{
		    break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
		    MDC_ATTR_SCAN_HANDLE_ATTR_VAL_MAP, 
			AttrLen, &AttrLen)))
		{
			break;
		}
		AttrCount++;
		*pAttrLen += AttrLen;

		/* Push Operational State attribute */
		if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, 
		    pScanner->MdcAttrOpStat, &AttrLen)))
		{
		    break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, MDC_ATTR_OP_STAT, 
		    AttrLen, &AttrLen)))
		{
		    break;
		}
		AttrCount++;
		*pAttrLen += AttrLen;

		/* Push Attribute Count and Length */
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, AttrCount, 
		    (intu16)(*pAttrLen + AttrLength), &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		/* Push Scanner Handle */
		if(ERROR_SUCCESS != (err = PushHANDLE(pBuffStack, 
		    pScanner->MdcAttrIdHandle, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, AttrLength, NULL, NULL);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushCFGScannerAttribute
 *
 * @brief This function pushes Configuration Scanner Attributes to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pCFGScanner		:	Pointer to Configuration Scanner Object
 * @param AttrCount			:	Attribute Count
 * @param AttrLength		:	Attribute Length
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes Configuration Scanner Attributes to Buffer Stack
 ******************************************************************************/
ERR_CODE PushCFGScannerAttribute(
	PTR_BUFFSTACK pBuffStack, /* [IN] Pointer to Buffer Stack */         
	PCFGSCANNER pCFGScanner,  /* [IN] Pointer to Configuration Scanner Object */       
	intu16 AttrCount, 		  /* [IN] Attribute Count */                 
	intu16 AttrLength, 		  /* [IN] Attribute Length */                
	intu16 *pAttrLen		  /* [OUT] Pointer to size of data pushed */ 
)
{
	ERR_CODE err;
	intu16 AttrLen;
    
    *pAttrLen = 0;
    do
    {
        if(pCFGScanner->OptionalAttributeFlag & OPT_CFGSCAN_TRANS_WINDOW)
        {
            /* Push Transmit Window */
    		if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, 
    		    pCFGScanner->MdcAttrTxWind, &AttrLen)))
    		{
    		    break;
    		}
    		*pAttrLen += AttrLen;
    		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
    		    MDC_ATTR_TX_WIND, AttrLen, &AttrLen)))
			{
			    break;
			}
			AttrCount++;
			*pAttrLen += AttrLen;
        }
        if(pCFGScanner->OptionalAttributeFlag & OPT_CFGSCAN_CONFIRM_TIMEOUT)
        {
            /* Push Confirm Timeout */
    		if(ERROR_SUCCESS != (err = PushRelativeTime(pBuffStack, 
    		    pCFGScanner->MdcAttrConfirmTimeout, &AttrLen)))
    		{
    		    break;
    		}
    		*pAttrLen += AttrLen;
   			if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
   			    MDC_ATTR_CONFIRM_TIMEOUT, AttrLen, &AttrLen)))
    		{
    		    break;
    		}
			AttrCount++;
    		*pAttrLen += AttrLen;
        }
        /* Push Confirm Mode */
    	if(ERROR_SUCCESS != (err = PushConfirmMode(pBuffStack, 
    	    pCFGScanner->MdcAttrConfirmMode, &AttrLen)))
		{
		    break;
		}
		*pAttrLen += AttrLen;
    	if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
    	    MDC_ATTR_CONFIRM_MODE, AttrLen, &AttrLen)))
		{
		    break;
		}
		AttrCount++;
		*pAttrLen += AttrLen;

		/* Push Scanner Class Attributes */
		if(ERROR_SUCCESS != (err = PushScannerAttribute(pBuffStack, 
		    pCFGScanner->pScanner, AttrCount, 
		    (intu16)(*pAttrLen + AttrLength), &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;

	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushEPICFGScannerAttribute
 *
 * @brief This function pushes Episoidic Scanner Attributes to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pEPICFGScanner	:	Pointer to Episoidic Scanner Object
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes Episoidic Scanner Attributes to Buffer Stack
 ******************************************************************************/
ERR_CODE PushEPICFGScannerAttribute(
	PTR_BUFFSTACK pBuffStack,	   /* [IN] Pointer to Buffer Stack */                 
	PEPICFGSCANNER pEPICFGScanner, /* [IN] Pointer to Episoidic Scanner Object*/ 
	intu16 *pAttrLen			   /* [OUT] Pointer to size of data pushed */         	
)									
{									
	ERR_CODE err;
	intu16 AttrCount = 0;
	intu16 AttrLen;
    *pAttrLen = 0;
    do
    {
        if(pEPICFGScanner->OptionalAttributeFlag & OPT_EPISCAN_MIN_RPT_INT)
        {
            /* Push Min-Reporting-Interval */
    		if(ERROR_SUCCESS != (err = PushRelativeTime(pBuffStack, 
    		    pEPICFGScanner->MdcAttrScanRepPdMin, &AttrLen)))
    		{
    		    break;
    		}
    		*pAttrLen += AttrLen;
    		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
    		    MDC_ATTR_SCAN_REP_PD_MIN, AttrLen, &AttrLen)))
    		{
    		    break;
    		}
			AttrCount++;
			*pAttrLen += AttrLen;
        }
		/* Push CFGScanner Class Attributes */
		if(ERROR_SUCCESS != (err = PushCFGScannerAttribute(pBuffStack, 
		    pEPICFGScanner->pCfgScanner, 
			AttrCount, (intu16)(*pAttrLen), &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushObjectClass(pBuffStack, 
			MDC_MOC_SCAN_CFG_EPI, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushPERICFGScannerAttribute
 *
 * @brief This function pushes Periodic Scanner Attributes to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pEPICFGScanner	:	Pointer to Periodic Scanner Object
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes Periodic Scanner Attributes to Buffer Stack
 ******************************************************************************/
ERR_CODE PushPERICFGScannerAttribute(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */           
	PPERICFGSCANNER pPERICFGScanner,/* [IN] Pointer to Periodic Scanner Object*/ 
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */   
)
{
	ERR_CODE err;
	intu16 AttrCount = 0;
	intu16 AttrLen;
	*pAttrLen = 0;
    do
    {
        /* Push Reporting-Interval */
		if(ERROR_SUCCESS != (err = PushRelativeTime(pBuffStack, 
		    pPERICFGScanner->MdcAttrScanRepPd, &AttrLen)))
		{
		    break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
		    MDC_ATTR_SCAN_REP_PD, AttrLen, &AttrLen)))
		{
		    break;
		}
		AttrCount++;
		*pAttrLen += AttrLen;
    		
		/* Push CFGScanner Class Attributes */
		if(ERROR_SUCCESS != (err = PushCFGScannerAttribute(pBuffStack, 
		    pPERICFGScanner->pCfgScanner, AttrCount, 
		    (intu16)(*pAttrLen), &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushObjectClass(pBuffStack, 
			MDC_MOC_SCAN_CFG_PERI, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushSegmEntryElem
 *
 * @brief This function pushes PM Segment Entry Element to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pSegmEntryElem	:	Pointer to SegmEntryElem
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes PM Segment Entry Element to Buffer Stack
 ******************************************************************************/
ERR_CODE PushSegmEntryElem(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */          
	SegmEntryElem *pSegmEntryElem, 	/* [IN] Pointer to SegmEntryElem */
	intu16* pAttrLen				/* [OUT] Pointer to size of data pushed */  
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushAttrValMap(pBuffStack, 
		    &pSegmEntryElem->attr_val_map, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;

		if(ERROR_SUCCESS != (err = PushHANDLE(pBuffStack, 
				ieee_htons(pSegmEntryElem->handle), &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushTYPE(pBuffStack, 
		    &pSegmEntryElem->metric_type, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;

		if(ERROR_SUCCESS != (err = PushOID_Type(pBuffStack, 
				ieee_htons(pSegmEntryElem->class_id), &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushSegmEntryElemList
 *
 * @brief This function recursively pushes PM Segment Entry Element to Buffer 
 *        Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param AttrCount			:	Attribute Count
 * @param pSegmEntryElem	:	Pointer to SegmEntryElem
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function recursively pushes PM Segment Entry Element to Buffer Stack
 ******************************************************************************/
ERR_CODE PushSegmEntryElemList(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */        
	intu16 AttrCount, 				/* [IN] Attribute Count */
	SegmEntryElem *pSegmEntryElem, 	/* [IN] Pointer to SegmEntryElem */       
	intu16* pAttrLen				/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	*pAttrLen = 0;
	if(1 != AttrCount)
	{
		intu16 AttrLen = 0;
		SegmEntryElem *temp = pSegmEntryElem;
		(uint_8_ptr)temp += sizeof(OID_Type) + sizeof(TYPE) + sizeof(HANDLE) + 
		    ATTR_VAL_MAP_HEADER_SIZE + temp->attr_val_map.length;
		if(ERROR_SUCCESS == (err = PushSegmEntryElemList(pBuffStack, 
			(intu16)(AttrCount - 1), temp, &AttrLen)))
		{
			*pAttrLen += AttrLen;

			if(ERROR_SUCCESS == (err = PushSegmEntryElem(pBuffStack, 
				pSegmEntryElem, &AttrLen)))
			{
				*pAttrLen += AttrLen;
			}
			else
			{
				(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
				*pAttrLen = 0;
			}
		} 
		else
		{
			(void)PopStack(pBuffStack, AttrLen, NULL, NULL);
		}
		return err;
	}
	return PushSegmEntryElem(pBuffStack, pSegmEntryElem, pAttrLen);
}

/**************************************************************************//*!
 *
 * @name  PushSegmElemEntry
 *
 * @brief This function pushes PM Segment Entry Element List to Buffer Stack 
 *
 * @param pBuffStack			:	Pointer to Buffer Stack
 * @param pSegmEntryElemList	:	Pointer to SegmEntryElemList
 * @param pAttrLen				:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes PM Segment Entry Element List to Buffer Stack
 ******************************************************************************/
ERR_CODE PushSegmElemEntry(
	PTR_BUFFSTACK pBuffStack,				/* [IN] Pointer to Buffer Stack */        
	SegmEntryElemList *pSegmEntryElemList, 	/* [IN] Pointer to 
													SegmEntryElemList */       
	intu16* pAttrLen						/* [OUT] Pointer to size of data 
													 pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushSegmEntryElemList(pBuffStack, 
				ieee_htons(pSegmEntryElemList->count), 
			(SegmEntryElem *)&pSegmEntryElemList->value, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				ieee_htons(pSegmEntryElemList->count), 
				ieee_htons(pSegmEntryElemList->length), &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushPMSegmentAttribute
 *
 * @brief This function pushes PM Segment Attributes to Buffer Stack 
 *
 * @param pPMSegment		:	Pointer to PM Segment
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes PM Segment Attributes to Buffer Stack
 ******************************************************************************/
static ERR_CODE PushPMSegmentAttribute(
	PPMSEGMENT pPMSegment,		/* [IN] Pointer to PM Segment */
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */
	intu16 *pAttrLength			/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen;
	intu16 AttrCount = 0;
	*pAttrLength = 0;
	
	do
	{
	    if(ERROR_SUCCESS != (err = PushRelativeTime(pBuffStack, 
			pPMSegment->MdcAttrTransferTimeout, &AttrLen)))
	    {
	        break;
	    }
	    *pAttrLength += AttrLen;
	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
			MDC_ATTR_TRANSFER_TIMEOUT, AttrLen, &AttrLen)))
	    {
	        break;
	    }
	    *pAttrLength += AttrLen;
	    AttrCount++;
	    
	    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_CONFIRM_TIMEOUT)
	    {
    	    if(ERROR_SUCCESS != 
    	        (err = PushRelativeTime(pBuffStack, 
				pPMSegment->MdcAttrConfirmTimeout, &AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				MDC_ATTR_CONFIRM_TIMEOUT , AttrLen, &AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    AttrCount++;
	    }
	    
	    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_SEG_STATISTICS)
	    {
    	    intu16 SegStatAttrLen;
    	    pPMSegment->pMdcAttrSegStats->value->segm_stat_entry.length = ieee_htons(pPMSegment->pMdcAttrSegStats->value->segm_stat_entry.length);
    	    if(ERROR_SUCCESS != 
    	        (err = Pushoctet_string(pBuffStack, 
				&pPMSegment->pMdcAttrSegStats->value->segm_stat_entry, 
				&AttrLen)))
    	    {
    	        break;
    	    }
    	    pPMSegment->pMdcAttrSegStats->value->segm_stat_entry.length = ieee_htons(pPMSegment->pMdcAttrSegStats->value->segm_stat_entry.length);
    	    *pAttrLength += AttrLen;
    	    SegStatAttrLen = AttrLen;
    	    if(ERROR_SUCCESS != 
    	        (err = PushSegmStatType(pBuffStack, 
    	        		ieee_htons(pPMSegment->pMdcAttrSegStats->value->segm_stat_type), 
				&AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    SegStatAttrLen += AttrLen;
    	    
    	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
    	    		ieee_htons(pPMSegment->pMdcAttrSegStats->count) , 
    	    		ieee_htons(pPMSegment->pMdcAttrSegStats->length), &AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    SegStatAttrLen += AttrLen;
    	    
    	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				MDC_ATTR_SEG_STATS, SegStatAttrLen, &AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    AttrCount++;
	    }
	    
	    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_USAGE_CNT)
	    {
    	    if(ERROR_SUCCESS != 
    	        (err = Pushintu32(pBuffStack, pPMSegment->MdcAttrsegUsageCnt, 
				&AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				MDC_ATTR_SEG_USAGE_CNT, AttrLen, &AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    AttrCount++;
	    } 
	    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_DATE_TIME_ADJUST)
	    {
    		AttrLen = sizeof(AbsoluteTimeAdjust);
    		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, 
				&pPMSegment->MdcAttrTimeAbsAdjust, AttrLen)))
    		{
    			break;
    		}	
            *pAttrLength += AttrLen;
    	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				MDC_ATTR_TIME_ABS_ADJUST, AttrLen, &AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    AttrCount++;
	    } 
	    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_ABS_TIME)
	    {
    		AttrLen = sizeof(AbsoluteTime);
    		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, 
				&pPMSegment->MdcAttrTimeEndSeg, AttrLen)))
    		{
    			break;
    		}	
            *pAttrLength += AttrLen;
    	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				MDC_ATTR_TIME_END_SEG, AttrLen, &AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    AttrCount++;

    		AttrLen = sizeof(AbsoluteTime);
    		if(ERROR_SUCCESS != (err = PushStack(pBuffStack, 
				&pPMSegment->MdcAttrTimeStartSeg, AttrLen)))
    		{
    			break;
    		}	
            *pAttrLength += AttrLen;
    	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				MDC_ATTR_TIME_START_SEG, AttrLen, &AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    AttrCount++;
	    } 
	    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_LABEL)
	    {
	    	pPMSegment->pMdcAttrPmSegLabelString->length = ieee_htons(pPMSegment->pMdcAttrPmSegLabelString->length);
    		if(ERROR_SUCCESS != (err = Pushoctet_string(pBuffStack, 
				pPMSegment->pMdcAttrPmSegLabelString, &AttrLen)))
    		{
    			break;
    		}	
            *pAttrLength += AttrLen;
    	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				MDC_ATTR_PM_SEG_LABEL_STRING, AttrLen, &AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    AttrCount++;
	    } 
	    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_SAMPLE_PERIOD)
	    {
    		if(ERROR_SUCCESS != (err = PushRelativeTime(pBuffStack, 
				pPMSegment->MdcAttrTimePdSamp, &AttrLen)))
    		{
    			break;
    		}	
            *pAttrLength += AttrLen;
    	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				MDC_ATTR_TIME_PD_SAMP, AttrLen, &AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    AttrCount++;
	    } 

		if(ERROR_SUCCESS != (err = PushOperationalState(pBuffStack, 
			pPMSegment->MdcAttrOpStat, &AttrLen)))
		{
			break;
		}	
        *pAttrLength += AttrLen;
	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
			MDC_ATTR_OP_STAT, AttrLen, &AttrLen)))
	    {
	        break;
	    }
	    *pAttrLength += AttrLen;
	    AttrCount++;

	    if(pPMSegment->OptionalAttributeFlag & OPT_PMSEG_PERSON_ID)
	    {
    		if(ERROR_SUCCESS != (err = PushPersonId(pBuffStack, 
				pPMSegment->MdcAttrPmSegPersonId, &AttrLen)))
    		{
    			break;
    		}	
            *pAttrLength += AttrLen;
    	    if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				MDC_ATTR_PM_SEG_PERSON_ID , AttrLen, &AttrLen)))
    	    {
    	        break;
    	    }
    	    *pAttrLength += AttrLen;
    	    AttrCount++;
	    } 

		{
		    intu16 SegEntryMapLen = 0;
		    if(ERROR_SUCCESS != (err = PushSegmElemEntry(pBuffStack, 
    			&pPMSegment->pMdcAttrPmSegMap->segm_entry_elem_list, &AttrLen)))
    		{
    		    break;
    		}
    		*pAttrLength += AttrLen;
    		SegEntryMapLen += AttrLen;

		    if(ERROR_SUCCESS != (err = PushSegmEntryHeader(pBuffStack, 
		    		ieee_htons(pPMSegment->pMdcAttrPmSegMap->segm_entry_header), &AttrLen)))
    		{
    		    break;
    		}
    		*pAttrLength += AttrLen;
    		SegEntryMapLen += AttrLen;
    		
    		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
    		    MDC_ATTR_PM_SEG_MAP, 
    			SegEntryMapLen, &AttrLen)))
    		{
    			break;
    		}
    	    AttrCount++;
    		*pAttrLength += AttrLen;

		}
		if(ERROR_SUCCESS != (err = PushInstNumber(pBuffStack, 
			pPMSegment->MdcAttrIdInstNo, &AttrLen)))
		{
			break;
		}	
        *pAttrLength += AttrLen;		

		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
			MDC_ATTR_ID_INSTNO, AttrLen, &AttrLen)))
		{
			break;
		}
	    AttrCount++;
		*pAttrLength += AttrLen;
		/* Push Attribute Count and Length */
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, AttrCount, 
			*pAttrLength, &AttrLen)))
		{
			break;
		}
		*pAttrLength += AttrLen;
		if(ERROR_SUCCESS != (err = PushInstNumber(pBuffStack, 
			pPMSegment->MdcAttrIdInstNo, &AttrLen)))
		{
			break;
		}	
        *pAttrLength += AttrLen;		
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLength, NULL, NULL);
		*pAttrLength = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushPmStoreAttributeList
 *
 * @brief This function pushes PM Store Attributes to Buffer Stack based on 
 *        Attribute ID List
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pPMStore			:	Pointer to PM Store Object
 * @param pAttributeIdList	:	Pointer to AttributeIdList
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes PM Store Attributes to Buffer Stack based on Attribute
 * ID List
 ******************************************************************************/
static ERR_CODE PushPmStoreAttributeList(
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to Buffer Stack */
	PPMSTORE pPMStore,					/* [IN] Pointer to PM Store Object */          					       
	AttributeIdList *pAttributeIdList	/* [IN] Pointer to AttributeIdList */
)
{
    ERR_CODE err = ERROR_SUCCESS;   
    intu16 AttrLen;
    intu16 AttrLength = 0;
    intu16 AttrCount = 0;
    intu16 i;
    OID_Type *pOidType;
    pOidType = &pAttributeIdList->value[0];
    do
    {
        
        for(i = 0; i< ieee_ntohs(pAttributeIdList->count); i++)
        {
            if(ERROR_SUCCESS != (err = PushPmStoreAttribute(pBuffStack, 
				pPMStore, ieee_ntohs(pOidType[i]), &AttrLen)))
            {
                break;
            }
            AttrLength += AttrLen;
            AttrCount++;
            
        }
        if(ERROR_SUCCESS != err)
        {
            break;
        }
        if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, AttrCount, 
			AttrLength, &AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;
        if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, 
			pPMStore->MdcAttrIdHandle, &AttrLen)))
        {
        	break;;
        }
        AttrLength += AttrLen;
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, AttrLength, NULL, NULL);
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  PushPmStoreAttribute
 *
 * @brief This function pushes PM Store Attributes to Buffer Stack based on 
 *        Attribute ID
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pPMStore			:	Pointer to PM Store Object
 * @param AttributeId		:	Attribute ID
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes PM Store Attributes to Buffer Stack based on Attribute 
 * ID
 ******************************************************************************/
static ERR_CODE PushPmStoreAttribute(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */
	PPMSTORE pPMStore, 			/* [IN] Pointer to PM Store Object */     
	OID_Type AttributeId, 		/* [IN] Attribute ID */                   
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */ 
)
{
    ERR_CODE err;
    intu16 AttrLen;
    *pAttrLen = 0;
    switch (AttributeId)
    {
        case MDC_ATTR_CLEAR_TIMEOUT:
        {
            /* Push Clear Timeout */
        	if(ERROR_SUCCESS != (err = PushRelativeTime(pBuffStack, 
        	    pPMStore->MdcAttrClearTimeout, &AttrLen)))
        	{
        	    break;
        	}
        	*pAttrLen += AttrLen;
        	if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
        	    MDC_ATTR_CLEAR_TIMEOUT, AttrLen, &AttrLen)))
        	{
        	    break;
        	}
        	*pAttrLen += AttrLen;
            break;
        }
        case MDC_ATTR_NUM_SEG:
        {
            /* Push number of segments */
        	if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, 
        	    pPMStore->MdcAttrNumSeg, &AttrLen)))
        	{
        	    break;
        	}
        	*pAttrLen += AttrLen;
        	if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
        	    MDC_ATTR_NUM_SEG, AttrLen, &AttrLen)))
        	{
        	    break;
        	}
        	*pAttrLen += AttrLen;
            break;
        }
        case MDC_ATTR_TIME_PD_SAMP:
        {
            if(pPMStore->OptionalAttributeFlag & OPT_PMSTORE_SAMPLE_PRD)
            {        
                /* Push sample period */
            	if(ERROR_SUCCESS != (err = PushRelativeTime(pBuffStack, 
            	    pPMStore->MdcAttrTimePdSamp, &AttrLen)))
            	{
            	    break;
            	}
            	*pAttrLen += AttrLen;
            	if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
            	    MDC_ATTR_TIME_PD_SAMP, AttrLen, &AttrLen)))
            	{
            	    break;
            	}
            	*pAttrLen += AttrLen;
            }
            break;
        }   
        case MDC_ATTR_PM_STORE_LABEL_STRING:
        {
            if(pPMStore->OptionalAttributeFlag & OPT_PMSTORE_LABEL_STRING)
            {        
                /* Push PM Store Label String */
            	if(ERROR_SUCCESS != (err = Pushoctet_string(pBuffStack, 
            	    pPMStore->pMdcAttrPmStoreLabelString, &AttrLen)))
            	{
            	    break;
            	}
            	*pAttrLen += AttrLen;
            	if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
            	    MDC_ATTR_PM_STORE_LABEL_STRING, AttrLen, &AttrLen)))
            	{
            	    break;
            	}
            	*pAttrLen += AttrLen;
            }
            break;
        }
        case MDC_ATTR_OP_STAT:
        {
            /* Push Operational State */
        	if(ERROR_SUCCESS != (err = PushOperationalState(pBuffStack, 
        	    pPMStore->MdcAttrOpState, &AttrLen)))
        	{
        	    break;
        	}
        	*pAttrLen += AttrLen;

        	if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
        	    MDC_ATTR_OP_STAT, AttrLen, &AttrLen)))
        	{
        	    break;
        	}
        	*pAttrLen += AttrLen;
            break;
        }
        case MDC_ATTR_METRIC_STORE_USAGE_CNT:
        {
            if(pPMStore->OptionalAttributeFlag & OPT_PMSTORE_USAGE_CNT)
            {        
                /* Push store usage count */
            	if(ERROR_SUCCESS != (err = Pushintu32(pBuffStack, 
            	    pPMStore->MdcAttrMetricStoreUsageCnt, &AttrLen)))
            	{
            	    break;
            	}
            	*pAttrLen += AttrLen;
            	if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
            	    MDC_ATTR_METRIC_STORE_USAGE_CNT, AttrLen, &AttrLen)))
            	{
            	    break;
            	}
            	*pAttrLen += AttrLen;
            }
            break;
        }
        case MDC_ATTR_METRIC_STORE_CAPAC_CNT:
        {
            if(pPMStore->OptionalAttributeFlag & OPT_PMSTORE_CAPACITY_COUNT)
            {        
                /* Push store capacity count */
            	if(ERROR_SUCCESS != (err = Pushintu32(pBuffStack, 
            	    pPMStore->MdcAttrMetricStoreCapacCnt, &AttrLen)))
            	{
            	    break;
            	}
            	*pAttrLen += AttrLen;
            	if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
            	    MDC_ATTR_METRIC_STORE_CAPAC_CNT, AttrLen, &AttrLen)))
            	{
            	    break;
            	}
            	*pAttrLen += AttrLen;
            }
            break;
        }
        case MDC_ATTR_METRIC_STORE_SAMPLE_ALG:
        {
            /* Push Store Sample Algorithm */
        	if(ERROR_SUCCESS != (err = PushStoSampleAlg(pBuffStack, 
        	    pPMStore->MdcAttrMetricStoreSampleAlg, &AttrLen)))
        	{
        	    break;
        	}
        	*pAttrLen += AttrLen;
        	if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
        	    MDC_ATTR_METRIC_STORE_SAMPLE_ALG, AttrLen, &AttrLen)))
        	{
        	    break;
        	}
        	*pAttrLen += AttrLen;
            break;
        }
        case MDC_ATTR_PM_STORE_CAPAB:
        {
            /* Push PM store Capability */
        	if(ERROR_SUCCESS != (err = PushPMStoreCapab(pBuffStack, 
        	    pPMStore->MdcAttrPmStoreCapab, &AttrLen)))
        	{
        	    break;
        	}
        	*pAttrLen += AttrLen;
        	if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
        	    MDC_ATTR_PM_STORE_CAPAB, AttrLen, &AttrLen)))
        	{
        	    break;
        	}
        	*pAttrLen += AttrLen;
            break;
        }
        default:
            break;
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  PushAllPMStoreAttribute
 *
 * @brief This function pushes All PM Store Attributes to Buffer Stack 
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pPMStore			:	Pointer to PM Store Object
 * @param pAttrLen			:	Pointer to size of data pushed 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 *******************************************************************************
 * This function pushes All PM Store Attributes to Buffer Stack
 ******************************************************************************/
static ERR_CODE PushAllPMStoreAttribute(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */        
	PPMSTORE pPMStore, 			/* [IN] Pointer to PM Store Object */     
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */                 
)								
{
	ERR_CODE err;
	intu16 AttrCount = 0;
	intu16 AttrLen;
	*pAttrLen = 0;
    
    do
    {
        
        /* Push Clear Timeout */
    	if(ERROR_SUCCESS != (err = PushPmStoreAttribute(pBuffStack, 
    	    pPMStore, MDC_ATTR_CLEAR_TIMEOUT, &AttrLen)))
    	{
    	    break;
    	}
    	AttrCount++;
    	*pAttrLen += AttrLen;

        /* Push number of segments */
    	if(ERROR_SUCCESS != (err = PushPmStoreAttribute(pBuffStack, 
    	    pPMStore, MDC_ATTR_NUM_SEG, &AttrLen)))
    	{
    	    break;
    	}
    	AttrCount++;
    	*pAttrLen += AttrLen;

        if(pPMStore->OptionalAttributeFlag & OPT_PMSTORE_SAMPLE_PRD)
        {        
            /* Push sample period */
        	if(ERROR_SUCCESS != (err = PushPmStoreAttribute(pBuffStack, 
        	    pPMStore, MDC_ATTR_TIME_PD_SAMP, &AttrLen)))
        	{
        	    break;
        	}
        	AttrCount++;
        	*pAttrLen += AttrLen;
        }
        if(pPMStore->OptionalAttributeFlag & OPT_PMSTORE_LABEL_STRING)
        {        
            /* Push PM Store Label String */
        	if(ERROR_SUCCESS != (err = PushPmStoreAttribute(pBuffStack, 
        	    pPMStore, MDC_ATTR_PM_STORE_LABEL_STRING, &AttrLen)))
        	{
        	    break;
        	}
        	AttrCount++;
        	*pAttrLen += AttrLen;
        }
        /* Push Operational State */
    	if(ERROR_SUCCESS != (err = PushPmStoreAttribute(pBuffStack, 
    	    pPMStore, MDC_ATTR_OP_STAT, &AttrLen)))
    	{
    	    break;
    	}
    	AttrCount++;
    	*pAttrLen += AttrLen;

        if(pPMStore->OptionalAttributeFlag & OPT_PMSTORE_USAGE_CNT)
        {        
            /* Push store usage count */
        	if(ERROR_SUCCESS != (err = PushPmStoreAttribute(pBuffStack, 
        	    pPMStore, MDC_ATTR_METRIC_STORE_USAGE_CNT, &AttrLen)))
        	{
        	    break;
        	}
        	AttrCount++;
        	*pAttrLen += AttrLen;
        }

        if(pPMStore->OptionalAttributeFlag & OPT_PMSTORE_CAPACITY_COUNT)
        {        
            /* Push store capacity count */
        	if(ERROR_SUCCESS != (err = PushPmStoreAttribute(pBuffStack, 
        	    pPMStore, MDC_ATTR_METRIC_STORE_CAPAC_CNT, &AttrLen)))
        	{
        	    break;
        	}
        	AttrCount++;
        	*pAttrLen += AttrLen;
        }

        /* Push Store Sample Algorithm */
    	if(ERROR_SUCCESS != (err = PushPmStoreAttribute(pBuffStack, 
    	    pPMStore, MDC_ATTR_METRIC_STORE_SAMPLE_ALG, &AttrLen)))
    	{
    	    break;
    	}
    	AttrCount++;
    	*pAttrLen += AttrLen;

        /* Push PM store Capability */
    	if(ERROR_SUCCESS != (err = PushPmStoreAttribute(pBuffStack, 
    	    pPMStore, MDC_ATTR_PM_STORE_CAPAB, &AttrLen)))
    	{
    	    break;
    	}
    	AttrCount++;
    	*pAttrLen += AttrLen;

		/* Push Attribute Count and Length */
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, AttrCount, 
			*pAttrLen, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;

        /* Push PM store Handle */
    	if(ERROR_SUCCESS != (err = PushHANDLE(pBuffStack, 
    	    pPMStore->MdcAttrIdHandle, &AttrLen)))
    	{
    	    break;
    	}
    	*pAttrLen += AttrLen;
    }while(0);
    
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
    }
	return err;
}
/* Application Interface */
/**************************************************************************//*!
 *
 * @name  AgentDimDeInitialize
 *
 * @brief This function handles DIM de-initialize
 *
 * @return 
 *******************************************************************************
 * This function initializes DIM 
 ******************************************************************************/
void AgentDimDeInitialize(void)
{
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    PCONFIGURATION pConf = NULL;
    if(pMds != NULL)
    {
        intu16 i;
        pConf = &(*(pMds->aConfiguration))[pMds->ConfigSelected];;
        /* Clear all Scanner Timers */
        
        for(i = 0; i < pConf->EpiScanCount; i++)
        {
            PEPICFGSCANNER pEpiScanner = &(*pConf->pEpiScanner)[i];
            pEpiScanner->pCfgScanner->pScanner->MdcAttrOpStat = OS_DISABLED;
            if(pEpiScanner->TimerIndex != (intu8)INVALID_TIME_COUNT)
            {
                (void)PHD_Remove_Timer(&pEpiScanner->TimerIndex);
            }
        }
        
        for(i = 0; i < pConf->PeriScanCount; i++)
        {
            PPERICFGSCANNER pPeriScanner = &(*pConf->pPeriScanner)[i];
            pPeriScanner->pCfgScanner->pScanner->MdcAttrOpStat = OS_DISABLED;
            if(pPeriScanner->TimerIndex != (intu8)INVALID_TIME_COUNT)
            {
                (void)PHD_Remove_Timer(&pPeriScanner->TimerIndex);
            }
        }
            
        pMds->ConfigSelected = 0;
    }
}
/**************************************************************************//*!
 *
 * @name  GetPHDAssocInfo
 *
 * @brief This function returns pointer to the PHD association information 
 *        structure
 *
 * @return Pointer to the PHD association information structure
 *******************************************************************************
 * This function returns the pointer to the PHD association information  
 ******************************************************************************/
PhdAssociationInformation* GetPHDAssocInfo(void)
{
    PhdAssociationInformation* gPhdAssocInfo = NULL;
    DataProtoList* pDataProtoList = NULL;
    Ieee11073GetAppCallback()(IEEE11073_GET_DATAPROTO, &pDataProtoList);
    if(pDataProtoList != NULL)
        gPhdAssocInfo = (PhdAssociationInformation*)&pDataProtoList->value[0].data_proto_info.value[0];
    return gPhdAssocInfo;
}
#define PROTO_VERSION   0
#define ENCODING_RULES  1
#define NOMEN_VERSION   2
#define FUNCT_UNITS     3
#define SYS_TYPE        4
#define SYS_ID          5
#define CONFIG_ID       6
#define DATA_REQMODE    7
#define ATTR_LIST       8

/**************************************************************************//*!
 *
 * @name  GetPhdAssocFieldPtr
 *
 * @brief This function returns pointer to field in PHD Association 
 *        information structure
 *
 * @param ID		:	Field ID
 *
 * @return PhdAssociationInformation*
 *******************************************************************************
 * This function returns the pointer to the PHD association information  
 ******************************************************************************/
void* GetPhdAssocFieldPtr(
    intu8 ID            /* [IN] Field ID */
)
{
    PhdAssociationInformation* pPhdAssocInfo = GetPHDAssocInfo();
    if(pPhdAssocInfo == NULL)
        return NULL;
    switch(ID)
    {
        case PROTO_VERSION:
            return (void*)&pPhdAssocInfo->protocolVersion;
        case ENCODING_RULES:
            return (void*)&pPhdAssocInfo->encodingRules;
        case NOMEN_VERSION:
            return (void*)&pPhdAssocInfo->nomenclatureVersion;
        case FUNCT_UNITS:
            return (void*)&pPhdAssocInfo->functionalUnits;
        case SYS_TYPE:
            return (void*)&pPhdAssocInfo->systemType;
        case SYS_ID:
            return (void*)&pPhdAssocInfo->system_id;
        case CONFIG_ID:
        case DATA_REQMODE:
        case ATTR_LIST:
            {
                intu8* pTemp = (intu8*)&pPhdAssocInfo->system_id;
                pTemp += OCTET_STRING_HEADER_SIZE + ieee_ntohs(((octet_string*)pTemp)->length);
                switch(ID)
                {
                    case CONFIG_ID:
                        return (void*)pTemp;
                    case DATA_REQMODE:
                        return (void*)(pTemp + sizeof(intu16));
                    case ATTR_LIST:
                        return (void*)(pTemp + sizeof(intu16)+sizeof(DataReqModeCapab));
                }
            }
        default:
            return NULL;
    }
}
/**************************************************************************//*!
 *
 * @name  AgentDimInitialize
 *
 * @brief This function initializes DIM 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: Always
 *******************************************************************************
 * This function initializes DIM 
 ******************************************************************************/
ERR_CODE AgentDimInitialize(void)
{
    PMDS pMds;
    PCONFIGURATION pConf = NULL;
    // Initialize Dim for Selected Configuration.
    Ieee11073GetAppCallback()(IEEE11073_INITIALIZE_DIM, &pDIM);
    pMds = GetMdsHandle(MDS_HANDLE);
    if(pMds != NULL)
    {
        intu16 i;
        DataReqModeCapab* pDataReqModeCapab = GetPhdAssocFieldPtr(DATA_REQMODE);
        intu16 ScannerCount;    
        pConf = &(*(pMds->aConfiguration))[pMds->ConfigSelected];
        if(pDataReqModeCapab == NULL)
        {
            return ERROR_DIM_INIT_FAILED;
        }
        if(!(pDataReqModeCapab->data_req_mode_flags & DATA_REQ_SUPP_INIT_AGENT))
        {

          ScannerCount = (intu16)(pConf->EpiScanCount + pConf->PeriScanCount);

          if(ScannerCount == 0)
          {
                pDIM = NULL;
                Ieee11073GetAppCallback()(IEEE11073_INITIALIZE_DIM_FAILED, NULL);

                return ERROR_DIM_INIT_FAILED;
          }
        }
        /* Clear all Scanner's operational State */
        for(i = 0; i < pConf->EpiScanCount; i++)
        {
            PEPICFGSCANNER pEpiScanner = &(*pConf->pEpiScanner)[i];
            pEpiScanner->pCfgScanner->pScanner->MdcAttrOpStat = OS_DISABLED;
            pEpiScanner->TimerIndex = (intu8)INVALID_TIME_COUNT;
        }
        for(i = 0; i < pConf->PeriScanCount; i++)
        {
            PPERICFGSCANNER pPeriScanner = &(*pConf->pPeriScanner)[i];
            pPeriScanner->pCfgScanner->pScanner->MdcAttrOpStat = OS_DISABLED;
            pPeriScanner->TimerIndex = (intu8)INVALID_TIME_COUNT;
        }
    }
    pMds->ConfigSelected = 0;
    return ERROR_SUCCESS;
}

/**************************************************************************//*!
 *
 * @name  GetMetricAttributeSize
 *
 * @brief This function returns Metric Attribute Size
 *
 * @param pMetric		:	Pointer to Metric Object
 *
 * @return Metric Attribute Size
 ******************************************************************************
 * This function returns Metric Attribute Size
 *****************************************************************************/
static intu16 GetMetricAttributeSize(
	PMETRIC pMetric		/* [IN] Pointer to Metric Object */
)
{
    intu16 Size;
    Size = sizeof(HANDLE) + ATTR_VAL_MAP_HEADER_SIZE + /* Size of Handle */    
           ATTR_HEADER_SIZE + sizeof(TYPE) +           /* Size of TYPE */
           ATTR_HEADER_SIZE + sizeof(MetricSpecSmall); /* size of Metric Struct Small */
    
    if((pMetric->OptionalAttributeFlag & OPT_MET_SUPP_TYPES) &&
       (pMetric->pMdcAttrSupplementalTypes != NULL)
      ) 
    {
        Size += ATTR_HEADER_SIZE + 
            pMetric->pMdcAttrSupplementalTypes->length + 
            SUPPLEMENTAL_TYPE_LIST_HEADER_SIZE;
    }
    if(pMetric->OptionalAttributeFlag & OPT_MET_STRUCT_SMALL)
    {
        Size += ATTR_HEADER_SIZE + sizeof(MetricStructureSmall);
    }
    if(pMetric->OptionalAttributeFlag & OPT_MET_MSMT_STAT)
    {
        Size += ATTR_HEADER_SIZE + sizeof(MeasurementStatus);
    }
    if(pMetric->OptionalAttributeFlag & OPT_MET_ID_PHYS)
    {
        if((pMetric->MetricIdPhysioList.ID_type == METRIC_ID) &&
           (pMetric->MetricIdPhysioList.u.pMdcAttrIdPhysio != NULL)
          )
        {
            Size += ATTR_HEADER_SIZE +  sizeof(OID_Type);
        }
        else
        {
           /* Metric ID List */
          if((pMetric->MetricIdPhysioList.ID_type == METRIC_ID_LIST) &&
             (pMetric->MetricIdPhysioList.u.pMdcAttrIdPhysioList != NULL)
            ) 
          {
            Size += ATTR_HEADER_SIZE + METRIC_ID_LIST_HEADER_SIZE +
              pMetric->MetricIdPhysioList.u.pMdcAttrIdPhysioList->length;
          }
        }
    }    
    if(pMetric->OptionalAttributeFlag & OPT_MET_ID_PART)
    {
        Size += ATTR_HEADER_SIZE + sizeof(NomPartition);
    }    
    if(pMetric->OptionalAttributeFlag & OPT_MET_UNIT_CODE)
    {
        Size += ATTR_HEADER_SIZE + sizeof(OID_Type);
    }
    if(pMetric->OptionalAttributeFlag & OPT_MET_SRC_HNDL_REF)
    {
        Size += ATTR_HEADER_SIZE + sizeof(HANDLE);
    }
    if((pMetric->OptionalAttributeFlag & OPT_MET_ID_LABEL) &&
       (pMetric->pMdcAttrIdLabelString != NULL)
      ) 
    {
        Size += ATTR_HEADER_SIZE + OCTET_STRING_HEADER_SIZE + 
            pMetric->pMdcAttrIdLabelString->length;
    }
    if((pMetric->OptionalAttributeFlag & OPT_MET_UNIT_LABEL) &&
       (pMetric->pMdcAttrUnitLabelString != NULL)
      ) 
    {
        Size += ATTR_HEADER_SIZE + OCTET_STRING_HEADER_SIZE + 
            pMetric->pMdcAttrUnitLabelString->length;
    }
    if(pMetric->OptionalAttributeFlag & OPT_MET_MSMT_ACTIVE)
    {
        Size += ATTR_HEADER_SIZE +  sizeof(FLOAT_Type);
    }
    if((pMetric->OptionalAttributeFlag & OPT_MET_ATTRVALMAP) &&
       (pMetric->pMdcAttrAttributeValMap != NULL)
      ) 
    {
        Size += ATTR_HEADER_SIZE + ATTR_VAL_MAP_HEADER_SIZE +
            pMetric->pMdcAttrAttributeValMap->length;
    }
    return Size;
}

/**************************************************************************//*!
 *
 * @name  GetNumericAttributeSize
 *
 * @brief This function returns Numeric Attribute Size
 *
 * @param pNumeric		:	Pointer to Numeric Object
 *
 * @return Numeric Attribute Size
 ******************************************************************************
 * This function returns Numeric Attribute Size
 *****************************************************************************/
static intu16 GetNumericAttributeSize(
	PNUMERIC pNumeric		/* [IN] Pointer to Numeric Object */
)
{
    intu16 Size;
    Size = (intu16)(GetMetricAttributeSize(pNumeric->pMetric) + 
        OBJECT_CLASS_SIZE);
    if(pNumeric->OptionalAttributeFlag & OPT_NUMERIC_ACCURACY)
    {
        Size += sizeof(FLOAT_Type) + ATTR_HEADER_SIZE;
    }
    return Size;
}

/**************************************************************************//*!
 *
 * @name  GetRtsaAttributeSize
 *
 * @brief This function returns RTSA Attribute Size
 *
 * @param pRtsa		:	Pointer to RTSA Object
 *
 * @return RTSA Attribute Size
 ******************************************************************************
 * This function returns RTSA Attribute Size
 *****************************************************************************/
static intu16 GetRtsaAttributeSize(
	PRTSA pRtsa		/* [IN] Pointer to RTSA Object */
)
{
    intu16 Size;
    Size = (intu16)(GetMetricAttributeSize(pRtsa->pMetric) + 
        OBJECT_CLASS_SIZE);
    Size += sizeof(RelativeTime) + ATTR_HEADER_SIZE; 
    switch(pRtsa->ScaleRangeSpec.scale_range_spec_type)
    {
        case RTSA_SCALE_RANGE_SPEC_8:
            Size += sizeof(ScaleRangeSpec8) + ATTR_HEADER_SIZE;
            break;
        case RTSA_SCALE_RANGE_SPEC_16:
            Size += sizeof(ScaleRangeSpec16) + ATTR_HEADER_SIZE;
            break;
        case RTSA_SCALE_RANGE_SPEC_32:
            Size += sizeof(ScaleRangeSpec32) + ATTR_HEADER_SIZE;
            break;
    }
#if 0
    Size += pRtsa->pMdcAttrSimpSaObsVal->length + OCTET_STRING_HEADER_SIZE + 
        ATTR_HEADER_SIZE; 
#endif        
    Size += sizeof(SaSpec) + ATTR_HEADER_SIZE;
    return Size;
}


/**************************************************************************//*!
 *
 * @name  GetEnumAttributeSize
 *
 * @brief This function returns Enumeration Attribute Size
 *
 * @param pEnumeration		:	Pointer to Enumeration Object
 *
 * @return Enumeration Attribute Size
 ******************************************************************************
 * This function returns Enumeration Attribute Size
 *****************************************************************************/
static intu16 GetEnumAttributeSize(
	PENUMERATION pEnumeration	/* [IN] Pointer to Enumeration Object */
)
{
	intu16 size = (intu16)(GetMetricAttributeSize(pEnumeration->pMetric) + OBJECT_CLASS_SIZE);

	if(pEnumeration->OptionalAttributeFlag & OPT_ENUM_OBS_VAL_PART)
	{
		size += sizeof(NomPartition) + ATTR_HEADER_SIZE;
	}
	return size;
}


/**************************************************************************//*!
 *
 * @name  GetEpiCfgScannerAttributeSize
 *
 * @brief This function returns Episoidic Scanner Attribute Size
 *
 * @param pEpiScanner		:	Pointer to Episoidic Scanner Object
 *
 * @return Episoidic Scanner Attribute Size
 ******************************************************************************
 * This function returns Episoidic Scanner Attribute Size
 *****************************************************************************/
intu16 GetEpiCfgScannerAttributeSize(
	PEPICFGSCANNER pEpiScanner	/* [IN] Pointer to Episoidic Scanner Object */
)
{
	intu16 Size;
	Size = (intu16)(GetCfgScannerAttributeSize(pEpiScanner->pCfgScanner) + 
	    OBJECT_CLASS_SIZE);

	if(pEpiScanner->OptionalAttributeFlag & OPT_EPISCAN_MIN_RPT_INT)
	{
		Size += sizeof(RelativeTime) + ATTR_HEADER_SIZE;
	}
    return Size;   
}

/**************************************************************************//*!
 *
 * @name  GetPeriCfgScannerAttributeSize
 *
 * @brief This function returns Periodic Scanner Attribute Size
 *
 * @param pPeriScanner		:	Pointer to Periodic Scanner Object
 *
 * @return Periodic Scanner Attribute Size
 ******************************************************************************
 * This function returns Periodic Scanner Attribute Size
 *****************************************************************************/
intu16 GetPeriCfgScannerAttributeSize(
	PPERICFGSCANNER pPeriScanner  /* [IN] Pointer to Periodic Scanner Object */
)
{
	intu16 Size;
	Size = (intu16)(GetCfgScannerAttributeSize(pPeriScanner->pCfgScanner) + 
	    OBJECT_CLASS_SIZE);
	Size += sizeof(RelativeTime) + ATTR_HEADER_SIZE;
    return Size;   
}

/**************************************************************************//*!
 *
 * @name  GetScannerAttributeSize
 *
 * @brief This function returns Scanner Attribute Size
 *
 * @param pScanner		:	Pointer to Scanner Object
 *
 * @return Scanner Attribute Size
 ******************************************************************************
 * This function returns Scanner Attribute Size
 *****************************************************************************/
static intu16 GetScannerAttributeSize(
	PSCANNER pScanner	/* [IN] Pointer to Scanner Object */
)
{
	intu16 Size;
	Size  = sizeof(HANDLE) + ATTR_VAL_MAP_HEADER_SIZE + 
	        sizeof(OperationalState) + ATTR_HEADER_SIZE;

	Size += pScanner->pMdcAttrScanHandleAttrValMap->length + 
		SCAN_HANDLE_ATTR_VAL_MAP_HEADER_SIZE +
		ATTR_HEADER_SIZE;
	return Size;
}

/**************************************************************************//*!
 *
 * @name  GetCfgScannerAttributeSize
 *
 * @brief This function returns Configuration Scanner Attribute Size
 *
 * @param pCfgScanner		:	Pointer to Configuration Scanner Object
 *
 * @return Configuration Scanner Attribute Size
 ******************************************************************************
 * This function returns Configuration Scanner Attribute Size
 *****************************************************************************/
static intu16 GetCfgScannerAttributeSize(
	PCFGSCANNER pCfgScanner	/* [IN] Pointer to Configuration Scanner Object */
)
{
	intu16 Size;
	Size = GetScannerAttributeSize(pCfgScanner->pScanner);
	Size += sizeof(ConfirmMode) + ATTR_HEADER_SIZE;

	if(pCfgScanner->OptionalAttributeFlag & OPT_CFGSCAN_CONFIRM_TIMEOUT)
	{
		Size += sizeof(RelativeTime) + ATTR_HEADER_SIZE;
	}
	
	if(pCfgScanner->OptionalAttributeFlag & OPT_CFGSCAN_TRANS_WINDOW)
	{
		Size += sizeof(intu16) + ATTR_HEADER_SIZE;
	}
	return Size;
}


/**************************************************************************//*!
 *
 * @name  GetPmStoreAttributeSize
 *
 * @brief This function returns PM Store Attribute Size based on Attribute ID
 *
 * @param pPmStore		:	Pointer to PM Store Object
 * @param AttributeId	:	Attribute ID
 *
 * @return PM Store Attribute Size
 ******************************************************************************
 * This function returns PM Store Attribute Size based on Attribute ID
 *****************************************************************************/
intu16 GetPmStoreAttributeSize(
	PPMSTORE pPmStore,		/* [IN] Pointer to PM Store Object */
	OID_Type AttributeId	/* [IN] Attribute ID */
)
{
    intu16 size = 0;
    switch(AttributeId)
    {
        case MDC_ATTR_ID_HANDLE:
        case MDC_ATTR_OP_STAT:
        case MDC_ATTR_NUM_SEG:
        case MDC_ATTR_PM_STORE_CAPAB:
        case MDC_ATTR_METRIC_STORE_SAMPLE_ALG:
            size = sizeof(intu16);
            break;
        case MDC_ATTR_CLEAR_TIMEOUT:
            size = sizeof(RelativeTime);
            break;
        case MDC_ATTR_TIME_PD_SAMP:
            if(pPmStore->OptionalAttributeFlag & OPT_PMSTORE_SAMPLE_PRD)
            {
                size = sizeof(RelativeTime);
            } 
            break;
        case MDC_ATTR_METRIC_STORE_CAPAC_CNT:
            if(pPmStore->OptionalAttributeFlag & OPT_PMSTORE_CAPACITY_COUNT)
            {
                size = sizeof(intu32);
            }
            break;
        case MDC_ATTR_METRIC_STORE_USAGE_CNT:
            if(pPmStore->OptionalAttributeFlag & OPT_PMSTORE_USAGE_CNT)
            {
                size = sizeof(intu32);
            }
            break;
        case MDC_ATTR_PM_STORE_LABEL_STRING:
            if(pPmStore->OptionalAttributeFlag & OPT_PMSTORE_LABEL_STRING)
            {
                size = (intu16)(OCTET_STRING_HEADER_SIZE  + 
                    pPmStore->pMdcAttrPmStoreLabelString->length);
            }
            break;
        default:
            size = 0;
            break;
    }
    if(size != 0)
    {
        size += ATTR_HEADER_SIZE;
    }
    return size;
}

/**************************************************************************//*!
 *
 * @name  PMStoreGetObjectAttributeSize
 *
 * @brief This function returns PM Store Attribute Size 
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pPmStore			:	Pointer to PM Store Object
 *
 * @return PM Store Attribute Size
 ******************************************************************************
 * This function returns PM Store Attribute Size
 *****************************************************************************/
intu16 PMStoreGetObjectAttributeSize(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */
	PPMSTORE pPmStore			/* [IN] Pointer to PM Store Object */
)
{
    AttributeIdList *pAttributeIdList = GetStackTop(pBuffStack);
    intu16 i;
    intu16 OutBuffSize = 0;
    
    /* Compute Size */
    if(ieee_ntohs(pAttributeIdList->count) == 0)
    {
        OutBuffSize = GetPmStoreAllAttributeSize(pPmStore);
    } 
    else
    {
        OID_Type *pOidType;
        pOidType = &pAttributeIdList->value[0];
        for(i = 0; i < pAttributeIdList->count; i++)
        {
            OutBuffSize += GetPmStoreAttributeSize(pPmStore, ieee_ntohs(pOidType[i]));
        }
        OutBuffSize += GetPmStoreAttributeSize(pPmStore, MDC_ATTR_ID_HANDLE);
    }
    OutBuffSize += GetDataApduHeaderSize();
    return OutBuffSize;     
}

/**************************************************************************//*!
 *
 * @name  GetPmStoreAllAttributeSize
 *
 * @brief This function returns All PM Store Attribute Size 
 *
 * @param pPmStore			:	Pointer to PM Store Object
 *
 * @return All PM Store Attribute Size
 ******************************************************************************
 * This function returns All PM Store Attribute Size
 *****************************************************************************/
static intu16 GetPmStoreAllAttributeSize(
	PPMSTORE pPmStore	/* [IN] Pointer to PM Store Object */
)
{
    intu16 size = 0;
    
    size += GetPmStoreAttributeSize(pPmStore, MDC_ATTR_ID_HANDLE);
    size += GetPmStoreAttributeSize(pPmStore, MDC_ATTR_PM_STORE_CAPAB);
    size += GetPmStoreAttributeSize(pPmStore, MDC_ATTR_METRIC_STORE_SAMPLE_ALG);
    size += GetPmStoreAttributeSize(pPmStore, MDC_ATTR_OP_STAT);
    size += GetPmStoreAttributeSize(pPmStore, MDC_ATTR_NUM_SEG);
    size += GetPmStoreAttributeSize(pPmStore, MDC_ATTR_CLEAR_TIMEOUT);
    
    if(pPmStore->OptionalAttributeFlag & OPT_PMSTORE_CAPACITY_COUNT)
    {
        size += GetPmStoreAttributeSize(pPmStore, 
			MDC_ATTR_METRIC_STORE_CAPAC_CNT);
    }
    if(pPmStore->OptionalAttributeFlag & OPT_PMSTORE_USAGE_CNT)
    {
        size += GetPmStoreAttributeSize(pPmStore, 
			MDC_ATTR_METRIC_STORE_USAGE_CNT);
    }
       
    if(pPmStore->OptionalAttributeFlag & OPT_PMSTORE_LABEL_STRING)
    {
        size += GetPmStoreAttributeSize(pPmStore, 
			MDC_ATTR_PM_STORE_LABEL_STRING);
    }
    if(pPmStore->OptionalAttributeFlag & OPT_PMSTORE_SAMPLE_PRD)
    {
        size += GetPmStoreAttributeSize(pPmStore, 
			MDC_ATTR_TIME_PD_SAMP);
    }
    return size;
}


/**************************************************************************//*!
 *
 * @name  GetConfigrationEventReportSize
 *
 * @brief This function returns Configuration Event Report Size
 *
 * @param pConfiguration	:	Pointer to CONFIGURATION
 *
 * @return Configuration Event Report Size
 ******************************************************************************
 * This function returns Configuration Event Report Size
 *****************************************************************************/
intu16 GetConfigrationEventReportSize(
	PCONFIGURATION pConfiguration	/* [IN] Pointer to CONFIGURATION */
)
{
    intu16 i = 0;
    intu16 Size = 0;
    
    /* Compute Numeric Class Attribute Size */
    for(i= 0; i < pConfiguration->NumericCount; i++)
    {
        PNUMERIC pNumeric = &(*pConfiguration->pNumeric)[i];
        Size +=  GetNumericAttributeSize(pNumeric);      
    }
    /* Compute RT-SA Class Attribute Size */
    for(i= 0; i < pConfiguration->RtsaCount; i++)
    {
        PRTSA pRtsa = &(*pConfiguration->pRtSa)[i];
        Size +=  GetRtsaAttributeSize(pRtsa);      
    }
    /* Compute Enumeration Class Attribute Size */
    for(i= 0; i < pConfiguration->EnumCount; i++)
    {
        PENUMERATION pEnumeration = &(*pConfiguration->pEnumeration)[i];
        Size +=  GetEnumAttributeSize(pEnumeration);      
    }
    /* Compute Episoidic Configuration Scanner Class Attribute Size */
    for(i= 0; i < pConfiguration->EpiScanCount; i++)
    {
        PEPICFGSCANNER pEpiScanner = &(*pConfiguration->pEpiScanner)[i];
        Size +=  GetEpiCfgScannerAttributeSize(pEpiScanner);      
    }
    /* Compute Periodic Configuration Scanner Class Attribute Size */
    for(i= 0; i < pConfiguration->PeriScanCount; i++)
    {
        PPERICFGSCANNER pPeriScanner = &(*pConfiguration->pPeriScanner)[i];
        Size +=  GetPeriCfgScannerAttributeSize(pPeriScanner);      
    }
    /* Compute Periodic Configuration Scanner Class Attribute Size */
    for(i= 0; i < pConfiguration->PmStoreCount; i++)
    {
        PPMSTORE pPmStore = &(*pConfiguration->pPmStore)[i];
        Size +=  GetPmStoreAllAttributeSize(pPmStore);
        Size += sizeof(OID_Type);   /* Size of Object Class */      
    }
    return Size;        
}
/**************************************************************************//*!
 *
 * @name  PushConfigurationEventReport
 *
 * @brief This function pushes the configuration event report onto the Buffer 
 *		  Stack
 *
 * @param bFlag			:	if True, Push Full configuration event report
 * @param pBuffStack	:	Pointer to the Buffer Stack
 * @param pMds			:	Pointer to the MDS object
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INSUFFICIENT_MEMORY		: if Unable to push data in Buffer 
 *										  Stack
 ******************************************************************************
 * This function pushes the configuration event report onto the top of the 
 * Buffer Stack
 *****************************************************************************/
ERR_CODE PushConfigurationEventReport(
	intu8 bFlag,				/* [IN] if True, Send Push configuration 
									event report */				 
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to the Buffer Stack */
	PMDS pMds					/* [IN] Pointer to the MDS object */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    PCONFIGURATION pConfiguration = 
        &(*(pMds->aConfiguration))[pMds->ConfigSelected];
    intu16 AttrLen;
    intu16 AttrLength = 0;
    intu16 ClassCount = 0;
    intu16 i = 0;
    do
    {
        if(bFlag == TRUE)
        {            
            /* Push PM Store Attributes */
            for(i = 0; i < pConfiguration->PmStoreCount; i++)
            {
                PPMSTORE pPmStore = &(*(pConfiguration->pPmStore))[i];
                if(ERROR_SUCCESS != (err = 
                    PushAllPMStoreAttribute(pBuffStack, pPmStore, &AttrLen)))
                {
                    break;
                }
                AttrLength += AttrLen;

        		if(ERROR_SUCCESS != (err = PushObjectClass(pBuffStack, 
        		    MDC_MOC_VMO_PMSTORE, &AttrLen)))
        		{
        			break;
        		}
        		AttrLength += AttrLen;
            }
            if(ERROR_SUCCESS != err)
            {
                break;
            }
            ClassCount += pConfiguration->PmStoreCount;

            /* Push Periodic Scanner Attributes */
            for(i = 0; i < pConfiguration->PeriScanCount; i++)
            {
                PPERICFGSCANNER pPeriScanner = 
                    &(*(pConfiguration->pPeriScanner))[i];
                if(ERROR_SUCCESS != (err = 
                    PushPERICFGScannerAttribute(pBuffStack, 
                    pPeriScanner, &AttrLen)))
                {
                    break;
                }
                AttrLength += AttrLen;
            }
            if(ERROR_SUCCESS != err)
            {
                break;
            }
            ClassCount += pConfiguration->PeriScanCount;

            /* Push Episoidic Scanner Attributes */
            for(i = 0; i < pConfiguration->EpiScanCount; i++)
            {
                PEPICFGSCANNER pEpiScanner = 
                    &(*(pConfiguration->pEpiScanner))[i];
                if(ERROR_SUCCESS != (err = 
                    PushEPICFGScannerAttribute(pBuffStack, 
                    pEpiScanner, &AttrLen)))
                {
                    break;
                }
                AttrLength += AttrLen;
            }
            if(ERROR_SUCCESS != err)
            {
                break;
            }
            ClassCount += pConfiguration->EpiScanCount;
            
            /* Push Enumeration Attributes */
            for(i = 0; i < pConfiguration->EnumCount; i++)
            {
                PENUMERATION pEnumeration = 
                    &(*(pConfiguration->pEnumeration))[i];
                if(ERROR_SUCCESS != (err = 
                    PushEnumerationAttribute(pBuffStack, 
                    pEnumeration, &AttrLen)))
                {
                    break;
                }
                AttrLength += AttrLen;
            }
            if(ERROR_SUCCESS != err)
            {
                break;
            }
            ClassCount += pConfiguration->EnumCount;
            
            /* Push RT-SA Attributes */
            for(i = 0; i < pConfiguration->RtsaCount; i++)
            {
                PRTSA pRtSa = &(*(pConfiguration->pRtSa))[i];
                if(ERROR_SUCCESS != (err = 
                    PushRTSAAttribute(pBuffStack, pRtSa, &AttrLen)))
                {
                    break;
                }
                AttrLength += AttrLen;
            }
            if(ERROR_SUCCESS != err)
            {
                break;
            }
            ClassCount += pConfiguration->RtsaCount;
            
            /* Push Numeric Attributes */
            for(i =0; i < pConfiguration->NumericCount; i++)
            {
                PNUMERIC pNumeric = &(*(pConfiguration->pNumeric))[i];
                if(ERROR_SUCCESS != (err = 
                    PushNumericAttribute(pBuffStack, pNumeric, &AttrLen)))
                {
                    break;
                }
                AttrLength += AttrLen;
            }
            if(ERROR_SUCCESS != err)
            {
                break;
            }
            ClassCount += pConfiguration->NumericCount;  
        }
        if(ERROR_SUCCESS != (err = PushConfigReportHeader(pBuffStack, 
            pConfiguration->ConfigurationID, ClassCount, &AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;        
        if(ERROR_SUCCESS != (err = PushConfConfigDataRptHeader(pBuffStack, 
            pMds->MdcAttrIdHandle, 0xFFFFFFFF, &AttrLen)))
        {
            break;
        }
        AttrLength += AttrLen;
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, AttrLength, NULL, NULL);
    }
    return err;
}
/**************************************************************************//*!
 *
 * @name  AgentDimSendNextConfigEventReport
 *
 * @brief This function sends the next configuration, if available
 *
 * @param bFlag		:	if True, Send Full configuration event report
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INSUFFICIENT_MEMORY		: if Unable to push data in Buffer 
 *										  Stack
 *			ERROR_NO_MORE_CONFIGURATIONS	: if the agent has already tried all 
 *										  the configurations
 ******************************************************************************
 * This function is called when the manager rejects a configuration. The agent
 * then sends the next configuration, if present
 *****************************************************************************/
ERR_CODE AgentDimSendNextConfigEventReport(
	intu8 bFlag		/* [IN/OUT] if True, Send Full configuration event report*/ 
)
{
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    
    if(pMds->ConfigSelected < (pMds->ConfigCount - 1))
    {      
     pMds->ConfigSelected++;
    }
    else
    {              
       return ERROR_NO_MORE_CONFIGURATIONS;
    }
    
    return AgentDimSendConfigEventReport(bFlag);
}
/**************************************************************************//*!
 *
 * @name  AgentDimSendConfigEventReport
 *
 * @brief This function sends configuration event report
 *
 * @param bFlag		:	if True, Send Full configuration event report
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                    Stack
 ******************************************************************************
 * This function gets the size of the configuration event report, creates and 
 * sends it.
 *****************************************************************************/
ERR_CODE AgentDimSendConfigEventReport(
	intu8 bFlag		/* [IN/OUT] if True, Send Full configuration event report*/ 
)
{
    ERR_CODE err;
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    PCONFIGURATION pConfiguration = 
        &(*(pMds->aConfiguration))[pMds->ConfigSelected];
    PTR_BUFFSTACK pBuffStack;
    intu16 OutBuffSize;

    if((pConfiguration->ConfigurationID >= STANDARD_CONFIG_START) &&
        (pConfiguration->ConfigurationID <= STANDARD_CONFIG_END))
    {
        OutBuffSize = GetConfigReportHeaderSize();
    } 
    else 
    {
        bFlag = TRUE;
    }

    if(bFlag == TRUE)
    {
        OutBuffSize = (intu16)(GetConfigrationEventReportSize(pConfiguration) +
            GetConfigReportHeaderSize());
    } 
    pBuffStack = StackInit(OutBuffSize);
    do
    {
    
        if(ERROR_SUCCESS != (err = PushConfigurationEventReport(bFlag, 
            pBuffStack, pMds)))
        {
            break;
        }
        err = ObjectSendEventReport(TRUE, pBuffStack);    
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)StackDeInit(pBuffStack);
    }
    return err;   
}

/**************************************************************************//*!
 *
 * @name  PushSegmentEntryElem
 *
 * @brief This function pushes the PM-Segment entry element onto the Buffer 
 *		  Stack
 *
 * @param pBuffStack			:	Pointer to the Buffer Stack
 * @param pSegmEntryElem		:	Pointer to the PM segment entry element
 * @param pAttrLen				:	Pointer to the length
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                    Stack
 ******************************************************************************
 * This function pushes the Attribute Val Map, handle, type, Oid type for the 
 * element of the PM-segment onto the top of the Buffer Stack
 *****************************************************************************/
ERR_CODE PushSegmentEntryElem(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to the Buffer Stack */  
    SegmEntryElem *pSegmEntryElem,	/* [IN] Pointer to the PM segment entry 
										element */ 
	intu16 *pAttrLen				/* [IN] Pointer to the length */ 
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushAttrValMap(pBuffStack, 
		    &pSegmEntryElem->attr_val_map, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushHANDLE(pBuffStack, 
		    pSegmEntryElem->handle, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushTYPE(pBuffStack, 
		    &pSegmEntryElem->metric_type, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushOID_Type(pBuffStack, 
		    pSegmEntryElem->class_id, &AttrLen)))
		{
		    break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}
/**************************************************************************//*!
 *
 * @name  PushAttributeModEntry
 *
 * @brief This function pushes the Attribute Modify Entry onto the Buffer Stack
 *
 * @param pBuffStack			:	Pointer to the Buffer Stack
 * @param pAttributeModEntry	:	Pointer to the Attribute Modify Entry
 * @param pAttrLen				:	Pointer to the length
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                    Stack
 ******************************************************************************
 * This function pushes the modify operator and Attribute Id and value defined 
 * by the input entry onto the top of the Buffer Stack
 *****************************************************************************/
ERR_CODE PushAttributeModEntry(
	PTR_BUFFSTACK pBuffStack,				/* [IN] Pointer to the Buffer 
												Stack */  
    AttributeModEntry *pAttributeModEntry,	/* [IN] Pointer to the Attribute 
												Modify Entry */ 
	intu16 *pAttrLen						/* [IN] Pointer to the length */ 
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushAVA_Type(pBuffStack, 
		    &pAttributeModEntry->attribute, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushModifyOperator(pBuffStack, 
		    pAttributeModEntry->modify_operator, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}
/**************************************************************************//*!
 *
 * @name  PushHandleAttrValMapEntry
 *
 * @brief This function pushes the Handle Attribute Val Map Entry onto the
 *		  Buffer Stack
 *
 * @param pBuffStack				:	Pointer to the Buffer Stack
 * @param pHandleAttrValMapEntry	:	Pointer to the Handle Attr Val Map 
 *										Entry
 * @param pAttrLen					:	Pointer to the length
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                    Stack
 ******************************************************************************
 * This function pushes the Attribute Val Map and object handle defined by the 
 * input Handle Attribute Val Map onto the top of the Buffer Stack
 *****************************************************************************/
ERR_CODE PushHandleAttrValMapEntry(
	PTR_BUFFSTACK pBuffStack,						/* [IN] Pointer to the 
														Buffer Stack*/  
	HandleAttrValMapEntry *pHandleAttrValMapEntry,	/* [IN] Pointer to the 
														Handle Attr Val Map 
														Entry */ 
	intu16* pAttrLen								/* [IN] Pointer to the 
														length */ 
)	
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushAttrValMap(pBuffStack, 
		    &pHandleAttrValMapEntry->attr_val_map, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		/* Push Metric Source Handle Reference */
		if(ERROR_SUCCESS != (err = PushHANDLE(pBuffStack, 
		    pHandleAttrValMapEntry->obj_handle, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}
/**************************************************************************//*!
 *
 * @name  PushAVA_Type
 *
 * @brief This function pushes the Attribute id and its value onto the Buffer 
 *		  Stack
 *
 * @param pBuffStack	:	Pointer to the Buffer Stack
 * @param pAVA_Type		:	Pointer to the Attribute Id and value
 * @param pAttrLen		:	Pointer to the length
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                    Stack
 ******************************************************************************
 * This function pushes the Attribute id and value onto the top of the Buffer
 * Stack
 *****************************************************************************/
ERR_CODE PushAVA_Type(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to the Buffer Stack */ 
	AVA_Type *pAVA_Type,		/* [IN] Pointer to the Attribute Id and 
									value */  
	intu16 *pAttrLen			/* [IN] Pointer to the length */ 
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
    	if(ERROR_SUCCESS != (err = PushAny(pBuffStack, 
    	    &pAVA_Type->attribute_value, &AttrLen)))
    	{
    		break;
    	}
    	*pAttrLen += AttrLen;
		/* Push OID */
		if(ERROR_SUCCESS != (err = PushOID_Type(pBuffStack, 
		    pAVA_Type->attribute_id, &AttrLen)))
		{
		    break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  GetPmStoreHandle
 *
 * @brief This function returns pointer of the object if the Object handle 
 *		  matches any PM-Store object handle in DIM
 *
 * @param objHandle	:	Object Handle
 *
 * @return PPMSTORE		:   Pointer to the PM-Store Object
 *		   NULL			:   In case object is not a PM-Store object
 ******************************************************************************
 * This function matches the input handle with all the PM-Store 
 * handles in DIM and returns pointer to that object.
 *****************************************************************************/
static PPMSTORE GetPmStoreHandle(
	HANDLE objHandle		/* [IN] Object Handle */
)
{
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    PCONFIGURATION pConfiguration = 
        &(*pMds->aConfiguration)[pMds->ConfigSelected];
    PPMSTORE pPmStore = &(*pConfiguration->pPmStore)[0];
    intu16 i;
    
    for(i = 0; i < pConfiguration->PmStoreCount; i++)
    {
        if(objHandle == pPmStore[i].MdcAttrIdHandle)
            return &pPmStore[i];        
    }
    return NULL;
}

/**************************************************************************//*!
 *
 * @name  GetNumericHandle
 *
 * @brief This function returns pointer of the object if the Object handle 
 *		  matches any Numeric object handle in DIM
 *
 * @param objHandle	:	Object Handle
 *
 * @return PNUMERIC		:   Pointer to the Numeric Object
 *		   NULL			:   In case object is not a Numeric object
 ******************************************************************************
 * This function matches the input handle with all the Numeric 
 * handles in DIM and returns pointer to that object.
 *****************************************************************************/
static PNUMERIC GetNumericHandle(
	HANDLE objHandle		/* [IN] Object Handle */
)
{
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    PCONFIGURATION pConfiguration = 
        &(*pMds->aConfiguration)[pMds->ConfigSelected];
    PNUMERIC pNumeric = &(*pConfiguration->pNumeric)[0];
    intu16 i;
    
    for(i = 0; i < pConfiguration->NumericCount; i++)
    {
        if(objHandle == pNumeric[i].pMetric->MdcAttrIdHandle)
            return &pNumeric[i];        
    }
    return NULL;
}
   

/**************************************************************************//*!
 *
 * @name  GetRtsaHandle
 *
 * @brief This function returns pointer of the object if the Object handle 
 *		  matches any RT-SA object handle in DIM
 *
 * @param objHandle	:	Object Handle
 *
 * @return PRTSA		:   Pointer to the RT-SA Object
 *		   NULL			:   In case object is not an RT-SA object
 ******************************************************************************
 * This function matches the input handle with all the RT-SA 
 * handles in DIM and returns pointer to that object.
 *****************************************************************************/
static PRTSA GetRtsaHandle(
	HANDLE objHandle		/* [IN] Object Handle */
)
{
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    PCONFIGURATION pConfiguration = 
        &(*pMds->aConfiguration)[pMds->ConfigSelected];
    PRTSA pRtSa = &(*pConfiguration->pRtSa)[0];
    intu16 i;
    
    for(i = 0; i < pConfiguration->RtsaCount; i++)
    {
        if(objHandle == pRtSa[i].pMetric->MdcAttrIdHandle)
            return &pRtSa[i];        
    }
    return NULL;
}
    
/**************************************************************************//*!
 *
 * @name  GetEnumHandle
 *
 * @brief This function returns pointer of the object if the Object handle 
 *		  matches any Enumeration object handle in DIM
 *
 * @param objHandle	:	Object Handle
 *
 * @return PENUMERATION		:   Pointer to the Enumeration Object
 *		   NULL				:   In case object is not an Enumeration object
 ******************************************************************************
 * This function matches the input handle with all the Enumeration 
 * handles in DIM and returns pointer to that object.
 *****************************************************************************/
static PENUMERATION GetEnumHandle(
	HANDLE objHandle		/* [IN] Object Handle */
)
{
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    PCONFIGURATION pConfiguration = &(*pMds->aConfiguration)[pMds->ConfigSelected];
    PENUMERATION pEnumeration = &(*pConfiguration->pEnumeration)[0];
    intu16 i;
    
    for(i = 0; i < pConfiguration->EnumCount; i++)
    {
        if(objHandle == pEnumeration[i].pMetric->MdcAttrIdHandle)
        {          
            return &pEnumeration[i];        
        }
    }
    return NULL;
}

/**************************************************************************//*!
 *
 * @name  GetEpiCfgScanHandle
 *
 * @brief This function returns pointer of the object if the Object handle 
 *		  matches any Episoidic scanner object handle in DIM
 *
 * @param objHandle	:	Object Handle
 *
 * @return PEPICFGSCANNER	:   Pointer to the Episoidic Scanner Object
 *		   NULL				:   In case object is not an Episoidic Scanner 
 *								object
 ******************************************************************************
 * This function matches the input handle with all the Episoidic Scanner 
 * handles in DIM and returns pointer to that object.
 *****************************************************************************/
static PEPICFGSCANNER GetEpiCfgScanHandle(
	HANDLE objHandle		/* [IN] Object Handle */
)
{
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    PCONFIGURATION pConfiguration = 
        &(*pMds->aConfiguration)[pMds->ConfigSelected];
    PEPICFGSCANNER pEpiScanner = &(*pConfiguration->pEpiScanner)[0];
    intu16 i;
    
    for(i = 0; i < pConfiguration->EpiScanCount; i++)
    {
        if(objHandle == pEpiScanner[i].pCfgScanner->pScanner->MdcAttrIdHandle)
            return &pEpiScanner[i];        
    }
    return NULL;
}

/**************************************************************************//*!
 *
 * @name  GetPeriCfgScanHandle
 *
 * @brief This function returns pointer of the object if the Object handle 
 *		  matches any periodic scanner object handle in DIM
 *
 * @param objHandle	:	Object Handle
 *
 * @return PPERICFGSCANNER	:   Pointer to the Periodic Scanner Object
 *		   NULL				:   In case object is not a Periodic Scanner object
 ******************************************************************************
 * This function matches the input handle with all the Periodic Scanner 
 * Handles in DIM and returns pointer to that object.
 *****************************************************************************/
static PPERICFGSCANNER GetPeriCfgScanHandle(
	HANDLE objHandle		/* [IN] Object Handle */
)
{
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    PCONFIGURATION pConfiguration = 
        &(*pMds->aConfiguration)[pMds->ConfigSelected];
    PPERICFGSCANNER pPeriScanner = &(*pConfiguration->pPeriScanner)[0];
    intu16 i;
    
    for(i = 0; i < pConfiguration->PeriScanCount; i++)
    {
        if(objHandle == pPeriScanner[i].pCfgScanner->pScanner->MdcAttrIdHandle)
            return &pPeriScanner[i];        
    }
    return NULL;
}

/**************************************************************************//*!
 *
 * @name  GetMdsHandle
 *
 * @brief This function returns pointer of the object if the Object is an MDS 
 *		  object
 *
 * @param obj_handle	:	Object Handle
 *
 * @return PMDS			:   Pointer to the MDS Object
 *		   NULL			:   In case object is not an MDS object
 ******************************************************************************
 * This function matches the input handle with the MDS handle in 
 * DIM and returns pointer to that object.
 *****************************************************************************/
static PMDS GetMdsHandle(
	HANDLE obj_handle		/* [IN] Object Handle */
)
{
    if((obj_handle == MDS_HANDLE) && (pDIM != NULL))
    {
        return pDIM->pMds;
    }
    return NULL;
}
/**************************************************************************//*!
 *
 * @name  GetObjectByHandle
 *
 * @brief This function returns the type and the pointer of the object defined
 *		  by the input handle
 *
 * @param obj_handle	:	Object Handle
 * @param objType		:	Pointer to the object type
 *
 * @return void*		:   Pointer to the Object
 *		   NULL			:   If object not found
 ******************************************************************************
 * This function matches the input handle with the handles of all objects in 
 * DIM and returns the type of the object and pointer to that object.
 *****************************************************************************/
void* GetObjectByHandle(
	HANDLE obj_handle,		/* [IN] Object Handle */ 
	intu16 *objType			/* [OUT] Pointer to the object type */ 
)
{
    void *pObject = NULL;
    *objType = 0;
    do
    {
	    (PMDS)pObject = GetMdsHandle(obj_handle);
        if(NULL != pObject)
   		{
          *objType = MDC_MOC_VMS_MDS_SIMP;
          break;
      }

	    (PPMSTORE)pObject = GetPmStoreHandle(obj_handle);
	    if(NULL != pObject)
	    {
	        *objType = MDC_MOC_VMO_PMSTORE;
	        break;
	    }
	    
	    (PNUMERIC)pObject = GetNumericHandle(obj_handle);
	    if(NULL != pObject)
	    {
	        *objType = MDC_MOC_VMO_METRIC_NU;
	        break;
	    }

	    (PRTSA)pObject = GetRtsaHandle(obj_handle);
	    if(NULL != pObject)
	    {
	        *objType = MDC_MOC_VMO_METRIC_SA_RT;
	        break;
	    }

	    (PENUMERATION)pObject = GetEnumHandle(obj_handle);
	    if(NULL != pObject)
	    {
	        *objType = MDC_MOC_VMO_METRIC_ENUM;
	        break;
	    }

	    (PEPICFGSCANNER)pObject = GetEpiCfgScanHandle(obj_handle);
	    if(NULL != pObject)
	    {
	        *objType = MDC_MOC_SCAN_CFG_EPI;
	        break;
	    }

	    (PPERICFGSCANNER)pObject = GetPeriCfgScanHandle(obj_handle);
	    if(NULL != pObject)
	    {
	        *objType = MDC_MOC_SCAN_CFG_PERI;
	        break;
	    }

    }while(0);
    return pObject;
}
/**************************************************************************//*!
 *
 * @name  PushScanReportInfoHeader
 *
 * @brief This function pushes the Scan Report Info Header into the Buffer 
 *		  Stack
 *
 * @param pBuffStack	:	Pointer to the Buffer Stack
 * @param DataRequestId	:	Data request Id
 * @param ScanRptNo		:	Scan report number
 * @param Count			:	Count(Number of scans/Number of person Scans)
 * @param pAttrLen		:	Pointer to the length
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function adds the header over the scan event report data.
 *****************************************************************************/
ERR_CODE PushScanReportInfoHeader(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to the Buffer Stack */ 
    DataReqId DataRequestId,	/* [IN] Data request Id */ 
	intu16 ScanRptNo,			/* [IN] Scan report number */ 
	intu16 Count,				/* [IN] Count(Number of scans/Number of person 
									Scans) */ 
    intu16 *pAttrLen			/* [IN] Pointer to the length */ 
)
{
    ERR_CODE err;
    intu16 AttrLen;
    *pAttrLen = 0;
    do
    {
        if(ERROR_SUCCESS != (err = PushObsScanListHeader(pBuffStack, Count, (intu16)GetDataSize(pBuffStack), &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = PushScanReportHeader(pBuffStack, DataRequestId, ScanRptNo, &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        (void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  GetAttrValMapPtr
 *
 * @brief This function gets the Pointer of the object and returns the pointer
 *		  to the Attribute Val Map
 *
 * @param handle	:	Handle of the metric Object
 *
 * @return AttrValMap*	:	in case Attribute Val Map is present
 *		   NULL			:	in case not present
 ******************************************************************************
 * This function takes the handle of the metric object as input and calculates 
 * the pointer to the Attribute Val Map.
 *****************************************************************************/
AttrValMap* GetAttrValMapPtr(
	HANDLE handle		/* [IN] Handle of the metric Object */
)
{
    AttrValMap* pAttrValMap = NULL;
    void* pObject;
    intu16 ObjType;

    pObject = GetObjectByHandle(handle, &ObjType);
    switch(ObjType)
    {        
        case MDC_MOC_VMO_METRIC_NU:
            if(((PNUMERIC)pObject)->pMetric->OptionalAttributeFlag & OPT_MET_ATTRVALMAP)
            {                
                pAttrValMap = ((PNUMERIC)pObject)->pMetric->pMdcAttrAttributeValMap;
            }
            break;

        case MDC_MOC_VMO_METRIC_SA_RT:
            if(((PRTSA)pObject)->pMetric->OptionalAttributeFlag & OPT_MET_ATTRVALMAP)
            {                
                pAttrValMap = ((PRTSA)pObject)->pMetric->pMdcAttrAttributeValMap;
            }
            break;

        case MDC_MOC_VMO_METRIC_ENUM:
            if(((PENUMERATION)pObject)->pMetric->OptionalAttributeFlag & OPT_MET_ATTRVALMAP)
            {                
                pAttrValMap = ((PENUMERATION)pObject)->pMetric->pMdcAttrAttributeValMap;
            }
            break;
        default:
            break;
    }
    return pAttrValMap;
}
/**************************************************************************//*!
 *
 * @name  GetAttrValMapFromHandleAttrValMap
 *
 * @brief This function gets the Pointer of the object and returns the pointer
 *		  to the Attribute Val Map
 *
 * @param handle	        :	Handle of the metric Object
 * @param pHandleAttrValMap	:   Pointer to the handle attribute val map of the 
 *                              scanner
 *
 * @return AttrValMap*	:	in case input Handle is valid
 *		   NULL					:	in case not valid
 ******************************************************************************
 * This function takes the handle of the scanner object as input and calculates 
 * the pointer to the Handle Attribute Val Map.
 *****************************************************************************/
AttrValMap* GetAttrValMapFromHandleAttrValMap(
    HANDLE handle,                      /* [IN] Handle of the metric Object */
    HandleAttrValMap* pHandleAttrValMap /* [IN] Pointer to the handle attribute 
                                            val map of the scanner*/
)
{
    AttrValMap* pAttrValMap = NULL;
    intu16 j;
    HandleAttrValMapEntry* pHandleAttrValMapEntry = 
		&pHandleAttrValMap->value[0];
    for(j = 0; j < ieee_ntohs(pHandleAttrValMap->count); j++)
    {        
        if(ieee_ntohs(pHandleAttrValMapEntry->obj_handle) == handle)
        {
            pAttrValMap = &pHandleAttrValMapEntry->attr_val_map;
            break;
        }
        (intu8*)pHandleAttrValMapEntry += 
        	ieee_ntohs(pHandleAttrValMapEntry->attr_val_map.length) + sizeof(HANDLE) + ATTR_HEADER_SIZE;
    }
    return pAttrValMap;
}
    
/**************************************************************************//*!
 *
 * @name  GetHandleAttrValMapPtr
 *
 * @brief This function gets the Pointer of the object and returns the pointer
 *		  to the Handle Attribute Val Map
 *
 * @param handle	:	Handle of the Scanner Object
 *
 * @return HandleAttrValMap*	:	in case input Handle is valid
 *		   NULL					:	in case not valid
 ******************************************************************************
 * This function takes the handle of the scanner object as input and calculates 
 * the pointer to the Handle Attribute Val Map.
 *****************************************************************************/
HandleAttrValMap* GetHandleAttrValMapPtr(
	HANDLE handle		/* [IN] Handle of the Scanner Object */
)
{
    HandleAttrValMap* pHandleAttrValMap = NULL;
    void* pObject;
    intu16 ObjType;

    pObject = GetObjectByHandle(handle, &ObjType);     
    switch(ObjType)
    {        
        case MDC_MOC_SCAN_CFG_EPI:
            pHandleAttrValMap = ((PEPICFGSCANNER)pObject)->pCfgScanner->pScanner->pMdcAttrScanHandleAttrValMap;
            break;
        case MDC_MOC_SCAN_CFG_PERI:
            pHandleAttrValMap = ((PPERICFGSCANNER)pObject)->pCfgScanner->pScanner->pMdcAttrScanHandleAttrValMap;
            break;
        default:
            break;
    }

    return pHandleAttrValMap;
}

/**************************************************************************//*!
 *
 * @name  MatchAttrListWithAttrValMap
 *
 * @brief This function matches the Observation Scan with the Attribute 
 *		  Val Map of the metric object
 *
 * @param pObsScan	:	Pointer to the Observation Scan
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if the Observation Scan does not exactly 
 *									  match the Attribute Val Map
 ******************************************************************************
 * This function matches the Observation Scan with the Attribute Val Map of the 
 * object and returns an error if they do not match
 *****************************************************************************/
ERR_CODE MatchAttrListWithAttrValMap(
	ObservationScan* pObsScan		/* [IN] Pointer to the Observation Scan */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    AttrValMap* pAttrValMap = GetAttrValMapPtr(ieee_ntohs(pObsScan->obj_handle));
    if(pAttrValMap == NULL)
    {
        return ERROR_INVALID_PARAM;
    }
    else
    {
        intu16 i, j;
        AVA_Type* pAvaType = &pObsScan->attributes.value[0];
        for(i = 0; i < ieee_ntohs(pObsScan->attributes.count); i++)
        {
            AttrValMapEntry* pAttrValMapEntry = &pAttrValMap->value[0];                                
            for(j = 0; j < ieee_htons(pAttrValMap->count); j++)
            {
                if(pAttrValMapEntry->attribute_id == 
                    ieee_ntohs(pAvaType->attribute_id))
                {
                    break;
                }
                (intu8*)pAttrValMapEntry += sizeof(AttrValMapEntry);
            }
            if(j == pAttrValMap->count)
            {                                    
                return ERROR_INVALID_PARAM;
            }
            (intu8*)pAvaType += ieee_ntohs(pAvaType->attribute_value.length) + 
                ANY_HEADER_SIZE + sizeof(OID_Type);
        }
    }
    return err;    
}
/**************************************************************************//*!
 *
 * @name  MatchAttrListWithHandleAttrValMap
 *
 * @brief This function matches the Observation Scan with the Handle Attribute 
 *		  Val Map of the Scanner object
 *
 * @param pObsScan	:	Pointer to the Observation Scan
 * @param handle	:	Handle of the Scanner Class
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if the Observation Scan does not match 
 *									  any handle entry in the Handle Attribute 
 *									  Val Map
 ******************************************************************************
 * This function matches the Observation Scan and returns an error if it is not 
 * found in the Handle Attribute Val Map
 *****************************************************************************/
ERR_CODE MatchAttrListWithHandleAttrValMap(
	ObservationScan* pObsScan,		/* [IN] Pointer to the Observation Scan */
	HANDLE handle					/* [IN] Handle of the Scanner Class */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    HandleAttrValMap* pHandleAttrValMap = GetHandleAttrValMapPtr(handle);
    HandleAttrValMapEntry* pHandleAttrValMapEntry = 
		&pHandleAttrValMap->value[0];
    AttrValMap* pAttrValMap;
    intu16 i, j;
    AVA_Type* pAvaType = &pObsScan->attributes.value[0];
    /* search for this handle in handle attr val map */
    for(j = 0; j < ieee_ntohs(pHandleAttrValMap->count); j++)
    {        
        if(pHandleAttrValMapEntry->obj_handle == ieee_ntohs(pObsScan->obj_handle))
        {
            break;
        }
        (intu8*)pHandleAttrValMapEntry += 
        	ieee_ntohs(pHandleAttrValMapEntry->attr_val_map.length) + sizeof(HANDLE) + 
            ATTR_HEADER_SIZE;
    }
    if(j == pHandleAttrValMap->count)
    {
        return ERROR_INVALID_PARAM;
    }
    pAttrValMap = &pHandleAttrValMapEntry->attr_val_map;
    for(i = 0; i < ieee_ntohs(pObsScan->attributes.count); i++)
    {
        AttrValMapEntry* pAttrValMapEntry = &pAttrValMap->value[0];                                
        for(j = 0; j < ieee_ntohs(pAttrValMap->count); j++)
        {
            if(pAttrValMapEntry->attribute_id == ieee_ntohs(pAvaType->attribute_id))
            {
                break;
            }
            (intu8*)pAttrValMapEntry += sizeof(AttrValMapEntry);
        }
        if(j == pAttrValMap->count)
        {                                    
            return ERROR_INVALID_PARAM;
        }
        (intu8*)pAvaType += ieee_ntohs(pAvaType->attribute_value.length) + 
            ANY_HEADER_SIZE + sizeof(OID_Type);
    }
    return err;    
}
/**************************************************************************//*!
 *
 * @name  ScanAttrListWithAttrValMap
 *
 * @brief This function checks whether the attributes in Observation Scan is 
 *		  present in Attribute Val Map(if defined) of that object
 *
 * @param pObsScan	:	Pointer to the Observation Scan
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if any attribute of Observation Scan 
 *									  matches with any attribute of Attribute 
 *									  Val Map
 ******************************************************************************
 * This function scans the Observation Scan and returns an error in case any 
 * attribute of the Observation Scanis present in Attribute Val Map of that 
 * object
 *****************************************************************************/
ERR_CODE ScanAttrListWithAttrValMap(
	ObservationScan* pObsScan		/* [IN] Pointer to the Observation Scan */
) 
{
    ERR_CODE err = ERROR_SUCCESS;
    AttrValMap* pAttrValMap = GetAttrValMapPtr(ieee_ntohs(pObsScan->obj_handle));
    if(pAttrValMap == NULL)
    {
        return err;
    }
    else
    {
        intu16 i, j;
        AVA_Type* pAvaType = &pObsScan->attributes.value[0];
        for(i = 0; i < ieee_ntohs(pObsScan->attributes.count); i++)
        {
            AttrValMapEntry* pAttrValMapEntry = &pAttrValMap->value[0];                                
            for(j = 0; j < pAttrValMap->count; j++)
            {
                if(pAttrValMapEntry->attribute_id == 
                    ieee_ntohs(pAvaType->attribute_id))
                {
                    return ERROR_INVALID_PARAM;
                }
                (intu8*)pAttrValMapEntry += sizeof(AttrValMapEntry);
            }
            (intu8*)pAvaType += ieee_ntohs(pAvaType->attribute_value.length) + 
				ANY_HEADER_SIZE + sizeof(OID_Type);
        }
    }
    return err;    
}
/**************************************************************************//*!
 *
 * @name  GetAttrValMapDataSize
 *
 * @brief This function returns the data size defined by the input Attribute 
 *		  Val Map
 *
 * @param pAttrValMap	:	Pointer to the Attribute Val Map
 *
 * @return Size
 ******************************************************************************
 * This function takes Attribute Val Map as input and calculates the data size 
 * defined by it.
 *****************************************************************************/
intu16 GetAttrValMapDataSize(
	AttrValMap* pAttrValMap		/* [IN] Pointer to the Attribute Val Map */
)
{
    intu16 size = 0;
    intu16 i;
    AttrValMapEntry* temp = &pAttrValMap->value[0];
    for(i = 0; i < ieee_ntohs(pAttrValMap->count); i++)
    {
        size += ieee_ntohs(temp->attribute_len);
        (intu8*)temp += sizeof(AttrValMapEntry);
    }
    return size;
}
/**************************************************************************//*!
 *
 * @name  GetObjectFixedDataSize
 *
 * @brief This function gets the Attribute Val Map of the object and returns 
		  the size of the data in the val map
 *
 * @param handle	:	Handle of the object class
 *
 * @return Size
 ******************************************************************************
 * This function returns the data size defined by the Attribute Val Map of the
 * object.
 *****************************************************************************/
intu16 GetObjectFixedDataSize(
	HANDLE handle			/* [IN] Handle of the object class */
)
{
    intu16 size;
    AttrValMap* pAttrValMap = GetAttrValMapPtr(handle);
    size = GetAttrValMapDataSize(pAttrValMap);
    return size;
}
/**************************************************************************//*!
 *
 * @name  GetFixedFormatEventRptSize
 *
 * @brief This function calculates the data size of fixed format event 
 *		  report
 *
 * @param pObsScanList	:	Pointer to the Observation Scan List
 *
 * @return Size
 ******************************************************************************
 * This function returns the data size of the fixed format event report
 *****************************************************************************/    
intu16 GetFixedFormatEventRptSize(
	ObservationScanList* pObsScanList	/* [IN] Pointer to the Observation Scan
											List */
)
{
    intu16 size = 0;
    intu16 i;
    ObservationScan* pObsScan = &pObsScanList->value[0];
    for(i = 0; i < ieee_ntohs(pObsScanList->count); i++)
    {        
        size += GetObjectFixedDataSize(ieee_ntohs(pObsScan->obj_handle));
        size += ATTR_HEADER_SIZE; /* for handle and length */
    }
    return size;
}
/**************************************************************************//*!
 *
 * @name  GetMPFixedFormatEventRptSize
 *
 * @brief This function calculates the data size of multi person fixed  
 *		  format event report
 *
 * @param pScanRptPerVarList	:	Pointer to the Scan Report Per Person
 *
 * @return Size
 ******************************************************************************
 * This function returns the data size of the multi person fixed format 
 * event report
 *****************************************************************************/
intu16 GetMPFixedFormatEventRptSize(
	ScanReportPerVarList* pScanRptPerVarList /* [IN] Pointer to the Scan Report 
												Per Person */
)
{
    intu16 size = 0;
    intu16 i;
    ObservationScanList* pObsScanList = &pScanRptPerVarList->value[0].
		obs_scan_var;
    for(i = 0; i < ieee_ntohs(pScanRptPerVarList->count); i++)
    {
        size += GetFixedFormatEventRptSize((ObservationScanList*) pObsScanList);
        size += sizeof(intu16); /* person id */
        size += ATTR_HEADER_SIZE;
        (intu8*)pObsScanList += ieee_ntohs(pObsScanList->length) + (sizeof(intu16) * 3);
    }
    size += ATTR_HEADER_SIZE; /* peron count, length */
    return size;
}
/**************************************************************************//*!
 *
 * @name  GetVarFormatEventRptSize
 *
 * @brief This function calculates the data size of variable format event 
 *		  report
 *
 * @param pObsScanList	:	Pointer to the Observation Scan List
 *
 * @return Size
 ******************************************************************************
 * This function returns the data size of the variable format event report
 *****************************************************************************/
intu16 GetVarFormatEventRptSize(
	ObservationScanList* pObsScanList	/* [IN] Pointer to the Observation Scan
											List */
)
{
    return ieee_ntohs(pObsScanList->length);
}
/**************************************************************************//*!
 *
 * @name  GetMPVarFormatEventRptSize
 *
 * @brief This function calculates the data size of multi person variable  
 *		  format event report
 *
 * @param pScanPerVarList	:	Pointer to the Scan Report Per Person
 *
 * @return Size
 ******************************************************************************
 * This function returns the data size of the multi person variable format 
 * event report
 *****************************************************************************/
intu16 GetMPVarFormatEventRptSize(
	ScanReportPerVarList* pScanPerVarList	/* [IN] Pointer to the Scan Report 
												Per Person */
)
{
    return ieee_ntohs(pScanPerVarList->length);
}
/**************************************************************************//*!
 *
 * @name  GetGroupedFormatEventRptSize
 *
 * @brief This function returns the size of the data according to the Handle 
 *		  Attribute Val Map of the Scanner
 *
 * @param handle	:	Scanner(Episoidic or Periodic) Class Handle
 *
 * @return Size
 ******************************************************************************
 * This function returns the data size the scanner sends
 *****************************************************************************/
intu16 GetGroupedFormatEventRptSize(
	HANDLE handle		/* [IN] Scanner Class Handle */
)
{
  intu16 size = 0;
  void* pObject;
  intu16 ObjType;   
  intu16 i;
  AttrValMap* pAttrValMap;
  HandleAttrValMap* pHandleAttrValMap;
  HandleAttrValMapEntry* pHandleAttrValMapEntry;
    
  pObject = GetObjectByHandle(handle, &ObjType);

  if(ObjType == MDC_MOC_SCAN_CFG_EPI)
  {
        pHandleAttrValMap = ((PEPICFGSCANNER)pObject)->pCfgScanner->pScanner->pMdcAttrScanHandleAttrValMap;
  }
    
  if(ObjType == MDC_MOC_SCAN_CFG_PERI)
  {                                                                                                      
        pHandleAttrValMap = ((PPERICFGSCANNER)pObject)->pCfgScanner->pScanner->pMdcAttrScanHandleAttrValMap;
  }


  pHandleAttrValMapEntry = &pHandleAttrValMap->value[0];
  for(i = 0; i < ieee_ntohs(pHandleAttrValMap->count); i++)
  {
        pAttrValMap = &pHandleAttrValMapEntry->attr_val_map;
        size += GetAttrValMapDataSize(pAttrValMap);
        (intu8*)pHandleAttrValMapEntry += pHandleAttrValMapEntry->attr_val_map.length + sizeof(HANDLE) + ATTR_VAL_MAP_HEADER_SIZE;
  }
  size += sizeof(intu16); /* length */
    
  return size;
}
/**************************************************************************//*!
 *
 * @name  PushObsScanList
 *
 * @brief This function pushes the Observation Scan List for fixed or grouped 
 *		  format event report
 *
 * @param pBuffStack	:	Pointer to the Buffer Stack
 * @param pObsScanList	:	Pointer to the Observation Scan List
 * @param pAttrLen		:	Pointer to the Attribute length
 * @param handle		:	Handle of the object which sends measurements 
 * @param GrpdFormat	:	True for Grouped Format Event Report
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function Observation Scan for fixed/grouped format event report
 *****************************************************************************/
ERR_CODE PushObsScanList(
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to the Buffer Stack */ 
	ObservationScanList* pObsScanList,	/* [IN] Pointer to the Observation Scan 
											List */ 
	intu16* pAttrLen,					/* [OUT] Pointer to the Attribute 
											length */ 
    HANDLE handle,                      /* [IN] Handle of the object which 
                                            sends measurements */											
	intu8 GrpdFormat					/* [IN] True for Grouped Format Event 
											Report */ 
)
{    
    ERR_CODE err = ERROR_SUCCESS;
    intu16 i, j;
    ObservationScan* pObsScan;
    AttrValMap* pAttrValMap;
    intu16 AttrLen; 
    intu16 AttrLength = 0;    
    AttributeList* pAttrList;
    AttrValMapEntry* pAttrValMapEntry;
    Any* pAny;        
    
    *pAttrLen = 0;  

    pObsScan = &pObsScanList->value[0];
    for(i = 0; i < ieee_ntohs(pObsScanList->count); i++)
    {
        AttrLength = 0;
        if(GrpdFormat)
        {
            HandleAttrValMap* pHandleAttrValMap = 
                GetHandleAttrValMapPtr(handle);
            pAttrValMap = 
                GetAttrValMapFromHandleAttrValMap(ieee_ntohs(pObsScan->obj_handle), 
                pHandleAttrValMap);
        }
        else
        {            
            pAttrValMap = GetAttrValMapPtr(ieee_ntohs(pObsScan->obj_handle));
        }
        (intu8*)pAttrValMapEntry = (intu8*)&pAttrValMap->value[0] + 
			((ieee_ntohs(pAttrValMap->count) - 1) * sizeof(AttrValMapEntry));
        
        pAttrList = GetAttrListByHandle(ieee_ntohs(pObsScan->obj_handle), 
            pObsScanList);
        if(pAttrList == NULL)
            return ERROR_INVALID_DATA;
        
        for(j = 0; j < ieee_ntohs(pAttrValMap->count); j++)
        {        
            pAny = GetAttrValueByAttrId(pAttrValMapEntry->attribute_id, 
				pAttrList);
            
            /* If attribute Id is not found then incomplete data */
            if(pAny == NULL)
                return ERROR_INVALID_DATA;
            
            /* Data length should be equal to attribute length specified */
            if(ieee_ntohs(pAny->length) != ieee_ntohs(pAttrValMapEntry->attribute_len))
                return ERROR_INVALID_DATA;
            
            do
            {
                if(ERROR_SUCCESS != (err = PushStack(pBuffStack, 
                		&pAny->value[0], ieee_ntohs(pAny->length))))
                {
                    break;
                }
                AttrLength += ieee_ntohs(pAny->length);
                *pAttrLen += ieee_ntohs(pAny->length);
            }while(0);
            if(ERROR_SUCCESS != err)
            {
                (void)PopStack(pBuffStack, AttrLength, NULL, NULL);
                return ERROR_INSUFFICIENT_MEMORY;
            }
                        
            (intu8*)pAttrValMapEntry -= sizeof(AttrValMapEntry);
        }
        
        err = Pushintu16(pBuffStack, AttrLength, &AttrLen);
        *pAttrLen += AttrLen;
        
        if(!GrpdFormat)
        {
            /* Push Handle */
            err = PushHANDLE(pBuffStack, ieee_ntohs(pObsScan->obj_handle), 
                &AttrLen);
            *pAttrLen += AttrLen;
        }

        (intu8*)pObsScan += ieee_ntohs(pObsScan->attributes.length) + 
            sizeof(HANDLE) + ATTR_HEADER_SIZE; 
    }
    return err;
}
/**************************************************************************//*!
 *
 * @name  PushFixedFormatEvntRpt
 *
 * @brief This function pushes the fixed format event report onto the 
 *		  buffer stack
 *
 * @param handle		:	Handle of the object which has to send 
 *							measurements
 * @param pBuffStack	:	Pointer to the Buffer Stack
 * @param pObsScanList	:	Pointer to an array of Observation Scan List
 * @param bConfirm  	:	True for confirmed event report
 * @param ScanCount  	:	Number of Scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function creates fixed format event report
 *****************************************************************************/
ERR_CODE PushFixedFormatEvntRpt(
	HANDLE handle,						/* [IN] Handle of the object which has 
											to send measurements */
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to the Buffer Stack */
	ObservationScanList* (*pObsScanList)[],	
	                                    /* [IN] Pointer to an array of 
	                                        Observation Scan List */
    intu8 bConfirm,                     /* [IN] True for confirmed event 
                                            report */											
    intu16 ScanCount                    /* [IN] Number of scans */                                            
)        
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 AttrLen;
    intu16 i;
    ObservationScanList* pObservationScanList;
    intu16 NumScans = 0;
    do
    {
        for(i = ScanCount; i > 0; i--)
        {                
            pObservationScanList = (*pObsScanList)[i - 1];
            if(ERROR_SUCCESS != (err = PushObsScanList(pBuffStack, 
                pObservationScanList, &AttrLen, handle, FALSE)))
            {
                break;
            }
            NumScans += ieee_ntohs(pObservationScanList->count);
        }
        if(ERROR_SUCCESS != err)
        {
            break;
        }
        if(ERROR_SUCCESS != (err = PushScanReportInfoHeader(pBuffStack, 
            DATA_REQ_ID_AGENT_INITIATED, g_scan_report_num++, 
			NumScans, &AttrLen)))
        {
            break;
        }
        if(bConfirm)
        {
            if(ERROR_SUCCESS != (err = PushConfScanRptFixedHeader(pBuffStack, 
                handle, 0xFFFFFFFF, &AttrLen)))
            {
                break;
            }
        }
        else
        {
            if(ERROR_SUCCESS != (err = PushUnConfScanRptFixedHeader(pBuffStack, 
                handle, 0xFFFFFFFF, &AttrLen)))
            {
                break;
            }
        }
    }while(0);
    return err;
}
/**************************************************************************//*!
 *
 * @name  PushVarFormatEvntRpt
 *
 * @brief This function pushes the variable format event report onto the 
 *		  buffer stack
 *
 * @param handle		:	Handle of the object which has to send 
 *							measurements
 * @param pBuffStack	:	Pointer to the Buffer Stack
 * @param pObsScanList	:	Pointer to an array of Observation Scan List
 * @param bConfirm  	:	True for confirmed event report
 * @param ScanCount  	:	Number of Scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function creates variable format event report
 *****************************************************************************/
ERR_CODE PushVarFormatEvntRpt(
	HANDLE handle,						/* [IN] Handle of the object which has 
											to send measurements*/ 
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to the Buffer Stack */
	ObservationScanList* (*pObsScanList)[],	
	                                    /* [IN] Pointer to an array of 
	                                        Observation Scan List */
    intu8 bConfirm,                     /* [IN] True for confirmed event 
                                            report */											
    intu16 ScanCount                    /* [IN] Number of scans */                                            
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 AttrLen;
    intu16 i;
    ObservationScanList* pObservationScanList;
    intu16 NumScans = 0;
    do
    {
        for(i = ScanCount; i > 0; i--)
        {                
            pObservationScanList = (*pObsScanList)[i - 1];
            if(ERROR_SUCCESS != (err = PushStack(pBuffStack, 
    			&pObservationScanList->value[0], 
    			ieee_ntohs(pObservationScanList->length))))
            {
                break;
            }
            NumScans += ieee_ntohs(pObservationScanList->count);
        }
        if(ERROR_SUCCESS != err)
        {
            break;
        }
        if(ERROR_SUCCESS != (err = PushScanReportInfoHeader(pBuffStack, 
            DATA_REQ_ID_AGENT_INITIATED, g_scan_report_num++, 
			NumScans, &AttrLen)))
        {
            break;
        }
        if(bConfirm)
        {            
            if(ERROR_SUCCESS != (err = PushConfScanRptVarHeader(pBuffStack, 
                handle, 0xFFFFFFFF, &AttrLen)))
            {
                break;
            }
        }
        else
        {            
            if(ERROR_SUCCESS != (err = PushUnConfScanRptVarHeader(pBuffStack, 
                handle, 0xFFFFFFFF, &AttrLen)))
            {
                break;
            }
        }
    }while(0);
    return err;
}
/**************************************************************************//*!
 *
 * @name  PushMPVarFormatEvntRpt
 *
 * @brief This function pushes the multi person variable format event report 
 *		  onto the buffer stack
 *
 * @param handle				:	Handle of the object which has to send 
 *									measurements
 * @param pBuffStack			:	Pointer to the Buffer Stack
 * @param pScanRptPerVarList	:	Pointer to an array of Scan report per 
 *                                  person
 * @param bConfirm          	:	True for confirmed event report
 * @param ScanCount          	:	Number of Scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function creates multi person variable format event report
 *****************************************************************************/
ERR_CODE PushMPVarFormatEvntRpt(
	HANDLE handle,								/* [IN] Handle of the object 
													which has to send 
													measurements */ 
	PTR_BUFFSTACK pBuffStack,					/* [IN] Pointer to the Buffer 
													Stack */ 
    ScanReportPerVarList* (*pScanRptPerVarList)[],	
                                                /* [IN] Pointer to an array of
                                                    Scan report per person */
	intu8 bConfirm,                             /* [IN] True for confirmed 
	                                                event report */ 
    intu16 ScanCount                            /* [IN] Number of Scans */	                                                
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 AttrLen;
    intu16 i;
    ScanReportPerVarList* pScanReportPerVarList; 
    intu16 NumScans = 0;
    do
    {
        for(i = ScanCount; i > 0 ; i--)
        {
            pScanReportPerVarList = (*pScanRptPerVarList)[i - 1];    
            if(ERROR_SUCCESS != (err = PushStack(pBuffStack, 
    			&pScanReportPerVarList->value[0], 
    			ieee_ntohs(pScanReportPerVarList->length))))
            {
                break;
            }
            NumScans += ieee_ntohs(pScanReportPerVarList->count);
        }
        if(ERROR_SUCCESS != err)
        {
            break;
        }
        if(ERROR_SUCCESS != (err = PushScanReportInfoHeader(pBuffStack, 
            DATA_REQ_ID_AGENT_INITIATED, g_scan_report_num++, 
			NumScans, &AttrLen)))
        {
            break;
        }
        if(bConfirm)
        {
            if(ERROR_SUCCESS != (err = PushConfScanRptMPVarHeader(pBuffStack, 
                handle, 0xFFFFFFFF, &AttrLen)))
            {
                break;
            }
        }
        else
        {
            if(ERROR_SUCCESS != (err = PushUnConfScanRptMPVarHeader(pBuffStack, 
                handle, 0xFFFFFFFF, &AttrLen)))
            {
                break;
            }
        }
    }while(0);
    return err;
}
/**************************************************************************//*!
 *
 * @name  PushScanPerVarList
 *
 * @brief This function pushes the multi person scan per var list for fixed or 
 *		  grouped format event report
 *
 * @param pBuffStack			:	Pointer to the Buffer Stack
 * @param pScanRptPerVarList	:	Pointer to the Scan report per person
 * @param handle    			:	Handle of the object which has to send 
 *	                                measurements 
 * @param GrpdFormat			:	True for Grouped Format Event Report
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function pushes scans for fixed/grouped format event report
 *****************************************************************************/
ERR_CODE PushScanPerVarList(
	PTR_BUFFSTACK pBuffStack,					/* [IN] Pointer to the Buffer 
													Stack */ 
	ScanReportPerVarList* pScanRptPerVarList,	/* [IN] Pointer to the Scan 
													report per person */
	HANDLE handle,                              /* [IN] Handle of the object 
	                                                which has to send 
	                                                measurements */
	intu8 GrpdFormat							/* [IN] True for Grouped Format
													Event Report */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 i;
    intu16 AttrLen;
    ScanReportPerVar* pScanRptPerVar = &pScanRptPerVarList->value[0];
    ObservationScanList* pObsScanList;
    for(i = 0; i < ieee_ntohs(pScanRptPerVarList->count); i++)
    {
        pObsScanList = &pScanRptPerVar->obs_scan_var;
        if(ERROR_SUCCESS != (err = PushObsScanList(pBuffStack, pObsScanList, 
			&AttrLen, handle, GrpdFormat))) 
        {
            break;
        }
        if(!GrpdFormat)
        {            
            if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				ieee_ntohs(pObsScanList->count), AttrLen, &AttrLen)))
            {
                break;
            }
        }
        if(ERROR_SUCCESS != (err = PushPersonId(pBuffStack, 
			ieee_ntohs(pScanRptPerVar->person_id), &AttrLen)))
        {
            break;
        }
        (intu8*)pScanRptPerVar += ieee_ntohs(pObsScanList->length) + 
            (sizeof(intu16) * 3);
    }
    return err;
}
/**************************************************************************//*!
 *
 * @name  PushMPFixedFormatEvntRpt
 *
 * @brief This function pushes the multi person fixed format event report 
 *		  onto the buffer stack
 *
 * @param handle				:	Handle of the object which has to send 
 *									measurements
 * @param pBuffStack			:	Pointer to the Buffer Stack
 * @param pScanRptPerVarList	:	Pointer to an array of Scan report per 
 *                                  person
 * @param bConfirm          	:	True for confirmed event report
 * @param ScanCount          	:	Number of Scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function creates multi person fixed format event report
 *****************************************************************************/
ERR_CODE PushMPFixedFormatEvntRpt(
	HANDLE handle,								/* [IN] Handle of the object 
													which has to send 
													measurements */
	PTR_BUFFSTACK pBuffStack,					/* [IN] Pointer to the Buffer 
													Stack */
    ScanReportPerVarList* (*pScanRptPerVarList)[],	
                                                /* [IN] Pointer to an array of
                                                    Scan report per person */
	intu8 bConfirm,                             /* [IN] True for confirmed 
	                                                event report */ 
    intu16 ScanCount                            /* [IN] Number of Scans */	                                                
)
{
    ERR_CODE err = ERROR_SUCCESS;    
    intu16 AttrLen;
    intu16 i;
    ScanReportPerVarList* pScanReportPerVarList; 
    intu16 NumScans = 0;
    do
    {
        for(i = ScanCount; i > 0; i--)
        {
            pScanReportPerVarList = (*pScanRptPerVarList)[i - 1];    
            if(ERROR_SUCCESS != (err = PushScanPerVarList(pBuffStack, 
    			pScanReportPerVarList, handle, FALSE)))
            {
                break;
            }
            NumScans += ieee_ntohs(pScanReportPerVarList->count);
        }
        if(ERROR_SUCCESS != err)
        {
            break;
        }
        if(ERROR_SUCCESS != (err = PushScanReportInfoHeader(pBuffStack, 
            DATA_REQ_ID_AGENT_INITIATED, g_scan_report_num++, 
			NumScans, &AttrLen)))
        {
            break;
        }
        if(bConfirm)
        {
            if(ERROR_SUCCESS != (err = PushConfScanRptMPFixedHeader(pBuffStack, 
                handle, 0xFFFFFFFF, &AttrLen)))
            {
                break;
            }
        }
        else
        {
            if(ERROR_SUCCESS != (err = PushUnConfScanRptMPFixedHeader(pBuffStack, 
                handle, 0xFFFFFFFF, &AttrLen)))
            {
                break;
            }
        }
    }while(0);
    return err;
}
/**************************************************************************//*!
 *
 * @name  SetMDSEventRptTimeout
 *
 * @brief This function sets the timeout value for MDS confirmed event report
 *
 * @param pBuffStack			:	Pointer to the Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: always
 ******************************************************************************
 * This function sets the timeout value for MDS confirmed event report
 *****************************************************************************/
ERR_CODE SetMDSEventRptTimeout(
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to the Buffer Stack */
) 
{
    ERR_CODE err = ERROR_SUCCESS;
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    
    if(pMds->pMdcAttrConfirmTimeout != NULL) 
    {      
     /* Relative Time is based on 8KHz clock, Timer implementation is 
        based on milliseconds */
     pBuffStack->Timeout = (intu32)((*(pMds->pMdcAttrConfirmTimeout)) >> 3);
    }
    else 
    {      
     pBuffStack->Timeout = (intu32)(TO_CER); 
    }

    return err;
}
/**************************************************************************//*!
 *
 * @name  AgentSendFixedFormatEventRpt
 *
 * @brief This function sends the fixed format event report
 *
 * @param handle		:	Handle of the object which has to send 
 *							measurements
 * @param pObsScanList	:	Pointer to an array Observation Scan Lists
 * @param bConfirm  	:	True for confirmed event report
 * @param ScanCount  	:	Number of scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function calls other functions to create fixed format event report 
 * and sends it in case of success
 *****************************************************************************/    
ERR_CODE AgentSendFixedFormatEventRpt(
	HANDLE handle,						/* [IN] Handle of the object which has 
											to send measurements */
	ObservationScanList* (*pObsScanList)[],	
	                                    /* [IN] Pointer to an array of 
	                                        Observation Scan Lists */
    intu8 bConfirm,                     /* [IN] True for confirmed event 
                                            report */											
    intu16 ScanCount                    /* [IN] Number of scans */                                            
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 OutBuffSize = 0;
    PTR_BUFFSTACK pBuffStack;
    intu16 i;
    ObservationScanList* pObservationScanList;
    
    for(i = 0; i < ScanCount; i++)
    {
        pObservationScanList = (*pObsScanList)[i];
        OutBuffSize += 
            (intu16)GetFixedFormatEventRptSize(pObservationScanList);
    }
    OutBuffSize += GetScanRptHeaderSize();         
               
    pBuffStack = StackInit(OutBuffSize);    

    (void)SetMDSEventRptTimeout(pBuffStack);
    do
    {
        if(ERROR_SUCCESS != (err = PushFixedFormatEvntRpt(handle, pBuffStack, 
            pObsScanList, bConfirm, ScanCount)))
        {
            break;
        }
    
        err = ObjectSendEventReport(TRUE, pBuffStack);
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        StackDeInit(pBuffStack);
    }
    
    return err;    
}
/**************************************************************************//*!
 *
 * @name  AgentSendMPFixedFormatEventRpt
 *
 * @brief This function sends the multi person fixed format event report
 *
 * @param handle				:	Handle of the object which has to send 
 *									measurements
 * @param pScanRptPerVarList	:	Pointer to an array of Scan report per 
 *                                  person
 * @param bConfirm          	:	True for confirmed event report
 * @param ScanCount          	:	Number of Scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function calls other functions to create multi person fixed format 
 * event report and sends it in case of success
 *****************************************************************************/
ERR_CODE AgentSendMPFixedFormatEventRpt(
	HANDLE handle,								/* [IN] Handle of the object	
												    which has to send 
												    measurements */
	ScanReportPerVarList* (*pScanRptPerVarList)[],	
	                                            /* [IN] Pointer to an array 
	                                                Scan report per person */
	intu8 bConfirm,                             /* [IN] True for confirmed 
	                                                event report */ 
    intu16 ScanCount                            /* [IN] Number of Scans */	                                                
)
{
    ERR_CODE err = ERROR_SUCCESS;
    PTR_BUFFSTACK pBuffStack;
    ScanReportPerVarList* pScanReportPerVarList;
    intu16 i;
    intu16 OutBuffSize = 0;
    for(i = 0; i < ScanCount; i++)
    {
        pScanReportPerVarList = (*pScanRptPerVarList)[i];   
        OutBuffSize += 
            (intu16)GetMPFixedFormatEventRptSize(pScanReportPerVarList); 
    }
    OutBuffSize += GetScanRptHeaderSize();
    
    pBuffStack = StackInit(OutBuffSize);

    (void)SetMDSEventRptTimeout(pBuffStack);
    do
    {
        if(ERROR_SUCCESS != (err = PushMPFixedFormatEvntRpt(handle, 
            pBuffStack, pScanRptPerVarList, bConfirm, ScanCount)))
        {
            break;
        }
   
        err = ObjectSendEventReport(TRUE, pBuffStack);
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        StackDeInit(pBuffStack);
    }
    
    return err;    
}
/**************************************************************************//*!
 *
 * @name  AgentSendVarFormatEventRpt
 *
 * @brief This function sends the variable format event report
 *
 * @param handle		:	Handle of the object which has to send 
 *							measurements
 * @param pObsScanList	:	Pointer to an array Observation Scan Lists
 * @param bConfirm  	:	True for confirmed event report
 * @param ScanCount          	:	Number of Scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function calls other functions to create grouped format event report 
 * and sends it in case of success
 *****************************************************************************/    
ERR_CODE AgentSendVarFormatEventRpt(
	HANDLE handle,						/* [IN] Handle of the object which has 
											to send measurements */
	ObservationScanList* (*pObsScanList)[],	
	                                    /* [IN] Pointer to an array of 
	                                        Observation Scan Lists */
	intu8 bConfirm,                             /* [IN] True for confirmed 
	                                                event report */ 
    intu16 ScanCount                            /* [IN] Number of Scans */	                                                
)
{
    ERR_CODE err = ERROR_SUCCESS;
    
    intu16 OutBuffSize = 0;
    PTR_BUFFSTACK pBuffStack;
    intu16 i;
    ObservationScanList* pObservationScanList;
    
    for(i = 0; i < ScanCount; i++)
    {
        pObservationScanList = (*pObsScanList)[i];
        OutBuffSize += 
            (intu16)GetVarFormatEventRptSize(pObservationScanList);
    }
    OutBuffSize += GetScanRptHeaderSize();         
    
    pBuffStack = StackInit(OutBuffSize);
    
    (void)SetMDSEventRptTimeout(pBuffStack);

    do
    {
        if(ERROR_SUCCESS != (err = PushVarFormatEvntRpt(handle, pBuffStack, 
            pObsScanList, bConfirm, ScanCount)))
        {
            break;
        }
        err = ObjectSendEventReport(TRUE, pBuffStack);
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        StackDeInit(pBuffStack);
    }
    
    return err;    
}
/**************************************************************************//*!
 *
 * @name  AgentSendMPVarFormatEventRpt
 *
 * @brief This function sends the multi person variable format event report
 *
 * @param handle				:	Handle of the object which has to send 
 *									measurements
 * @param pScanRptPerVarList	:	Pointer to an array of Scan report per 
 *                                  person
 * @param bConfirm          	:	True for confirmed event report
 * @param ScanCount          	:	Number of Scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function calls other functions to create multi person variable format 
 * event report and sends it in case of success
 *****************************************************************************/
ERR_CODE AgentSendMPVarFormatEventRpt(
	HANDLE handle,								/* [IN] Handle of the object 
													which has to send 
													measurements */
	ScanReportPerVarList* (*pScanRptPerVarList)[],	
	                                            /* [IN] Pointer to an array 
	                                                Scan report per person */
	intu8 bConfirm,                             /* [IN] True for confirmed 
	                                                event report */ 
    intu16 ScanCount                            /* [IN] Number of Scans */	                                                
)
{
    ERR_CODE err = ERROR_SUCCESS;
    PTR_BUFFSTACK pBuffStack;
    ScanReportPerVarList* pScanReportPerVarList;
    intu16 i;
    intu16 OutBuffSize = 0;
    
    for(i = 0; i < ScanCount; i++)
    {
        pScanReportPerVarList = (*pScanRptPerVarList)[i];   
        OutBuffSize += 
            (intu16)GetMPVarFormatEventRptSize(pScanReportPerVarList); 
    }
    OutBuffSize += GetScanRptHeaderSize();
    
    pBuffStack = StackInit(OutBuffSize);
    
    (void)SetMDSEventRptTimeout(pBuffStack);

    do
    {
        if(ERROR_SUCCESS != (err = PushMPVarFormatEvntRpt(handle, pBuffStack, 
            pScanRptPerVarList, bConfirm, ScanCount)))
        {
            break;
        }
        err = ObjectSendEventReport(TRUE, pBuffStack);
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        StackDeInit(pBuffStack);
    }
    
    return err;    
}

/**************************************************************************//*!
 *
 * @name  PushGroupedFormatEvntRpt
 *
 * @brief This function pushes the grouped format event report onto the 
 *		  buffer stack
 *
 * @param handle		:	Handle of the object which has to send 
 *							measurements
 * @param pBuffStack	:	Pointer to the Buffer Stack
 * @param pObsScanList	:	Pointer to an array of Observation Scan List
 * @param ScanCount		:	Number of Scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function creates grouped format event report
 *****************************************************************************/
ERR_CODE PushGroupedFormatEvntRpt(
	HANDLE handle,						/* [IN] Handle of the object which has 
											to send measurements */
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to the Buffer Stack */
    ObservationScanList* (*pObsScanList)[],	
                                        /* [IN] Pointer to an array of 
                                            Observation Scan List */
	intu16 ScanCount					/* [IN] Number of Scans */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 i;
    intu16 AttrLen;
    ObservationScanList* pObservationScanList;
    intu16 ObjType;
    void* pObject = GetObjectByHandle(handle, &ObjType);
    intu8 bConfirm = FALSE;
    switch(ObjType)
    {
        case MDC_MOC_SCAN_CFG_EPI:
        {
            PEPICFGSCANNER pEpiCfgScanner = (PEPICFGSCANNER)pObject;
            if(pEpiCfgScanner->pCfgScanner->MdcAttrConfirmMode == CONFIRMED)
            {
                bConfirm = TRUE;
            }
            if(pEpiCfgScanner->pCfgScanner->OptionalAttributeFlag & 
                OPT_CFGSCAN_CONFIRM_TIMEOUT)
            {
                pBuffStack->Timeout = (intu32)
                    (pEpiCfgScanner->pCfgScanner->MdcAttrConfirmTimeout >> 3);
            }
        }
            break;

        case MDC_MOC_SCAN_CFG_PERI:
        {
            PPERICFGSCANNER pPeriCfgScanner = (PPERICFGSCANNER)pObject;
            if(pPeriCfgScanner->pCfgScanner->MdcAttrConfirmMode == CONFIRMED)
            {
                bConfirm = TRUE;
            }
            if(pPeriCfgScanner->pCfgScanner->OptionalAttributeFlag & 
                OPT_CFGSCAN_CONFIRM_TIMEOUT)
            {
                pBuffStack->Timeout = (intu32)
                    (pPeriCfgScanner->pCfgScanner->MdcAttrConfirmTimeout >> 3);
            }
        }
            break;
    }
    
    do
    {
        for(i = ScanCount; i > 0; i--)
        {            
            pObservationScanList = (*pObsScanList)[i - 1];
            if(ERROR_SUCCESS != (err = PushObsScanList(pBuffStack, 
                pObservationScanList, &AttrLen, handle, TRUE)))
            {
                break;
            }            
        }
        if(ERROR_SUCCESS != err)
        {
            break;
        }
        if(ERROR_SUCCESS != (err = PushScanReportInfoHeader(pBuffStack, 
            DATA_REQ_ID_AGENT_INITIATED, g_scan_report_num++, ScanCount, &AttrLen)))
        {
            break;
        }
        if(ObjType == MDC_MOC_SCAN_CFG_EPI)
        {            
            if(bConfirm)
            {                
                err = PushConfUnBufScanRptGrpHeader(pBuffStack, handle, 0xFFFFFFFF, &AttrLen);
            }
            else
            {
                err = PushUnConfUnBufScanRptGrpHeader(pBuffStack, handle, 0xFFFFFFFF, &AttrLen);
            }
        }
        else if(ObjType == MDC_MOC_SCAN_CFG_PERI)
        {            
            if(bConfirm)
            {                
                err = PushConfBufScanRptGrpHeader(pBuffStack, handle, 0xFFFFFFFF, &AttrLen);
            }
            else
            {
                err = PushUnConfBufScanRptGrpHeader(pBuffStack, handle, 0xFFFFFFFF, &AttrLen);
            }
        }
        if(ERROR_SUCCESS != err)                            
        {
            break;
        }
    }while(0);
    return err;
}
/**************************************************************************//*!
 *
 * @name  PushMPGroupedFormatEvntRpt
 *
 * @brief This function pushes the multi person grouped format event report 
 *		  onto the buffer stack
 *
 * @param handle				:	Handle of the object which has to send 
 *									measurements
 * @param pBuffStack			:	Pointer to the Buffer Stack
 * @param pScanRptPerVarList	:	Pointer to the Scan report per person
 * @param ScanCount 			:	Number of Scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function creates multi person grouped format event report
 *****************************************************************************/
ERR_CODE PushMPGroupedFormatEvntRpt(
	HANDLE handle,								/* [IN] Handle of the object 
													which has to send 
													measurements */ 
	PTR_BUFFSTACK pBuffStack,					/* [IN] Pointer to the Buffer 
													Stack */
    ScanReportPerVarList* (*pScanRptPerVarList)[],
                                            	/* [IN] Pointer to the Scan 
													report per person */
    intu16 ScanCount                            /* [IN] Number of Scans */													
)
{
    ERR_CODE err = ERROR_SUCCESS;    
    intu16 AttrLen;
    intu16 ObjType;
    void* pObject = GetObjectByHandle(handle, &ObjType);
    intu8 bConfirm = FALSE;
    ScanReportPerVarList* pScanReportPerVarList;
    intu16 i;
    intu16 NumScans = 0;
    switch(ObjType)
    {
        case MDC_MOC_SCAN_CFG_EPI:
        {
            PEPICFGSCANNER pEpiCfgScanner = (PEPICFGSCANNER)pObject;
            if(pEpiCfgScanner->pCfgScanner->MdcAttrConfirmMode == CONFIRMED)
            {
                bConfirm = TRUE;
            }
            if(pEpiCfgScanner->pCfgScanner->OptionalAttributeFlag & 
                OPT_CFGSCAN_CONFIRM_TIMEOUT)
            {
                pBuffStack->Timeout = (intu32)
                    (pEpiCfgScanner->pCfgScanner->MdcAttrConfirmTimeout >> 3);
            }
        }
            break;

        case MDC_MOC_SCAN_CFG_PERI:
        {
            PPERICFGSCANNER pPeriCfgScanner = (PPERICFGSCANNER)pObject;
            if(pPeriCfgScanner->pCfgScanner->MdcAttrConfirmMode == CONFIRMED)
            {
                bConfirm = TRUE;
            }
            if(pPeriCfgScanner->pCfgScanner->OptionalAttributeFlag & 
                OPT_CFGSCAN_CONFIRM_TIMEOUT)
            {
                pBuffStack->Timeout = (intu32)
                    (pPeriCfgScanner->pCfgScanner->MdcAttrConfirmTimeout >> 3);
            }
        }
            break;
    }
    do
    {
        for(i = ScanCount; i >0; i--)
        {
            pScanReportPerVarList = (*pScanRptPerVarList)[i - 1];
            if(ERROR_SUCCESS != (err = PushScanPerVarList(pBuffStack, 
    			pScanReportPerVarList, handle, TRUE)))
            {
                break;
            }
            NumScans += ieee_ntohs(pScanReportPerVarList->count); 
        }
        if(ERROR_SUCCESS != err)
        {
            break;
        }
        if(ERROR_SUCCESS != (err = PushScanReportInfoHeader(pBuffStack, 
            DATA_REQ_ID_AGENT_INITIATED, g_scan_report_num++, NumScans, &AttrLen)))
        {
            break;
        }
        if(ObjType == MDC_MOC_SCAN_CFG_EPI)
        {            
            if(bConfirm)
            {                
                err = PushConfUnBufScanRptMPGrpHeader(pBuffStack, handle, 0xFFFFFFFF, &AttrLen);
            }
            else
            {
                err = PushUnConfUnBufScanRptMPGrpHeader(pBuffStack, handle, 0xFFFFFFFF, &AttrLen);
            }
        }
        else if(ObjType == MDC_MOC_SCAN_CFG_PERI)
        {            
            if(bConfirm)
            {                
                err = PushConfBufScanRptMPGrpHeader(pBuffStack, handle, 0xFFFFFFFF, &AttrLen);
            }
            else
            {
                err = PushUnConfBufScanRptMPGrpHeader(pBuffStack, handle, 0xFFFFFFFF, &AttrLen);
            }
        }
        if(ERROR_SUCCESS != err)                            
        {
            break;
        }
    }while(0);
    return err;
}
/**************************************************************************//*!
 *
 * @name  AgentSendGroupedFormatEventRpt
 *
 * @brief This function sends the grouped format event report
 *
 * @param handle		:	Handle of the object which has to send 
 *							measurements
 * @param pObsScanList	:	Pointer to an array of Observation Scan List
 * @param ScanCount		:	Number of scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function calls other functions to create grouped format event report 
 * and sends it in case of success
 *****************************************************************************/    
ERR_CODE AgentSendGroupedFormatEventRpt(
	HANDLE handle,						/* [IN] Handle of the object which has 
											to send measurements */
	ObservationScanList* (*pObsScanList)[],	
	                                    /* [IN] Pointer to an array of 
	                                        Observation Scan List */
	intu16 ScanCount					/* [IN] Number of scans */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 OutBuffSize = 0;
    PTR_BUFFSTACK pBuffStack;
    OutBuffSize += (intu16)((GetGroupedFormatEventRptSize(handle) * ScanCount) +
		GetScanRptHeaderSize());
    
    pBuffStack = StackInit(OutBuffSize);    

    do
    {
        if(ERROR_SUCCESS != (err = PushGroupedFormatEvntRpt(handle, pBuffStack, pObsScanList, ScanCount)))
        {
            break;
        }
        err = ObjectSendEventReport(TRUE, pBuffStack);
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        StackDeInit(pBuffStack);
    }
    
    return err;    
}
/**************************************************************************//*!
 *
 * @name  AgentSendMPGroupedFormatEventRpt
 *
 * @brief This function sends the multi person grouped format event report
 *
 * @param handle				:	Handle of the object which has to send 
 *									measurements
 * @param pScanRptPerVarList	:	Pointer to an array of Scan report per 
 *                                  person
 * @param ScanCount         	:	Number of Scans
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function calls other functions to create multi person grouped format 
 * event report and sends it in case of success
 *****************************************************************************/
ERR_CODE AgentSendMPGroupedFormatEventRpt(
	HANDLE handle,								/* [IN] Handle of the object 
													which has to send 
													measurements */
	ScanReportPerVarList* (*pScanRptPerVarList)[],	
	                                            /* [IN] Pointer to an array of 
	                                                Scan report per person */
    intu16 ScanCount                            /* [IN] Number of Scans */	                                                
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 OutBuffSize = 0;
    PTR_BUFFSTACK pBuffStack;
    intu16 i;
    ScanReportPerVarList* pScanReportPerVarList;
    for(i = 0; i < ScanCount; i++)
    {
        pScanReportPerVarList = (*pScanRptPerVarList)[i];                
        OutBuffSize += (intu16)((GetGroupedFormatEventRptSize(handle) + 
		    sizeof(PersonId)) * (ieee_ntohs(pScanReportPerVarList->count)));
    }
    OutBuffSize += GetScanRptHeaderSize();
    
    pBuffStack = StackInit(OutBuffSize);    

    do
    {
        if(ERROR_SUCCESS != (err = PushMPGroupedFormatEvntRpt(handle, 
            pBuffStack, pScanRptPerVarList, ScanCount)))
        {
            break;
        }
        err = ObjectSendEventReport(TRUE, pBuffStack);
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        StackDeInit(pBuffStack);
    }
    return err;    
}

/**************************************************************************//*!
 *
 * @name  UpdateMetricAttribute
 *
 * @brief This function validates and updates the Metric base class attribute 
 *		  changed 
 *
 * @param pObject	:	Pointer to the Metric base class
 * @param pAvaType	:	Pointer to the new attribute value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function checks whether the attribute that has changed, can be changed 
 * after configuration is accepted by the host and updates the DIM if input is 
 * valid
 *****************************************************************************/
ERR_CODE UpdateMetricAttribute(
    PMETRIC pObject,		/* [IN] Pointer to the Metric base class */ 
    AVA_Type* pAvaType		/* [IN] Pointer to the new attribute value */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;
    switch(ieee_ntohs(pAvaType->attribute_id))
    {
        case MDC_ATTR_METRIC_SPEC_SMALL:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(MetricSpecSmall))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrMetricSpecSmall = 
                ieee_ntohs(pAvaType->attribute_value.value[0]);
            break;
        case MDC_ATTR_METRIC_STRUCT_SMALL:
        
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(MetricStructureSmall))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrMetricStructSmall.ms_struct = 
                pAvaType->attribute_value.value[0];
            pObject->MdcAttrMetricStructSmall.ms_comp_no = 
                pAvaType->attribute_value.value[1];
            pObject->OptionalAttributeFlag |= OPT_MET_STRUCT_SMALL;
            break;
        case MDC_ATTR_MSMT_STAT:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(MeasurementStatus))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrMetricSpecSmall = 
                ieee_ntohs(pAvaType->attribute_value.value[0]);
            pObject->OptionalAttributeFlag |= OPT_MET_MSMT_STAT;
            break;
        case MDC_ATTR_ID_PHYSIO:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(OID_Type))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MetricIdPhysioList.ID_type = METRIC_ID;
            *pObject->MetricIdPhysioList.u.
                pMdcAttrIdPhysio = 
                ieee_ntohs(pAvaType->attribute_value.value[0]);
            pObject->OptionalAttributeFlag |= OPT_MET_ID_PHYS;
            break;
        case MDC_ATTR_ID_PHYSIO_LIST:
        {
            MetricIdList *pMetricIdList = (MetricIdList *)
                &pAvaType->attribute_value.value[0];
            int i;
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                (sizeof(intu16) * 2 + ieee_ntohs(pMetricIdList->length)))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            
            pObject->MetricIdPhysioList.ID_type = METRIC_ID_LIST;
            pObject->MetricIdPhysioList.u.pMdcAttrIdPhysioList->count = 
                ieee_ntohs(pMetricIdList->count);
            pObject->MetricIdPhysioList.u.pMdcAttrIdPhysioList->length = 
                ieee_ntohs(pMetricIdList->length);
                
            for(i =0; i < ieee_ntohs(pMetricIdList->count); i++)
            {
                pObject->MetricIdPhysioList.u.pMdcAttrIdPhysioList->value[i] = 
                    ieee_ntohs(pMetricIdList->value[i]);
            }
                
            pObject->OptionalAttributeFlag &= ~OPT_MET_ID_PHYS;
            break;
        }
        case MDC_ATTR_METRIC_ID_PART:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(NomPartition))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrMetricIdPart = 
                ieee_ntohs(pAvaType->attribute_value.value[0]);
            pObject->OptionalAttributeFlag |= OPT_MET_ID_PART;
            break;
        case MDC_ATTR_UNIT_CODE:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(OID_Type))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrUnitCode = 
                ieee_ntohs(pAvaType->attribute_value.value[0]);
            pObject->OptionalAttributeFlag |= OPT_MET_UNIT_CODE;
            break;
        case MDC_ATTR_ATTRIBUTE_VAL_MAP:
        {
            AttrValMap* pAttrValMap = (AttrValMap*)
                &pAvaType->attribute_value.value[0];
            intu16 i;
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                (ieee_ntohs(pAttrValMap->length) + ATTR_HEADER_SIZE))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->pMdcAttrAttributeValMap->count = ieee_ntohs(pAttrValMap->count);
            pObject->pMdcAttrAttributeValMap->length = ieee_ntohs(pAttrValMap->length);
            
            for(i = 0; i < ieee_ntohs(pAttrValMap->count); i++)
            {
                pObject->pMdcAttrAttributeValMap->value[i].attribute_id = 
                    ieee_ntohs(pAttrValMap->value[i].attribute_id);    
                pObject->pMdcAttrAttributeValMap->value[i].attribute_len = 
                    ieee_ntohs(pAttrValMap->value[i].attribute_len);    
            }

            pObject->OptionalAttributeFlag |= OPT_MET_ATTRVALMAP;
        }
        break;
        case MDC_ATTR_SOURCE_HANDLE_REF:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(HANDLE))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrSourceHandleRef = 
                ieee_ntohs(pAvaType->attribute_value.value[0]);
            pObject->OptionalAttributeFlag |= OPT_MET_SRC_HNDL_REF;
            break;
        case MDC_ATTR_ID_LABEL_STRING:
        {
            octet_string* pLabelString = (octet_string*)
                &pAvaType->attribute_value.value[0];
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                (OCTET_STRING_HEADER_SIZE + ieee_ntohs(pLabelString->length)))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->pMdcAttrIdLabelString->length = ieee_ntohs(pLabelString->length);

            (void)memcpy(&pObject->pMdcAttrIdLabelString->value[0], 
                &pLabelString->value[0], ieee_ntohs(pLabelString->length));

            pObject->OptionalAttributeFlag |= OPT_MET_ID_LABEL;
        }
        break;

        case MDC_ATTR_UNIT_LABEL_STRING:
        {
            octet_string* pUnitLabelString = (octet_string*)
                &pAvaType->attribute_value.value[0];
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                (OCTET_STRING_HEADER_SIZE + ieee_ntohs(pUnitLabelString->length)))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->pMdcAttrUnitLabelString->length = ieee_ntohs(pUnitLabelString->length);

            (void)memcpy(&pObject->pMdcAttrUnitLabelString->value[0], 
                &pUnitLabelString->value[0], ieee_ntohs(pUnitLabelString->length));
            pObject->OptionalAttributeFlag |= OPT_MET_UNIT_LABEL;
        }
        break;

        case MDC_ATTR_TIME_STAMP_ABS:
        case MDC_ATTR_TIME_STAMP_REL:
        case MDC_ATTR_TIME_STAMP_REL_HI_RES:
            break;
        case MDC_ATTR_TIME_PD_MSMT_ACTIVE:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(FLOAT_Type))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrTimePdMsmtActive = 
                ieee_ntohl(pAvaType->attribute_value.value[0]);
            pObject->OptionalAttributeFlag |= OPT_MET_MSMT_ACTIVE;
            break;
        default:
            err = ERROR_INVALID_PARAM;
            break;
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  UpdateNumericAttribute
 *
 * @brief This function validates and updates the Numeric class attribute 
 *		  changed 
 *
 * @param pObject	:	Pointer to the Numeric class
 * @param pAvaType	:	Pointer to the new attribute value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function checks whether the attribute that has changed, can be changed 
 * after configuration is accepted by the host and updates the DIM if input is 
 * valid
 *****************************************************************************/
ERR_CODE UpdateNumericAttribute(
    PNUMERIC pObject,		/* [IN] Pointer to the Numeric class */ 
    AVA_Type* pAvaType		/* [IN] Pointer to the new attribute value */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;
    switch(ieee_ntohs(pAvaType->attribute_id))
    {
        case MDC_ATTR_NU_VAL_OBS_SIMP:
        case MDC_ATTR_NU_CMPD_VAL_OBS_SIMP:
        case MDC_ATTR_NU_VAL_OBS_BASIC:
        case MDC_ATTR_NU_CMPD_VAL_OBS_BASIC:
        case MDC_ATTR_NU_VAL_OBS:
        case MDC_ATTR_NU_CMPD_VAL_OBS:
            break;
        default:
            err = UpdateMetricAttribute(pObject->pMetric, pAvaType);
            break;
    }
    return err;
    
}


/**************************************************************************//*!
 *
 * @name  UpdateRtsaAttribute
 *
 * @brief This function validates and updates the RT-SA class attribute 
 *		  changed 
 *
 * @param pObject	:	Pointer to the RT-SA class
 * @param pAvaType	:	Pointer to the new attribute value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function checks whether the attribute that has changed, can be changed 
 * after configuration is accepted by the host and updates the DIM if input is 
 * valid
 *****************************************************************************/
ERR_CODE UpdateRtsaAttribute(
    PRTSA pObject,			/* [IN] Pointer to the RT-SA class */ 
    AVA_Type* pAvaType		/* [IN] Pointer to the new attribute value */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;
    switch(ieee_ntohs(pAvaType->attribute_id))
    {
        case MDC_ATTR_SIMP_SA_OBS_VAL:
            break;
        default:
            err = UpdateMetricAttribute(pObject->pMetric, pAvaType);
            break;
    }
    return err;
    
}
/**************************************************************************//*!
 *
 * @name  UpdateEnumAttribute
 *
 * @brief This function validates and updates the Enumeration class attribute 
 *		  changed 
 *
 * @param pObject	:	Pointer to the Enumeration class
 * @param pAvaType	:	Pointer to the new attribute value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function checks whether the attribute that has changed, can be changed 
 * after configuration is accepted by the host and updates the DIM if input is 
 * valid
 *****************************************************************************/
ERR_CODE UpdateEnumAttribute(
    PENUMERATION pObject,		/* [IN] Pointer to the Enumeration class */
    AVA_Type* pAvaType			/* [IN] Pointer to the new attribute value */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    switch(ieee_ntohs(pAvaType->attribute_id))
    {
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_OID:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_BIT_STR:
        case MDC_ATTR_ENUM_OBS_VAL_BASIC_BIT_STR:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_STR:
        case MDC_ATTR_VAL_ENUM_OBS:
            break;
        case MDC_ATTR_ENUM_OBS_VAL_PART:
            if(ieee_ntohs(pAvaType->attribute_value.length) != sizeof(NomPartition))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrEnumObsValPart = ieee_ntohs(pAvaType->attribute_value.value[0]);
            pObject->OptionalAttributeFlag |= OPT_ENUM_OBS_VAL_PART;
            break;
        default:
            err = UpdateMetricAttribute(pObject->pMetric, pAvaType);
            break;
    }
    return err;    
}

/**************************************************************************//*!
 *
 * @name  UpdatePmStoreAttribute
 *
 * @brief This function validates and updates the PM Store class attribute 
 *		  changed 
 *
 * @param pObject	:	Pointer to the PM Store class
 * @param pAvaType	:	Pointer to the new attribute value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function checks whether the attribute that has changed, can be changed 
 * after configuration is accepted by the host and updates the DIM if input is 
 * valid
 *****************************************************************************/
ERR_CODE UpdatePmStoreAttribute(
    PPMSTORE pObject,		/* [IN] Pointer to the PM Store class */ 
    AVA_Type* pAvaType		/* [IN] Pointer to the new attribute value */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;
    switch(ieee_ntohs(pAvaType->attribute_id))
    {
        case MDC_ATTR_METRIC_STORE_USAGE_CNT:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(intu32))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrMetricStoreUsageCnt = 
                ieee_ntohl(pAvaType->attribute_value.value[0]);
            pObject->OptionalAttributeFlag |= OPT_PMSTORE_USAGE_CNT;
            break;
        case MDC_ATTR_OP_STAT:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(OperationalState))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrOpState = 
                ieee_ntohs(pAvaType->attribute_value.value[0]);
            break;
        case MDC_ATTR_NUM_SEG:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(intu16))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrNumSeg = 
                ieee_ntohs(pAvaType->attribute_value.value[0]);
            break;
        case MDC_ATTR_CLEAR_TIMEOUT:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(RelativeTime))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrClearTimeout = 
                ieee_ntohl(pAvaType->attribute_value.value[0]);
            break;
        default:
            err = ERROR_INVALID_PARAM;
            break;
    }
    return err;
    
}

/**************************************************************************//*!
 *
 * @name  UpdateScannerAttribute
 *
 * @brief This function validates and updates the Scanner base class attribute 
 *		  changed 
 *
 * @param pObject	:	Pointer to the scanner base class
 * @param pAvaType	:	Pointer to the new attribute value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function checks whether the attribute that has changed, can be changed 
 * after configuration is accepted by the host and updates the DIM if input is 
 * valid
 *****************************************************************************/
ERR_CODE UpdateScannerAttribute(
    PSCANNER pObject,		/* [IN] Pointer to the scanner base class */ 
    AVA_Type* pAvaType		/* [IN] Pointer to the new attribute value */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;
    switch(pAvaType->attribute_id)
    {
        case MDC_ATTR_SCAN_HANDLE_ATTR_VAL_MAP:
        {
            HandleAttrValMap* pHandleAttrValMap = 
                (HandleAttrValMap*)&pAvaType->attribute_value.value[0];
            intu16 i;
            HandleAttrValMapEntry *pSrc, *pDest;
            if(ieee_ntohs(pAvaType->attribute_value.length) != (ieee_ntohs(pHandleAttrValMap->length) + 
                ATTR_HEADER_SIZE))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->pMdcAttrScanHandleAttrValMap->count = 
                ieee_ntohs(pHandleAttrValMap->count);
            pObject->pMdcAttrScanHandleAttrValMap->length = 
                ieee_ntohs(pHandleAttrValMap->length);
            pSrc = &pHandleAttrValMap->value[0];
            pDest = &pObject->pMdcAttrScanHandleAttrValMap->value[0];
            for(i = 0; i < ieee_ntohs(pHandleAttrValMap->count); i++)
            {
                intu16 j;
                AttrValMapEntry *pSrcEnt, *pDestEnt;
                pDest->obj_handle = ieee_ntohs(pSrc->obj_handle);
                pDest->attr_val_map.count = ieee_ntohs(pSrc->attr_val_map.count);
                pDest->attr_val_map.length = ieee_ntohs(pSrc->attr_val_map.length);
                pSrcEnt = &pSrc->attr_val_map.value[0];
                pDestEnt = &pDest->attr_val_map.value[0];
                for(j = 0; j < pDest->attr_val_map.count; j++)
                {
                    pDestEnt[j].attribute_id = ieee_ntohs(pSrcEnt[j].attribute_id);
                    pDestEnt[j].attribute_len = ieee_ntohs(pSrcEnt[j].attribute_len);
                }
                (intu8*)pSrc = (intu8*)&pSrcEnt[j];
                (intu8*)pDest = (intu8*)&pDestEnt[j];
            }
            break;
        }
        default:
            err = ERROR_INVALID_PARAM;
            break;
    }
    return err;

}


/**************************************************************************//*!
 *
 * @name  UpdateCfgScannerAttribute
 *
 * @brief This function validates and updates the configurable Scanner 
 *		  attribute changed 
 *
 * @param pObject	:	Pointer to the configurable scanner class
 * @param pAvaType	:	Pointer to the new attribute value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function checks whether the attribute that has changed, can be changed 
 * after configuration is accepted by the host and updates the DIM if input is 
 * valid
 *****************************************************************************/
ERR_CODE UpdateCfgScannerAttribute(
    PCFGSCANNER pObject,	/* [IN] Pointer to the configurable scanner class*/
    AVA_Type* pAvaType		/* [IN] Pointer to the new attribute value */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;
    switch(ieee_ntohs(pAvaType->attribute_id))
    {
        case MDC_ATTR_CONFIRM_MODE:
            if(ieee_ntohs(pAvaType->attribute_value.length) != sizeof(ConfirmMode))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrConfirmMode = ieee_ntohs(pAvaType->attribute_value.value[0]);
            break;
        
        case MDC_ATTR_CONFIRM_TIMEOUT:
            if(ieee_ntohs(pAvaType->attribute_value.length) != sizeof(RelativeTime))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrConfirmTimeout = ieee_ntohl(pAvaType->attribute_value.value[0]);
            pObject->OptionalAttributeFlag |= OPT_CFGSCAN_CONFIRM_TIMEOUT;
            break;
        default:
            err = UpdateScannerAttribute(pObject->pScanner, pAvaType);
            break;
    }
    return err;

}

/**************************************************************************//*!
 *
 * @name  UpdateEpiScannerAttribute
 *
 * @brief This function validates and updates the episoidic Scanner attribute 
 *		  changed 
 *
 * @param pObject	:	Pointer to the episoidic scanner class
 * @param pAvaType	:	Pointer to the new attribute value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function checks whether the attribute that has changed, can be changed 
 * after configuration is accepted by the host and updates the DIM if input is 
 * valid
 *****************************************************************************/
ERR_CODE UpdateEpiScannerAttribute(
    PEPICFGSCANNER pObject,	/* [IN] Pointer to the episoidic scanner class */ 
    AVA_Type* pAvaType		/* [IN] Pointer to the new attribute value */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;
    switch(ieee_ntohs(pAvaType->attribute_id))
    {
        case MDC_ATTR_SCAN_REP_PD_MIN:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(RelativeTime))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrScanRepPdMin = 
                ieee_ntohl(pAvaType->attribute_value.value[0]);
            pObject->OptionalAttributeFlag |= OPT_EPISCAN_MIN_RPT_INT;
            break;
        default:
            err = UpdateCfgScannerAttribute(pObject->pCfgScanner, pAvaType);
            break;
    }
    return err;
    
}

/**************************************************************************//*!
 *
 * @name  UpdatePeriScannerAttribute
 *
 * @brief This function validates and updates the Periodic Scanner attribute 
 *		  changed 
 *
 * @param pObject	:	Pointer to the periodic scanner class
 * @param pAvaType	:	Pointer to the new attribute value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function checks whether the attribute that has changed, can be changed 
 * after configuration is accepted by the host and updates the DIM if input is 
 * valid
 *****************************************************************************/
ERR_CODE UpdatePeriScannerAttribute(
    PPERICFGSCANNER pObject,	/* [IN] Pointer to the periodic scanner class*/ 
    AVA_Type* pAvaType			/* [IN] Pointer to the new attribute value */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;
    switch(ieee_ntohs(pAvaType->attribute_id))
    {
        case MDC_ATTR_SCAN_REP_PD:
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(RelativeTime))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrScanRepPd = 
                ieee_ntohl(pAvaType->attribute_value.value[0]);
            break;
        default:
            err = UpdateCfgScannerAttribute(pObject->pCfgScanner, pAvaType);
            break;
    }
    return err;
    
}

/**************************************************************************//*!
 *
 * @name  UpdateMdsAttribute
 *
 * @brief This function validates the MDS attribute changed 
 *
 * @param pObject	:	Pointer to the MDS
 * @param pAvaType	:	Pointer to the new attribute value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function checks whether the attribute that has changed, can be changed 
 * after configuration is accepted by the host
 *****************************************************************************/
ERR_CODE UpdateMdsAttribute(
    PMDS pObject,			/* [IN] Pointer to the MDS */ 
    AVA_Type* pAvaType		/* [IN] Pointer to the new attribute value */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;
    UNUSED(pObject)
    switch(ieee_ntohs(pAvaType->attribute_id))
    {
        case MDC_ATTR_ATTRIBUTE_VAL_MAP:
        {
            AttrValMap* pAttrValMap = (AttrValMap*)
                &pAvaType->attribute_value.value[0];
            intu16 i;
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                (ieee_ntohs(pAttrValMap->length) + ATTR_HEADER_SIZE))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            if(pObject->pMdcAttrValMap != NULL) 
            {              
             pObject->pMdcAttrValMap->count = ieee_ntohs(pAttrValMap->count);
             pObject->pMdcAttrValMap->length = ieee_ntohs(pAttrValMap->length);
            
             for(i = 0; i < ieee_ntohs(pAttrValMap->count); i++)
             {
                pObject->pMdcAttrValMap->value[i].attribute_id = 
                    ieee_ntohs(pAttrValMap->value[i].attribute_id);    
                pObject->pMdcAttrValMap->value[i].attribute_len = 
                    ieee_ntohs(pAttrValMap->value[i].attribute_len);    
             }
            }
            else 
            {
                err = ERROR_INVALID_PARAM;
                break;             
            }
            
        }
        break;
        case MDC_ATTR_MDS_TIME_INFO:
        {
            MdsTimeInfo* pMdsTimeInfo = (MdsTimeInfo*)
                &pAvaType->attribute_value.value[0];
            if(ieee_ntohs(pAvaType->attribute_value.length) != sizeof(MdsTimeInfo))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            
            if(pObject->pMdcAttrMdsTimeInfo != NULL) 
            {              
             pObject->pMdcAttrMdsTimeInfo->mds_time_cap_state = 
                 ieee_ntohs(pMdsTimeInfo->mds_time_cap_state);                
             pObject->pMdcAttrMdsTimeInfo->time_sync_protocol = 
                 ieee_ntohs(pMdsTimeInfo->time_sync_protocol);
             pObject->pMdcAttrMdsTimeInfo->time_sync_accuracy = 
                 ieee_ntohl(pMdsTimeInfo->time_sync_accuracy);
             pObject->pMdcAttrMdsTimeInfo->time_resolution_abs_time = 
                 ieee_ntohs(pMdsTimeInfo->time_resolution_abs_time);
             pObject->pMdcAttrMdsTimeInfo->time_resolution_rel_time = 
                 ieee_ntohs(pMdsTimeInfo->time_resolution_rel_time);
             pObject->pMdcAttrMdsTimeInfo->time_resolution_high_res_time = 
                 ieee_ntohl(pMdsTimeInfo->time_resolution_high_res_time);
            }
            else 
            {
                err = ERROR_INVALID_PARAM;
                break;             
            }            
        }
        break;                
        case MDC_ATTR_TIME_ABS:
        {
            AbsoluteTime* pAbsTime = (AbsoluteTime*)
                &pAvaType->attribute_value.value[0];
            if(ieee_ntohs(pAvaType->attribute_value.length) != sizeof(AbsoluteTime))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            if(pObject->pMdcAttrTimeAbs != NULL)
            {              
             (void)memcpy(pObject->pMdcAttrTimeAbs, pAbsTime, sizeof(AbsoluteTime));
            }
            else 
            {
                err = ERROR_INVALID_PARAM;
                break;             
            }
        }
        break;
        case MDC_ATTR_TIME_REL:
        {
            if(ieee_ntohs(pAvaType->attribute_value.length) != sizeof(RelativeTime))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            pObject->MdcAttrTimeRel = 
                ieee_ntohl(pAvaType->attribute_value.value[0]);
        }
        break;
        case MDC_ATTR_TIME_REL_HI_RES:
        {
            HighResRelativeTime* pHighResRelTime = (HighResRelativeTime*)
                &pAvaType->attribute_value.value[0];
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(HighResRelativeTime))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            (void)memcpy(&pObject->MdcAttrTimeRelHiRes, pHighResRelTime, 
                sizeof(HighResRelativeTime));
        }
        break;
        
        case MDC_ATTR_TIME_ABS_ADJUST:
        {
            AbsoluteTimeAdjust* pAbsTimeAdjust = (AbsoluteTimeAdjust*)
                &pAvaType->attribute_value.value[0];
            if(ieee_ntohs(pAvaType->attribute_value.length) != 
                sizeof(AbsoluteTimeAdjust))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            if(pObject->pMdcAttrTimeAbsAdjust != NULL) 
            {              
             (void)memcpy(pObject->pMdcAttrTimeAbsAdjust, pAbsTimeAdjust, 
                sizeof(AbsoluteTimeAdjust));
            }
            else 
            {
                err = ERROR_INVALID_PARAM;
                break;             
            }            
        }
        break;        
        case MDC_ATTR_POWER_STAT:
        {
            if(ieee_ntohs(pAvaType->attribute_value.length) != sizeof(PowerStatus))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            if(pObject->pMdcAttrPowerStatus != NULL) 
            {              
             *(pObject->pMdcAttrPowerStatus) = ieee_ntohs(pAvaType->attribute_value.value[0]);
            }
            else 
            {
                err = ERROR_INVALID_PARAM;
                break;             
            }            
        }
        break;
        
        case MDC_ATTR_VAL_BATT_CHARGE:
        {
            if(ieee_ntohs(pAvaType->attribute_value.length) != sizeof(intu16))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            
            if(pObject->pMdcAttrValBattCharge != NULL) 
            {              
             *(pObject->pMdcAttrValBattCharge) = ieee_ntohs(pAvaType->attribute_value.value[0]);
            }
            else 
            {
                err = ERROR_INVALID_PARAM;
                break;             
            }            
        }
        break;
        
        case MDC_ATTR_TIME_BATT_REMAIN:
        {
            BatMeasure* pBatMeasure = (BatMeasure*)&pAvaType->attribute_value.value[0];
            if(ieee_ntohs(pAvaType->attribute_value.length) != sizeof(BatMeasure))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            if(pObject->pMdcAttrTimeBattRemain != NULL) 
            {              
             pObject->pMdcAttrTimeBattRemain->value = ieee_ntohl(pBatMeasure->value);
             pObject->pMdcAttrTimeBattRemain->unit = ieee_ntohs(pBatMeasure->unit);
            }
            else 
            {
                err = ERROR_INVALID_PARAM;
                break;             
            }
            
        }
        case MDC_ATTR_CONFIRM_TIMEOUT:
        {
            if(ieee_ntohs(pAvaType->attribute_value.length) != sizeof(RelativeTime))
            {
                err = ERROR_INVALID_PARAM;
                break;
            }
            if(pObject->pMdcAttrConfirmTimeout != NULL) 
            {              
             *(pObject->pMdcAttrConfirmTimeout) = ieee_ntohl(pAvaType->attribute_value.value[0]);
            } 
            else 
            {
                err = ERROR_INVALID_PARAM;
                break;             
            }
        }
            break;
        default:
            return ERROR_INVALID_PARAM;
            break;
    }
    return err;
}
/**************************************************************************//*!
 *
 * @name  UpdateAttribute
 *
 * @brief This function is updates the attribute value in the DIM
 *
 * @param ObjType	:	Type of object class
 * @param pObject	:	Pointer to the object class
 * @param pAvaType	:	Pointer to the new attribute value
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are not correct
 ******************************************************************************
 * This function updates the attribute value in the DIM
 *****************************************************************************/
ERR_CODE UpdateAttribute(
    intu16 ObjType,			/* [IN] Type of object class */ 
    void* pObject,			/* [IN] Pointer to the object class */ 
    AVA_Type* pAvaType		/* [IN] Pointer to the new attribute value */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;   
    switch(ObjType)
    {    
        case MDC_MOC_VMS_MDS_SIMP:
            err = UpdateMdsAttribute((PMDS)pObject, pAvaType);
            break;
        case MDC_MOC_VMO_METRIC_NU:
            err = UpdateNumericAttribute((PNUMERIC)pObject, pAvaType);
            break;
        case MDC_MOC_VMO_METRIC_SA_RT:
            err = UpdateRtsaAttribute((PRTSA)pObject, pAvaType);
            break;
        case MDC_MOC_VMO_METRIC_ENUM:
            err = UpdateEnumAttribute((PENUMERATION)pObject, pAvaType);
            break;
        case MDC_MOC_VMO_PMSTORE:
            err = UpdatePmStoreAttribute((PPMSTORE)pObject, pAvaType);
            break;
        case MDC_MOC_SCAN_CFG_EPI:
            err = UpdateEpiScannerAttribute((PEPICFGSCANNER)pObject, pAvaType);
            break;
        case MDC_MOC_SCAN_CFG_PERI:
            err = UpdatePeriScannerAttribute((PPERICFGSCANNER)pObject, pAvaType);
            break;           
        default:
            return ERROR_INVALID_PARAM;
            break;            
    }
    
    return err;
}
/**************************************************************************//*!
 *
 * @name  UpdateDIM
 *
 * @brief This function updates the DIM when the configuration is changed 
 *		  without releasing the association
 *
 * @param pObsScanList	:	Pointer to the Observation Scan List
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are not correct
 ******************************************************************************
 * This function updates the DIM with the new values of the attributes.
 *****************************************************************************/
ERR_CODE UpdateDIM(
    ObservationScanList* pObsScanList	/* [IN] Pointer to the Observation 
											Scan List */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 i, j;
    void* pObject;
    intu16 ObjType;
    ObservationScan* pObsScan = &pObsScanList->value[0];
    AVA_Type* pAvaType;
    for(i = 0; i < pObsScanList->count; i++)
    {
        pObject = GetObjectByHandle(pObsScan->obj_handle, &ObjType);
        pAvaType = &pObsScan->attributes.value[0];
        
        for(j = 0; j < pObsScan->attributes.count; j++)
        {
            err = UpdateAttribute(ObjType, pObject, pAvaType);
            if(ERROR_SUCCESS != err)
            {
                return ERROR_INVALID_PARAM;
            }
            (intu8*)pAvaType += pAvaType->attribute_value.length + 
                ATTR_HEADER_SIZE;
        }
        (intu8*)pObsScan += pObsScan->attributes.length + ATTR_HEADER_SIZE + 
            sizeof(HANDLE);     
    }
    return err;       
}
/**************************************************************************//*!
 *
 * @name  ScanAllActiveScanners
 *
 * @brief This function checks if the object in the Observation Scan is present 
 *        in any other active scanner except for the one defined by input 
 *        Scanner object
 *
 * @param pObsScan  :	Pointer to the Observation Scan 
 * @param handle    :	Handle of the Scanner Object
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are not correct
 ******************************************************************************
 * This function returns an error if the same object handle is present in any 
 * other active scanner
 *****************************************************************************/
ERR_CODE ScanAllActiveScanners(
    ObservationScan* pObsScan,  /* [IN] Pointer to the Observation Scan */ 
    HANDLE handle               /* [IN] Handle of the Scanner Object*/
)
{
    ERR_CODE err = ERROR_SUCCESS;
    PMDS pMds = GetMdsHandle(MDS_HANDLE);
    PCONFIGURATION pConf = NULL;
    if(pMds != NULL)
    {
        intu16 i, j;
        pConf = &(*(pMds->aConfiguration))[pMds->ConfigSelected];

        for(i = 0; i < pConf->EpiScanCount; i++)
        {
            PEPICFGSCANNER pEpiScanner = &(*pConf->pEpiScanner)[i];
            HandleAttrValMap* pHandleAttrValMap = pEpiScanner->pCfgScanner->pScanner->pMdcAttrScanHandleAttrValMap;
            
            if((handle != ieee_ntohs(pEpiScanner->pCfgScanner->pScanner->MdcAttrIdHandle)) && 
               (pEpiScanner->pCfgScanner->pScanner->MdcAttrOpStat == OS_ENABLED))
            {
                HandleAttrValMapEntry* pHandleAttrValMapEntry = &pHandleAttrValMap->value[0];
                for(j = 0; j < pHandleAttrValMap->count; j++)
                {
                    if(pObsScan->obj_handle == pHandleAttrValMapEntry->obj_handle)
                    {
                        return ERROR_INVALID_PARAM;
                    }
                    (intu8*)pHandleAttrValMapEntry += pHandleAttrValMapEntry->attr_val_map.length + sizeof(HANDLE) + ATTR_HEADER_SIZE;
                }
            }
        }

        for(i = 0; i < pConf->PeriScanCount; i++)
        {
          PPERICFGSCANNER pPeriScanner = &(*pConf->pPeriScanner)[i];
          HandleAttrValMap* pHandleAttrValMap = pPeriScanner->pCfgScanner->pScanner->pMdcAttrScanHandleAttrValMap;
            
          if((handle != ieee_ntohs(pPeriScanner->pCfgScanner->pScanner->MdcAttrIdHandle)) && 
             (pPeriScanner->pCfgScanner->pScanner->MdcAttrOpStat == OS_ENABLED))
          {
            HandleAttrValMapEntry* pHandleAttrValMapEntry = &pHandleAttrValMap->value[0];
            for(j = 0; j < pHandleAttrValMap->count; j++)
            {
              if(pObsScan->obj_handle == pHandleAttrValMapEntry->obj_handle)
              {
                return ERROR_INVALID_PARAM;
              }
              (intu8*)pHandleAttrValMapEntry += pHandleAttrValMapEntry->attr_val_map.length + sizeof(HANDLE) + ATTR_HEADER_SIZE;
            }
          }
        }
    }
    return err;        
}
#define MAX_TEMPORARY_STORED_MSR        25
/**************************************************************************//*!
 *
 * @name  SendMeasurements
 *
 * @brief This function is called by the upper layer to send multi person 
 *		  measurements
 *
 * @param pMsr			:	Pointer to the measurements
 * @param bPersonFlag	:	Multi person measurements or not
 * @param handle		:	Handle of the object which has to send 
 *							measurements
 * @param ReportType	:	Type of the report							
 * @param ScanCount		:	Number of scans
 * @param bConfirm		:	True for confirmed event report
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are not correct
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function validates the input parameters and calls the function to send 
 * measurements depending on the report type.
 *****************************************************************************/
static ERR_CODE SendMeasurements(
	void* pMsr,			/* [IN] Pointer to the measurements */ 
	intu8 bPersonFlag,	/* [IN] Multi person measurements or not */ 
	HANDLE handle,		/* [IN] Handle of the object which has to send 
 							measurements */ 
	intu8 ReportType,	/* [IN] Type of the report */ 
	intu16 ScanCount,	/* [IN] Number of scans */
	intu8 bConfirm      /* [IN] True for confirmed event report */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;
    ObservationScanList* (*pObsScanList)[];
    ObservationScanList* pObservationScanList;
    void* pObject;
    intu16 ObjType;
    ObservationScan* pObsScan;
    intu16 i, j, k;
    ScanReportPerVarList* (*pScanRptPerVarList)[];
    ScanReportPerVarList* pScanReportPerVarList;
    DataReqModeCapab* pDataReqModeCapab = GetPhdAssocFieldPtr(DATA_REQMODE);
    UNUSED(bPersonFlag);
    
    if(ScanCount > MAX_TEMPORARY_STORED_MSR)
    {
        return ERROR_INVALID_PARAM;
    }
    if(pDataReqModeCapab == NULL)
    {
        return ERROR_INVALID_PARAM;
    }

    if((!(pDataReqModeCapab->data_req_mode_flags & DATA_REQ_SUPP_INIT_AGENT)) 
        && (handle == MDS_HANDLE))
    {
        return ERROR_INVALID_PARAM;
    }
        
	pObject = GetObjectByHandle(handle, &ObjType);
    switch(ReportType)
    {
        case SCAN_REPORT_FIXED:
        {
            if(ObjType != MDC_MOC_VMS_MDS_SIMP)
            {
                return ERROR_INVALID_PARAM;
            }
            pObsScanList = pMsr;

            for(j = 0; j < ScanCount; j++)
            {
                pObservationScanList = (*pObsScanList)[j];
                pObsScan = &pObservationScanList->value[0];    
                
                for(i = 0; i < ieee_ntohs(pObservationScanList->count); i++)
                {
                    /* Check whether all attributes of Attr Val Map present 
    				in obs scan list*/
                    err = MatchAttrListWithAttrValMap(pObsScan);
                    /* if not return with an error */
                    if(ERROR_SUCCESS != err)
                    {
                        return ERROR_INVALID_PARAM;
                    }
                    (intu8*)pObsScan += ieee_ntohs(pObsScan->attributes.length) + 
                        ATTR_HEADER_SIZE + sizeof(HANDLE);
                }
            }
            /* Call AgentSendFixedFormat Event report */            
            err = AgentSendFixedFormatEventRpt(handle, pObsScanList, bConfirm, 
                ScanCount);
        }
        break;
        case SCAN_REPORT_MP_FIXED:
        {
            pScanRptPerVarList = pMsr;
            if(ObjType != MDC_MOC_VMS_MDS_SIMP)
            {
                return ERROR_INVALID_PARAM;
            }
            for(k = 0; k < ScanCount; k++)
            {
                pScanReportPerVarList = (*pScanRptPerVarList)[k];
                pObservationScanList = 
                    &pScanReportPerVarList->value[0].obs_scan_var;
                for(i = 0; i < ieee_ntohs(pScanReportPerVarList->count); i++)
                {
                    pObsScan = &pObservationScanList->value[0];
                    for(j = 0; j < ieee_ntohs(pObservationScanList->count); j++)
                    {
                        err = MatchAttrListWithAttrValMap(pObsScan); 
                        /* if not return with an error */
                        if(ERROR_SUCCESS != err)
                        {
                            return ERROR_INVALID_PARAM;
                        }                
                        (intu8*)pObsScan += ieee_ntohs(pObsScan->attributes.length) + 
                            ATTR_HEADER_SIZE + sizeof(HANDLE);
                    }
                    (intu8*)pObservationScanList += 
                        ieee_ntohs(pObservationScanList->length) + (sizeof(intu16) * 3);
                }
            }
            /* Call AgentSendMPFixedFormat Event report */            
            err = AgentSendMPFixedFormatEventRpt(handle, pScanRptPerVarList, 
                bConfirm, ScanCount);
        }
        break;
        case SCAN_REPORT_VAR:
        {
            if(ObjType != MDC_MOC_VMS_MDS_SIMP)
            {
                return ERROR_INVALID_PARAM;
            }
            pObsScanList = pMsr;
            for(j = 0; j < ScanCount; j++)
            {
                pObservationScanList = (*pObsScanList)[j];
                pObsScan = &pObservationScanList->value[0];    
                
                for(i = 0; i < ieee_ntohs(pObservationScanList->count); i++)
                {
                    /* Check whether any attributes of Attr Val Map present 
    				in obs scan list*/
                    err = ScanAttrListWithAttrValMap(pObsScan);
                    /* if yes return with an error */
                    if(ERROR_SUCCESS != err)
                    {
                        return ERROR_INVALID_PARAM;
                    }                
                    (intu8*)pObsScan += ieee_ntohs(pObsScan->attributes.length) + 
                        ATTR_HEADER_SIZE + sizeof(HANDLE);
                }
                err = UpdateDIM(pObservationScanList);
            }
            if(ERROR_SUCCESS != err)
            {
                return ERROR_INVALID_PARAM;
            }
            /* Call AgentSendFixedFormat Event report */            
            err = AgentSendVarFormatEventRpt(handle, pObsScanList, bConfirm, 
                ScanCount);
        }
        break;
        case SCAN_REPORT_MP_VAR:
        {
            pScanRptPerVarList = pMsr;
            
            if(ObjType != MDC_MOC_VMS_MDS_SIMP)
            {
                return ERROR_INVALID_PARAM;
            }
            for(k = 0; k < ScanCount; k++)
            {
                pScanReportPerVarList = (*pScanRptPerVarList)[k];
                pObservationScanList = 
                    &pScanReportPerVarList->value[0].obs_scan_var;
                for(i = 0; i < ieee_ntohs(pScanReportPerVarList->count); i++)
                {
                    pObsScan = &pObservationScanList->value[0];
                    for(j = 0; j < ieee_ntohs(pObservationScanList->count); j++)
                    {
                        err = ScanAttrListWithAttrValMap(pObsScan); 
                        /* if yes return with an error */
                        if(ERROR_SUCCESS != err)
                        {
                            return ERROR_INVALID_PARAM;
                        }                
                        (intu8*)pObsScan += ieee_ntohs(pObsScan->attributes.length) + 
                            ATTR_HEADER_SIZE + sizeof(HANDLE);
                    }
                    (intu8*)pObservationScanList += 
                        ieee_ntohs(pObservationScanList->length) + (sizeof(intu16) * 3);
                }
            }
            /* Call AgentSendMPFixedFormat Event report */            
            err = AgentSendMPVarFormatEventRpt(handle, pScanRptPerVarList, bConfirm, ScanCount);
        }
        break;
        case SCAN_REPORT_GROUPED:
        {            
            if((ObjType != MDC_MOC_SCAN_CFG_EPI) && (ObjType != MDC_MOC_SCAN_CFG_PERI))
            {
                return ERROR_INVALID_PARAM;
            }
  
            if(ObjType == MDC_MOC_SCAN_CFG_EPI)
            {
              PEPICFGSCANNER pEpiScanner = (PEPICFGSCANNER)pObject;
              PSCANNER pScanner = pEpiScanner->pCfgScanner->pScanner;
              if( (pScanner->MdcAttrOpStat == OS_DISABLED) || 
					        ((pScanner->MdcAttrOpStat == OS_ENABLED) && (pEpiScanner->TimerIndex != (intu8)INVALID_TIME_COUNT))
					      )
              {
                return ERROR_INVALID_REQUEST;
              }
            }

            if(ObjType == MDC_MOC_SCAN_CFG_PERI)
            {                                                                                                      
              PPERICFGSCANNER pPeriScanner = (PPERICFGSCANNER)pObject;
              PSCANNER pScanner = pPeriScanner->pCfgScanner->pScanner;
                
              if((pScanner->MdcAttrOpStat == OS_DISABLED) || 
					       ((pScanner->MdcAttrOpStat == OS_ENABLED) && (pPeriScanner->TimerIndex != (intu8)INVALID_TIME_COUNT)))
              {
                return ERROR_INVALID_REQUEST;
              }
            }

            pObsScanList = pMsr;
            for(j = 0; j < ScanCount; j++)
            {                
              pObservationScanList = (*pObsScanList)[j];       
              pObsScan = &pObservationScanList->value[0];
              for(i = 0; i < ieee_ntohs(pObservationScanList->count); i++)
              {
                 err = ScanAllActiveScanners(pObsScan, handle);
                 if(ERROR_SUCCESS != err)
                 {
                    return err;
                 }
                 /* Check whether all attributes of Attr Val Map present in obs scan list*/
                 err = MatchAttrListWithHandleAttrValMap(pObsScan, handle);
                 /* if not return with an error */
                 if(ERROR_SUCCESS != err)
                 {
                    return ERROR_INVALID_PARAM;
                 }
                (intu8*)pObsScan += ieee_ntohs(pObsScan->attributes.length) + ATTR_HEADER_SIZE + sizeof(HANDLE);
              }
            }
            /* Call AgentSendFixedFormat Event report */            
            err = AgentSendGroupedFormatEventRpt(handle, pObsScanList, ScanCount);

		      /* Restart Episoidic Scanner Minimum Reporting Interval Timer */
		    	if((ERROR_SUCCESS == err) && (ObjType == MDC_MOC_SCAN_CFG_EPI) &&
			       (((PEPICFGSCANNER)pObject)->OptionalAttributeFlag & OPT_EPISCAN_MIN_RPT_INT))
      		{
			      PEPICFGSCANNER pEpiScanner = (PEPICFGSCANNER)pObject;
			      TIMER_OBJECT TimerObject;
			      TimerObject.arg = pEpiScanner;
			      TimerObject.pfnTimerCallback = EpiCfgScannerTimerCallback;
			      TimerObject.msCount = (pEpiScanner->MdcAttrScanRepPdMin >> 3);
	          pEpiScanner->TimerIndex = PHD_Add_Timer(&TimerObject);
			    }
        }
        break;        

        case SCAN_REPORT_MP_GROUPED:
        {
            pScanRptPerVarList = pMsr;
            if((ObjType != MDC_MOC_SCAN_CFG_EPI) && ObjType != MDC_MOC_SCAN_CFG_PERI)
            {
              return ERROR_INVALID_PARAM;
            }
            if(ObjType == MDC_MOC_SCAN_CFG_EPI)
            {
              PEPICFGSCANNER pEpiScanner = (PEPICFGSCANNER)pObject;
              PSCANNER pScanner = pEpiScanner->pCfgScanner->pScanner;
              if((pScanner->MdcAttrOpStat == OS_DISABLED) || 
					       ((pScanner->MdcAttrOpStat == OS_ENABLED) && (pEpiScanner->TimerIndex != (intu8)INVALID_TIME_COUNT)))
              {
                    return ERROR_INVALID_REQUEST;
              }
            }

            if(ObjType == MDC_MOC_SCAN_CFG_PERI)
            {                                                                                                      
              PPERICFGSCANNER pPeriScanner = (PPERICFGSCANNER)pObject;
              PSCANNER pScanner = pPeriScanner->pCfgScanner->pScanner;
              if((pScanner->MdcAttrOpStat == OS_DISABLED) || 
					       ((pScanner->MdcAttrOpStat == OS_ENABLED) && (pPeriScanner->TimerIndex != (intu8)INVALID_TIME_COUNT)))
              {
                  return ERROR_INVALID_REQUEST;
              }
            }
            
            for(k = 0; k < ScanCount; k++)
            {
              pScanReportPerVarList = (*pScanRptPerVarList)[k];
              pObservationScanList = &pScanReportPerVarList->value[0].obs_scan_var;
              for(j = 0; j < ieee_ntohs(pScanReportPerVarList->count); j++)
              {                
                pObsScan = &pObservationScanList->value[0];       
                for(i = 0; i < ieee_ntohs(pObservationScanList->count); i++)
                {
                  err = ScanAllActiveScanners(pObsScan, handle);
                  if(ERROR_SUCCESS != err)
                  {
                     return err;
                  }
                  /* Check whether all attributes of Attr Val Map present in obs scan list*/
                  err = MatchAttrListWithHandleAttrValMap(pObsScan, handle);
                  /* if not return with an error */
                  if(ERROR_SUCCESS != err)
                  {
                    return ERROR_INVALID_PARAM;
                  }
                      (intu8*)pObsScan += ieee_ntohs(pObsScan->attributes.length) + ATTR_HEADER_SIZE + sizeof(HANDLE);
                }
                (intu8*)pObservationScanList += ieee_ntohs(pObservationScanList->length) + ATTR_HEADER_SIZE + sizeof(PersonId);
              }
            }
            /* Call AgentSendFixedFormat Event report */            
            err = AgentSendMPGroupedFormatEventRpt(handle, pScanRptPerVarList, ScanCount);

		        /* Restart Episoidic Scanner Minimum Reporting Interval Timer */
			      if((ERROR_SUCCESS == err) && (ObjType == MDC_MOC_SCAN_CFG_EPI) &&
			         (((PEPICFGSCANNER)pObject)->OptionalAttributeFlag & OPT_EPISCAN_MIN_RPT_INT))
			      {
			        PEPICFGSCANNER pEpiScanner = (PEPICFGSCANNER)pObject;
			        TIMER_OBJECT TimerObject;
			        TimerObject.arg = pEpiScanner;
			        TimerObject.pfnTimerCallback = EpiCfgScannerTimerCallback;
			        TimerObject.msCount = (pEpiScanner->MdcAttrScanRepPdMin >> 3);
	            pEpiScanner->TimerIndex = PHD_Add_Timer(&TimerObject);
			      }

        }
        break;
    }
    return err;
}
/**************************************************************************//*!
 *
 * @name  AgentSendPersonMeasurements
 *
 * @brief This function is called by the upper layer to send multi person 
 *		  measurements
 *
 * @param pScanRptPerVarList	:	Pointer to an array of Scan Report List 
 *                                  per person
 * @param Handle				:	Handle of the object which has to send 
 *									measurements
 * @param ReportType			:	Type of the report 
 *									(MPfixed/MPvariable/MPgrouped)
 * @param ScanCount				:	Number of scans
 * @param bConfirm		        :	True for confirmed event report
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are not correct
 *			ERROR_INVALID_DATA		: if data in Scan Report List is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function sends and validate multi person measurements to be sent.
 *****************************************************************************/
ERR_CODE AgentSendPersonMeasurements(
	ScanReportPerVarList* (*pScanRptPerVarList)[],	
	                                            /* [IN] Pointer to an array of 
	                                                Scan Report List per 
	                                                person */ 
	HANDLE Handle,								/* [IN] Handle of the object 
													which has to send 
													measurements*/ 
	intu8 ReportType,							/* [IN] Type of the report  */ 
	intu16 ScanCount,							/* [IN] Number of scans */ 
	intu8 bConfirm                              /* [IN] True for confirmed 
	                                                event report */
)
{
    return SendMeasurements((void*)pScanRptPerVarList, TRUE, Handle, ReportType, ScanCount, bConfirm);
}

/**************************************************************************//*!
 *
 * @name  AgentSendMeasurements
 *
 * @brief This function is called by the upper layer to send measurements when 
 *		  multi person reports are not supported
 *
 * @param pObsScanList	:	Pointer to an array of observation scan lists
 * @param Handle		:	Handle of the object which has to send measurements
 * @param ReportType	:	Type of the report (fixed/variable/grouped)
 * @param ScanCount		:	Number of scans, valid only for scanner
 * @param bConfirm		:	True for confirmed event report
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are not correct
 *			ERROR_INVALID_DATA		: if data in observation scan list is 
 *										incorrect
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push data in Buffer 
 *                                      Stack
 ******************************************************************************
 * This function sends and validate measurements to be sent.
 *****************************************************************************/
ERR_CODE AgentSendMeasurements(
	ObservationScanList* (*pObsScanList)[],	/* [IN] Pointer to an array of 
											    observation scan lists */ 
	HANDLE Handle,						    /* [IN] Handle of the object 
										    	which has to send 
											    measurements*/ 
	intu8 ReportType,					    /* [IN] Type of the report */ 
	intu16 ScanCount,					    /* [IN] Number of scans */ 
	intu8 bConfirm                          /* [IN] True for confirmed event 
	                                            report */
)
{
    return SendMeasurements((void*)pObsScanList, FALSE, Handle, ReportType, 
		ScanCount, bConfirm);
}
    
/**************************************************************************//*!
 *
 * @name  PushEnumVal
 *
 * @brief  
 *		  
 *
 * @param pBuffStack	:
 * @param pEnumVal		:
 * @param pAttrLen   	:
 *
 * @return ERR_CODE
 *
 ******************************************************************************
 * 
 *****************************************************************************/
static ERR_CODE PushEnumVal(
    PTR_BUFFSTACK pBuffStack,
    EnumVal *pEnumVal,
    intu16 *pAttrLen
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
        switch(pEnumVal->choice)
        {
            case OBJ_ID_CHOSEN:
            {
        		  err = PushOID_Type(pBuffStack, pEnumVal->u.enum_obj_id, &AttrLen);
              break;
            }
            case TEXT_STRING_CHOSEN:
            {
           		err = Pushoctet_string(pBuffStack, &pEnumVal->u.enum_text_string, &AttrLen);
              break;
            }
            case BIT_STR_CHOSEN:
            {
        		  err = Pushintu32(pBuffStack, pEnumVal->u.enum_bit_str, &AttrLen);
              break;
            }
        }
        if(ERROR_SUCCESS != err)
        {
            break;
        }
		*pAttrLen = AttrLen;
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, pEnumVal->choice, pEnumVal->length, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
	}while(0);
	
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushEnumObsValue
 *
 * @brief  
 *		  
 *
 * @param pBuffStack	:
 * @param pEnumObsValue		:
 * @param pAttrLen   	:
 *
 * @return ERR_CODE
 *
 ******************************************************************************
 * 
 *****************************************************************************/
static ERR_CODE PushEnumObsValue(
    PTR_BUFFSTACK pBuffStack,
    EnumObsValue *pEnumObsValue,
    intu16 *pAttrLen
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
	  err = PushEnumVal(pBuffStack, &pEnumObsValue->value, &AttrLen);
		if(err != ERROR_SUCCESS)
		{
			break;
		}
		*pAttrLen = AttrLen;
		
		err = PushAttribute(pBuffStack, pEnumObsValue->metric_id, pEnumObsValue->state, &AttrLen);
		if(err != ERROR_SUCCESS)
		{
			break;
		}
		*pAttrLen += AttrLen;
		
	}while(0);
	
	if(err != ERROR_SUCCESS)
	{
		(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
		*pAttrLen = 0;
	}
	
	return err;
}
/**************************************************************************//*!
 *
 * @name  AddEntryToObsScanList
 *
 * @brief This function is called by the upper layer to create Observation Scan
 *		  List
 *
 * @param handle	    :	Handle of the object 
 * @param AttrId	    :	Attribute ID
 * @param AttrLen		:	Attribute length
 * @param pAttrVal  	:	Pointer to the Attribute value
 * @param pObsScanList  :	Pointer to the Observation Scan List
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are not correct
 ******************************************************************************
 * This function creates Observation Scan List based on the input parameters
 *****************************************************************************/
ERR_CODE AddEntryToObsScanList(
    HANDLE handle,      /* [IN] Handle of the object */ 
    OID_Type AttrId,    /* [IN] Attribute ID */ 
    intu16 AttrLen,     /* [IN] Attribute length */ 
    void* pAttrVal,     /* [IN] Pointer to the Attribute value */ 
    ObservationScanList* pObsScanList
                        /* [OUT] Pointer to the Observation Scan List */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 i;
    ObservationScan* pObsScan = &pObsScanList->value[0];
    for(i = 0; i < ieee_ntohs(pObsScanList->count); i++)
    {
        if(ieee_ntohs(pObsScan->obj_handle) == handle)
        {
            break;
        }
        (intu8*)pObsScan += ieee_ntohs(pObsScan->attributes.length) + sizeof(HANDLE) + 
            ATTR_HEADER_SIZE;
    }
    if(i == ieee_ntohs(pObsScanList->count))
    {
        /* 
            Add new Observation Scan
        */
        err = PushAttrObs(handle, AttrId, AttrLen, pAttrVal, &pObsScan->attributes.value[0]);
        if(err != ERROR_SUCCESS)
        {
            return ERROR_INVALID_PARAM;
        }
        pObsScanList->count = ieee_htons(ieee_ntohs(pObsScanList->count) + 1);
        pObsScanList->length = ieee_htons(ieee_ntohs(pObsScanList->length) + AttrLen + 
            ATTR_HEADER_SIZE + sizeof(HANDLE) + sizeof(OID_Type) + ANY_HEADER_SIZE);        
        pObsScan->obj_handle = ieee_htons(handle);
        pObsScan->attributes.count = ieee_htons(1);
        pObsScan->attributes.length = ieee_htons((sizeof(OID_Type) + ANY_HEADER_SIZE + AttrLen));        
    }
    else if(i == (ieee_ntohs(pObsScanList->count) - 1) )
    {
      /* 
       *   Check for Attribute ID in this Observation Scan 
       *   If matches with existing then return DUPLICATE Entry       
       *   otherwise ADD Entry in Observation Scan List
       */
      AVA_Type* pAvaType = &pObsScan->attributes.value[0];
      for(i = 0; i < ieee_htons(pObsScan->attributes.count); i++)
      {
        if(ieee_ntohs(pAvaType->attribute_id) == AttrId)
        {
            return ERROR_INVALID_PARAM;
        }
        (intu8*)pAvaType += ieee_htons(pAvaType->attribute_value.length) + ANY_HEADER_SIZE + sizeof(OID_Type);
      }
      
      err = PushAttrObs(handle, AttrId, AttrLen, pAttrVal, pAvaType);
      if(err != ERROR_SUCCESS)
      {        
        return ERROR_INVALID_PARAM;
      }
      pObsScan->attributes.count = ieee_htons(ieee_ntohs(pObsScan->attributes.count) + 1);
      pObsScan->attributes.length = ieee_htons(ieee_ntohs(pObsScan->attributes.length) + AttrLen + ATTR_HEADER_SIZE);        
      pObsScanList->length = ieee_htons(ieee_ntohs(pObsScanList->length) + AttrLen + ATTR_HEADER_SIZE);
    }
    else
    {
        /* 
            Already New Handle is being Updated 
            return ERR.
        */
        return ERROR_INVALID_PARAM;
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  AddEntryToScanRptPerVarList
 *
 * @brief This function is called by the upper layer to create Scan Report Per 
 *		  Var List
 *
 * @param handle	    	:	Handle of the object 
 * @param PersonID      	:	Person Id 
 * @param AttrId	    	:	Attribute ID
 * @param AttrLen		    :	Attribute length
 * @param pAttrVal  	    :	Pointer to the Attribute value
 * @param pScanRptPerVarList:	Pointer to the Scan Per Var List
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are not correct
 ******************************************************************************
 * This function creates Scan report Per Var List based on the input parameters
 *****************************************************************************/
ERR_CODE AddEntryToScanRptPerVarList(
    HANDLE handle,      /* [IN] Handle of the object */ 
    PersonId PersonID,  /* [IN] Person Id */ 
    OID_Type AttrId,    /* [IN] Attribute ID */ 
    intu16 AttrLen,     /* [IN] Attribute length */ 
    void* pAttrVal,     /* [IN] Pointer to the Attribute value */ 
    ScanReportPerVarList* pScanRptPerVarList    
                        /* [OUT] Pointer to the Scan Per Var List */ 
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 i;
    ScanReportPerVar* pScanRptPerVar = &pScanRptPerVarList->value[0];
    for(i = 0; i < ieee_ntohs(pScanRptPerVarList->count); i++)
    {
        if(ieee_ntohs(pScanRptPerVar->person_id) == PersonID)
        {
            break;
        }
        (intu8*)pScanRptPerVar += (ieee_ntohs(pScanRptPerVar->obs_scan_var.length) + 
            sizeof(PersonId) + ATTR_HEADER_SIZE);
    }
    if(i == ieee_ntohs(pScanRptPerVarList->count))
    {
        ObservationScanList* pObsScanList = &pScanRptPerVar->obs_scan_var;
        err = AddEntryToObsScanList(handle, AttrId, AttrLen, pAttrVal, 
            pObsScanList);
        if(err != ERROR_SUCCESS)
        {
            return err;
        }        
        /* Add new person ID */
        pScanRptPerVarList->count = 
            ieee_htons(ieee_ntohs(pScanRptPerVarList->count) + 1);
        pScanRptPerVarList->length = ieee_htons(ieee_ntohs(pScanRptPerVarList->length) + 
            sizeof(PersonId) + ATTR_HEADER_SIZE + pObsScanList->length);
        pScanRptPerVar->person_id = ieee_htons(PersonID);            
    }
    else if(i == (ieee_ntohs(pScanRptPerVarList->count) - 1))
    {
        /* Add entry to observation scan list */
        ObservationScanList* pObsScanList = &pScanRptPerVar->obs_scan_var;
        err = AddEntryToObsScanList(handle, AttrId, AttrLen, pAttrVal, 
            pObsScanList);
        if(err != ERROR_SUCCESS)
        {
            return err;
        }        
        pScanRptPerVarList->length = ieee_htons(ieee_ntohs(pScanRptPerVarList->length) + 
             AttrLen + ATTR_HEADER_SIZE);        
    }
    else
    {
        return ERROR_INVALID_PARAM;
    }
    return err;
}

    
/**************************************************************************//*!
 *
 * @name  ValidateAttrSize
 *
 * @brief This function Validates Attribute size
 *
 * @param AttrId	:	Attribute ID
 * @param AttrLen	:	Attribute Length
 * @param pAttrVal	:	Pointer to Attribute Data
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates Attribute size
 *****************************************************************************/
static ERR_CODE ValidateAttrSize(
    OID_Type AttrId,            /* [IN] AttrId */	
    intu16 AttrLen,             /* [IN] Attribute Length */		
    void* pAttrVal              /* [IN] Pointer to Attribute Data */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    switch(AttrId)
    {
        case MDC_ATTR_TIME_BATT_REMAIN:
            /* 
                Type BatMeasure 
            */
            if(AttrLen != sizeof(BatMeasure))
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        case MDC_ATTR_CONFIRM_MODE:
        case MDC_ATTR_NUM_SEG:
        case MDC_ATTR_OP_STAT:
        case MDC_ATTR_POWER_STAT:
        case MDC_ATTR_VAL_BATT_CHARGE:
        case MDC_ATTR_ENUM_OBS_VAL_BASIC_BIT_STR:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_OID:
        case MDC_ATTR_MSMT_STAT:
        case MDC_ATTR_METRIC_STRUCT_SMALL:
        case MDC_ATTR_METRIC_SPEC_SMALL:
        case MDC_ATTR_ID_PHYSIO:
        case MDC_ATTR_METRIC_ID_PART:
        case MDC_ATTR_UNIT_CODE:
        case MDC_ATTR_SOURCE_HANDLE_REF:
        case MDC_ATTR_NU_VAL_OBS_BASIC:
        case MDC_ATTR_ENUM_OBS_VAL_PART:

            /* 
                Type intu16 
            */
            if(AttrLen != sizeof(intu16))
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        case MDC_ATTR_SCAN_REP_PD_MIN:
        case MDC_ATTR_SCAN_REP_PD:
        case MDC_ATTR_CONFIRM_TIMEOUT:
        case MDC_ATTR_CLEAR_TIMEOUT:
        case MDC_ATTR_METRIC_STORE_USAGE_CNT:
        case MDC_ATTR_TIME_REL:
        case MDC_ATTR_TIME_STAMP_REL:
        case MDC_ATTR_TIME_PD_MSMT_ACTIVE:
        case MDC_ATTR_NU_VAL_OBS_SIMP:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_BIT_STR:
            /* 
                Type intu32 
            */
            if(AttrLen != sizeof(intu32))
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        case MDC_ATTR_NU_CMPD_VAL_OBS_SIMP:
        {
            /* 
                Type 
                {
                    intu16,
                    intu16,
                    {intu32}.......
                }
            */
            SimpleNuObsValueCmp* pVal = (SimpleNuObsValueCmp*)pAttrVal;             
            intu16 size = (intu16)((sizeof(SimpleNuObsValue) * pVal->count) + 
                ATTR_HEADER_SIZE);   
            if((AttrLen != size) || 
                (pVal->length != (size - ATTR_HEADER_SIZE)))
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        }
        case MDC_ATTR_ID_PHYSIO_LIST:
        case MDC_ATTR_NU_CMPD_VAL_OBS_BASIC:
        {
            /* 
                Type 
                {
                    intu16,
                    intu16,
                    {intu16}.......
                }
            */
            BasicNuObsValueCmp* pVal = (BasicNuObsValueCmp*)pAttrVal;             
            intu16 size = (intu16)((sizeof(BasicNuObsValue) * ieee_htons(pVal->count)) + 
                ATTR_HEADER_SIZE);   
            if((AttrLen != size) || 
                (ieee_htons(pVal->length) != (size - ATTR_HEADER_SIZE)))
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        }
        case MDC_ATTR_NU_VAL_OBS:
            if(AttrLen != sizeof(NuObsValue))
            {
                err = ERROR_INVALID_PARAM;
            }
           break;
        case MDC_ATTR_NU_CMPD_VAL_OBS:
        {
            NuObsValueCmp* pVal = (NuObsValueCmp*)pAttrVal;             
            intu16 size = (intu16)((sizeof(NuObsValue) * pVal->count) + 
                ATTR_HEADER_SIZE);   
            if((AttrLen != size) || 
                (pVal->length != (size - ATTR_HEADER_SIZE)))
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        }
        case MDC_ATTR_ATTRIBUTE_VAL_MAP:
        {
            /* 
                Type 
                {
                    intu16,
                    intu16,
                       {intu16,
                       intu16}.......
                }
            */
            AttrValMap *pVal = (AttrValMap*)pAttrVal;
            intu16 size = (intu16)((sizeof(AttrValMapEntry) * pVal->count) +
                ATTR_HEADER_SIZE);
            if((AttrLen != size) || 
                (pVal->length != (size - ATTR_HEADER_SIZE)))
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        }
        case MDC_ATTR_UNIT_LABEL_STRING:
        case MDC_ATTR_ID_LABEL_STRING:
        case MDC_ATTR_SIMP_SA_OBS_VAL:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_STR:
        {
            /* 
                Type 
                {
                    intu16,
                    {intu8}.....
                }
            */
            octet_string *pVal = (octet_string*)pAttrVal;
            intu16 size = (intu16)(pVal->length +
                OCTET_STRING_HEADER_SIZE);
            if(AttrLen != size)
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        }
        case MDC_ATTR_TIME_ABS_ADJUST:
            /* 
                Type 
                {
                    intu8, intu8,
                    intu8, intu8,
                    intu8, intu8
                }
            */
            if(AttrLen != sizeof(AbsoluteTimeAdjust))
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        case MDC_ATTR_TIME_REL_HI_RES:
        case MDC_ATTR_TIME_ABS:
        case MDC_ATTR_TIME_STAMP_ABS:
        case MDC_ATTR_TIME_STAMP_REL_HI_RES:
            /* 
                Type 
                {
                    intu8, intu8,
                    intu8, intu8,
                    intu8, intu8,
                    intu8, intu8
                }
            */
            if(AttrLen != sizeof(AbsoluteTime))
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        case MDC_ATTR_VAL_ENUM_OBS:
        {
            /*
                Type
                EnumObsValue
            */
            EnumObsValue* pVal = (EnumObsValue*)pAttrVal;
            intu16 size = sizeof(OID_Type) + sizeof(MeasurementStatus) +
                ATTR_HEADER_SIZE;
            switch(pVal->value.choice)
            {
                case OBJ_ID_CHOSEN:
                    size += sizeof(OID_Type);
                    break;
                case TEXT_STRING_CHOSEN:
                {
                    octet_string *pTemp = &pVal->value.u.enum_text_string;
                    size += OCTET_STRING_HEADER_SIZE + pTemp->length;
                    break;
                }
                case BIT_STR_CHOSEN:
                    size += sizeof(intu32);
                    break;
                default:
                    return ERROR_INVALID_PARAM;
                    break;
            }
            if(AttrLen != size)
            {
                err = ERROR_INVALID_PARAM;
            }

            break;
        }
        case MDC_ATTR_MDS_TIME_INFO:
            /* 
                Type 
                {
                    MdsTimeInfo
                }
            */
            if(AttrLen != sizeof(MdsTimeInfo))
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        case MDC_ATTR_SCAN_HANDLE_ATTR_VAL_MAP:
        {
            
            /* 
                Type 
                {
                    HandleAttrValMap
                }
            */
            intu16 i, size, size1 = 0;
            HandleAttrValMap* pHandleAttrValMap = (HandleAttrValMap*)pAttrVal;
            HandleAttrValMapEntry *pEntry = &pHandleAttrValMap->value[0];
            /*
                1. Verify All AttrVal Map 
            */
            for(i = 0; i < pHandleAttrValMap->count; i++)
            {
                intu16 size2;
                size2 = (intu16)(pEntry->attr_val_map.length + ATTR_HEADER_SIZE);
                err = ValidateAttrSize(MDC_ATTR_ATTRIBUTE_VAL_MAP, 
                    size2, &pEntry->attr_val_map);
                if(ERROR_SUCCESS != err)
                {
                    break;
                }
                size2 += sizeof(HANDLE);
                size1 += size2;
                (intu8*)pEntry += size2;
            }
            if(ERROR_SUCCESS != err)
            {
                break;
            }
            size = (intu16)(ATTR_HEADER_SIZE + pHandleAttrValMap->length);
            if((AttrLen != size) || 
                (size1 != pHandleAttrValMap->length))
            {
                err = ERROR_INVALID_PARAM;
            }
            break;
        }
        default:
            err = ERROR_INVALID_PARAM;
            break;
    }
    return err;
}
/**************************************************************************//*!
 *
 * @name  ValidateMdsAttribute
 *
 * @brief This function Validates MDS Attributes
 *
 * @param AttrId	:	Attribute ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates MDS Attributes
 *****************************************************************************/
static ERR_CODE ValidateMdsAttribute(
    OID_Type AttrId                     /* [IN] Attribute ID */	
)
{
    switch(AttrId)
    {
        case MDC_ATTR_ATTRIBUTE_VAL_MAP:
        case MDC_ATTR_MDS_TIME_INFO:
        case MDC_ATTR_TIME_ABS:
        case MDC_ATTR_TIME_REL:
        case MDC_ATTR_TIME_REL_HI_RES:
        case MDC_ATTR_TIME_ABS_ADJUST:
        case MDC_ATTR_POWER_STAT:
        case MDC_ATTR_VAL_BATT_CHARGE:
        case MDC_ATTR_TIME_BATT_REMAIN:
        case MDC_ATTR_CONFIRM_TIMEOUT:
            return ERROR_SUCCESS;
            break;
        default: 
            break;
    }
    return ERROR_INVALID_PARAM;
}

/**************************************************************************//*!
 *
 * @name  ValidateMetricAttribute
 *
 * @brief This function Validates Metric Attributes
 *
 * @param AttrId	:	Attribute ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates Metric Attributes
 *****************************************************************************/
static ERR_CODE ValidateMetricAttribute(
    OID_Type AttrId            /* [IN] Attribute ID */	
)
{
    switch(AttrId)
    {
        case MDC_ATTR_METRIC_SPEC_SMALL:
        case MDC_ATTR_METRIC_STRUCT_SMALL:
        case MDC_ATTR_MSMT_STAT:
        case MDC_ATTR_ID_PHYSIO:
        case MDC_ATTR_ID_PHYSIO_LIST:
        case MDC_ATTR_METRIC_ID_PART:
        case MDC_ATTR_UNIT_CODE:
        case MDC_ATTR_ATTRIBUTE_VAL_MAP:
        case MDC_ATTR_SOURCE_HANDLE_REF:
        case MDC_ATTR_ID_LABEL_STRING:
        case MDC_ATTR_UNIT_LABEL_STRING:
        case MDC_ATTR_TIME_STAMP_ABS:
        case MDC_ATTR_TIME_STAMP_REL:
        case MDC_ATTR_TIME_STAMP_REL_HI_RES:
        case MDC_ATTR_TIME_PD_MSMT_ACTIVE:
            return ERROR_SUCCESS;
            break;
        default: 
            break;
    }
    return ERROR_INVALID_PARAM;
}

/**************************************************************************//*!
 *
 * @name  ValidateNumericAttribute
 *
 * @brief This function Validates Numeric Attributes
 *
 * @param AttrId	:	Attribute ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates Numeric Attributes
 *****************************************************************************/
static ERR_CODE ValidateNumericAttribute(
    OID_Type AttrId            /* [IN] Attribute ID */	
)
{
    switch(AttrId)
    {
        case MDC_ATTR_NU_VAL_OBS_SIMP:
        case MDC_ATTR_NU_CMPD_VAL_OBS_SIMP:
        case MDC_ATTR_NU_VAL_OBS_BASIC:
        case MDC_ATTR_NU_CMPD_VAL_OBS_BASIC:
        case MDC_ATTR_NU_VAL_OBS:
        case MDC_ATTR_NU_CMPD_VAL_OBS:
            return ERROR_SUCCESS;
            break;
        default:
            return ValidateMetricAttribute(AttrId); 
            break;
    }
    return ERROR_INVALID_PARAM;
}

/**************************************************************************//*!
 *
 * @name  ValidateRtsaAttribute
 *
 * @brief This function Validates RTSA Attributes
 *
 * @param AttrId	:	Attribute ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates RTSA Attributes
 *****************************************************************************/
static ERR_CODE ValidateRtsaAttribute(
    OID_Type AttrId            /* [IN] Attribute ID */	
)
{
    switch(AttrId)
    {
        case MDC_ATTR_SIMP_SA_OBS_VAL:
            return ERROR_SUCCESS;
            break;
        default:
            return ValidateMetricAttribute(AttrId); 
            break;
    }
    return ERROR_INVALID_PARAM;
}

/**************************************************************************//*!
 *
 * @name  ValidateEnumAttribute
 *
 * @brief This function Validates Enumeration Attributes
 *
 * @param AttrId	:	Attribute ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates Enumeration Attributes
 *****************************************************************************/
static ERR_CODE ValidateEnumAttribute(
    OID_Type AttrId            /* [IN] Attribute ID */	
)
{
    switch(AttrId)
    {
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_OID:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_BIT_STR:
        case MDC_ATTR_ENUM_OBS_VAL_BASIC_BIT_STR:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_STR:
        case MDC_ATTR_VAL_ENUM_OBS:
        case MDC_ATTR_ENUM_OBS_VAL_PART:
            return ERROR_SUCCESS;
            break;
        default:
            return ValidateMetricAttribute(AttrId); 
            break;
    }
    return ERROR_INVALID_PARAM;
}

/**************************************************************************//*!
 *
 * @name  ValidatePmStoreAttribute
 *
 * @brief This function Validates PM-Store Attributes
 *
 * @param AttrId	:	Attribute ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates PM-Store Attributes
 *****************************************************************************/
static ERR_CODE ValidatePmStoreAttribute(
    OID_Type AttrId            /* [IN] Attribute ID */	
)
{
    switch(AttrId)
    {
        case MDC_ATTR_METRIC_STORE_USAGE_CNT:
        case MDC_ATTR_OP_STAT:
        case MDC_ATTR_NUM_SEG:
        case MDC_ATTR_CLEAR_TIMEOUT:
            return ERROR_SUCCESS;
            break;
        default:
            break;
    }
    return ERROR_INVALID_PARAM;
}

/**************************************************************************//*!
 *
 * @name  ValidateScannerAttribute
 *
 * @brief This function Validates Scanner Attributes
 *
 * @param AttrId	:	Attribute ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates Scanner Attributes
 *****************************************************************************/
static ERR_CODE ValidateScannerAttribute(
    OID_Type AttrId            /* [IN] Attribute ID */	
)
{
    switch(AttrId)
    {
        case MDC_ATTR_SCAN_HANDLE_ATTR_VAL_MAP:
            return ERROR_SUCCESS;
            break;
        default:
            break;
    }
    return ERROR_INVALID_PARAM;
}

/**************************************************************************//*!
 *
 * @name  ValidateCfgScannerAttribute
 *
 * @brief This function Validates Configuration Scanner Attributes
 *
 * @param AttrId	:	Attribute ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates Configuration Scanner Attributes
 *****************************************************************************/
static ERR_CODE ValidateCfgScannerAttribute(
    OID_Type AttrId            /* [IN] Attribute ID */	
)
{
    switch(AttrId)
    {
        case MDC_ATTR_CONFIRM_MODE:
        case MDC_ATTR_CONFIRM_TIMEOUT:
            return ERROR_SUCCESS;
            break;
        default:
            return ValidateScannerAttribute(AttrId);
            break;
    }
    return ERROR_INVALID_PARAM;
}

/**************************************************************************//*!
 *
 * @name  ValidateEpiScanAttribute
 *
 * @brief This function Validates Episiodic Scanner Attributes
 *
 * @param AttrId	:	Attribute ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates Episiodic Scanner Attributes
 *****************************************************************************/
static ERR_CODE ValidateEpiScanAttribute(
    OID_Type AttrId            /* [IN] Attribute ID */	
)
{
    switch(AttrId)
    {
        case MDC_ATTR_SCAN_REP_PD_MIN:
            return ERROR_SUCCESS;
            break;
        default:
            return ValidateCfgScannerAttribute(AttrId);
            break;
    }
    return ERROR_INVALID_PARAM;
}

/**************************************************************************//*!
 *
 * @name  ValidatePeriScanAttribute
 *
 * @brief This function Validates Periodic Scanner Attributes
 *
 * @param AttrId	:	Attribute ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates Periodic Scanner Attributes
 *****************************************************************************/
static ERR_CODE ValidatePeriScanAttribute(
    OID_Type AttrId            /* [IN] Attribute ID */	
)
{
    switch(AttrId)
    {
        case MDC_ATTR_SCAN_REP_PD:
            return ERROR_SUCCESS;
            break;
        default:
            return ValidateCfgScannerAttribute(AttrId);
            break;
    }
    return ERROR_INVALID_PARAM;
}

/**************************************************************************//*!
 *
 * @name  ValidateAttr
 *
 * @brief This function Validates Attributes valid for Object Handle
 *
 * @param Handle	:	Object Handle 
 * @param AttrId	:	Attribute ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function Validates Attributes valid for Object Handle
 *****************************************************************************/
static ERR_CODE ValidateAttr(
    HANDLE Handle,			    /* [IN] Object Handle */
    OID_Type AttrId            /* [IN] Attribute ID */	
)
{
    intu16 ObjectType;
    ERR_CODE err = ERROR_SUCCESS;
    void *pObject;
    pObject = GetObjectByHandle(Handle, &ObjectType);
    switch(ObjectType)
    {
        case MDC_MOC_VMS_MDS_SIMP:
            err = ValidateMdsAttribute(AttrId);
            break;
        case MDC_MOC_VMO_METRIC_NU:
            err = ValidateNumericAttribute(AttrId);
            break;
        case MDC_MOC_VMO_METRIC_SA_RT:
            err = ValidateRtsaAttribute(AttrId);
            break;
        case MDC_MOC_VMO_METRIC_ENUM:
            err = ValidateEnumAttribute(AttrId);
            break;
        case MDC_MOC_VMO_PMSTORE:
            err = ValidatePmStoreAttribute(AttrId);
            break;
        case MDC_MOC_SCAN_CFG_EPI:
            err = ValidateEpiScanAttribute(AttrId);
            break;
        case MDC_MOC_SCAN_CFG_PERI:
            err = ValidatePeriScanAttribute(AttrId);
            break;           
        default:
            err = ERROR_INVALID_PARAM;
            break;            
    }
    return err;
}

/**************************************************************************//*!
 *
 * @name  PushAttrObs
 *
 * @brief This function pushes Attribute to Observation Scan
 *
 * @param Handle	:	Object Handle 
 * @param AttrId	:	Attribute ID
 * @param AttrLen	:	Attribute Length
 * @param pAttrVal	:	Pointer to Attribute Data
 * @param pAvaType	:	Pointer to Ava Type
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_PARAM		: if Input parameters are incorrect
 ******************************************************************************
 * This function checks whether the attribute that has changed, can be changed 
 * after configuration is accepted by the host and updates the DIM if input is 
 * valid
 *****************************************************************************/
static ERR_CODE PushAttrObs(
    HANDLE Handle,			    /* [IN] Object Handle */
    OID_Type AttrId,            /* [IN] Attribute ID */	
    intu16 AttrLen,             /* [IN] Attribute Length */		
    void* pAttrVal,             /* [IN] Pointer to Attribute Data */
    void* pAvaType              /* [OUT] Pointer to Ava Type */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    intu16 Attrlen;
    PTR_BUFFSTACK pBuffStack = StackInit(0);

    err = ValidateAttr(Handle, AttrId);
    if(ERROR_SUCCESS != err)
    {
        StackDeInit(pBuffStack);
        return err;
    }
    
    err = ValidateAttrSize(AttrId, AttrLen, pAttrVal);
    if(ERROR_SUCCESS != err)
    {
        StackDeInit(pBuffStack);
        return err;
    }
    
    pBuffStack->total_size = (intu32)(AttrLen + ATTR_HEADER_SIZE);
    (intu8*)pBuffStack->top = (intu8*)pAvaType + pBuffStack->total_size;
    
    switch(AttrId)
    {
        case MDC_ATTR_TIME_BATT_REMAIN:
            err = PushBatMeasure(pBuffStack, (BatMeasure*)pAttrVal, &Attrlen);
            break;
        case MDC_ATTR_TIME_ABS_ADJUST:
        case MDC_ATTR_TIME_REL_HI_RES:
        case MDC_ATTR_TIME_ABS:
        case MDC_ATTR_TIME_STAMP_ABS:
        case MDC_ATTR_TIME_STAMP_REL_HI_RES:
        case MDC_ATTR_METRIC_STRUCT_SMALL:
            err = PushStack(pBuffStack, pAttrVal, AttrLen);
            break;
        case MDC_ATTR_CONFIRM_MODE:
        case MDC_ATTR_NUM_SEG:
        case MDC_ATTR_OP_STAT:
        case MDC_ATTR_VAL_BATT_CHARGE:
        case MDC_ATTR_POWER_STAT:
        case MDC_ATTR_ENUM_OBS_VAL_BASIC_BIT_STR:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_OID:
        case MDC_ATTR_METRIC_SPEC_SMALL:
        case MDC_ATTR_ID_PHYSIO:
        case MDC_ATTR_METRIC_ID_PART:
        case MDC_ATTR_UNIT_CODE:
        case MDC_ATTR_MSMT_STAT:
        case MDC_ATTR_SOURCE_HANDLE_REF:
        case MDC_ATTR_ENUM_OBS_VAL_PART:

            err = Pushintu16(pBuffStack, *(intu16*)pAttrVal, &Attrlen);
            break;
        case MDC_ATTR_ID_PHYSIO_LIST:
            err = PushMetricIdList(pBuffStack, (MetricIdList*)pAttrVal, &Attrlen);
            break;
        case MDC_ATTR_ATTRIBUTE_VAL_MAP:
            err = PushAttrValMap(pBuffStack, (AttrValMap*)pAttrVal, &Attrlen);
            break;
        case MDC_ATTR_SIMP_SA_OBS_VAL:
        case MDC_ATTR_UNIT_LABEL_STRING:
        case MDC_ATTR_ID_LABEL_STRING:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_STR:
            err = Pushoctet_string(pBuffStack, (octet_string*)pAttrVal, &Attrlen);        
            break;
        case MDC_ATTR_SCAN_REP_PD_MIN:
        case MDC_ATTR_SCAN_REP_PD:
        case MDC_ATTR_CONFIRM_TIMEOUT:
        case MDC_ATTR_CLEAR_TIMEOUT:
        case MDC_ATTR_METRIC_STORE_USAGE_CNT:
        case MDC_ATTR_TIME_REL:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_BIT_STR:
        case MDC_ATTR_TIME_STAMP_REL:
        case MDC_ATTR_TIME_PD_MSMT_ACTIVE:
            err = Pushintu32(pBuffStack, *(intu32*)pAttrVal, &Attrlen);
            break;
        case MDC_ATTR_NU_VAL_OBS_SIMP:
        case MDC_ATTR_NU_CMPD_VAL_OBS_SIMP:
        case MDC_ATTR_NU_VAL_OBS_BASIC:
        case MDC_ATTR_NU_CMPD_VAL_OBS_BASIC:
        case MDC_ATTR_NU_VAL_OBS:
        case MDC_ATTR_NU_CMPD_VAL_OBS:
            err = PushNumericVal(pBuffStack, AttrId, pAttrVal, &Attrlen);
            break;
        case MDC_ATTR_VAL_ENUM_OBS:
            err = PushEnumObsValue(pBuffStack, (EnumObsValue*)pAttrVal, &Attrlen);
            break;    
        case MDC_ATTR_MDS_TIME_INFO:
            err = PushMdsTimeInfo(pBuffStack, (MdsTimeInfo*)pAttrVal, &Attrlen);
            break;
        case MDC_ATTR_SCAN_HANDLE_ATTR_VAL_MAP:
            err = PushHandleAttrValMap(pBuffStack, (HandleAttrValMap*)pAttrVal, &Attrlen);
            break;
        default:
            err = ERROR_INVALID_PARAM;
            break;
    }
    if(ERROR_SUCCESS == err)
    {
        err = PushAttribute(pBuffStack, AttrId, AttrLen, &Attrlen); 
    }
    StackDeInit(pBuffStack);
    return err;
}
