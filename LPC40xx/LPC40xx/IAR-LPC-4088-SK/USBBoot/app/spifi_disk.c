/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : spifi_disk.c
 *    Description : spifi disk driver
 *
 *    History :
 *    1. Date        : November, 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial revision
 *    $Revision: 28 $
 **************************************************************************/
#include <string.h>
#include "LPC407x_8x.h"
#include "disk.h"
#include "spifi_disk.h"
#include "fat.h"
#include "spifi_DiskImg.h"
#include "spifi_rom_api.h"

static void _Read(uint8_t * pData, uint32_t Add);
static void _Write(uint8_t * pData, uint32_t Add);
static void _Verify(uint8_t * pData, uint32_t Add);

static DiskCtrlBlk_t _DskCtrlBlk;

static __no_init uint8_t Spifi_Fat[SPIFI_FAT_DATA_SIZE];
static SPIFIobj Obj;

#define SPIFI_ADDR  0x28000000
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
  if (Add < SPIFI_RSVD_SECTORS*SPIFI_SECTOR_SIZE)
	{
    memcpy(pData,&Spifi_BootSect,SPIFI_DEF_BLK_SIZE);
  }
	else if (Add < (SPIFI_RSVD_SECTORS*SPIFI_SECTOR_SIZE + SPIFI_FAT_DATA_SIZE))
	     {

         Add += (uint32_t)&Spifi_Fat - SPIFI_RSVD_SECTORS*SPIFI_SECTOR_SIZE;
         memcpy(pData,(void const *)Add,SPIFI_DEF_BLK_SIZE);
		   }
       else if(Add < (SPIFI_FAT_SIZE))
            {
              Add += (uint32_t) &Spifi_DirEntry -(SPIFI_RSVD_SECTORS*SPIFI_SECTOR_SIZE + SPIFI_FAT_DATA_SIZE);
              memcpy(pData,(void const *)Add,SPIFI_DEF_BLK_SIZE);
            }
            else
            {
              Add += SPIFI_ADDR;
              Add -= (SPIFI_FAT_SIZE);
              memcpy(pData,(void const *)Add,SPIFI_DEF_BLK_SIZE);
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
  SPIFIopers opers;

  if (Add < SPIFI_RSVD_SECTORS*SPIFI_SECTOR_SIZE)
	{/*Can't write boot sector*/
  }
	else if (Add < (SPIFI_RSVD_SECTORS*SPIFI_SECTOR_SIZE + SPIFI_FAT_DATA_SIZE))
	     {/*Write Fat table*/
         Add += (uint32_t)&Spifi_Fat - SPIFI_RSVD_SECTORS*SPIFI_SECTOR_SIZE;
         memcpy((void *)Add,pData,SPIFI_DEF_BLK_SIZE);
		   }
       else if(Add < (SPIFI_FAT_SIZE))
            {/**/
              Add -= (SPIFI_RSVD_SECTORS*SPIFI_SECTOR_SIZE + SPIFI_FAT_DATA_SIZE);
              for(int i = 0; i<SPIFI_DEF_BLK_SIZE; i++,Add++,pData++)
              {
                if((0 ==(Add%32)) && (0xE5 == *pData))
                {/*Erase file*/
                  pdir_entry_t pfile = (pdir_entry_t)((uint32_t)Spifi_DirEntry+Add);
                  if(0xE5 != pfile->DIR_Name[0])
                  {/*File exist here!!! So we can erase it*/
                    
                    opers.dest =  (char *)((pfile->DIR_FstClusLO-2)* \
                                    SPIFI_SECT_PER_CLUSTER*SPIFI_SECTOR_SIZE);
                    opers.length = pfile->DIR_FileSize;
                    opers.scratch = NULL;
                    opers.protect = 0;
                    opers.options = 0;
                    spifi_erase(&Obj, &opers);
                  }
                }
              }
            }
            else
            {/*Write flash*/
              Add -= SPIFI_FAT_SIZE;
              opers.dest = (char *)Add;
              opers.length = SPIFI_DEF_BLK_SIZE;
              opers.scratch = NULL;
              opers.protect = 0;
              opers.options = 0;
              spifi_program(&Obj, (char *)pData, &opers);
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
void SpifiDiskInit (void)
{
uint32_t n,m,cluster,index;


  for(n=0;n<SPIFI_FAT_DATA_SIZE ;n++)
  {
    Spifi_Fat[n] = 0xFF;
  }

  Spifi_Fat[0]= 0xF8;
  Spifi_Fat[1]= 0xFF;
  Spifi_Fat[2]= 0xFF;
  /* Build Fat table*/
  for(int file = 1; file < SPIFI_ROOT_ENR_CNT; file++)
  {
    if(0xE5 != Spifi_DirEntry[file].DIR_Name[0])
    {
      n = Spifi_DirEntry[file].DIR_FstClusLO;
      m = n + (Spifi_DirEntry[file].DIR_FileSize-1)/SPIFI_SECTOR_SIZE/SPIFI_SECT_PER_CLUSTER;
      for(;n <= m;n++)
      {
        if(n == m) cluster = 0xFFF;
        else cluster = n+1;
        index = n*3;
        
        if(index&0x1)
        { 
          index >>= 1;
          Spifi_Fat[index] &= 0x0F;
          Spifi_Fat[index] |= (cluster<<4)&0xF0;
          Spifi_Fat[index+1] = (cluster>>4)&0xFF;
        }
        else
        {
          index >>= 1;
          Spifi_Fat[index] = (cluster)&0xFF;
          Spifi_Fat[index+1] &= 0xF0;
          Spifi_Fat[index+1] |= (cluster>>8)&0x0F;
        }
      }
    }
  }

  if(0 == spifi_init(&Obj, 7, S_RCVCLK | S_FULLCLK, SPIFIClock/1000000))
  {
    _DskCtrlBlk.BlockNumb = SPIFI_TOTAL_SECT;
    _DskCtrlBlk.BlockSize= SPIFI_SECTOR_SIZE;
    _DskCtrlBlk.DiskStatus=DiskCommandPass;
    _DskCtrlBlk.DiskType=DiskUnknow;
    _DskCtrlBlk.WriteProtect=0;
    _DskCtrlBlk.MediaChanged=0;
  }
  else
  {
    _DskCtrlBlk.BlockNumb = 0;
    _DskCtrlBlk.BlockSize= 0;
    _DskCtrlBlk.DiskStatus=DiskUknowError;
    _DskCtrlBlk.DiskType=DiskUnknow;
    _DskCtrlBlk.WriteProtect=0;
    _DskCtrlBlk.MediaChanged=0;
  }
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
pDiskCtrlBlk_t SpifiGetDiskCtrlBkl (void)
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
DiskStatusCode_t SpifiDiskIO (uint8_t * pData,uint32_t BlockStart,
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

