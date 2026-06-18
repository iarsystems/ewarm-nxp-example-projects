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
 * @file Ieee11073ClassCallback.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains IEEE11073 Agent Library Callback Handling
 *
 *****************************************************************************/
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <ieee11073.h>
#ifdef LITTLE_ENDIAN
#include <ieee11073_phd_types_kinetis.h>
#else
#include <ieee11073_phd_types.h>
#endif
#include <RealTimerCounter.h>
#include <stack.h>
#include <til.h>
#include <ieee11073_presentationlayer.h>
#include <ieee11073_ClassCallback.h> 
#include <ieee11073_association.h>
#include <ieee11073_DimClasses.h>
/*****************************************************************************
 * Macro's
 *****************************************************************************/
#define APDU_HEADER_SIZE				(4)
#define RLRQ_DATA_LENGTH				(2)
#define PHD_MAX_RX_SIZE					(8192)
/*****************************************************************************
 * Local Variables
 *****************************************************************************/
static volatile PTR_BUFFSTACK pAllocBuffStack = NULL;
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static ERR_CODE ParseAPDU(PHDCOMSTATE State, PTR_BUFFSTACK pBuffStack);
static ERR_CODE ParseApduSendComplete(PHDCOMSTATE State, 
	PTR_BUFFSTACK pBuffStack);

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  Ieee11073PHDClassCallback
 *
 * @brief This callback function is called from SHIM
 *
 * @param eEventId	:	Transport Event
 * @param pArg		:	Pointer to Event Specific Data
 *
 * @return Pointer to Buffer Stack for TRANSPORT_GETDATABUFFER Event otherwise
 *         NULL
 ******************************************************************************
 * This callback function is called from SHIM
 *****************************************************************************/
void* Ieee11073PHDClassCallback(
	TRANSPORTEVENTID eEventId,	/* [IN] Transport Event */
	void* pArg					/* [IN/OUT] Pointer to Event Specific Data */
)
{
	ERR_CODE err = ERROR_SUCCESS;
	PHDCOMSTATE State = Ieee11073GetState();
	switch(eEventId)
	{
		case TRANSPORT_CONNECT:
			{
				/* This is required to clean up */
				if(pAllocBuffStack)
				{
					    /* Previously allocated memory to receive Data */
					    StackDeInit(pAllocBuffStack);
					    pAllocBuffStack = NULL;
				}
                    
        /* Clean-up */
        Ieee11073Cleanup();				 
				
				Ieee11073SetState(UNASSOCIATED);
				/* Initiate Association Procedure */
				(Ieee11073GetAppCallback())(IEEE11073_TRANSPORT_CONNECT, NULL);

			}
			break;
		case TRANSPORT_DISCONNECT:
			{
				if(State != DISCONNECTED)
				{
					Ieee11073SetState(DISCONNECTED);
					/* This is required to clean up */
					if(pAllocBuffStack)
					{
					    /* Previously allocated memory to receive Data */
					    StackDeInit(pAllocBuffStack);
					    pAllocBuffStack = NULL;
					}
                    
            /* Clean-up */
              Ieee11073Cleanup();
				    /* Initiate clean-up */
              (Ieee11073GetAppCallback())(IEEE11073_TRANSPORT_DISCONNECT, NULL);
        }
			}
			break;
	    case TRANSPORT_GET_XFER_SIZE:
	    {
	        PTR_TIL_XFER_SIZE pxfer_size = (PTR_TIL_XFER_SIZE)pArg;
			APDU *pApdu = (APDU*)pxfer_size->in_buff; 
			pxfer_size->transfer_size = (intu16)(ieee_ntohs(pApdu->length) + 
			    APDU_HEADER_SIZE);
	        break;
	    }
		case TRANSPORT_GETDATABUFFER:
        {
    		/* Initialize STACK based on Size and then pass PTR_BUFFSTACK to 
			lower layer */
			intu16 size = *(intu16*)pArg;
            pAllocBuffStack = StackInit(size);
            return pAllocBuffStack;
        }
			break;
		case TRANSPORT_DATARECIEVED:
			{			
				/* APDU Recieved */ 
				pAllocBuffStack = NULL;
				(void)ParseAPDU(State, (PTR_BUFFSTACK)pArg);
				return NULL;
			}
			break;
		case TRANSPORT_DATASENDCOMPLETE:
			{
			    (void)ParseApduSendComplete(State, (PTR_BUFFSTACK)pArg);
			    return NULL;
			}
			break;
		default:
			err = ERROR_UNSOLICITED;
			break;
		}
	return NULL;
}
/**************************************************************************//*!
 *
 * @name  Ieee11073Cleanup
 *
 * @brief This function cleans up Ieee11073 Buffers and Timers
 *
 * @param None
 *
 * @return None
 ******************************************************************************
 * This function cleans up Ieee11073 Buffers and Timers
 *****************************************************************************/
void Ieee11073Cleanup(void)
{
	intu8 timer_index;
	
    /* Remove Active Timers */
    timer_index = Ieee11073GetTimerIndex();
    (void)PHD_Remove_Timer(&timer_index);
    Ieee11073SetTimerIndex(timer_index);
	
    /* Cleanup previously allocated memory */
	StackDeInit(Ieee11073GetBuffStack());
	Ieee11073SetBuffStack(NULL);
    
    ResetAssociationRetryCount();
	ResetSendInvokeId();
	ObjectAccessCallback(NULL);
	AgentDimDeInitialize();
}
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  ParseAPDU
 *
 * @brief This function parses APDU received from Manager
 *
 * @param State			:	Agent State
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                        Stack
 ******************************************************************************
 * This function parses APDU received from Manager
 *****************************************************************************/
static ERR_CODE ParseAPDU(
	PHDCOMSTATE State,			/* [IN] Agent State */
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
	APDU *pApdu = NULL;
	intu16 Apdu_Choice;
	ERR_CODE err = ERROR_INVALID_REQUEST;
	intu16 BuffSize = (intu16)GetDataSize(pBuffStack);
#if 0		
	/* RKG: to be moved to GETDATABUFFER Case. 
		Consume entire packet and then send ROER */
	if(BuffSize > PHD_MAX_RX_SIZE) 
	{
		Any invalid;
		invalid.length = 0;
		/* agent can rx max of 8192 (0x2000) bytes of APDU 
			else protocol violation */
		/* send roer with error code (protocol violation = 23) */
		StackDeInit(pBuffStack);
		err = SendErrorResult(PROTOCOL_VIOLATION, &invalid, 0);
		g_Ieee11073_PHD_com_state = UNASSOCIATED;
		return err;
	}
#endif
	if(State == DISCONNECTED)
	{
		/* error */
		return ERROR_SUCCESS;;
	}

	(void)PopStack(pBuffStack, APDU_HEADER_SIZE, &pApdu, NULL);
	if(BuffSize != (ieee_ntohs(pApdu->length) + APDU_HEADER_SIZE))
	{
	    StackDeInit(pBuffStack);
		/* send Reject Result */
		err = SendRorj(BADLY_STRUCTURED_APDU, 0);
		return err;
	}
	Apdu_Choice = ieee_ntohs(pApdu->choice);
	switch(Apdu_Choice)
	{
		case AARQ_CHOSEN:
			{
			    /* Clean Up */
				Ieee11073Cleanup();
				/* We are done with pBuffStack */
				StackDeInit(pBuffStack);
				
				if((State == UNASSOCIATED) || 
					(State == ASSOCIATING))
				{
					/* agent-agent association */
					/* send aare apdu (rejected permanent) */
					/* Associate_result = REJECTED_PERMANENT = 1 */
					
					DataProto sDataProto;
					sDataProto.data_proto_id = 0;
					sDataProto.data_proto_info.length = 0;
					err = SendAssociationResponse(REJECTED_PERMANENT, 
						&sDataProto);
				}
				else
				{
					err = SendAbort(ABORT_REASON_UNDEFINED);
				}
			}
			break;
		case AARE_CHOSEN:
			{
				if(State == ASSOCIATING)
				{
					err = ValidateAssociationResponse(pBuffStack);
				}
				else
				{
					(void)SendAbort(ABORT_REASON_UNDEFINED);
				}
			}
			break;
		case RLRQ_CHOSEN:
			{
				Release_request_reason RelReqReason;

				(void)PopStack(pBuffStack, RLRQ_DATA_LENGTH, &RelReqReason, NULL);
				StackDeInit(pBuffStack);
				
				RelReqReason = ieee_ntohs(RelReqReason);

                Ieee11073Cleanup();

		    	if((State == UNASSOCIATED) || 
					(State == ASSOCIATING))
				{
					/* send abort and transition to unassociated state */
					err = SendAbort(ABORT_REASON_UNDEFINED);
				}
				else
				{
					/* send assoc release response */
					/* transition to unassociated state */
					err = SendAssociationReleaseResponse(
					    RELEASE_RESPONSE_REASON_NORMAL);
				}
				Ieee11073GetAppCallback()(IEEE11073_ASSOCIATION_RELEASED, 
				    (void*)(RelReqReason));
			}
			break;
		case RLRE_CHOSEN:
			{
				if(State == DISASSOCIATING)
				{
					(void)ValidateReleaseResponse(pBuffStack);
					Ieee11073SetState(UNASSOCIATED);
				}
				else
				{
					err = SendAbort(ABORT_REASON_UNDEFINED);
				}
			}
			break;
		case ABRT_CHOSEN:
		{
		    ABRT_apdu *pAbrtApdu;
		    (void)PopStack(pBuffStack, sizeof(Abort_reason), &pAbrtApdu, NULL);    
		    Ieee11073GetAppCallback()(IEEE11073_ABORT, 
				(void*)(ieee_ntohs(pAbrtApdu->reason)));
			/* Free Stack */
			ObjectAccessCallback(NULL);
			StackDeInit(pBuffStack);
			Ieee11073SetState(UNASSOCIATED);
			break;
	    }
		case PRST_CHOSEN:
			if((State == UNASSOCIATED) || 
				(State == ASSOCIATING))
			{
				StackDeInit(pBuffStack);
				/* send abort request */
				err = SendAbort((uint_16)ABORT_REASON_UNDEFINED);
			}
			else
			{
				err = ParsePrstApdu(State, pBuffStack);
			}
			break;
		default:
			{
				StackDeInit(pBuffStack);
				/* 
					Rorj is presentation APDU
					Dispatch Routine does not initiate State 
					Change for Presentation APDU's
				*/
				Ieee11073SetState(UNASSOCIATED);
				/* send Reject Result */
				err = SendRorj(UNRECOGNIZED_APDU, 0);
			}
			break;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  ParseApduSendComplete
 *
 * @brief This function handles send complete events received from SHIM
 *
 * @param State			:	Agent State
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS				: if Successful
 *			ERROR_INVALID_REQUEST	: if Unsuccessful
 ******************************************************************************
 * This function parses APDU received from Manager
 *****************************************************************************/
static ERR_CODE ParseApduSendComplete(
	PHDCOMSTATE State,			/* [IN] Agent State */               
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */   
)
{
    ERR_CODE err;
    APDU *pApdu;
    
    /* Check for Invalid Argument */
    if(pBuffStack == NULL)
        return ERROR_INVALID_PARAM;
    
    pApdu = (APDU*)GetStackTop(pBuffStack);
    switch(ieee_ntohs(pApdu->choice))
    {
        case AARQ_CHOSEN:
        case AARE_CHOSEN:
        case RLRQ_CHOSEN:
        case RLRE_CHOSEN:
        case ABRT_CHOSEN:
            err = ParseAssociationSendComplete(State, pBuffStack);
            break;
        case PRST_CHOSEN:
            err = ParsePrstSendComplete(State, pBuffStack);
            break;
        default:
            StackDeInit(pBuffStack);
            err = ERROR_SUCCESS;
            break;
    }
    return err;       
}

/******************************************************************************
 *
 *    @name        PHD_Remove_Timer
 *
 *    @brief       This funtion calls the Timer api to remove the timer from 
 *                 the queue
 *
 *    @param       pindex : Pointer to Timer Index
 *
 *    @return      ERR_SUCCESS          : Timer removed successfully
 *                 ERR_INVALID_PARAM    : Inavlid timer index
 *
 *****************************************************************************
 * This function when called removes the timer specified by the timer index
 *****************************************************************************/
uint_8 PHD_Remove_Timer(uint_8_ptr pindex)
{
#if MAX_TIMER_OBJECTS
    uint_8 err = (uint_8)ERR_INVALID_PARAM;
    if(*pindex != (uint_8)INVALID_TIME_COUNT)
    {
        (void)RemoveTimerQ(*pindex);
        *pindex = (intu8)INVALID_TIME_COUNT;
        err = ERROR_SUCCESS;
    }
    return err;
#else
    UNUSED(pindex)
	return ERROR_SUCCESS;
#endif	
}
        
/******************************************************************************
 *
 *    @name        PHD_Add_Timer
 *
 *    @brief       This funtion calls the Timer api to add a timer to the queue
 *
 *    @param       pTimerObject : Pointer to Timer Object
 *
 *    @return      ERR_SUCCESS  : Timer added successfully
 *                 Others       : In case of error
 *
 *****************************************************************************
 * This function when called starts a timer 
 *****************************************************************************/
uint_8 PHD_Add_Timer(PTIMER_OBJECT pTimerObject)
{
	#if MAX_TIMER_OBJECTS    
		return AddTimerQ(pTimerObject);
	#else
	    UNUSED(pTimerObject)
		return ERR_TIMER_QUEUE_FULL;	
	#endif	
}
