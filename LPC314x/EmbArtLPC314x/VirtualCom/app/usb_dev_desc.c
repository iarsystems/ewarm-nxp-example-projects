/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    File name   : usb_desc.c
 *    Description : usb decriptors module
 *
 *    History :
 *    1. Date        : August 10, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "usb_desc.h"

const UsbStandardDeviceDescriptorStr_t UsbStandardDeviceDescriptorStr =
{
  sizeof(UsbStandardDeviceDescriptorStr_t),   // bLength
  UsbDescriptorDevice,                        // bDescriptorType
  0x200,                                      // bcdUSB
  UsbDeviceClassCommunications,               // bDeviceClass
  0,                                          // bDeviceSubClass
  0,                                          // bDeviceProtocol
  Ep0MaxSize,                                 // bMaxPacketSize0
  0xFFFF,                                     // idVendor
  0x003B,                                     // idProduct
  0x0000,                                     // bcdDevice
  iManufacturerStr,                           // iManufacturer
  iProductStr,                                // iProduct
  iSerialNumberStr,                           // iSerialNumber
  1,                                          // bNumConfigurations
};

#pragma data_alignment=4
const Int8U UsbFS_Cfg0[] =
{
// Configuration Descriptor
  // bLength
  sizeof(UsbStandardConfigurationDescriptor_t),
  // bDescriptorType
  UsbDescriptorConfiguration,
  // wTotalLength
  _2BL(sizeof(UsbStandardConfigurationDescriptor_t)+
  (2 * sizeof(UsbStandardInterfaceDescriptor_t))+
   sizeof(CDC_HeaderFuncDescriptor_t)+
   sizeof(CDC_CallManagementFuncDescriptor_t)+
   sizeof(CDC_AbstrCtrlFuncDescriptor_t)+
  (sizeof(CDC_UnionFuncDescriptor_t)+1)+
  (3 * sizeof(UsbStandardEpDescriptor_t))),
  // bNumInterfaces
  2,
  // bConfigurationValue
  1,
  // iConfiguration
  0,
  // bmAttributes
  UsbConfigurationCommmonAttr,
  // bMaxPower
  UsbConfigPower_mA(100),

// Interface Descriptors AbstractControlModel
  // bLength
  sizeof(UsbStandardInterfaceDescriptor_t),
  // bDescriptorType
  UsbDescriptorInterface,
  // bInterfaceNumber
  CDC_CTRL_INTERFACE_IND,
  // bAlternateSetting
  0,
  // bNumEndpoints
  1,
  // bInterfaceClass
  CommInterfaceClassCode,
  // bInterfaceSubClass
  AbstractControlModel,
  // bInterfaceProtocol
  0,
  // iInterface
  0,

// Header Functional Descriptor
  // bFunctionLength
  sizeof(CDC_HeaderFuncDescriptor_t),
  // bDescriptorType
  CS_INTERFACE,
  // bDescriptorSubtype
  Header,
  // bcdCDC
  _2BL(0x110),

// Union Functional Descriptor
  // bFunctionLength
  sizeof(CDC_UnionFuncDescriptor_t)+1,
  // bDescriptorType
  CS_INTERFACE,
  // bDescriptorSubtype
  Union,
  // bMasterInterface
  CDC_CTRL_INTERFACE_IND,
  // bDataInterface
  CDC_DATA_INTERFACE_IND,

//Call Management Functional Descriptor
  // bFunctionLength
  sizeof(CDC_CallManagementFuncDescriptor_t),
  // bDescriptorType
  CS_INTERFACE,
  // bDescriptorSubtype
  CallManagement,
  // bmCapabilities
  0,
  // bDataInterface
  1,

//ACM Functional Descriptor
  // bFunctionLength
  sizeof(CDC_AbstrCtrlFuncDescriptor_t),
  // bDescriptorType
  CS_INTERFACE,
  // bDescriptorSubtype
  AbstractControlManagement,
  // bmCapabilities
  (2 | (CDC_DEVICE_SUPPORT_BREAK?4:0)),

// Endpoint Descriptors ReportEp
  // bLength
  sizeof(UsbStandardEpDescriptor_t),
  // bDescriptorType
  UsbDescriptorEp,
  // bEndpointAddress
  UsbEpIn(ReportEp>>1),
  // bmAttributes
  UsbEpTransferInterrupt,
  // wMaxPacketSize
  _2BL(ReportEpMaxSize),
  // bInterval
  0x1,

// Interface Descriptors Data Class Interface
  // bLength
  sizeof(UsbStandardInterfaceDescriptor_t),
  // bDescriptorType
  UsbDescriptorInterface,
  // bInterfaceNumber
  CDC_DATA_INTERFACE_IND,
  // bAlternateSetting
  0,
  // bNumEndpoints
  2,
  // bInterfaceClass
  DataInterfaceClassCodes,
  // bInterfaceSubClass
  0,
  // bInterfaceProtocol
  0,
  // iInterface
  0,

// Endpoint Descriptors CommOutEp
  // bLength
  sizeof(UsbStandardEpDescriptor_t),
  // bDescriptorType
  UsbDescriptorEp,
  // bEndpointAddress
  UsbEpOut(CommOutEp>>1),
  // bmAttributes
  UsbEpTransferBulk,
  // wMaxPacketSize
  _2BL(CommOutEpMaxSize),
  // bInterval
  0x00,

// Endpoint Descriptors CommInEp
  // bLength
  sizeof(UsbStandardEpDescriptor_t),
  // bDescriptorType
  UsbDescriptorEp,
  // bEndpointAddress
  UsbEpIn(CommInEp>>1),
  // bmAttributes
  UsbEpTransferBulk,
  // wMaxPacketSize
  _2BL(CommInEpMaxSize),
  // bInterval
  0x00,
  0,
};

static const Int8U * const UsbFS_CfgArray[] =
{
  UsbFS_Cfg0,
  NULL,
};

static const UsbEP_ExtDesc_t UsbEPExt_Cfg0EP1 =
{
  1,                     // Configuration
  UsbEpIn(ReportEp>>1),  // EP address
  {
    1,0
  }
};
static const UsbEP_ExtDesc_t UsbEPExt_Cfg0EP2 =
{
  1,                     // Configuration
  UsbEpIn(CommOutEp>>1),  // EP address
  {
    1,0
  }
};

static const UsbEP_ExtDesc_t UsbEPExt_Cfg0EP3 =
{
  1,                     // Configuration
  UsbEpOut(CommInEp>>1),  // EP address
  {
    1,0
  }
};

static const UsbEP_ExtDesc_t * const UsbFS_EPExtArray[] =
{
  &UsbEPExt_Cfg0EP1,
  &UsbEPExt_Cfg0EP2,
  &UsbEPExt_Cfg0EP3,
  NULL,
};

#pragma data_alignment=4
const Int8U UsbLanguagesStr [] =
{
  // Length of LanguagesStr + 2
  6,
  // Descriptor Type
  UsbDescriptorString,
  // Languages ID
  // Languages1 ID English
  0x09,0x04,
  // Languages2 ID Bulgarian
  0x02,0x04
};

#pragma data_alignment=4
const Int8U ManufacturerStrLan1 [] =
{
  24, // length
  UsbDescriptorString,  // Descriptor
  'I',0,'A',0,'R',0,' ',0,'S',0,'y',0,'s',0,'t',0,'e',0,'m',0,'s',0, // Str
};

#pragma data_alignment=4
const Int8U ManufacturerStrLan2 [] =
{
  24, //length
  UsbDescriptorString, // Descriptor
  'È',0,'À',0,'Ð',0,' ',0,'Ñ',0,'è',0,'ñ',0,'ò',0,'å',0,'ì',0,'è',0, // Str
};

#pragma data_alignment=4
const Int8U ProductStrLan1 [] =
{
  32, //length
  UsbDescriptorString, // Descriptor
  'E',0,'m',0,'b',0,' ',0,'A',0,'r',0,'t',0,' ',0,'L',0,'P',0,'C',0,'3',0,'1',0,'4',0,'x',0, // Str
};

#pragma data_alignment=4
const Int8U ProductStrLan2 [] =
{
  32, //length
  UsbDescriptorString, // Descriptor
  'Å',0,'ì',0,'á',0,' ',0,'À',0,'ð',0,'ò',0,' ',0,'Ë',0,'Ï',0,'Ö',0,'3',0,'1',0,'4',0,'õ',0, // Str
};

#pragma data_alignment=4
const Int8U SerialNumberStrLan1 [] =
{
  18, //length
  UsbDescriptorString, // Descriptor
  '0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'1',0, // Str
};

const Int8U * const UsbLanguages1Strings[] =
{
  // iManufacturerStr
  ManufacturerStrLan1,
  // iProductStr
  ProductStrLan1,
  // iSerialNumberStr
  SerialNumberStrLan1,
  // Terminator
  NULL
};

const Int8U * const UsbLanguages2Strings[] =
{
  // iManufacturerStr
  ManufacturerStrLan2,
  // iProductStr
  ProductStrLan2,
  // iSerialNumberStr
  SerialNumberStrLan1,
  // Terminator
  NULL
};

static const Int8U * const * const UsbFS_StringArray[] =
{
  UsbLanguages1Strings,
  UsbLanguages2Strings,
  NULL,
};

#if USB_HIGH_SPEED > 0
#pragma data_alignment=4
const UsbStandardDeviceQualifierDescriptor_t UsbQualifierDescriptor =
{
  sizeof(UsbStandardDeviceQualifierDescriptor_t),
  UsbDescriptorDeviceQualifier,
  0x200,
  0,
  0,
  0,
  Ep0MaxSize,
  1,
  0
};

#pragma data_alignment=4
static const Int8U UsbFS_OtherCfg0[] =
{
  sizeof(UsbStandardConfigurationDescriptor_t),
  UsbDescriptorOtherSeedConfiguration,
  // wTotalLength
  _2BL(sizeof(UsbStandardConfigurationDescriptor_t)+
  (2 * sizeof(UsbStandardInterfaceDescriptor_t))+
   sizeof(CDC_HeaderFuncDescriptor_t)+
   sizeof(CDC_CallManagementFuncDescriptor_t)+
   sizeof(CDC_AbstrCtrlFuncDescriptor_t)+
  (sizeof(CDC_UnionFuncDescriptor_t)+1)+
  (3 * sizeof(UsbStandardEpDescriptor_t))),
  // bNumInterfaces
  2,
  // bConfigurationValue
  1,
  // iConfiguration
  0,
  // bmAttributes
  UsbConfigurationCommmonAttr,
  // bMaxPower
  UsbConfigPower_mA(100),

// Interface Descriptors AbstractControlModel
  // bLength
  sizeof(UsbStandardInterfaceDescriptor_t),
  // bDescriptorType
  UsbDescriptorInterface,
  // bInterfaceNumber
  CDC_CTRL_INTERFACE_IND,
  // bAlternateSetting
  0,
  // bNumEndpoints
  1,
  // bInterfaceClass
  CommInterfaceClassCode,
  // bInterfaceSubClass
  AbstractControlModel,
  // bInterfaceProtocol
  0,
  // iInterface
  0,

// Header Functional Descriptor
  // bFunctionLength
  sizeof(CDC_HeaderFuncDescriptor_t),
  // bDescriptorType
  CS_INTERFACE,
  // bDescriptorSubtype
  Header,
  // bcdCDC
  _2BL(0x110),

// Union Functional Descriptor
  // bFunctionLength
  sizeof(CDC_UnionFuncDescriptor_t)+1,
  // bDescriptorType
  CS_INTERFACE,
  // bDescriptorSubtype
  Union,
  // bMasterInterface
  CDC_CTRL_INTERFACE_IND,
  // bDataInterface
  CDC_DATA_INTERFACE_IND,

//Call Management Functional Descriptor
  // bFunctionLength
  sizeof(CDC_CallManagementFuncDescriptor_t),
  // bDescriptorType
  CS_INTERFACE,
  // bDescriptorSubtype
  CallManagement,
  // bmCapabilities
  0,
  // bDataInterface
  1,

//ACM Functional Descriptor
  // bFunctionLength
  sizeof(CDC_AbstrCtrlFuncDescriptor_t),
  // bDescriptorType
  CS_INTERFACE,
  // bDescriptorSubtype
  AbstractControlManagement,
  // bmCapabilities
  (2 | (CDC_DEVICE_SUPPORT_BREAK?4:0)),

// Endpoint Descriptors ReportEp
  // bLength
  sizeof(UsbStandardEpDescriptor_t),
  // bDescriptorType
  UsbDescriptorEp,
  // bEndpointAddress
  UsbEpIn(ReportEp>>1),
  // bmAttributes
  UsbEpTransferInterrupt,
  // wMaxPacketSize
  _2BL(ReportEpMaxSize),
  // bInterval
  0x1,

// Interface Descriptors Data Class Interface
  // bLength
  sizeof(UsbStandardInterfaceDescriptor_t),
  // bDescriptorType
  UsbDescriptorInterface,
  // bInterfaceNumber
  CDC_DATA_INTERFACE_IND,
  // bAlternateSetting
  0,
  // bNumEndpoints
  2,
  // bInterfaceClass
  DataInterfaceClassCodes,
  // bInterfaceSubClass
  0,
  // bInterfaceProtocol
  0,
  // iInterface
  0,

// Endpoint Descriptors CommOutEp
  // bLength
  sizeof(UsbStandardEpDescriptor_t),
  // bDescriptorType
  UsbDescriptorEp,
  // bEndpointAddress
  UsbEpOut(CommOutEp>>1),
  // bmAttributes
  UsbEpTransferBulk,
  // wMaxPacketSize
  _2BL(CommOutEpMaxSize_1),
  // bInterval
  0x00,

// Endpoint Descriptors CommInEp
  // bLength
  sizeof(UsbStandardEpDescriptor_t),
  // bDescriptorType
  UsbDescriptorEp,
  // bEndpointAddress
  UsbEpIn(CommInEp>>1),
  // bmAttributes
  UsbEpTransferBulk,
  // wMaxPacketSize
  _2BL(CommInEpMaxSize_1),
  // bInterval
  0x00,
  0,
};

static const Int8U * const UsbFS_OtherCfgArray[] =
{
  UsbFS_OtherCfg0,
  NULL,
};

#endif // USB_HIGH_SPEED > 0

const void * const UsbDescArray[] =
{
  UsbFS_CfgArray,
  UsbFS_EPExtArray,
  UsbLanguagesStr,
  UsbFS_StringArray,
#if USB_HIGH_SPEED > 0
  UsbFS_OtherCfgArray,
  UsbFS_CfgArray,
  UsbFS_EPExtArray,
  UsbLanguagesStr,
  UsbFS_StringArray,    // Full and High speed device have same strings descriptors
  UsbFS_OtherCfgArray,
#endif // USB_HIGH_SPEED > 0
  NULL,
};
