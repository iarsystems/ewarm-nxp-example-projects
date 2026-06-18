/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    File name   : usb_hw.h
 *    Description : usb module (HAL) include file
 *
 *    History :
 *    1. Date        : June 16, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/

#include "includes.h"

#ifndef __USB_HW_H
#define __USB_HW_H

#ifdef USB_HW_GLOBAL
#define USB_HW_EXTERN
#else
#define USB_HW_EXTERN  extern
#endif

typedef union _UsbDevStat_t
{
  Int32U Data;
  struct
  {
    Int32U                 :16;
    Int32U  Connect        : 1;
    Int32U  Suspend        : 1;
    Int32U                 : 6;
    Int32U  ConnectChange  : 1;
    Int32U  SuspendChange  : 1;
    Int32U  Reset          : 1;
    Int32U                 : 1;
    Int32U  VBus           : 1;
    Int32U                 : 3;
  };
} UsbDevStat_t;

typedef enum _USB_DevStatusReqType_t
{
  USB_DevConnectStatus = 0, USB_SuspendStatus, USB_ResetStatus
} USB_DevStatusReqType_t;

typedef enum _USB_ErrorCodes_t
{
  USB_OK = 0,USB_PLL_ERROR, USB_INTR_ERROR,
  USB_EP_OCCUPIER, USB_MEMORY_FULL, USB_BUF_OVERFLOW,
  USB_EP_NOT_VALID, UB_EP_SETUP_UNDERRUN, USB_EP_STALLED,
  UB_EP_SETUP_OVERWRITE, USB_EP_FATAL_ERROR
} USB_ErrorCodes_t;

typedef enum _USB_Endpoint_t
{
  CTRL_ENP_OUT=0, CTRL_ENP_IN,
  ENP1_OUT      , ENP1_IN    ,
  ENP2_OUT      , ENP2_IN    ,
  ENP3_OUT      , ENP3_IN    ,
  ENP4_OUT      , ENP4_IN    ,
  ENP_MAX_NUMB
} USB_Endpoint_t;

typedef enum _USB_IO_Status_t
{
  NOT_READY = 0, NO_SERVICED, BEGIN_SERVICED, COMPLETE, BUFFER_UNDERRUN, BUFFER_OVERRUN,
  SETUP_OVERWRITE, STALLED, NOT_VALID
} USB_IO_Status_t;

#define bmUSB_EP0InterruptOut       0x00000001
#define bmUSB_EP0InterruptIn        0x00000002
#define bmUSB_Ep1InterruptOut       0x00000004
#define bmUSB_Ep1InterruptIn        0x00000008
#define bmUSB_Ep2InterruptOut       0x00000010
#define bmUSB_Ep2InterruptIn        0x00000020
#define bmUSB_Ep3InterruptOut       0x00000040
#define bmUSB_Ep3InterruptIn        0x00000080
#define bmUSB_Ep4InterruptOut       0x00000100
#define bmUSB_Ep4InterruptIn        0x00000200
#define bmUSB_FrameInterrupt        0x40000000
#define bmUSB_DevStatusInterrupt    0x80000200

#define bmUSB_Connect               0x00010000
#define bmUSB_ConnectChange         0x01000000
#define bmUSB_Suspend               0x00020000
#define bmUSB_SuspendChange         0x02000000
#define bmUSB_BusReset              0x04000000
#define bmUSB_VBus                  0x10000000

#define bmUSB_EpSetupPacket         0x00000100
#define bmUSB_EpStall               0x00000001
#define bmUSB_EpStallStatus         0x00000002
#define bmUSB_EpPOStatus            0x00000010
#define bmUSB_EpCondStall           0x00000080

#define DEVCMDSTAT_WR_1_CLR_MASK    (bmUSB_EpSetupPacket | bmUSB_ConnectChange | bmUSB_SuspendChange | bmUSB_BusReset)

#define USB_EP_VALID(pEP) (0 != pEP->MaxSize)

typedef struct _PacketMemUse_t
{
  USB_Endpoint_t EpAddr;
  Int16U         Start;
  Int16U         Size;
  struct _PacketMemUse_t * pNext;
} PacketMemUse_t, *pPacketMemUse_t;

typedef struct _UsbEP_ExtData_t
{
  Boolean DoubleBuff;
} UsbEP_ExtData_t, *pUsbEP_ExtData_t;

typedef union _USB_EpDeskTbl_t
{
  Int32U Data;
  struct
  {
    Int32U    BufferOffset  :16;
    Int32U    Size          :10;
    Int32U    T             : 1;
    Int32U    RF_TV         : 1;
    Int32U    TR            : 1;
    Int32U    S             : 1;
    Int32U    D             : 1;
    Int32U    A             : 1;
  };
} USB_EpDeskTbl_t, *pUSB_EpDeskTbl_t;

#define bmUSB_DescAct       0x80000000
#define bmUSB_DescDis       0x40000000
#define bmUSB_DescStall     0x20000000
#define bmUSB_DescToggleRst 0x10000000
#define bmUSB_DescToggle    0x08000000
#define bmUSB_DescIso       0x04000000

typedef struct _EpCnfg_t
{
  Int32U              MaxSize;
  UsbEpTransferType_t EpType;
  void *              pFn;
  Int32U              Offset;
  Int32U              Size;
  USB_IO_Status_t     Status;
  pInt8U              pBuffer;
  Int32U              HwBuffer0;
  Int32U              HwBuffer1;
  pUSB_EpDeskTbl_t    pEpCtrl;
  union
  {
    Int8U Flags;
    struct
    {
      Int8U bDoubleBuffered     : 1;
      Int8U SwCurrBuffer        : 1;
      Int8U bZeroPacket         : 1;
      Int8U bZeroPacketPossible : 1;
    };
  };
} EpCnfg_t, *pEpCnfg_t;

USB_HW_EXTERN EpCnfg_t EpCnfg[ENP_MAX_NUMB];

/*************************************************************************
 * Function Name: USB_HwInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init USB
 *
 *************************************************************************/
void USB_HwInit(void);

/*************************************************************************
 * Function Name: USB_HwReset
 * Parameters: none
 *
 * Return: none
 *
 * Description: Reset USB engine
 *
 *************************************************************************/
void USB_HwReset (void);

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
                               Boolean Enable);

/*************************************************************************
 * Function Name: USB_SetAdd
 * Parameters: Int32U DevAdd - device address between 0 - 127
 *
 * Return: none
 *
 * Description: Set device address
 *
 *************************************************************************/
void USB_SetAdd(Int32U DevAdd);
#define USB_SetDefAdd() USB_SetAdd(0)

/*************************************************************************
 * Function Name: USB_ConnectRes
 * Parameters: Boolean Conn
 *
 * Return: none
 *
 * Description: Connect USB
 *
 *************************************************************************/
void USB_ConnectRes (Boolean Conn);

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
void USB_Configure (Boolean Configure);

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
void USB_WakeUp (void);
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
Boolean USB_GetDevStatus (USB_DevStatusReqType_t Type);

/*************************************************************************
 * Function Name: USB_SetStallEP
 * Parameters: USB_Endpoint_t EndPoint, Boolean Stall
 *
 * Return: none
 *
 * Description: The endpoint stall/unstall
 *
 *************************************************************************/
void USB_SetStallEP (USB_Endpoint_t EndPoint, Boolean Stall);

/*************************************************************************
 * Function Name: USB_GetStallEP
 * Parameters: USB_Endpoint_t EndPoint, pBoolean pStall
 *
 * Return: none
 *
 * Description: Stall both direction of the CTRL EP
 *
 *************************************************************************/
void USB_StallCtrlEP (void);

/*************************************************************************
 * Function Name: USB_GetStallEP
 * Parameters: USB_Endpoint_t EndPoint, pBoolean pStall
 *
 * Return: none
 *
 * Description: Get stall state of the endpoint
 *
 *************************************************************************/
void USB_GetStallEP (USB_Endpoint_t EndPoint, pBoolean pStall);

/*************************************************************************
 * Function Name: USB_EP_IO
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: none
 *
 * Description: Endpoint Write (IN)
 *
 *************************************************************************/
void USB_EP_IO(USB_Endpoint_t EP);

/*************************************************************************
 * Function Name: USB_EpLogToPhysAdd
 * Parameters: Int8U EpLogAdd
 *
 * Return: USB_Endpoint_t
 *
 * Description: Convert the logical to physical address
 *
 *************************************************************************/
USB_Endpoint_t USB_EpLogToPhysAdd (Int8U EpLogAdd);

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
Int32U USB_GetFrameNumb (void);
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
void USB_StatusPhase (Boolean In);

/*************************************************************************
 * Function Name: USB_ISR
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB interrupt subroutine
 *
 *************************************************************************/
void USB_ISR (void);

#endif //__USB_HW_H
