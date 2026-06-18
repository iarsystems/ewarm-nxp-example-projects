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
 * @file Ieee11073PresentationLayer.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains IEEE11073 Presentation Layer Handling
 *
 *****************************************************************************/
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <stack.h>
#include <ieee11073_nom_codes.h>
#include <ieee11073_presentationlayer.h>
#include <ieee11073_ClassCallback.h> 
#include <ieee11073_association.h>
#include <ieee11073_service.h>
#include <ieee11073_DimClasses.h>
/*****************************************************************************
 * Macro's
 *****************************************************************************/

#define PushERROR			Pushintu16
#define PushRorjProblem		Pushintu16
#define PushConfigRptId     Pushintu16

#define CONFIGURATION_TIMEOUT			10000	/* 10 sec Timeout */
#define DEFAULT_RESPONSE_TIMEOUT		3000	/* 3 sec Timeout */	


/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static void PHD_Configuration_Timer_Callback(void* arg);
static ERR_CODE PushRejectResult(PTR_BUFFSTACK pBuffStack, RorjProblem problem,
	InvokeIDType InvokeID);
static ERR_CODE PushErrorResult(PTR_BUFFSTACK pBuffStack, ERROR ErrRes, Any* pAny, 
	InvokeIDType InvokeID);
static ERR_CODE PushPresentationApdu(PTR_BUFFSTACK pBuffStack);

/*****************************************************************************
 * Local Variables
 *****************************************************************************/
static volatile InvokeIDType g_SendInvokeId = 0;

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  SendErrorResult
 *
 * @brief This function sends ROER APDU to Manager
 *
 * @param errCode	:	Error Code
 * @param pAny		:	Pointer to Error Code Info
 * @param InvokeID	:	Invoke ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: Insufficient Memory
 ******************************************************************************
 * This function sends ROER APDU to Manager
 *****************************************************************************/
ERR_CODE SendErrorResult(
	ERROR errCode,			/* [IN] Error Code */
	Any* pAny,				/* [IN] Pointer to Error Code Info */
	InvokeIDType InvokeID	/* [IN] Invoke ID */
)
{
	ERR_CODE err;
	PTR_BUFFSTACK pBuffStack;
	intu16 OutBuffSize = (intu16)(ANY_HEADER_SIZE + pAny->length + 
	                       sizeof(ERROR) + GetDataApduHeaderSize());
	pBuffStack = StackInit(OutBuffSize);
	if(pBuffStack == NULL)
	{
		return ERROR_INSUFFICIENT_MEMORY;
	}
	do
	{
	  err = PushErrorResult(pBuffStack, errCode, pAny, InvokeID);
		if(err != ERROR_SUCCESS)
		{
			break;
		}
		err = SendPresentationApdu(pBuffStack);
		if(err != ERROR_SUCCESS)
		{
			break;
		}
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		StackDeInit(pBuffStack);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  SendRorj
 *
 * @brief This function sends RORJ APDU to Manager
 *
 * @param problem	:	Reject Code
 * @param InvokeID	:	Invoke ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INSUFFICIENT_MEMORY	: Insufficient Memory
 ******************************************************************************
 * This function sends RORJ APDU to Manager
 *****************************************************************************/
ERR_CODE SendRorj(
	RorjProblem problem,	/* [IN] Reject Code */
	InvokeIDType InvokeID	/* [IN] Invoke ID */
)
{
	ERR_CODE err;
	PTR_BUFFSTACK pBuffStack;
	intu16 OutBuffSize = (intu16)(sizeof(RejectResult) + GetDataApduHeaderSize());
	pBuffStack = StackInit(OutBuffSize);
	if(pBuffStack == NULL)
	{
		return ERROR_INSUFFICIENT_MEMORY;
	}
	do
	{
	  err = PushRejectResult(pBuffStack, problem, InvokeID);
		if(err != ERROR_SUCCESS)
		{
			break;
		}
		
		err = SendPresentationApdu(pBuffStack);
		if(err != ERROR_SUCCESS)
		{
			break;
		}
	}while(0);
	if(ERROR_SUCCESS != err)
	{
		StackDeInit(pBuffStack);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushRejectResult
 *
 * @brief This function pushes RORJ Apdu to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 * @param problem		:	Reject Code
 * @param InvokeID		:	Invoke ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
******************************************************************************
 * This function pushes RORJ Apdu to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushRejectResult(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */
	RorjProblem problem ,			/* [IN] Reject Code */
	InvokeIDType InvokeID			/* [IN] Invoke ID */
)
{
	ERR_CODE err;
	intu16 DataLen;
	do
	{
		if(ERROR_SUCCESS != (err = PushRorjProblem(pBuffStack, problem, 
			&DataLen)))
		{
			break;
		}
		if(ERROR_SUCCESS != (err = PushRorjApdu(pBuffStack, InvokeID)))
		{
			break;
		}

	}while(0);
	if(ERROR_SUCCESS != err)
	{
	    StackDeInit(pBuffStack);
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushErrorResult
 *
 * @brief This function pushes Roer Apdu to Buffer Stack
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 * @param problem		:	Error Code
 * @param pAny			:	Pointer to Error Code Info
 * @param InvokeID		:	Invoke ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
******************************************************************************
 * This function pushes Roer Apdu to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushErrorResult(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */
	ERROR ErrRes,					/* [IN] Error Code */
	Any* pAny,						/* [IN] Ponter to Error Code Info */
	InvokeIDType InvokeID			/* [IN] Invoke ID */
)
{
	ERR_CODE err;
	intu16 DataLen;
	do
	{
		if(ERROR_SUCCESS != (err = PushAny(pBuffStack, pAny, &DataLen)))
		{
			break;
		}
		if(ERROR_SUCCESS != (err = PushERROR(pBuffStack, ErrRes, &DataLen)))
		{
			break;
		}
		if(ERROR_SUCCESS != (err = PushRoerApdu(pBuffStack, InvokeID)))
		{
			break;
		}
	}while(0);
	if(ERROR_SUCCESS != err)
		StackDeInit(pBuffStack);
	return err;
}

/**************************************************************************//*!
 *
 * @name  PushConfigReportHeader
 *
 * @brief This function pushes Configuration Report Header to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param ConfId			:	Configuration Report ID
 * @param ConfigObjListCnt	:	Configuration Object List Count
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
******************************************************************************
 * This function pushes Configuration Report Header to Buffer Stack
 *****************************************************************************/
ERR_CODE PushConfigReportHeader(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */        
	ConfigId ConfId, 				/* [IN] Configuration  ID */                  
    intu16 ConfigObjListCnt, 		/* [IN] Configuration Object List Count */                     
	intu16* pAttrLen				/* [OUT] Pointer to size of data pushed */
)									
{
    ERR_CODE err;
    intu16 AttrLen;
    *pAttrLen = 0;

    do
    {
        if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, ConfigObjListCnt, 
			(intu16)GetDataSize(pBuffStack), &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = PushConfigRptId(pBuffStack, ConfId, 
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
    pBuffStack->Timeout = CONFIGURATION_TIMEOUT;

    return err;
}

/**************************************************************************//*!
 *
 * @name  PushEventReportArgSimple
 *
 * @brief This function pushes Event Report Argument Simple APDU to Buffer 
 *        Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param handle			:	Object Handle
 * @param EventTime			:	Event Time
 * @param EventType			:	Event Report Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Event Report Argument Simple APDU to Buffer Stack
 *****************************************************************************/
ERR_CODE PushEventReportArgSimple(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */        
	HANDLE handle, 					/* [IN] Object Handle */                  
    RelativeTime EventTime, 		/* [IN] Event Time */                     
	OID_Type EventType, 			/* [IN] Event Report Type */              
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */
)
{
    ERR_CODE err;
    intu16 AttrLen;
    *pAttrLen = 0;
    
    do
    {        
        if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, 
			(intu16)GetDataSize(pBuffStack), &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = PushOID_Type(pBuffStack, EventType, 
			&AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = PushRelativeTime(pBuffStack, EventTime, 
			&AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = PushHANDLE(pBuffStack, handle, &AttrLen)))
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
 * @name  PushUnConfEventRpt
 *
 * @brief This function pushes UnConfirmed Event Report APDU to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param handle			:	Object Handle
 * @param EventTime			:	Event Time
 * @param EventType			:	Event Report Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes UnConfirmed Event Report APDU to Buffer Stack
 *****************************************************************************/
ERR_CODE PushUnConfEventRpt(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */        
	HANDLE handle, 					/* [IN] Object Handle */                  
    RelativeTime EventTime, 		/* [IN] Event Time */                     
	OID_Type EventType, 			/* [IN] Event Report Type */              
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */
)
{
    ERR_CODE err;
    intu16 AttrLen;
    *pAttrLen = 0;

    do
    {
        if(ERROR_SUCCESS != (err = PushEventReportArgSimple(pBuffStack, handle, 
			EventTime, EventType, &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = PushRoivEventReportApdu(pBuffStack, 
			g_SendInvokeId++)))
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
 * @name  PushConfEventRpt
 *
 * @brief This function pushes Confirmed Event Report APDU to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param handle			:	Object Handle
 * @param EventTime			:	Event Time
 * @param EventType			:	Event Report Type
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Confirmed Event Report APDU to Buffer Stack
 *****************************************************************************/
ERR_CODE PushConfEventRpt(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */
	HANDLE handle,					/* [IN] Object Handle */
    RelativeTime EventTime,			/* [IN] Event Time */
	OID_Type EventType,				/* [IN] Event Report Type */
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */
)
{
    ERR_CODE err;
    intu16 AttrLen;
    *pAttrLen = 0;

    do
    {
        if(ERROR_SUCCESS != (err = PushEventReportArgSimple(pBuffStack, handle, 
			EventTime, EventType, &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = PushRoivConfirmedEventReportApdu(pBuffStack, 
			g_SendInvokeId++)))
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
 * @name  PushSetResultSimple
 *
 * @brief This function pushes RORS Confirmed Set APDU to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param handle			:	Object Handle
 * @param InvokeId			:	Invoke ID
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes RORS Confirmed Set APDU to Buffer Stack
 *****************************************************************************/
ERR_CODE PushSetResultSimple(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */
	HANDLE handle,					/* [IN] Object Handle */
	InvokeIDType InvokeId,			/* [IN] Invoke ID */
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */
)
{
    ERR_CODE err;
    intu16 AttrLen;
    *pAttrLen = 0;

    do
    {
        if(ERROR_SUCCESS != (err = PushHANDLE(pBuffStack, handle, &AttrLen)))
        {
            break;
        }
        *pAttrLen += AttrLen;
        if(ERROR_SUCCESS != (err = PushRorsConfirmedSetApdu(pBuffStack, 
			InvokeId)))
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
 * @name  PushDataApdu
 *
 * @brief This function pushes DATA APDU to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param InvokeId			:	Invoke ID
 * @param Choice			:	Data Apdu Type
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes DATA APDU to Buffer Stack
 *****************************************************************************/
ERR_CODE PushDataApdu(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */
	InvokeIDType InvokeID,			/* [IN] Invoke ID */
	intu16 Choice					/* [IN] DATA Apdu Type */
)
{
	intu16 DataLength = 0;
	intu16 DataLen;
	ERR_CODE err;
	do
	{
		if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, 
			(intu16)GetDataSize(pBuffStack), &DataLen)))
		{
			break;
		}
		if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, Choice, &DataLen)))
		{
			break;
		}
		if(ERROR_SUCCESS != (err = Pushintu16(pBuffStack, InvokeID, &DataLen)))
		{
			break;
		}
		if(ERROR_SUCCESS != (err = PushPresentationApdu(pBuffStack)))
		{
			break;
		}
	}while(0);

	return err;
}

/**************************************************************************//*!
 *
 * @name  PushPresentationApdu
 *
 * @brief This function pushes Presentation APDU to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function pushes Presentation APDU to Buffer Stack
 *****************************************************************************/
static ERR_CODE PushPresentationApdu(
	PTR_BUFFSTACK pBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
	ERR_CODE err;
	intu16 AttrLen;

	err = Pushintu16(pBuffStack, (intu16)GetDataSize(pBuffStack), &AttrLen);
	
  if(err == ERROR_SUCCESS)
  {
   err = PushPrstApdu(pBuffStack);
  }

	return err;
}

/**************************************************************************//*!
 *
 * @name  ParsePrstApdu
 *
 * @brief This function parses Presentation Apdu Received from Manager
 *
 * @param State				:	Agent State
 * @param pBuffStack		:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS	: if Successful
 *			Others			: if Unsuccessful
 ******************************************************************************
 * This function parses Presentation Apdu Received from Manager
 *****************************************************************************/
ERR_CODE ParsePrstApdu(
	PHDCOMSTATE State,				/* [IN] Agent State */
	PTR_BUFFSTACK pBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
	ERR_CODE err;
	PRST_apdu *pPrstApdu = NULL;
	DATA_apdu *pDataApdu = NULL;
	InvokeIDType InvokeID;

	(void)PopStack(pBuffStack, PRST_APDU_HEADER_SIZE, &pPrstApdu, NULL);
	
	(void)PopStack(pBuffStack, DATA_APDU_HEADER_SIZE, &pDataApdu, NULL);
	InvokeID = ieee_ntohs(pDataApdu->invoke_id);
	switch(ieee_ntohs(pDataApdu->choice.choice))
	{
		case ROIV_CMIP_GET_CHOSEN:
			if(State != DISASSOCIATING)
			{
				err = ObjectAccessGetService(InvokeID, pBuffStack);
			} 
			else
			{
			    StackDeInit(pBuffStack);
			    err = ERROR_SUCCESS;
			}
			break;
	    /* Manager Initiated Data Transmission */	
		case ROIV_CMIP_EVENT_REPORT_CHOSEN:
		case ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN:
			if((State == CONFIGURING_SENDING_CONFIG) || 
				(State == CONFIGURING_WAITING_APPROVAL))
			{
				/* transmit roer-- no such object instance*/
				Any parameter;
				parameter.length = 0;
			    StackDeInit(pBuffStack);
				err = SendErrorResult(NO_SUCH_OBJECT_INSTANCE, &parameter, 
					InvokeID);
			}
			else if(State == OPERATING)
			{
			    err = ObjectAccessEventReportService(InvokeID, pBuffStack, 
			        (intu8)( 
    				(ieee_ntohs(pDataApdu->choice.choice) == 
    				ROIV_CMIP_EVENT_REPORT_CHOSEN) ?
    				FALSE: TRUE)); 
			}
			break;

		case ROIV_CMIP_SET_CHOSEN:
		case ROIV_CMIP_CONFIRMED_SET_CHOSEN:
			if((State == CONFIGURING_SENDING_CONFIG) || 
				(State == CONFIGURING_WAITING_APPROVAL))
			{
				/* transmit roer-- no such object instance*/
				Any parameter;
				parameter.length = 0;
			    StackDeInit(pBuffStack);
				err = SendErrorResult(NO_SUCH_OBJECT_INSTANCE, &parameter, 
					InvokeID);
			}
			else if(State == OPERATING)
			{
				err = ObjectAccessSetService(InvokeID, pBuffStack, (intu8)( 
    				(ieee_ntohs(pDataApdu->choice.choice) == ROIV_CMIP_SET_CHOSEN) ?
    				FALSE: TRUE)
    				);
			}
			break;
		
		case ROIV_CMIP_ACTION_CHOSEN:
		case ROIV_CMIP_CONFIRMED_ACTION_CHOSEN:
			if((State == CONFIGURING_SENDING_CONFIG) || 
				(State == CONFIGURING_WAITING_APPROVAL))
			{
				/* transmit roer-- no such object instance*/
				Any parameter;
				parameter.length = 0;
			    StackDeInit(pBuffStack);
				err = SendErrorResult(NO_SUCH_OBJECT_INSTANCE, &parameter,
					InvokeID);
			}
			else if(State == OPERATING)
			{
			    err = ObjectAccessActionService(InvokeID, pBuffStack, (intu8)( 
    				(ieee_ntohs(pDataApdu->choice.choice) == ROIV_CMIP_ACTION_CHOSEN)
					? FALSE: TRUE)); 
			}
			break;

		case RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN:
		{
		    EventReportResultSimple *pEvntRptRsltSmp = 
				&pDataApdu->choice.u.rors_cmipConfirmedEventReport;
		    APDU *pApdu = (APDU*)GetStackTop(Ieee11073GetBuffStack());
		    /* Check for Invoke ID */
		    if(InvokeID != 
				ieee_ntohs(((DATA_apdu*)(&pApdu->u.prst.value[0]))->invoke_id))
		    {
		        /* We recieved wrong invoke ID from Manager */
		        StackDeInit(pBuffStack);
		        return ERROR_SUCCESS;
		    } 

			if((State == CONFIGURING_SENDING_CONFIG) || 
				(State == DISASSOCIATING))
			{
				StackDeInit(pBuffStack);
				/* send abort */
				return SendAbort((intu16)ABORT_REASON_UNDEFINED);
			}
			if(State == CONFIGURING_WAITING_APPROVAL)
			{
				ConfigReportRsp *pConfigReportRsp = 
					(ConfigReportRsp *)&pEvntRptRsltSmp->event_reply_info.value;
				ConfigResult Result = ieee_ntohs(pConfigReportRsp->config_result);
			    intu8 timer_index;


                if(ieee_ntohs(pEvntRptRsltSmp->event_type)!= MDC_NOTI_CONFIG)
                {
                    /* We Recieved an Invalid Respnse */
                    StackDeInit(pBuffStack);
                    return ERROR_SUCCESS;
                }
			    /* Remove Timer */
			    timer_index = Ieee11073GetTimerIndex();
			    (void)PHD_Remove_Timer(&timer_index);
			    StackDeInit(Ieee11073GetBuffStack());
                Ieee11073SetBuffStack(NULL);
                Ieee11073SetTimerIndex(timer_index);
                ObjectAccessCallback(Ieee11073GetBuffStack());
				StackDeInit(pBuffStack);

				/*
				    if(pDataApdu->choice.u.rors_cmipConfirmedEventReport)
				    (unsupported-config), transition to sending config
				    (accepted-config) , transition to operating state */
				switch(Result)
				{
				    case ACCEPTED_CONFIG:
                    {
                        Ieee11073SetState(OPERATING);
    					Ieee11073GetAppCallback()(IEEE11073_OPERATING, NULL);
                        
                        err = ERROR_SUCCESS;
                    }
                    break;
				    case UNSUPPORTED_CONFIG:
				    {
   	                    Ieee11073SetState(CONFIGURING_SENDING_CONFIG);
				        if(ERROR_SUCCESS != 
							AgentDimSendNextConfigEventReport(FALSE))
				        {
            			    err = SendAssociationReleaseRequest(
								RELEASE_REQUEST_REASON_NO_MORE_CONFIG);
				        }
				    }
    			    break;
				    case STANDARD_CONFIG_UNKNOWN:
				    {
   	                    Ieee11073SetState(CONFIGURING_SENDING_CONFIG);
				        if(ERROR_SUCCESS != AgentDimSendConfigEventReport(TRUE))
				        {
            			    err = SendAssociationReleaseRequest(
								RELEASE_REQUEST_REASON_NO_MORE_CONFIG);
				        }
				    }
			        break;
				}
			} 
			else if(State == OPERATING)
			{
		        intu8 timer_index;
			    /* Remove Timer */
			    timer_index = Ieee11073GetTimerIndex();
			    (void)PHD_Remove_Timer(&timer_index);
                Ieee11073SetTimerIndex(timer_index);
                ObjectAccessCallback(Ieee11073GetBuffStack()); 
    		    Ieee11073GetAppCallback()(IEEE11073_EVNTRPT_SENT,
    		        Ieee11073GetBuffStack());
    		    StackDeInit(pBuffStack);
    		    StackDeInit(Ieee11073GetBuffStack());
    		    Ieee11073SetBuffStack(NULL);
		        err = ERROR_SUCCESS;
			}
			break;
		}
		case ROER_CHOSEN:
		    err = ERROR_SUCCESS;
			if(State == OPERATING)
			{
    		    /* Match previous Send with RORS and FreeStack Memory */
    		    ErrorResult *pErrResult;
    		    APDU *pApdu = (APDU*)GetStackTop(Ieee11073GetBuffStack());
    		    if(InvokeID != 
					ieee_ntohs(((DATA_apdu*)(&pApdu->u.prst.value[0]))->invoke_id))
    		    {
    		        /* We recieved wrong invoke ID from Manager */
    		        StackDeInit(pBuffStack);
    		    } 
    		    else
    		    {
    		        intu8 timer_index;

    			    /* Remove Timer */
    			    timer_index = Ieee11073GetTimerIndex();
    			    (void)PHD_Remove_Timer(&timer_index);
                    Ieee11073SetTimerIndex(timer_index);

    		        /* Clear Previous allocated buffer */
    		        StackDeInit(Ieee11073GetBuffStack());
        		    Ieee11073SetBuffStack(NULL);
        		    
        		    ObjectAccessCallback(Ieee11073GetBuffStack());  
        		    (void)PopStack(pBuffStack, ieee_ntohs(pDataApdu->choice.length), 
						&pErrResult, NULL);
					pErrResult->error_value = ieee_ntohs(pErrResult->error_value);
					pErrResult->parameter.length = 
					    ieee_ntohs(pErrResult->parameter.length);
        		    Ieee11073GetAppCallback()(IEEE11073_ERROR, pErrResult);
        		    StackDeInit(pBuffStack);
        		    
    		    }
			}
			else
			{
			    StackDeInit(pBuffStack);
				/* send abort */
				err = SendAbort((intu16)ABORT_REASON_UNDEFINED);
			}
			break;
		
		case RORJ_CHOSEN:
		    err = ERROR_SUCCESS;
			if(State == OPERATING)
			{
    		    /* Match previous Send with RORS and FreeStack Memory */
    		    RejectResult *pRejResult;
    		    APDU *pApdu = (APDU*)GetStackTop(Ieee11073GetBuffStack());
    		    if(InvokeID != 
					ieee_ntohs(((DATA_apdu*)(&pApdu->u.prst.value[0]))->invoke_id))
    		    {
    		        /* We recieved wrong invoke ID from Manager */
    		        StackDeInit(pBuffStack);
    		    } 
    		    else
    		    {
    		        intu8 timer_index;

    			    /* Remove Timer */
    			    timer_index = Ieee11073GetTimerIndex();
    			    (void)PHD_Remove_Timer(&timer_index);
                    Ieee11073SetTimerIndex(timer_index);

    		        /* Clear Previous allocated buffer */
        		    StackDeInit(Ieee11073GetBuffStack());
        		    Ieee11073SetBuffStack(NULL);

        		    ObjectAccessCallback(Ieee11073GetBuffStack());
        		    (void)PopStack(pBuffStack, ieee_ntohs(pDataApdu->choice.length), 
						&pRejResult, NULL);
					pRejResult->problem = ieee_ntohs(pRejResult->problem);
        		    Ieee11073GetAppCallback()(IEEE11073_REJECT, pRejResult);
        		    StackDeInit(pBuffStack);
    		    }
			}
			else
			{
			    StackDeInit(pBuffStack);
				/* send abort */
				err = SendAbort((intu16)ABORT_REASON_UNDEFINED);
			}
			break;
		case RORS_CMIP_GET_CHOSEN:
		case RORS_CMIP_CONFIRMED_SET_CHOSEN:
		case RORS_CMIP_CONFIRMED_ACTION_CHOSEN:
            StackDeInit(pBuffStack);
			if(State != OPERATING)
			{
				/* send abort */
				err = SendAbort((intu16)ABORT_REASON_UNDEFINED);
			}
			break;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  ParsePrstSendComplete
 *
 * @brief This function handles Presentation Apdu Send Complete
 *
 * @param State				:	Agent State
 * @param pBuffStack		:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS	: if Successful
 *			Others			: if Unsuccessful
 ******************************************************************************
 * This function handles Presentation Apdu Send Complete
 *****************************************************************************/
ERR_CODE ParsePrstSendComplete(
	PHDCOMSTATE State,				/* [IN] Agent State */
	PTR_BUFFSTACK pBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
    APDU *pApdu = (APDU*) GetStackTop(pBuffStack);
    DATA_apdu *pDataApdu = (DATA_apdu *)&pApdu->u.prst.value[0];
    intu16 choice = ieee_ntohs(pDataApdu->choice.choice);
    /* Start Timer */
	TIMER_OBJECT ConfigTimerObject;

    switch(choice)
    {
        case ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN:
        {
            if(State == CONFIGURING_SENDING_CONFIG)
            {
                Ieee11073SetState(CONFIGURING_WAITING_APPROVAL);
                
            }
            if(pBuffStack->Timeout == INVALID_TIME_COUNT)
            {
                pBuffStack->Timeout = DEFAULT_RESPONSE_TIMEOUT;
            }
			ConfigTimerObject.msCount = (intu16)pBuffStack->Timeout;
			ConfigTimerObject.pfnTimerCallback = 
				PHD_Configuration_Timer_Callback;
		    ConfigTimerObject.arg = pBuffStack;
			/* start timer */
            Ieee11073SetBuffStack(pBuffStack);
			Ieee11073SetTimerIndex(PHD_Add_Timer(&ConfigTimerObject));
            return ERROR_SUCCESS;
            break;
        }
        case ROIV_CMIP_EVENT_REPORT_CHOSEN:
            ObjectAccessCallback(pBuffStack); 
  		    Ieee11073GetAppCallback()(IEEE11073_EVNTRPT_SENT,
                pBuffStack);
            break;            
        case ROER_CHOSEN:
        case RORJ_CHOSEN:
        case RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN:
        case RORS_CMIP_GET_CHOSEN:
        case RORS_CMIP_CONFIRMED_SET_CHOSEN:
        case RORS_CMIP_CONFIRMED_ACTION_CHOSEN:
        case ROIV_CMIP_GET_CHOSEN:
        case ROIV_CMIP_SET_CHOSEN:
        case ROIV_CMIP_ACTION_CHOSEN:
        case ROIV_CMIP_CONFIRMED_SET_CHOSEN:
        case ROIV_CMIP_CONFIRMED_ACTION_CHOSEN:
            break;
    }
    /* Free Memory */
    StackDeInit(pBuffStack);
    return ERROR_SUCCESS;
}
/******************************************************************************
 *
 *    @name        PHD_Configuration_Timer_Callback
 *
 *    @brief       This function is called whenever configuration timeout 
 *                 occurs
 *
 *    @param       arg : Argument passed by Timer ISR
 *
 *    @return      None
 *
 *****************************************************************************
 * This function is called whenever no response is received by the device 
 * after sending the configuration event report within the configuration 
 * timeout time
 *****************************************************************************/
static void PHD_Configuration_Timer_Callback(
	void* arg	/* [IN] Argument passed by Timer ISR */
)
{
    ObjectAccessCallback((PTR_BUFFSTACK)arg); 
    if(NULL != arg)
    {
        StackDeInit((PTR_BUFFSTACK)arg);
        Ieee11073SetBuffStack(NULL);
    }
    if(Ieee11073GetState() == CONFIGURING_WAITING_APPROVAL)
    {
        Ieee11073GetAppCallback()(IEEE11073_CONFIGURATION_TIMEDOUT,
            NULL);    
    }
    else
    {
        Ieee11073GetAppCallback()(IEEE11073_EVENTRPT_TIMEDOUT,
            NULL);    
    }    /* Send abort */
	(void)SendAbort((intu16) ABORT_REASON_RESPONSE_TIMEOUT);
}
/**************************************************************************//*!
 *
 * @name  ResetSendInvokeId
 *
 * @brief  This function resets SendInvoke Id
 *
 * @return 
 ******************************************************************************
 * This function resets SendInvoke Id
 *****************************************************************************/
void ResetSendInvokeId(void)
{
    g_SendInvokeId = 0;  
}
