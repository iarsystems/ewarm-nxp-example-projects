/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2003
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

#pragma data_alignment=4
const UsbStandardDeviceDescriptorStr_t UsbStandardDeviceDescriptorStr =
{
  sizeof(UsbStandardDeviceDescriptorStr_t),
  UsbDescriptorDevice,
  0x200,
  0,
  0,
  0,
  Ep0MaxSize,
  0xFFFF,
  0x0001,
  0x0000,
  iManufacturerStr,
  iProductStr,
  iSerialNumberStr,
  1,
};

#pragma data_alignment=4
const Int16U mouseDescriptor[] =
{
  0x0105, // Usage Page (Generic Desktop)
  0x0209, // Usage (Mouse)
  0x01A1, // Collection (Application)
  0x0109, //  Usage (Pointer)
  0x00A1, //  Collection (Physical)
  0x0905, //    Usage Page (Buttons)
  0x0119, //    Usage Minimumù (01)
  0x0329, //    Usage Maximum (03)
  0x0015, //    Logical Minimum (0)
  0x0125, //    Logical Maximum (1)
  0x0395, //    Report Count (3)
  0x0175, //    Report Size (1)
  0x0281, //    3 Button bits
  0x0195, //    Report Count (1)
  0x0575, //    Report Size (6)
  0x0181, //    6 bit padding
  0x0105, //    Generic desktop
  0x3009, //    Usage(X)
  0x3109, //    Usage(Y)
  0x8115, //    Logical Minimum (-127)
  0x7F25, //    Logical Maximum (127)
  0x0875, //    Report Size (8)
  0x0295, //    Report Count (2)
  0x0681, //    2 position bytes
  0xC0C0  // End collection x2
};

#pragma data_alignment=4
const Int8U UsbHidDescriptor[] =
{
  sizeof(UsbHidDescriptor_t),
  Hid,
  _2BL(0x0111),
  0,
  1,
  HidReport,
  _2BL(sizeof(mouseDescriptor)),
};

#pragma data_alignment=4
const Int8U UsbStandardConfigurationDescriptor[] =
{
  sizeof(UsbStandardConfigurationDescriptor_t),
  UsbDescriptorConfiguration,
  _2BL(sizeof(UsbStandardConfigurationDescriptor_t)+
   sizeof(UsbStandardInterfaceDescriptor_t)+
   sizeof(UsbHidDescriptor_t)+
   sizeof(UsbStandardEpDescriptor_t)),
  1,
  1,
  0,
  UsbConfigurationCommmonAttr,
  UsbConfigPower_mA(100),

  sizeof(UsbStandardInterfaceDescriptor_t),
  UsbDescriptorInterface,
  0,
  0,
  1,
  UsbDeviceClassHumanInterface,
  NoSubclass,
  Mouse,
  0,

  sizeof(UsbHidDescriptor_t),
  Hid,
  _2BL(0x0111),
  0,
  1,
  HidReport,
  _2BL(sizeof(mouseDescriptor)),

  sizeof(UsbStandardEpDescriptor_t),
  UsbDescriptorEp,
  UsbEpIn(ReportEp>>1),
  UsbEpTransferInterrupt,
  _2BL(ReportEpMaxSize),
  10,

  0,
};

#pragma data_alignment=4
const Int8U UsbLanguagesStr [] =
{
  // Length of LanguagesStr + 2
  4,
  // Desciptor Type
  UsbDescriptorString,
  // Languages ID
  // Languages1 ID Englich
  0x09,0x04,
};

#pragma data_alignment=4
const Int8U ManufacturerStrLan1 [] =
{
  22, // length
  UsbDescriptorString,  // Descriptor
  'I',0,'A',0,'R',0,' ',0,'S',0,'y',0,'s',0,'t',0,'e',0,'m',0, // Str
};

#pragma data_alignment=4
const Int8U ProductStrLan1 [] =
{
  98, //length
  UsbDescriptorString, // Descriptor
  'I',0,'A',0,'R',0,' ',0,'E',0,'m',0,'b',0,'e',0,'d',0,'d',0,'e',0,
  'd',0,' ',0,'W',0,'o',0,'r',0,'k',0,'b',0,'e',0,'n',0,'c',0,'h',0,
  ' ',0,'A',0,'R',0,'M',0,' ',0,'-',0,' ',0,'H',0,'I',0,'D',0,' ',0,
  'D',0,'e',0,'v',0,'i',0,'c',0,'e',0,' ',0,'e',0,'x',0,'a',0,'m',0,
  'p',0,'l',0,'e',0,'!',0
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

const Int8U * const * const UsbString[] =
{
  UsbLanguages1Strings,
};

