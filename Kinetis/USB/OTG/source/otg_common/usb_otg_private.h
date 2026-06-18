/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor;
* All Rights Reserved
*
*************************************************************************** 
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
**************************************************************************
*
* Comments:
*
*   This is the header file for the OTG driver
*
*END************************************************************************/

#ifndef USB_OTG_PRIVATE_H_
#define USB_OTG_PRIVATE_H_

/* Public constants */
enum { srp_not_started , srp_se0 , srp_dp_puls};
#define OTG_STATUS_BM_REQ_TYPE  0x80
#define OTG_STATUS_SELECTOR     0xF000


/* USB register access macro */
#define USB_OTG_REG(dev_num, REG)	(REG)
#define USB_OTG_USB_ON(dev_num) ((CTL & (CTL_USB_EN_SOF_EN_MASK | CTL_HOST_MODE_EN_MASK ))?(TRUE):(FALSE))

typedef struct usb_otg_status
{ 
 uint_8              id;          /* Current ID state */
 uint_8              vbus_valid;
 uint_8              sess_valid;
 uint_8              sess_end;
 //uint_8              line_change;
 uint_16             ms_since_line_changed;
 uint_16             host_req_poll_timer;
 uint_8              line_stable;
 uint_8              tmr_1ms;  
 uint_8              live_se0;
 uint_8              live_jstate;
 uint_8              srp_support;
 uint_8              hnp_support;
 
 uint_8              b_timeout_en;
 uint_16             b_timeout;
 uint_8              a_conn;
 uint_8              a_bus_drop; 
 uint_8              a_bus_req;
 uint_8              a_clr_err; 
 uint_8              b_conn; 
 uint_16             b_conn_dbnc_time; 
 uint_8              a_set_b_hnp_en;   
 uint_8              a_srp_det; 
 uint_8              a_srp_det_state; 
 uint_16             a_srp_pulse_duration;
 uint_8              hnp_req; 
 
}USB_OTG_STATUS;



/* Public types */
typedef struct usb_otg_struct
{
	uint_8                  deviceNumber;      /* Current USB device number */
	
	uint_8					        deviceState;	     /* Current device state (A or B) */
	uint_8					        subState;		       /* Current SM sub-state */
	
	uint_8                  srpRequest;        /* SRP was requested by application (B device) */
	uint_8                  busRequest;        /* HNP was requested by application (B device) */
	uint_8                  busRelease;        /* USB bus released (B device) */
	uint_8                  powerUp;
	uint_8                  hnpEnabled;
	
	USB_OTG_STATUS          otg_status;
	DEV_INSTANCE_PTR        dev_inst_ptr;
	
	uint_8                  pending_ext_isr;   /* An interrupt from the external OTG circuit is currently pending */	
	
	USB_EVENT_STRUCT        otg_app_event;     /* The app event signaled internally. Bases on this, the application callback is called */
	
	OTG_INIT_STRUCT         *init_struct;      /* Application initialization structure containing also external circuit access functions */
	otg_event_callback		  callback;		       /* Application registered callback function */
}USB_OTG_STRUCT;

/* Public definitions */

/* Public functions */

extern void _usb_otg_init_a_device(_usb_otg_handle otg_handle);
extern void _usb_otg_init_se0_detect(_usb_otg_handle otg_handle);
extern void _usb_otg_dp_pullup_enable(_usb_otg_handle otg_handle, boolean enable);


#endif /* USB_OTG_PRIVATE_H_ */
