/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : scsi.h
 *    Description : SCSI include file
 *
 *    History :
 *    1. Date        : September 27, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : January 27, 2006
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Add Unit Start/Stop command implement
 *    3. Date        : October 23, 2007
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Adapt for the USB framework 2
 *
 *    $Revision: 28 $
 **************************************************************************/
#ifndef __SCSI_H
#define __SCSI_H

#include "bot.h"
#include "scsi_cmd.h"

#ifdef SCSI_GOBALS
#define SCSI_EXTERN
#else
#define SCSI_EXTERN extern
#endif

#define ScsiStallIn     1
#define ScsiStallOut    2
#define ScsiStallBoth   3

typedef struct
{
  uint32_t Message[3];  // Offset 0 Message Type
                      // Implement Only For IO
                      //  Offset 1 BlockAddress
                      //  Offset 2 NumbOfBocks
  ScsiStatusCode_t Status;
  StartStop_t      UnitSSStatus;
} ScsiDrvObj, *pScsiDrvObj;

typedef struct _ScsiEpStrl_t
{
  uint8_t * pData;
  uint32_t  Size;
} ScsiEpStrl_t, *pScsiEpStrl_t;

SCSI_EXTERN uint32_t * pScsiMessage[SCSI_LUN_NUMB];

/*************************************************************************
 * Function Name: ScsiInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init SCSI
 *
 *************************************************************************/
void ScsiInit (void);

/*************************************************************************
 * Function Name: UsbClassBotInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB Bulk-only transport init
 *
 *************************************************************************/
void UsbClassBotInit (void);

/*************************************************************************
 * Function Name: UsbClassBotConfigure
 * Parameters:  pUsbDevCtrl_t pDev
 *
 * Return: none
 *
 * Description: USB Class storage configure
 *
 *************************************************************************/
void UsbClassBotConfigure (pUsbDevCtrl_t pDev);

/*************************************************************************
 * Function Name: UsbBotRstInterface
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB Bulk-only transport interface reset
 *
 *************************************************************************/
void UsbBotRstInterface (void);

/*************************************************************************
 * Function Name: BotInEpStall
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Stall BOT In EP
 *
 *************************************************************************/
void BotInEpStall (void);

/*************************************************************************
 * Function Name: BotOutEpStall
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Stall BOT Out EP
 *
 *************************************************************************/
void BotOutEpStall (void);

/*************************************************************************
 * Function Name: UsbClassBotRequest
 * Parameters: pUsbSetupPacket_t pSetup
 *
 * Return: UsbCommStatus_t
 *
 * Description: Implement USB Class storage requests
 *
 *************************************************************************/
UsbCommStatus_t UsbClassBotRequest (pUsbSetupPacket_t pSetup);

/*************************************************************************
 * Function Name: BotOutEP
 * Parameters:  USB_Endpoint_t EP
 *
 * Return: none
 *
 * Description: USB BOT Out EP handler
 *
 *************************************************************************/
void BotOutEP (USB_Endpoint_t EP);

/*************************************************************************
 * Function Name: BotInEP
 * Parameters:  USB_Endpoint_t EP
 *
 * Return: none
 *
 * Description: USB BOT In EP handler
 *
 *************************************************************************/
void BotInEP (USB_Endpoint_t EP);

/*************************************************************************
 * Function Name: BotUnstallCallBack
 * Parameters: USB_Endpoint_t EP
 *
 * Return: none
 *
 * Description: Unstall callback function
 *
 *************************************************************************/
void BotUnstallCallBack (USB_Endpoint_t EP);

/*************************************************************************
 * Function Name: BotReceiveDataInit
 * Parameters: uint8_t * pData, uint32_t DataSize, Boolean DataComplete
 *
 * Return: BotToUserStatus_t
 *
 * Description: Init Receive
 *
 *************************************************************************/
BotToUserStatus_t BotReceiveDataInit (uint8_t * pData, uint32_t DataSize,
                                      Boolean DataComplete);

/*************************************************************************
 * Function Name: BotSendDataInit
 * Parameters:  const uint8_t * pData, uint32_t DataSize, Boolean DataComplete
 *
 * Return: BotToUserStatus_t
 *
 * Description: Init Transmit
 *
 *************************************************************************/
BotToUserStatus_t BotSendDataInit (const uint8_t * pData, uint32_t DataSize,
                                   Boolean DataComplete);

/*************************************************************************
 * Function Name: BotSendStatus
 * Parameters:  uint32_t DataSize, uint8_t * pData, Boolean DataComplete
 *
 * Return: BotToUserStatus_t
 *
 * Description: Init Transmit of CSW
 *
 *************************************************************************/
void BotSendStatus (void);

/*************************************************************************
 * Function Name: ScsiCommImpl
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Scsi commands implement
 *
 *************************************************************************/
inline void ScsiCommImpl (void);

/*************************************************************************
 * Function Name: ScsiCbwValid
 * Parameters:  none
 *
 * Return: Boolean
 *
 * Description: CBW valid and meaningful check
 *
 *************************************************************************/
inline Boolean ScsiCbwValid(void);

/*************************************************************************
 * Function Name: ScsiTestUnitReadyImp
 * Parameters: none
 *
 * Return: none
 *
 * Description: TEST UNIT READY command implement
 *
 *************************************************************************/
inline void ScsiTestUnitReadyImp (void);

/*************************************************************************
 * Function Name: ScsiTestUntilReadyData
 * Parameters:  uint32_t MediaReady
 *
 * Return: void
 *
 * Description: Prepare  TEST UNIT READY data for sending
 *
 *************************************************************************/
void ScsiTestUntilReadyData (uint32_t MediaReady);

/*************************************************************************
 * Function Name: ScsiRequestSenseImp
 * Parameters:  none
 *
 * Return: Boolean
 *
 * Description: REQUEST SENSE command implement
 *
 *************************************************************************/
inline void ScsiRequestSenseImp (void);

/*************************************************************************
 * Function Name: ScsiModeSelect6Imp
 * Parameters:  none
 *
 * Return: none
 *
 * Description: MODE SELECT command implement
 *
 *************************************************************************/
inline void ScsiModeSelect6Imp (void);

/*************************************************************************
 * Function Name: ScsiModeSense6Imp
 * Parameters:  none
 *
 * Return: none
 *
 * Description: MODE SENSE command implement
 *
 *************************************************************************/
inline void ScsiModeSense6Imp (void);

/*************************************************************************
 * Function Name: ScsiModeSenseData
 * Parameters:  uint32_t WriteProtect
 *
 * Return: none
 *
 * Description: Prepare MODE SENSE data for sending
 *
 *************************************************************************/
void ScsiModeSenseData (uint32_t WriteProtect);

/*************************************************************************
 * Function Name: ScsiInquiryImp
 * Parameters:  none
 *
 * Return: none
 *
 * Description:INQUIRY command implement
 *
 *************************************************************************/
inline void ScsiInquiryImp (void);

/*************************************************************************
 * Function Name: ScsiInquiryData
 * Parameters: const uint8_t * pData, uint32_t Size
 *
 * Return: none
 *
 * Description: Prepare INQUIRY data for sending
 *
 *************************************************************************/
void ScsiInquiryData (const uint8_t * pData, uint32_t Size);

/*************************************************************************
 * Function Name: ScsiMediaRemovalImp
 * Parameters:  none
 *
 * Return: none
 *
 * Description: PREVENT ALLOW MEDIUM REMOVAL command implement
 *
 *************************************************************************/
inline void ScsiMediaRemovalImp(void);

/*************************************************************************
 * Function Name: ScsiReadCapacity10Imp
 * Parameters:  none
 *
 * Return: none
 *
 * Description: READ CAPACITY (10) command implement
 *
 *************************************************************************/
inline void ScsiReadCapacity10Imp (void);

/*************************************************************************
 * Function Name: ScsiReadCapacityData
 * Parameters:  uint32_t LogBlockAdd, uint32_t BlockSize
 *
 * Return: none
 *
 * Description: Prepare READ CAPACITY data for sending
 *
 *************************************************************************/
void ScsiReadCapacityData (uint32_t LogBlockAdd, uint32_t BlockSize);

/*************************************************************************
 * Function Name: ScsiRead10Imp
 * Parameters:  none
 *
 * Return: none
 *
 * Description: READ(10) command implement
 *
 *************************************************************************/
inline void ScsiRead10Imp (void);

/*************************************************************************
 * Function Name: ScsiReadData
 * Parameters: const uint8_t * pData, uint32_t Size, Boolean DataComplete
 *
 * Return: none
 *
 * Description: Prepare READ for sending
 *
 *************************************************************************/
void ScsiReadData(const uint8_t * pData, uint32_t Size, Boolean DataComplete);

/*************************************************************************
 * Function Name: ScsiWrite10Imp
 * Parameters: none
 *
 * Return: none
 *
 * Description: WRITE(10) command implement
 *
 *************************************************************************/
inline void ScsiWrite10Imp (void);

/*************************************************************************
 * Function Name: ScsiMediaStartStopImp
 * Parameters: none
 *
 * Return: none
 *
 * Description: START STOP UNIT command implement
 *
 *************************************************************************/
inline void ScsiMediaStartStopImp (void);

/*************************************************************************
 * Function Name: ScsiWriteData
 * Parameters:  uint8_t * pData, uint32_t Size, Boolean DataComplete
 *
 * Return: none
 *
 * Description: Prepare WRITE data for receiving
 *
 *************************************************************************/
void ScsiWriteData(uint8_t * pData, uint32_t Size, Boolean DataComplete);

/*************************************************************************
 * Function Name: ScsiVerify10Imp
 * Parameters:  none
 *
 * Return: none
 *
 * Description: VERIFY (10) command implement
 *
 *************************************************************************/
void ScsiVerify10Imp (void);

/*************************************************************************
 * Function Name: ScsiReadFormatCapcityImp
 * Parameters:  none
 *
 * Return: none
 *
 * Description: READ FORMAT CAPACITIES implement
 *
 *************************************************************************/
void ScsiReadFormatCapcityImp (void);

/*************************************************************************
 * Function Name: ScsiReadFormatCapcityData
 * Parameters:  uint8_t * pData, uint32_t Size
 *
 * Return: none
 *
 * Description: Prepare Data of READ FORMAT CAPACITIES command request
 *
 *************************************************************************/
void ScsiReadFormatCapcityData (uint8_t * pData, uint32_t Size);

/*************************************************************************
 * Function Name: ScsiCmdStatus
 * Parameters:  ScsiStatusCode_t Status
 *
 * Return: none
 *
 * Description: Prepare Status data for sending
 *
 *************************************************************************/
void ScsiCmdStatus (ScsiStatusCode_t Status);

/*************************************************************************
 * Function Name: ScsiCmdError
 * Parameters:  uint32_t Status, uint32_t Stall
 *
 * Return: none
 *
 * Description: Prepare Error data for sending
 *
 *************************************************************************/
void ScsiCmdError (uint32_t Status, uint32_t Stall);

#endif // __SCSI_H
