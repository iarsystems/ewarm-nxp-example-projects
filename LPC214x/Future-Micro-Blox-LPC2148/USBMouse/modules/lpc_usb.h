/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : lpc_usb.h
 *    Description : Define USB module
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
 *    4. Date        : December 20, 2005
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Change user function prototype
 *
 *    $Revision: 28 $
**************************************************************************/

#include "includes.h"

#ifndef __LPC_USB_H
#define __LPC_USB_H

#ifdef LPC_USB_GLOBAL
#define LPC_USB_EXTERN
#else
#define LPC_USB_EXTERN  extern
#endif

typedef enum
{
  USB_OK = 0,USB_PLL_ERROR,USB_INTR_ERROR,
} USB_ErrorCodes_t;

typedef enum
{
  CTRL_ENP_OUT=0, CTRL_ENP_IN,
  ENP1_OUT      , ENP1_IN    ,
  ENP2_OUT      , ENP2_IN    ,
  ENP3_OUT      , ENP3_IN    ,
  ENP4_OUT      , ENP4_IN    ,
  ENP5_OUT      , ENP5_IN    ,
  ENP6_OUT      , ENP6_IN    ,
  ENP7_OUT      , ENP7_IN    ,
  ENP8_OUT      , ENP8_IN    ,
  ENP9_OUT      , ENP9_IN    ,
  ENP10_OUT     , ENP10_IN   ,
  ENP11_OUT     , ENP11_IN   ,
  ENP12_OUT     , ENP12_IN   ,
  ENP13_OUT     , ENP13_IN   ,
  ENP14_OUT     , ENP14_IN   ,
  ENP15_OUT     , ENP15_IN   ,
  ENP_MAX_NUMB
} USB_Endpoint_t;

typedef enum
{
  USB_LowPrior = 0,
  USB_HighPrior,
} USB_IntrPriority_t;

typedef enum
{
  USB_NotFast = 0,
  USB_EP_Fast,
  USB_FrameFast,
} USB_DevIntrPriority_t;

typedef union
{
  Int32U Data;
  struct
  {
    Int32U Frame      : 1;
    Int32U Fast       : 1;
    Int32U Slow       : 1;
    Int32U Status     : 1;
    Int32U CcEmpty    : 1;
    Int32U CdFull     : 1;
    Int32U EpRx       : 1;
    Int32U EpTx       : 1;
    Int32U EpRealize  : 1;
    Int32U Error      : 1;
    Int32U            :22;
  };
}UsbDevIntrStat_t;

typedef union
{
  Int8U Data;
  struct
  {
    Int8U  Connect        : 1;
    Int8U  ConnectChange  : 1;
    Int8U  Suspend        : 1;
    Int8U  SuspendChange  : 1;
    Int8U  Reset          : 1;
    Int8U                 : 3;
  };
}UsbDevStat_t;

typedef enum
{
  UsbEp0Out = 0, UsbEp0In,
  UsbEp1Out,  UsbEp1In,
  UsbEp2Out,  UsbEp2In,
  UsbEp3Out,  UsbEp3In,
  UsbEp4Out,  UsbEp4In,
  UsbEp5Out,  UsbEp5In,
  UsbEp6Out,  UsbEp6In,
  UsbEp7Out,  UsbEp7In,
  UsbEp8Out,  UsbEp8In,
  UsbEp9Out,  UsbEp9In,
  UsbEp10Out, UsbEp10In,
  UsbEp11Out, UsbEp11In,
  UsbEp12Out, UsbEp12In,
  UsbEp13Out, UsbEp13In,
  UsbEp14Out, UsbEp14In,
  UsbEp15Out, UsbEp15In,
  UsbResetEvent,UsbConnectEvent,UsbSuspendEvent,
  UsbErrorEvent,UsbSofEvent,UsbHighPrioIntrEvent,
  UsbLastEvent
}UsbUserEvent_t;

typedef enum
{
  USB_DevConnectStatus = 0, USB_SuspendStatus, USB_ResetStatus
} USB_DevStatusReqType_t;

typedef Int32U UsbDefStatus_t;

typedef void (* UserFunc_t)(void * Arg);

#define CMD_USB_SEL_EP              0x00
#define CMD_USB_SEL_CLR_INT_EP      0x40
#define CMD_USB_SET_EP_STAT         0x140
#define CMD_USB_SET_ADDRESS         0xD0
#define CMD_USB_CFG_DEV             0xD8
#define CMD_USB_CLR_BUF             0xF2
#define CMD_USB_SET_MODE            0xF3
#define CMD_USB_RD_FRAME_NUMB       0xF5
#define CMD_USB_VAL_BUF             0xFA
#define CMD_USB_RD_ERROR_STAT       0xFB
#define CMD_USB_RD_TEST_REG         0xFD
#define CMD_USB_SET_DEV_STAT        0x1FE
#define CMD_USB_GET_DEV_STAT        0xFE
#define CMD_USB_GET_ERROR           0xFF

#define USB_CMD_WR                  0x00000500
#define USB_DATA_WR                 0x00000100
#define USB_DATA_RD                 0x00000200

#define bmUSB_FrameInterrupt        0x00000001
#define bmUSB_FastInterrupt         0x00000002
#define bmUSB_SlowInterrupt         0x00000004
#define bmUSB_DevStatusInterrupt    0x00000008
#define bmUSB_CommRegEmptyInterrupt 0x00000010
#define bmUSB_CommDataFullInterrupt 0x00000020
#define bmUSB_RxPacketInterrupt     0x00000040
#define bmUSB_TxPacketInterrupt     0x00000080
#define bmUSB_EPRealizeInterrupt    0x00000100
#define bmUSB_ErrorInterrupt        0x00000200

#define bmUSB_Connect               0x00000001
#define bmUSB_ConnectChange         0x00000002
#define bmUSB_Suspend               0x00000004
#define bmUSB_SuspendChange         0x00000008
#define bmUSB_BusReset              0x00000010

#define bmUSB_EpStall               0x00000001
#define bmUSB_EpStallStatus         0x00000002
#define bmUSB_EpSetupPacket         0x00000004
#define bmUSB_EpPOStatus            0x00000010
#define bmUSB_EpCondStall           0x00000080

#define bmUSB_CtrlRdEna             0x00000001
#define bmUSB_CtrlWrEna             0x00000002

#define bmUSB_PacketOverWritten     0x00000001

LPC_USB_EXTERN volatile Int32U USB_EpSoftPrio;
extern UserFunc_t UsbUserFun[];

#if USB_SOF_EVENT > 0
LPC_USB_EXTERN volatile Int32U USB_SofNumbHold;
#endif

/*************************************************************************
 * Function Name: USB_Cmd
 * Parameters:  Int16U Command, Int8U Data
 *
 * Return: Int32U - command result
 *
 * Description: Implement commands transmit to USB Engine
 *
 *************************************************************************/
Int32U USB_Cmd (Int16U Command, Int8U Data);

/*************************************************************************
 * Function Name: USB_UserFuncRegistering
 * Parameters: UserFunc_t UserFunc, UsbUserEvent_t UserFuncInd
 *
 * Return: UserFunc_t
 *
 * Description: Registered User callback function
 *
 *************************************************************************/
UserFunc_t USB_UserFuncRegistering (UserFunc_t UserFunc,
                                   UsbUserEvent_t UserFuncInd);

/*************************************************************************
 * Function Name: USB_EpIntrClr
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: Int8U
 *
 * Description: Clear the EP interrupt flag and return the current EP status
 *
 *************************************************************************/
Int8U USB_EpIntrClr(USB_Endpoint_t EndPoint);

/*************************************************************************
 * Function Name: USB_HwInit
 * Parameters: LPC_VicIrqSlots_t IntrSlot
 *
 * Return: none
 *
 * Description: Init USB
 *
 *************************************************************************/
void USB_HwInit(LPC_VicIrqSlots_t IntrSlot);

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
 * Function Name: USB_SetEpPrio
 * Parameters: USB_Endpoint_t EndPoint,Boolean EpPrio
 *
 * Return: none
 *
 * Description: Set Endpoint priority
 *
 *************************************************************************/
inline void USB_SetEpPrio (USB_Endpoint_t EndPoint,Boolean EpPrio);

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
                             Int32U MaxPacketSize, Boolean IntrEna);

/*************************************************************************
 * Function Name: USB_ClearBuffer
 * Parameters: USB_Endpoint_t EndPoint - endpoint index
 *
 * Return: Int32U Packet overwrite status
 *
 * Description: Clear buffer of the corresponding endpoint
 *
 *************************************************************************/
Int32U USB_ClearBuffer(USB_Endpoint_t EndPoint);

/*************************************************************************
 * Function Name: USB_ValidateBuffer
 * Parameters: USB_Endpoint_t EndPoint - endpoint index
 *
 * Return: none
 *
 * Description: Validate buffer(only for IN endpoints)
 *
 *************************************************************************/
void USB_ValidateBuffer(USB_Endpoint_t EndPoint);

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
 * Function Name: USB_Connect
 * Parameters: Boolean Conn
 *
 * Return: none
 *
 * Description: Connect USB
 *
 *************************************************************************/
void USB_Connect (Boolean Conn);

/*************************************************************************
 * Function Name: USB_Configure
 * Parameters: Boolean Configure
 *
 * Return: none
 *
 * Description: Configure device
 *        Configure != 0 enable all Realize EP
 *
 *************************************************************************/
void USB_Configure (Boolean Configure);

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
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: Boolean
 *
 * Description: Get stall state of the endpoint
 *
 *************************************************************************/
Boolean USB_GetStallEP (USB_Endpoint_t EndPoint);

/*************************************************************************
 * Function Name: USB_EpWrite
 * Parameters: USB_Endpoint_t EndPoint, Int32U * pData, Int32U Count
 *
 * Return: none
 *
 * Description: Endpoint Write (IN)
 *
 *************************************************************************/
void USB_EpWrite (USB_Endpoint_t EndPoint, Int32U * pData, Int32U Count);

/*************************************************************************
 * Function Name: USB_EpRead
 * Parameters: USB_Endpoint_t EndPoint, Int32U * pData, Int32U Count
 *
 * Return: Int32
 *
 * Description: Endpoint Read (OUT)
 *
 *************************************************************************/
Int32U USB_EpRead (USB_Endpoint_t EndPoint, Int32U * pData, Int32U Count);

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
Int32U USB_GetFrameNumb (void);
#endif

#if USB_DMA > 0
typedef enum
{
  UsbDmaNoServiced = 0,UsbDmaBeingServiced,
  UsbDmaNormalCompletion, UsbDmaDataUnderrun,
  UsbDmaDataOverrun = 8, UsbDmaSystemError,
  UsbDmaParametersError,
} UsbDmaStateCode_t;

typedef enum
{
  UsbDmaNormalMode = 0, UsbDmaAtleMode,
} UsbDmaMode_t;

#pragma pack(1)
typedef struct
{
  Int32U PacketLength       :16;
  Int32U PacketValid        : 1;
  Int32U FrameNumb          :15;
} DmaIsoPacket_t, *pDmaIsoPacket_t;

typedef struct
{
  void * pNextDD;
  union
  {
    Int32U Ctrl;
    struct
    {
      Int32U DmaMode        : 2;
      Int32U NextDDValid    : 1;
      Int32U                : 1;
      Int32U Isochronous    : 1;
      Int32U MaxPacketSize  :11;
      Int32U DmaBufferLegtn :16;
    };
  };
  pInt32U pDmaBuffer;
  union
  {
    Int32U Status;
    struct
    {
      Int32U  DdRetired     : 1;
      Int32U  DdState       : 4;
      Int32U  PacketValid   : 1;
      Int32U  LsByteExtr    : 1;
      Int32U  MsByteExtr    : 1;
      Int32U  MessLenPos    : 6;
      Int32U                : 2;
      Int32U  PresentCnt    :16;
    };
  };
  pDmaIsoPacket_t pDmaIsoPacket;
} USB_DmaDesc_t, *pUSB_DmaDesc_t;
#pragma pack()

/*************************************************************************
 * Function Name: USB_DmaReset
 * Parameters:  Int32U IntrEna
 *
 * Return: none
 *
 * Description: Reset USB DMA
 *
 *************************************************************************/
void USB_DmaReset (Int32U IntrEna);

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
          pDmaIsoPacket_t pDmaIsoPacket, Boolean EpTransferType);

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
void USB_DmaRestattTransfer (USB_Endpoint_t EndPoint,Int32U DmaDescInd);


/*************************************************************************
 * Function Name: USB_DmaGetDesc
 * Parameters: Int32U DmaDescInd
 *
 * Return: pUSB_DmaDesc_t
 *
 * Description: Return pointer to DMA descriptor
 *
 *************************************************************************/
pUSB_DmaDesc_t USB_DmaGetDesc (Int32U DmaDescInd);

/*************************************************************************
 * Function Name: USB_DmaDisable
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: none
 *
 * Description: Disable DMA transfer for the EP
 *
 *************************************************************************/
void USB_DmaDisable (USB_Endpoint_t EndPoint);

#endif  // USB_DMA
#endif //__LPC_USB_H
