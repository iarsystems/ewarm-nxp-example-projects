/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : usb_hid_mouse.h
 *    Description : USB HID Mouse Routines header
 *
 *    History :
 *    1. Date        : July 12, 2012
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/

#ifndef __USB_HID_MOUSE_H
#define __USB_HID_MOUSE_H

#include "stdint.h"

extern USBD_API_T* pUsbApi;

typedef struct _MouseReport_t
{
  uint8_t Buttons;
  int8_t  X;
  int8_t  Y;
} MouseReport_t, *pMouseReport_t;

extern MouseReport_t MouseReport;

/* Mouse report status flag */
extern uint8_t report_changed;

ErrorCode_t USB_Configure_Event (USBD_HANDLE_T hUsb);
ErrorCode_t HID_GetReport( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t* plength);
ErrorCode_t HID_SetReport( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t length);
ErrorCode_t HID_Endpoint_Handler(USBD_HANDLE_T hUsb, void* data, uint32_t event);
ErrorCode_t USB_HID_Init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size);

#endif /* __USB_HID_MOUSE_H */
