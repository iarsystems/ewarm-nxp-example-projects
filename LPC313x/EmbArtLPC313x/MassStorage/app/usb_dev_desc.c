/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : usb_desc.c
 *    Description : usb decriptors module
 *
 *    History :
 *    1. Date        : 22, August 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "usb_dev_desc.h"

const UsbStandardDeviceDescriptorStr_t UsbStandardDeviceDescriptorStr =
{
  sizeof(UsbStandardDeviceDescriptorStr_t),   // bLength
  UsbDescriptorDevice,                        // bDescriptorType
  0x200,                                      // bcdUSB
  0,                                          // bDeviceClass
  0,                                          // bDeviceSubClass
  0,                                          // bDeviceProtocol
  Ep0MaxSize,                                 // bMaxPacketSize0
  0xFFFF,                                     // idVendor
  0x0032,                                     // idProduct
  0x0000,                                     // bcdDevice
  iManufacturerStr,                           // iManufacturer
  iProductStr,                                // iProduct
  iSerialNumberStr,                           // iSerialNumber
  1,                                          // bNumConfigurations
};

#pragma data_alignment=4
static const Int8U UsbFS_Cfg0[] =
{
// Configuration Descriptor
  sizeof(UsbStandardConfigurationDescriptor_t),
  UsbDescriptorConfiguration,
  (1*sizeof(UsbStandardConfigurationDescriptor_t)+
   1*sizeof(UsbStandardInterfaceDescriptor_t)+
   2*sizeof(UsbStandardEpDescriptor_t)),
  (1*sizeof(UsbStandardConfigurationDescriptor_t)+
   1*sizeof(UsbStandardInterfaceDescriptor_t)+
   2*sizeof(UsbStandardEpDescriptor_t)) >> 8,
  1,
  1,
  0,
  UsbConfigurationCommmonAttr,
  UsbConfigPower_mA(100),
  sizeof(UsbStandardInterfaceDescriptor_t),
  UsbDescriptorInterface,
  0,
  0,
  2,
  UsbDeviceClassStorage,
  MscSubClassScsi,
  MscProtocolBulkOnly,
  0,
  sizeof(UsbStandardEpDescriptor_t),
  UsbDescriptorEp,
  UsbEpIn(BulkInEp>>1),
  UsbEpTransferBulk,
  0xFF & BulkInEpMaxSize,
  BulkInEpMaxSize>>8,
  0,
  sizeof(UsbStandardEpDescriptor_t),
  UsbDescriptorEp,
  UsbEpOut(BulkOutEp>>1),
  UsbEpTransferBulk,
  0xFF & BulkOutEpMaxSize,
  BulkOutEpMaxSize>>8,
  0,
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
  UsbEpIn(BulkInEp>>1),  // EP address
  {
    1,0
  }
};

static const UsbEP_ExtDesc_t UsbEPExt_Cfg0EP2 =
{
  1,                     // Configuration
  UsbEpOut(BulkOutEp>>1),  // EP address
  {
    1,0
  }
};

static const UsbEP_ExtDesc_t * const UsbFS_EPExtArray[] =
{
  &UsbEPExt_Cfg0EP1,
  &UsbEPExt_Cfg0EP2,
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
  'E',0,'m',0,'b',0,' ',0,'A',0,'r',0,'t',0,' ',0,'L',0,'P',0,'C',0,'3',0,'1',0,'3',0,'x',0, // Str
};

#pragma data_alignment=4
const Int8U ProductStrLan2 [] =
{
  32, //length
  UsbDescriptorString, // Descriptor
  'Å',0,'ì',0,'á',0,' ',0,'À',0,'ð',0,'ò',0,' ',0,'Ë',0,'Ï',0,'Ö',0,'3',0,'1',0,'3',0,'õ',0, // Str
};

#pragma data_alignment=4
const Int8U SerialNumberStrLan1 [] =
{
  26, //length
  UsbDescriptorString, // Descriptor
  '0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'1',0, // Str
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
  (1*sizeof(UsbStandardConfigurationDescriptor_t)+
   1*sizeof(UsbStandardInterfaceDescriptor_t)+
   2*sizeof(UsbStandardEpDescriptor_t)),
  (1*sizeof(UsbStandardConfigurationDescriptor_t)+
   1*sizeof(UsbStandardInterfaceDescriptor_t)+
   2*sizeof(UsbStandardEpDescriptor_t)) >> 8,
  1,
  1,
  0,
  UsbConfigurationCommmonAttr,
  UsbConfigPower_mA(100),
  sizeof(UsbStandardInterfaceDescriptor_t),
  UsbDescriptorInterface,
  0,
  0,
  2,
  UsbDeviceClassStorage,
  MscSubClassScsi,
  MscProtocolBulkOnly,
  0,
  sizeof(UsbStandardEpDescriptor_t),
  UsbDescriptorEp,
  UsbEpIn(BulkInEp>>1),
  UsbEpTransferBulk,
  BulkInEpMaxSize1,
  BulkInEpMaxSize1>>8,
  0,
  sizeof(UsbStandardEpDescriptor_t),
  UsbDescriptorEp,
  UsbEpOut(BulkOutEp>>1),
  UsbEpTransferBulk,
  BulkOutEpMaxSize1,
  BulkOutEpMaxSize1>>8,
  0,
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
