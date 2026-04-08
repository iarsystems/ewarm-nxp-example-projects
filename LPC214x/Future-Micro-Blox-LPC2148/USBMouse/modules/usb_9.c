/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2003
 *
 *    File name   : usb_9.c
 *    Description : USB chapter 9 (device framework) module
 *
 *    History :
 *    1. Date        : August 5, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : December 19, 2005
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Modify user callback function for EP0
 *        Change parameters transferred to function form packet type to
 *        pointer to UsbEp0Ctrl structure
 *    3. Date        : July 6, 2006
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Fix problem with the device deconfiguration after removing of the
 *       USB cable (in the UsbSetDevState function is added calling to
 *       UsbUserConfigure with NULL parameter)
 *
 *    $Revision: 28 $
**************************************************************************/
#include "usb_9.h"

UsbEpCtrl_t UsbEp0Ctrl;
UsbSetupPacket_t UsbEp0SetupPacket;

#pragma data_alignment=4
Int8U EpCtrlDataBuf[Ep0MaxSize];
UsbDevCtrl_t UsbDevCtrl = {{UsbDevStatusUnknow,UsbDevStatusNoSuspend},0,0,0,0};

// User functions
CommUserFpnt_t UsbCoreUserFun[UsbUserLastInd] =
{
  // UsbUserClass
  NULL,
  // UsbUserVendor
  NULL,
  // UsbClassEp0OutPacket
  NULL,
  // UsbVendorEp0OutPacket
  NULL,
  // UsbUserConfigure
  NULL,
  // UsbUserReset
  NULL,
  // UsbUserConnect
  NULL,
  // UsbUserSuspend
  NULL,
};

/*************************************************************************
 * Function Name: UsbCoreInit
 * Parameters:  CommUserFpnt_t UserCoreConfigure
 *
 * Return: none
 *
 * Description: USB init core
 *
 *************************************************************************/
void UsbCoreInit (CommUserFpnt_t UserCoreConfigure)
{
  UsbEp0Ctrl.EpStatus.Status = UsbSetupPacket;
  UserCoreConfigure(NULL);
  UsbCoreUserFuncRegistering(UserCoreConfigure,UsbUserConfigure);
  UsbSetDevState(UsbDevStatusUnknow);
  // Registered Core functions
  USB_UserFuncRegistering(UsbDevResetCallback  ,UsbResetEvent  );
  USB_UserFuncRegistering(UsbDevConnectCallback,UsbConnectEvent);
  USB_UserFuncRegistering(UsbDevSuspendCallback,UsbSuspendEvent);
  USB_UserFuncRegistering(UsbCtrlEpCallback    ,UsbEp0Out      );
  USB_UserFuncRegistering(UsbCtrlEpCallback    ,UsbEp0In       );
}

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
                                          UsbCoreUserFuncInd_t UserFuncInd)
{
CommUserFpnt_t PrevUserFunc = UsbCoreUserFun[UserFuncInd];
  UsbCoreUserFun[UserFuncInd] = UserCoreFunc;
  return(PrevUserFunc);
}

/*************************************************************************
 * Function Name: UsbCoreReq
 * Parameters:  UsbCoreReqType_t Type
 *
 * Return: Int32U
 *
 * Description: Return device states
 *
 *************************************************************************/
Int32U UsbCoreReq (UsbCoreReqType_t Type)
{
  switch(Type)
  {
  case UsbCoreReqDevState:
    return(UsbDevCtrl.State.DS);
  case UsbCoreReqDevSusState:
    return(UsbDevCtrl.State.Suspend);
  case UsbCoreReqConfiquration:
    return(UsbDevCtrl.Configuration);
  case UsbCoreReqInterface:
    return(UsbDevCtrl.Interface);
  case UsbCoreReqAlternateSetting:
    return(UsbDevCtrl.AlternateSetting);
  case UsbCoreReqDevOwnAddress:
    return(UsbDevCtrl.DevAdd);
  case UsbCoreReqWakeUpEnableStatus:
    return(UsbDevCtrl.Feature.RemoteWakeupEnable);
  case UsbCoreReqSelfPoweredStatus:
    return(UsbDevCtrl.Feature.SelfPowered);
  }
  return((Int32U) -1);
}

/*************************************************************************
 * Function Name: UsbWakeUp
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Wake-up device from suspend mode
 *
 *************************************************************************/
void UsbWakeUp (void)
{
  if(UsbDevCtrl.Feature.RemoteWakeupEnable && UsbDevCtrl.State.Suspend)
  {
    USB_WakeUp();
  }
}

/*************************************************************************
 * Function Name: UsbSetDevState
 * Parameters:  _UsbDevState_t DevState
 *
 * Return: none
 *
 * Description: Set device state
 *
 *************************************************************************/
void UsbSetDevState (_UsbDevState_t DevState)
{
  UsbDevCtrl.State.DS = DevState;
  switch (DevState)
  {
  case UsbDevStatusConfigured:
    USB_Configure(TRUE);
    break;
  case UsbDevStatusAddress:
    UsbCoreUserFun[UsbUserConfigure](NULL);
    UsbDevCtrl.Configuration = UsbDevCtrl.AlternateSetting =\
    UsbDevCtrl.Interface = 0;
    USB_Configure(FALSE);
    break;
  default:
    UsbCoreUserFun[UsbUserConfigure](NULL);
    UsbDevCtrl.DevAdd = UsbDevCtrl.Configuration = \
    UsbDevCtrl.AlternateSetting = UsbDevCtrl.Interface = 0;
    USB_Configure(FALSE);
  }
}

/*************************************************************************
 * Function Name: UsbGetDevState
 * Parameters:  none
 *
 * Return: UsbDevState_t
 *
 * Description: Get device state
 *
 *************************************************************************/
UsbDevState_t UsbGetDevState (void)
{
  return(UsbDevCtrl.State);
}

/*************************************************************************
 * Function Name: UsbSetDevSuspend
 * Parameters:  none
 *
 * Return: UsbDevState_t
 *
 * Description: Set device suspend mode
 *
 *************************************************************************/
void UsbSetDevSuspend (Boolean Suspend)
{
  UsbDevCtrl.State.Suspend = Suspend ? UsbDevStatusSuspend : UsbDevStatusNoSuspend;
}

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
                                  UsbDevCtrl_t * UsbDev)
{
UsbStandardInterfaceDescriptor_t *pData;
  // Find Interface
  for(pData = (UsbStandardInterfaceDescriptor_t *)pConfiguration;
      (Int8U*)pData < (Int8U *)pConfiguration + pConfiguration->wTotalLength;
      pData =(UsbStandardInterfaceDescriptor_t *)((Int8U *)pData + pData->bLength))
  {
    if((pData->bDescriptorType == UsbDescriptorInterface) &&
       (pData->bInterfaceNumber == UsbDev->Interface) &&
       (pData->bAlternateSetting == UsbDev->AlternateSetting))
    {
      return(UsbPass);
    }
  }
  return(UsbFault);
}

/*************************************************************************
 * Function Name: UsbFindConfiguration
 * Parameters:  Int32U
 *
 * Return: UsbStandardConfigurationDescriptor_t *
 *
 * Description: Return pointer to current configuration
 *
 *************************************************************************/
UsbStandardConfigurationDescriptor_t * UsbFindConfiguration(Int32U Configuration)
{
UsbStandardConfigurationDescriptor_t * pData =
 (UsbStandardConfigurationDescriptor_t *) &UsbStandardConfigurationDescriptor[0];
  while(pData->bLength)
  {
    if(pData->bDescriptorType == UsbDescriptorConfiguration)
    {
      if(pData->bConfigurationValue == Configuration)
      {
         return(pData);
      }
    }
    pData = (UsbStandardConfigurationDescriptor_t *)((Int8U *)pData + pData->bLength);
  }
  return(NULL);
}

/*************************************************************************
 * Function Name: UsbDevConnectCallback
 * Parameters:  void * pArg
 *
 * Return: none
 *
 * Description: USB connect callback
 *
 *************************************************************************/
void UsbDevConnectCallback (void * pArg)
{
  UsbSetDevState(pArg?UsbDevStatusAttached:UsbDevStatusUnknow);
  if(UsbCoreUserFun[UsbUserConnect] != NULL)
  {
    UsbCoreUserFun[UsbUserConnect](pArg);
  }
}

/*************************************************************************
 * Function Name: UsbDevSuspendCallback
 * Parameters:  void * pArg
 *
 * Return: none
 *
 * Description: USB suspend callback
 *
 *************************************************************************/
void UsbDevSuspendCallback (void * pArg)
{
  UsbSetDevSuspend(pArg?UsbDevStatusSuspend:UsbDevStatusNoSuspend);
  if(UsbCoreUserFun[UsbUserSuspend] != NULL)
  {
    UsbCoreUserFun[UsbUserSuspend](pArg);
  }
}

/*************************************************************************
 * Function Name: UsbDevResetCallback
 * Parameters:  void * pArg
 *
 * Return: none
 *
 * Description: USB reset callback
 *
 *************************************************************************/
void UsbDevResetCallback (void * pArg)
{
  UsbSetDevState(UsbDevStatusDefault);
  UsbCoreUserFun[UsbUserConfigure](NULL);

  if(UsbCoreUserFun[UsbUserReset] != NULL)
  {
    UsbCoreUserFun[UsbUserReset](pArg);
  }
}

/*************************************************************************
 * Function Name: UsbEp0SetupWrite
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB EP Write implement (IN)
 *
 *************************************************************************/
void UsbEp0SetupWrite (void)
{
Int32U CurrCount = Ep0MaxSize;
  if((UsbEp0Ctrl.Counter == Ep0MaxSize) && UsbEp0Ctrl.EpStatus.NoZeroLength)
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpSetupLastIn;
  }
  else if(UsbEp0Ctrl.Counter < Ep0MaxSize)
  {
    CurrCount = UsbEp0Ctrl.Counter;
    UsbEp0Ctrl.EpStatus.Status = UsbEpSetupLastIn;
  }
  else
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpSetupDataIn;
  }

  USB_EpWrite(CTRL_ENP_IN,(Int32U *)UsbEp0Ctrl.pData,CurrCount);
  UsbEp0Ctrl.Counter -= CurrCount;
  UsbEp0Ctrl.pData += CurrCount;
}

/*************************************************************************
 * Function Name: UsbSetConfigurtonState
 * Parameters:  Int32U
 *
 * Return: UsbCommStatus_t
 *
 * Description: Set configuration state
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbSetConfigurtonState (Int32U Configuration)
{
  if(Configuration)
  {
    // Find configuration
    UsbStandardConfigurationDescriptor_t * pData = UsbFindConfiguration(Configuration);
    UsbDevCtrl_t UsbDevCtrlNew = UsbDevCtrl;

    if(pData == NULL)
    {
      return(UsbFault);
    }
    // Set Interface and Alternative Setting
    UsbDevCtrlNew.Configuration = Configuration;
    UsbDevCtrlNew.Interface = UsbDevCtrlNew.AlternateSetting = 0;
    UsbDevCtrlNew.Feature.RemoteWakeupEnable = 0;
    UsbDevCtrlNew.Feature.SelfPowered = pData->bmAttributes.SelfPowered?1:0;
    // Whether the interface is exist into current configuration?
    if(UsbFindInterface(pData,&UsbDevCtrlNew) != UsbPass)
    {
      return(UsbFault);
    }
    // User function that realize EP and init buffers
    UsbCoreUserFun[UsbUserConfigure]((void *)&UsbDevCtrlNew);
    UsbDevCtrl = UsbDevCtrlNew;
    UsbSetDevState(UsbDevStatusConfigured);
  }
  else
  {
    UsbCoreUserFun[UsbUserConfigure](NULL);
    UsbSetDevState(UsbDevStatusAddress);
  }
  return(UsbPass);
}

/*************************************************************************
 * Function Name: UsbDevStatus
 * Parameters:  Int8U * pData, Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: Usb Return device status into pData
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbDevStatus(Int8U * pData, Int16U Index)
{
  if(Index == 0)
  {
    UsbEp0Ctrl.pData = pData;
    UsbEp0Ctrl.Counter = 2;
    *pData++ = UsbDevCtrl.Feature.Data;
    *pData   = 0;
    return(UsbPass);
  }
  return(UsbFault);
}

/*************************************************************************
 * Function Name: UsbInterfaceStatus
 * Parameters:  Int8U * pData, Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Return interface status into pData
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbInterfaceStatus(Int8U * pData, Int16U Index)
{
  UsbEp0Ctrl.pData = pData;
  UsbEp0Ctrl.Counter = 2;
  *pData++ = 0;
  *pData   = 0;
  return(UsbPass);
}

/*************************************************************************
 * Function Name: UsbEpStatus
 * Parameters:  Int8U * pData, Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Return Ep status into pData
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbEpStatus(Int8U * pData, Int16U Index)
{
  if((Index & 0x7F) > 16)
  {
    return(UsbFault);
  }
  UsbEp0Ctrl.pData = pData;
  UsbEp0Ctrl.Counter = 2;
  *pData++ = USB_GetStallEP(USB_EpLogToPhysAdd(Index)) ? 1:0;
  *pData = 0;
  return(UsbPass);
}

/*************************************************************************
 * Function Name: UsbGetStatus
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Get status implement
 *
 *************************************************************************/
inline
void UsbGetStatus (void)
{
  if((UsbDevCtrl.State.DS >= UsbDevStatusAddress) &&
     // Length must be 2
     (UsbEp0SetupPacket.wLength.Word == 2) &&
     // Direction of data from device
     (UsbEp0SetupPacket.mRequestType.Dir == 1) &&
     // wValue must be 0
     (UsbEp0SetupPacket.wValue.Word == 0) )
  {
    // This request is valid only for Address and Configured device's states
    switch(UsbEp0SetupPacket.mRequestType.Recipient)
    {
    case UsbRecipientDevice:
      if(UsbDevStatus(EpCtrlDataBuf,UsbEp0SetupPacket.wIndex.Word) == UsbPass)
      {
        UsbEp0SetupWrite();
      }
      else
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    case UsbRecipientInterface:
      if(UsbInterfaceStatus(EpCtrlDataBuf,UsbEp0SetupPacket.wIndex.Word) == UsbPass)
      {
        UsbEp0SetupWrite();
      }
      else
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    case UsbRecipientEndpoint:
      if(UsbEpStatus(EpCtrlDataBuf,UsbEp0SetupPacket.wIndex.Word) == UsbPass)
      {
        UsbEp0SetupWrite();
      }
      else
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    default:
      UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
    }
  }
  else
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
  }
}

/*************************************************************************
 * Function Name: UsbClearFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Clear Device feature implement
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbClearDevFeature(Int16U Feature,Int16U Index)
{
  if((Index != 0) || (Feature != UsbDeviceRemoteWakeupSelector))
  {
    return(UsbFault);
  }
  UsbDevCtrl.Feature.RemoteWakeupEnable = 0;
  return(UsbPass);
}
/*************************************************************************
 * Function Name: UsbClearInterfaceFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Clear Interface feature implement
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbClearInterfaceFeature(Int16U Feature,Int16U Index)
{
  return(UsbFault);
}

/*************************************************************************
 * Function Name: UsbClearEpFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Clear Ep feature implement
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbClearEpFeature(Int16U Feature,Int16U Index)
{
USB_Endpoint_t UsbEpAdd;
  if(((Index & 0x7F) > 16) || (Feature != UsbEpHaltSelector))
  {
    return(UsbFault);
  }
  UsbEpAdd = USB_EpLogToPhysAdd(Index);
  USB_SetStallEP(UsbEpAdd,FALSE);
  if (UsbUserFun[UsbEpAdd] != NULL)
  {
    UsbUserFun[UsbEpAdd]((void *)UsbEpUnstall);
  }
  return(UsbPass);
}

/*************************************************************************
 * Function Name: UsbClearFeature
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Clear feature implement
 *
 *************************************************************************/
inline
void UsbClearFeature (void)
{
  if((UsbDevCtrl.State.DS >= UsbDevStatusAddress) &&
     // Length must be 0
     (UsbEp0SetupPacket.wLength.Word == 0))
  {
    switch(UsbEp0SetupPacket.mRequestType.Recipient)
    {
    case UsbRecipientDevice:
      if(UsbClearDevFeature(UsbEp0SetupPacket.wValue.Word,UsbEp0SetupPacket.wIndex.Word) == UsbPass)
      {
        USB_EpWrite(CTRL_ENP_IN,NULL,0);
        UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
      }
      else
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    case UsbRecipientInterface:
      if((UsbDevCtrl.State.DS > UsbDevStatusAddress) &&
         (UsbClearInterfaceFeature(UsbEp0SetupPacket.wValue.Word,UsbEp0SetupPacket.wIndex.Word) == UsbPass))
      {
        USB_EpWrite(CTRL_ENP_IN,NULL,0);
        UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
      }
      else
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    case UsbRecipientEndpoint:
      if(((UsbDevCtrl.State.DS > UsbDevStatusAddress) || ((UsbEp0SetupPacket.wIndex.Word & 0x7F) == 0)) &&
         (UsbClearEpFeature(UsbEp0SetupPacket.wValue.Word,UsbEp0SetupPacket.wIndex.Word) == UsbPass))
      {
        USB_EpWrite(CTRL_ENP_IN,NULL,0);
        UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
      }
      else
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    default:
      UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
    }
  }
  else
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
  }
}

/*************************************************************************
 * Function Name: UsbSetFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Set Device feature implement
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbSetDevFeature(Int16U Feature,Int16U Index)
{
  if((Index != 0) || (Feature != UsbDeviceRemoteWakeupSelector))
  {
    return(UsbFault);
  }
  UsbDevCtrl.Feature.RemoteWakeupEnable = 1;
  return(UsbPass);
}
/*************************************************************************
 * Function Name: UsbSetInterfaceFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Set Interface feature implement
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbSetInterfaceFeature(Int16U Feature,Int16U Index)
{
  return(UsbFault);
}

/*************************************************************************
 * Function Name: UsbSetEpFeature
 * Parameters:  Int16U Feature,Int16U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB Set Ep feature implement
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbSetEpFeature(Int16U Feature,Int16U Index)
{
USB_Endpoint_t UsbEpAdd;
  if(((Index & 0x7F) > 16) || (Feature != UsbEpHaltSelector))
  {
    return(UsbFault);
  }
  UsbEpAdd = USB_EpLogToPhysAdd(Index);
  USB_SetStallEP(UsbEpAdd,TRUE);
  if (UsbUserFun[UsbEpAdd] != NULL)
  {
    UsbUserFun[UsbEpAdd]((void *)UsbEpStall);
  }
  return(UsbPass);
}

/*************************************************************************
 * Function Name: UsbSetFeature
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set feature implement
 *
 *************************************************************************/
inline
void UsbSetFeature (void)
{
  if((UsbDevCtrl.State.DS >= UsbDevStatusAddress) &&
     // Length must be 0
     (UsbEp0SetupPacket.wLength.Word == 0))
  {
    switch(UsbEp0SetupPacket.mRequestType.Recipient)
    {
    case UsbRecipientDevice:
      if(UsbSetDevFeature(UsbEp0SetupPacket.wValue.Word,UsbEp0SetupPacket.wIndex.Word) == UsbPass)
      {
        USB_EpWrite(CTRL_ENP_IN,NULL,0);
        UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
      }
      else
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    case UsbRecipientInterface:
      if((UsbDevCtrl.State.DS > UsbDevStatusAddress) &&
         (UsbSetInterfaceFeature(UsbEp0SetupPacket.wValue.Word,UsbEp0SetupPacket.wIndex.Word) == UsbPass))
      {
        USB_EpWrite(CTRL_ENP_IN,NULL,0);
        UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
      }
      else
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    case UsbRecipientEndpoint:
      if(((UsbDevCtrl.State.DS > UsbDevStatusAddress) || ((UsbEp0SetupPacket.wIndex.Lo & 0x7F) == 0)) &&
         (UsbSetEpFeature(UsbEp0SetupPacket.wValue.Word,UsbEp0SetupPacket.wIndex.Word) == UsbPass))
      {
        USB_EpWrite(CTRL_ENP_IN,NULL,0);
        UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
      }
      else
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    default:
      UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
    }
  }
  else
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
  }
}

/*************************************************************************
 * Function Name: UsbSetAddress
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set address request implement
 *
 *************************************************************************/
inline
void UsbSetAddress (void)
{
  if((UsbDevCtrl.State.DS >= UsbDevStatusDefault) &&
     (UsbEp0SetupPacket.mRequestType.Recipient == 0) &&
     (UsbEp0SetupPacket.wIndex.Word  == 0) &&
     (UsbEp0SetupPacket.wLength.Word == 0) &&
     (UsbEp0SetupPacket.wValue.Word < 128))
  {
    USB_EpWrite(CTRL_ENP_IN,NULL,0);
    UsbEp0Ctrl.EpStatus.Status = UsbEpStatusInSetAdd;
  }
  else
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
  }
}

/*************************************************************************
 * Function Name: UsbGetDescriptorDevice
 * Parameters:  Int8U Index - must be 0
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB get device's descriptor request implement
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbGetDescriptorDevice(Int8U Index)
{
  if(Index == 0)
  {
    UsbEp0Ctrl.Counter = UsbStandardDeviceDescriptorStr.bLength;
    UsbEp0Ctrl.EpStatus.NoZeroLength = FALSE;
    if(UsbEp0Ctrl.Counter >= UsbEp0SetupPacket.wLength.Word)
    {
      UsbEp0Ctrl.Counter = UsbEp0SetupPacket.wLength.Word;
      UsbEp0Ctrl.EpStatus.NoZeroLength = TRUE;
    }
    UsbEp0Ctrl.pData = (Int8U *)&UsbStandardDeviceDescriptorStr;
    UsbEp0SetupWrite();
    return(UsbPass);
  }
  return(UsbFault);
}

/*************************************************************************
 * Function Name: UsbGetDescriptorConfiguration
 * Parameters:  Int8U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB get configuration's descriptor request implement
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbGetDescriptorConfiguration(Int8U Index)
{
UsbStandardConfigurationDescriptor_t *pData;
  for(pData = (UsbStandardConfigurationDescriptor_t *)UsbStandardConfigurationDescriptor;
      pData->bLength;
      pData=(UsbStandardConfigurationDescriptor_t*)((Int8U *)pData + pData->bLength))
  {
    if((pData->bDescriptorType == UsbDescriptorConfiguration) &&
       ((UsbStandardDeviceDescriptorStr.bNumConfigurations == 1) ||
       (pData->bConfigurationValue == Index)))
    {
      break;
    }
  }
  if(pData->bLength == 0)
  {
     return(UsbFault);
  }
  UsbEp0Ctrl.Counter = ((UsbStandardConfigurationDescriptor_t*)pData)->wTotalLength;
  UsbEp0Ctrl.EpStatus.NoZeroLength = FALSE;
  if(UsbEp0Ctrl.Counter > UsbEp0SetupPacket.wLength.Word)
  {
    UsbEp0Ctrl.Counter = UsbEp0SetupPacket.wLength.Word;
    UsbEp0Ctrl.EpStatus.NoZeroLength = TRUE;
  }
  UsbEp0Ctrl.pData = (Int8U *)pData;
  UsbEp0SetupWrite();
  return(UsbPass);
}

/*************************************************************************
 * Function Name: UsbGetDescriptorString
 * Parameters:  Int8U Index
 *
 * Return: UsbCommStatus_t
 *
 * Description: USB get string's descriptor request implement
 *
 *************************************************************************/
inline
UsbCommStatus_t UsbGetDescriptorString(Int8U Index)
{
Int8U **pData;
Int32U i;
  if(Index == 0)
  {
    UsbEp0Ctrl.Counter = UsbLanguagesStr[UsbDescLengthOffset];
    UsbEp0Ctrl.EpStatus.NoZeroLength = FALSE;
    if(UsbEp0Ctrl.Counter >= UsbEp0SetupPacket.wLength.Word)
    {
      UsbEp0Ctrl.Counter = UsbEp0SetupPacket.wLength.Word;
      UsbEp0Ctrl.EpStatus.NoZeroLength = TRUE;
    }
    UsbEp0Ctrl.pData = (Int8U *)&UsbLanguagesStr;
    UsbEp0SetupWrite();
    return(UsbPass);
  }
  else
  {
    Int32U LanguagesOffset = 0;
    // Find Languages index
    for(i = 2; i < UsbLanguagesStr[UsbDescLengthOffset]; i+=2)
    {
      if((UsbLanguagesStr[i] == UsbEp0SetupPacket.wIndex.Lo) &&
         (UsbLanguagesStr[i+1] == UsbEp0SetupPacket.wIndex.Hi))
      {
        break;
      }
      ++LanguagesOffset;
    }
    if(i >= UsbLanguagesStr[UsbDescLengthOffset])
    {
      return(UsbFault);
    }
    // Find String index
    for(pData = (Int8U **)UsbString[LanguagesOffset]; pData ; ++pData)
    {
      if(--Index == 0)
      {
        break;
      }
    }
    if(pData == NULL)
    {
      return(UsbFault);
    }
    UsbEp0Ctrl.Counter = *(*pData+UsbDescLengthOffset);
    UsbEp0Ctrl.EpStatus.NoZeroLength = FALSE;
    if(UsbEp0Ctrl.Counter >= UsbEp0SetupPacket.wLength.Word)
    {
      UsbEp0Ctrl.Counter = UsbEp0SetupPacket.wLength.Word;
      UsbEp0Ctrl.EpStatus.NoZeroLength = TRUE;
    }
    UsbEp0Ctrl.pData = *pData;
    UsbEp0SetupWrite();
  }
  return(UsbPass);
}

/*************************************************************************
 * Function Name: UsbGetDescriptor
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB get descriptor request implement
 *
 *************************************************************************/
inline
void UsbGetDescriptor (void)
{
    if(UsbDevCtrl.State.DS >= UsbDevStatusDefault)
    {
      switch(UsbEp0SetupPacket.wValue.Hi)
      {
      case UsbDescriptorDevice:
        if(UsbGetDescriptorDevice(UsbEp0SetupPacket.wValue.Lo) != UsbPass)
        {
          UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
        }
        break;
      case UsbDescriptorConfiguration:
        if(UsbGetDescriptorConfiguration(UsbEp0SetupPacket.wValue.Lo) != UsbPass)
        {
          UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
        }
        break;
      case UsbDescriptorString:
        if(UsbGetDescriptorString(UsbEp0SetupPacket.wValue.Lo) != UsbPass)
        {
          UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
        }
        break;
      default:
        if ((UsbCoreUserFun[UsbUserGetDescriptor] == NULL) ||
            ((Int32U)UsbCoreUserFun[UsbUserGetDescriptor](&UsbEp0Ctrl) != UsbPass))
        {
          UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
        }
      }
    }
    else
    {
      UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
    }
}

/*************************************************************************
 * Function Name: UsbSetDescriptor
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set descriptor
 *
 *************************************************************************/
inline
void UsbSetDescriptor(void)
{
  if(UsbDevCtrl.State.DS >= UsbDevStatusAddress)
  {
    // This request is valid only for Address state and Configured state.
    // If supported
    UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
  }
  else
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
  }
}

/*************************************************************************
 * Function Name: UsbGetConfiguration
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Get configuration implement
 *
 *************************************************************************/
inline
void UsbGetConfiguration(void)
{
  if((UsbDevCtrl.State.DS >= UsbDevStatusAddress) &&
     (UsbEp0SetupPacket.wValue.Word == 0) &&
     (UsbEp0SetupPacket.wIndex.Word == 0) &&
     (UsbEp0SetupPacket.wLength.Word == 1))
  {
    UsbEp0Ctrl.Counter = 1;
    EpCtrlDataBuf[0] = UsbCoreReq(UsbCoreReqConfiquration);
    UsbEp0Ctrl.pData = EpCtrlDataBuf;
    UsbEp0SetupWrite();
  }
  else
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
  }
}

/*************************************************************************
 * Function Name: UsbSetConfiguration
 * Parameters:  none
 *
 * Return: none
 *
 * Description: USB Set configuration implement
 *
 *************************************************************************/
inline
void UsbSetConfiguration(void)
{
  if((UsbDevCtrl.State.DS >= UsbDevStatusAddress) &&
     (UsbEp0SetupPacket.wIndex.Word == 0) &&
     (UsbEp0SetupPacket.wLength.Word == 0) &&
     (UsbSetConfigurtonState(UsbEp0SetupPacket.wValue.Lo) == UsbPass))
  {
    USB_EpWrite(CTRL_ENP_IN,NULL,0);
    UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
  }
  else
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
  }
}

/*************************************************************************
 * Function Name: UsbGetInterface
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Implement get interface request
 *
 *************************************************************************/
inline
void UsbGetInterface (void)
{
  if((UsbDevCtrl.State.DS == UsbDevStatusConfigured) &&
     (UsbEp0SetupPacket.wIndex.Word == UsbDevCtrl.Interface) &&
     (UsbEp0SetupPacket.wLength.Word == 1) &&
     (UsbEp0SetupPacket.wValue.Word == 0))
  {
    UsbEp0Ctrl.Counter = 1;
    EpCtrlDataBuf[0] = UsbDevCtrl.AlternateSetting;
    UsbEp0Ctrl.pData = EpCtrlDataBuf;
    UsbEp0SetupWrite();
  }
  else
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
  }
}

/*************************************************************************
 * Function Name: UsbSetInterface
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Implement set interface request
 *
 *************************************************************************/
inline
void UsbSetInterface (void)
{
  if((UsbDevCtrl.State.DS == UsbDevStatusConfigured) &&
     (UsbEp0SetupPacket.wLength.Word == 0))
  {
    UsbStandardConfigurationDescriptor_t * pData = UsbFindConfiguration(UsbDevCtrl.Configuration);
    UsbDevCtrl_t UsbDevCtrlNew = UsbDevCtrl;

    UsbDevCtrlNew.Interface = UsbEp0SetupPacket.wIndex.Word;
    UsbDevCtrlNew.AlternateSetting = UsbEp0SetupPacket.wValue.Word;
    UsbDevCtrlNew.Feature.RemoteWakeupEnable = 0;

    // Whether the interface is exist into current configuration?
    if(UsbFindInterface(pData,&UsbDevCtrlNew) != UsbPass)
    {
      UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      return;
    }

    UsbCoreUserFun[UsbUserConfigure]((void *)&UsbDevCtrlNew);
    UsbDevCtrl = UsbDevCtrlNew;

    USB_EpWrite(CTRL_ENP_IN,NULL,0);
    UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
  }
  else
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
  }
}

/*************************************************************************
 * Function Name: UsbSynchFrame
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Implement synch frame request
 *
 *************************************************************************/
inline
void UsbSynchFrame (void)
{
  if( USB_SOF_EVENT &&
     (UsbDevCtrl.State.DS == UsbDevStatusConfigured) &&
     (UsbEp0SetupPacket.wLength.Word == 2) &&
     (UsbEp0SetupPacket.wValue.Word == 0))
  {
  Int32U FrameNumb = 0;
  #if USB_SOF_EVENT > 0
    FrameNumb = USB_GetFrameNumb();
  #endif
    UsbEp0Ctrl.Counter = 2;
    EpCtrlDataBuf[0]   = FrameNumb;
    EpCtrlDataBuf[1]   = FrameNumb>>8;
    UsbEp0Ctrl.pData   = EpCtrlDataBuf;
    UsbEp0SetupWrite();
  }
  else
  {
    UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
  }
}

/*************************************************************************
 * Function Name: UsbCtrlEpCallback
 * Parameters:  void * pArg
 *
 * Return: none
 *
 * Description: USB Ctrl EP Callback
 *
 *************************************************************************/
void UsbCtrlEpCallback (void * pArg)
{
Int32U PacketSize;

  switch((Int32U)pArg)
  {
  case UsbSetupPacket:
setup_packet_overwrited:
    UsbEp0Ctrl.pData   = EpCtrlDataBuf;
    UsbEp0Ctrl.Counter = USB_EpRead(CTRL_ENP_OUT,(Int32U *)EpCtrlDataBuf,Ep0MaxSize);
    if(UsbEp0Ctrl.EpStatus.Status != UsbEpPause)
    {
      // Copy new setup packet int setup buffer
      memcpy((Int8U *)&UsbEp0SetupPacket,EpCtrlDataBuf,sizeof(UsbEp0SetupPacket));
    }
    switch(UsbEp0SetupPacket.mRequestType.Type)
    {
    // Standard
    case UsbTypeStandart:
      // Decoding standard request
      switch (UsbEp0SetupPacket.bRequest)
      {
      case GET_STATUS:
        UsbGetStatus();
        break;
      case CLEAR_FEATURE:
        UsbClearFeature();
        break;
      case SET_FEATURE:
        UsbSetFeature();
        break;
      case SET_ADDRESS:
        UsbSetAddress();
        break;
      case GET_DESCRIPTOR:
        if(UsbEp0SetupPacket.mRequestType.Recipient == UsbRecipientDevice)
        {
          UsbGetDescriptor();
        }
        // Only get descriptor for device is standard request
        else if ((UsbCoreUserFun[UsbUserGetDescriptor] != NULL) &&
                 ((Int32U)UsbCoreUserFun[UsbUserGetDescriptor](&UsbEp0Ctrl) == UsbUserSendPacket))
        {
          if(UsbEp0Ctrl.Counter >= UsbEp0SetupPacket.wLength.Word)
          {
            UsbEp0Ctrl.Counter = UsbEp0SetupPacket.wLength.Word;
            UsbEp0Ctrl.EpStatus.NoZeroLength = TRUE;
          }
          else
          {
             UsbEp0Ctrl.EpStatus.NoZeroLength = FALSE;
          }
          UsbEp0SetupWrite();
        }
        else
        {
          UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
        }
        break;
      case SET_DESCRIPTOR:
        // Optional (only for configuration and string descriptors)
        UsbSetDescriptor();
        break;
      case GET_CONFIGURATION:
        UsbGetConfiguration();
        break;
      case SET_CONFIGURATION:
        UsbSetConfiguration();
        break;
      case GET_INTERFACE:
        UsbGetInterface();
        break;
      case SET_INTERFACE:
        UsbSetInterface();
        break;
      case SYNCH_FRAME:
        UsbSynchFrame();
        break;
      default:
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    // Class
    case UsbTypeClass:
      if(UsbCoreUserFun[UsbUserClass] != NULL)
      {
        switch((Int32U)UsbCoreUserFun[UsbUserClass](&UsbEp0Ctrl))
        {
        case UsbUserNone:
          break;
        case UsbUserSendPacket:
          if(UsbEp0Ctrl.Counter >= UsbEp0SetupPacket.wLength.Word)
          {
            UsbEp0Ctrl.Counter = UsbEp0SetupPacket.wLength.Word;
            UsbEp0Ctrl.EpStatus.NoZeroLength = TRUE;
          }
          else
          {
            UsbEp0Ctrl.EpStatus.NoZeroLength = FALSE;
          }
          UsbEp0SetupWrite();
          break;
        case UsbUserReceivePacket:
          if((UsbEp0Ctrl.Counter == UsbEp0SetupPacket.wLength.Word) &&
             (UsbCoreUserFun[UsbClassEp0OutPacket] != NULL))
          {
            UsbEp0Ctrl.EpStatus.Status = UsbEpClassDataOut;
          }
          else
          {
            UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
          }
          break;
        case UsbUserSendAckn:
          USB_EpWrite(CTRL_ENP_IN,NULL,0);
          UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
          break;
        default :
          UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
        }
      }
      else
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    // Vendor
    case UsbTypeVendor:
      if(UsbCoreUserFun[UsbUserVendor] != NULL)
      {
        switch((Int32U)UsbCoreUserFun[UsbUserClass](&UsbEp0Ctrl))
        {
        case UsbUserNone:
          break;
        case UsbUserSendPacket:
          if(UsbEp0Ctrl.Counter >= UsbEp0SetupPacket.wLength.Word)
          {
            UsbEp0Ctrl.Counter = UsbEp0SetupPacket.wLength.Word;
            UsbEp0Ctrl.EpStatus.NoZeroLength = TRUE;
          }
          else
          {
            UsbEp0Ctrl.EpStatus.NoZeroLength = FALSE;
          }
          UsbEp0SetupWrite();
          break;
        case UsbUserReceivePacket:
          if((UsbEp0Ctrl.Counter == UsbEp0SetupPacket.wLength.Word) &&
             (UsbCoreUserFun[UsbVendorEp0OutPacket] != NULL))
          {
            UsbEp0Ctrl.EpStatus.Status = UsbEpVendorDataOut;
          }
          else
          {
            UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
          }
          break;
        case UsbUserSendAckn:
          USB_EpWrite(CTRL_ENP_IN,NULL,0);
          UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
          break;
        default :
          UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
        }
      }
      else
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    // Other
    default:
      UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
    }
    break;
  // Out Packet
  case UsbDataOutPacket:
    switch(UsbEp0Ctrl.EpStatus.Status)
    {
    case UsbEpSetupDataIn:
    case UsbEpSetupLastIn:
      // Clear output EP buffer
      UsbEp0Ctrl.EpStatus.Status = UsbEpStatusOut;
      UsbEp0Ctrl.Counter = USB_EpRead(CTRL_ENP_OUT,(Int32U *)EpCtrlDataBuf,Ep0MaxSize);
      if (UsbEp0Ctrl.Counter & 0x80000000)
      {
         goto setup_packet_overwrited;
      }
      break;
    case UsbEpWaitStatusOut:
      // Clear output EP buffer
      UsbEp0Ctrl.EpStatus.Status = UsbSetupPacket;
      UsbEp0Ctrl.Counter = USB_EpRead(CTRL_ENP_OUT,(Int32U *)EpCtrlDataBuf,Ep0MaxSize);
      if (UsbEp0Ctrl.Counter & 0x80000000)
      {
         goto setup_packet_overwrited;
      }
      break;
    case UsbEpSetupDataOut:
      UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      break;
    case UsbEpClassDataOut:
      PacketSize = USB_EpRead(CTRL_ENP_OUT,(Int32U *)UsbEp0Ctrl.pData,Ep0MaxSize);
      if (UsbEp0Ctrl.Counter & 0x80000000)
      {
         goto setup_packet_overwrited;
      }
      UsbEp0Ctrl.pData += PacketSize;
      UsbEp0Ctrl.Counter -= PacketSize;
      if(UsbEp0Ctrl.Counter == 0)
      {
        switch((Int32U)UsbCoreUserFun[UsbClassEp0OutPacket](NULL))
        {
        case UsbUserNone:
          UsbEp0Ctrl.EpStatus.Status = UsbSetupPacket;
          break;
        case UsbUserSendAckn:
          USB_EpWrite(CTRL_ENP_IN,NULL,0);
          UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
          break;
        default:
          UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
        }
      }
      else if (PacketSize != Ep0MaxSize)
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    case UsbEpVendorDataOut:
      PacketSize = USB_EpRead(CTRL_ENP_OUT,(Int32U *)UsbEp0Ctrl.pData,Ep0MaxSize);
      if (UsbEp0Ctrl.Counter & 0x80000000)
      {
         goto setup_packet_overwrited;
      }
      UsbEp0Ctrl.pData += PacketSize;
      UsbEp0Ctrl.Counter -= PacketSize;
      if(UsbEp0Ctrl.Counter == 0)
      {
        switch((Int32U)UsbCoreUserFun[UsbVendorEp0OutPacket](NULL))
        {
        case UsbUserNone:
          UsbEp0Ctrl.EpStatus.Status = UsbSetupPacket;
          break;
        case UsbUserSendAckn:
          USB_EpWrite(CTRL_ENP_IN,NULL,0);
          UsbEp0Ctrl.EpStatus.Status = UsbEpStatusIn;
          break;
        default:
          UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
        }
      }
      else if (PacketSize != Ep0MaxSize)
      {
        UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
      }
      break;
    default:
      UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
    }
    break;
  // In Packet
  case UsbDataInPacket:
    switch(UsbEp0Ctrl.EpStatus.Status)
    {
    case UsbEpStatusOut:
    case UsbEpWaitStatusIn:
    case UsbEpStatusIn:
      UsbEp0Ctrl.EpStatus.Status = UsbSetupPacket;
      break;
    case UsbEpSetupDataIn:
      UsbEp0SetupWrite();
      break;
    case UsbEpSetupLastIn:
      UsbEp0Ctrl.EpStatus.Status = UsbEpWaitStatusOut;
      break;
    case UsbEpStatusInSetAdd:
      USB_SetAdd(UsbEp0SetupPacket.wValue.Lo);
      UsbDevCtrl.DevAdd = UsbEp0SetupPacket.wValue.Lo;
      if(UsbEp0SetupPacket.wValue.Lo)
      {
        UsbSetDevState(UsbDevStatusAddress);
        UsbEp0Ctrl.EpStatus.Status = UsbSetupPacket;
      }
      else
      {
        UsbSetDevState(UsbDevStatusDefault);
        UsbCoreUserFun[UsbUserConfigure](NULL);
      }
      break;
    default:
      UsbEp0Ctrl.EpStatus.Status = UsbEpStall;
    }
    break;
  }
  if (UsbEp0Ctrl.EpStatus.Status == UsbEpStall)
  {
    USB_SetStallEP(CTRL_ENP_OUT,TRUE);
    USB_SetStallEP(CTRL_ENP_IN ,TRUE);
    UsbEp0Ctrl.EpStatus.Status = UsbSetupPacket;
  }
}
