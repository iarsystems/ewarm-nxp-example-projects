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
 * @file Ieee11073Association.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains IEEE11073 Agent Library Association Layer 
 *        Implementation
 *
 *****************************************************************************/
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <stack.h>
#include <til.h>
#include <ieee11073.h>
#ifdef LITTLE_ENDIAN
#include <ieee11073_phd_types_kinetis.h>
#else
#include <ieee11073_phd_types.h>
#endif
#include <ieee11073_dimfuncproto.h>
#include <RealTimerCounter.h>
#include <ieee11073_presentationlayer.h>
#include <ieee11073_association.h>
#include <ieee11073_ClassCallback.h> 
#include <ieee11073_dataproto.h>
#include <ieee11073_DimClasses.h>
/*****************************************************************************
 * Macro's
 *****************************************************************************/
#define PushAbort_reason			Pushintu16
#define PushAssociate_result		Pushintu16
#define PushRelease_request_reason	Pushintu16
#define PushRelease_response_reason	Pushintu16
#define PushAssociationVersion		Pushintu32
#define PushApduHeader				PushAttribute

/* 10 sec Association Timeout */
#define ASSOC_TIMEOUT           10000   
/* 3 sec Timeout */
#define ASSOC_RELEASE_TIMEOUT	3000	
/* Association Retry Count */
#define RC_ASSOC						(3)		
#define ASSOC_DATA_LENGTH				(4)
#define ASSOC_REQ_HEADER				(4)
#define ABRT_DATA_LENGTH				(2)
#define RLRE_DATA_LENGTH				(2)
#define RLRQ_DATA_LENGTH				(2)
#define ASSOC_RSP_HEADER_SIZE			(2)
#define ASSOC_REJECT_DATA_SIZE			(6)
#define ABRT_APDU_DATA_SIZE				(2)

/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/

/*****************************************************************************
 * Local Variables
 *****************************************************************************/
static volatile intu8 AssocRetryCount = RC_ASSOC;

/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static void PHD_Association_Timer_Callback(void* arg);
static void PHD_Dissociation_Timer_Callback(void* arg);
static ERR_CODE PushApdu(PTR_BUFFSTACK pBuffStack, intu16 Choice);
static ERR_CODE PushAarqApdu(PTR_BUFFSTACK pBuffStack);
static ERR_CODE PushRlrqApdu(PTR_BUFFSTACK pBuffStack);
static ERR_CODE PushAbrtApdu(PTR_BUFFSTACK pBuffStack);
static ERR_CODE DispatchAssociationLayerApdu(intu16 Choice, 
	PTR_BUFFSTACK pBuffStack);
static ERR_CODE PushAareApdu(PTR_BUFFSTACK pBuffStack);
static ERR_CODE PushRlreApdu(PTR_BUFFSTACK pBuffStack);
static ERR_CODE PushAssociationRequest(PTR_BUFFSTACK pBuffStack, 
    AssociationVersion AssocVersion, DataProtoList *pDataProtoList);
static ERR_CODE PushAssociationResponse(PTR_BUFFSTACK pBuffStack, 
    Associate_result assoc_result, DataProto *pDataProto);
static ERR_CODE PushAssociationReleaseRequest(PTR_BUFFSTACK pBuffStack, 
    Release_request_reason RelReqRes);
static ERR_CODE PushAbort(PTR_BUFFSTACK pBuffStack, Abort_reason AbrtReqRes);
static ERR_CODE SendAssociationRequest(PTR_BUFFSTACK pBuffStack);
static ERR_CODE PushAssociationReleaseResponse(PTR_BUFFSTACK pBuffStack,				
	Release_response_reason RelResReason);
static ERR_CODE ValidateDataProto(PTR_BUFFSTACK pBuffStack);
/*****************************************************************************
 * Global Functions
 *****************************************************************************/

/**************************************************************************//*!
 *
 * @name  PushPrstApdu
 *
 * @brief This function pushes Presentation APDU to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Presentation APDU to Buffer Stack
 *****************************************************************************/
ERR_CODE PushPrstApdu(
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
	/* All States other than DISCONNECTED */
	switch(Ieee11073GetState())
	{
		case DISCONNECTED:
			break;
		default:
		    return PushApdu(pBuffStack, (intu16)PRST_CHOSEN); 
			break;
	}
	return ERROR_INVALID_REQUEST;
}

/**************************************************************************//*!
 *
 * @name  SendPresentationApdu
 *
 * @brief This function sends Prensation APDU to Manager
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function sends Prensation APDU to Manager
 *****************************************************************************/
ERR_CODE SendPresentationApdu(
	PTR_BUFFSTACK pBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
    return DispatchAssociationLayerApdu(PRST_CHOSEN, pBuffStack);
}

/**************************************************************************//*!
 *
 * @name  SendAbort
 *
 * @brief This function sends Abort APDU to Manager
 *
 * @param AbortReason	:	Abort Request Reason
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function sends Abort APDU to Manager
 *****************************************************************************/
ERR_CODE SendAbort(
	Abort_reason AbortReason	/* [IN] Abort Request Reason */ 
)
{
	ERR_CODE err;
	PTR_BUFFSTACK pSendBuffStack;
	intu16 OutBuffSize;
  intu8 timer_index;
    
  /* Disable all the timers associated with IEEE11073 */
  timer_index = Ieee11073GetTimerIndex();
  (void)PHD_Remove_Timer(&timer_index);
  Ieee11073SetTimerIndex(timer_index);

	/* Set State to Unassociated */
	OutBuffSize = (intu16)(ABRT_DATA_LENGTH + APDU_HEADER_SIZE);
	pSendBuffStack = StackInit(OutBuffSize);	

  err = PushAbort(pSendBuffStack, AbortReason);
  if(ERROR_SUCCESS == err)
  {
   err = DispatchAssociationLayerApdu(ABRT_CHOSEN, pSendBuffStack);
  }
	
	if(ERROR_SUCCESS != err)
	{
	    StackDeInit(pSendBuffStack);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  AgentSendAssociationReleaseRequest
 *
 * @brief This function sends Association Release Request to Manager
 *
 * @param RelReqRes	:	Release Request Reason
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function sends Association Release Request to Manager
 *****************************************************************************/
ERR_CODE AgentSendAssociationReleaseRequest(
	Release_request_reason RelReqRes	/* [IN] Release Request Reason */
)
{
    return SendAssociationReleaseRequest(RelReqRes);
}

/**************************************************************************//*!
 *
 * @name  SendAssociationReleaseRequest
 *
 * @brief This function sends Association Release Request to Manager
 *
 * @param RelReqRes	:	Release Request Reason
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function sends Association Release Request to Manager
 *****************************************************************************/
ERR_CODE SendAssociationReleaseRequest(
	Release_request_reason RelReqRes	/* [IN] Release Request Reason */
)
{
	ERR_CODE err;
	PTR_BUFFSTACK pSendBuffStack;
	intu16 OutBuffSize;
	
    /* Make Sure nothing is pending */
    Ieee11073Cleanup();    

	OutBuffSize = (intu16)(RLRQ_DATA_LENGTH + APDU_HEADER_SIZE);
	pSendBuffStack = StackInit(OutBuffSize);
	
	do
	{
	    if(ERROR_SUCCESS != (err = PushAssociationReleaseRequest(
	        pSendBuffStack, RelReqRes)))
	    {
	        break;
	    }
	    
		/* Send APDU*/
		err = DispatchAssociationLayerApdu(RLRQ_CHOSEN, pSendBuffStack);
	}while(0);
	if(ERROR_SUCCESS != err)
	{
	    StackDeInit(pSendBuffStack);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  SendAssociationReleaseResponse
 *
 * @brief This function sends Association Release Response to Manager
 *
 * @param RelReqRes	:	Release Response Reason
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function sends Association Release Response to Manager
 *****************************************************************************/
ERR_CODE SendAssociationReleaseResponse(
	Release_response_reason RelResReason	/* [IN] Release Response Reason */
)
{
	ERR_CODE err;
	PTR_BUFFSTACK pSendBuffStack;
	intu16 OutBuffSize;
	OutBuffSize = (intu16)(RLRE_DATA_LENGTH + APDU_HEADER_SIZE);
	pSendBuffStack = StackInit(OutBuffSize);
	do
	{
    	if(ERROR_SUCCESS != (err = PushAssociationReleaseResponse(
    	    pSendBuffStack, RelResReason)))
	    {
	        break;
	    }
		/* Send APDU*/
		err = DispatchAssociationLayerApdu(RLRE_CHOSEN, pSendBuffStack);
	}while(0);
	if(ERROR_SUCCESS != err)
	{
	    StackDeInit(pSendBuffStack);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  SendAssociationResponse
 *
 * @brief This function sends Association Response to Manager
 *
 * @param AssocResult	:	Association Result
 * @param pDataProto	:	Pointer to Data Proto
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function sends Association Response to Manager
 *****************************************************************************/
ERR_CODE SendAssociationResponse(
	Associate_result AssocResult,	/* [IN] Association Result*/
	DataProto* pDataProto			/* [IN] Pointer to Data Proto */
)
{
	ERR_CODE err;
	PTR_BUFFSTACK pSendBuffStack;
	intu16 OutBuffSize = (intu16)(sizeof(DataProtoId) + ANY_HEADER_SIZE + 
		pDataProto->data_proto_info.length + 
		ASSOC_RSP_HEADER_SIZE + APDU_HEADER_SIZE);
	pSendBuffStack = StackInit(OutBuffSize);
	if(pSendBuffStack == NULL)
		return ERROR_INSUFFICIENT_MEMORY;
	do
	{
	    if(ERROR_SUCCESS != (err = PushAssociationResponse(pSendBuffStack, 
	        AssocResult, pDataProto)))
	    {
	        break;
	    }
		err = DispatchAssociationLayerApdu(AARE_CHOSEN, pSendBuffStack);
	}while(0);
	if(ERROR_SUCCESS != err)
	{
	    StackDeInit(pSendBuffStack);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  AgentSendAssocReq
 *
 * @brief This function sends Association Request Packet to Manager
 *
 * @param pDataProtoList	:	Pointer to DataProtoList
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function returns Association Request Packet to Manager
 *****************************************************************************/
ERR_CODE AgentSendAssociationRequest(
	DataProtoList *pDataProtoList	/* [IN] Pointer to DataProtoList */
)
{
    ERR_CODE err;
    PTR_BUFFSTACK pBuffStack;
    
    /* Check if Already associated */
    PHDCOMSTATE State = Ieee11073GetState();

    /* Make Sure nothing is pending */
    Ieee11073Cleanup();  
    
    if(State != UNASSOCIATED)
        return ERROR_INVALID_REQUEST;
    pBuffStack = StackInit((intu16)(((intu16)DATAPROTOLIST_HEADER_SIZE + ieee_htons(pDataProtoList->length)) + 
		(intu16)(ASSOC_DATA_LENGTH + APDU_HEADER_SIZE)));
    do
    {
        if(ERROR_SUCCESS != (err = PushAssociationRequest(pBuffStack, ASSOC_VERSION1, pDataProtoList)))
        {
            break;
        }
        err = SendAssociationRequest(pBuffStack);
    }while(0);
    if(ERROR_SUCCESS != err)
    {
        StackDeInit(pBuffStack);
    }
    return err;    
}

/**************************************************************************//*!
 *
 * @name  ValidateReleaseResponse
 *
 * @brief This function checks Release Response received from Manager
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS	: Always
 ******************************************************************************
 * This function checks Release Response received from Manager
 *****************************************************************************/
ERR_CODE ValidateReleaseResponse(
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
    RLRE_apdu *pRlreApdu;
#if 0
    intu8 timer_index;
    
    /* Free Association Release Request Stack Memory */
    StackDeInit(Ieee11073GetBuffStack());
    Ieee11073SetBuffStack(NULL);

	/* Remove Timer Associated */
	timer_index = Ieee11073GetTimerIndex();
	PHD_Remove_Timer(&timer_index);
	Ieee11073SetTimerIndex(timer_index);    

    AssocRetryCount = RC_ASSOC;
#endif    
    (void)PopStack(pBuffStack, sizeof(Release_response_reason), &pRlreApdu, NULL);
    
    /* Send Application Callback */
    Ieee11073GetAppCallback()(IEEE11073_ASSOCIATION_RELEASED, 
        (void*)(ieee_ntohs(pRlreApdu->reason)));
    /* Free Stack Memory */    
    StackDeInit(pBuffStack);
    
    return ERROR_SUCCESS;
}

/**************************************************************************//*!
 *
 * @name  ValidateAssociationResponse
 *
 * @brief This function checks Association Response received from Manager
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function checks Association Response received from Manager. This 
 * function initiates state transition based on association response
 *****************************************************************************/
ERR_CODE ValidateAssociationResponse(PTR_BUFFSTACK pBuffStack)
{
	ERR_CODE err;
	AARE_apdu *pAareApdu = NULL;
    intu8 timer_index;
    
	/* Remove Timer Associated */
	timer_index = Ieee11073GetTimerIndex();
	(void)PHD_Remove_Timer(&timer_index);
	Ieee11073SetTimerIndex(timer_index);    
	
    /* Free Association Request Stack Memory */
    StackDeInit(Ieee11073GetBuffStack());
    Ieee11073SetBuffStack(NULL);

	/* Reset Association Retry Count */
	AssocRetryCount = RC_ASSOC;
	/* Pop Association Response Header from Stack */
	err = PopStack(pBuffStack, ASSOC_RSP_HEADER_SIZE, (void*)&pAareApdu, NULL);
	if(err != ERROR_SUCCESS)
		return err;
	switch(ieee_ntohs(pAareApdu->result))
	{
		case ACCEPTED:
			/* 
				1. Update g_ChosenDataProtocol
				2. Transition to Operating State
			*/
        	err = ValidateDataProto(pBuffStack);
        	/* Free Stack Memory */
        	StackDeInit(pBuffStack);
        	if(err != ERROR_SUCCESS)
        	{
        	    /* Send Association Release Request */
			    err = SendAssociationReleaseRequest(
					RELEASE_REQUEST_REASON_NO_MORE_CONFIG);
                return err;
        	}
		    err = AgentDimInitialize();
		    if(err != ERROR_SUCCESS)
		    {
		        err = SendAbort(ABORT_REASON_UNDEFINED);
		        return err;
		    }
		    Ieee11073SetState(OPERATING);
			Ieee11073GetAppCallback()(IEEE11073_OPERATING, NULL);
			break;
		case ACCEPTED_UNKNOWN_CONFIG:
			/*
				Transition to Configuring State
				Sending config state
			*/
        	err = ValidateDataProto(pBuffStack);
        	/* Free Stack Memory */
        	StackDeInit(pBuffStack);
        	if(err != ERROR_SUCCESS)
        	{
        	    /* Send Association Release Request */   
			    err = SendAssociationReleaseRequest(
					RELEASE_REQUEST_REASON_NO_MORE_CONFIG);
                return err;
        	}
       	    Ieee11073SetState(CONFIGURING_SENDING_CONFIG);
		    err = AgentDimInitialize();
		    if(err != ERROR_SUCCESS)
		    {
		        err = SendAbort(ABORT_REASON_UNDEFINED);
		        return err;
		    }

        err = AgentDimSendConfigEventReport(TRUE);		
			/* Call Object Access Event Report Service
			   Send configuration 
			   Send Application Call back to initiate 
			   Configuration Event Report Send */
			break;
		case REJECTED_PERMANENT:
		case REJECTED_TRANSIENT:
		case REJECTED_NO_COMMON_PROTOCOL:
		case REJECTED_NO_COMMON_PARAMETER:
		case REJECTED_UNKNOWN:
		case REJECTED_UNAUTHORIZED:
		case REJECTED_UNSUPPORTED_ASSOC_VERSION:
			/*
				Transition to UnAssociated State
				No further attempts to connect
			*/	
        	/* Free Stack Memory */
        	StackDeInit(pBuffStack);
			Ieee11073SetState(UNASSOCIATED);
			break;
	}
	return err;
}
	
/**************************************************************************//*!
 *
 * @name  ParseAssociationSendComplete
 *
 * @brief This function handles Association Send Complete Event
 *
 * @param State			:	Agent State
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS	: Always
 ******************************************************************************
 * This function handles Association Send Complete Event
 *****************************************************************************/
ERR_CODE ParseAssociationSendComplete(
	PHDCOMSTATE State,				/* [IN] Agent State */
	PTR_BUFFSTACK pBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
    ERR_CODE err = ERROR_SUCCESS;
    APDU *pApdu = GetStackTop(pBuffStack);
    intu8 timer_index;
    intu16 Choice = ieee_ntohs(pApdu->choice);
    switch(Choice)
    {
        case AARQ_CHOSEN:
        {
        
            TIMER_OBJECT AssocTimerObject;
            if(State == ASSOCIATING)
            {
                if(pBuffStack->Timeout == INVALID_TIME_COUNT)
                {
                    pBuffStack->Timeout = ASSOC_TIMEOUT;
                }
                AssocTimerObject.msCount = pBuffStack->Timeout;
                AssocTimerObject.pfnTimerCallback = PHD_Association_Timer_Callback;
                AssocTimerObject.arg = pBuffStack;
                Ieee11073SetBuffStack(pBuffStack);
    			      /* start timer */
    			      Ieee11073SetTimerIndex(PHD_Add_Timer(&AssocTimerObject));
            }
			      return ERROR_SUCCESS;
            break;
        }
        case RLRQ_CHOSEN:
        {
            TIMER_OBJECT DisAssocTimerObject; 
            if(State == DISASSOCIATING)
            {
                
                if(pBuffStack->Timeout == INVALID_TIME_COUNT)
                {
                    pBuffStack->Timeout = ASSOC_RELEASE_TIMEOUT;
                }
                DisAssocTimerObject.msCount = pBuffStack->Timeout;
                DisAssocTimerObject.pfnTimerCallback = 
                    PHD_Dissociation_Timer_Callback;
                DisAssocTimerObject.arg = pBuffStack;
    			Ieee11073SetBuffStack(pBuffStack);
    			/* start timer */
    			Ieee11073SetTimerIndex(PHD_Add_Timer(&DisAssocTimerObject));
            }
			return ERROR_SUCCESS;
            break;
        }
        case AARE_CHOSEN:
        case RLRE_CHOSEN:
        {
            /* Disable all the timers associated with IEEE11073 */
            timer_index = Ieee11073GetTimerIndex();
            (void)PHD_Remove_Timer(&timer_index);
            Ieee11073SetTimerIndex(timer_index);
            AssocRetryCount = RC_ASSOC;
#if 0
            if(Choice == RLRE_CHOSEN)
            {
                Ieee11073GetAppCallback()(IEEE11073_ASSOCIATION_RELEASED, 
				    (void*)NULL);
            }
#endif            
            break;
        }
        case ABRT_CHOSEN:
        {
            /* This event is already notified to App when we recieve 
            Release Request from Manager */
            break;
        }
    }
    StackDeInit(pBuffStack);
    return err;  
}

/*****************************************************************************
 * Local Functions
 *****************************************************************************/

/**************************************************************************//*!
 *
 * @name  SendAssociationRequest
 *
 * @brief This function sends Association Request to Manager
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function sends Association Request to Manager 
 *****************************************************************************/
static ERR_CODE SendAssociationRequest(
	PTR_BUFFSTACK pBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
    /* Association Request Timeout */
    pBuffStack->Timeout = ASSOC_TIMEOUT;
    return DispatchAssociationLayerApdu(AARQ_CHOSEN, pBuffStack);
}

/**************************************************************************//*!
 *
 * @name  PushAarqApdu
 *
 * @brief This function pushes AARQ APDU to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes AARQ APDU to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushAarqApdu(
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
	/* UnAssociated, Associating State */
	switch(Ieee11073GetState())
	{
		case UNASSOCIATED:
		case ASSOCIATING:
			return PushApdu(pBuffStack, (intu16)AARQ_CHOSEN);
			break;
		default:
			break;
	}
	return ERROR_INVALID_REQUEST;
}

/**************************************************************************//*!
 *
 * @name  PushRlrqApdu
 *
 * @brief This function pushes RLRQ APDU to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes RLRQ APDU to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushRlrqApdu(
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)	
{
	/* Sending Config, Waiting Approval, Operating */
	switch(Ieee11073GetState())
	{
		case CONFIGURING_SENDING_CONFIG:
		case CONFIGURING_WAITING_APPROVAL:
		case OPERATING:
			return PushApdu(pBuffStack, (intu16)RLRQ_CHOSEN); 
			break;
		default:
			break;
	}
	return ERROR_INVALID_REQUEST;
}

/**************************************************************************//*!
 *
 * @name  PushAbrtApdu
 *
 * @brief This function pushes ABRT APDU to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes ABRT APDU to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushAbrtApdu(
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
	/* All States other than DISCONNECTED */
	switch(Ieee11073GetState())
	{
		case DISCONNECTED:
			break;
		default:
			return PushApdu(pBuffStack, (intu16)ABRT_CHOSEN); 
			break;
	}
	return ERROR_INVALID_REQUEST;
}

/**************************************************************************//*!
 *
 * @name  PushAareApdu
 *
 * @brief This function pushes AARE APDU to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes AARE APDU to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushAareApdu(
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
	switch(Ieee11073GetState())
	{
		case UNASSOCIATED:
		case ASSOCIATING:		
			return PushApdu(pBuffStack, (intu16)AARE_CHOSEN); 
			break;
		default:
			break;
	}
	return ERROR_INVALID_REQUEST;
 }

/**************************************************************************//*!
 *
 * @name  PushRlreApdu
 *
 * @brief This function pushes RLRE APDU to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes RLRE APDU to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushRlreApdu(
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
	switch(Ieee11073GetState())
	{
		case CONFIGURING_SENDING_CONFIG:
		case CONFIGURING_WAITING_APPROVAL: 
		case OPERATING:
		case DISASSOCIATING:
			return PushApdu(pBuffStack, (intu16)RLRE_CHOSEN); 
			break;
		default:
			break;
	}
	return ERROR_INVALID_REQUEST;
}

/**************************************************************************//*!
 *
 * @name  PushApdu
 *
 * @brief This function pushes APDU to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 * @param Choice		:	APDU Type
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes APDU to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushApdu(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */
	intu16 Choice				/* [IN] APDU Type */
)
{
	intu16 AttrLen;
	return PushApduHeader(pBuffStack, Choice, 
			(intu16)GetDataSize(pBuffStack), &AttrLen);
}

/**************************************************************************//*!
 *
 * @name  PushAssociationRequest
 *
 * @brief This function pushes Association Request to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param AssocVersion		:	Association Version
 * @param pDataProtoList	:	Pointer to Data Protolist
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Association Request to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushAssociationRequest(
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to Buffer Stack */
	AssociationVersion AssocVersion,	/* [IN] Association Version */
	DataProtoList *pDataProtoList		/* [IN] Pointer to Data Protolist */
)
{
	/* 
		1. Create AssociationRequest Structure
		2. Send Association Request Structure to Service Layer
		3. Start a Timer (Timeout = TO_ASSOC)
		4. increment retry count each time. If it reaches maximum (=RC_ASSOC) , 
		    send abort request with the reason (response timeout(2))

		    (Abort_reason) AbrtRes = ABORT_REASON_RESPONSE_TIMEOUT;
		    SendAbort(AbrtRes);		
	*/
	ERR_CODE err;
	intu16 AttrLen;
	do
	{
	    if(ERROR_SUCCESS != (err = PushDataProtoList(pBuffStack, 
			pDataProtoList, &AttrLen)))
	    {
	        break;
	    }
	    if(ERROR_SUCCESS != (err = PushAssociationVersion(pBuffStack, 
			AssocVersion, &AttrLen)))
	    {
	        break;
	    }
    	if(ERROR_SUCCESS != (err = PushAarqApdu(pBuffStack)))
    	{
    	    break;   
    	}
	}while(0);
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushAssociationResponse
 *
 * @brief This function pushes Association Response to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param assoc_result		:	Association Result
 * @param pDataProto		:	Pointer to Data Proto
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Association Response to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushAssociationResponse(
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to Buffer Stack */  
	Associate_result assoc_result, 		/* [IN] Association Result */      
	DataProto *pDataProto				/* [IN] Pointer to Data Proto */
)
{
	/* 
		1. Create AssociationResponse Structure
		2. Send Association Response Structure to Service Layer
	*/
	ERR_CODE err;
	intu16 AttrLen;
	do
	{
		if(ERROR_SUCCESS != (err = PushDataProto(pBuffStack, 
			pDataProto, &AttrLen)))
		{
			break;
		}
		if(ERROR_SUCCESS != (err = PushAssociate_result(pBuffStack, 
			assoc_result, &AttrLen)))
		{
			break;
		}
    	if(ERROR_SUCCESS != (err = PushAareApdu(pBuffStack)))
    	{
    	    break;   
    	}
	}while(0);
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushAssociationReleaseRequest
 *
 * @brief This function pushes Association Release Request to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param RelReqRes			:	Release Request Reason
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Association Release Request to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushAssociationReleaseRequest(
	PTR_BUFFSTACK pBuffStack,			/* [IN] Pointer to Buffer Stack */ 
	Release_request_reason RelReqRes	/* [IN] Release Request Reason */      
)
{
	ERR_CODE err;
	intu16 AttrLen;
	do
	{
    	if(ERROR_SUCCESS != (err = PushRelease_request_reason(pBuffStack, 
			RelReqRes, &AttrLen)))
    	{
    	    break;   
    	}
    	if(ERROR_SUCCESS != (err = PushRlrqApdu(pBuffStack)))
    	{
    	    break;   
    	}
	}while(0);
	return err;
	/* 
		1. Create Association Release Request
		2. Set State to DISASSOCIATING 
		3. Send Data to Service Layer
	*/
}

/**************************************************************************//*!
 *
 * @name  PushAssociationReleaseResponse
 *
 * @brief This function pushes Association Release Response to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param RelReqRes			:	Release Response Reason
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Association Release Response to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushAssociationReleaseResponse(
	PTR_BUFFSTACK pBuffStack,				/* [IN] Pointer to Buffer Stack */ 
	Release_response_reason RelResReason	/* [IN] Release Request Reason */  
)
{
	ERR_CODE err;
	intu16 AttrLen;
	do
	{
	    if(ERROR_SUCCESS != (err = PushRelease_response_reason(pBuffStack, 
			RelResReason, &AttrLen)))
    	{
    	    break;   
    	}
    	if(ERROR_SUCCESS != (err = PushRlreApdu(pBuffStack)))
    	{
    	    break;   
    	}
	}while(0);
	return err;
	/* 
		1. create Association release response
		2. set state to UNASSOCIATED
		3. Send data to service layer
	*/
}

/**************************************************************************//*!
 *
 * @name  PushAbort
 *
 * @brief This function pushes Abort APDU to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param AbrtReqRes		:	Abort Request Reason
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Abort APDU to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushAbort(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */
	Abort_reason AbrtReqRes		/* [IN] Abort Request Reason */ 
)
{
	/* 
		1. create Abort request
		2. set state to Unassociated
		3. send data to service layer
	*/
	ERR_CODE err;
	intu16 AttrLen;
	do
	{
	    if(ERROR_SUCCESS != (err = PushAbort_reason(pBuffStack, 
			AbrtReqRes, &AttrLen)))
    	{
    	    break;   
    	}
    	if(ERROR_SUCCESS != (err = PushAbrtApdu(pBuffStack)))
    	{
    	    break;   
    	}
	}while(0);
	return err;
}

/**************************************************************************//*!
 *
 * @name  ValidateDataProto
 *
 * @brief This function checks DataProto received from Manager
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_NOTSUPPORTED			: if Failed
 ******************************************************************************
 * This function checks DataProto received from Manager
 *****************************************************************************/
static ERR_CODE ValidateDataProto(
	PTR_BUFFSTACK pBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
    DataProto *pDataProto = NULL;
    (void)PopStack(pBuffStack, 
                   (intu16)(sizeof(DataProtoId) + ANY_HEADER_SIZE), 
                   &pDataProto, 
                   NULL);
    if(ieee_ntohs(pDataProto->data_proto_id) != DATA_PROTO_ID_20601)
        return ERROR_NOTSUPPORTED;
    return ERROR_SUCCESS;
    
}

/**************************************************************************//*!
 *
 * @name  DispatchAssociationLayerApdu
 *
 * @brief This function sends APDU through TIL also maintains Agent State
 *        machine
 *
 * @param Choice			:	Apdu Type
 * @param pBuffStack		:	Pointer to Buffer Stack Apdu Type
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: If successful
 *			ERROR_UNINITIALIZED_SHIM	: If Uninitialized Shim pointer is passed
 *			ERROR_SEND_FAILED			: If Failed to send data through Shim
 ******************************************************************************
 * This function sends APDU through TIL also maintains Agent State machine
 *****************************************************************************/
static ERR_CODE DispatchAssociationLayerApdu(
	intu16 Choice,				/* [IN] Apdu Type */
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
	ERR_CODE err;
	PHDCOMSTATE OldState, NewState;
	OldState = Ieee11073GetState();
	switch(Choice)
	{
		case AARQ_CHOSEN:
			NewState = ASSOCIATING;
			break;
		case RLRQ_CHOSEN:
			NewState = DISASSOCIATING;
			break;
		case ABRT_CHOSEN:
		    AssocRetryCount = RC_ASSOC;
			NewState = UNASSOCIATED;
			break;
		case PRST_CHOSEN:
			/* Maintain Previous State */
			NewState = OldState;
			break;
		case AARE_CHOSEN:
			/* Set State to Unassociated */
			NewState = UNASSOCIATED;
			break;
		case RLRE_CHOSEN:
			/* In Case we are in Disassociating State then 
			no State Transition */
			NewState = Ieee11073GetState();
			if( NewState != DISASSOCIATING)
			{
				/* Set State to Unassociated */
				NewState = UNASSOCIATED;
			}
			break;
	}
	/* Set New State */
	Ieee11073SetState(NewState);
	err = TIL_SendAPDU(Ieee11073GetTil(), Ieee11073GetShim(), FALSE,   
		0, Ieee11073GetSendQos(), pBuffStack);
    if(err != ERROR_SUCCESS)
    {
        /* Restore Previous State */
        Ieee11073SetState(OldState);
    }
	return err;
}

/******************************************************************************
 *
 *    @name        PHD_Association_Timer_Callback
 *
 *    @brief       This function is called whenever association timeout occurs
 *
 *    @param       arg : Argument passed by Timer ISR
 *
 *    @return      None
 *
 *****************************************************************************
 * This function is called whenever association response is not received 
 * within the association timeout time (10 seconds)
 *****************************************************************************/
static void PHD_Association_Timer_Callback(
	void* arg
)
{
    
    /* Decrement association retry count */
    if(AssocRetryCount != 0)
    {                
        AssocRetryCount--;
		/* send association request */
	    (void)SendAssociationRequest((PTR_BUFFSTACK)arg);
	}
    else
    {
        /* association failed for max retry count, so trasition to unassociated
        state, and reset the retry count. Now it is upto the application to 
        start the association */
        Ieee11073SetState(UNASSOCIATED);
        AssocRetryCount = RC_ASSOC;
        /* Free Stack Memory */        
        StackDeInit(Ieee11073GetBuffStack());
        Ieee11073SetBuffStack(NULL);

        /* Send abort */
        (void)SendAbort((uint_16) ABORT_REASON_RESPONSE_TIMEOUT);

        /* send error to app layer */
        Ieee11073GetAppCallback()(IEEE11073_ASSOCIATION_RELEASED, 
            (void*)(ASSOCIATION_TIMEDOUT));
    }
}

/******************************************************************************
 *
 *    @name        PHD_Dissociation_Timer_Callback
 *
 *    @brief       This function is called whenever dissociation timeout occurs
 *
 *    @param       arg : Argument passed by Timer ISR
 *
 *    @return      None
 *
 *****************************************************************************
 * This function is called whenever the timer expires after the dissciation 
 * request is sent by the device i.e. no packet received in response to the 
 * association release request
 *****************************************************************************/    
static void PHD_Dissociation_Timer_Callback(
	void* arg
)
{
    RLRQ_apdu *pRlrqApdu = GetStackTop((PTR_BUFFSTACK)arg);

    /* Free Stack Memory */        
    StackDeInit(Ieee11073GetBuffStack());
    Ieee11073SetBuffStack(NULL);

    Ieee11073GetAppCallback()(IEEE11073_ASSOCIATION_RELEASED, 
        (void*)(ieee_ntohs(pRlrqApdu->reason)));
    /* Send abort */
    (void)SendAbort((uint_16) ABORT_REASON_RESPONSE_TIMEOUT);
}
/**************************************************************************//*!
 *
 * @name  ResetAssociationRetryCount
 *
 * @brief  This function resets Association Retry Counter
 *
 * @return 
 ******************************************************************************
 * This function resets Association Retry Counter
 *****************************************************************************/
void ResetAssociationRetryCount(void)
{
    AssocRetryCount = RC_ASSOC;  
}
