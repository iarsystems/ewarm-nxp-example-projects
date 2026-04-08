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
* $FileName: hidkeyboard.c$
* $Version : 3.4.26.0$
* $Date    : Sep-17-2009$
*
* Comments:
*
*   This file is an example of device drivers for the HID class. This example
*   demonstrates the keyboard functionality. Note that a real keyboard driver also
*   needs to distinguish between intentionally repeated and unrepeated key presses.
*   This example simply demonstrates how to receive data from a USB Keyboard. 
*   Interpretation of data is upto the application and customers can add the code
*   for it.
*
*END************************************************************************/
#include "types.h"
#include "psptypes.h"
#include "usb_host_hid.h"
#include "usb_host_hub_sm.h"
#include "usbevent.h"
#include "host_common.h" 
#include "hidkeyboard.h"
#include "host_driver.h"
#include "khci.h"


/***************************************
**
** Globals
*/
USB_EVENT_STRUCT USB_Keyboard_Event;

#define USB_EVENT_CTRL          0x01
#define USB_EVENT_DATA          0x02 
#define MAX_SUPPORTED_USAGE_ID  45

volatile DEVICE_STRUCT keyboard_hid_device = { 0 };
extern _usb_host_handle host_handle;





void process_kbd_buffer(uchar_ptr buffer);


static const uchar HID_table[MAX_SUPPORTED_USAGE_ID][2] = {
    {0, 0},                     /* Usage ID  0 */
    {0, 0},                     /* Usage ID  1 */
    {0, 0},                     /* Usage ID  2 */
    {0, 0},                     /* Usage ID  3 */
    {0x61, 'A'},                /* Usage ID  4 */
    {'b', 'B'},                 /* Usage ID  5 */
    {'c', 'C'},                 /* Usage ID  6 */
    {'d', 'D'},                 /* Usage ID  7 */
    {'e', 'E'},                 /* Usage ID  8 */
    {'f', 'F'},                 /* Usage ID  9 */
    {'g', 'G'},                 /* Usage ID 10 */
    {'h', 'H'},                 /* Usage ID 11 */
    {'i', 'I'},                 /* Usage ID 12 */
    {'j', 'J'},                 /* Usage ID 13 */
    {'k', 'K'},                 /* Usage ID 14 */
    {'l', 'L'},                 /* Usage ID 15 */
    {'m', 'M'},                 /* Usage ID 16 */
    {'n', 'N'},                 /* Usage ID 17 */
    {'o', 'O'},                 /* Usage ID 18 */
    {'p', 'P'},                 /* Usage ID 19 */
    {'q', 'Q'},                 /* Usage ID 20 */
    {'r', 'R'},                 /* Usage ID 21 */
    {'s', 'S'},                 /* Usage ID 22 */
    {'t', 'T'},                 /* Usage ID 23 */
    {'u', 'U'},                 /* Usage ID 24 */
    {'v', 'V'},                 /* Usage ID 25 */
    {'w', 'W'},                 /* Usage ID 26 */
    {'x', 'X'},                 /* Usage ID 27 */
    {'y', 'Y'},                 /* Usage ID 28 */
    {'z', 'Z'},                 /* Usage ID 29 */
    {'1', '!'},                 /* Usage ID 30 */
    {'2', '@'},                 /* Usage ID 31 */
    {'3', '#'},                 /* Usage ID 32 */
    {'4', '$'},                 /* Usage ID 33 */
    {'5', '%'},                 /* Usage ID 34 */
    {'6', '^'},                 /* Usage ID 35 */
    {'7', '&'},                 /* Usage ID 36 */
    {'8', '*'},                 /* Usage ID 37 */
    {'9', '('},                 /* Usage ID 38 */
    {'0', ')'},                 /* Usage ID 39 */
    {'\n', '\n'},               /* Usage ID 40 */
    {0x1B, 0x1B},               /* Usage ID 41 */
    {0x7F, 0x7F},               /* Usage ID 43 */
    {0x9, 0x9},                 /* Usage ID 42 */
    {0x20, 0x20},               /* Usage ID 44 */

};


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : Main_Task
* Returned Value : none
* Comments       :
*     Execution starts here
*
*END*--------------------------------------------------------------------*/
void Keyboard_Task(void)
{
    USB_STATUS status = USB_OK;
    static TR_INIT_PARAM_STRUCT tr;
    static _usb_pipe_handle pipe;
    static uint_32 check = 0, usb_event_flag = 1;     
    static uchar_ptr buffer;
    static HID_COMMAND_PTR hid_com;
    
    if(NULL == buffer)
      buffer = USB_mem_alloc_uncached(HID_KEYBOARD_BUFFER_SIZE);
    if (buffer == NULL) {
        printf("\nMemory allocation failed.");
        fflush(stdout);
        exit(1);
    }
    
    if(NULL == hid_com)
	  	hid_com = (HID_COMMAND_PTR) USB_mem_alloc_uncached(sizeof(HID_COMMAND));
	  if(NULL == hid_com) 
	  {
	    printf("\nError allocating buffer!");
		fflush(stdout);
		exit(1);
	  }
	  
    switch (keyboard_hid_device.DEV_STATE) 
    {
      case USB_DEVICE_IDLE:
          break;

      case USB_DEVICE_ATTACHED:
          keyboard_hid_device.DEV_STATE = USB_DEVICE_SET_INTERFACE_STARTED;

          status = _usb_hostdev_select_interface(keyboard_hid_device.DEV_HANDLE, keyboard_hid_device.INTF_HANDLE, (pointer) & keyboard_hid_device.CLASS_INTF);
          if(status != USB_OK) 
          {
              printf("\nError in _usb_hostdev_select_interface: %x", status);
              fflush(stdout);
              exit(1);
          }
          break;

      case USB_DEVICE_SET_INTERFACE_STARTED:
          break;

      case USB_DEVICE_INTERFACED:
          printf("Keyboard device interfaced, setting protocol...\n");
          /* now we will set the USB Hid standard boot protocol */
          keyboard_hid_device.DEV_STATE = USB_DEVICE_SETTING_PROTOCOL;

          hid_com->CLASS_PTR = (CLASS_CALL_STRUCT_PTR) & keyboard_hid_device.CLASS_INTF;
          hid_com->CALLBACK_FN = usb_host_hid_keyboard_ctrl_callback;
          hid_com->CALLBACK_PARAM = 0;

          /* Force the keyboard to behave as in USB Hid class standard boot protocol */
          status = usb_class_hid_set_protocol(hid_com, USB_PROTOCOL_HID_KEYBOARD);

          if(status != USB_STATUS_TRANSFER_QUEUED) {
              printf("\nError in usb_class_hid_set_protocol: %x", status);
              fflush(stdout);
          }
          break;

      case USB_DEVICE_INUSE:
  			    if(0 == check)
  			    {
  			      check = 1;
  			      
  			      pipe = _usb_hostdev_find_pipe_handle(keyboard_hid_device.DEV_HANDLE, keyboard_hid_device.INTF_HANDLE, USB_INTERRUPT_PIPE, USB_RECV);   
  			      if(pipe)
  			      {
           			printf("Keyboard device ready, try to press the keyboard\n");
           			fflush(stdout);
  			      } 
  			    }

     			/******************************************************************
                Initiate a transfer request on the interrupt pipe
     			******************************************************************/
                
                    _usb_event_clear(&USB_Keyboard_Event, USB_EVENT_CTRL | USB_EVENT_DATA);  
         			      usb_hostdev_tr_init(&tr, usb_host_hid_keyboard_recv_callback, NULL);
   
                  	tr.RX_BUFFER = buffer;
                  	tr.RX_LENGTH = HID_KEYBOARD_BUFFER_SIZE;	                  	
                  	
                  	status = _usb_host_recv_data(host_handle, pipe, &tr);
                  	                  	
                  	if (status != USB_STATUS_TRANSFER_QUEUED) {
                     	printf("\nError in _usb_host_recv_data: %x", status);
                     	fflush(stdout);
                    }
                
                /* Wait untill we get the data from keyboard. */
                while(_usb_event_wait_ticks(&USB_Keyboard_Event,USB_EVENT_CTRL | USB_EVENT_DATA, FALSE, 0) == USB_EVENT_NOT_SET){
                  _usb_khci_task();                  
                } 
                
                /* if not detached in the meanwhile */
                //_usb_event_clear(&USB_Event, USB_EVENT_CTRL | USB_EVENT_DATA);
                if(keyboard_hid_device.DEV_STATE == USB_DEVICE_INUSE) {	                         
                   process_kbd_buffer((uchar *)buffer);  
                    //_usb_event_clear(&USB_Event, USB_EVENT_CTRL | USB_EVENT_DATA);      
                                               	                
                }
          break;

      case USB_DEVICE_DETACHED:
    	  check = 0;
          printf("Going to idle state\n");
          keyboard_hid_device.DEV_STATE = USB_DEVICE_IDLE;
          break;
      }
  
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_hid_keyboard_event
* Returned Value : None
* Comments       :
*     Called when HID device has been attached, detached, etc.
*END*--------------------------------------------------------------------*/

void usb_host_hid_keyboard_event(
    /* [IN] pointer to device instance */
    _usb_device_instance_handle dev_handle,
    /* [IN] pointer to interface descriptor */
    _usb_interface_descriptor_handle intf_handle,
    /* [IN] code number for event causing callback */
    uint_32 event_code)
{
    INTERFACE_DESCRIPTOR_PTR intf_ptr = (INTERFACE_DESCRIPTOR_PTR) intf_handle;

    fflush(stdout);
    switch (event_code) {

    case USB_ATTACH_EVENT:
        printf("----- Attach Event -----\n");
        /* Drop through config event for the same processing */
    case USB_CONFIG_EVENT:
        printf("State = %d", keyboard_hid_device.DEV_STATE);
        printf("  Class = %d", intf_ptr->bInterfaceClass);
        printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
        printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
        fflush(stdout);

        if(keyboard_hid_device.DEV_STATE == USB_DEVICE_IDLE) {
            keyboard_hid_device.DEV_HANDLE = dev_handle;
            keyboard_hid_device.INTF_HANDLE = intf_handle;
            keyboard_hid_device.DEV_STATE = USB_DEVICE_ATTACHED;
        }
        else {
            printf("HID device already attached\n");
            fflush(stdout);
        }
        break;

    case USB_INTF_EVENT:
        printf("----- Interfaced Event -----\n");
        keyboard_hid_device.DEV_STATE = USB_DEVICE_INTERFACED;
        break;

    case USB_DETACH_EVENT:
        /* Use only the interface with desired protocol */
        printf("\n----- Detach Event -----\n");
        printf("State = %d", keyboard_hid_device.DEV_STATE);
        printf("  Class = %d", intf_ptr->bInterfaceClass);
        printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
        printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
        fflush(stdout);

        keyboard_hid_device.DEV_HANDLE = NULL;
        keyboard_hid_device.INTF_HANDLE = NULL;
        keyboard_hid_device.DEV_STATE = USB_DEVICE_DETACHED;
        break;
    }

    /* notify application that status has changed */
    _usb_event_set(&USB_Keyboard_Event, USB_EVENT_CTRL);
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_hid_ctrl_callback
* Returned Value : None
* Comments       :
*     Called when a control pipe command is completed. 
*
*END*--------------------------------------------------------------------*/

void usb_host_hid_keyboard_ctrl_callback(
    /* [IN] pointer to pipe */
    _usb_pipe_handle pipe_handle,
    /* [IN] user-defined parameter */
    pointer user_parm,
    /* [IN] buffer address */
    uchar_ptr buffer,
    /* [IN] length of data transferred */
    uint_32 buflen,
    /* [IN] status, hopefully USB_OK or USB_DONE */
    uint_32 status)
{
    UNUSED(pipe_handle)
    UNUSED(user_parm)
    UNUSED(buffer)
    UNUSED(buflen)
    
    if(status == USBERR_ENDPOINT_STALLED) {
        printf("\nHID Set_Protocol Request BOOT is not supported!\n");
        fflush(stdout);
    }
    else if(status) {
        printf("\nHID Set_Protocol Request BOOT failed!: 0x%x ... END!\n", status);
        fflush(stdout);
        exit(1);
    }

    if(keyboard_hid_device.DEV_STATE == USB_DEVICE_SETTING_PROTOCOL)
        keyboard_hid_device.DEV_STATE = USB_DEVICE_INUSE;

    /* notify application that status has changed */
    _usb_event_set(&USB_Keyboard_Event, USB_EVENT_CTRL);
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_hid_recv_callback
* Returned Value : None
* Comments       :
*     Called when a interrupt pipe transfer is completed.
*
*END*--------------------------------------------------------------------*/

void usb_host_hid_keyboard_recv_callback(
    /* [IN] pointer to pipe */
    _usb_pipe_handle pipe_handle,
    /* [IN] user-defined parameter */
    pointer user_parm,
    /* [IN] buffer address */
    uchar_ptr buffer,
    /* [IN] length of data transferred */
    uint_32 buflen,
    /* [IN] status, hopefully USB_OK or USB_DONE */
    uint_32 status)
{
    UNUSED(pipe_handle)
    UNUSED(user_parm)
    UNUSED(buffer)
    UNUSED(buflen)
    UNUSED(status)
    
    /* notify application that data are available */
    _usb_event_set(&USB_Keyboard_Event, USB_EVENT_DATA);
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : process_kbd_buffer
* Returned Value : None
* Comments       :
*      The way keyboard works is that it sends reports of 8 bytes of data
*      every time keys are pressed. However, it reports all the keys
*      that are pressed in a single report. The following code should
*      really be implemented by a user in the way he would like it to be.
*END*--------------------------------------------------------------------*/
void process_kbd_buffer(uchar_ptr buffer)
{
    /* a little array in which we count how long each key is pressed */
    static uchar special_last;

    uint_32 i, shift = 0;
    uchar code;

    /* The first byte in buffer gives special key status.
     ** Process only the keys which are newly pressed. */
    code = (uint_8)((buffer[0] ^ special_last) & buffer[0]);
    special_last = buffer[0];

    shift = 0;
    /* Check Modifiers in byte 0 (see HID specification 1.11 page 56) */
    if(code & 0x01) {
        printf("LEFT CTRL ");
    }
    if(code & 0x02) {
        printf("LEFT SHIFT ");
    }
    if(code & 0x04) {
        printf("LEFT ALT ");
    }
    if(code & 0x08) {
        printf("LEFT GUI ");
    }
    if(code & 0x10) {
        printf("RIGHT CTRL ");
    }
    if(code & 0x20) {
        printf("RIGHT SHIFT ");
    }
    if(code & 0x40) {
        printf("RIGHT ALT ");
    }
    if(code & 0x80) {
        printf("RIGHT GUI ");
    }

    /* immediate state of left or right SHIFT */
    if(buffer[0] & 0x22) {
        shift = 1;
    }

    /* Byte 1 is reserved (HID specification 1.11 page 60) */

    /*
     ** Build initial press-map by checking Keybcodes in bytes 2 to 7 
     ** (HID spec 1.11 page 60)
     */
    for(i = HID_KEYBOARD_BUFFER_SIZE - 1; i >= 2; i--) {
        code = buffer[i];

        /* if valid keyboard code was received */
        if(code > 1) {
            if(code <= MAX_SUPPORTED_USAGE_ID && HID_table[code][shift]) {
                printf("%c",HID_table[code][shift]);
            }
            else {
                /* Print hexadecimal code */
                printf("\\x%02x",code);
            }

            /* only print the newest key */
            break;
        }
    }

    fflush(stdout);
}

