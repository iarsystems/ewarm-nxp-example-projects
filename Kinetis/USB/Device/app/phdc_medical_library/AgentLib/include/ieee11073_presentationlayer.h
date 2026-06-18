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
 * @file ieee11073_presentationlayer.h
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
#ifndef _IEEE11073_PRESENTATIONLAYER_H
#define _IEEE11073_PRESENTATIONLAYER_H
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <stack.h>
#include <ieee11073.h>
#ifdef LITTLE_ENDIAN
#include <ieee11073_phd_types_kinetis.h>
#else
#include <ieee11073_phd_types.h>
#endif
#include <ieee11073_dimstruct.h>
#include <ieee11073_dimfuncproto.h>
/******************************************************************************
 * Macros
 *****************************************************************************/
/* This function pushes Roiv Event Report DATA APDU to Buffer Stack */
#define PushRoivEventReportApdu(a,b)				\
	PushDataApdu(a, b, ROIV_CMIP_EVENT_REPORT_CHOSEN)
/* This function pushes Roiv Confirmed Event 
   Report DATA APDU to Buffer Stack */
#define PushRoivConfirmedEventReportApdu(a,b)		\
	PushDataApdu(a, b, ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN)
/* This function pushes Roiv Get DATA APDU to Buffer Stack */
#define PushRoivGetApdu(a,b)						\
	PushDataApdu(a, b, ROIV_CMIP_GET_CHOSEN)
/* This function pushes Roiv Set DATA APDU to Buffer Stack */
#define PushRoivSetApdu(a,b)						\
	PushDataApdu(a, b, ROIV_CMIP_SET_CHOSEN)
/* This function pushes Roiv Confirmed Set DATA APDU to Buffer Stack */
#define PushRoivConfirmedSetApdu(a,b)				\
	PushDataApdu(a, b, ROIV_CMIP_CONFIRMED_SET_CHOSEN)
/* This function pushes Roiv Action DATA APDU to Buffer Stack */
#define PushRoivActionApdu(a,b)						\
	PushDataApdu(a, b, ROIV_CMIP_ACTION_CHOSEN)
/* This function pushes Roiv Confirmed Action DATA APDU to Buffer Stack */
#define PushRoivConfirmedActionApdu(a,b)			\
	PushDataApdu(a, b, ROIV_CMIP_CONFIRMED_ACTION_CHOSEN)
/* This function pushes Rors Confirmed Event Report DATA APDU to Buffer Stack */
#define PushRorsConfirmedEventReportApdu(a,b)		\
	PushDataApdu(a, b, RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN)
/* This function pushes Rors Get DATA APDU to Buffer Stack */
#define PushRorsGetApdu(a,b)						\
	PushDataApdu(a, b, RORS_CMIP_GET_CHOSEN)
/* This function pushes Rors Confirmed Set DATA APDU to Buffer Stack */
#define PushRorsConfirmedSetApdu(a,b)				\
	PushDataApdu(a, b, RORS_CMIP_CONFIRMED_SET_CHOSEN)
/* This function pushes Rors Confirmed Action DATA APDU to Buffer Stack */
#define PushRorsConfirmedActionApdu(a,b)			\
	PushDataApdu(a, b, RORS_CMIP_CONFIRMED_ACTION_CHOSEN)
/* This function pushes Roer DATA APDU to Buffer Stack */
#define PushRoerApdu(a,b)	PushDataApdu(a, b, ROER_CHOSEN)
/* This function pushes Rorj DATA APDU to Buffer Stack */
#define PushRorjApdu(a,b)	PushDataApdu(a, b, RORJ_CHOSEN)


/* This function pushes Segment Data Confirmed Event 
   Report APDU to Buffer Stack */
#define PushSegmentDataRptHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_SEGMENT_DATA, d)
/* This function pushes Buffered Fixed Format Confirmed Scan Event 
   Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushConfBufScanRptFixedHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_FIXED, d)
/* This function pushes Buffered Variable Format Confirmed Scan 
   Event Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushConfBufScanRptVarHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_VAR, d)
/* This function pushes Buffered Grouped Format Confirmed Scan 
   Event Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushConfBufScanRptGrpHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_GROUPED, d)
/* This function pushes Buffered Multi Person Fixed Format Confirmed 
   Scan vent Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushConfBufScanRptMPFixedHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_MP_FIXED, d)
/* This function pushes Buffered Multi Person Variable Format Confirmed 
   Scan Event Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushConfBufScanRptMPVarHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_MP_VAR, d)
/* This function pushes Buffered Multi Person Grouped Format Confirmed 
   Scan Event Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushConfBufScanRptMPGrpHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_MP_GROUPED, d)

/* This function pushes UnBuffered Fixed Format Confirmed Scan Event 
   Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushConfUnBufScanRptFixedHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_FIXED, d)
/* This function pushes UnBuffered Variable Format Confirmed Scan 
   Event Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushConfUnBufScanRptVarHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_VAR, d)
/* This function pushes UnBuffered Grouped Format Confirmed Scan 
   Event Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushConfUnBufScanRptGrpHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_GROUPED, d)
/* This function pushes UnBuffered Multi Person Fixed Format Confirmed 
   Scan vent Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushConfUnBufScanRptMPFixedHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_MP_FIXED, d)
/* This function pushes UnBuffered Multi Person Variable Format Confirmed 
   Scan Event Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushConfUnBufScanRptMPVarHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_MP_VAR, d)
/* This function pushes UnBuffered Multi Person Grouped Format Confirmed 
   Scan Event Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushConfUnBufScanRptMPGrpHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_MP_GROUPED, d)

/* This function pushes Confirmed Configuration Event Report APDU to 
   Buffer Stack. This format of report is sent via MDS */
#define PushConfConfigDataRptHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_CONFIG, d)
/* This function pushes Confirmed Scan Report Fixed Format 
   Event Report APDU to Buffer Stack. This format of report is sent via MDS */
#define PushConfScanRptFixedHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_SCAN_REPORT_FIXED, d)
/* This function pushes Confirmed Scan Report Variable Format 
   Event Report APDU to Buffer Stack. This format of report is sent via MDS */
#define PushConfScanRptVarHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_SCAN_REPORT_VAR, d)
/* This function pushes Confirmed Scan Report Multi Person Fixed Format 
   Event Report APDU to Buffer Stack. This format of report is sent via MDS */
#define PushConfScanRptMPFixedHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_SCAN_REPORT_MP_FIXED, d)
/* This function pushes Confirmed Scan Report Multi Person Variable Format 
   Event Report APDU to Buffer Stack. This format of report is sent via MDS */
#define PushConfScanRptMPVarHeader(a,b,c,d) \
    PushConfEventRpt(a, b, c, MDC_NOTI_SCAN_REPORT_MP_VAR, d)

/* This function pushes UnConfirmed Configuration Event Report APDU to 
   Buffer Stack. This format of report is sent via MDS */
#define PushUnConfConfigDataRptHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_CONFIG, d)
/* This function pushes UnConfirmed Scan Report Fixed Format 
   Event Report APDU to Buffer Stack. This format of report is sent via MDS */
#define PushUnConfScanRptFixedHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_SCAN_REPORT_FIXED, d)
/* This function pushes UnConfirmed Scan Report Variable Format 
   Event Report APDU to Buffer Stack. This format of report is sent via MDS */
#define PushUnConfScanRptVarHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_SCAN_REPORT_VAR, d)
/* This function pushes UnConfirmed Scan Report Multi Person Fixed Format 
   Event Report APDU to Buffer Stack. This format of report is sent via MDS */
#define PushUnConfScanRptMPFixedHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_SCAN_REPORT_MP_FIXED, d)
/* This function pushes UnConfirmed Scan Report Multi Person Variable Format 
   Event Report APDU to Buffer Stack. This format of report is sent via MDS */
#define PushUnConfScanRptMPVarHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_SCAN_REPORT_MP_VAR, d)

/* This function pushes Buffered Fixed Format UnConfirmed Scan Event 
   Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushUnConfBufScanRptFixedHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_FIXED, d)
/* This function pushes Buffered Variable Format UnConfirmed Scan 
   Event Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushUnConfBufScanRptVarHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_VAR, d)
/* This function pushes Buffered Grouped Format UnConfirmed Scan 
   Event Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushUnConfBufScanRptGrpHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_GROUPED, d)
/* This function pushes Buffered Multi Person Fixed Format UnConfirmed 
   Scan vent Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushUnConfBufScanRptMPFixedHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_MP_FIXED, d)
/* This function pushes Buffered Multi Person Variable Format UnConfirmed 
   Scan Event Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushUnConfBufScanRptMPVarHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_MP_VAR, d)
/* This function pushes Buffered Multi Person Grouped Format UnConfirmed 
   Scan Event Report APDU to Buffer Stack. This format of report is sent via 
   a Periodic Scanner */
#define PushUnConfBufScanRptMPGrpHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_BUF_SCAN_REPORT_MP_GROUPED, d)
    
/* This function pushes UnBuffered Fixed Format UnConfirmed Scan Event 
   Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushUnConfUnBufScanRptFixedHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_FIXED, d)
/* This function pushes UnBuffered Variable Format UnConfirmed Scan 
   Event Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushUnConfUnBufScanRptVarHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_VAR, d)
/* This function pushes UnBuffered Grouped Format UnConfirmed Scan 
   Event Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushUnConfUnBufScanRptGrpHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_GROUPED, d)
/* This function pushes UnBuffered Multi Person Fixed Format UnConfirmed 
   Scan vent Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushUnConfUnBufScanRptMPFixedHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_MP_FIXED, d)
/* This function pushes UnBuffered Multi Person Variable Format UnConfirmed 
   Scan Event Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushUnConfUnBufScanRptMPVarHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_MP_VAR, d)
/* This function pushes UnBuffered Multi Person Grouped Format UnConfirmed 
   Scan Event Report APDU to Buffer Stack. This format of report is sent via 
   an Episodic Scanner */
#define PushUnConfUnBufScanRptMPGrpHeader(a,b,c,d) \
    PushUnConfEventRpt(a, b, c, MDC_NOTI_UNBUF_SCAN_REPORT_MP_GROUPED, d)

/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/    
/* This function pushes DATA APDU to Buffer Stack */
extern ERR_CODE PushDataApdu(
	PTR_BUFFSTACK pBuffStack, 
	InvokeIDType InvokeID, 
    intu16 Choice);

/* This function pushes Confirmed Event Report APDU to Buffer Stack */
extern ERR_CODE PushConfEventRpt(
	PTR_BUFFSTACK pBuffStack, 
	HANDLE handle, 
    RelativeTime EventTime, 
    OID_Type EventType, 
    intu16 *pAttrLen);

/* This function pushes UnConfirmed Event Report APDU to Buffer Stack */
extern ERR_CODE PushUnConfEventRpt(
	PTR_BUFFSTACK pBuffStack, 
	HANDLE handle, 
    RelativeTime EventTime, 
    OID_Type EventType, 
    intu16 *pAttrLen);

/* This function pushes RORS Confirmed Set APDU to Buffer Stack */
extern ERR_CODE PushSetResultSimple(
	PTR_BUFFSTACK pBuffStack, 
	HANDLE handle, 
    InvokeIDType InvokeId, 
    intu16 *pAttrLen);

/* This function pushes Configuration Report Header to Buffer Stack */
extern ERR_CODE PushConfigReportHeader(
	PTR_BUFFSTACK pBuffStack, 
	ConfigId ConfId, 
    intu16 ConfigObjListCnt, 
    intu16* pAttrLen);

/* This function sends ROER APDU to Manager */
extern ERR_CODE SendErrorResult(
	ERROR errCode, 
	Any* pAny, 
	InvokeIDType InvokeID);

/* This function sends Abort APDU to Manager */
extern ERR_CODE SendAbort(
	Abort_reason AbrtReqRes);

/* This function sends Prensation APDU to Manager */
extern ERR_CODE SendPresentationApdu(
	PTR_BUFFSTACK pBuffStack);

/* This function sends RORJ APDU to Manager */
extern ERR_CODE SendRorj(
	RorjProblem problem, 
	InvokeIDType InvokeID);

/* This function pushes Event Report Argument Simple APDU to Buffer 
   Stack */
extern ERR_CODE PushEventReportArgSimple(
	PTR_BUFFSTACK pBuffStack,
	HANDLE handle,
	RelativeTime EventTime,
	OID_Type EventType,
	intu16 *pAttrLen);

/* This function parses Presentation Apdu Received from Manager */
extern ERR_CODE ParsePrstApdu(
	PHDCOMSTATE State,
	PTR_BUFFSTACK pBuffStack);

/* This function pushes DATA APDU to Buffer Stack */
extern ERR_CODE PushDataApdu(
	PTR_BUFFSTACK pBuffStack, 
	InvokeIDType InvokeID, 
	intu16 Choice);

/* This function sends ROER APDU to Manager */
extern ERR_CODE SendErrorResult(
	ERROR errCode, 
	Any* pAny, 
	InvokeIDType InvokeID);

/* This function sends RORJ APDU to Manager */
extern ERR_CODE SendRorj(
	RorjProblem problem, 
	InvokeIDType InvokeID);

/* This function pushes RORS Confirmed Set APDU to Buffer Stack */
extern ERR_CODE PushSetResultSimple(
	PTR_BUFFSTACK pBuffStack, 
	HANDLE handle, 
	InvokeIDType InvokeId, 
	intu16 *pAttrLen);

/* This function handles Presentation Apdu Send Complete */
extern ERR_CODE ParsePrstSendComplete(
	PHDCOMSTATE State, 
	PTR_BUFFSTACK pBuffStack);

/* This function resets SendInvoke Id */
extern void ResetSendInvokeId(void);

#endif /* _IEEE11073_PRESENTATIONLAYER_H */
