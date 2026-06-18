/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 **************************************************************************//*!
 *
 * @file bridge_app.c
 *
 * @author
 *
 * @version
 *
 * @date July-09-2009
 *
 * @brief   This file contains the application code of the Bridge device  
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "hidef.h"          /* for EnableInterrupts macro */
#include "derivative.h"     /* include peripheral declarations */
#include "types.h"          /* User Defined Data Types */

#include "serial_class.h"
#include "usb_phdc.h"
#include "bridge_app.h"
#ifdef __MCF52xxx_H__
#include "realtimercounter_cfv2.h"
#else
#include "realtimercounter.h"
#endif
/* skip the inclusion in dependency statge */
#ifndef __NO_SETJMP
   #include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>			

#if (defined _MCF51MM256_H) || (defined _MCF51JE256_H)
#include "exceptions.h"
#endif
/******************************************************************************
 * Local Variables
 *****************************************************************************/
static uint_8 usb_enum_complete = FALSE;
static uint_8 sci_enum_complete = FALSE;

/******************************************************************************
 * Local Function Prototypes
 *****************************************************************************/
extern void display_led(
    uint_8 val);
void TestApp_Init(void);
void TestApp_Task(void);
static void SCI_App_Callback(
    uint_8 controller_ID, 
    uint_8 event_type, 
    void* val);
static void USB_App_Callback(
      uint_8 controller_ID, 
      uint_8 event_type,    
      void* val
);

/*****************************************************************************
 * Local Functions
 *****************************************************************************/

/******************************************************************************
 *
 *    @name        USB_App_Callback
 *
 *    @brief       This function handles the callback from the USB layer
 *
 *    @param       controller_ID    : Controller ID
 *    @param       event_type       : Type of event
 *    @param       val              : Value of the event
 *
 *    @return      None
 *
 *****************************************************************************
 * This function is called from the lower layer whenever an event occurs on 
 * the USB transport. This sets a variable according to the event_type
 *****************************************************************************/
static void USB_App_Callback(
      uint_8 controller_ID,     /* [IN] Controller ID */
      uint_8 event_type,        /* [IN] Type of event */
      void* val                 /* [IN] Value of the event */
)
{
#ifndef MC9S08
    UNUSED (controller_ID)
#endif
    PTR_USB_APP_EVENT_DATA_RECEIVED event_struct = 
        (PTR_USB_APP_EVENT_DATA_RECEIVED)val;

    switch(event_type)
    {
        case USB_APP_BUS_RESET:
        {            
            usb_enum_complete = FALSE;
        }
        break;
        
        case USB_APP_ENUM_COMPLETE:
        {
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
            usb_enum_complete = TRUE;
            /* Initialize the SCI interface */
            (void)SCI_Class_Init(&serial_init, SCI_App_Callback);            
        }
        break;
        
        case USB_APP_GET_TRANSFER_SIZE:
        {
            
			volatile PTR_USB_CLASS_PHDC_XFER_SIZE xfer_size = 
				(PTR_USB_CLASS_PHDC_XFER_SIZE)val;
#if USB_METADATA_SUPPORTED
			if(xfer_size->meta_data_packet)
			{
				PTR_USB_META_DATA_MSG_PREAMBLE metadata_preamble_ptr =
					(PTR_USB_META_DATA_MSG_PREAMBLE)xfer_size->in_buff;
				xfer_size->transfer_size = (USB_PACKET_SIZE)
					(metadata_preamble_ptr->opaque_data_size + 
					METADATA_HEADER_SIZE);
			}
			else
#endif
            {
                uint_16_ptr pApdu = (uint_16_ptr)xfer_size->in_buff;
                xfer_size->transfer_size = (uint_16)(ieee_htons((uint_16)(pApdu[1])) + 
                    (uint_16)APDU_HEADER_SIZE);
            }            
        }
        break;
        
        case USB_APP_GET_DATA_BUFF:
		{
			/* called by lower layer to get recv buffer */
			volatile PTR_USB_CLASS_PHDC_RX_BUFF usb_rx_buff = 
				(PTR_USB_CLASS_PHDC_RX_BUFF)val;
            SCI_Class_Write(SERIAL_CONTROLLER_ID,
                usb_rx_buff->in_buff, (uint_16) usb_rx_buff->in_size);

		}
        break;
        
        case USB_APP_DATA_RECEIVED:
        {
            
			volatile PTR_USB_APP_EVENT_DATA_RECEIVED usb_rx_buff = 
				(PTR_USB_APP_EVENT_DATA_RECEIVED)val;
            SCI_Class_Write(SERIAL_CONTROLLER_ID,
                usb_rx_buff->buffer_ptr, (uint_16) usb_rx_buff->size);        
        }
        break;
        
        case USB_APP_SEND_COMPLETE:
        {
            SCI_Class_Read(SERIAL_CONTROLLER_ID, (uint_8_ptr)NULL, 0);
        }
        break;
        
        case USB_APP_META_DATA_PARAMS_CHANGED:
        case USB_APP_ERROR:
        {
            (void)USB_Class_PHDC_Recv_Data(USB_CONTROLLER_ID, 
                PHDC_BULK_OUT_QOS, NULL, 0);
        }
        break;
    }
    return;
}
/******************************************************************************
 *
 *    @name        SCI_App_Callback
 *
 *    @brief       This function handles the callback from the SCI layer
 *
 *    @param       controller_ID    : Controller ID
 *    @param       event_type       : Type of event
 *    @param       val              : Value of the event
 *
 *    @return      None
 *
 *****************************************************************************
 * This function is called from the lower layer whenever an event occurs on 
 * the SCI transport. This sets a variable according to the event_type
 *****************************************************************************/
static void SCI_App_Callback(
      uint_8 controller_ID,     /* [IN] Controller ID */
      uint_8 event_type,        /* [IN] Type of event */
      void* val                 /* [IN] Value of the event */
)
{
#ifndef MC9S08
    UNUSED (controller_ID)
#endif    
    PTR_SERIAL_APP_EVENT_DATA_RECEIVED event_struct = 
        (PTR_SERIAL_APP_EVENT_DATA_RECEIVED)val;
    
    switch(event_type)
    {
        case SERIAL_APP_ENUM_COMPLETE:
        {
            sci_enum_complete = TRUE;
        }
        break;

        case SERIAL_APP_DATA_RECEIVED:
        {
            PTR_SERIAL_APP_EVENT_DATA_RECEIVED rx_buff = 
            (PTR_SERIAL_APP_EVENT_DATA_RECEIVED)val;
            (void)USB_Class_PHDC_Send_Data (USB_CONTROLLER_ID, FALSE, 0, 
                SEND_DATA_QOS,(uint_8_ptr)rx_buff->buffer_ptr, 
                (USB_PACKET_SIZE)(rx_buff->size));
        }
        break;                        
        
        case SERIAL_APP_SEND_COMPLETE:
        {
            (void)USB_Class_PHDC_Recv_Data(USB_CONTROLLER_ID, 
                PHDC_BULK_OUT_QOS, NULL, 0);
        }
        break;

        case SERIAL_APP_GET_TRANSFER_SIZE:
        {
            PTR_SERIAL_CLASS_XFER_SIZE xfer_size = 
                (PTR_SERIAL_CLASS_XFER_SIZE)val;
            uint_16_ptr pApdu = (uint_16_ptr)xfer_size->in_buff;
            xfer_size->transfer_size = (uint_16)(ieee_htons(pApdu[1]) + 
                (uint_16)APDU_HEADER_SIZE);            
        }
        break;

        case SERIAL_APP_GET_DATA_BUFF:
        {
            PTR_SERIAL_CLASS_RX_BUFF rx_buff = (PTR_SERIAL_CLASS_RX_BUFF)val;
            (void)USB_Class_PHDC_Send_Data (USB_CONTROLLER_ID, FALSE, 0, 
                SEND_DATA_QOS,(uint_8_ptr)rx_buff->in_buff, 
                (USB_PACKET_SIZE)(rx_buff->in_size));
        }
        break;        
    }
    return;
}

/******************************************************************************
 *
 *   @name        TestApp_Init
 *
 *   @brief       This function is the entry for the Serial Bridge Application
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function is the entry for the Serial Bridge Application
 *****************************************************************************/
void TestApp_Init(void)
{
    DisableInterrupts;
	#if (defined _MCF51MM256_H) || (defined _MCF51JE256_H)
	usb_int_dis();
	#endif	
    /* SERIAL_INIT serial_init = 
    {
        SERIAL_CONTROLLER_ID,
        FALSE,
        0,
        1,
        8,
        (uint_16)57600,
        MAX_SERIAL_RECV_BUFFER   
    };
    */
    /* Initialize the USB interface */
    (void)USB_Class_PHDC_Init(USB_CONTROLLER_ID, USB_App_Callback, NULL);
    EnableInterrupts;
	#if (defined _MCF51MM256_H) || (defined _MCF51JE256_H)
	usb_int_en();
	#endif	    
}

/******************************************************************************
 *
 *   @name        TestApp_Task
 *
 *   @brief       Application task function. It is called from the main loop
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * Application task function. It is called from the main loop
 *****************************************************************************/
void TestApp_Task(void)
{
 return;
}
