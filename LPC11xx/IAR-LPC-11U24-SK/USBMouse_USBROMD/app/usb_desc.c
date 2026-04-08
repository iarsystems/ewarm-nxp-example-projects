/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : usb_desc.c
 *    Description : USB Decriptors Module
 *
 *    History :
 *    1. Date        : July 12, 2012
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

/* USB Standard Device Descriptor */
#pragma data_alignment = 4
const uint8_t USB_DeviceDescriptor[] =
{
  USB_DEVICE_DESC_SIZE,              /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(0x0200), /* 2.00 */          /* bcdUSB */
  0x00,                              /* bDeviceClass */
  0x00,                              /* bDeviceSubClass */
  0x00,                              /* bDeviceProtocol */
  USB_MAX_PACKET0,                   /* bMaxPacketSize0 */
  WBVAL(0xFFFF),                     /* idVendor */
  WBVAL(0x005D),                     /* idProduct */
  WBVAL(0x0100), /* 1.00 */          /* bcdDevice */
  0x01,                              /* iManufacturer */
  0x02,                              /* iProduct */
  0x03,                              /* iSerialNumber */
  0x01                               /* bNumConfigurations */
};

#pragma data_alignment = 4
const uint8_t HID_ReportDescriptor[] =
{
  HID_UsagePage  (HID_USAGE_PAGE_GENERIC),    // Usage Page (Generic Desktop)
  HID_Usage      (HID_USAGE_GENERIC_MOUSE),   // Usage (Mouse)
  HID_Collection (HID_Application),           // Collection (Application)
    HID_Usage     (HID_USAGE_GENERIC_POINTER),//   Usage (Pointer)
    HID_Collection(HID_Physical),             //   Collection (Application)
      HID_UsagePage  (HID_USAGE_PAGE_BUTTON), //     Usage Page (Buttons)
      HID_ReportSize (1),                     //     Report Size (1)
      HID_ReportCount(3),                     //     Report Count (3)
      HID_UsageMin   (1),                     //     Usage Minimum (1)
      HID_UsageMax   (3),                     //     Usage Maximum (3)
      HID_LogicalMin (0),                     //     Logical Minimum (0)
      HID_LogicalMax (1),                     //     Logical Maximum (1)
      HID_Input      (HID_Data | HID_Variable | HID_Absolute),     //    Input(Data,Var,Abs)
      HID_ReportSize (5),                     //     Report Size (5)
      HID_ReportCount(1),                     //     Report Count (1)
      HID_Input      (HID_Constant | HID_Variable | HID_Absolute), //    Input(Const,Var,Abs)
      HID_UsagePage  (HID_USAGE_PAGE_GENERIC),//     Generic desktop
      HID_ReportSize (8),                     //     Report Size (8)
      HID_ReportCount(2),                     //     Report Count (2)
      HID_Usage      (HID_USAGE_GENERIC_X),   //     Usage(X)
      HID_Usage      (HID_USAGE_GENERIC_Y),   //     Usage(Y)
      HID_LogicalMin (0x80),                  //     Logical Minimum (-128)
      HID_LogicalMax (0x7F),                  //     Logical Maximum (127)
      HID_Input      (HID_Data | HID_Variable | HID_Relative),     //    Input(Data,Var,Rel)
    HID_EndCollection,                        //   End collection
  HID_EndCollection                           // End collection
};

const uint32_t HID_ReportDescriptorSize =  sizeof(HID_ReportDescriptor);

#pragma data_alignment = 4
const uint8_t USB_FsConfigDescriptor[] = {
/* Configuration 1 */
  USB_CONFIGUARTION_DESC_SIZE,        /* bLength */
  USB_CONFIGURATION_DESCRIPTOR_TYPE,  /* bDescriptorType */
  WBVAL(                              /* wTotalLength */
    1*USB_CONFIGUARTION_DESC_SIZE +
    1*USB_INTERFACE_DESC_SIZE     +
    HID_DESC_SIZE                 +
    1*USB_ENDPOINT_DESC_SIZE
  ),
  0x01,                               /* bNumInterfaces */
  0x01,                               /* bConfigurationValue */
  0x00,                               /* iConfiguration */
  (USB_SELF_POWERED?USB_CONFIG_SELF_POWERED:0) | 
  (USB_REMOTE_WAKEUP?USB_CONFIG_REMOTE_WAKEUP:0), /* bmAttributes */
  USB_CONFIG_POWER_MA(100),           /* bMaxPower */

/* Interface 0, Alternate Setting 0, HID Class */
  USB_INTERFACE_DESC_SIZE,            /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  HID_INTERFACE_0,                    /* bInterfaceNumber */
  0x00,                               /* bAlternateSetting */
  0x01,                               /* bNumEndpoints */
  USB_DEVICE_CLASS_HUMAN_INTERFACE,   /* bInterfaceClass */
  HID_BOOT_DEVICE?HID_SUBCLASS_BOOT:0,/* bInterfaceSubClass */
  HID_PROTOCOL_MOUSE,                 /* bInterfaceProtocol */
  0x04,                               /* iInterface */

/* HID Class Descriptor */
  HID_DESC_SIZE,                      /* bLength */
  HID_HID_DESCRIPTOR_TYPE,            /* bDescriptorType */
  WBVAL(0x0111), /* 1.11 */           /* bcdHID */
  0x00,                               /* bCountryCode */
  0x01,                               /* bNumDescriptors */
  HID_REPORT_DESCRIPTOR_TYPE,         /* bDescriptorType */
  WBVAL(HID_REPORT_DESC_SIZE),        /* wDescriptorLength */

/* Endpoint, HID Interrupt In */
  USB_ENDPOINT_DESC_SIZE,             /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
  HID_EP_IN,                          /* bEndpointAddress */
  USB_ENDPOINT_TYPE_INTERRUPT,        /* bmAttributes */
  WBVAL(0x0003),                      /* wMaxPacketSize */
  USB_POOLING_PERIOD,                 /* bInterval */

/* Terminator */
  0                                   /* bLength */
};

#pragma data_alignment = 4
const uint8_t USB_StringDescriptor[] =
{
  // Length of LanguagesStr + 2
  0x04,                            /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,      /* bDescriptorType */
  // Languages  ID
  // Languages1 ID English
  WBVAL(0x0409), /* US English */  /* wLANGID */
  // Manufacturer
  24, // length
  USB_STRING_DESCRIPTOR_TYPE,
  'I',0,'A',0,'R',0,' ',0,'S',0,'y',0,'s',0,'t',0,'e',0,'m',0,'s',0,
  // Product
  100, //length
  USB_STRING_DESCRIPTOR_TYPE,
  'I',0,'A',0,'R',0,' ',0,'E',0,'m',0,'b',0,'e',0,'d',0,'d',0,'e',0,
  'd',0,' ',0,'W',0,'o',0,'r',0,'k',0,'b',0,'e',0,'n',0,'c',0,'h',0,
  ' ',0,'A',0,'R',0,'M',0,' ',0,'-',0,' ',0,'U',0,'S',0,'B',0,'R',0,
  'O',0,'M',0,'D',0,' ',0,'H',0,'I',0,'D',0,' ',0,'e',0,'x',0,'a',0,
  'm',0,'p',0,'l',0,'e',0,'!',0,
  // Serial Number
  18, //length
  USB_STRING_DESCRIPTOR_TYPE,
  '0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'1',0,
  // Interface0
  // Index 0x04: Interface 0, Alternate Setting 0
  (3*2 + 2),                       // bLength (3 Char + Type + lenght) 
  USB_STRING_DESCRIPTOR_TYPE,
  'H', 0,
  'I', 0,
  'D', 0
};
