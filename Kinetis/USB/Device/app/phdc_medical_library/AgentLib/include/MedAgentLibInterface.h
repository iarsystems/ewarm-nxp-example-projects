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
 * @file MedAgentLibInterface.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file Defines Ieee11073 Agent Library Interface functions and 
 *        Data Types
 *
 *****************************************************************************/
#ifndef _MEDAGENTLIB_INTERFACE_H
#define _MEDAGENTLIB_INTERFACE_H
/******************************************************************************
 * Includes
 *****************************************************************************/
#include "type.h"
#include "error.h"
#include "mds_config.h"
#include "til.h"
#ifdef LITTLE_ENDIAN
#include <ieee11073_phd_types_kinetis.h>
#else
#include "ieee11073_phd_types.h"
#endif
/******************************************************************************
 * MACROS
 *****************************************************************************/
/* These MACROS define Type of Report to be send to manager */
#define SCAN_REPORT_FIXED       (0)
#define SCAN_REPORT_MP_FIXED    (1)
#define SCAN_REPORT_VAR         (2)
#define SCAN_REPORT_MP_VAR      (3)
#define SCAN_REPORT_GROUPED     (4)
#define SCAN_REPORT_MP_GROUPED  (5)

/******************************************************************************
 * Types
 *****************************************************************************/
/* Agent Library Callback Events */
typedef enum _IEEE11073_EVENT
{
	/* 
		Callback Event when the device is successfully 
	   	connected to any transport 
	*/
	IEEE11073_TRANSPORT_CONNECT,
	/* 
		Callback Event when the device is disconnected 
	   	from the transport 
	*/
	IEEE11073_TRANSPORT_DISCONNECT,
	/* 
		Callback Event when the device has sent or received 
	   	an association release request 
	*/
	IEEE11073_ASSOCIATION_RELEASING,
	/* 
		Callback Event when the association between the 
	   	device and the Manager is released 
	*/
	IEEE11073_ASSOCIATION_RELEASED,
	/* 
		Callback Event when the device did not receive the response
	   	to the configuration event report within the timeout 
	   	specified by the IEEE11073-20601 specification 
	*/
    IEEE11073_CONFIGURATION_TIMEDOUT,
	/* 
		Callback Event when a device configuration is 
	   	rejected by the manager 
	*/
	IEEE11073_CONFIG_REJECTED,
	/* 
		Callback Event whenever an error result packet 
	   	is received by the device 
	*/
	IEEE11073_ERROR,
	/* 
		Callback Event whenever a reject result packet 
	   	is received by the device 
	*/
	IEEE11073_REJECT,
	/* 
		Callback Event whenever an abort packet is sent 
	   	or received by the device 
	*/
	IEEE11073_ABORT,
	/* 
		Callback Event when the device has successfully 
	   	established an association with the manager and 
	   	a device configuration is accepted by the manager 
	*/
	IEEE11073_OPERATING,
	/* 
		Callback Event whenever any event report is sent 
	   	over the transport 
	*/
	IEEE11073_EVNTRPT_SENT,
	/* 
		This event is received by the application periodically 
	   	with the period equal to the reporting interval of the 
	   	periodic scanner, if any 
	*/
    IEEE11073_PERIODIC_SCANNER_EVENT,
	/* 
		Callback Event whenever any request to clear a PM Segment 
	   	is received by the device 
	*/
	IEEE11073_CLEAR_PMSEGMENT,
	/* 	
		Callback Event whenever any request to send any PM Segment 
	   	data is received by the device 
	*/
	IEEE11073_TRIG_PMSEGMENT,
	/* 	
		The application should return pointer to the DIM upon 
	   	receiving this event 
	*/
	IEEE11073_INITIALIZE_DIM,
	/* 
		Callback Event when the Medical library needs a pointer 
	   	to the data proto list 
	*/
	IEEE11073_GET_DATAPROTO,
	/* 
		Callback Event whenever DIM initialization fails 
	*/
	IEEE11073_INITIALIZE_DIM_FAILED,
	/* 
		Callback Event when the device did not receive the response
	   	to the event report within the timeout. 
	*/
	IEEE11073_EVENTRPT_TIMEDOUT
}IEEE11073_EVENT;

/* Application Callback function pointer */
typedef void(_CODE_PTR_ MED_APP_CALLBACK)(IEEE11073_EVENT event_id, void *pvoid);

/* Structures for PM segment Information */

typedef struct _PMSEGMDATAXFER
{
    HANDLE Handle;
    InstNumber SegInstNum;
}PMSEGDATAXFER, *PPMSEGDATAXFER;

typedef struct _TRIGPMSEGDATA
{
    HANDLE Handle;
    TrigSegmDataXferRsp TrigSegmDataRsp;
}TRIGSEGMDATAXFRRSP, *PTRIGSEGMDATAXFRRSP;

typedef struct _CLRPMSEGMINFO
{
    HANDLE Handle;
    InstNumber SegmInstNum;    
}CLRPMSEGMINFO,*PCLRPMSEGMINFO;


#if (defined(_MC9S08MM128_H) || defined(_MC9S08JE128_H))
#pragma CODE_SEG PAGED_ROM
#endif
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
/* This funtion initializes IEEE11073 Agent Library */
extern ERR_CODE Ieee11073Initialize(
	PTIL pTil, 
	eShimID ShimId, 
    MED_APP_CALLBACK pfnAppCallback);

/* Association Interface */
/* Send Association Request to Manager */
extern ERR_CODE AgentSendAssociationRequest(
	DataProtoList *pDataProtoList);

/* Send Association Release Request to Manager */
extern ERR_CODE AgentSendAssociationReleaseRequest(
	Release_request_reason RelReqRes);

/* MDS Interface */
/* Send Measurement Data to Manager */
extern ERR_CODE AgentSendMeasurements(
	ObservationScanList* (*pObsScanList)[], 
    HANDLE Handle, 
    intu8 ReportType, 
    intu16 ScanCount, 
    intu8 bConfirm);

/* Send Person Measurement Data to Manager */
extern ERR_CODE AgentSendPersonMeasurements(
    ScanReportPerVarList* (*pScanRptPerVarList)[], 
    HANDLE Handle, 
    intu8 ReportType, 
    intu16 ScanCount, 
    intu8 bConfirm);

/* Helper Functions to Add Object Entry in Observation scan List */
extern ERR_CODE AddEntryToObsScanList(
	HANDLE Handle, 
	OID_Type AttrId, 
    intu16 AttrLen, 
    void* pAttrVal, 
    ObservationScanList* pObsScanList);

/* Helper Functions to Add Object Entry in Observation scan List */
extern ERR_CODE AddEntryToScanRptPerVarList(
	HANDLE Handle, 
	PersonId PersonID,
    OID_Type AttrId, 
    intu16 AttrLen, 
    void* pAttrVal,
    ScanReportPerVarList* pScanRptPerVarList);

	/* PM Store Interface */
	/* Helper Function for Updating PMSEGMENT Data on Persistant Storage Medium */
	extern ERR_CODE UpdatePmSegmentEntry(
		HANDLE Handle, 
		InstNumber InstNum,
	    ObservationScanList *pObsScanList);

	/* Send PMSegment Data to Manager */
	extern ERR_CODE SendSegmentData(
		SegmentDataEvent* pSegmDataEvent, 
	    HANDLE PmStoreHandle);

#if (defined(_MC9S08MM128_H) || defined(_MC9S08JE128_H))
#pragma CODE_SEG DEFAULT
#endif

#endif /* _MEDAGENTLIB_INTERFACE_H */
