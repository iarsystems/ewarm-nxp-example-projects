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
 * @file Ieee11073Service.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains IEEE11073 Agent Library Service Layer 
 *        Implementation
 *
 *****************************************************************************/
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <stack.h>
#include <ieee11073.h>
#include <ieee11073_nom_codes.h>
#ifdef LITTLE_ENDIAN
#include <ieee11073_phd_types_kinetis.h>
#else
#include <ieee11073_phd_types.h>
#endif
#include <ieee11073_dimstruct.h>
#include <ieee11073_dimfuncproto.h>
#include <ieee11073_presentationlayer.h>
#include <ieee11073_service.h>
#include <ieee11073_DimClasses.h>
/*****************************************************************************
 * Macro's
 *****************************************************************************/


/*****************************************************************************
 * Local Variables
 *****************************************************************************/
static volatile intu8 EventRptSentFlag = FALSE;

/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/

extern void Watchdog_Reset(void);
 
extern void* GetObjectByHandle(HANDLE objHandle, intu16 *objType);

extern PEPICFGSCANNER GetEpiCfgScanHandle(HANDLE objHandle);
extern ERR_CODE EpiCfgScannerSetArgument(PTR_BUFFSTACK pBuffStack,
    PEPICFGSCANNER pEpiScanner, PTR_BUFFSTACK pRspBuffStack);
extern intu16 GetEpiCfgScannerSetArgumentSize(PTR_BUFFSTACK pReqBuffStack);

extern PPERICFGSCANNER GetPeriCfgScanHandle(HANDLE objHandle);
extern ERR_CODE PeriCfgScannerSetArgument(PTR_BUFFSTACK pBuffStack,
    PPERICFGSCANNER pPeriScanner, PTR_BUFFSTACK pRspBuffStack);
extern intu16 GetPeriCfgScannerSetArgumentSize(PTR_BUFFSTACK pReqBuffStack);

extern intu16 GetPmStoreObjectMethodSize(OID_Type ActionType, 
	PTR_BUFFSTACK pReqBuffStack, PPMSTORE pmStore);
extern ERR_CODE PMStoreGetObjectAttribute(PPMSTORE pPmStore, 
	PTR_BUFFSTACK pReqBuffStack, PTR_BUFFSTACK pRspBuffStack);
extern intu16 PMStoreGetObjectAttributeSize(PTR_BUFFSTACK pBuffStack, 
	PPMSTORE pPmStore);
extern ERR_CODE PMStoreObjectMethods(OID_Type ActionType, 
	PTR_BUFFSTACK pReqBuffStack, PTR_BUFFSTACK pRspBuffStack, 
	PMSTORE* pPmStore);

extern intu16 GetMdsObjectMethodSize(OID_Type ActionType);

extern ERR_CODE MDSGetObjectAttribute(PTR_BUFFSTACK pReqBuffStack, 
	PTR_BUFFSTACK pRspBuffStack);
extern intu16 MdsGetObjectAttributeSize(PTR_BUFFSTACK pBuffStack);

/* Service Layer Functions */
ERR_CODE ObjectSendEventReport(intu8 bconfirm, PTR_BUFFSTACK pBuffStack);

ERR_CODE ObjectAccessGetService(InvokeIDType InvokeID, 
	PTR_BUFFSTACK pReqBuffStack);

ERR_CODE ObjectAccessSetService(InvokeIDType InvokeID, 
	PTR_BUFFSTACK pBuffStack, intu8 bRsp);

ERR_CODE ObjectAccessEventReportService(InvokeIDType InvokeID, 
	PTR_BUFFSTACK pBuffStack, intu8 bRsp);

ERR_CODE ObjectAccessActionService(InvokeIDType InvokeID, 
	PTR_BUFFSTACK pBuffStack, intu8 bRsp);
/*****************************************************************************
 * Global Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  ObjectAccessGetService
 *
 * @brief  This function is called from lower Layer. 
 *         Used by Manager to retrieve values of Agent MDS and 
 *         PM-Store Objects
 *
 * @param InvokeID		:	Invoke ID
 * @param pReqBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                    Stack
 ******************************************************************************
 * This function is called from lower Layer. Used by Manager to retrieve 
 * values of Agent MDS and PM-Store Objects
 *****************************************************************************/
ERR_CODE ObjectAccessGetService(
	InvokeIDType InvokeID,			/* [IN] Invoke ID */
	PTR_BUFFSTACK pReqBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
    /* Check for No such action if Agent does not support */
    ERR_CODE err = ERROR_SUCCESS;
    HANDLE *obj_handle;
    PTR_BUFFSTACK pRspBuffStack;
    
    
    (void)PopStack(pReqBuffStack, sizeof(HANDLE), &obj_handle, NULL);
    *obj_handle = ieee_ntohs(*obj_handle);
	if(*obj_handle == MDS_HANDLE)/* MDS Handle */				
	{
			intu16 OutBuffSize = MdsGetObjectAttributeSize(pReqBuffStack);
			pRspBuffStack = StackInit(OutBuffSize);
			if(NULL == pRspBuffStack)
			{
    			/* Free Stack Memory */
    			(void)StackDeInit(pReqBuffStack);
		        /* Send Reject Result */
		        return SendRorj(RESOURCE_LIMITATION, InvokeID);
			}
			/* call MDSGetObjectAttribute in DIM 
			   pass the attribute ID list */
			err = MDSGetObjectAttribute(pReqBuffStack, pRspBuffStack);
	} 
	else
	{   
	    void *pObject;
	    intu16 objType;
      Any invalid = {0};
      
	    if(Ieee11073GetState() != OPERATING)
	    {
			 /* Free Stack Memory */
			 (void)StackDeInit(pReqBuffStack);
			 /*
    			send roer(no-such-object-instance = 1) when attempt to 
    			reference any other object except mds before operating
			 */
	        return SendErrorResult(NO_SUCH_OBJECT_INSTANCE, &invalid, 
	            InvokeID);
	    }
	    
        pObject = GetObjectByHandle(*obj_handle, &objType);
        switch(objType)
        {
            case MDC_MOC_VMO_PMSTORE:
            {
    			    intu16 OutBuffSize = PMStoreGetObjectAttributeSize(
					    pReqBuffStack, (PPMSTORE)pObject);
    			    pRspBuffStack = StackInit(OutBuffSize);
    			    if(NULL == pRspBuffStack)
    			    {
        			  /* Free Stack Memory */
        			  (void)StackDeInit(pReqBuffStack);
    		        /* Send Reject Result */
    		        return SendRorj(RESOURCE_LIMITATION, InvokeID);
    			    }
                err = PMStoreGetObjectAttribute((PPMSTORE)pObject, pReqBuffStack, pRspBuffStack);
            }
              break;
            case MDC_MOC_VMO_METRIC_NU:            
            case MDC_MOC_VMO_METRIC_SA_RT:
            case MDC_MOC_VMO_METRIC_ENUM:
            case MDC_MOC_SCAN_CFG_EPI:
            case MDC_MOC_SCAN_CFG_PERI:
             {              
    			    /*
        			  send roer (invalid object instance  = 17)	
        			  (when object exists but is not mds or pm store)
    			    */
    			    /* Free Stack Memory */
    			    (void)StackDeInit(pReqBuffStack);
    	        return SendErrorResult(INVALID_OBJECT_INSTANCE, &invalid, InvokeID);
             }    	         
             break;
               
            default:
    			  /*
        			send roer(no-such-object-instance = 1) when illegal handle
    			  */  
      			/* Free Stack Memory */
      			(void)StackDeInit(pReqBuffStack);
    	        return SendErrorResult(NO_SUCH_OBJECT_INSTANCE, &invalid, InvokeID);	        
             break;
        }
	}
	/* Free Stack Memory */
	(void)StackDeInit(pReqBuffStack);

    if(ERROR_SUCCESS == err)
    {
        /* Push MDS Attribute Header */
        /* Check for Error Condition */
        err = PushRorsGetApdu(pRspBuffStack, InvokeID);
        if(ERROR_SUCCESS != err)
        {
	        /* Free Response Stack */
	        (void)StackDeInit(pRspBuffStack);
	        /* Send Reject Result */
	        return SendRorj(RESOURCE_LIMITATION, InvokeID);
        }
    } 
    else
    {
        /* Send Reject Result */
        return SendRorj(RESOURCE_LIMITATION, 
            InvokeID);
    }
	
    err = SendPresentationApdu(pRspBuffStack);
    if(ERROR_SUCCESS != err)
    {
        /* Unable to send Data through Transport */
        StackDeInit(pRspBuffStack);
    }
	return err;
}

/**************************************************************************//*!
 *
 * @name  ObjectSendEventReport
 *
 * @brief  This function is called from upper Layer. 
 *         Used by Agent Specialization to send event report to Manager
 *
 * @param bconfirm		:	Confirmed Event Report flag
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS	: if Successful
 *			Others			: if Unsuccessful
 ******************************************************************************
 * This function is called from upper Layer. Used by Agent Specialization to 
 * send event report to Manager
 *****************************************************************************/
ERR_CODE ObjectSendEventReport(
	intu8 bconfirm,				/* [IN] Confirmed Event Report flag */
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
	ERR_CODE err;
	PHDCOMSTATE assocState = Ieee11073GetState();
  
  UNUSED(bconfirm)
    
    while(EventRptSentFlag == TRUE)
    {
        /* Wait until previous Report is sent and response is received */
       Watchdog_Reset();
    }
    
    if((assocState == OPERATING) && (Ieee11073GetState() != OPERATING))
    {
     /* The agent has just been de-associated while waiting for a previous report to be sent */
     return ERROR_SEND_FAILED;
    }
      
    EventRptSentFlag = TRUE;
    err = SendPresentationApdu(pBuffStack);
	if(ERROR_SUCCESS != err)
	{
	    EventRptSentFlag = FALSE;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  ObjectAccessSetService
 *
 * @brief  This function is called from lower Layer. 
 *         Used by Manager to set values of Agent Scanner Objects
 *
 * @param InvokeID		:	Invoke ID
 * @param pReqBuffStack	:	Pointer to Buffer Stack
 * @param bRsp			:	Response Flag
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                    Stack
 *          ERROR_UNSUPPORTED			: Action not supported by Object
 ******************************************************************************
 * This function is called from lower Layer. Used by Manager to set values of 
 * Agent Scanner Objects
 *****************************************************************************/
ERR_CODE ObjectAccessSetService(
	InvokeIDType InvokeID,			/* [IN] Invoke ID */
	PTR_BUFFSTACK pReqBuffStack,	/* [IN] Pointer to Buffer Stack */
	intu8 bRsp						/* [IN] Response Flag */
)
{
    ERR_CODE err;
    HANDLE *obj_handle;
    PTR_BUFFSTACK pRspBuffStack = NULL;
    intu16 objType;
    void *pObject;
    Any invalid = {0};
    
    (void)PopStack(pReqBuffStack, sizeof(HANDLE), &obj_handle, NULL);
    *obj_handle = ieee_ntohs(*obj_handle);

	  pObject = GetObjectByHandle(*obj_handle, &objType);

    /* Set Argument is defined only for Scanner Objects */
    if(!(
        (objType == MDC_MOC_SCAN_CFG_EPI) ||
        (objType == MDC_MOC_SCAN_CFG_PERI) ||
        (pObject != NULL)
        ))
    {
		/*
			send roer(no-such-object-instance = 17) when illegal request
		*/
		/* Free Stack Memory */
		(void)StackDeInit(pReqBuffStack);
        return SendErrorResult(INVALID_OBJECT_INSTANCE, 
            &invalid, InvokeID);	        
    }
    
    if(bRsp == TRUE)
    {
        /* Allocate Response Buffer */
        intu16 OutBuffSize = GetScannerSetArgumentSize(pReqBuffStack);

/*        
        OutBuffSize = (intu16)((objType == MDC_MOC_SCAN_CFG_EPI) ?
            GetEpiCfgScannerSetArgumentSize(pReqBuffStack)  
            :     
            GetPeriCfgScannerSetArgumentSize(pReqBuffStack)
            );
*/            
        if(OutBuffSize != 0)
        {
            OutBuffSize += GetSetResultSimpleHeaderSize();
            
            pRspBuffStack = StackInit(OutBuffSize);
            
            /* Check for Fail Condition */
            if(pRspBuffStack == NULL)
            {
            	/* Free Stack Memory */
            	(void)StackDeInit(pReqBuffStack);
    	        /* Send Reject Result */
    	        return SendRorj(RESOURCE_LIMITATION, InvokeID);
            }
        } 
    }
	
  err = ERROR_UNSUPPORTED;
	switch(objType)
	{
	    /* Set Service is defined only for Scanner Classes */
        case MDC_MOC_SCAN_CFG_EPI:
            err = EpiCfgScannerSetArgument(pReqBuffStack, pObject, pRspBuffStack);
            break;
        case MDC_MOC_SCAN_CFG_PERI:
            err = PeriCfgScannerSetArgument(pReqBuffStack, pObject, pRspBuffStack);
            break;
	}

	/* Free Stack Memory */
	(void)StackDeInit(pReqBuffStack);
    if(ERROR_SUCCESS != err)
    {
        /* Unsupported request */
        StackDeInit(pRspBuffStack);
        /* Send Reject Result */
        return SendRorj(RESOURCE_LIMITATION, InvokeID);
    }
    if(bRsp == TRUE)
    {
        intu16 AttrLen;
        err = PushSetResultSimple(pRspBuffStack, 
            *obj_handle, InvokeID, &AttrLen);
        if(ERROR_SUCCESS != err)
        {
            StackDeInit(pRspBuffStack);
	        /* Send Reject Result */
	        return SendRorj(RESOURCE_LIMITATION, InvokeID);
        }
	    err = SendPresentationApdu(pRspBuffStack);
	    if(ERROR_SUCCESS != err)
	    {
	        (void)StackDeInit(pRspBuffStack);
	    }
    }
	return err;
}

/**************************************************************************//*!
 *
 * @name  ObjectAccessEventReportService
 *
 * @brief  This function is called from lower Layer when 
 *         Confirmed/Unconfirmed EVENT Report Chosen APDU is received
 *
 * @param InvokeID		:	Invoke ID
 * @param pBuffStack	:	Pointer to Buffer Stack
 * @param bRsp			:	Response Flag
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                    Stack
 *          ERROR_UNSUPPORTED			: Action not supported by Object
 ******************************************************************************
 * This function is called from lower Layer when Confirmed/Unconfirmed 
 * EVENT Report Chosen APDU is received
 *****************************************************************************/
ERR_CODE ObjectAccessEventReportService(
	InvokeIDType InvokeID,		/* [IN] Invoke ID */               
	PTR_BUFFSTACK pBuffStack, 	/* [IN] Pointer to Buffer Stack */ 
	intu8 bRsp					/* [IN] Response Flag */           
)
{
    Any invalid = 
    {
        0
    };
    UNUSED(bRsp)
    
    /* Free Stack Memory */
	StackDeInit(pBuffStack);
	return SendErrorResult(INVALID_OBJECT_INSTANCE, &invalid, InvokeID);
}

/**************************************************************************//*!
 *
 * @name  ObjectAccessSetService
 *
 * @brief  This function is called from lower Layer.
 *         Used by Manager to invoke actions supported by agent
 *
 * @param InvokeID		:	Invoke ID
 * @param pReqBuffStack	:	Pointer to Buffer Stack
 * @param bRsp			:	Response Flag
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: if Successful
 *			ERROR_INVALID_REQUEST		: if Unsuccessful
 *			ERROR_INSUFFICIENT_MEMORY	: if Unable to push Header in Buffer 
 *                                    Stack
 *          ERROR_UNSUPPORTED			: Action not supported by Object
 ******************************************************************************
 * This function is called from lower Layer. Used by Manager to invoke 
 * actions supported by agent.
 *****************************************************************************/
ERR_CODE ObjectAccessActionService(
	InvokeIDType InvokeID,			/* [IN] Invoke ID */               
	PTR_BUFFSTACK pReqBuffStack, 	/* [IN] Pointer to Buffer Stack */ 
	intu8 bRsp						/* [IN] Response Flag */           
)
{
    ERR_CODE err;
    void *pObject;
    intu16 objType;
    intu16 OutBuffSize;
    PTR_BUFFSTACK pRspBuffStack = NULL;
	ActionArgumentSimple *pActionArgumentSimple;
    Any invalid = 
    {
        0
    };
    
    (void)PopStack(pReqBuffStack, (sizeof(HANDLE) + sizeof(OID_Type)), 
        &pActionArgumentSimple, NULL);
    pActionArgumentSimple->obj_handle = 
		ieee_ntohs(pActionArgumentSimple->obj_handle);
    pActionArgumentSimple->action_type = 
		ieee_ntohs(pActionArgumentSimple->action_type);
    
	pObject = GetObjectByHandle(pActionArgumentSimple->obj_handle, &objType);
	switch(objType)
	{
	    case MDC_MOC_VMS_MDS_SIMP:
	    {
	        if(bRsp == TRUE)
	        {
	            /* Compute Response Buffer Size and Allocate Stack */
	            OutBuffSize = 
	                GetMdsObjectMethodSize(pActionArgumentSimple->action_type);
				if(OutBuffSize != 0)
				{
					pRspBuffStack = StackInit(OutBuffSize);
					if(NULL == pRspBuffStack)
					{
						/* Free Stack Memory */
						(void)StackDeInit(pReqBuffStack);
						/* Send Reject Result */
						return SendRorj(RESOURCE_LIMITATION, 
							InvokeID);
					}
				}
	        }
	        err = MDSObjectMethods(pActionArgumentSimple->action_type, 
	            pReqBuffStack, pRspBuffStack);
	        break;
	    }

        case MDC_MOC_VMO_PMSTORE:
	      {
	        if(bRsp)
	        {
	            /* Compute Response Buffer Size and Allocate Stack */
	            OutBuffSize = GetPmStoreObjectMethodSize(
	                pActionArgumentSimple->action_type, pReqBuffStack,
	                (PPMSTORE)pObject);
	                
                pRspBuffStack = StackInit(OutBuffSize);
                if(NULL == pRspBuffStack)
                {
                	/* Free Stack Memory */
                	(void)StackDeInit(pReqBuffStack);
        	        /* Send Reject Result */
        	        return SendRorj(RESOURCE_LIMITATION, 
        	            InvokeID);
                }
	        }
            /* Handle PmStore Action Request*/
            err = PMStoreObjectMethods(pActionArgumentSimple->action_type, 
                pReqBuffStack, pRspBuffStack, (PMSTORE*)pObject);
            if(err == ERROR_OBJECT_BUSY)
            {
                Any invalid = 
                {
                    0
                };
                /* Free Stack Memory */
                StackDeInit(pReqBuffStack);
                StackDeInit(pRspBuffStack);
                return SendErrorResult(MDC_RET_CODE_OBJ_BUSY, &invalid, 
					InvokeID);
                
            }
            break;
	      }
	      
        case MDC_MOC_VMO_METRIC_NU:
        case MDC_MOC_VMO_METRIC_SA_RT:
        case MDC_MOC_VMO_METRIC_ENUM:
        case MDC_MOC_SCAN_CFG_EPI:
        case MDC_MOC_SCAN_CFG_PERI:
        default:            
            StackDeInit(pReqBuffStack);
            return SendErrorResult(INVALID_OBJECT_INSTANCE, &invalid, InvokeID);
            break;
	}
	(void)StackDeInit(pReqBuffStack);
	if(ERROR_UNSUPPORTED == err)
	{
	    Any invalid = {0, 0};
	    /* Unsupported Error */
	    return SendErrorResult(NO_SUCH_ACTION, &invalid, InvokeID);
	}
	if((bRsp == TRUE) && (ERROR_SUCCESS == err))
	{
	    intu16 AttrLen;
	    do 
	    {
	        
    	    if(ERROR_SUCCESS != (err = PushActionResultSimpleHeader(
				pRspBuffStack, pActionArgumentSimple->obj_handle,
    	        pActionArgumentSimple->action_type,
    	        &AttrLen)))
    	    {
    	        break;
    	    }
    	    if(ERROR_SUCCESS != (err = PushRorsConfirmedActionApdu(
				pRspBuffStack, InvokeID))) 
    	    {
    	        break;
    	    }
	    }while(0);
	    if(ERROR_SUCCESS != err)
	    {
	        
	        (void)StackDeInit(pRspBuffStack);
	        /* Send Reject Result */
	        return SendRorj(RESOURCE_LIMITATION, InvokeID);
	    } 
	    else
	    {
	        err = SendPresentationApdu(pRspBuffStack);
	        if(ERROR_SUCCESS != err)
	        {
	            /* Unable to Send Data through Transport */
	            StackDeInit(pRspBuffStack);
            }
	    }
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  ObjectAccessCallback
 *
 * @brief  This function is called from lower Layer.
 *         Invoked to signal that Event report is sent
 *
 * @param pReqBuffStack	:	Pointer to Buffer Stack
 *
 * @return 
 ******************************************************************************
 * This function is called from lower Layer. Invoked to signal that Event 
 * report is sent
 *****************************************************************************/
void ObjectAccessCallback(
	PTR_BUFFSTACK pReqBuffStack 	/* [IN] Pointer to Buffer Stack */ 
)
{
    UNUSED(pReqBuffStack)
    EventRptSentFlag = FALSE;  
}
