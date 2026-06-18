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
 * @file til.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file is Transport Independent Layer Interface Header File
 *
 *****************************************************************************/
#ifndef _TIL_H
#define _TIL_H

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "error.h"
#include "type.h"
#include "stack.h"

#pragma pack(1)

/******************************************************************************
 * Types
 *****************************************************************************/
/*
  This Enumerated Data Type specifies different Events that are listened to by
  Transport Layer
*/
typedef enum _TRANSPORTEVENTID
{
	TRANSPORT_CONNECT = 0,
	TRANSPORT_DISCONNECT,
	TRANSPORT_DATARECIEVED,
	TRANSPORT_DATASENDCOMPLETE,
	TRANSPORT_GETDATABUFFER,
	TRANSPORT_GET_XFER_SIZE
}TRANSPORTEVENTID;

/* This Structure is Passed by SHIM Agent to TIL to request for Transfer Size */
typedef struct _TIL_XFER_SIZE
{
    uint_8_ptr in_buff;         /* Pointer to Buffer */
    uint_16 in_size;            /* Length of Buffer */
    uint_16 transfer_size;      /* Transfer Size */
}TIL_XFER_SIZE, *PTR_TIL_XFER_SIZE;

/* Transport Independent Layer Receive Buffer Structure */
typedef struct _TIL_RX_BUFF
{
    uint_16 in_size;            /* Size of input Buffer */
    uint_8_ptr in_buff;         /* Pointer to input Buffer */
    uint_16 out_size;           /* Size of Output Buffer */
    uint_8_ptr out_buff;        /* Pointer to Output Buffer */
    boolean meta_data_packet;   /* meta data packet flag */
}TIL_RX_BUFF, *PTR_TIL_RX_BUFF;

/* Application Callback Function Type */
typedef void*(_CODE_PTR_ APP_CALLBACK)(TRANSPORTEVENTID event_id, void* pArg);

/* Shim Initialize Function Type */
typedef ERR_CODE (_CODE_PTR_ PFN_SHIM_INITIALIZE)(APP_CALLBACK pAppCallback);

/* Shim De-Initialize Function Type */
typedef ERR_CODE (_CODE_PTR_ PFN_SHIM_DEINITIALIZE)(void);

/* Shim Send Data Function Type */
typedef ERR_CODE (_CODE_PTR_ PFN_SHIM_SEND_DATA)(
    boolean          meta_data,
    uint_8           num_tfr,
    uint_8           current_qos,
    PTR_BUFFSTACK    pBuffStack
	);

/* Shim Receive Function Type */
typedef ERR_CODE (_CODE_PTR_ PFN_SHIM_RECV_DATA)(
    uint_8           current_qos,
    PTR_BUFFSTACK    pBuffStack
	);

/* This Enumerated Data Type specifies SHIM IDs*/	
typedef enum
{
	SHIM_USB,
	SHIM_SERIAL,		/* Currently Not Supported */
	SHIM_TCP_ID,		/* Currently Not Supported */
	SHIM_BLUETOOTH		/* Currently Not Supported */
}eShimID;

/* Shim Interface Structure */
typedef struct _SHIM
{
	/* SHIM ID*/
	eShimID ShimId;
	/* Initialize Shim */
	PFN_SHIM_INITIALIZE	pfnShimInitialize;
	/* Deinitialize Shim */
	PFN_SHIM_DEINITIALIZE pfnShimDeInitialize;
	/* Send Data */
	PFN_SHIM_SEND_DATA pfnShimSendData;
	/* Receive Data */
	PFN_SHIM_RECV_DATA pfnShimRecvData;
}SHIM, *PSHIM;

/* Transport Independent Layer Structure */
typedef struct _TIL
{
	uint_8 ShimCount;
	PSHIM (*aShim)[];
}TIL, *PTIL;

/******************************************************************************
 * Global Functions
 *****************************************************************************/

/* This function initializes Transport Independent Layer */
extern void TIL_Initialize(PTIL pTil);

/* This function de-initializes Transport Independent Layer */
extern void TIL_DeInitialize(void);

/* This function initializes SHIM */
extern PSHIM TIL_StartTransport(
	PTIL pTil,
	eShimID ShimID,
	APP_CALLBACK pAppCallback);

/* This function de-initializes SHIM */
extern ERR_CODE TIL_StopTransport(
	PTIL pTil,
	eShimID ShimID);

/* This function sends Data through Shim */
extern ERR_CODE TIL_SendAPDU(
	PTIL pTil,
	eShimID ShimID,
	boolean metadata,
	uint_8 num_tfr,
	uint_8 current_qos,
	PTR_BUFFSTACK pBuffStack);

/* This function recevies Data from Shim */
extern ERR_CODE TIL_RecvAPDU(
	PTIL pTil,
	eShimID ShimID,
	uint_8 current_qos,
    PTR_BUFFSTACK pBuffStack);

#if !defined(__IAR_SYSTEMS_ICC__)
#pragma options align=reset
#endif
	
#endif