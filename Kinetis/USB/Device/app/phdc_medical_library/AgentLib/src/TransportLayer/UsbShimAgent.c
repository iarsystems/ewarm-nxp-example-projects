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
 * @file UsbShimAgent.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains USB Shim Implementation
 *
 *****************************************************************************/
/******************************************************************************
 * Includes
 *****************************************************************************/
#include "UsbShimAgent.h"
#include "usb_phdc.h"
#include <string.h>

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define USB_CONTROLLER_ID		(0)
#define MIN(a,b)    (((a) > (b)) ? (b) : (a))
#define MAX_USB_XMIT_QUEUE_ELEMENTS  (4)

/*****************************************************************************
 * Local Types
 *****************************************************************************/
#pragma pack(1)

typedef struct _usb_shim_xmit_queue
{
    uint_8 producer;
    uint_8 consumer;
    PTR_BUFFSTACK   usb_buffer[MAX_USB_XMIT_QUEUE_ELEMENTS];
}USB_SHIM_XMIT_QUEUE, *PTR_USB_SHIM_XMIT_QUEUE;

#pragma options align=reset

/*****************************************************************************
 * Local Variables
 *****************************************************************************/
/* Stores Send and receive Buffer Stack Pointers */
static PTR_BUFFSTACK pUsbShimRecvBuffer = NULL;
static APP_CALLBACK pfnAppCallback = NULL;

static USB_SHIM_XMIT_QUEUE gUsbQueue;
/*****************************************************************************
 * Local Functions Prototypes 
 *****************************************************************************/
#if (defined(_MC9S08MM128_H) || defined(_MC9S08JE128_H))
#pragma CODE_SEG PAGED_ROM
#endif
static ERR_CODE UsbShimInitialize(APP_CALLBACK pAppCallback);
static ERR_CODE UsbShimDeInitilize();
static ERR_CODE UsbShimSendData(
    boolean          meta_data,
    uint_8           num_tfr,
    uint_8           current_qos,
    PTR_BUFFSTACK    pBuffStack
	);
static ERR_CODE UsbShimRecvData(
    uint_8           current_qos,
    PTR_BUFFSTACK    pBuffStack
	);
static void USBShimCallback(
    uint_8 controller_ID,  /* [IN] Controller ID */
    uint_8 event_type,     /* [IN] type of the event */
    void* val              /* [IN] Pointer to Received Buffer */
);
static ERR_CODE UsbShimCancelSendData(void);

#if (defined(_MC9S08MM128_H) || defined(_MC9S08JE128_H))
#pragma CODE_SEG DEFAULT
#endif
/*****************************************************************************
 * Global Variables 
 *****************************************************************************/
/* USB Shim Structure */
const SHIM USBShim = 
{
	SHIM_USB,
	UsbShimInitialize,
	UsbShimDeInitilize,
	UsbShimSendData,
	UsbShimRecvData
};

/*****************************************************************************
 * Local Functions 
 *****************************************************************************/
#if (defined(_MC9S08MM128_H) || defined(_MC9S08JE128_H))
#pragma CODE_SEG PAGED_ROM
#endif
/**************************************************************************//*!
 *
 * @name  UsbShimInitialize
 *
 * @brief This function initializes USB Shim 
 *
 * @param pAppCallback	:	Application Callback function Pointer
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: If Successful
 *			ERROR_SHIM_INIT_FAILED	: If Failed
 ******************************************************************************
 * This funtion initializes USB Shim
 *****************************************************************************/
static ERR_CODE UsbShimInitialize(
	APP_CALLBACK pAppCallback  /* [IN] Application Callback function Pointer */
)
{
	uint_8 status;
	ERR_CODE err = ERROR_SUCCESS;
	if(pAppCallback == NULL)
	{
	    return ERROR_SHIM_INIT_FAILED; 
	}
	/* USB PHDC Class Initialization */
	pfnAppCallback = pAppCallback;
	status = USB_Class_PHDC_Init(USB_CONTROLLER_ID, USBShimCallback, NULL);
	if(status != USB_OK)
	{
		err = ERROR_SHIM_INIT_FAILED;
	}

	return err;
}

/**************************************************************************//*!
 *
 * @name  UsbShimDeInitilize
 *
 * @brief This function de-initializes USB Shim 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS			: Always
 ******************************************************************************
 * This funtion de-initializes USB Shim
 *****************************************************************************/
static ERR_CODE UsbShimDeInitilize(void)
{
	pfnAppCallback = NULL;
	return ERROR_SUCCESS;
}

/**************************************************************************//*!
 *
 * @name  UsbShimSendData
 *
 * @brief This function sends Data Buffer through USB PHDC Class Driver
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
 * This funtion sends Data Buffer through USB PHDC Class Driver
 *****************************************************************************/
static ERR_CODE UsbShimSendData(
    boolean          meta_data,		/* [IN] Metadata Flag */
    uint_8           num_tfr,		/* [IN] Number of Transfers */
    uint_8           current_qos,	/* [IN] Data QOS */
    PTR_BUFFSTACK    pBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
	uint_8 status;
	ERR_CODE err = ERROR_SUCCESS;
	if(gUsbQueue.producer - gUsbQueue.consumer < 
	    MAX_USB_XMIT_QUEUE_ELEMENTS)
	{
	    gUsbQueue.usb_buffer[gUsbQueue.producer % MAX_USB_XMIT_QUEUE_ELEMENTS] = 
	        pBuffStack; 
	    gUsbQueue.producer++;
	}
	else
	{
	    return ERROR_SEND_FAILED;
	}
	/* Class Send Data */
	status = USB_Class_PHDC_Send_Data(USB_CONTROLLER_ID, meta_data, 
		num_tfr, current_qos, (uint_8_ptr)pBuffStack->top, 
		(USB_PACKET_SIZE)pBuffStack->current_size);
	
	if(status != USB_OK)
	{
		err = ERROR_SEND_FAILED;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  UsbShimRecvData
 *
 * @brief This function receives Data Buffer from USB PHDC Class Driver
 *
 * @param current_qos	:	Data QOS
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS		: If Successful
 *          ERROR_RECV_FAILED		: If Failed
 ******************************************************************************
 * This funtion receives Data Buffer from USB PHDC Class Driver
 *****************************************************************************/
static ERR_CODE UsbShimRecvData(
    uint_8           current_qos,	/* [IN] Data QOS */
    PTR_BUFFSTACK    pBuffStack		/* [IN] Pointer to Buffer Stack */
)
{
	uint_8 status;
	ERR_CODE err = ERROR_SUCCESS;
	uint_8_ptr ptemp = (uint_8_ptr)pBuffStack->top;
	uint_8_ptr pStart = (uint_8_ptr)&ptemp[pBuffStack->current_size];
	/* Class Recv Data */
    status = USB_Class_PHDC_Recv_Data(USB_CONTROLLER_ID, current_qos, 
		pStart, (USB_PACKET_SIZE)pBuffStack->total_size);

	if(status != USB_OK)
	{
		err = ERROR_RECV_FAILED;
	}
	return err;
}

/**************************************************************************//*!
 *
 * @name  USBShimCallback
 *
 * @brief This function receives USB PHDC Class Driver Events
 *
 * @param controller_ID	:	Controller ID
 * @param event_type	:	type of the event
 * @param val			:	Pointer to Received Buffer
 *
 * @return None
 ******************************************************************************
 * This funtion receives USB PHDC Class Driver Events
 *****************************************************************************/
static void USBShimCallback(
    uint_8 controller_ID,  /* [IN] Controller ID */
    uint_8 event_type,     /* [IN] type of the event */
    void* val              /* [IN] Pointer to Received Buffer */
)
{
    UNUSED(controller_ID)
    
	switch(event_type)
	{
		case USB_APP_BUS_RESET:
	  case USB_APP_CONFIG_CHANGED:
	        {
	          (void)UsbShimCancelSendData();
	          (void)pfnAppCallback(TRANSPORT_DISCONNECT, NULL);
	        }
			break;

		case USB_APP_ENUM_COMPLETE:
	        pfnAppCallback(TRANSPORT_CONNECT, NULL);
			break;
	    case USB_APP_GET_TRANSFER_SIZE:
	    {
            PTR_USB_CLASS_PHDC_XFER_SIZE pxfer_size = 
				(PTR_USB_CLASS_PHDC_XFER_SIZE)val;
#if USB_METADATA_SUPPORTED
			if(pxfer_size->meta_data_packet)
			{
				PTR_USB_META_DATA_MSG_PREAMBLE metadata_preamble_ptr =
					(PTR_USB_META_DATA_MSG_PREAMBLE)pxfer_size->in_buff;
				pxfer_size->transfer_size = (USB_PACKET_SIZE)
					(metadata_preamble_ptr->opaque_data_size + 
					METADATA_HEADER_SIZE);
			}
			else
#endif
			{
			    (void)pfnAppCallback(TRANSPORT_GET_XFER_SIZE, 
			        (PTR_TIL_XFER_SIZE)&pxfer_size->in_buff);
			}
	        break;
	    }
	    case USB_APP_ERROR:
	    {
	        PTR_USB_PHDC_ERROR_STRUCT pError = (PTR_USB_PHDC_ERROR_STRUCT)val;
	        
	        (void)USB_Class_PHDC_Recv_Data(USB_CONTROLLER_ID, pError->qos, NULL, 0);
	        break;
	    }
#if USB_METADATA_SUPPORTED
	    case USB_APP_META_DATA_PARAMS_CHANGED:
	        {
	            PTR_USB_APP_EVENT_METADATA_PARAMS pMetaData = 
	            (PTR_USB_APP_EVENT_METADATA_PARAMS)val;
	            USB_Class_PHDC_Recv_Data(USB_CONTROLLER_ID, pMetaData->qos, 
	                NULL, 0);
	            break;
	        }
#endif	        
		case USB_APP_GET_DATA_BUFF:
		{
			/* called by lower layer to get recv buffer */
			PTR_USB_CLASS_PHDC_RX_BUFF rx_buff = 
				(PTR_USB_CLASS_PHDC_RX_BUFF)val;
		    if(pUsbShimRecvBuffer == NULL)
		    {
	            pUsbShimRecvBuffer = (PTR_BUFFSTACK)pfnAppCallback(
				    TRANSPORT_GETDATABUFFER, (void*)&rx_buff->transfer_size);

		        if(pUsbShimRecvBuffer == NULL)
				{
				    /* Unable to allocate BUFFSTACK */
				    (void)USB_Class_PHDC_Recv_Data(USB_CONTROLLER_ID, 
				        rx_buff->qos, NULL, 0);
				    return;
				}

                pUsbShimRecvBuffer->top = 
                    (uint_8_ptr)pUsbShimRecvBuffer->top - 
                    (uint_16)pUsbShimRecvBuffer->total_size;

    			pUsbShimRecvBuffer->current_size = rx_buff->in_size;	
                /* Copy Buffer */
			    (void)memcpy(pUsbShimRecvBuffer->top, rx_buff->in_buff, 
                    (size_t)MIN(pUsbShimRecvBuffer->total_size, rx_buff->in_size));
		    }
		    else
		    {
		        pUsbShimRecvBuffer->current_size += rx_buff->in_size;
		    }
			(void)USB_Class_PHDC_Recv_Data(USB_CONTROLLER_ID, rx_buff->qos, 
			    &((uint_8_ptr)pUsbShimRecvBuffer->top)[pUsbShimRecvBuffer->current_size], 
			    (uint_16)(GetStackSize(pUsbShimRecvBuffer) - 
			    GetDataSize(pUsbShimRecvBuffer))); 
            break;
		}
		case USB_APP_DATA_RECEIVED:
		{		    
			PTR_USB_APP_EVENT_DATA_RECEIVED buff_ptr = 
				(PTR_USB_APP_EVENT_DATA_RECEIVED)val;
	        PTR_BUFFSTACK pTemp;
            
	        /* Call TRANSPORT_GETDATABUFFER to fetch Buffer Stack */
            if(pUsbShimRecvBuffer == NULL)
            {
                /* We received short packet */
                pUsbShimRecvBuffer = (PTR_BUFFSTACK)pfnAppCallback(
				    TRANSPORT_GETDATABUFFER, (void*)&buff_ptr->transfer_size);
                if(pUsbShimRecvBuffer != NULL)
                {
                    
                    pUsbShimRecvBuffer->top = 
                        (uint_8_ptr)pUsbShimRecvBuffer->top - 
                        (uint_16)pUsbShimRecvBuffer->total_size;
        			pUsbShimRecvBuffer->current_size = buff_ptr->size;	
                    /* Copy Buffer */
    			    (void)memcpy(pUsbShimRecvBuffer->top, 
    			        buff_ptr->buffer_ptr, 
    			        (size_t)MIN(pUsbShimRecvBuffer->total_size, buff_ptr->size));
                }
            }
            else
            {
                pUsbShimRecvBuffer->current_size += buff_ptr->size;    
            }

			pTemp = pUsbShimRecvBuffer;
			pUsbShimRecvBuffer = NULL;
			(void)pfnAppCallback(TRANSPORT_DATARECIEVED, pTemp);
		    /* Initiate Next Receive */
		    (void)USB_Class_PHDC_Recv_Data(USB_CONTROLLER_ID, buff_ptr->qos, NULL, 0);
			break;
		}
		case USB_APP_SEND_COMPLETE:
		{
			uint_8 consumer = gUsbQueue.consumer;
		    PTR_BUFFSTACK pBuffStack = gUsbQueue.usb_buffer[consumer % MAX_USB_XMIT_QUEUE_ELEMENTS];
		    gUsbQueue.consumer++;
		    
			(void)pfnAppCallback(TRANSPORT_DATASENDCOMPLETE, 
				pBuffStack);
		}
		break;
		default:
			break;
	}
	return;
}

/**************************************************************************//*!
 *
 * @name  UsbShimSendData
 *
 * @brief This function clears the PHDC transmit queue
 * 
 *
 * @return ERR_CODE
 *			ERROR_SUCCESS		: If Successful
 *          ERROR_SEND_FAILED		: If Failed
 ******************************************************************************
 * This funtion sends Data Buffer through USB PHDC Class Driver
 *****************************************************************************/
static ERR_CODE UsbShimCancelSendData(void)
{
  
  /* Clean buffers */	
	while(gUsbQueue.producer != gUsbQueue.consumer)
	{	           
	 PTR_BUFFSTACK buff = gUsbQueue.usb_buffer[gUsbQueue.consumer % MAX_USB_XMIT_QUEUE_ELEMENTS];
	 if(buff != NULL)
	 {
	   /* This buffer was pending for transmission when the USB_RESET came */
	   /* Deallocate the memory as we're not going to call the callback */
	   StackDeInit(buff);
	 }
	 gUsbQueue.consumer++;
	}
	gUsbQueue.consumer = 0;
	gUsbQueue.producer = 0; 
	
	return ERROR_SUCCESS;
}
  


#if (defined(_MC9S08MM128_H) || defined(_MC9S08JE128_H))
#pragma CODE_SEG DEFAULT
#endif
