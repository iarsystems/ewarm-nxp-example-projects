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
 * @file ieee11073.h
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
#ifndef _IEEE11073_H
#define _IEEE11073_H
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <stack.h>
#include <til.h>
#ifdef LITTLE_ENDIAN
#include <ieee11073_phd_types_kinetis.h>
#else
#include <ieee11073_phd_types.h>
#endif

/******************************************************************************
 * Macros
 *****************************************************************************/
 /* Report Type */
#define SCAN_REPORT_FIXED       (0)
#define SCAN_REPORT_MP_FIXED    (1)
#define SCAN_REPORT_VAR         (2)
#define SCAN_REPORT_MP_VAR      (3)
#define SCAN_REPORT_GROUPED     (4)
#define SCAN_REPORT_MP_GROUPED  (5)

/* Association Timed out code */
#define ASSOCIATION_TIMEDOUT    (5) 

/* Invalid Timer Index */
#define INVALID_TIMER_INDEX 	(0xFF)

/******************************************************************************
 * Types
 *****************************************************************************/
/* device states */
#pragma pack(1)

typedef enum _PHDCOMSTATE
{
    /* No Transport Connected */
    DISCONNECTED = 0,
    /* Transport connected, Not yet Associated */
    UNASSOCIATED = 1,
    /* Association Request sent to the manager */
    ASSOCIATING = 2,
    /* Sending Configuration to the Manager */
    CONFIGURING_SENDING_CONFIG = 3,
    /* Waiting for configuration response from the manager */
    CONFIGURING_WAITING_APPROVAL = 4,
    /* Confiuration accepted by the manager */
    OPERATING = 5,
    /* Disassociating State */
    DISASSOCIATING = 6
}PHDCOMSTATE;

/* Application Callback events */
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
typedef void(_CODE_PTR_ MED_APP_CALLBACK)(
	IEEE11073_EVENT event_id, 
	void *pvoid);

typedef struct _AssocData
{
    /* 
        Stores Pointer to TIL
    */
	PTIL pTil;
    /* 
        Stores SHIM ID
    */
	eShimID ShimId;
    /* 
        Stores IEEE11073 Association State
    */
	PHDCOMSTATE AssocState;
    /* 
        Stores Timer Index
    */
	intu8 TimerIndex;
    /* 
        Stores Send and Recv QOS
    */
	intu8 SendQos, RecvQos;
    /* 
        Stores Applcation Callback
    */
	MED_APP_CALLBACK pfnAppCallback;
    /* 
        Stores last APDU send 
    */
	PTR_BUFFSTACK pBuffStack;   
}ASSOC_STRUCT;
#pragma options align=reset
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
/* This funtion initializes IEEE11073 Agent Library */
extern ERR_CODE Ieee11073Initialize(
	PTIL pTil, 
	eShimID ShimId, 
    MED_APP_CALLBACK pfnAppCallback);

/* This funtion returns Send Data QOS */
extern intu8 Ieee11073GetSendQos(void);

/* This funtion returns Receive Data QOS */
extern intu8 Ieee11073GetRecvQos(void);

/* This funtion sets Send Data QOS */
extern void Ieee11073SetSendQos(intu8 SendQos);

/* This funtion sets Receive Data QOS */
extern void Ieee11073SetRecvQos(intu8 RecvQos);

/* This funtion returns IEEE11073 Library State */
extern PHDCOMSTATE Ieee11073GetState(void);

/* This funtion returns IEEE11073 Library State */
extern void Ieee11073SetState(PHDCOMSTATE State);

/* This funtion returns Pointer to TIL */
extern PTIL Ieee11073GetTil(void);

/* This funtion returns Pointer to Shim */
extern eShimID Ieee11073GetShim(void);

/* This funtion returns Pointer to Application Callback Function */
extern MED_APP_CALLBACK Ieee11073GetAppCallback(void);

/* This funtion sets Timer Index */
extern void Ieee11073SetTimerIndex(intu8 TimerIndex);

/* This funtion returns Timer Index */
extern intu8 Ieee11073GetTimerIndex(void);

/* This funtion returns Pointer to Buffer Stack */
extern PTR_BUFFSTACK Ieee11073GetBuffStack(void);

/* This funtion saves Pointer to Buffer Stack in Ieee11073 Structure */
extern void Ieee11073SetBuffStack(PTR_BUFFSTACK pBuffStack);

#endif /* _IEEE11073_H */
