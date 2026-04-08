#include <stdint.h>
#include <string.h>
#include "mw_usbd_rom_api.h"

extern USBD_API_T* pUsbApi;
extern const uint8_t HID_ReportDescriptor[];
extern const uint16_t HID_ReportDescSize;

 /** Callback for event - USB configuration number changed. This event fires when a the USB host changes
   *  the selected configuration number. On receiving configuration change request from host, the stack
   *  enables/configures the endpoints needed by the new configuration before calling this callback
   *  function.
   *  \n
   *  \note This event is called from USB_ISR context and hence is time-critical. Having delays in this
   *  callback will prevent the device from enumerating correctly or operate properly.
   *
   */

ErrorCode_t USB_Configure_Event (USBD_HANDLE_T hUsb) 
{
USB_CORE_CTRL_T* pCtrl = (USB_CORE_CTRL_T*)hUsb;
uint8_t dummyReport = 0;

  if (pCtrl->config_value)  /* Check if USB is configured */
  {
    pUsbApi->hw->WriteEP(hUsb, HID_EP_IN, (uint8_t*)&dummyReport, sizeof(dummyReport));
  }
  return LPC_OK;
}

  /**
  *  HID get report callback function.
  *
  *  This function is provided by the application software. This function gets called
  *  when host sends a HID_REQUEST_GET_REPORT request. The setup packet data (\em pSetup)
  *  is passed to the callback so that application can extract the report ID, report
  *  type and other information need to generate the report. \note HID reports are sent
  *  via interrupt IN endpoint also. This function is called only when report request
  *  is received on control endpoint. Application should implement \em HID_EpIn_Hdlr to
  *  send reports to host via interrupt IN endpoint.
  *
  *
  *  \param[in] hHid Handle to HID function driver.
  *  \param[in] pSetup Pointer to setup packet recived from host.
  *  \param[in, out] pBuffer  Pointer to a pointer of data buffer containing report data.
  *                       Pointer-to-pointer is used to implement zero-copy buffers.
  *                       See \ref USBD_ZeroCopy for more details on zero-copy concept.
  *  \param[in] length  Amount of data copied to destination buffer.
  *  \return The call back should returns \ref ErrorCode_t type to indicate success or error condition.
  *          \retval LPC_OK On success.
  *          \retval ERR_USBD_UNHANDLED  Event is not handled hence pass the event to next in line.
  *          \retval ERR_USBD_xxx  For other error conditions.
  *
  */

ErrorCode_t HID_GetReport( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t* length)
{
uint8_t dummyReport = 0;

  switch (pSetup->wValue.WB.H) 
  {
    case HID_REPORT_INPUT:
      /* On HID_GetReport, send dummy report.
       User implementation may be added to send a real report */
      **pBuffer = dummyReport;
      *length = sizeof(dummyReport);
      break;
    case HID_REPORT_OUTPUT:
      return (ERR_USBD_STALL);
    case HID_REPORT_FEATURE:
      return (ERR_USBD_STALL);
  }
  return (LPC_OK);
}

  /**
  *  HID set report callback function.
  *
  *  This function is provided by the application software. This function gets called
  *  when host sends a HID_REQUEST_SET_REPORT request. The setup packet data (\em pSetup)
  *  is passed to the callback so that application can extract the report ID, report
  *  type and other information need to modify the report. An application might choose
  *  to ignore input Set_Report requests as meaningless. Alternatively these reports
  *  could be used to reset the origin of a control (that is, current position should
  *  report zero).
  *
  *  \param[in] hHid Handle to HID function driver.
  *  \param[in] pSetup Pointer to setup packet recived from host.
  *  \param[in, out] pBuffer  Pointer to a pointer of data buffer containing report data.
  *                       Pointer-to-pointer is used to implement zero-copy buffers.
  *                       See \ref USBD_ZeroCopy for more details on zero-copy concept.
  *  \param[in] length  Amount of data copied to destination buffer.
  *  \return The call back should returns \ref ErrorCode_t type to indicate success or error condition.
  *          \retval LPC_OK On success.
  *          \retval ERR_USBD_UNHANDLED  Event is not handled hence pass the event to next in line.
  *          \retval ERR_USBD_xxx  For other error conditions.
  *
  */

ErrorCode_t HID_SetReport( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t length)
{
uint8_t dummyReport;

  if (length == 0)
    return LPC_OK;

  switch (pSetup->wValue.WB.H)
  {
    case HID_REPORT_INPUT:
      return (ERR_USBD_STALL);
    /* On HID_SetReport, store report in dummy buffer.
       User implementation may be added to handle the report */
    case HID_REPORT_OUTPUT:
      dummyReport = **pBuffer;
      break;
    case HID_REPORT_FEATURE:
      return (ERR_USBD_STALL);
  }
  return (LPC_OK);
}

  /**
  *  Optional Interrupt endpoint event handler.
  *
  *  The application software could provide Interrupt endpoint event handler.
  *  Application which send reports to host on interrupt endpoint should provide
  *  an endpoint event handler through this data member. This data memeber is
  *  ignored if the interface descriptor \em intf_desc doesn't have any interrupt
  *  endpoint descriptor associated.
  *  \n
  *
  *  \param[in] hUsb Handle to the USB device stack.
  *  \param[in] data Handle to HID function driver.
  *  \param[in] event  Type of endpoint event. See \ref USBD_EVENT_T for more details.
  *  \return The call back should return \ref ErrorCode_t type to indicate success or error condition.
  *          \retval LPC_OK On success.
  *          \retval ERR_USBD_UNHANDLED  Event is not handled hence pass the event to next in line.
  *          \retval ERR_USBD_xxx  For other error conditions.
  *
  */

ErrorCode_t HID_Endpoint_Handler (USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
  USB_HID_CTRL_T* pHidCtrl = (USB_HID_CTRL_T*)data;
  uint8_t dummyReport;

  switch (event) {
    case USB_EVT_IN:
      pUsbApi->hw->WriteEP(hUsb, pHidCtrl->epin_adr, (uint8_t*)&dummyReport, sizeof(dummyReport));
      break;
    case USB_EVT_OUT:
      pUsbApi->hw->ReadEP(hUsb, pHidCtrl->epout_adr, (uint8_t*)&dummyReport);
      break;
  }
  return LPC_OK;
}


ErrorCode_t usb_hid_init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size)
{
USBD_HID_INIT_PARAM_T hid_param;
USB_HID_REPORT_T reports_data[1];
ErrorCode_t ret;

  /* init reports_data */
  reports_data[0].len       = HID_ReportDescSize;
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

  /* update memory variables */
  *mem_base = hid_param.mem_base;
  *mem_size = hid_param.mem_size;

  return ret;
}
