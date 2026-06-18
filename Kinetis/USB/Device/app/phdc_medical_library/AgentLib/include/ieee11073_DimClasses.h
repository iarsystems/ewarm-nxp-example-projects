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
 * @file ieee11073_DimClasses.h
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
#ifndef _IEEE11073_DIMCLASSES_H
#define _IEEE11073_DIMCLASSES_H

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <stack.h>
#include <ieee11073_phd_types.h>

/******************************************************************************
 * Types
 *****************************************************************************/
/* structure to hold Triggered PM Segment Data Transfer response */
typedef struct _TRIGPMSEGDATA
{
    HANDLE Handle;
    TrigSegmDataXferRsp TrigSegmDataRsp;
}TRIGSEGMDATAXFRRSP, *PTRIGSEGMDATAXFRRSP;

/* Structure to hold information about the segment to be cleared */
typedef struct _CLRPMSEGMINFO
{
    HANDLE Handle;
    InstNumber SegmInstNum;    
}CLRPMSEGMINFO, *PCLRPMSEGMINFO;

/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
/* This function returns the type and the pointer of the object defined 
   by the input handle */
extern void* GetObjectByHandle(
	HANDLE objHandle, 
	intu16 *objType);

  extern intu16 GetScannerSetArgumentSize(PTR_BUFFSTACK pReqBuffStack);

	/* This function handles Episoidic Configuration Scanner Set Argument 
 	   Request */
	extern ERR_CODE EpiCfgScannerSetArgument(
		PTR_BUFFSTACK pBuffStack,
	    PEPICFGSCANNER pEpiScanner, 
	    PTR_BUFFSTACK pRspBuffStack);

	/* This function returns size of Response Buffer for Episoidic 
 	   Configuration Scanner Set Argument Request */
	extern intu16 GetEpiCfgScannerSetArgumentSize(
		PTR_BUFFSTACK pReqBuffStack);

	/* This function handles Periodic Configuration Scanner Set Argument 
	   Request */
	extern ERR_CODE PeriCfgScannerSetArgument(
		PTR_BUFFSTACK pBuffStack,
	    PPERICFGSCANNER pPeriScanner, 
	    PTR_BUFFSTACK pRspBuffStack);

	/* This function returns size of Response Buffer for Periodic 
 	   Configuration Scanner Set Argument Request */
	extern intu16 GetPeriCfgScannerSetArgumentSize(
		PTR_BUFFSTACK pReqBuffStack);

	/* This function returns size of PM Store Object Method Response Size */
	extern intu16 GetPmStoreObjectMethodSize(
		OID_Type ActionType, 
		PTR_BUFFSTACK pReqBuffStack, 
		PPMSTORE pmStore);
		
	/* This function handles PM Store Get Object Attribute request */
	extern ERR_CODE PMStoreGetObjectAttribute(
		PPMSTORE pPmStore, 
		PTR_BUFFSTACK pReqBuffStack, 
		PTR_BUFFSTACK pRspBuffStack);
		
	/* This function returns PM Store Attribute Size */
	extern intu16 PMStoreGetObjectAttributeSize(
		PTR_BUFFSTACK pBuffStack, 
		PPMSTORE pPmStore);
		
	/* This function handles PM Store Object Methods */
	extern ERR_CODE PMStoreObjectMethods(
		OID_Type ActionType, 
		PTR_BUFFSTACK pReqBuffStack, 
		PTR_BUFFSTACK pRspBuffStack, 
		PMSTORE* pPmStore);

/* This function returns size of MDS Object Method Response Size */
extern intu16 GetMdsObjectMethodSize(
	OID_Type ActionType);

/* This function handles MDS Get Object Attribute Request */
extern ERR_CODE MDSGetObjectAttribute(
	PTR_BUFFSTACK pReqBuffStack, 
	PTR_BUFFSTACK pRspBuffStack);

/* This function returns size of Response Buffer for MDS Get Object 
   Attrbiute Request */
extern intu16 MdsGetObjectAttributeSize(
	PTR_BUFFSTACK pBuffStack);

/* This function initializes DIM */
extern ERR_CODE AgentDimInitialize(void);

/* This function sends configuration event report */
extern ERR_CODE AgentDimSendConfigEventReport(
	intu8 bFlag);

/* This function sends the next configuration, if available */
extern ERR_CODE AgentDimSendNextConfigEventReport(
	intu8 bFlag);

/* This function cleans up Ieee11073 Buffers and Timers */
extern void Ieee11073Cleanup(void);

/* This function is called from lower Layer.
   Invoked to signal that Event report is sent */
extern void ObjectAccessCallback(
	PTR_BUFFSTACK pReqBuffStack);

/* This function handles DIM de-initialize */
extern void AgentDimDeInitialize(void);

/* This function is called by the upper layer to create Observation Scan
   List */
extern ERR_CODE AddEntryToObsScanList(
	HANDLE handle,
	OID_Type AttrId,
	intu16 AttrLen,
	void* pAttrVal,
	ObservationScanList* pObsScanList);

/* This function is called by the upper layer to create Scan Report Per 
   Var List */
extern ERR_CODE AddEntryToScanRptPerVarList(
	HANDLE handle,
	PersonId PersonID,
	OID_Type AttrId,
	intu16 AttrLen,
	void* pAttrVal,
	ScanReportPerVarList* pScanRptPerVarList);

/* This function is called by the upper layer to send measurements when 
   multi person reports are not supported */
extern ERR_CODE AgentSendMeasurements(
	ObservationScanList* (*pObsScanList)[], 
    HANDLE Handle, 
    intu8 ReportType, 
    intu16 ScanCount, 
    intu8 bConfirm);

/* This function is called by the upper layer to send multi person 
   measurements */
extern ERR_CODE AgentSendPersonMeasurements(
    ScanReportPerVarList* (*pScanRptPerVarList)[], 
    HANDLE Handle, 
    intu8 ReportType, 
    intu16 ScanCount, 
    intu8 bConfirm);

/* This function sends PM Segment Data Event to Manager */
extern ERR_CODE SendSegmentData(
	SegmentDataEvent* pSegmDataEvent,
	HANDLE PmStoreHandle);

#endif /* _IEEE11073_DIMCLASSES_H */