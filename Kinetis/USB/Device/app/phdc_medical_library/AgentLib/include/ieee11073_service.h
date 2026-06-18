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
 * @file ieee11073_service.h
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
#ifndef _IEEE11073_SERVICE_H
#define _IEEE11073_SERVICE_H
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <stack.h>
#include <ieee11073_phd_types.h>
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
/* Service Layer Functions */

/* This function is called from upper Layer. 
   Used by Agent Specialization to send event report to Manager */
extern ERR_CODE ObjectSendEventReport(
	intu8 bconfirm, 
	PTR_BUFFSTACK pBuffStack);

/* This function is called from lower Layer. 
   Used by Manager to retrieve values of Agent MDS and PM-Store Objects */
extern ERR_CODE ObjectAccessGetService(
	InvokeIDType InvokeID, 
	PTR_BUFFSTACK pReqBuffStack);

/* This function is called from lower Layer. 
   Used by Manager to set values of Agent Scanner Objects */
extern ERR_CODE ObjectAccessSetService(
	InvokeIDType InvokeID, 
	PTR_BUFFSTACK pBuffStack, 
	intu8 bRsp);

/* This function is called from lower Layer when 
   Confirmed/Unconfirmed EVENT Report Chosen APDU is received */
extern ERR_CODE ObjectAccessEventReportService(
	InvokeIDType InvokeID, 
	PTR_BUFFSTACK pBuffStack, 
	intu8 bRsp);

/* This function is called from lower Layer.
   Used by Manager to invoke actions supported by agent */
extern ERR_CODE ObjectAccessActionService(
	InvokeIDType InvokeID, 
	PTR_BUFFSTACK pBuffStack, 
	intu8 bRsp);

#endif /* _IEEE11073_SERVICE_H */
