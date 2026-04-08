/*
 * Copyright (c) 2010 Freescale Semiconductor;
 * All Rights Reserved
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
 */
/*
 * $FileName: usb_host_hid.c$
 * $Version	: 3.0.4.0$
 * $Date	: Jun-17-2009$
 * Comments:
 *	This file Contains the implementation of class driver for HID devices.
 */

#include "usb_host_hid.h"

static USB_HID_CLASS_INTF_STRUCT_PTR hid_anchor = NULL;

/*
 * Function Name	: usb_hid_mass_init
 * Returned Value	: None
 * Comments			:
 *	This function is called by common class to initialize the class driver. It
 *	is called in response to a select interface call by application
 * Parameters		:
 *	pbs_ptr - [IN]  structure with USB pipe information on the interface
 *	ccs_ptr - [IN] printer call struct pointer
 */

void usb_class_hid_init(PIPE_BUNDLE_STRUCT_PTR pbs_ptr,
						CLASS_CALL_STRUCT_PTR ccs_ptr)
{
	USB_HID_CLASS_INTF_STRUCT_PTR if_ptr = ccs_ptr->class_intf_handle;
	USB_STATUS status;

	/* Make sure the device is still attached */
	USB_lock();

	status = usb_host_class_intf_init(pbs_ptr, if_ptr, &hid_anchor);

	if (status == USB_OK) {
		/*
		 * We generate a code_key based on the attached device. This is
		 * used to verify that the device has not been detached and
		 * replaced with another.
		 */
		ccs_ptr->code_key = 0;

		ccs_ptr->code_key = usb_host_class_intf_validate(ccs_ptr);

#if 0
		if_ptr->IFNUM =
			((INTERFACE_DESCRIPTOR_PTR)if_ptr->G.intf_handle)->bInterfaceNumber;

		if_ptr->P_CONTROL = usb_hostdev_get_pipe_handle(pbs_ptr,
														USB_CONTROL_PIPE,
														0);
		if_ptr->P_INT_IN = usb_hostdev_get_pipe_handle(pbs_ptr,
													   USB_INTERRUPT_PIPE,
													   USB_RECV);
#endif
	}

	/* Signal that an error has occured by setting the "code_key" to 0 */
/*
	if (status || !if_ptr->P_INT_IN || !if_ptr->P_CONTROL) {
		ccs_ptr->code_key = 0;
	}
*/
	USB_unlock();
}

/*
 * Function Name	: usb_class_hid_cntrl_callback
 * Returned Value	: USB_OK if command has been passed on USB.
 * Comments			:
 *	This is the callback used when HID information is sent or received
 * Parameters		:
 *	pipe - [IN] Unused
 *	param - [IN] Pointer to the class interface instance
 *	buffer - [IN] Data buffer
 *	len - [IN] Length of buffer
 *	status - [IN] Error code (if any)
 */
static void usb_class_hid_cntrl_callback(ptr pipe,
										 ptr param,
										 uchar_ptr buffer,
										 uint_32 len,
										 USB_STATUS status)
{
	USB_HID_CLASS_INTF_STRUCT_PTR if_ptr;

	/*
	 * There is need for USB_lock in the callback function, and there is
	 * also no need to check if the device is still attached (otherwise
	 * this callback would never have been called!
	 */

	/* Get class interface handle, reset IN_SETUP and call callback */
	if_ptr = (USB_HID_CLASS_INTF_STRUCT_PTR) param;

	if_ptr->IN_SETUP = FALSE;

	if (if_ptr->USER_CALLBACK) {
		if_ptr->USER_CALLBACK(pipe, if_ptr->USER_PARAM, buffer, len,
				      status);
	}
}

/*
 * Function Name	: usb_class_hid_cntrl_common
 * Returned Value	: USB_OK if command has been passed on USB.
 * Comments			:
 *	This function is used to send a control request
 * Parameters		:
 *	com_ptr - [IN] The communication device common command structure
 *	bmrequesttype - [IN] Bitmask of the request type
 *	brequest - [IN] Request code
 *	wvalue - [IN] Value to copy into WVALUE field of the REQUEST
 *	wlength - [IN] Length of the data associated with REQUEST
 *	data - [IN] Pointer to data buffer used to send/recv
 */
static USB_STATUS usb_class_hid_cntrl_common(HID_COMMAND_PTR com_ptr,
											 uint_8 bmrequesttype,
											 uint_8 brequest,
											 uint_16 wvalue,
											 uint_16 wlength,
											 uchar_ptr data)
{
	USB_HID_CLASS_INTF_STRUCT_PTR if_ptr;
	USB_SETUP req;
	USB_STATUS status = USBERR_NO_INTERFACE;

	USB_lock();

	/* Validity checking */
	if (usb_host_class_intf_validate(com_ptr->CLASS_PTR)) {
		if_ptr =
		    (USB_HID_CLASS_INTF_STRUCT_PTR) com_ptr->CLASS_PTR->class_intf_handle;

		status = usb_hostdev_validate(if_ptr->G.dev_handle);
	}

	if (!status && if_ptr->IN_SETUP) {
		status = USBERR_TRANSFER_IN_PROGRESS;
	}

	if (!status) {
		/* Save the higher level callback and ID */
		if_ptr->USER_CALLBACK = com_ptr->CALLBACK_FN;
		if_ptr->USER_PARAM = com_ptr->CALLBACK_PARAM;

		/* Setup the request */
		req.BMREQUESTTYPE = bmrequesttype;
		req.BREQUEST = brequest;
		htou16(req.WVALUE, wvalue);
		htou16(req.WINDEX,
			   ((INTERFACE_DESCRIPTOR_PTR) if_ptr->G.intf_handle)->bInterfaceNumber);

		htou16(req.WLENGTH, wlength);

		status = _usb_hostdev_cntrl_request(if_ptr->G.dev_handle,
											&req,
											data,
											usb_class_hid_cntrl_callback,
											if_ptr);

		if (status == USB_STATUS_TRANSFER_QUEUED) {
			if_ptr->IN_SETUP = TRUE;
		}
	}

	USB_unlock();

	return status;
}

/*
 * Function Name  : usb_class_hid_get_report
 * Returned Value : USB_OK if command has been passed on USB.
 * Comments       :
 *     This function is called by the application to get a report from the HID
 *     device
 * Parameters		:
 *	com_ptr - [IN] Class Interface structure pointer
 *	rid - [IN] Report ID (see HID specification)
 *	rtype - [IN] Report type (see HID specification)
 *	buf - [IN] Buffer to receive report data
 *	blen - [IN] length of the Buffer
 */
USB_STATUS usb_class_hid_get_report(HID_COMMAND_PTR com_ptr,
									uint_8 rid,
									uint_8 rtype,
									ptr buf,
									uint_16 blen)
{
	return usb_class_hid_cntrl_common(com_ptr,
									  REQ_TYPE_IN | REQ_TYPE_CLASS |
									  REQ_TYPE_INTERFACE,
									  GET_REPORT,
									  (uint_16) ((rtype << 8) | rid),
									  blen,
									  buf);
}

/*
 * Function Name  : usb_class_hid_set_report
 * Returned Value : USB_OK if command has been passed on USB
 * Comments       :
 *     This function is called by the application to send a report to the HID
 *     device
 * Parameters		:
 *	com_ptr - [IN] Class Interface structure pointer
 *	rid - [IN] Report ID (see HID specification)
 *	rtype - [IN] Report type (see HID specification)
 *	buf - [IN] Buffer to receive report data
 *	blen - [IN] length of the Buffer
 */

USB_STATUS usb_class_hid_set_report(HID_COMMAND_PTR com_ptr,
									uint_8 rid,
									uint_8 rtype,
									ptr buf,
									uint_16 blen)
{
	return usb_class_hid_cntrl_common(com_ptr,
									  REQ_TYPE_OUT | REQ_TYPE_CLASS |
									  REQ_TYPE_INTERFACE,
									  SET_REPORT,
									  (uint_16) ((rtype << 8) | rid),
									  blen,
									  buf);
}

/*
 * Function Name  : usb_class_hid_get_idle
 * Returned Value : USB_OK if command has been passed on USB.
 * Comments       :
 *     This function is called by the application to read the idle rate of a
 *     particular HID device report
 * Parameters		:
 *	com_ptr - [IN] Class Interface structure pointer
 *	rid - [IN] Report ID (see HID specification)
 *	idle_rate - [OUT] idle rate of this report
 */
USB_STATUS usb_class_hid_get_idle(HID_COMMAND_PTR com_ptr,
								  uint_8 rid,
								  uint_8_ptr idle_rate)
{
	return usb_class_hid_cntrl_common(com_ptr,
									  REQ_TYPE_IN | REQ_TYPE_CLASS |
									  REQ_TYPE_INTERFACE,
									  GET_IDLE,
									  rid,
									  1,
									  idle_rate);
}

/*
 * Function Name	: usb_class_hid_set_idle
 * Returned Value	: USB_OK if command has been passed on USB
 * Comments			:
 *	This function is called by the application to silence a particular report
 *	on interrupt In pipe until a new event occurs or specified  time elapses
 * Parameters		:
 *	com_ptr - [IN] Class Interface structure pointer
 *	rid - [IN] Report ID (see HID specification)
 *	duration - [IN] Idle rate
 */
USB_STATUS usb_class_hid_set_idle(HID_COMMAND_PTR com_ptr,
								  uint_8 rid,
								  uint_8 duration)
{
	return usb_class_hid_cntrl_common(com_ptr,
									  REQ_TYPE_OUT | REQ_TYPE_CLASS |
									  REQ_TYPE_INTERFACE,
									  SET_IDLE,
									  (uint_16) ((duration << 8) | rid),
									  0,
									  NULL);
}

/*
 * Function Name  : usb_class_hid_get_protocol
 * Returned Value : USB_OK if command has been passed on USB.
 * Comments       :
 *     Reads the active protocol (boot protocol or report protocol)
 * Parameters		:
 *	com_ptr - [IN] Class Interface structure pointer
 *	protocol - [IN] Protocol (1 byte, 0 = Boot Protocol or
 *							  1 = Report Protocol
 */
USB_STATUS usb_class_hid_get_protocol(HID_COMMAND_PTR com_ptr,
									  uchar_ptr protocol)
{
	return usb_class_hid_cntrl_common(com_ptr,
									  REQ_TYPE_IN | REQ_TYPE_CLASS |
									  REQ_TYPE_INTERFACE,
									  GET_PROTOCOL,
									  0,
									  1,
									  protocol);
}

/*
 * Function Name	: usb_class_hid_set_protocol
 * Returned Value	: USB_OK if command has been passed on USB
 * Comments			:
 *	Switches between the boot protocol and the report protocol (or vice versa)
 * Parameters		:
 *	com_ptr - [IN] Class Interface structure pointer
 *	protocol - [IN] The protocol (0 = Boot, 1 = Report)
 */
USB_STATUS usb_class_hid_set_protocol(HID_COMMAND_PTR com_ptr,
									  uint_8 protocol)
{
	return usb_class_hid_cntrl_common(com_ptr,
									  REQ_TYPE_OUT | REQ_TYPE_CLASS |
									  REQ_TYPE_INTERFACE,
									  SET_PROTOCOL,
									  protocol,
									  0,
									  NULL);
}
