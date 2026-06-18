/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    File name   : usb_hw.c
 *    Description : usb module (HAL)
 *
 *    History :
 *    1. Date        : June 3, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *        USB framework rev. 2 light version with DMA transfer support
 *    2. Date        : 10, November 2009
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Adapt for KSK-i.MX25
 *
 *    $Revision: 4265 $
 **************************************************************************/
#define USB_HW_GLOBAL
#include "usb_hw.h"

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

static const UsbEP_ExtData_t USB_CtrlEpExtDescr1 =
{
  0
};

#if  USB_SOF_EVENT > 0
Int32U  USB_SofNumbHold;
#endif


#pragma section="DATA_NO_CACHE"

#pragma location="DATA_NO_CACHE"
#pragma data_alignment=4096
static __no_init dHQ_t _dHQ[ENP_MAX_NUMB];

#pragma location="DATA_NO_CACHE"
#pragma data_alignment=64
static __no_init dTD_t _dTD[ENP_MAX_NUMB];



/*************************************************************************
 * Function Name: _FlushEP
 * Parameters: USB_Endpoint_t EP
 *
 * Return: none
 *
 * Description: Flush EP
 *
 *************************************************************************/
static
void _FlushEP(USB_Endpoint_t EP)
{
Int32U mask = 1UL << EP;
  // Flush Pipe
  do
  {
    UOG_ENDPTFLUSH = mask;
    while (UOG_ENDPTFLUSH & mask);
  }
  while(UOG_ENDPTSTAT & mask);
}

/*************************************************************************
 * Function Name: _InitdDT
 * Parameters: pdTD_t pdTD, pInt8U  pBuffer, Int32U Size
 *
 * Return: none
 *
 * Description: Set TD Descriptor
 *
 *************************************************************************/
static
void _InitdDT(pdTD_t pdTD, pInt8U  pBuffer, Int32U Size)
{
Int32U tmp;
pInt32U p_buff;
  // 1. Set the terminate bit to “1”.
  pdTD->T = 1;
  // 2. Fill in total bytes with transfer size.
  pdTD->TotalBytes = Size;
  // 3. Set the interrupt on complete if desired.
  pdTD->IOC = 1;
  // 4. Initialize the status field with the active bit set to “1” and all remaining status bits set to “0”.
  pdTD->Status = (1UL<<7);
  // 5. Fill in buffer pointer page 0 and the current offset to point to the start of the data buffer.
  pdTD->BufferPage0 = (Int32U)pBuffer;

  tmp = 4096 - ((4096-1) & (Int32U)pBuffer);
  if(tmp < Size)
  {
    Size    -=  tmp;
    p_buff   = &pdTD->BufferPage1;
    pBuffer +=  tmp;

    for(Int32U i = 0; i < 4; i++)
    {
      *p_buff = (Int32U)pBuffer;
      if(4096 >= Size)
      {
        break;
      }
      pBuffer += 4096;
      Size    -= 4096;
    }
  }
}

/*************************************************************************
 * Function Name: _InitdDT
 * Parameters: USB_Endpoint_t EP, pdTD_t pdTD
 *
 * Return: Boolean
 *
 * Description: Prime EP
 *
 *************************************************************************/
static
Boolean _PrimePipe(USB_Endpoint_t EP, pdTD_t pdTD)
{
pdHQ_t p_dhq = _dHQ + EP;
Int32U mask = (EP&1)?1<<((EP>>1)+16):1<<(EP>>1);

  assert(0 == p_dhq->dTD.Status);
  // Case 1: Link list is empty
  // 1. Write dQH next pointer AND dQH terminate bit to 0 as a single DWord operation.
  p_dhq->dTD.NextPointer = (Int32U)pdTD & ~1UL;
  for(volatile Int32U i = 25; i; i--);
  // 2. Clear active & halt bit in dQH (in case set from a previous error).
  p_dhq->dTD.Status = 0;
  // 3. Prime endpoint by writing ‘1’ to correct bit position in ENDPTPRIME.
  UOG_ENDPTPRIME = mask;
  if (    0 == (EP & ~1UL)
      &&  (1 & UOG_ENDPTSETUPSTAT)
     )
  {
    return(0);
  }
  return(1);
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
  // Init SOF number hold
  #if  USB_SOF_EVENT > 0
  USB_SofNumbHold = 0;
  #endif

  // Enable needed clocks
  MCR &= ~((1UL << 31) | // External 24 MHz clock
           (1UL << 16))  // USB clk from UPLL output
             ;
  CGCR0_bit.hclk_usbotg = 1;

  // Disable USB interrupts
  ASIC_DisableIntSource(INT_USBOTG);

  // Init USB Phy
  // Phy register Init
  UOG_PORTSC1_bit.PTS   = 0;            // UTMI/UTMI+
  UOG_PORTSC1_bit.PHCD  = 0;            // Enable PHY clk
  USB_PHY_CTRL_TEST     = 0;

  // Embedded Phy Init
  USB_PHY_CTRL_FUNC_bit.utmi_reset = 1;
  USB_PHY_CTRL_FUNC_bit.utmi_reset = 0;
  USB_PHY_CTRL_FUNC_bit.utmi_USBEN = 1;

  while(0 == USB_OTG_MIRROR_bit.OTGUTMICLK);

  // Init USB device controller
  USB_CTRL_bit.OSIC = 0;    // clear phy type field
  USB_CTRL_bit.OWIE = 1;    // OTG Wake-up Interrupt Enable
  USB_CTRL_bit.OPM  = 1;    // The USBPWR and OC pins are not used by the OTG core.

  // Disconnect device
  USB_ConnectRes(FALSE);

  // Set address 0
  USB_SetDefAdd();

  // Init controls endpoints
  USB_HwReset();
  // Init Device status
  UsbSetDevState(UsbDevStatusUnknow);

  // Enable USB interrupts
  // USB interrupt connect to VIC
  // USB interrupt enable
  ASIC_SetupIntr(USB_ISR,FALSE,INT_USBOTG,USB_INTR_PRIORITY);
  ASIC_EnableIntSource(INT_USBOTG);
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
  // Clear all setup token semaphores
  UOG_ENDPTSETUPSTAT = UOG_ENDPTSETUPSTAT;
  // Clear all the endpoint complete status bits
  UOG_ENDPTCOMPLETE = UOG_ENDPTCOMPLETE;
  // Cancel all primed EP
  UOG_ENDPTFLUSH = 0xFFFFFFFF;

  // Clear pending interrupt
  UOG_USBSTS  = (1UL << 0)  // UI
              | (1UL << 1)  // UEI
              | (1UL << 2)  // PCI
              | (1UL << 6)  // URI
              | (1UL << 8)  // SLI
               ;

  // Init dHQ
  for (Int32U i = 2; i < ENP_MAX_NUMB; i++)
  {
    _dHQ[i].IOS        = 0;
    _dHQ[i].MaxPack    = 0;
    _dHQ[i].ZL         = 1;
    _dHQ[i].Mult       = 0;
    _dHQ[i].dTD.Status = 0;
    _dHQ[i].dTD.T      = 1;
  }

  // Control EP Init
  USB_RealizeEp(&USB_CtrlEpDescr0,&USB_CtrlEpExtDescr0,TRUE);
  USB_RealizeEp(&USB_CtrlEpDescr1,&USB_CtrlEpExtDescr1,TRUE);

  // set address of dHQ
  UOG_ENDPOINTLISTADDR = (Int32U)&_dHQ;

  // Init endpoints registers
  Int32U *p_ep_reg = (pInt32U)&UOG_ENDPTCTRL1;
  for (Int32U i = 1; i < GET_HW_PIPE_NUMB(); i++)
  {
    *p_ep_reg = (2UL << 2) // BUILK_RX
              | (2UL <<18) // BUILK_TX
             ;
    ++p_ep_reg;
  }

  UOG_USBMODE_bit.CM   = 2;   // device only controller
  UOG_USBMODE_bit.SLOM = 1;   // Setup Lockouts Off

  // Enable Device interrupts
  UOG_USBINTR = (1UL << 0)  // UI
              | (1UL << 1)  // UEI
              | (1UL << 2)  // PCI
              | (USB_ERROR_EVENT ? (1UL << 4) : 0)  // SEI
              | (1UL << 6)  // URI
              | (USB_SOF_EVENT ? (1UL << 7) : 0)    // SRE
              | (1UL << 8)  // SLI
              ;
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
USB_Endpoint_t ep = (USB_Endpoint_t)USB_EpLogToPhysAdd(pEP_Desc->bEndpointAddress);
pInt32U p_ep_reg = ((pInt32U)&UOG_ENDPTCTRL0) + (ep>>1);
pdHQ_t p_dhq;
pEpCnfg_t p_ep;

  _FlushEP(ep);
  if (ep & 1)
  {
    *p_ep_reg &= ~(1UL<<23);  // TXE
  }
  else
  {
    *p_ep_reg &= ~(1UL<< 7);  // RXE
  }

  if (Enable)
  {
    // Set EP status
    p_ep = EpCnfg + ep;
    p_ep->Status  = NOT_READY;
    // Init EP flags
    p_ep->Flags = 0;
    // Set endpoint type
    p_ep->EpType = (UsbEpTransferType_t)pEP_Desc->bmAttributes.TransferType;
    // Init EP max packet size
    p_ep->MaxSize = pEP_Desc->wMaxPacketSize;

    p_dhq = _dHQ + ep;

    p_dhq->IOS        = 0;
    p_dhq->MaxPack    = p_ep->MaxSize;
    p_dhq->ZL         = 0;
    p_dhq->Mult       = 0;
    p_dhq->dTD.Status = 0;
    p_dhq->dTD.T      = 1;

    if (ep & 1)
    {
      *p_ep_reg = (*p_ep_reg & 0x0000FFFFUL)
                | (1UL<<22) // TXR
                | (1UL<<23) // TXE
                ;
    }
    else
    {
      *p_ep_reg = (*p_ep_reg & 0xFFFF0000UL)
                | (1UL<< 6) // RXR
                | (1UL<< 7) // RXE
                ;
    }

    switch (p_ep->EpType)
    {
    case UsbEpTransferIsochronous:

      if (ep & 1)
      {
        *p_ep_reg |=(1UL<<18) // ISO_TX
                  ;
      }
      else
      {
        *p_ep_reg |= (1UL<< 2) // ISO_RX
                  ;
      }
      break;
    case UsbEpTransferBulk:
      if (ep & 1)
      {
        *p_ep_reg |= (2UL<<18) // BUILK_TX
                  ;
      }
      else
      {
        *p_ep_reg |= (2UL<< 2) // BUILK_RX
                  ;
      }
      break;
    case UsbEpTransferInterrupt:
      if (ep & 1)
      {
        *p_ep_reg |= (3UL<<18) // INT_TX
                  ;
      }
      else
      {
        *p_ep_reg |= (3UL<< 2) // INT_RX
                  ;
      }
      break;
    default:
       p_dhq->IOS = 1;
      if (ep & 1)
      {
        *p_ep_reg |= (0UL<<18) // CTRL_TX
                  ;
      }
      else
      {
        *p_ep_reg |= (0UL<< 2) // CTRL_RX
                  ;
      }
    }
  }
  else
  {
    EpCnfg[ep].MaxSize = 0;
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
  UOG_DEVICEADDR_bit.USBADR = DevAdd;
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
  UOG_USBCMD_bit.RS = Conn;
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
  UOG_PORTSC1_bit.FPR = 1;
}
#endif // USB_REMOTE_WAKEUP != 0

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
pInt32U p_ep_reg = ((pInt32U)&UOG_ENDPTCTRL0) + (EP>>1);
Boolean CurrStallStatus;
Int32U mask;

  USB_GetStallEP(EP,&CurrStallStatus);
  if(CurrStallStatus != Stall)
  {
    if(1 & EP)
    {
      mask = 1UL << 0;
    }
    else
    {
      mask = 1UL << 16;
    }

    if(Stall)
    {
      // Stall EP
      *p_ep_reg |=  mask;
    }
    else
    {
      // Unstall EP
      *p_ep_reg &= ~mask;
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
  // Stall both directions
  UOG_ENDPTCTRL0 |= (1UL<<0) | (1UL<<16);
}

/*************************************************************************
 * Function Name: USB_GetStallEP
 * Parameters: USB_Endpoint_t EndPoint, pBoolean pStall
 *
 * Return: none
 *
 * Description: Get stall state of the endpoint
 *
 *************************************************************************/
void USB_GetStallEP (USB_Endpoint_t EP, pBoolean pStall)
{
pInt32U p_ep_reg = ((pInt32U)&UOG_ENDPTCTRL0) + (EP>>1);
Int32U mask;
  mask = (1 & EP)?(1UL<<0):(1UL<<16);
  *pStall = (mask & *p_ep_reg) != 0;
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
pdTD_t p_dtd;
pEpCnfg_t p_ep = EpCnfg + EP;

  if(p_ep->Status != BEGIN_SERVICED &&
     p_ep->Status != NO_SERVICED)
  {
    volatile Int32U dummy = 0;
    return;
  }
  p_dtd = _dTD + EP;
  p_ep->Status = BEGIN_SERVICED;
  _InitdDT(p_dtd,p_ep->pBuffer,p_ep->Size);
  _PrimePipe(EP,p_dtd);
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
  else
  {
    USB_IO_Data(CTRL_ENP_OUT,NULL,0,NULL);
  }
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
Int32U intr_status = UOG_USBSTS;

  USB_INTR_ENTRY_HOOK();

  if((1UL << 6) & intr_status)  // URI
  {
    // Reset interrupt
    USB_HwReset();
    UsbDevResetCallback();
    UsbDevSuspendCallback(FALSE);
  }

  if((1UL << 2) & intr_status)  // PCI
  {
    // Port change
    UOG_USBSTS = 1UL << 2;
  }

  if ((1UL << 8) & intr_status) // SLI
  {
    // Suspend
    UOG_USBSTS = (1UL << 8);
    USB_CTRL  |= 1UL << 27;     // OTG Wake-up Interrupt enable
    UsbDevSuspendCallback(TRUE);
  }
  if ((1UL << 31) & USB_CTRL)
  {
    USB_CTRL &= ~(1UL << 27);   // OTG Wake-up Interrupt Disable
    UsbDevSuspendCallback(FALSE);
  }
  if ((1UL << 7) & intr_status) // SRI
  {
    // SOF
    UOG_USBSTS = (1UL << 7);
    USB_FRAME_HOOK(0);
  }
  if ((1UL << 0) & intr_status) // USB transaction
  {

    UOG_USBSTS = 1UL << 0;
    // Setup (RX)
    if (UOG_ENDPTSETUPSTAT_bit.ENDPTSETUPSTAT0)
    {
      UOG_ENDPTSETUPSTAT = 1;
      UOG_ENDPTCOMPLETE  = 1UL | (1UL << 16);
      do
      {
        UOG_USBCMD_bit.SUTW = 1;
        memcpy(UsbEp0SetupPacket.Data,_dHQ[CTRL_ENP_OUT].SetUpBuff,sizeof(UsbEp0SetupPacket));
      }
      while(0 == UOG_USBCMD_bit.SUTW);
      UOG_USBCMD_bit.SUTW = 0;
      // reset pending IN data
      //_FlushEP(CTRL_ENP_IN);
      USB_IO_Data(CTRL_ENP_IN,NULL,(Int32U)-1,NULL);
      USB_IO_Data(CTRL_ENP_OUT,NULL,(Int32U)-1,NULL);
#if 0
      // Prepare status OUT phase
      if (   (UsbDevice2Host == UsbEp0SetupPacket.mRequestType.Dir)
          && (0              == UsbEp0SetupPacket.wLength.Word)
         )
      {
        USB_StatusHandler(CTRL_ENP_OUT);
      }
#endif

      // Processing Setup packet
      USB_SetupHandler();

      if(STALLED == EpCnfg[CTRL_ENP_OUT].Status)
      {
        // Error or unsupported request
        USB_StallCtrlEP();
      }
    }
    else
    {

      Int32U comp_status;
      if (0 != (comp_status = UOG_ENDPTCOMPLETE))
      {
        Int32U mask;
        USB_Endpoint_t ep;
        // In (TX)
        for (ep = CTRL_ENP_IN, mask = 1UL<<16;
                 ep < ENP_MAX_NUMB
              && comp_status & 0xFFFF0000;
              ep += 2, mask <<= 1)
        {
          if (comp_status & mask)
          {
            comp_status ^= mask;
            UOG_ENDPTCOMPLETE = mask;
            if(0 == _dTD[ep].Status)
            {
              EpCnfg[ep].Status = COMPLETE;
            }
            else
            {
              EpCnfg[ep].Status   = DMA_ERROR;
            }

            EpCnfg[ep].Offset = _dTD[ep].TotalBytes;
            if(EpCnfg[ep].pFn)
            {
              ((void(*)(USB_Endpoint_t))EpCnfg[ep].pFn)(ep);
            }
          }
        }
        // Out (RX)
        for (ep = CTRL_ENP_OUT, mask = 1UL;
                 ep < ENP_MAX_NUMB
              && comp_status & 0x0000FFFF;
              ep += 2, mask <<= 1)
        {
          if (comp_status & mask)
          {
            comp_status ^= mask;
            UOG_ENDPTCOMPLETE = mask;
            if(0 == _dTD[ep].Status)
            {
              EpCnfg[ep].Status = COMPLETE;
            }
            else if((1UL<<5) & _dTD[ep].Status) // Data Buffer Error
            {
              _dTD[ep].TotalBytes = EpCnfg[ep].Size;
              EpCnfg[ep].Status   = BUFFER_OVERRUN;
            }
            else
            {
              _dTD[ep].TotalBytes = EpCnfg[ep].Size;
              EpCnfg[ep].Status   = DMA_ERROR;
            }

            EpCnfg[ep].Offset = EpCnfg[ep].Size - _dTD[ep].TotalBytes;
            if(EpCnfg[ep].pFn)
            {
              ((void(*)(USB_Endpoint_t))EpCnfg[ep].pFn)(ep);
            }
          }
        }
      }
    }
  }

  if ((1UL << 1) & intr_status) // USB Error Interrupt
  {
    UOG_USBSTS = 1UL << 1;
  }

  USB_INTR_EXIT_HOOK();
}
