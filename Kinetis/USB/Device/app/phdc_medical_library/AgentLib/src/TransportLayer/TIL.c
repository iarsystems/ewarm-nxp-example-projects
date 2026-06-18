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
 * @file TIL.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains Transport Independent Layer Implementation
 *
 *****************************************************************************/
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <stack.h>
#include <til.h>

/*****************************************************************************
 * Local Variables
 *****************************************************************************/
/* Pointer to TIL */
static PTIL g_pTil = NULL;

/*****************************************************************************
 * Local Functions Prototypes - None
 *****************************************************************************/
static PSHIM TIL_GetShim(PTIL pTil, eShimID ShimID);
 /*****************************************************************************
 * Local Functions 
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  TIL_GetShim
 *
 * @brief This funtion returns Pointer to Shim
 *
 * @param pTil			:	Pointer to TIL
 * @param ShimID		:	SHIM ID
 *
 * @return SHIM Pointer if successful otherwise NULL
 ******************************************************************************
 * This funtion initializes SHIM
 *****************************************************************************/
static PSHIM TIL_GetShim(
	PTIL pTil,						/* [IN] Pointer to TIL */
	eShimID ShimID 			    	/* [IN] SHIM ID */
)
{
    uint_16 i;
    PSHIM pShim;
    if(pTil == NULL)
        return NULL;
    pShim = (PSHIM)(*pTil->aShim)[0];
    for(i = 0; i < pTil->ShimCount; i++)
    {
        if(ShimID == pShim[i].ShimId)
        {
            return &pShim[i];
        }
    }
    return NULL;
}
/*****************************************************************************
 * Global Variables - None
 *****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  TIL_Initialize
 *
 * @brief This funtion initializes Transport Independent Layer
 *
 * @param pTil		:	Pointer to TIL
 *
 * @return None
 ******************************************************************************
 * This funtion initializes Transport Independent Layer
 *****************************************************************************/
void TIL_Initialize(
	PTIL pTil		/* [IN] Pointer to TIL */
)
{
	g_pTil = pTil;
	return;
}

/**************************************************************************//*!
 *
 * @name  TIL_DeInitialize
 *
 * @brief This funtion de-initializes Transport Independent Layer
 *
 * @param pTil		:	Pointer to TIL
 *
 * @return None
 ******************************************************************************
 * This funtion de-initializes Transport Independent Layer
 *****************************************************************************/
void TIL_DeInitialize(void)
{
	g_pTil = NULL;
	return;
}

/**************************************************************************//*!
 *
 * @name  TIL_StartTransport
 *
 * @brief This funtion initializes SHIM
 *
 * @param pTil			:	Pointer to TIL
 * @param ShimID		:	SHIM ID
 * @param pAppCallback	:	Application Callback function
 *
 * @return SHIM Pointer if successful otherwise NULL
 ******************************************************************************
 * This funtion initializes SHIM
 *****************************************************************************/
PSHIM TIL_StartTransport(
	PTIL pTil,						/* [IN] Pointer to TIL */
	eShimID ShimID,			    	/* [IN] SHIM ID */
	APP_CALLBACK pAppCallback		/* [IN] Application Callback function */
)
{
	PSHIM pShim;
    ERR_CODE err;
	if(pTil == NULL)
		return NULL;
	
	pShim = TIL_GetShim(pTil, ShimID);
	
	if(pShim == NULL)
	{
	    return NULL; 
	}
	
	err = pShim->pfnShimInitialize(pAppCallback);
	
	if(ERROR_SUCCESS == err)
	{
	    return pShim;
	}
	return NULL;
}

/**************************************************************************//*!
 *
 * @name  TIL_StopTransport
 *
 * @brief This funtion de-initializes SHIM
 *
 * @param pTil			:	Pointer to TIL
 * @param ShimID		:	SHIM ID
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: If successful
 *			ERROR_UNINITIALIZED_SHIM	: If Uninitialized Shim pointer is passed
 ******************************************************************************
 * This funtion de-initializes SHIM
 *****************************************************************************/
ERR_CODE TIL_StopTransport(
	PTIL pTil,			        /* [IN] Pointer to TIL */ 
	eShimID ShimID 				/* [IN] SHIM ID */
)
{
	ERR_CODE err = ERROR_UNINITIALIZED_SHIM;
	PSHIM pShim;

	if(pTil == NULL)
		return ERROR_UNINITIALIZED_SHIM;
	
	pShim = TIL_GetShim(pTil, ShimID);
	
	if(pShim == NULL)
	    return ERROR_UNINITIALIZED_SHIM; 
	
	if(!pShim && pShim->pfnShimDeInitialize)
		return err;
	return pShim->pfnShimDeInitialize();
}

/**************************************************************************//*!
 *
 * @name  TIL_SendAPDU
 *
 * @brief This funtion sends Data through Shim
 *
 * @param pTil			:	Pointer to TIL
 * @param ShimID		:	SHIM ID
 * @param metadata		:	Metadata Flag
 * @param num_tfr		:	Number of Transfers
 * @param current_qos	:	Data QOS
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: If successful
 *			ERROR_UNINITIALIZED_SHIM	: If Uninitialized Shim pointer is passed
 *			ERROR_SEND_FAILED			: If Failed to send data through Shim
 ******************************************************************************
 * This funtion sends Data through Shim
 *****************************************************************************/
ERR_CODE TIL_SendAPDU(
	PTIL pTil,					/* [IN] Pointer to TIL */ 
	eShimID ShimID,				/* [IN] SHIM ID */
	boolean metadata,			/* [IN] Metadata Flag */
	uint_8 num_tfr,				/* [IN] Number of Transfers */
	uint_8 current_qos,			/* [IN] Data QOS */
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
	PSHIM pShim;

	if(pTil == NULL)
		return ERROR_UNINITIALIZED_SHIM;
	
	pShim = TIL_GetShim(pTil, ShimID);
	
	if(pShim == NULL)
	    return ERROR_UNINITIALIZED_SHIM; 
	
	if(!pShim && pShim->pfnShimSendData)
		return ERROR_UNINITIALIZED_SHIM;
	return pShim->pfnShimSendData(metadata, num_tfr, current_qos, pBuffStack);
}

/**************************************************************************//*!
 *
 * @name  TIL_RecvAPDU
 *
 * @brief This funtion recevies Data from Shim
 *
 * @param pTil			:	Pointer to TIL
 * @param ShimID		:	SHIM ID
 * @param current_qos	:	Data QOS
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: If successful
 *			ERROR_UNINITIALIZED_SHIM	: If Uninitialized Shim pointer is passed
 *			ERROR_RECV_FAILED			: If failed to receive data from shim
 ******************************************************************************
 * This funtion recevies Data from Shim
 *****************************************************************************/
ERR_CODE TIL_RecvAPDU(
	PTIL pTil,					/* [IN] Pointer to TIL */ 
	eShimID ShimID,				/* [IN] SHIM ID */
	uint_8 current_qos,			/* [IN] Data QOS */               
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
	ERR_CODE err = ERROR_UNINITIALIZED_SHIM;
	PSHIM pShim;

	if(pTil == NULL)
		return ERROR_UNINITIALIZED_SHIM;
	
	pShim = TIL_GetShim(pTil, ShimID);
	
	if(pShim == NULL)
	    return ERROR_UNINITIALIZED_SHIM; 
	
	if(!pShim && pShim->pfnShimRecvData)
		return err;
	return pShim->pfnShimRecvData(current_qos, pBuffStack);
}