/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    File name   : usb_hw.c
 *    Description : usb module (HAL)
 *
 *    History :
 *    1. Date        : April 8, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *        USB framework rev. 2 light version with DMA transfer support
 *    2. Date        : May 12, 2011
 *       Author      : Stanimir Bonev
 *       Description : Port for LPC11Uxx
 *
 *    $Revision: 28 $
 **************************************************************************/
#define USB_HW_GLOBAL
#include "usb_hw.h"

#pragma section="USB_PACKET_MEMORY"

static volatile UsbDevStat_t USB_DevStatus;

static const UsbStandardEpDescriptor_t USB_CtrlEpDescr0 =
{
  sizeof(UsbStandardEpDescriptor_t),
  UsbDescriptorEp,
  UsbEpOut(CTRL_ENP_OUT>>1),
  {(Int8U)UsbEpTransferControl | (Int8U)UsbEpSynchNoSynchronization | (Int8U)UsbEpUsageData},
  Ep0MaxSize,
  0
};

static const UsbEP_ExtData_t USB_CtrlEpExtDescr0 =
{
  0
};

static const UsbStandardEpDescriptor_t USB_CtrlEpDescr1 =
{
  sizeof(UsbStandardEpDescriptor_t),
  UsbDescriptorEp,
  UsbEpIn(CTRL_ENP_IN>>1),
  {(Int8U)UsbEpTransferControl | (Int8U)UsbEpSynchNoSynchronization | (Int8U)UsbEpUsageData},
  Ep0MaxSize,
  0
};

static const Boolean UsbEpDoubleBuffType[] =
{
  FALSE,  /* OUT 0 */
  FALSE,  /* IN 0  */
  TRUE,   /* OUT 1 */
  TRUE,   /* IN 1  */
  TRUE,   /* OUT 2 */
  TRUE,   /* IN 2  */
  TRUE,   /* OUT 3 */
  TRUE,   /* IN 3  */
  TRUE,   /* OUT 4 */
  TRUE,   /* IN 4  */
};

#if  USB_SOF_EVENT > 0
Int32U  USB_SofNumbHold;
#endif

#pragma location="USB_PACKET_MEMORY"
__root __no_init Int8U USB_PacketMemory[2048];
USB_EpDeskTbl_t * const pUSB_EP_Desc = (USB_EpDeskTbl_t *)USB_PacketMemory;

pPacketMemUse_t pPacketMemUse;
PacketMemUse_t PacketMemBuff[ENP_MAX_NUMB];

static Int32U IsoTransEP;

/*************************************************************************
 * Function Name: UsbMalloc
 * Parameters: none
 *
 * Return: void *
 *
 * Description: Return address of free element of the PacketMemBuff massive
 *
 *************************************************************************/
static
void * UsbMalloc(void)
{
Int32U Count;
  for (Count = 0; Count < ENP_MAX_NUMB; Count++)
  {
    if (PacketMemBuff[Count].Size == 0)
    {
      return(&PacketMemBuff[Count]);
    }
  }
  return(NULL);
}

/*************************************************************************
 * Function Name: UsbFree
 * Parameters: pPacketMemUse_t pPntr
 *
 * Return: none
 *
 * Description: Free element of the PacketMemBuff massive
 *
 *************************************************************************/
inline static
void UsbFree(pPacketMemUse_t pPntr)
{
  pPntr->Size = 0;
}

/*************************************************************************
 * Function Name: USB_AllocateBuffer
 * Parameters: pInt16U pOffset,Int32U PacketSize,
 *             USB_Endpoint_t EndPoint
 *
 * Return: Boolean
 *
 * Description: Allocate a new buffer
 *
 *************************************************************************/
static
Boolean USB_AllocateBuffer (pInt16U pOffset,Int32U PacketSize,
                            USB_Endpoint_t EndPoint)
{
pPacketMemUse_t  pPacketMem = pPacketMemUse, pPacketMemNext, pPacketMemUseNew;

  /* Offset alignment by 64 */
  *pOffset =  ((sizeof(USB_EpDeskTbl_t) * ENP_MAX_NUMB*2) + (64ul-1)) & ~(64ul-1);
  /* Finding free memory block from the USB packet memory */
  while(pPacketMem != NULL)
  {
    *pOffset = pPacketMem->Start + pPacketMem->Size;
    /* Offset alignment by 64 */
    *pOffset +=  (64-1);
    *pOffset &= ~(64-1);
    pPacketMemNext = pPacketMem->pNext;
    if((pPacketMem->pNext == NULL) ||
       (pPacketMemNext->Start >= *pOffset + PacketSize))
    {
      break;
    }
    pPacketMem = pPacketMem->pNext;
  }
  /* Is enough free space for new block? */
  if((*pOffset + PacketSize) >=
     (Int32U)__segment_size("USB_PACKET_MEMORY")
      )
  {
    /* Not enough space in the USB packet memory */
    return(FALSE);
  }

  pPacketMemUseNew = UsbMalloc();
  if(pPacketMemUseNew == NULL)
  {
    /* too many point are realized */
    return(FALSE);
  }
  if(pPacketMemUse == NULL)
  {
    pPacketMemUse = pPacketMemUseNew;
    pPacketMemUse->pNext = NULL;
  }
  else
  {
    pPacketMemUseNew->pNext  = pPacketMem->pNext;
    pPacketMem->pNext        = pPacketMemUseNew;
  }
  pPacketMemUseNew->EpAddr = EndPoint;
  pPacketMemUseNew->Start  = *pOffset;
  pPacketMemUseNew->Size   = PacketSize;
  return(TRUE);
}

/*************************************************************************
 * Function Name: USB_ReleaseBuffer
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: none
 *
 * Description: Release the buffer/s
 *
 *************************************************************************/
static
void USB_ReleaseBuffer (USB_Endpoint_t EndPoint)
{
pPacketMemUse_t  pPacketMem, pPacketMemPrev;
  pPacketMem = pPacketMemUse;
  while(pPacketMem != NULL)
  {
    if(pPacketMem->EpAddr == EndPoint)
    {
      pPacketMemPrev->pNext = pPacketMem->pNext;
      UsbFree(pPacketMem);
      pPacketMem = pPacketMemPrev->pNext;
    }
    else
    {
      pPacketMemPrev = pPacketMem;
      pPacketMem = pPacketMem->pNext;
    }
    if (pPacketMem == NULL)
    {
      break;
    }
  }
}

/*************************************************************************
 * Function Name: USB_DectiveEP
 * Parameters: IUSB_Endpoint_t EndPoint
 *
 * Return: none
 *
 * Description: Deactivate EP
 *
 *************************************************************************/
static
void USB_DectiveEP(USB_Endpoint_t EndPoint)
{
  Int32U mask = 1ul << EndPoint;
  USB_EPSKIP = mask;
  while(USB_EPSKIP & mask);
  USB_INTSTAT = mask;
}

/*************************************************************************
 * Function Name: USB_HwInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init USB
 *
 *************************************************************************/
void USB_HwInit(void)
{
  /* Init SOF number hold */
  #if  USB_SOF_EVENT > 0
  USB_SofNumbHold = 0;
  #endif

  /*Init USB clock. Use USBPLL as USP clock source*/
  CLK_SetUsbPll(CLK_ENABLE, (48MHZ/MOSC)-1, 1);

  CLK_SetUsbClk(USBSEL_CLKUSBPLL, 1);

  /*Enable USB regs and USB RAM*/
  USBCLKCTRL_bit.AP_CLK = 0;
  SYSAHBCLKCTRL_bit.USB = 1;
  SYSAHBCLKCTRL_bit.USBRAM = 1;
  /* NeedClk normal mode(not ALWAYS ON) */
  USB_DEVCMDSTAT_bit.PLL_ON = 0;

  /*Power up USB PHY*/
  PDRUNCFG_bit.USBPAD_PD = 0;
  /*USB IO assign*/
  SYSAHBCLKCTRL_bit.GPIO = 1;
  /*Select Connect and Vbus pin function
    and disable Pull Up/Down resistors*/
  USB_VBUS_IOCON = USB_VBUS_IOCON_INIT;
  USB_CONNECT_IOCON = USB_CONNECT_IOCON_INIT;

  /* Disconnect device */
  USB_ConnectRes(FALSE);

  /* Set address 0 */
  USB_SetDefAdd();

  /* clear all pending interrupts */
  USB_INTSTAT = 0xC00003FF;
  USB_INTROUTING = (USB_FRAME_PRIORITY << 30)
             | (USB_DEV_PRIORITY   << 31)
             | (USB_EP_PRIORITY         )
             ;

  /* Init controls endpoints */
  USB_HwReset();
  /* Init Device status */
  UsbSetDevState(UsbDevStatusUnknow);
  /* Init Device state var */
  USB_DevStatus.Data = USB_DEVCMDSTAT;

  /* Enable USB interrupts */
  /* USB interrupt connect to VIC */
  /* USB interrupt enable */
  NVIC_IntEnable(NVIC_USB_IRQ);
  NVIC_IntPri(NVIC_USB_IRQ,USB_INTR_PRIORITY);

#if USB_FRAME_PRIORITY || USB_DEV_PRIORITY || USB_EP_PRIORITY
  NVIC_IntEnable(NVIC_USB_FIQ);
  NVIC_IntPri(NVIC_USB_FIQ,USB_FIQ_INTR_PRIORITY);
#endif
#if USB_REMOTE_WAKEUP
  NVIC_IntEnable(NVIC_USB_WAKEUP);
  NVIC_IntPri(NVIC_USB_WAKEUP,USB_WAKEUP_INTR_PRIORITY);
#endif
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
  pUSB_EpDeskTbl_t pUSB_EpDeskTbl = (pUSB_EpDeskTbl_t)USB_PacketMemory;
  /* Disable all endpoint interrupts */
  USB_INTEN &= ~0x3FFul;
  /* Init all EP descriptorts */
  USB_EPLISTSTART  = (Int32U)USB_PacketMemory;
  USB_DATABUFSTART = (Int32U)USB_PacketMemory;

  for(int i = CTRL_ENP_OUT; i < ENP_MAX_NUMB*2; i++)
  {
    pUSB_EpDeskTbl++->Data = bmUSB_DescDis;
  }
  memset(PacketMemBuff,0x00, sizeof(PacketMemBuff));
  pPacketMemUse = NULL;
  IsoTransEP = 0;

  /* USB_Configure */
  USB_Configure(FALSE);
  /* Control EP Init */
  USB_RealizeEp(&USB_CtrlEpDescr0,&USB_CtrlEpExtDescr0,TRUE);
  USB_RealizeEp(&USB_CtrlEpDescr1,&USB_CtrlEpExtDescr0,TRUE);

  /* Enable Device interrupts */
  USB_INTEN |=  bmUSB_DevStatusInterrupt |(USB_SOF_EVENT ? bmUSB_FrameInterrupt : 0);
  /* enable USB */
  USB_DEVCMDSTAT_bit.DEV_EN = 1;
}

/*************************************************************************
 * Function Name: USB_RealizeEp
 * Parameters: const UsbStandardEpDescriptor_t * pEP_Desc,
 *             const UsbEP_ExtData_t * pUsbEP_ExtData,
 *             Boolean Enable
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: Enable or disable an endpoint
 *
 *************************************************************************/
USB_ErrorCodes_t USB_RealizeEp(const UsbStandardEpDescriptor_t * pEP_Desc,
                               const UsbEP_ExtData_t * pUsbEP_ExtData,
                               Boolean Enable)
{

USB_Endpoint_t EP = (USB_Endpoint_t)USB_EpLogToPhysAdd(pEP_Desc->bEndpointAddress);
Int32U Mask = (1 << EP);
Int16U Offset,Offset1;
pEpCnfg_t p_ep_cnfg = &EpCnfg[EP];
  if (Enable)
  {
    /* Set EP status */
    p_ep_cnfg->Status  = NOT_READY;
    /* Init EP flags */
    p_ep_cnfg->Flags = 0;
    p_ep_cnfg->bDoubleBuffered = pUsbEP_ExtData->DoubleBuff && UsbEpDoubleBuffType[EP];
    /* Set endpoint type */
    p_ep_cnfg->EpType = (UsbEpTransferType_t)pEP_Desc->bmAttributes.TransferType;
    /* Init EP max packet size */
    p_ep_cnfg->MaxSize = pEP_Desc->wMaxPacketSize;

    if(!USB_AllocateBuffer(&Offset,p_ep_cnfg->MaxSize,EP))
    {
      return(USB_MEMORY_FULL);
    }
    if( p_ep_cnfg->bDoubleBuffered ||
       (CTRL_ENP_OUT == EP))
    {
      /* Allocate packet second buffer */
      if(!USB_AllocateBuffer(&Offset1,p_ep_cnfg->MaxSize,EP))
      {
        /* release buffer */
        USB_ReleaseBuffer(EP);
        return(USB_MEMORY_FULL);
      }
    }
    p_ep_cnfg->pEpCtrl = pUSB_EP_Desc + (EP*2);
    /* single buffered EP */
    p_ep_cnfg->HwBuffer0 = (Int32U)USB_PacketMemory + Offset;
    p_ep_cnfg->HwBuffer1 = (Int32U)USB_PacketMemory + Offset1;

    USB_EPBUFCFG &= ~Mask;
    USB_EPINUSE  &= ~Mask;
    /* disable, reset toggle bit and set EP type */
    p_ep_cnfg->pEpCtrl->Data = (((CTRL_ENP_OUT != EP) && (CTRL_ENP_IN != EP))?bmUSB_DescToggleRst:0);
    if(UsbEpTransferIsochronous == p_ep_cnfg->EpType)
    {
      p_ep_cnfg->pEpCtrl->Data |= bmUSB_DescIso;
      IsoTransEP |= 1ul << EP;
    }

    /* init buffer offset */
    p_ep_cnfg->pEpCtrl->BufferOffset = p_ep_cnfg->HwBuffer0 >> 6;
    /* init buffer size */
    p_ep_cnfg->pEpCtrl->Size = p_ep_cnfg->MaxSize;
    p_ep_cnfg->pEpCtrl->A = !(EP & 1);

    if(p_ep_cnfg->bDoubleBuffered)
    {
      USB_EPBUFCFG |= Mask;
      /* double buffered EP */
      /* disable, reset toggle bit and set EP type */
      (p_ep_cnfg->pEpCtrl+1)->Data = (UsbEpTransferIsochronous == p_ep_cnfg->EpType)?bmUSB_DescIso:0;
      /* init buffer offset */
      (p_ep_cnfg->pEpCtrl+1)->BufferOffset = p_ep_cnfg->HwBuffer1 >> 6;
      /* init buffer size */
      (p_ep_cnfg->pEpCtrl+1)->Size = p_ep_cnfg->MaxSize;
      (p_ep_cnfg->pEpCtrl+1)->A = !(EP & 1);
    }
    else if (CTRL_ENP_OUT == EP)
    {
      /* Setup packet buffer */
      /* init buffer offset */
      (p_ep_cnfg->pEpCtrl+1)->Data = (p_ep_cnfg->HwBuffer1 >> 6) & 0xFFFF;
    }

    /* Enable endpoint interrupt */
    USB_INTEN |= Mask;
  }
  else
  {
    /* Disable endpoint interrupt */
    USB_INTEN &= ~Mask;
    p_ep_cnfg->pEpCtrl->Data = bmUSB_DescDis;
    if(UsbEpTransferIsochronous == p_ep_cnfg->EpType)
    {
      IsoTransEP &= ~(1ul << EP);
    }
    if(p_ep_cnfg->bDoubleBuffered)
    {
      (p_ep_cnfg->pEpCtrl+1)->Data = bmUSB_DescDis;
    }
    p_ep_cnfg->MaxSize = 0;
  }
  return(USB_OK);
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
  USB_DEVCMDSTAT_bit.DEV_ADDR = DevAdd;
}

/*************************************************************************
 * Function Name: USB_ConnectRes
 * Parameters: Boolean Conn
 *
 * Return: none
 *
 * Description: Connect USB
 *
 *************************************************************************/
void USB_ConnectRes (Boolean Conn)
{
  USB_DEVCMDSTAT_bit.DCON = Conn;
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
}

#if USB_REMOTE_WAKEUP != 0
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
  USBCLKCTRL_bit.AP_CLK = 1;
  USB_DEVCMDSTAT_bit.DSUS = 0;
}
#endif /* USB_REMOTE_WAKEUP != 0 */

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
    return(!!USB_DevStatus.Connect);
  case USB_SuspendStatus:
    return(!!USB_DevStatus.Suspend);
  case USB_ResetStatus:
    return(!!USB_DevStatus.Reset);
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
void USB_SetStallEP (USB_Endpoint_t EP, Boolean Stall)
{
Boolean CurrStallStatus;
pEpCnfg_t p_ep_cnfg = &EpCnfg[EP];

  USB_GetStallEP(EP,&CurrStallStatus);
  if(CurrStallStatus != Stall)
  {

    if(Stall)
    {
      USB_DectiveEP(EP);
      p_ep_cnfg->Status = STALLED;
    }

    /* reset toggle bit when ustall the EP */
    p_ep_cnfg->SwCurrBuffer   = !!(USB_EPINUSE & (1ul << EP));
    p_ep_cnfg->pEpCtrl->A     = 0;
    p_ep_cnfg->pEpCtrl->S     = Stall;
    p_ep_cnfg->pEpCtrl->RF_TV = 0;
    p_ep_cnfg->pEpCtrl->TR    = !Stall && !p_ep_cnfg->SwCurrBuffer;
    if(p_ep_cnfg->bDoubleBuffered)
    {
      (p_ep_cnfg->pEpCtrl+1)->A     = 0;
      (p_ep_cnfg->pEpCtrl+1)->S     = Stall;
      (p_ep_cnfg->pEpCtrl+1)->RF_TV = 0;
      (p_ep_cnfg->pEpCtrl+1)->TR    = !Stall && p_ep_cnfg->SwCurrBuffer;
    }

    if(!Stall)
    {
      p_ep_cnfg->Status = NOT_READY;
      if(!EP & 1)
      {
        /* activer OUT EPs after unstall */
        /* init buffer offset */
        p_ep_cnfg->pEpCtrl->BufferOffset = p_ep_cnfg->HwBuffer0 >> 6;
        /* init buffer size */
        p_ep_cnfg->pEpCtrl->Size = p_ep_cnfg->MaxSize;
        /* disable, reset toggle bit and set EP type */
        p_ep_cnfg->pEpCtrl->A = 1;
      }
    }
  }
}

/*************************************************************************
 * Function Name: USB_StallCtrlEP
 * Parameters: none
 *
 * Return: none
 *
 * Description: Stall both direction of the CTRL EP
 *
 *************************************************************************/
void USB_StallCtrlEP (void)
{
  EpCnfg[CTRL_ENP_OUT].Status     = STALLED;
  EpCnfg[CTRL_ENP_OUT].pEpCtrl->A = 0;
  EpCnfg[CTRL_ENP_OUT].pEpCtrl->S = 1;
  EpCnfg[CTRL_ENP_IN].Status      = STALLED;
  EpCnfg[CTRL_ENP_IN].pEpCtrl->A  = 0;
  EpCnfg[CTRL_ENP_IN].pEpCtrl->S  = 1;
}

/*************************************************************************
 * Function Name: USB_GetStallEP
 * Parameters: USB_Endpoint_t EP, pBoolean pStall
 *
 * Return: none
 *
 * Description: Get stall state of the endpoint
 *
 *************************************************************************/
void USB_GetStallEP (USB_Endpoint_t EP, pBoolean pStall)
{
pEpCnfg_t p_ep_cnfg = &EpCnfg[EP];

  *pStall = p_ep_cnfg->pEpCtrl->S != 0;
}

/*************************************************************************
 * Function Name: USB_EP_IO
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: none
 *
 * Description: Endpoint Write (IN)
 *
 *************************************************************************/
void USB_EP_IO(USB_Endpoint_t EP)
{
Int32U Count;
pEpCnfg_t p_ep_cnfg = &EpCnfg[EP];
pInt8U p_buffer;
USB_EpDeskTbl_t * p_ep_ctrl;

  if(   (p_ep_cnfg->Status != BEGIN_SERVICED)
     && (p_ep_cnfg->Status != NO_SERVICED)
     )
  {
    return;
  }
  if(EP & 1)
  {
    while(1)
    {
      Count = p_ep_cnfg->Size - p_ep_cnfg->Offset;
      if(Count == 0 && !p_ep_cnfg->bZeroPacket)
      {
        break;
      }

      /* Get adress if current buffer and EP control entry */
      p_buffer = (pInt8U)p_ep_cnfg->HwBuffer0;
      p_ep_ctrl = p_ep_cnfg->pEpCtrl;
      if(p_ep_cnfg->bDoubleBuffered)
      {
        if(p_ep_cnfg->SwCurrBuffer)
        {
          p_buffer = (pInt8U)p_ep_cnfg->HwBuffer1;
          p_ep_ctrl = p_ep_cnfg->pEpCtrl+1;
        }
        if(p_ep_ctrl->A)
        {
          /* no more free buffers*/
          break;
        }
      }
      if(p_ep_ctrl->A)
      {
        break;
      }

      /* Set Status */
      p_ep_cnfg->Status = BEGIN_SERVICED;
      /* Get data size */
      if(p_ep_cnfg->bZeroPacketPossible && Count == p_ep_cnfg->MaxSize)
      {
        p_ep_cnfg->bZeroPacketPossible = 0;
        p_ep_cnfg->bZeroPacket = 1;
      }
      Count = MIN(Count,p_ep_cnfg->MaxSize);
      if(Count)
      {
        memcpy(p_buffer,p_ep_cnfg->pBuffer+p_ep_cnfg->Offset,Count);
        p_ep_cnfg->Offset += Count;
      }
      p_ep_cnfg->SwCurrBuffer ^= 1;
      p_ep_ctrl->Size = Count;
      p_ep_ctrl->BufferOffset = (Int32U)p_buffer>>6;
      p_ep_ctrl->A = 1;
      if(!p_ep_cnfg->bDoubleBuffered)
      {
        break;
      }
    }

    if(!p_ep_cnfg->bZeroPacket && !Count)
    {
      p_ep_cnfg->Status = COMPLETE;
      /* call callback function */
      if(p_ep_cnfg->pFn)
      {
        ((void(*)(USB_Endpoint_t))p_ep_cnfg->pFn)(EP);
      }
    }
  }
  else
  {
    while(1)
    {
      /* Get adress if current buffer and EP control entry */
      p_buffer = (pInt8U)p_ep_cnfg->HwBuffer0;
      p_ep_ctrl = p_ep_cnfg->pEpCtrl;
      if(p_ep_cnfg->bDoubleBuffered)
      {
        if(p_ep_cnfg->SwCurrBuffer)
        {
          p_buffer = (pInt8U)p_ep_cnfg->HwBuffer1;
          p_ep_ctrl = p_ep_cnfg->pEpCtrl+1;
        }
        if(p_ep_ctrl->A)
        {
          /* no more filled buffers*/
          break;
        }
      }
      if(p_ep_ctrl->A)
      {
        break;
      }

      /* Get data size */
      Count =  p_ep_cnfg->MaxSize - p_ep_ctrl->Size;
      if(Count > (p_ep_cnfg->Size - p_ep_cnfg->Offset))
      {
        p_ep_cnfg->Status = BUFFER_OVERRUN;
        Count = p_ep_cnfg->Size - p_ep_cnfg->Offset;
      }
      else if (Count < p_ep_cnfg->MaxSize)
      {
        p_ep_cnfg->Status = BUFFER_UNDERRUN;
        p_ep_cnfg->Size = p_ep_cnfg->Offset + Count;
      }
      else
      {
        p_ep_cnfg->Status = BEGIN_SERVICED;
      }

      memcpy(p_ep_cnfg->pBuffer+p_ep_cnfg->Offset,p_buffer,Count);
      p_ep_cnfg->Offset += Count;
      p_ep_cnfg->SwCurrBuffer ^= 1;

      /* Activade EP's buffer */
      p_ep_ctrl->Size = p_ep_cnfg->MaxSize;
      p_ep_ctrl->BufferOffset = (Int32U)p_buffer>>6;
      p_ep_ctrl->A = 1;

      if (!(Count = (p_ep_cnfg->Size - p_ep_cnfg->Offset)))
      {
        p_ep_cnfg->Status = COMPLETE;
        break;
      }
    }

    if (p_ep_cnfg->Status != BEGIN_SERVICED && p_ep_cnfg->Status != NO_SERVICED)
    {
      /* call callback function */
      if(p_ep_cnfg->pFn)
      {
        ((void(*)(USB_Endpoint_t))p_ep_cnfg->pFn)(EP);
      }
    }
  }
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

#if USB_SOF_EVENT > 0
/*************************************************************************
 * Function Name: USB_GetFrameNumb
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Return current value of SOF number
 *
 *************************************************************************/
Int32U USB_GetFrameNumb (void)
{
  return(USB_SofNumbHold);
}
#endif

/*************************************************************************
 * Function Name: USB_StatusPhase
 * Parameters: Boolean In
 *
 * Return: none
 *
 * Description: Prepare status phase
 *
 *************************************************************************/
void USB_StatusPhase (Boolean In)
{
  if(In)
  {
    USB_IO_Data(CTRL_ENP_IN,NULL,0,NULL);
  }
}

#if USB_REMOTE_WAKEUP
/*************************************************************************
 * Function Name: USBWakeup_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB interrupt subroutine
 *
 *************************************************************************/
void USBWakeup_IRQHandler (void)
{
  USB_WKPINTR_ENTRY_HOOK();
  USB_WKPINTR_EXIT_HOOK();
  NVIC_ClrPend(NVIC_USB_WAKEUP);
}
#endif

#if USB_FRAME_PRIORITY || USB_DEV_PRIORITY || USB_EP_PRIORITY
/*************************************************************************
 * Function Name: USB_FIQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB interrupt subroutine
 *
 *************************************************************************/
void USB_FIQHandler (void)
{
Int32U intr;
  USB_FIQINTR_ENTRY_HOOK();
  intr  =  USB_INTSTAT;
  intr &=  USB_INTROUTING;

#if USB_DEV_PRIORITY
  if(intr & bmUSB_DevStatusInterrupt)
  {
    USB_INTSTAT = bmUSB_DevStatusInterrupt;
    Int32U dev_stat = USB_DEVCMDSTAT;
    dev_stat &= ~USB_DEVCMDSTAT_WR_1_CLR_MASK;
    if(USB_DEVCMDSTAT_bit.DCON_C)
    {
      USB_DEVCMDSTAT = dev_stat | bmUSB_ConnectChange;
      UsbDevConnectCallback(USB_DEVCMDSTAT_bit.DCON);
    }
    if(USB_DEVCMDSTAT_bit.DSUS_C)
    {
      USB_DEVCMDSTAT = dev_stat | bmUSB_SuspendChange;
      UsbDevSuspendCallback(USB_DEVCMDSTAT_bit.DSUS);
    }
    if(USB_DEVCMDSTAT_bit.DRES_C)
    {
      USB_DEVCMDSTAT = dev_stat | bmUSB_BusReset;
      USB_HwReset();
      UsbDevResetCallback();
    }
  }
#endif

#if USB_FRAME_PRIORITY
  if(intr & bmUSB_FrameInterrupt)
  {
    USB_INTSTAT = bmUSB_FrameInterrupt;
    USB_Endpoint_t ep = ENP1_OUT;
    Int32U iso = IsoTransEP;
    Int32U mask = bmUSB_Ep1InterruptOut;
    while(iso)
    {
      if(mask & iso)
      {
         USB_EP_IO(ep);
      }
      mask  <<= 1;
      ++ep;
    }
#if USB_SOF_EVENT > 0
  #if USB_SOF_FRAME_NUMB > 0
    USB_FRAME_HOOK(INFO_bit.FRAME_NR);
  #else
    USB_FRAME_HOOK(0);
  #endif
#endif
  }
#endif

  intr &= 0x3FF;
  Int32U mask = bmUSB_EP0InterruptOut;
  USB_Endpoint_t ep = CTRL_ENP_OUT;
  while(intr)
  {
    if(intr & mask)
    {
      USB_INTSTAT = mask;
      intr &= ~mask;
      if(   (CTRL_ENP_OUT == ep)
         && USB_DEVCMDSTAT_bit.SETUP
         )
      {
        /* Setup packet */
        /* init IO to receive Setup packet */
        USB_IO_Data(CTRL_ENP_OUT,NULL,(Int32U)-1,NULL);
        USB_IO_Data(CTRL_ENP_IN,NULL, (Int32U)-1,NULL);
        /* Retet In and OUT Endpoints */
        EpCnfg[CTRL_ENP_OUT].pEpCtrl->Data = bmUSB_DescAct
                                     | ((0x3FF & EpCnfg[CTRL_ENP_OUT].MaxSize) << 16)
                                     | ((EpCnfg[CTRL_ENP_OUT].HwBuffer0 >> 6)& 0xFFFF)
                                     ;
        EpCnfg[CTRL_ENP_IN].pEpCtrl->Data = 0;
        /* Clear setup flasg */
        Int32U dev_stat = USB_DEVCMDSTAT;
        dev_stat &= ~USB_DEVCMDSTAT_WR_1_CLR_MASK;
        USB_DEVCMDSTAT = dev_stat | bmUSB_EpSetupPacket;

        memcpy(UsbEp0SetupPacket.Data, (void *)EpCnfg[CTRL_ENP_OUT].HwBuffer1, sizeof(UsbSetupPacket_t));

        /* reset EP IO ctrl */
        if (UsbEp0SetupPacket.mRequestType.Dir == UsbDevice2Host)
        {
          USB_StatusHandler(CTRL_ENP_OUT);
        }

        USB_SetupHandler();

        if(EpCnfg[CTRL_ENP_OUT].Status == STALLED)
        {
          USB_StallCtrlEP();
        }
      }
      else
      {
        if(ep == CTRL_ENP_OUT)
        {
          if(UsbEp0SetupPacket.mRequestType.Dir == UsbDevice2Host &&
             EpCnfg[CTRL_ENP_OUT].pFn)
          {
            ((void(*)(USB_Endpoint_t))EpCnfg[CTRL_ENP_OUT].pFn)(CTRL_ENP_OUT);
            break;
          }
        }
        else if(ep == CTRL_ENP_IN)
        {
          if(UsbEp0SetupPacket.mRequestType.Dir == UsbHost2Device &&
             EpCnfg[CTRL_ENP_IN].pFn)
          {
            ((void(*)(USB_Endpoint_t))EpCnfg[CTRL_ENP_IN].pFn)(CTRL_ENP_IN);
            break;
          }
        }
        USB_EP_IO(ep);
      }
    }
    ep++;
    mask <<= 1;
  }

  USB_FIQINTR_EXIT_HOOK();
  NVIC_ClrPend(NVIC_USB_FIQ);
}
#endif

/*************************************************************************
 * Function Name: USB_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB interrupt subroutine
 *
 *************************************************************************/
void USB_IRQHandler (void)
{

Int32U intr;
  USB_INTR_ENTRY_HOOK();
  intr  =  USB_INTSTAT;
  intr &= ~USB_INTROUTING;

#if 0 == USB_DEV_PRIORITY
  if(intr & bmUSB_DevStatusInterrupt)
  {
    USB_INTSTAT = bmUSB_DevStatusInterrupt;
    Int32U dev_stat = USB_DEVCMDSTAT;
    dev_stat &= ~DEVCMDSTAT_WR_1_CLR_MASK;
    if(USB_DEVCMDSTAT_bit.DCON_C)
    {
      USB_DEVCMDSTAT = dev_stat | bmUSB_ConnectChange;
      UsbDevConnectCallback(USB_DEVCMDSTAT_bit.DCON);
    }
    if(USB_DEVCMDSTAT_bit.DSUS_C)
    {
      USB_DEVCMDSTAT = dev_stat | bmUSB_SuspendChange;
      UsbDevSuspendCallback(USB_DEVCMDSTAT_bit.DSUS);
    }
    if(USB_DEVCMDSTAT_bit.DRES_C)
    {
      USB_DEVCMDSTAT = dev_stat | bmUSB_BusReset;
      USB_HwReset();
      UsbDevResetCallback();
    }
  }
#endif

#if 0 == USB_FRAME_PRIORITY
  if(intr & bmUSB_FrameInterrupt)
  {
    USB_INTSTAT = bmUSB_FrameInterrupt;
    USB_Endpoint_t ep = ENP1_OUT;
    Int32U iso = IsoTransEP;
    Int32U mask = bmUSB_Ep1InterruptOut;
    while(iso)
    {
      if(mask & iso)
      {
         USB_EP_IO(ep);
      }
      mask  <<= 1;
      ++ep;
    }
#if USB_SOF_EVENT > 0
  #if USB_SOF_FRAME_NUMB > 0
    USB_FRAME_HOOK(INFO_bit.FRAME_NR);
  #else
    USB_FRAME_HOOK(0);
  #endif
#endif
  }
#endif

  intr &= 0x3FF;
  Int32U mask = bmUSB_EP0InterruptOut;
  USB_Endpoint_t ep = CTRL_ENP_OUT;
  while(intr)
  {
    if(intr & mask)
    {
      USB_INTSTAT = mask;
      intr &= ~mask;
      if(   (CTRL_ENP_OUT == ep)
         && USB_DEVCMDSTAT_bit.SETUP
         )
      {
        /* Setup packet */
        /* init IO to receive Setup packet */
        USB_IO_Data(CTRL_ENP_OUT,NULL,(Int32U)-1,NULL);
        USB_IO_Data(CTRL_ENP_IN,NULL, (Int32U)-1,NULL);
        /* Retet In and OUT Endpoints */
        EpCnfg[CTRL_ENP_OUT].pEpCtrl->Data = bmUSB_DescAct
                                     | ((0x3FF & EpCnfg[CTRL_ENP_OUT].MaxSize) << 16)
                                     | ((EpCnfg[CTRL_ENP_OUT].HwBuffer0 >> 6) & 0xFFFF)
                                     ;
        EpCnfg[CTRL_ENP_IN].pEpCtrl->Data = 0;
        /* Clear setup flasg */
        Int32U dev_stat = USB_DEVCMDSTAT;
        dev_stat &= ~DEVCMDSTAT_WR_1_CLR_MASK;
        USB_DEVCMDSTAT = dev_stat | bmUSB_EpSetupPacket;

        memcpy(UsbEp0SetupPacket.Data, (void *)EpCnfg[CTRL_ENP_OUT].HwBuffer1, sizeof(UsbSetupPacket_t));

        /* reset EP IO ctrl */
        if (UsbEp0SetupPacket.mRequestType.Dir == UsbDevice2Host)
        {
          USB_StatusHandler(CTRL_ENP_OUT);
        }

        USB_SetupHandler();

        if(EpCnfg[CTRL_ENP_OUT].Status == STALLED)
        {
          USB_StallCtrlEP();
        }
      }
      else
      {
        if(ep == CTRL_ENP_OUT)
        {
          if(UsbEp0SetupPacket.mRequestType.Dir == UsbDevice2Host &&
             EpCnfg[CTRL_ENP_OUT].pFn)
          {
            ((void(*)(USB_Endpoint_t))EpCnfg[CTRL_ENP_OUT].pFn)(CTRL_ENP_OUT);
            break;
          }
        }
        else if(ep == CTRL_ENP_IN)
        {
          if(UsbEp0SetupPacket.mRequestType.Dir == UsbHost2Device &&
             EpCnfg[CTRL_ENP_IN].pFn)
          {
            ((void(*)(USB_Endpoint_t))EpCnfg[CTRL_ENP_IN].pFn)(CTRL_ENP_IN);
            break;
          }
        }
        USB_EP_IO(ep);
      }
    }
    ep++;
    mask <<= 1;
  }

  USB_INTR_EXIT_HOOK();
  NVIC_ClrPend(NVIC_USB_IRQ);
}
