/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : disk.c
 *    Description : disk driver
 *
 *    History :
 *    1. Date        : November, 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial revision
 *    $Revision: 28 $
 **************************************************************************/
#include "disk.h"
#include "Flash_disk.h"
#include "fat.h"
#include "DiskImg.h"
#include <string.h>
#include "Flash.h" 

static void _Read(uint8_t * pData, uint32_t Add);
static void _Write(uint8_t * pData, uint32_t Add);
static void _Verify(uint8_t * pData, uint32_t Add);

static DiskCtrlBlk_t _DskCtrlBlk;
static __no_init uint8_t Fat[FAT_DATA_SIZE];

/*************************************************************************
 * Function Name: _Read
 * Parameters: uint8_t * pData, uint32_t Add, uint32_t Length
 *
 * Return: none
 *
 * Description: Read from disk
 *
 *************************************************************************/
static void _Read(uint8_t * pData, uint32_t Add)
{
  if (Add < RSVD_SECTORS*SECTOR_SIZE)
	{
    memcpy(pData,&BootSect,DEF_BLK_SIZE);
  }
	else if (Add < (RSVD_SECTORS*SECTOR_SIZE + FAT_DATA_SIZE))
	     {

         Add += (uint32_t)&Fat - RSVD_SECTORS*SECTOR_SIZE;
         memcpy(pData,(void const *)Add,DEF_BLK_SIZE);
		   }
       else if(Add < (FAT_SIZE))
            {
              Add += (uint32_t) &DirEntry -(RSVD_SECTORS*SECTOR_SIZE + FAT_DATA_SIZE);
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
 * Parameters: uint8_t * pData, uint32_t Add, uint32_t Length
 *
 * Return: none
 *
 * Description: Write to disk
 *
 *************************************************************************/
static void _Write(uint8_t * pData, uint32_t Add)
{
  if (Add < RSVD_SECTORS*SECTOR_SIZE)
	{/*Can't write boot sector*/
  }
	else if (Add < (RSVD_SECTORS*SECTOR_SIZE + FAT_DATA_SIZE))
	     {/*Write Fat table*/
         Add += (uint32_t)&Fat - RSVD_SECTORS*SECTOR_SIZE;
         memcpy((void *)Add,pData,DEF_BLK_SIZE);
		   }
       else if(Add < (FAT_SIZE))
            {/**/
              Add -= (RSVD_SECTORS*SECTOR_SIZE + FAT_DATA_SIZE);
              for(int i = 0; i<DEF_BLK_SIZE; i++,Add++,pData++)
              {
                if((0 ==(Add%32)) && (0xE5 == *pData))
                {/*Erase file*/
                  pdir_entry_t pfile = (pdir_entry_t)((uint32_t)DirEntry+Add);
                  if(0xE5 != pfile->DIR_Name[0])
                  {/*File exist here!!! So we can erase it*/
                    void * startaddr = (void *)((pfile->DIR_FstClusLO-2)*\
                                        SECT_PER_CLUSTER*SECTOR_SIZE);
                    uint32_t filesize = pfile->DIR_FileSize;
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
 * Parameters: uint8_t * pData, uint32_t Add, uint32_t Length
 *
 * Return: None
 *
 * Description: None
 *
 *************************************************************************/
static void _Verify(uint8_t * pData, uint32_t Add)
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
uint32_t n,m,cluster,index;

  for(n=0; n < FAT_DATA_SIZE;n++)
  {
    Fat[n] = 0xFF;
  }

  Fat[0]= 0xF8;
  Fat[1]= 0xFF;
  Fat[2]= 0xFF;
  /* Build Fat table*/
  for(int file = 1; file < ROOT_ENR_CNT; file++)
  {
    if(0xE5 != DirEntry[file].DIR_Name[0])
    {
      n = DirEntry[file].DIR_FstClusLO;
      m = n + (DirEntry[file].DIR_FileSize-1)/SECTOR_SIZE/SECT_PER_CLUSTER;
      for(;n <= m;n++)
      {
        if(n == m) cluster = 0xFFF;
        else cluster = n+1;
        index = n*3;
        
        if(index&0x1)
        { 
          index >>= 1;
          Fat[index] &= 0x0F;
          Fat[index] |= (cluster<<4)&0xF0;
          Fat[index+1] = (cluster>>4)&0xFF;
        }
        else
        {
          index >>= 1;
          Fat[index] = (cluster)&0xFF;
          Fat[index+1] &= 0xF0;
          Fat[index+1] |= (cluster>>8)&0x0F;
        }
      }
    }
  }

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
 * Parameters: uint8_t * pData,uint32_t BlockStart,
 *             uint32_t BlockNum, DiskIoRequest_t IoRequest
 *
 * Return: DiskStatusCode_t
 *
 * Description:disk I/O
 *
 *************************************************************************/
DiskStatusCode_t DiskIO (uint8_t * pData,uint32_t BlockStart,
                           uint32_t BlockNum, DiskIoRequest_t IoRequest)
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
      _Write(pData,BlockStart*_DskCtrlBlk.BlockSize);
      BlockStart = BlockStart+_DskCtrlBlk.BlockSize;
    }
    break;
  case DiskRead:
    while(BlockNum--)
    {
      _Read(pData,BlockStart*_DskCtrlBlk.BlockSize);
      BlockStart = BlockStart+_DskCtrlBlk.BlockSize;
    }
    break;
  case DiskVerify:
    while(BlockNum--)
    {
      _Verify(pData,BlockStart*_DskCtrlBlk.BlockSize);
      BlockStart = BlockStart+_DskCtrlBlk.BlockSize;
    }
    break;
  default:
    return(DiskParametersError);
  }
  return(_DskCtrlBlk.DiskStatus);
}

