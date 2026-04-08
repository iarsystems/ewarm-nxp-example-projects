/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2003
 *
 *    File name   : lpc_usb.c
 *    Description : usb module (HAL)
 *
 *    History :
 *    1. Date        : June 13, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : August 4, 2005
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Modify some functions
 *    3. Date        : November 18, 2005
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Add DMA support
 *    4. Date        : December 9, 2005
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Fix DMA restart problem for IN EPs
 *    5. Date        : December 20, 2005
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Change user function prototype
 *    $Revision: 28 $
**************************************************************************/

#define LPC_USB_GLOBAL
#include "lpc_usb.h"

UsbDevStat_t USB_DevStatus;
UserFunc_t UsbUserFun [UsbLastEvent] =
{
  // Ep 0 Out
  NULL,
  // Ep 0 In
  NULL,
  // Ep 1 Out
  NULL,
  // EP 1 In
  NULL,
  // EP 2 Out
  NULL,
  // EP 2 Int
  NULL,
  // EP 3 Out
  NULL,
  // EP 3 In
  NULL,
  // EP 4 Out
  NULL,
  // EP 4 In
  NULL,
  // EP 5 Out
  NULL,
  // EP 5 In
  NULL,
  // EP 6 Out
  NULL,
  // EP 6 In
  NULL,
  // EP 7 Out
  NULL,
  // EP 7 In
  NULL,
  // EP 8 Out
  NULL,
  // EP 8 In
  NULL,
  // EP 9 Out
  NULL,
  // EP 9 In
  NULL,
  // EP 10 Out
  NULL,
  // EP 10 In
  NULL,
  // EP 11 Out
  NULL,
  // EP 11 In
  NULL,
  // EP 12 Out
  NULL,
  // EP 12 In
  NULL,
  // EP 13 Out
  NULL,
  // EP 13 In
  NULL,
  // EP 14 Out
  NULL,
  // EP 14 In
  NULL,
  // EP 15 Out
  NULL,
  // EP 15 In
  NULL,
  // UsbResetEvent
  NULL,
  // UsbConnectEvent
  NULL,
  // UsbSuspendEvent
  NULL,
  // UsbErrorEvent
  NULL,
  // UsbSofEvent
  NULL,
  // UsbHighPrioIntrEvent
  NULL,
};

/*************************************************************************
 * Function Name: USB_Cmd
 * Parameters:  Int16U Command, Int8U Data
 *
 * Return: Int32U - command result
 *
 * Description: Implement commands transmit to USB Engine
 *
 *************************************************************************/
Int32U USB_Cmd (Int16U Command, Int8U Data)
{
Int32U save, tmp = 0;
  // Disable interrupt and save current state of the interrupt flags
  save = __get_interrupt_state();
  __disable_interrupt();
  DEVINTCLR = bmUSB_CommDataFullInterrupt | bmUSB_CommRegEmptyInterrupt;
  // Load command in USB engine
  CMDCODE = ((Command&0xFF) << 16) + USB_CMD_WR;
  // Wait until command is accepted
  while ((DEVINTS & bmUSB_CommRegEmptyInterrupt) == 0);
  // clear Command reg. empty interrupt
  DEVINTCLR = bmUSB_CommRegEmptyInterrupt;
  // determinate next phase of the command
  switch (Command)
  {
  case CMD_USB_SET_ADDRESS:
  case CMD_USB_CFG_DEV:
  case CMD_USB_SET_MODE:
  case CMD_USB_SET_DEV_STAT:
    CMDCODE = (Data << 16) + USB_DATA_WR;
    while ((DEVINTS & bmUSB_CommRegEmptyInterrupt) == 0);
    break;
  case CMD_USB_RD_FRAME_NUMB:
  case CMD_USB_RD_TEST_REG:
    CMDCODE = (Command << 16) + USB_DATA_RD;
    while ((DEVINTS & bmUSB_CommDataFullInterrupt) == 0);
    DEVINTCLR = bmUSB_CommDataFullInterrupt;
    tmp = CMDDATA;
    CMDCODE = (Command << 16) + USB_DATA_RD;
    while ((DEVINTS & bmUSB_CommDataFullInterrupt) == 0);
    tmp |= CMDDATA << 8;
    break;
  case CMD_USB_GET_DEV_STAT:
  case CMD_USB_GET_ERROR:
  case CMD_USB_RD_ERROR_STAT:
  case CMD_USB_CLR_BUF:
    CMDCODE = (Command << 16) + USB_DATA_RD;
    while ((DEVINTS & bmUSB_CommDataFullInterrupt) == 0);
    tmp = CMDDATA;
    break;
  default:
    switch (Command & 0x1E0)
    {
    case CMD_USB_SEL_EP:
    case CMD_USB_SEL_CLR_INT_EP:
      CMDCODE = (Command << 16) + USB_DATA_RD;
      while ((DEVINTS & bmUSB_CommDataFullInterrupt) == 0);
      tmp = CMDDATA;
      break;
    case CMD_USB_SET_EP_STAT:
      CMDCODE = (Data << 16) + USB_DATA_WR;
      while ((DEVINTS & bmUSB_CommRegEmptyInterrupt) == 0);
      break;
    }
    break;
  }
  // restore the interrupt flags
  __set_interrupt_state(save);;
  return(tmp);
}

/*************************************************************************
 * Function Name: USB_UserFuncRegistering
 * Parameters: UserFunc_t UserFunc, UsbUserEvent_t UserFuncInd
 *
 * Return: UserFunc_t
 *
 * Description: Registering User callback function
 *
 *************************************************************************/
UserFunc_t USB_UserFuncRegistering (UserFunc_t UserFunc, UsbUserEvent_t UserFuncInd)
{
UserFunc_t PrevUserFunc = UsbUserFun[UserFuncInd];
  UsbUserFun[UserFuncInd] = (UserFunc_t) UserFunc;
  return (PrevUserFunc);
}

/*************************************************************************
 * Function Name: USB_EpIntrClr
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: Int8U
 *
 * Description: Clear the EP interrupt flag and return the current EP status
 *
 *************************************************************************/
Int8U USB_EpIntrClr(USB_Endpoint_t EndPoint)
{
  ENDPINTCLR = 1 << EndPoint;
  while ((DEVINTS & bmUSB_CommDataFullInterrupt) == 0);
  return(CMDDATA);
}

/*************************************************************************
 * Function Name: USB_HwInit
 * Parameters: LPC_VicIrqSlots_t IntrSlot
 *
 * Return: none
 *
 * Description: Init USB
 *
 *************************************************************************/
void USB_HwInit(LPC_VicIrqSlots_t IntrSlot)
{
  // Init software EP priority
  USB_EpSoftPrio = 0;
  // Init SOF number hold
  #if  USB_SOF_EVENT > 0
  USB_SofNumbHold = 0;
  #endif
  // Disable USB interrupts
  USBINTS_bit.EN_USB_INTS = 0;

  /* Turn on USB */
  PM_OpenPeripheral(0x80000000);

  // Enable Vbus sense and Connect
  PINSEL1_bit.P0_23 = 1;
  PINSEL1_bit.P0_31 = 2;

  // Init Pll for USB engine freq - 48MHz
  //USB_PLL_M, USB_PLL_P are define in lpc_usb_cfg.h
  PLL48CFG_bit.MSEL = USB_PLL_M-1;
  PLL48CFG_bit.PSEL = USB_PLL_P;

  PLL48CON_bit.PLLE = TRUE;     // Enable PLL
  PLL48FEED = PLLFEED_DATA1;
  PLL48FEED = PLLFEED_DATA2;

  while (!PLL48STAT_bit.PLOCK); // Wait PLL lock

  PLL48CON_bit.PLLC = TRUE;     // Connect PLL
  PLL48FEED = PLLFEED_DATA1;
  PLL48FEED = PLLFEED_DATA2;

  // USB interrupt connect to VIC
  VIC_SetVectoredIRQ(USB_ISR,(LPC_VicIrqSlots_t)IntrSlot,VIC_USB);
  VIC_EnableInt(1<<VIC_USB);
  // Enable USB interrupts
  USBINTS_bit.EN_USB_INTS = 1;
  // Disconnect device
  USB_Connect(FALSE);
  // Set Address 0
  USB_SetDefAdd();
  // Init controls endpoints
  USB_HwReset();
  // Init Device state var
  USB_DevStatus.Data = USB_Cmd(CMD_USB_GET_DEV_STAT,0);
}

/*************************************************************************
 * Function Name: USB_HwReset
 * Parameters: none
 *
 * Return: none
 *
 * Description: Reset USB engine
 *
 *************************************************************************/
void USB_HwReset (void)
{
  // Disable all endpoint interrupts
  ENDPINTEN = 0;
  // Frame is Hp interrupt
  DEVINTPRI = 1;
  // Clear all interrupts flag
  ENDPINTCLR  = DEVINTCLR = 0xFFFFFFFF;
  // USB_Configure
  USB_Configure(FALSE);
  // EndPoint Init
  USB_RealizeEndPoint(CTRL_ENP_OUT,0,Ep0MaxSize,TRUE);
  USB_RealizeEndPoint(CTRL_ENP_IN, 1,Ep0MaxSize,TRUE);

#if USB_DMA > 0
  // Enable End_of_Transfer_Interrupt and
  // System_Error_Interrupt USB DMA interrupts
  USB_DmaReset(DMA_INT_ENABLE_MASK);
#endif
  // Enable Device interrupts
  DEVINTEN = bmUSB_SlowInterrupt | bmUSB_DevStatusInterrupt |
            (USB_HIGH_PRIORITY_EVENT ? bmUSB_FastInterrupt  : 0) |
            (USB_SOF_EVENT           ? bmUSB_FrameInterrupt : 0) |
            (USB_ERROR_EVENT         ? bmUSB_ErrorInterrupt : 0);

}
/*************************************************************************
 * Function Name: USB_SetEpPrio
 * Parameters: USB_Endpoint_t EndPoint,Boolean EpPrio
 *
 * Return: none
 *
 * Description: Set Endpoint priority
 *
 *************************************************************************/
inline
void USB_SetEpPrio (USB_Endpoint_t EndPoint,Boolean EpPrio)
{
Int32U Mask = 1 << EndPoint;
  USB_EpSoftPrio = (EpPrio)?(USB_EpSoftPrio | Mask):(USB_EpSoftPrio & ~Mask);
}

/*************************************************************************
 * Function Name: USB_RealizeEndPoint
 * Parameters: USB_Endpoint_t EndPoint, Boolean EpPrio,
 *                Int32U MaxPacketSize, Boolean IntrEna
 *
 * Return: none
 *
 * Description: Enable or disable endpoint
 *
 *************************************************************************/
void USB_RealizeEndPoint( USB_Endpoint_t EndPoint, Boolean EpPrio,
                             Int32U MaxPacketSize, Boolean IntrEna)
{
Int32U Mask = (1 << EndPoint);
  // Init Ep software priority
  USB_SetEpPrio(EndPoint,EpPrio);
  if (MaxPacketSize)
  {
    // Clear  Realize interrupt bit
    DEVINTCLR = bmUSB_EPRealizeInterrupt;
    // Realize endpoint
    REALIZEENDP  |= Mask;
    // Set endpoint maximum packet size
    ENDPIND       = EndPoint;
    MAXPACKSIZE   = MaxPacketSize;
    // Wait for Realize complete
    while ((DEVINTS & bmUSB_EPRealizeInterrupt) == 0);
    // Enable endpoint interrupt
    if (IntrEna)
    {
      ENDPINTEN |= Mask;
    }
    else
    {
      ENDPINTEN &= ~Mask;
    }
  }
  else
  {
    Mask =~ Mask;
    // Disable relevant endpoint and interrupt
    REALIZEENDP &= Mask;
    ENDPINTEN   &= Mask;
  }
}

/*************************************************************************
 * Function Name: USB_ClearBuffer
 * Parameters: USB_Endpoint_t EndPoint - endpoint index
 *
 * Return: Int32U Packet overwrite status
 *
 * Description: Clear buffer of the corresponding endpoint
 *
 *************************************************************************/
Int32U USB_ClearBuffer(USB_Endpoint_t EndPoint)
{
  // Select endpoint
  USB_Cmd(CMD_USB_SEL_EP | EndPoint,0);
  // Clear selected end point
  return(USB_Cmd(CMD_USB_CLR_BUF,0));
}

/*************************************************************************
 * Function Name: USB_ValidateBuffer
 * Parameters: USB_Endpoint_t EndPoint - endpoint index
 *
 * Return: none
 *
 * Description: Validate buffer(only for IN Endpoints)
 *
 *************************************************************************/
void USB_ValidateBuffer(USB_Endpoint_t EndPoint)
{
  if (EndPoint & 1)
  {
    USB_Cmd(CMD_USB_SEL_EP | EndPoint,0);
    USB_Cmd(CMD_USB_VAL_BUF,0);
  }
}

/*************************************************************************
 * Function Name: USB_SetAdd
 * Parameters: Int32U DevAdd - device address between 0 - 127
 *
 * Return: none
 *
 * Description: Set device address
 *
 *************************************************************************/
void USB_SetAdd(Int32U DevAdd)
{
  USB_Cmd(CMD_USB_SET_ADDRESS,DevAdd | 0x80);
  USB_Cmd(CMD_USB_SET_ADDRESS,DevAdd | 0x80);
}

/*************************************************************************
 * Function Name: USB_Connect
 * Parameters: Boolean Conn
 *
 * Return: none
 *
 * Description: Connect USB
 *
 *************************************************************************/
void USB_Connect (Boolean Conn)
{
  USB_Cmd(CMD_USB_SET_DEV_STAT, (Conn ? bmUSB_Connect : 0));
}

/*************************************************************************
 * Function Name: USB_Configure
 * Parameters: Boolean Configure
 *
 * Return: none
 *
 * Description: Configure device
 *              When Configure != 0 enable all Realize Ep
 *
 *************************************************************************/
void USB_Configure (Boolean Configure)
{
  USB_Cmd(CMD_USB_CFG_DEV,Configure);
}

/*************************************************************************
 * Function Name: USB_WakeUp
 * Parameters: none
 *
 * Return: none
 *
 * Description: Wake up USB
 *
 *************************************************************************/
void USB_WakeUp (void)
{
  USB_Cmd(CMD_USB_SET_DEV_STAT, bmUSB_Suspend | (USB_DevStatus.Connect?bmUSB_Connect:0));
}

/*************************************************************************
 * Function Name: USB_GetDevStatus
 * Parameters: USB_DevStatusReqType_t Type
 *
 * Return: Boolean
 *
 * Description: Return USB device status
 *
 *************************************************************************/
Boolean USB_GetDevStatus (USB_DevStatusReqType_t Type)
{
  switch (Type)
  {
  case USB_DevConnectStatus:
    return(USB_DevStatus.Connect);
  case USB_SuspendStatus:
    return(USB_DevStatus.Suspend);
  case USB_ResetStatus:
    return(USB_DevStatus.Reset);
  }
  return(FALSE);
}

/*************************************************************************
 * Function Name: USB_SetStallEP
 * Parameters: USB_Endpoint_t EndPoint, Boolean Stall
 *
 * Return: none
 *
 * Description: The endpoint stall/unstall
 *
 *************************************************************************/
void USB_SetStallEP (USB_Endpoint_t EndPoint, Boolean Stall)
{
  USB_Cmd(CMD_USB_SET_EP_STAT | EndPoint, (Stall ? bmUSB_EpStall : 0));
}

/*************************************************************************
 * Function Name: USB_GetStallEP
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: Boolean
 *
 * Description: Get stall state of the endpoint
 *
 *************************************************************************/
Boolean USB_GetStallEP (USB_Endpoint_t EndPoint)
{
  return ((USB_Cmd(CMD_USB_SEL_EP | EndPoint, 0) & bmUSB_EpStallStatus) != 0);
}

/*************************************************************************
 * Function Name: USB_EpWrite
 * Parameters: USB_Endpoint_t EndPoint, Int32U * pData, Int32U Count
 *
 * Return: none
 *
 * Description: Endpoint Write (IN)
 *
 *************************************************************************/
void USB_EpWrite (USB_Endpoint_t EndPoint, Int32U * pData, Int32U Count)
{
  // Convert EP physical address to logical and set write enable bit
  USBCTRL = ((EndPoint << 1) & 0x3C) | bmUSB_CtrlWrEna;

  // Get data size
  TPKTLEN = Count;
  // Write data to SIE buffer
  if(Count)
  {
    do
    {
      TDATA = *pData++;
    }
    while (USBCTRL_bit.WR_EN);
  }
  else
  {
    do
    {
      TDATA = 0;
    }
    while (USBCTRL_bit.WR_EN);
  }

  USBCTRL = 0;

  USB_Cmd(CMD_USB_SEL_EP | EndPoint, 0);
  USB_Cmd(CMD_USB_VAL_BUF, 0);

}

/*************************************************************************
 * Function Name: USB_EpRead
 * Parameters: USB_Endpoint_t EndPoint, Int32U * pData, Int32U Count
 *
 * Return: Int32
 *
 * Description: Endpoint Read (OUT)
 *
 *************************************************************************/
Int32U USB_EpRead (USB_Endpoint_t EndPoint, Int32U * pData, Int32U Count)
{
  // Convert EP physical address to logical and set read enable bit
  USBCTRL = ((EndPoint << 1) & 0x3C) | bmUSB_CtrlRdEna;
  while (RCVEPKTLEN_bit.PKT_RDY == 0);
  // Get data size
  Count = RCVEPKTLEN_bit.PKT_LNGTH;
  // Read data from SIE buffer
  while (RCVEPKTLEN_bit.DV)
  {
    *pData++ = RCVEDATA;
  }

  USBCTRL = 0;

  USB_Cmd(CMD_USB_SEL_EP | EndPoint, 0);
  if(USB_Cmd(CMD_USB_CLR_BUF, 0) & bmUSB_PacketOverWritten)
  {
    Count |= 0x80000000;
  }
  return (Count);
}

/*************************************************************************
 * Function Name: USB_ISR
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB interrupt subroutine
 *
 *************************************************************************/
void USB_ISR (void)
{
Int32U EpIntr,Val,EpIntHold;
UsbDevIntrStat_t UsbDevIntrSt = {DEVINTS};

#if USB_DMA > 0
  if (USBINTS & 3)
  {
#endif
  #if USB_HIGH_PRIORITY_EVENT > 0
    // Fast EP interrupt
    if(UsbDevIntrSt.Fast)
    {
      DEVINTCLR = bmUSB_FastInterrupt;
      if(UsbUserFun[UsbHighPrioIntrEvent] != NULL)
      {
        UsbUserFun[UsbHighPrioIntrEvent]((void *)0);
      }
      // Clear Fast EP interrupt
    }
  #endif

  #if USB_ERROR_EVENT > 0
    // USB engine error interrupt
    if(UsbDevIntrSt.Error)
    {
      DEVINTCLR = bmUSB_ErrorInterrupt;
      Val = USB_Cmd(CMD_USB_RD_ERROR_STAT,0);
      if(UsbUserFun[UsbErrorEvent] != NULL)
      {
        UsbUserFun[UsbErrorEvent]((void *)Val);
      }
    }
  #endif

  #if USB_SOF_EVENT > 0
    // Frame interrupt
    if(UsbDevIntrSt.Frame)
    {
      DEVINTCLR = bmUSB_FrameInterrupt;
//      Val = USB_Cmd(CMD_USB_RD_FRAME_NUMB,0);
      if(UsbUserFun[UsbSofEvent] != NULL)
      {
        UsbUserFun[UsbSofEvent]((void *)Val);
      }
    }
  #endif

    // Device Status interrupt
    if(UsbDevIntrSt.Status)
    {
      // Clear Device status interrupt
      DEVINTCLR = bmUSB_DevStatusInterrupt;
      // Get device status
      USB_DevStatus.Data = USB_Cmd(CMD_USB_GET_DEV_STAT,0);
      // Device connection status
      if(USB_DevStatus.ConnectChange)
      {
        if(UsbUserFun[UsbConnectEvent] != NULL)
        {
          UsbUserFun[UsbConnectEvent]((void *)USB_DevStatus.Connect);
        }
      }
      // Device suspend status
      if(USB_DevStatus.SuspendChange)
      {
        if(UsbUserFun[UsbSuspendEvent] != NULL)
        {
          UsbUserFun[UsbSuspendEvent]((void *)USB_DevStatus.Suspend);
        }
      }
      // Device reset
      if(USB_DevStatus.Reset)
      {
        USB_HwReset();
        if(UsbUserFun[UsbResetEvent] != NULL)
        {
          UsbUserFun[UsbResetEvent](NULL);
        }
      }
    }

    // Slow EP interrupt
    if(UsbDevIntrSt.Slow)
    {
      // Clear Slow EP interrupt
      DEVINTCLR = bmUSB_SlowInterrupt;
      do
      {
        EpIntr = ENDPINTS;
        // First Software High priority and then low priority
        Int32U USB_EpSoftPrioHold = USB_EpSoftPrio;
        for (Int32U i = 2; i; --i, USB_EpSoftPrioHold ^= 0xFFFFFFFF)
        {
        Int32U EpIntrCurrPrio = EpIntr & USB_EpSoftPrioHold;
          // Output ctrl EP
          if(EpIntrCurrPrio & 1)
          {
            Val = USB_EpIntrClr(CTRL_ENP_OUT);
            if(UsbUserFun[UsbEp0Out] != NULL)
            {
              if(Val & bmUSB_EpSetupPacket)
              {
                UsbUserFun[UsbEp0Out]((void *)UsbSetupPacket);
              }
              else
              {
                UsbUserFun[UsbEp0Out]((void *)UsbDataOutPacket);
              }
            }
            break;
          }
          // All other endpoints
          for(Val = 1,EpIntHold = EpIntrCurrPrio >> 1; EpIntHold; ++Val,EpIntHold >>= 1)
          {
            if(EpIntHold & 1)
            {
              USB_EpIntrClr((USB_Endpoint_t)Val);
              if(UsbUserFun[Val] != NULL)
              {
                UsbUserFun[Val]((void *)((Val&1)?UsbDataInPacket:UsbDataOutPacket));
              }
              break;
            }
          }
        }
      }
      while(EpIntr);
    }
#if USB_DMA > 0
  }
  if (USBINTS_bit.USB_INT_REQ_DMA)
  {
    // First Software High priority and then low priority
    Int32U UsbDmaInt = 0,Tmp;
    Tmp = EOTINTSTAT;
    if(DMAINTEN & 1)
    {
      UsbDmaInt |= Tmp;
    }
    EOTINTCLR = Tmp;
    Tmp = NEWDDRINTSTAT;
    if(DMAINTEN & 2)
    {
      UsbDmaInt |= Tmp;
    }
    NEWDDRINTCLR = Tmp;
    Tmp = SYSERRINTSTAT;
    if(DMAINTEN & 4)
    {
      UsbDmaInt |= Tmp;
    }
    SYSERRINTCLR = Tmp;
    for (Int32U i = 2; i; --i, USB_EpSoftPrio ^= 0xFFFFFFFF)
    {
      // All endpoints without ctrl EP_In, ctrl EP_Out
      Int32U UsbDmaCurrPriorityInt = USB_EpSoftPrio & UsbDmaInt;
      for(Int32U Val = 2, EpMask = 4; UsbDmaCurrPriorityInt; ++Val,EpMask <<= 1)
      {
        Int32U EpPriorityMask = UsbDmaCurrPriorityInt & EpMask;
        if(EpPriorityMask == 0)
        {
          continue;
        }
        UsbDmaCurrPriorityInt &= ~EpMask;
        // Collect Interrupts status flags and clear interrupt flags
        if(EpPriorityMask && (UsbUserFun[Val] != NULL))
        {
          UsbUserFun[Val]((void *)UsbDmaPacket);
        }
      }
    }
  }
#endif
  VICVectAddr = 0;    // Clear interrupt in VIC.
}

/*************************************************************************
 * Function Name: USB_EpLogToPhysAdd
 * Parameters: Int8U EpLogAdd
 *
 * Return: USB_Endpoint_t
 *
 * Description: Convert the logical to physical address
 *
 *************************************************************************/
USB_Endpoint_t USB_EpLogToPhysAdd (Int8U EpLogAdd)
{
USB_Endpoint_t Address = (USB_Endpoint_t)((EpLogAdd & 0x0F)<<1);
  if(EpLogAdd & 0x80)
  {
    ++Address;
  }
  return(Address);
}

/*************************************************************************
 * Function Name: USB_GetFrameNumb
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Return current value of SOF number
 *
 *************************************************************************/
#if USB_SOF_EVENT > 0
Int32U USB_GetFrameNumb (void)
{
  return(USB_SofNumbHold);
}
#endif

/*************************************************************************
 *                          U S B   D M A  P a r t                       *
 *************************************************************************/

#if USB_DMA > 0
#pragma segment="DMA_RAM"
#pragma location="DMA_RAM"
#pragma data_alignment=128
__no_init pUSB_DmaDesc_t USB_DDCA[ENP_MAX_NUMB];

#pragma location="DMA_RAM"
__no_init USB_DmaDesc_t USB_DmaDesc[DMA_DD_MAX_NUMB];

/*************************************************************************
 * Function Name: USB_DmaReset
 * Parameters:  Int32U IntrEna
 *
 * Return: none
 *
 * Description: Reset USB DMA
 *
 *************************************************************************/
void USB_DmaReset (Int32U IntrEna)
{
  // Disable All DMA interrupts
  DMAINTEN = 0;
  // DMA Disable
  EPDMADIS      = 0xFFFFFFFF;
  // DMA Request clear
  DMARQSTCLR    = 0xFFFFFFFF;
  // End of Transfer Interrupt Clear
  EOTINTCLR     = 0xFFFFFFFF;
  // New DD Request Interrupt Clear
  NEWDDRINTCLR  = 0xFFFFFFFF;
  // System Error Interrupt Clear
  SYSERRINTCLR  = 0xFFFFFFFF;
  for(Int32U i = 0; i < ENP_MAX_NUMB; ++i)
  {
    USB_DDCA[i] = NULL;
  }
  // Set USB UDCA Head register
  UDCAHEAD = (Int32U)&USB_DDCA;
  // Enable DMA interrupts
  DMAINTEN = IntrEna;
}

/*************************************************************************
 * Function Name: USB_DmaInitTransfer
 * Parameters: USB_Endpoint_t EndPoint, Int32U DmaDescInd,
 *             pInt32U pData, Int32U EpMaxSize, Int32U Size
 *             pDmaIsoPacket_t pDmaIsoPacket,  Boolean EpTransferType
 *
 * Return: UsbDmaStateCode_t
 *
 * Description: Init Transfer by DMA
 *
 *************************************************************************/
UsbDmaStateCode_t USB_DmaInitTransfer (USB_Endpoint_t EndPoint,
          Int32U DmaDescInd, pInt32U pData, Int32U EpMaxSize, Int32U Size,
          pDmaIsoPacket_t pDmaIsoPacket, Boolean EpTransferType)
{
Int32U EpReg;

  if ((EndPoint == CTRL_ENP_OUT) || (EndPoint == CTRL_ENP_IN))
  {
    return(UsbDmaParametersError);
  }
  if (USB_DmaDesc[DmaDescInd].Status == UsbDmaBeingServiced)
  {
    return(UsbDmaBeingServiced);
  }

  // Init DMA Descriptor
  USB_DmaDesc[DmaDescInd].pNextDD        = NULL;
  USB_DmaDesc[DmaDescInd].NextDDValid    = FALSE;
  USB_DmaDesc[DmaDescInd].pDmaIsoPacket  = pDmaIsoPacket;
  USB_DmaDesc[DmaDescInd].DmaMode        = UsbDmaNormalMode;
  USB_DmaDesc[DmaDescInd].Isochronous    = EpTransferType;
  USB_DmaDesc[DmaDescInd].pDmaBuffer     = pData;
  USB_DmaDesc[DmaDescInd].DmaBufferLegtn = Size;
  USB_DmaDesc[DmaDescInd].MaxPacketSize  = EpMaxSize;
  USB_DmaDesc[DmaDescInd].Status         = UsbDmaNoServiced;

  // Set DD
  USB_DDCA[EndPoint] = &USB_DmaDesc[DmaDescInd];

  // Enable DMA Transfer
  EPDMAEN = 1 << EndPoint;

  // Check state of IN/OUT Ep buffer
  EpReg = USB_Cmd(CMD_USB_SEL_EP | EndPoint,0);

  if( ((EndPoint & 1) && !(EpReg & 0x60)) ||
     (!(EndPoint & 1) && ((EpReg & 0x60) == 0x60)))
  {
    if((USB_DmaDesc[DmaDescInd].DdState != UsbDmaBeingServiced))
    {
      // Retrigger DMA Transfer
      DMARQSTSET = 1 << EndPoint;
    }
  }

  return(UsbDmaNoServiced);
}

/*************************************************************************
 * Function Name: USB_DmaGetDesc
 * Parameters: Int32U DmaDescInd
 *
 * Return: pUSB_DmaDesc_t
 *
 * Description: Return pointer to DMA descriptor
 *
 *************************************************************************/
pUSB_DmaDesc_t USB_DmaGetDesc (Int32U DmaDescInd)
{
  return(&USB_DmaDesc[DmaDescInd]);
}

/*************************************************************************
 * Function Name: USB_DmaDisable
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: none
 *
 * Description: Disable DMA transfer for the EP
 *
 *************************************************************************/
void USB_DmaDisable (USB_Endpoint_t EndPoint)
{
  EPDMADIS = 1 << EndPoint;
}

/*************************************************************************
 * Function Name: USB_DmaRestattTransfer
 * Parameters: USB_Endpoint_t EndPoint, Int32U DmaDescInd,
 *             pInt32U pData, Int32U EpMaxSize, Int32U Size
 *             pDmaIsoPacket_t pDmaIsoPacket,  Boolean EpTransferType
 *
 * Return: none
 *
 * Description: Restart DMA Transfer
 *
 *************************************************************************/
void USB_DmaRestattTransfer (USB_Endpoint_t EndPoint,Int32U DmaDescInd)
{
  // Init DD DMA status
  USB_DmaDesc[DmaDescInd].Status = UsbDmaNoServiced;
  // Enable DMA Transfer
  EPDMAEN = 1 << EndPoint;
  // Check state of IN/OUT Ep buffer
  Int32U EpReg = USB_Cmd(CMD_USB_SEL_EP | EndPoint,0);
  if(!(EndPoint & 1) && ((EpReg & 0x60) == 0x60))
  {
    // Retrigger DMA Transfer
    DMARQSTSET = 1 << EndPoint;
  }
  else if ((EndPoint & 1) && !(EpReg & 0x60))
  {
    // Retrigger DMA Transfer
    DMARQSTSET = 1 << EndPoint;   // А защо е така кой ще ми каже????
  }
}

#endif // USB_DMA

