/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2009 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************
 *
 * THIS SOFTWARE IS PROV+IDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
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
 * @file SerialShimAgent.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains Serial Shim Implementation
 *
 *****************************************************************************/
/******************************************************************************
 * Includes
 *****************************************************************************/
#include "user_config.h"
#include "error.h"
#include "types.h"
#include "stack.h"
#include "til.h"
#include "serial_class.h"
#include "string.h"

static void * memcpy1 ( void * dest, const void * src, uint_32 num )
{
  uint_32 i;
  uint_8_ptr p_dest = (uint_8_ptr )dest;
  uint_8_ptr p_src = (uint_8_ptr )src;

  for (i = 0; i < num; i++) {
    *p_dest++ =  *p_src++;
  }

}

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define SERIAL_CONTROLLER_ID		(0)
#define MIN(a,b)    (((a) > (b)) ? (b) : (a))
#define MAX_SERIAL_XMIT_QUEUE_ELEMENTS  (4)
/*****************************************************************************
 * Local Variables
 *****************************************************************************/
/* Stores Send and receive Buffer Stack Pointers */
static PTR_BUFFSTACK pSerialShimRecvBuffer;
static APP_CALLBACK pfnAppCallback = NULL;

typedef struct _serial_shim_xmit_queue
{
    uint_8 producer;
    uint_8 consumer;
    PTR_BUFFSTACK   serial_buffer[MAX_SERIAL_XMIT_QUEUE_ELEMENTS];
}SERIAL_SHIM_XMIT_QUEUE, *PTR_SERIAL_SHIM_XMIT_QUEUE;

static SERIAL_SHIM_XMIT_QUEUE gSerialQueue;
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static ERR_CODE SerialShimInitialize(APP_CALLBACK pAppCallback);
static ERR_CODE SerialShimDeInitilize();
static ERR_CODE SerialShimSendData(
    boolean          meta_data,
    uint_8           num_tfr,
    uint_8           current_qos,
    PTR_BUFFSTACK    pBuffStack
	);
static ERR_CODE SerialShimRecvData(
    uint_8           current_qos,
    PTR_BUFFSTACK    pBuffStack
	);
static void SerialShimCallback(
    uint_8 controller_ID,  /* [IN] Controller ID */
    uint_8 event_type,     /* [IN] type of the event */
    void* val              /* [IN] Pointer to Received Buffer */
);


/*****************************************************************************
 * Global Variables
 *****************************************************************************/
/* Serial Shim Structure */
const SHIM SERIALShim =
{
	SHIM_SERIAL,
	SerialShimInitialize,
	SerialShimDeInitilize,
	SerialShimSendData,
	SerialShimRecvData
};
/*****************************************************************************
 * Local Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  SerialShimInitialize
 *
 * @brief This funtion initializes SERIAL Shim
 *
 * @param pAppCallback	:	Application Callback function Pointer
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: If Successful
 *			ERROR_SHIM_INIT_FAILED	: If Failed
 ******************************************************************************
 * This funtion initializes SERIAL Shim
 *****************************************************************************/
static ERR_CODE SerialShimInitialize(
	APP_CALLBACK pAppCallback  /* [IN] Application Callback function Pointer */
)
{
	uint_8 status;
	ERR_CODE err = ERROR_SUCCESS;
    SERIAL_INIT serial_init =
    {
        SERIAL_CONTROLLER_ID,
        FALSE,
        0,
        1,
        8,
        (uint_16)57600,
        MAX_SERIAL_RECV_BUFFER
    };
	if(NULL == pAppCallback)
	{
	    return ERROR_SHIM_INIT_FAILED;
	}
	/* Serial Class Initialization */
	pfnAppCallback = pAppCallback;
    status = SCI_Class_Init(&serial_init, SerialShimCallback);
    if(status != SERIAL_CLASS_OK)
    {
        return ERROR_SHIM_INIT_FAILED;
    }

	return err;
}

/**************************************************************************//*!
 *
 * @name  SerialShimDeInitilize
 *
 * @brief This funtion de-initializes SERIAL Shim
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: Always
 ******************************************************************************
 * This funtion de-initializes SERIAL Shim
 *****************************************************************************/
static ERR_CODE SerialShimDeInitilize(void)
{
	pfnAppCallback = NULL;
	return ERROR_SUCCESS;
}

/**************************************************************************//*!
 *
 * @name  SerialShimSendData
 *
 * @brief This funtion sends Data Buffer through SERIAL Class Driver
 *
 * @param meta_data		:	Metadata Flag
 * @param num_tfr		:	Number of Transfers
 * @param current_qos	:	Data QOS
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS		: If Successful
 *          ERROR_SEND_FAILED		: If Failed
 ******************************************************************************
 * This funtion sends Data Buffer through SERIAL Class Driver
 *****************************************************************************/
static ERR_CODE SerialShimSendData(
    boolean          meta_data,		/* [IN] Metadata Flag */
    uint_8           num_tfr,		/* [IN] Number of Transfers */
    uint_8           current_qos,	/* [IN] Data QOS */
    PTR_BUFFSTACK    pBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
	uint_8 status;
	ERR_CODE err = ERROR_SUCCESS;
    UNUSED(current_qos)
	UNUSED(meta_data)
	UNUSED(num_tfr)
	if(gSerialQueue.producer - gSerialQueue.consumer <
	    MAX_SERIAL_XMIT_QUEUE_ELEMENTS)
	{
	    gSerialQueue.serial_buffer[gSerialQueue.producer %
	        MAX_SERIAL_XMIT_QUEUE_ELEMENTS] =
	        pBuffStack;
	    gSerialQueue.producer++;
	}
	else
	{
	    return ERROR_SEND_FAILED;
	}
	/* Class Send Data */
	status = SCI_Class_Write(SERIAL_CONTROLLER_ID,
	    (uint_8_ptr)pBuffStack->top,
		(uint_16)pBuffStack->current_size);
	
	if(status != SERIAL_CLASS_OK)
	{
		err = ERROR_SEND_FAILED;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  SerialShimRecvData
 *
 * @brief This funtion receives Data Buffer from SERIAL Class Driver
 *
 * @param current_qos	:	Data QOS
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS		: If Successful
 *          ERROR_RECV_FAILED		: If Failed
 ******************************************************************************
 * This funtion receives Data Buffer from SERIAL Class Driver
 *****************************************************************************/
static ERR_CODE SerialShimRecvData(
    uint_8           current_qos,	/* [IN] Data QOS */
    PTR_BUFFSTACK    pBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
	uint_8 status;
	ERR_CODE err = ERROR_SUCCESS;
	uint_8_ptr ptemp = (uint_8_ptr)pBuffStack->top;
	uint_8_ptr pStart = (uint_8_ptr)&ptemp[pBuffStack->current_size];
	UNUSED (current_qos)
	
	/* Class Recv Data */
    status = SCI_Class_Read(SERIAL_CONTROLLER_ID, pStart,
        (uint_16)(pBuffStack->total_size - pBuffStack->current_size));

	if(status != SERIAL_CLASS_OK)
	{
		err = ERROR_RECV_FAILED;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  SerialShimCallback
 *
 * @brief This funtion receives SERIAL Class Driver Events
 *
 * @param controller_ID	:	Controller ID
 * @param event_type	:	type of the event
 * @param val			:	Pointer to Received Buffer
 *
 * @return None
 ******************************************************************************
 * This funtion receives SERIAL Class Driver Events
 *****************************************************************************/
static void SerialShimCallback(
    uint_8 controller_ID,  /* [IN] Controller ID */
    uint_8 event_type,     /* [IN] type of the event */
    void* val              /* [IN] Pointer to Received Buffer */
)
{
	switch(event_type)
	{
	    case SERIAL_APP_ENUM_COMPLETE:
        {
        	/* Inform Application that Shim is connected */
        	pfnAppCallback(TRANSPORT_CONNECT, NULL);
        }
    	break;

	    case SERIAL_APP_GET_TRANSFER_SIZE:
	    {
            PTR_SERIAL_CLASS_XFER_SIZE pxfer_size =
				(PTR_SERIAL_CLASS_XFER_SIZE)val;
		    pfnAppCallback(TRANSPORT_GET_XFER_SIZE,
		        (PTR_TIL_XFER_SIZE)&pxfer_size->in_buff);
	    }
    	break;

		case SERIAL_APP_GET_DATA_BUFF:
		{
			/* called by lower layer to get recv buffer */
			PTR_SERIAL_CLASS_RX_BUFF rx_buff =
				(PTR_SERIAL_CLASS_RX_BUFF)val;
		    if(pSerialShimRecvBuffer == NULL)
		    {
	            pSerialShimRecvBuffer = (PTR_BUFFSTACK)pfnAppCallback(
				    TRANSPORT_GETDATABUFFER, (void*)&rx_buff->transfer_size);

		        if(pSerialShimRecvBuffer == NULL)
				{
				    /* Unable to allocate BUFFSTACK */
				    SCI_Class_Read(controller_ID, NULL, 0);
				    return;
				}

                pSerialShimRecvBuffer->top =
                    (uint_8_ptr)pSerialShimRecvBuffer->top -
                    (uint_16)pSerialShimRecvBuffer->total_size;

    			pSerialShimRecvBuffer->current_size = rx_buff->in_size;	
                /* Copy Buffer */
			    (void)memcpy(pSerialShimRecvBuffer->top, rx_buff->in_buff,
                    MIN(pSerialShimRecvBuffer->total_size, rx_buff->in_size));
		    }
		    else
		    {
		        pSerialShimRecvBuffer->current_size += rx_buff->in_size;
		    }
			SCI_Class_Read(controller_ID,
			    &((uint_8_ptr)pSerialShimRecvBuffer->top)
			    [pSerialShimRecvBuffer->current_size],
			    (uint_16)(GetStackSize(pSerialShimRecvBuffer) -
			    GetDataSize(pSerialShimRecvBuffer)));
		}
    	break;

		case SERIAL_APP_DATA_RECEIVED:
		{
			PTR_SERIAL_APP_EVENT_DATA_RECEIVED buff_ptr =
				(PTR_SERIAL_APP_EVENT_DATA_RECEIVED)val;
	        PTR_BUFFSTACK pTemp;

	        /* Call TRANSPORT_GETDATABUFFER to fetch Buffer Stack */
            if(pSerialShimRecvBuffer == NULL)
            {
                /* We received short packet */
                pSerialShimRecvBuffer = (PTR_BUFFSTACK)pfnAppCallback(
				    TRANSPORT_GETDATABUFFER, (void*)&buff_ptr->transfer_size);
                if(pSerialShimRecvBuffer != NULL)
                {

                    pSerialShimRecvBuffer->top =
                        (uint_8_ptr)pSerialShimRecvBuffer->top -
                        (uint_16)pSerialShimRecvBuffer->total_size;
        			pSerialShimRecvBuffer->current_size = buff_ptr->size;	
                    /* Copy Buffer */

    			    (void)memcpy1((uint_8_ptr)pSerialShimRecvBuffer->top,
    			        (uint_8_ptr)buff_ptr->buffer_ptr,
    			        MIN(pSerialShimRecvBuffer->total_size,
    			        buff_ptr->size));
    			      			
                }
            }
            else
            {
                pSerialShimRecvBuffer->current_size += buff_ptr->size;
            }

			pTemp = pSerialShimRecvBuffer;
			pSerialShimRecvBuffer = NULL;
			pfnAppCallback(TRANSPORT_DATARECIEVED, pTemp);
		    /* Initiate Next Receive */
		    SCI_Class_Read(controller_ID, NULL, 0);
		}
    	break;
	
		case SERIAL_APP_SEND_COMPLETE:
		{
		    uint_8 consumer = gSerialQueue.consumer;
		    PTR_BUFFSTACK pBuffStack = gSerialQueue.serial_buffer
		        [consumer % MAX_SERIAL_XMIT_QUEUE_ELEMENTS];

		    gSerialQueue.consumer++;

			pfnAppCallback(TRANSPORT_DATASENDCOMPLETE,
				pBuffStack);			
		}
    	break;
    	
    	default:
			break;
	}
	return;
}

