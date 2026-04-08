/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : usb_hid_mouse.c
 *    Description : USB HID Mouse Routines
 *
 *    History :
 *    1. Date        : July 12, 2012
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 4271 $
 **************************************************************************/
#include "includes.h"

MouseReport_t MouseReport =
{
  0,                // Buttons
  0,                // X
  0,                // Y
};

/* Mouse report status flag */
uint8_t report_changed = 0;

/* Pointer to the report from host */
static uint8_t* hostReport;

/*************************************************************************
 * Function Name: CB_IsReportChanged
 *
 * Parameters:    uint8_t changed
 *
 * Return: none
 *
 * Description: Callback on checking the hid report status-flag.
 *
 *************************************************************************/
uint8_t CB_IsReportChanged()
{
  return report_changed;
}

/*************************************************************************
 * Function Name: CB_SetReportChange
 *
 * Parameters:    uint8_t changed
 *
 * Return: none
 *
 * Description: Callback on altering the hid report status-flag.
 *
 *************************************************************************/
void CB_SetReportChange(uint8_t changed)
{
  report_changed = changed;
}

/*************************************************************************
 * Function Name: USB_Configure_Event
 *
 * Parameters:    USBD_HANDLE_T hUsb
 *
 * Return: ErrorCode_t  LPC_OK - success
 *                       other - error
 *
 * Description: HID Configure Event Callback.
 *              Called automatically on HID Configure Event
 *
 *************************************************************************/
ErrorCode_t USB_Configure_Event (USBD_HANDLE_T hUsb)
{
USB_CORE_CTRL_T* pCtrl = (USB_CORE_CTRL_T*)hUsb;
  if (pCtrl->config_value)                  /* Check if USB is configured */
  {
    pUsbApi->hw->WriteEP(hUsb, HID_EP_IN, (uint8_t*)&MouseReport, sizeof(MouseReport_t));
  }
  return LPC_OK;
}

/*************************************************************************
 * Function Name: HID_GetReport
 *
 * Parameters:    USBD_HANDLE_T hHid
 *                USB_SETUP_PACKET* pSetup
 *                uint8_t** pBuffer
 *                uint16_t length
 *
 * Return: ErrorCode_t  LPC_OK - success
 *                       other - error
 *
 * Description: HID Get Report Request Callback.
 *              Called automatically on HID Get Report Request
 *
 *************************************************************************/
ErrorCode_t HID_GetReport( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t* length)
{
MouseReport_t dummyReport = {0,0,0};

  switch (pSetup->wValue.WB.H) {
    case HID_REPORT_INPUT:
      if (CB_IsReportChanged())
      {
        *pBuffer = (uint8_t*)&MouseReport;
        *length = sizeof( MouseReport_t );
        CB_SetReportChange(false);
      }
      else
      {
        *pBuffer = (uint8_t*)&dummyReport;
        *length = sizeof( MouseReport_t );
      }
      break;
    case HID_REPORT_OUTPUT:
      return (ERR_USBD_STALL);          /* Not Supported */
    case HID_REPORT_FEATURE:
      return (ERR_USBD_STALL);          /* Not Supported */
  }
  return (LPC_OK);
}

/*************************************************************************
 * Function Name: HID_SetReport
 *
 * Parameters:    USBD_HANDLE_T hHid
 *                USB_SETUP_PACKET* pSetup
 *                uint8_t** pBuffer
 *                uint16_t length
 *
 * Return: ErrorCode_t  LPC_OK - success
 *                       other - error
 *
 * Description: HID Set Report Request Callback.
 *              Called automatically on HID Set Report Request
 *
 *************************************************************************/
ErrorCode_t HID_SetReport( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t length)
{
  if (length == 0)
    return LPC_OK;

  switch (pSetup->wValue.WB.H) {
    case HID_REPORT_INPUT:
      return (ERR_USBD_STALL);          /* Not Supported */
    case HID_REPORT_OUTPUT:
      *hostReport = **pBuffer;          /* Handle report from host */
      break;
    case HID_REPORT_FEATURE:
      return (ERR_USBD_STALL);          /* Not Supported */
  }
  return (LPC_OK);
}

/*************************************************************************
 * Function Name: HID_Endpoint_Handler
 *
 * Parameters:    USBD_HANDLE_T hUsb
 *                void* data
 *                uint32_t event
 *
 * Return: ErrorCode_t  LPC_OK - success
 *                       false - error
 *
 * Description: USB Endpoint Event Callback.
 *              Called automatically on USB Endpoint Event
 *
 *************************************************************************/
ErrorCode_t HID_Endpoint_Handler(USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
USB_HID_CTRL_T* pHidCtrl = (USB_HID_CTRL_T*)data;
MouseReport_t dummyReport = {0,0,0};

  switch (event) {
    case USB_EVT_IN:
      if (CB_IsReportChanged())
      {
        pUsbApi->hw->WriteEP(hUsb, pHidCtrl->epin_adr, (uint8_t*)&MouseReport, sizeof( MouseReport_t ));
        CB_SetReportChange(false);
      }
      else
      {
        pUsbApi->hw->WriteEP(hUsb, pHidCtrl->epin_adr, (uint8_t*)&dummyReport, sizeof( MouseReport_t ));
      }
      break;
    case USB_EVT_OUT:
      pUsbApi->hw->ReadEP(hUsb, pHidCtrl->epout_adr, hostReport);
      break;
  }
  return LPC_OK;
}

/*************************************************************************
 * Function Name: USB_HID_Init
 *
 * Parameters:    USBD_HANDLE_T hUsb
 *                USB_INTERFACE_DESCRIPTOR* pIntfDesc
 *                uint32_t* mem_base
 *                uint32_t* mem_size
 *
 * Return: ErrorCode_t  LPC_OK - success
 *                       false - error
 *
 * Description: USB HID Initialization.
 *
 *************************************************************************/
ErrorCode_t USB_HID_Init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size)
{
USBD_HID_INIT_PARAM_T hid_param;
USB_HID_REPORT_T reports_data[1];
ErrorCode_t ret;

  /* init reports_data */
  reports_data[0].len       = HID_ReportDescriptorSize;
  reports_data[0].idle_time = 0;
  reports_data[0].desc      = (uint8_t *)&HID_ReportDescriptor[0];

  if ((pIntfDesc == 0) || (pIntfDesc->bInterfaceClass != USB_DEVICE_CLASS_HUMAN_INTERFACE))
    return ERR_FAILED;

  /* clear hid_param structure */
  memset((void*)&hid_param, 0, sizeof(USBD_HID_INIT_PARAM_T));

  /* init hid_param structure */
  hid_param.mem_base    = *mem_base;
  hid_param.mem_size    = *mem_size;
  hid_param.intf_desc   = (uint8_t*)pIntfDesc;
  hid_param.max_reports = 1;
  hid_param.report_data = reports_data;

  /* user defined functions */
  hid_param.HID_GetReport  = &HID_GetReport;
  hid_param.HID_SetReport  = &HID_SetReport;
  hid_param.HID_EpIn_Hdlr  = &HID_Endpoint_Handler;
  hid_param.HID_EpOut_Hdlr = &HID_Endpoint_Handler;

  /* init HID */
  ret = pUsbApi->hid->init(hUsb, &hid_param);

  /* allocate USB accessable memory space for report data */
  hostReport = (uint8_t*)hid_param.mem_base;
  hid_param.mem_base += 4;
  hid_param.mem_size += 4;

  /* update memory variables */
  *mem_base = hid_param.mem_base;
  *mem_size = hid_param.mem_size;

  return ret;
}
