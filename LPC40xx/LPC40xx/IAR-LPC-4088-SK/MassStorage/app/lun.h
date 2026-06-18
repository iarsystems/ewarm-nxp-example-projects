/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : lun.h
 *    Description : LUN common definitions module
 *
 *    History :
 *    1. Date        : November 15, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Datå        : November , 2012
 *       Author      : Stoyan Choynev
 *       Description : Fix support for more than one Lun
 *
 *    $Revision: 28 $
 **************************************************************************/
#ifndef __LUN_H
#define __LUN_H

#ifdef  LUN_GOBALS
#define LUN_EXTERN
#else
#define LUN_EXTERN extern
#endif

typedef enum _LunState_t
{
  LunCommandDecode = 0, LunRead, LunWrite, LunVerify
} LunState_t;

typedef enum _LunMessages_t
{
  LunInitMsg = 0, LunResetReqMsg, LunInquiryReqMsg,
  LunTestUntilReadyReqMsg,LunModeSense6ReqMsg, LunReadCapacity10ReqMsg,
  LunRead10ReqMsg, LunWrite10ReqMsg,
  LunVerify10ReqMsg, LunVerify10BytChkReqMsg,
  LunReadFormatCapacityReqMsg,
  LunDataReadyMsg
} LunMessages_t;

typedef struct _LunFpnt_t
{
  DiskInitFpnt_t   DiskInitFpnt;
  DiskStatusFpnt_t DiskStatusFpnt;
  DiskIoFpnt_t     DiskIoFpnt;
  uint8_t *        DiskInquiry;
  uint32_t         DiskInquirySize;
} LunFpnt_t, *pLunFpnt_t;

extern uint8_t Lun0Buffer[];

/*************************************************************************
 * Function Name: LunInit
 * Parameters: uint32_t LunInd,
 *             LunFpnt_t * LunFunDesc
 *
 * Return: none
 *
 * Description: LUN Init
 *
 *************************************************************************/
void LunInit (uint32_t LunInd,LunFpnt_t * LunFunDesc);

/*************************************************************************
 * Function Name: LunImp
 * Parameters:  uint32_t LunInd
 *
 * Return: Boolean 0 - not activity
 *                 1 - activity
 *
 * Description: LUN commands implementation
 *
 *************************************************************************/
Boolean LunImp (uint32_t LunInd);

#endif //__LUN_H
