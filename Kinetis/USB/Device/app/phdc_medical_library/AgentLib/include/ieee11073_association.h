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
 * @file ieee11073_association.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file defines the Association layer API's
 *
 *****************************************************************************/
#ifndef _IEEE11073_ASSOCIATION_H
#define _IEEE11073_ASSOCIATION_H
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
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
/* Association Layer Functions */

/* This function checks Release Response received from Manager */
extern ERR_CODE ValidateReleaseResponse(
	PTR_BUFFSTACK pBuffStack);

/* This function handles Association Send Complete Event */
extern ERR_CODE ParseAssociationSendComplete(
	PHDCOMSTATE State, 
	PTR_BUFFSTACK pBuffStack);

/* This function pushes Presentation APDU Header to Stack */
extern ERR_CODE PushPrstApdu(
	PTR_BUFFSTACK pBuffStack);

/* This function sends Association Response to the Manager */
extern ERR_CODE SendAssociationResponse(
	Associate_result assoc_result, 
	DataProto *pDataProto);

/* This Function Sends Association Release Request to Manager */
extern ERR_CODE SendAssociationReleaseRequest(
	Release_request_reason RelReqRes);

/* This Function Sends Association Release Response to Manager */
extern ERR_CODE SendAssociationReleaseResponse(
	Release_response_reason RelResReason);

/* This Function Sends Presentation Apdu to Manager */
extern ERR_CODE SendPresentationApdu(
	PTR_BUFFSTACK pBuffStack);

/* This function Sends Abort reason to Manager */
extern ERR_CODE SendAbort(
	Abort_reason AbrtReqRes);

/* This function validates AssociationResponse recieved from Manager */
extern ERR_CODE ValidateAssociationResponse(
	PTR_BUFFSTACK pBuffStack);

/* This function resets Association Retry Counter */
extern void ResetAssociationRetryCount(void);

/* This function sends Association Request Packet to Manager */
extern ERR_CODE AgentSendAssociationRequest(
	DataProtoList *pDataProtoList);

/* This function sends Association Release Request to Manager */
extern ERR_CODE AgentSendAssociationReleaseRequest(
	Release_request_reason RelReqRes);

#endif /* _IEEE11073_ASSOCIATION_H */
