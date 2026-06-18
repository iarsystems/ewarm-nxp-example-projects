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
 * @file Ieee11073dataproto.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains IEEE11073 Data Proto Handling
 *
 *****************************************************************************/
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
#include <ieee11073_dimstruct.h>
#include <ieee11073_dimfuncproto.h>
#include <ieee11073_dataproto.h>

/*****************************************************************************
 * Macro's
 *****************************************************************************/
#define PushDataProtoId				     Pushintu16


/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static ERR_CODE PushDataProtoEntryList(PTR_BUFFSTACK pBuffStack, intu16 AttrCount, 
	DataProto *pDataProto, intu16 *pAttrLen);
/*****************************************************************************
 * Global Functions
 *****************************************************************************/

/**************************************************************************//*!
 *
 * @name  PushDataProtoList
 *
 * @brief This function pushes DataProtoList to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pDataProtoList	:	Pointer to DataProtoList
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return Data Proto Size
 ******************************************************************************
 * This function pushes DataProtoList to Buffer Stack
 *****************************************************************************/
ERR_CODE PushDataProtoList(
	PTR_BUFFSTACK pBuffStack,		/* [IN] Pointer to Buffer Stack */
	DataProtoList* pDataProtoList,	/* [IN] Pointer to DataProtoList */
	intu16 *pAttrLen				/* [OUT] Pointer to size of data pushed */
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		if(ERROR_SUCCESS != (err = PushDataProtoEntryList(pBuffStack, 
			ieee_htons(pDataProtoList->count), (DataProto *)&pDataProtoList->value, 
			&AttrLen)))
		{
			break;
		}
		
		*pAttrLen += AttrLen;
		if(ERROR_SUCCESS != (err = PushAttribute(pBuffStack, 
				ieee_htons(pDataProtoList->count), ieee_htons(pDataProtoList->length), &AttrLen)))
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
 * @name  PushDataProtoList
 *
 * @brief This function pushes DataProto to Buffer Stack
 *
 * @param pBuffStack		:	Pointer to Buffer Stack
 * @param pDataProto		:	Pointer to DataProto
 * @param pAttrLen			:	Pointer to size of data pushed
 *
 * @return Data Proto Size
 ******************************************************************************
 * This function pushes DataProto to Buffer Stack
 *****************************************************************************/
ERR_CODE PushDataProto(
	PTR_BUFFSTACK pBuffStack,	/* [IN] Pointer to Buffer Stack */        
	DataProto* pDataProto, 		/* [IN] Pointer to DataProto */       
	intu16 *pAttrLen			/* [OUT] Pointer to size of data pushed */ 
)
{
	ERR_CODE err;
	intu16 AttrLen;
	*pAttrLen = 0;
	do
	{
		pDataProto->data_proto_info.length = ieee_htons(pDataProto->data_proto_info.length);
		
		if(ERROR_SUCCESS != (err = PushAny(pBuffStack, 
			&pDataProto->data_proto_info, &AttrLen)))
		{
			break;
		}
		*pAttrLen += AttrLen;
		pDataProto->data_proto_id = ieee_htons(pDataProto->data_proto_id);
		if(ERROR_SUCCESS != (err = PushDataProtoId(pBuffStack, 
			pDataProto->data_proto_id, &AttrLen)))
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

/*****************************************************************************
 * Local Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  PushDataProtoEntryList
 *
 * @brief This function pushes DataProto to Buffer Stack recursively
 *
 * @param pDataProtoList	:	Pointer to DataProtoList
 * @param pAttrLen			:	Pointer to sizeof data pushed
 *
 * @return Data Proto Size
 ******************************************************************************
 * This function pushes DataProto to Buffer Stack recursively
 *****************************************************************************/
static ERR_CODE PushDataProtoEntryList(
	PTR_BUFFSTACK pBuffStack, 
	intu16 AttrCount, 
	DataProto *pDataProto, 
	intu16 *pAttrLen
)
{
	ERR_CODE err;
	*pAttrLen = 0;
	if(1 != AttrCount)
	{
		intu16 AttrLen = 0;
		DataProto *temp = pDataProto;
		(uint_8_ptr)temp += sizeof(DataProtoId) + ANY_HEADER_SIZE + 
			pDataProto->data_proto_info.length;
		if(ERROR_SUCCESS == (err = PushDataProtoEntryList(pBuffStack, 
			(intu16)(AttrCount - 1), temp, &AttrLen)))
		{
			*pAttrLen += AttrLen;
			if(ERROR_SUCCESS == (err = PushDataProto(pBuffStack, 
				pDataProto, &AttrLen)))
			{
				*pAttrLen += AttrLen;
			}
			else
			{
				(void)PopStack(pBuffStack, *pAttrLen, NULL, NULL);
				*pAttrLen = 0;
			}
		} 
		else
		{
			(void)PopStack(pBuffStack, AttrLen, NULL, NULL);
		}
		return err;
	}
	return PushDataProto(pBuffStack, pDataProto, pAttrLen);
}
