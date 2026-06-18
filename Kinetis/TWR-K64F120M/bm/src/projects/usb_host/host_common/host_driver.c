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
 * $FileName: host_driver.c$
 * $Version : 3.4.17.0$
 * $Date    : Sep-11-2009$
 *
 * Comments:
 *   This file contains MQX-specific code to install low-level device
 *   driver functions
 */

#include "usb.h"

#include "hostapi.h"
#include "usbprv_host.h"
#include "khci.h"

#include "host_driver.h"

/*
 * Function Name  : _usb_host_driver_install
 * Returned Value : None
 * Comments       :
 *    Installs the device
 * Parameters     :
 *    device_number - [IN] device number
 *    callback_struct_ptr - [IN] address if the callback functions structure
 */
uint_8 _usb_host_driver_install(uint_32 device_number, ptr callback_struct_ptr)
{
	ptr callback_struct_table_ptr;

	callback_struct_table_ptr =
	    (USB_HOST_CALLBACK_FUNCTIONS_STRUCT_PTR) malloc(USBCFG_MAX_DRIVERS *
							    sizeof
							    (USB_HOST_CALLBACK_FUNCTIONS_STRUCT));

	if (!callback_struct_table_ptr) {
		return USBERR_DRIVER_INSTALL_FAILED;
	}

	*((USB_HOST_CALLBACK_FUNCTIONS_STRUCT_PTR)
	  (((USB_HOST_CALLBACK_FUNCTIONS_STRUCT_PTR) callback_struct_table_ptr)
	   + device_number)) = *((USB_HOST_CALLBACK_FUNCTIONS_STRUCT_PTR) callback_struct_ptr);

	return USB_OK;

}
