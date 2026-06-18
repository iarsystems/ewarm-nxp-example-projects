/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : usb_9.h
 *    Description : Define usb chapter 9 (device framework) module
 *
 *    History :
 *    1. Date        : August 5, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : December 19, 2005
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Add separate functions for In and Out Data transfer by EP0
 *        Change parameters transferred to function form packet type to
 *        pointer to UsbEp0Ctrl structure
 *
 *    $Revision: 28 $
**************************************************************************/
#include "includes.h"

#ifndef __USB_9_H
#define __USB_9_H

typedef enum
{
  UsbPass = 0, UsbFault, UsbNotSupport,
} UsbCommStatus_t;

typedef enum
{
  UsbDevStatusUnknow = 0, UsbDevStatusAttached, UsbDevStatusPowered,
  UsbDevStatusDefault, UsbDevStatusAddress, UsbDevStatusConfigured
} _UsbDevState_t;

typedef enum
{
  UsbSetupPacket = 0,UsbDataOutPacket,UsbDataInPacket,
  UsbDmaPacket,
  UsbEpStall,UsbEpUnstall,UsbEpPause,
  UsbEpSetupDataOut,
  UsbEpClassDataOut,
  UsbEpVendorDataOut,
  UsbEpSetupDataIn,UsbEpSetupLastIn,
  UsbEpWaitStatusOut,UsbEpStatusOut,
  UsbEpWaitStatusIn,UsbEpStatusIn,UsbEpStatusInSetAdd,
} _UsbEpStaus_t;

typedef enum
{
  UsbTypeStandart = 0,UsbTypeClass,UsbTypeVendor,UsbTypeReserved
} UsbSetupType_t;

typedef enum
{
  GET_STATUS = 0,CLEAR_FEATURE, Reserved0, SET_FEATURE, Reserved1,
  SET_ADDRESS, GET_DESCRIPTOR, SET_DESCRIPTOR, GET_CONFIGURATION,
  SET_CONFIGURATION, GET_INTERFACE, SET_INTERFACE, SYNCH_FRAME
} UsbSetupStandatRecuest_t;

typedef enum
{
  UsbRecipientDevice = 0,UsbRecipientInterface,UsbRecipientEndpoint,
  UsbRecipientOther,UsbRecipientReserved
} UsbSetupRecipient_t;

typedef enum
{
  UsbEpHaltSelector= 0, UsbDeviceRemoteWakeupSelector, UsbTestModeSelector
} UsbFeatureSelector_t;

typedef enum
{
  UsbUserClass,UsbUserVendor,UsbUserGetDescriptor,UsbClassEp0OutPacket,UsbVendorEp0OutPacket,
  UsbUserConfigure,UsbUserReset,UsbUserConnect,UsbUserSuspend,
  UsbUserLastInd,
} UsbCoreUserFuncInd_t;

typedef enum
{
  UsbCoreReqDevState = 0, UsbCoreReqDevSusState, UsbCoreReqConfiquration,
  UsbCoreReqInterface, UsbCoreReqAlternateSetting, UsbCoreReqDevOwnAddress,
  UsbCoreReqWakeUpEnableStatus, UsbCoreReqSelfPoweredStatus,
} UsbCoreReqType_t;

typedef enum
{
  UsbUserNone = 0, UsbUserSendPacket, UsbUserReceivePacket, UsbUserSendAckn,
  UsbUserStallCtrlEp,
} UsbUserFuncResponse_t;

#define UsbDevStatusNoSuspend 0
#define UsbDevStatusSuspend   1

typedef struct
{
  Int8U DS          : 3;
  Int8U             : 4;
  Int8U Suspend     : 1;
} UsbDevState_t;

typedef union
{
  Int8U mRequestTypeData;
  struct
  {
    Int8U Recipient : 5;
    Int8U Type      : 2;
    Int8U Dir       : 1;
  };
}UsbRequestType_t;

typedef union
{
  Int16U Word;
  struct
  {
    Int8U Lo;
    Int8U Hi;
  };
}TwoBytes_t;

typedef struct
{
  UsbRequestType_t  mRequestType;
  Int8U             bRequest;
  TwoBytes_t        wValue;
  TwoBytes_t        wIndex;
  TwoBytes_t        wLength;
} UsbSetupPacket_t;

typedef union
{
  Int8U Data;
  struct
  {
    Int8U SelfPowered         :1;
    Int8U RemoteWakeupEnable  :1;
    Int8U                     :6;
  };
} UsbDefFeature_t;

typedef struct
{
  UsbDevState_t     State;
  Int32U            Configuration;
  Int32U            Interface;
  Int32U            AlternateSetting;
  Int32U            DevAdd;
  UsbDefFeature_t   Feature;
} UsbDevCtrl_t;

typedef struct
{
  _UsbEpStaus_t     Status;
  Boolean           NoZeroLength;
} UsbEpSataus_t;

typedef struct
{
  Int8U *pData;
  Int32U Counter;
  UsbEpSataus_t EpStatus;
}UsbEpCtrl_t;


/*************************************************************************
 * Function Name: UsbCoreInit
 * Parameters:  CommUserFpnt_t UserCoreConfigure
 *
 * Return: none
 *
 * Description: USB init core
 *
 *************************************************************************/
void UsbCoreInit (CommUserFpnt_t UserCoreConfigure);

/*************************************************************************
 * Function Name: UsbCoreUserFuncRegistering
 * Parameters: CommUserFpnt_t UserFunc, UsbCoreUserFuncInd_t UserFuncInd
 *
 * Return: CommUserFpnt_t
 *
 * Description: Registering User core callback function
 *
 *************************************************************************/
CommUserFpnt_t UsbCoreUserFuncRegistering (CommUserFpnt_t UserCoreFunc,
                                          UsbCoreUserFuncInd_t UserFuncInd);

/*************************************************************************
 * Function Name: UsbCoreReq
 * Parameters:  UsbCoreReqType_t Type
 *
 * Return: Int32U
 *
 * Description: Return device states
 *
 *************************************************************************/
Int32U UsbCoreReq (UsbCoreReqType_t Type);

/*************************************************************************
 * Function Name: UsbWakeUp
 * Parameters:  none
 *
 * Return: none
 *
 * Description: WakeUp device from suspend mode
 *
 *************************************************************************/
void UsbWakeUp (void);

/*************************************************************************
 * Function Name: UsbSetDevState
 * Parameters:  _UsbDevState_t DevState
 *
 * Return: none
 *
 * Description: Set device state
 *
 *************************************************************************/
void UsbSetDevState (_UsbDevState_t DevState);

/*************************************************************************
 * Function Name: UsbGetDevState
 * Parameters:  none
 *
 * Return: UsbDevState_t
 *
 * Description: Get device state
 *
 *************************************************************************/
UsbDevState_t UsbGetDevState (void);

/*************************************************************************
 * Function Name: UsbSetDevSuspend
 * Parameters:  none
 *
 * Return: UsbDevState_t
 *
 * Description: Set device suspend mode
 *
 *************************************************************************/
void UsbSetDevSuspend (Boolean Suspend);

/*************************************************************************
 * Function Name: UsbFindInterface
 * Parameters:  UsbStandardConfigurationDescriptor_t * pConfiguration
 *              Int32U Interface,Int32U AlternativeSetting
 *
 * Return: UsbCommStatus_t
 *
 * Description: Set Interface and  Alternative setting state
 *
 *************************************************************************/
UsbCommStatus_t UsbFindInterface (UsbStandardConfigurationDescriptor_t * pConfiguration,
                                  UsbDevCtrl_t * UsbDev);

/*************************************************************************
 * Function Name: UsbFindConfiguration
 * Parameters:  Int32U
 *
 * Return: UsbStandardConfigurationDescriptor_t *
 *
 * Description: Return pointer to current configuration
 *
 *************************************************************************/
UsbStandardConfigurationDescriptor_t * UsbFindConfiguration(Int32U Configuration);

/*************************************************************************
 * Function Name: UsbDevConnectCallback
 * Parameters:  void * pArg
 *
 * Return: none
 *
 * Description: USB connect callback
 *
 *************************************************************************/
void UsbDevConnectCallback (void * pArg);

/*************************************************************************
 * Function Name: UsbDevSuspendCallback
 * Parameters:  void * pArg
 *
 * Return: none
 *
 * Description: USB suspend callback
 *
 *************************************************************************/
void UsbDevSuspendCallback (void * pArg);

/*************************************************************************
 * Function Name: UsbDevResetCallback
 * Parameters:  void * pArg
 *
 * Return: none
 *
 * Description: USB reset callback
 *
 *************************************************************************/
void UsbDevResetCallback (void * pArg);

/*************************************************************************
 * Function Name: UsbEp0SetupWrite
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB EP Write implement (IN)
 *
 *************************************************************************/
void UsbEp0SetupWrite (void);

/*************************************************************************
 * Function Name: UsbSetConfigurtonState
 * Parameters:  Int32U
 *
 * Return: UsbCommStatus_t
 *
 * Description: Set configuration state
 *
 *************************************************************************/
inline UsbCommStatus_t UsbSetConfigurtonState (Int32U Configuration);

/*************************************************************************
 * Function Name: UsbDevStatus
 * Parameters:  Int8U * pData, Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Return device status into pData
 *
 *************************************************************************/
inline UsbCommStatus_t UsbDevStatus(Int8U * pData, Int16U Index);

/*************************************************************************
 * Function Name: UsbInterfaceStatus
 * Parameters:  Int8U * pData, Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Return interface status into pData
 *
 *************************************************************************/
inline UsbCommStatus_t UsbInterfaceStatus(Int8U * pData, Int16U Index);

/*************************************************************************
 * Function Name: UsbEpStatus
 * Parameters:  Int8U * pData, Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Return Ep status into pData
 *
 *************************************************************************/
inline UsbCommStatus_t UsbEpStatus(Int8U * pData, Int16U Index);

/*************************************************************************
 * Function Name: UsbGetStatus
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Get status implement
 *
 *************************************************************************/
inline void UsbGetStatus (void);

/*************************************************************************
 * Function Name: UsbClearFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Clear Device feature implement
 *
 *************************************************************************/
inline UsbCommStatus_t UsbClearDevFeature(Int16U Feature,Int16U Index);

/*************************************************************************
 * Function Name: UsbClearInterfaceFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Clear Interface feature implement
 *
 *************************************************************************/
inline UsbCommStatus_t UsbClearInterfaceFeature(Int16U Feature,Int16U Index);

/*************************************************************************
 * Function Name: UsbClearEpFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Clear Ep feature implement
 *
 *************************************************************************/
inline UsbCommStatus_t UsbClearEpFeature(Int16U Feature,Int16U Index);

/*************************************************************************
 * Function Name: UsbClearFeature
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Clear feature implement
 *
 *************************************************************************/
inline void UsbClearFeature (void);

/*************************************************************************
 * Function Name: UsbSetFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Set Device feature implement
 *
 *************************************************************************/
inline UsbCommStatus_t UsbSetDevFeature(Int16U Feature,Int16U Index);

/*************************************************************************
 * Function Name: UsbSetInterfaceFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Set Interface feature implement
 *
 *************************************************************************/
inline UsbCommStatus_t UsbSetInterfaceFeature(Int16U Feature,Int16U Index);

/*************************************************************************
 * Function Name: UsbSetEpFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Set Ep feature implement
 *
 *************************************************************************/
inline UsbCommStatus_t UsbSetEpFeature(Int16U Feature,Int16U Index);

/*************************************************************************
 * Function Name: UsbSetFeature
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set feature implement
 *
 *************************************************************************/
inline void UsbSetFeature (void);

/*************************************************************************
 * Function Name: UsbSetAddress
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set address request implement
 *
 *************************************************************************/
inline void UsbSetAddress (void);

/*************************************************************************
 * Function Name: UsbGetDescriptorDevice
 * Parameters:  Int8U Index - must be 0
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB get device's descriptor request implement
 *
 *************************************************************************/
inline UsbCommStatus_t UsbGetDescriptorDevice(Int8U Index);

/*************************************************************************
 * Function Name: UsbGetDescriptorConfiguration
 * Parameters:  Int8U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB get configuration's descriptor request implement
 *
 *************************************************************************/
inline UsbCommStatus_t UsbGetDescriptorConfiguration(Int8U Index);

/*************************************************************************
 * Function Name: UsbGetDescriptorString
 * Parameters:  Int8U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB get string's descriptor request implement
 *
 *************************************************************************/
inline UsbCommStatus_t UsbGetDescriptorString(Int8U Index);

/*************************************************************************
 * Function Name: UsbGetDescriptor
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB get descriptor request implement
 *
 *************************************************************************/
inline void UsbGetDescriptor (void);

/*************************************************************************
 * Function Name: UsbSetDescriptor
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set descriptor
 *
 *************************************************************************/
inline void UsbSetDescriptor(void);

/*************************************************************************
 * Function Name: UsbGetConfiguration
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Get configuration implement
 *
 *************************************************************************/
inline void UsbGetConfiguration(void);

/*************************************************************************
 * Function Name: UsbSetConfiguration
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set configuration implement
 *
 *************************************************************************/
inline void UsbSetConfiguration(void);

/*************************************************************************
 * Function Name: UsbGetInterface
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Implement get interface request
 *
 *************************************************************************/
inline void UsbGetInterface (void);

/*************************************************************************
 * Function Name: UsbSetInterface
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Implement set interface request
 *
 *************************************************************************/
inline void UsbSetInterface (void);

/*************************************************************************
 * Function Name: UsbSynchFrame
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Implement synch frame request
 *
 *************************************************************************/
inline void UsbSynchFrame (void);

/*************************************************************************
 * Function Name: UsbCtrlEpCallback
 * Parameters:  void * pArg
 *
 * Return: none
 *
 * Description: USB Ctrl EP Callback
 *
 *************************************************************************/
void UsbCtrlEpCallback (void * pArg);

#endif //__USB_9_H
