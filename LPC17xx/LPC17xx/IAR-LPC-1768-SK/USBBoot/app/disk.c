/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : disk.c
 *    Description : disk driver
 *
 *    History :
 *    1. Date        : November, 2009
 *       Author      : Stoyan Choynev
 *       Description : Create
 *    $Revision: 28 $
 **************************************************************************/
#include "disk.h"
#include "fat.h"
#include "DiskImg.h"
#include <string.h>
#include "Flash.h" 

static void _Read(pInt8U pData, Int32U Add);
static void _Write(pInt8U pData, Int32U Add);
static void _Verify(pInt8U pData, Int32U Add);

static DiskCtrlBlk_t _DskCtrlBlk;
static Boolean       _bHC;


/*************************************************************************
 * Function Name: _Read
 * Parameters: pInt8U pData, Int32U Add, Int32U Length
 *
 * Return: none
 *
 * Description: Read from disk
 *
 *************************************************************************/
static void _Read(pInt8U pData, Int32U Add)
{
  if (Add < RSVD_SECTORS*SECTOR_SIZE)
	{
    memcpy(pData,&BootSect,DEF_BLK_SIZE);
  }
	else if (Add < (RSVD_SECTORS*SECTOR_SIZE + FAT_DATA_SIZE))
	     {

         Add += (Int32U)&Fat - RSVD_SECTORS*SECTOR_SIZE;
         memcpy(pData,(void const *)Add,DEF_BLK_SIZE);
		   }
       else if(Add < (FAT_SIZE))
            {
              Add += (Int32U) &DirEntry -(RSVD_SECTORS*SECTOR_SIZE + FAT_DATA_SIZE);
              memcpy(pData,(void const *)Add,DEF_BLK_SIZE);
            }
            else
            {
              Add -= (FAT_SIZE);
#if (CRP == NONPROT)
              memcpy(pData,(void const *)Add,DEF_BLK_SIZE);
#else
              /*Chip is read protected!!!*/
              memset(pData,0,DEF_BLK_SIZE);
#endif
            }
}
/*************************************************************************
 * Function Name: _Write
 * Parameters: pInt8U pData, Int32U Add, Int32U Length
 *
 * Return: none
 *
 * Description: Write to disk
 *
 *************************************************************************/
static void _Write(pInt8U pData, Int32U Add)
{
  if (Add < RSVD_SECTORS*SECTOR_SIZE)
	{/*Can't write boot sector*/
  }
	else if (Add < (RSVD_SECTORS*SECTOR_SIZE + FAT_DATA_SIZE))
	     {/*Write Fat table*/
         Add += (Int32U)&Fat - RSVD_SECTORS*SECTOR_SIZE;
         memcpy((void *)Add,pData,DEF_BLK_SIZE);
		   }
       else if(Add < (FAT_SIZE))
            {/**/
              Add -= (RSVD_SECTORS*SECTOR_SIZE + FAT_DATA_SIZE);
              for(int i = 0; i<DEF_BLK_SIZE; i++,Add++,pData++)
              {
                if((0 ==(Add%32)) && (0xE5 == *pData))
                {/*Erase file*/
                  pdir_entry_t pfile = (pdir_entry_t)((Int32U)DirEntry+Add);
                  if(0xE5 != pfile->DIR_Name[0])
                  {/*File exist here!!! So we can erase it*/
                    void * startaddr = (void *)((pfile->DIR_FstClusLO-2)*\
                                        SECT_PER_CLUSTER*SECTOR_SIZE);
                    Int32U filesize = pfile->DIR_FileSize;
#if (CRP != NONPROT)
                    if(0 == startaddr)
                    {/*Erase whole flash if chip is protected and the first
                       sector is to be erassed. If you want to unprotect the
                       chip juts delite the USBBOOT.BIN. This however will delete
                       the whole chip memory.*/
                      filesize = FLASH_MEMORY_SIZE;
                    }
#endif
                    FlashErase(startaddr,filesize);
                  }
                }
              }
            }
            else
            {/*Write flash*/
              FlashWrite((Add-FAT_SIZE),DEF_BLK_SIZE,pData);
            }
}

/*************************************************************************
 * Function Name: _Verify
 * Parameters: pInt8U pData, Int32U Add, Int32U Length
 *
 * Return: None
 *
 * Description: None
 *
 *************************************************************************/
static void _Verify(pInt8U pData, Int32U Add)
{
}


/*************************************************************************
 * Function Name: DiskInit
 * Parameters:  none
 *
 * Return: none
 *
 * Description: Init disk parameters
 *
 *************************************************************************/
void DiskInit (void)
{
  _DskCtrlBlk.BlockNumb = TOTAL_SECT;
  _DskCtrlBlk.BlockSize= SECTOR_SIZE;
  _DskCtrlBlk.DiskStatus=DiskCommandPass;
  _DskCtrlBlk.DiskType=DiskUnknow;
  _DskCtrlBlk.WriteProtect=0;
  _DskCtrlBlk.MediaChanged=0;
}

/*************************************************************************
 * Function Name: GetDiskCtrlBkl
 * Parameters:  none
 *
 * Return: pDiskCtrlBlk_t
 *
 * Description: Return pointer to status structure of the disk
 *
 *************************************************************************/
pDiskCtrlBlk_t GetDiskCtrlBkl (void)
{
  return(&_DskCtrlBlk);
}

/*************************************************************************
 * Function Name: DiskIO
 * Parameters: pInt8U pData,Int32U BlockStart,
 *             Int32U BlockNum, DiskIoRequest_t IoRequest
 *
 * Return: DiskStatusCode_t
 *
 * Description:disk I/O
 *
 *************************************************************************/
DiskStatusCode_t DiskIO (pInt8U pData,Int32U BlockStart,
                           Int32U BlockNum, DiskIoRequest_t IoRequest)
{
  if((NULL == pData) || (BlockStart+BlockNum > _DskCtrlBlk.BlockNumb))
  {
    return(DiskParametersError);
  }
  if (_DskCtrlBlk.DiskStatus)
  {
    return(_DskCtrlBlk.DiskStatus);
  }
  switch (IoRequest)
  {
  case DiskWrite:
    if(_DskCtrlBlk.WriteProtect)
    {
      return(DiskParametersError);
    }
    while(BlockNum--)
    {
      _Write(pData,_bHC?BlockStart:BlockStart*_DskCtrlBlk.BlockSize);
      BlockStart = _bHC?++BlockStart:BlockStart+_DskCtrlBlk.BlockSize;
    }
    break;
  case DiskRead:
    while(BlockNum--)
    {
      _Read(pData,_bHC?BlockStart:BlockStart*_DskCtrlBlk.BlockSize);
      BlockStart = _bHC?++BlockStart:BlockStart+_DskCtrlBlk.BlockSize;
    }
    break;
  case DiskVerify:
    while(BlockNum--)
    {
      _Verify(pData,_bHC?BlockStart:BlockStart*_DskCtrlBlk.BlockSize);
      BlockStart = _bHC?++BlockStart:BlockStart+_DskCtrlBlk.BlockSize;
    }
    break;
  default:
    return(DiskParametersError);
  }
  return(_DskCtrlBlk.DiskStatus);
}

