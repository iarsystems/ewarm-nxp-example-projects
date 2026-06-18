/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    File name   : usb_hw.h
 *    Description : usb module (HAL) include file
 *
 *    History :
 *    1. Date        : June 3, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/

#include "includes.h"

#ifndef __USB_HW_H
#define __USB_HW_H

#ifdef USB_HW_GLOBAL
#define USB_HW_EXTERN
#else
#define USB_HW_EXTERN  extern
#endif


typedef union _dTD_t
{
  Int8U Data[32];
  struct
  {
    // DWord0
    union
    {
      Int32U NextPointer;
      struct
      {
        Int32U             T : 1;
        Int32U               : 3;
        Int32U _NextPointer  :28;
      };
    };
    // DWord1
    union
    {
      Int32U DWord1;
      struct
      {
        Int32U Status        : 8;
        Int32U               : 2;
        Int32U MultO         : 2;
        Int32U               : 3;
        Int32U IOC           : 1;
        Int32U TotalBytes    :15;
        Int32U               : 1;
      };
    };
    // DWord2
    union
    {
      Int32U BufferPage0;
      struct
      {
        Int32U CurrentOffset :12;
        Int32U _BufferPage0 :20;
      };
    };
    // DWord3
    union
    {
      Int32U BufferPage1;
      struct
      {
        Int32U FrameNumber   :11;
        Int32U               : 1;
        Int32U _BufferPage1  :20;
      };
    };
    // DWord4
    Int32U BufferPage2;
    // DWord5
    Int32U BufferPage3;
    // DWord6
    Int32U BufferPage4;
    // DWord7 (Unused by HW)
    Int8U * pBuffer;
  };
} dTD_t, * pdTD_t;

typedef union _dHQ_t
{
  Int8U Data[64];
  struct
  {
    // DWord 0
    union
    {
      Int32U DWord0;
      struct
      {
        Int32U               :15;
        Int32U IOS           : 1;
        Int32U MaxPack       :11;
        Int32U               : 2;
        Int32U ZL            : 1;
        Int32U Mult          : 2;
      };
    };
    // DWord 1
    Int32U CurrPointer;
    dTD_t  dTD;
    Int8U  SetUpBuff[8];
    Int32U Reserved[4];
  };
} dHQ_t, *pdHQ_t;

typedef enum _USB_DevStatusReqType_t
{
  USB_DevConnectStatus = 0, USB_SuspendStatus, USB_ResetStatus
} USB_DevStatusReqType_t;

typedef enum _USB_ErrorCodes_t
{
  USB_OK = 0,USB_PLL_ERROR, USB_INTR_ERROR,
  USB_EP_OCCUPIER, USB_MEMORY_FULL, USB_BUF_OVERFLOW,
  USB_EP_NOT_VALID, UB_EP_SETUP_UNDERRUN, USB_EP_STALLED,
  UB_EP_SETUP_OVERWRITE, USB_EP_FATAL_ERROR, USB_DMA_DESC_OVERFLOW
} USB_ErrorCodes_t;

typedef enum _USB_Endpoint_t
{
  CTRL_ENP_OUT=0, CTRL_ENP_IN,
  ENP1_OUT      , ENP1_IN    ,
  ENP2_OUT      , ENP2_IN    ,
  ENP3_OUT      , ENP3_IN    ,
  ENP4_OUT      , ENP4_IN    ,
  ENP5_OUT      , ENP5_IN    ,
  ENP6_OUT      , ENP6_IN    ,
  ENP7_OUT      , ENP7_IN    ,
  ENP_MAX_NUMB
} USB_Endpoint_t;

#define GET_HW_PIPE_NUMB()       ((ENP_MAX_NUMB/2)+1)

typedef enum _USB_IO_Status_t
{
  NOT_READY = 0, NO_SERVICED, BEGIN_SERVICED, COMPLETE, BUFFER_UNDERRUN, BUFFER_OVERRUN, DMA_ERROR,
  SETUP_OVERWRITE, STALLED, NOT_VALID
} USB_IO_Status_t;

typedef struct _EpCnfg_t
{
  Int32U              MaxSize;
  UsbEpTransferType_t EpType;
  void *              pFn;
  Int32U              Offset;
  Int32U              Size;
  USB_IO_Status_t     Status;
  pInt8U              pBuffer;
  union
  {
    Int8U Flags;
    struct
    {
      Int8U bZeroPacket         : 1;
      Int8U bZeroPacketPossible : 1;
    };
  };
} EpCnfg_t, *pEpCnfg_t;

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

#define bmUSB_HP_Frame              0x00000001
#define bmUSB_HP_FastEp             0x00000002


#define USB_EP_VALID(pEP) (0 != pEP->MaxSize)

typedef struct _UsbEP_ExtData_t
{
  Boolean DMA_Transfer;
} UsbEP_ExtData_t, *pUsbEP_ExtData_t;

USB_HW_EXTERN EpCnfg_t EpCnfg[ENP_MAX_NUMB];

extern void VIC_SetVectoredIRQ(void(*pIRQSub)(), unsigned int Priority,
                        unsigned int VicIntSource);

/*************************************************************************
 * Function Name: USB_Cmd
 * Parameters:  Int16U Command, Int8U Data
 *
 * Return: Int32U - command result
 *
 * Description: Implement commands transmit to USB Engine
 *
 *************************************************************************/
static
Int32U USB_Cmd (Int16U Command, Int8U Data);

/*************************************************************************
 * Function Name: USB_EpIntrClr
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: Int8U
 *
 * Description: Clear the EP interrupt flag and return the current EP status
 *
 *************************************************************************/
static
Int8U USB_EpIntrClr(USB_Endpoint_t EndPoint);

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
